/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <qglobal.h>
#include <QInputContextFactory>
#include <QInputContext>

#include "duiinputmethodstate.h"
#include "duitheme.h"
#include "duicomponentdata.h"
#include "duiscenemanager.h"
#include "duideviceprofile.h"
#include "duilocale.h"
#include "duifeedbackplayer.h"
#include "duifeedbackplayer_p.h"
#include "duiapplicationifproxy.h"
#include "duiapplicationifadaptor.h"
#include "duiapplicationservice.h"
#include "duicomponentdata_p.h"
#include "duiwindow.h"
#include "duiapplicationwindow.h"
#include <DuiDebug>

#ifdef TESTABLE
/// for testability plugin
#include <QtPlugin>
#include <QPluginLoader>
#include <QLibraryInfo>
#include "testabilityinterface.h"
// end testability
#endif


namespace
{
    QStringList g_debug_prefixes;
    bool g_has_debug_whitelist(false);
    bool g_has_debug_blacklist(false);
    QtMsgType g_debug_level(QtCriticalMsg);
    bool g_loadDuiInputContext(true);
    FILE *debugingOutput;
}


bool duiRedirectOutput(const QString &filename)
{
    if (debugingOutput) {
        fclose(debugingOutput);
    }

    debugingOutput = fopen(filename.toUtf8(), "w");
    return debugingOutput != 0;
}

void duiMessageHandler(QtMsgType type, const char *msg)
{
    if (type < g_debug_level)
        return;

    if (type == QtDebugMsg || type == QtWarningMsg) {
        if (g_has_debug_whitelist) {
            QString msg_string(msg);
            bool passed(false);

            foreach(const QString & prefix, g_debug_prefixes) {
                if (msg_string.startsWith(prefix)) {
                    passed = true;
                    break;
                }
            }

            if (!passed)
                return;
        } else if (g_has_debug_blacklist) { // black list
            QString msg_string(msg);
            foreach(const QString & prefix, g_debug_prefixes) {
                if (msg_string.startsWith(prefix))
                    return;
            }
        }
    }

    FILE *out = debugingOutput;
    if (!out) {
        out = stderr;
    }
    fprintf(out, "%s\n", msg);
    if (type == QtFatalMsg)
        abort();
}

DuiComponentData *DuiComponentData::self = 0;

DuiComponentDataPrivate::DuiComponentDataPrivate()
    :
#ifdef QT_OPENGL_LIB
    softwareRendering(false),
#else
    softwareRendering(true),
#endif
    fullScreen(false),
    showBoundingRect(false),
    showSize(false),
    showPosition(false),
    showMargins(false),
    showObjectNames(false),
    showFps(false),
#ifdef __arm__
    showCursor(false),
#else
    showCursor(true),
#endif
    reverseLayout(false),
    prestarted(false),
    prestartMode(Dui::NoPrestart),
    theme(0),
    deviceProfile(0),
    feedbackPlayer(0),
    deviceName(0),
    service(0)

#ifdef TESTABLE
    ,
    testabilityInterface(0)
#endif
{

#ifdef TESTABLE
    // Activate testability plugin if exists
    QString testabilityPluginPostfix = ".so";
    QString testabilityPlugin = "testability/libtestability";

    testabilityPlugin = QLibraryInfo::location(QLibraryInfo::PluginsPath) + QDir::separator() + testabilityPlugin + testabilityPluginPostfix;
    QPluginLoader loader(testabilityPlugin.toLatin1().data());

    QObject *plugin = loader.instance();
    if (plugin) {
        duiDebug("DuiComponentData") << "Testability plugin loaded successfully!";
        testabilityInterface = qobject_cast<TestabilityInterface *>(plugin);

        if (testabilityInterface) {
            duiDebug("DuiComponentData") << "Testability interface obtained!";
            testabilityInterface->Initialize();
        } else {
            duiDebug("DuiComponentData") << "Failed to get testability interface!";
        }
    } else {
        duiDebug("DuiComponentData") << QString("Testability plugin %1 load failed with error: %2")
                                     .arg(testabilityPlugin).arg(loader.errorString());
    }
//end testability
#endif
}

DuiComponentDataPrivate::~DuiComponentDataPrivate()
{
#ifdef TESTABLE
    //remove the testability plugin if it exists
    //makes sure that all resources used by the plugin
    //are free when the application exits
    if (testabilityInterface) {
        delete testabilityInterface;
    }
#endif

    delete theme;
    theme = 0;

    delete feedbackPlayer;
    feedbackPlayer = 0;

    delete service;
    service = 0;

    delete deviceProfile;
    deviceProfile = 0;
}

void DuiComponentDataPrivate::debugInit(bool levelSet)
{
    //Command line variables override environment variables
    if (!g_has_debug_whitelist && !g_has_debug_blacklist) {
        // Check whether we got environment variables
        QByteArray list;
        list = qgetenv("DUI_OUTPUT_PREFIX");
        if (!list.isEmpty()) {
            g_has_debug_whitelist = true;
        }
        list = qgetenv("DUI_NO_OUTPUT_PREFIX");
        if (!list.isEmpty()) {
            if (g_has_debug_whitelist) {
                qCritical("Error: Please export either DUI_OUTPUT_PREFIX or DUI_NO_OUTPUT_PREFIX, not both");
                exit(EXIT_FAILURE);
            }
            g_has_debug_blacklist = true;
        }
        g_debug_prefixes = QString(list).split(':');
    }

    // check environment variable for debug level if no command line overrides it
    if (!levelSet) {
        QByteArray level = qgetenv("DUI_OUTPUT_LEVEL");

        if (!level.isEmpty()) {
            if (level == "debug") {
                g_debug_level = QtDebugMsg;
            } else if (level == "warning") {
                g_debug_level = QtWarningMsg;
            } else if (level == "critical") {
                g_debug_level = QtCriticalMsg;
            }
        }
    }
}

DuiComponentData *DuiComponentData::instance()
{
    return self;
}

static DuiComponentDataPrivate *gDuiComponentDataPrivate = 0;

DuiComponentData::DuiComponentData(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *service)
    : QObject(),
      d_ptr(new DuiComponentDataPrivate)
{
    Q_D(DuiComponentData);
    d->q_ptr = this;

    Q_ASSERT_X(!self, "DuiComponentData", "There should be one component data object only.");
    DuiComponentData::self = this;

    d->init(argc, argv, appIdentifier, service);
}

DuiComponentData::DuiComponentData(DuiApplicationService *service) :
    QObject(),
    d_ptr(new DuiComponentDataPrivate)
{
    Q_D(DuiComponentData);
    d->q_ptr = this;

    Q_ASSERT_X(!self, "DuiComponentData", "There should be one component data object only.");
    DuiComponentData::self = this;

    int argc = 0;
    char *argv = 0;

    d->init(argc, &argv, QString(), service);
}

void DuiComponentDataPrivate::init(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *newService)
{
    Q_Q(DuiComponentData);

    gDuiComponentDataPrivate = this;
    locale.connectSettings();
    QObject::connect(&locale, SIGNAL(settingsChanged()), q, SIGNAL(localeSettingsChanged()));

    // Remember if we were given the -reverse parameter
    // (we have to do it this way, because the QApplication
    // constructor "eats" the "-reverse" command line parameter.
    // Therefore, we can not parse it here.)
    reverseLayout = qApp->layoutDirection() == Qt::RightToLeft;

    DuiTheme::ThemeService themeService = DuiTheme::AnyTheme;
    bool outputLevelSet = false;

    // Configure application according to switches
    for (int i = 1; i < argc; ++i) {
        QString s(argv[i]);
        if (s == "-software")
            softwareRendering = true;
        else if (s == "-fullscreen")
            fullScreen = true;
        else if (s == "-show-br")
            showBoundingRect = true;
        else if (s == "-show-fps")
            showFps = true;
        else if (s == "-show-size")
            showSize = true;
        else if (s == "-show-position")
            showPosition = true;
        else if (s == "-show-cursor")
            showCursor = true;
        else if (s == "-show-object-names")
            q->setShowObjectNames(true);
        else if (s == "-dev") {
            showSize = true;
            showPosition = true;
        } else if (s == "-genimglist") {
            if (i < (argc - 1)) {
                imglistFilename = argv[i+1];
                i++;
            }
        } else if (s == "-remote-theme") {
            themeService = DuiTheme::RemoteTheme;
        } else if (s == "-output-level") {
            if (i < (argc - 1)) {
                i++;
                QString debugLevelString = argv[i];
                outputLevelSet = true;

                if (debugLevelString == "debug") {
                    g_debug_level = QtDebugMsg;
                } else if (debugLevelString == "warning") {
                    g_debug_level = QtWarningMsg;
                } else if (debugLevelString == "critical") {
                    g_debug_level = QtCriticalMsg;
                } else {
                    qCritical("%s: Error: Provide one of debug, warning or error to -output-level",
                              argv[0]);
                    exit(EXIT_FAILURE);
                }
            } else {
                qCritical("%s: Error: Provide one of debug, warning or error to -output-level",
                          argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (s == "-output-prefix" || s == "-no-output-prefix") {
            if (s == "-output-prefix")
                g_has_debug_whitelist = true;
            else
                g_has_debug_blacklist = true;

            if (g_has_debug_blacklist && g_has_debug_whitelist) {
                qCritical("%s: Error: Provide either -output-prefix or -no-output-prefix, not both",
                          argv[0]);
                exit(EXIT_FAILURE);
            }
            if (i < (argc - 1)) {
                i++;
                g_debug_prefixes << argv[i];
            } else {
                qCritical("%s: Error: Provide a prefix to filter for to %s",
                          argv[0], argv[i]);
                exit(EXIT_FAILURE);
            }
        } else if (s == "-output-file") {
            if (i < (argc - 1)) {
                i++;

                bool ok = duiRedirectOutput(argv[ i ]);

                if (! ok) {
                    qCritical("%s: Error: Opening of log file failed: %s",
                              argv[0], argv[i]);
                    exit(EXIT_FAILURE);
                }

            } else {
                qCritical("%s: Error: Please provide a log file name",
                          argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (s == "-disable-dui-input-context") {
            g_loadDuiInputContext = false;
        } else if (s == "-target") {
            if (i < (argc - 1)) {
                i++;
                deviceName = argv[i];

            } else {
                qCritical("%s: Error: Provide a device name to -target",
                          argv[0]);
                exit(EXIT_FAILURE);
            }

        } else if (s == "-h" || s.startsWith("-help") || s.startsWith("--help")) {
            duiDebug("DuiComponentData") << "Usage: " << argv[0] << "\n"
                                         << "  [-software] Enable software rendering\n"
                                         << "  [-fullscreen] Make the application fullscreen\n"
                                         << "  [-show-br] Show the bounding rectangle for all scene items\n"
                                         << "  [-show-fps] Show the FPS for the view (only with OpenGL rendering)\n"
                                         << "  [-show-size] Show widget sizes in the scene\n"
                                         << "  [-show-object-names] Show the names of the objects in the scene\n"
                                         << "  [-show-position] Show widget positions in the scene\n"
                                         << "  [-show-cursor] Force the cursor to be visible\n"
                                         << "  [-reverse] Change the layout direction to right-to-left direction\n"
                                         << "  [-dev] Enable development visualization mode\n"
                                         << "  [-genimglist filename] Generate list of requested images to filename\n"
                                         << "  [-remote-theme] Wait until remote theme daemon is available\n"
                                         << "  [-output-level debug|warning|critical] Only show messages of given output level or above\n"
                                         << "  [-output-prefix <prefix>] Only show debug messages that start with the given prefix\n"
                                         << "  [-no-output-prefix <prefix>] Only show debug messages that do not start with the given prefix\n"
                                         << "  [-target <name>] Use the target device profile\n"
                                         << "  [-prestart] Prestart the application (if supported)\n"
                                         << "\n";
            exit(0);

        } else if (s == "-prestart") {
            prestarted = true;
        }
    }

    //Setup duiDebug()
    debugInit(outputLevelSet);

    // If there was already a message handler, remove
    // own message handler again.
    QtMsgHandler handler(qInstallMsgHandler(duiMessageHandler));
    if (handler != 0) {
        qInstallMsgHandler(handler);
    }

    if (themeService == DuiTheme::RemoteTheme && !imglistFilename.isEmpty()) {
        qFatal("-genimglist switch can't be used with remote theme daemon");
    }

    QFileInfo fileInfo(argv[0]);
    QString themeIdentifier = fileInfo.fileName();
    if (!appIdentifier.isEmpty()) {
        QRegExp regExp("[0-9a-zA-Z_-]*");
        if (regExp.exactMatch(appIdentifier)) {
            themeIdentifier = appIdentifier;
        }
    }
    theme = new DuiTheme(themeIdentifier, imglistFilename, themeService);
    deviceProfile = new DuiDeviceProfile();

    QString catalog = appIdentifier;
    if (catalog.isEmpty())
        catalog = fileInfo.fileName();

    locale.addTranslationPath(TRANSLATION_DIR);
    // installs the libdui translations for the “Engineering English”:
    locale.installTrCatalog("libdui.qm");
    // installs the catalog for the “Engineering English”:
    locale.installTrCatalog(catalog + ".qm");
    // installs the libdui translations for the real translation:
    locale.installTrCatalog("libdui");
    // installs the real translations from the common translation catalog:
    locale.installTrCatalog("common");
    // installs the catalog for the real translation:
    locale.installTrCatalog(catalog);
    DuiLocale::setDefault(locale);

    // DuiLocale::setDefault(locale) also sets the
    // layoutDirection(). This overrides the effects of the -reverse
    // command line switch. Therefore, if the -reverse command line switch
    // was used, we have to set the RTL direction again here:
    if (reverseLayout)
        qApp->setLayoutDirection(Qt::RightToLeft);

    // set the input context
    if (g_loadDuiInputContext == true) {
        // QApplication::setInputContext takes ownership of this input context,
        // but it does not call setParent( qApp ); on the input context yet.
        // a bug for this is filed to qt software.
        QInputContext *ic = QInputContextFactory::create("DuiInputContext", 0);

        if (ic != 0) {
            qApp->setInputContext(ic);
        }
    }

    feedbackPlayer = new DuiFeedbackPlayer();
    if (!feedbackPlayer->d_ptr->init(themeIdentifier)) {
        delete feedbackPlayer;
        feedbackPlayer = 0;
    }

    // register dbus service
    appName = themeIdentifier;
    binaryName = argv[0];

    //appName cannot begin with number
    if (appName[0].isDigit()) {
        qFatal("DuiComponentData - application identifier must not begin with a digit.");
    }

#ifdef HAVE_DBUS
    // here we check for a working dbus session bus and give a big
    // fat error message when it does not exist.

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (connection.isConnected() == false) {
        qFatal("ERROR: No DBUS session bus found. Exiting now. Please make sure that a dbus session bus\n"
               "is running. In Scratchbox you should execute dui-sb-session start. On the target device\n"
               "it should already be running. You should also make sure that the DBUS_SESSION_BUS_ADDRESS\n"
               "environment variable is set correctly. For that you can execute the following command:\n"
               "source /tmp/session_bus_address.user\n");

        qApp->exit(-1);
    }


    if (newService == 0) {
        QString serviceName = "com.nokia." + appName;
        duiDebug("DuiComponentData") << "creating DuiComponentDataService with name:" << serviceName;
        service = new DuiApplicationService(serviceName);
    } else {
        service = newService;
    }

    service->registerService();
#else
    Q_UNUSED(newService);
#endif

    q->setShowCursor(showCursor);
}

DuiComponentData::~DuiComponentData()
{
    delete d_ptr;
    gDuiComponentDataPrivate = 0;
    self = 0;
}

QString DuiComponentData::deviceName()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::deviceName() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->deviceName;
}

bool DuiComponentData::softwareRendering()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::softwareRendering() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->softwareRendering;
}

bool DuiComponentData::fullScreen()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::fullScreen() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->fullScreen;
}

bool DuiComponentData::prestarted()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::prestarted() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->prestarted;
}

Dui::PrestartMode DuiComponentData::prestartMode()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::prestartMode() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->prestartMode;
}

bool DuiComponentData::showBoundingRect()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::showBoundingRect() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->showBoundingRect;
}

bool DuiComponentData::showFps()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::showFps() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->showFps;
}

bool DuiComponentData::showSize()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::showSizes() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->showSize;
}

bool DuiComponentData::showPosition()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::showPositions() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->showPosition;
}

bool DuiComponentData::showMargins()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::showMargins() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->showMargins;
}

bool DuiComponentData::showObjectNames()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::showObject() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->showObjectNames;
}

bool DuiComponentData::showCursor()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::showCursor() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->showCursor;
}

void DuiComponentData::setShowPosition(bool show)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setPositions() - DuiComponentData instance not yet created.");
    }
    gDuiComponentDataPrivate->showPosition = show;
}

void DuiComponentData::setShowSize(bool show)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setPositions() - DuiComponentData instance not yet created.");
    }
    gDuiComponentDataPrivate->showSize = show;
}

void DuiComponentData::setShowMargins(bool show)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setMargins() - DuiComponentData instance not yet created.");
    }
    gDuiComponentDataPrivate->showMargins = show;
}

void DuiComponentData::setShowObjectNames(bool show)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setShowObjectNames() - DuiComponentData instance not yet created.");
    }
    gDuiComponentDataPrivate->showObjectNames = show;
}

void DuiComponentData::setShowBoundingRect(bool show)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setShowBoundingRect() - DuiComponentData instance not yet created.");
    }
    gDuiComponentDataPrivate->showBoundingRect = show;
}

void DuiComponentData::setShowFps(bool show)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setShowFps() - DuiComponentData instance not yet created.");
    }
    gDuiComponentDataPrivate->showFps = show;
}

void DuiComponentData::setShowCursor(bool show)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setShowCursor() - DuiComponentData instance not yet created.");
    }

    if (show) {
        qApp->restoreOverrideCursor();
    } else {
        QPixmap cursor(QSize(1, 1));
        cursor.fill(Qt::transparent);
        qApp->setOverrideCursor(cursor);
    }

    gDuiComponentDataPrivate->showCursor = show;
}


void DuiComponentData::setPrestarted(bool flag)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setPrestarted() - DuiComponentData instance not yet created.");
    }
    gDuiComponentDataPrivate->prestarted = flag;
}

void DuiComponentData::setPrestartMode(Dui::PrestartMode mode)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setPrestartMode() - DuiComponentData instance not yet created.");
    }
    gDuiComponentDataPrivate->prestartMode = mode;
}

QList<DuiWindow *> DuiComponentData::windows()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::windows() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->windows;
}

DuiWindow *DuiComponentData::activeWindow()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::activeWindow() - DuiComponentData instance not yet created.");
    }

    if (gDuiComponentDataPrivate->windows.isEmpty())
        return 0;

    return gDuiComponentDataPrivate->windows.first();
}

DuiApplicationWindow *DuiComponentData::activeApplicationWindow()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::activeApplicationWindow() - DuiComponentData instance not yet created.");
    }

    return qobject_cast<DuiApplicationWindow *>(activeWindow());
}

void DuiComponentData::setActiveWindow(DuiWindow *w)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::setActiveWindow() - DuiComponentData instance not yet created.");
    }
    DuiWindow *activeWindow = DuiComponentData::activeWindow();
    if (activeWindow) {
        QObject::disconnect(activeWindow,
                            SIGNAL(orientationAngleChanged(Dui::OrientationAngle)),
                            DuiInputMethodState::instance(),
                            SLOT(setActiveWindowOrientationAngle(Dui::OrientationAngle)));
    }
    if (w) {
        QObject::connect(w, SIGNAL(orientationAngleChanged(Dui::OrientationAngle)),
                         DuiInputMethodState::instance(),
                         SLOT(setActiveWindowOrientationAngle(Dui::OrientationAngle)));
        DuiInputMethodState::instance()->setActiveWindowOrientationAngle(w->orientationAngle());
    }
    int oldIndex = gDuiComponentDataPrivate->windows.indexOf(w);

    if (oldIndex == -1)
        duiWarning("DuiComponentData::setActiveWindow()") << "attempting to activate unregistered window";
    else
        gDuiComponentDataPrivate->windows.move(oldIndex, 0);
}

void DuiComponentData::registerWindow(DuiWindow *w)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::registerWindow() - DuiComponentData instance not yet created.");
    }
    if (!gDuiComponentDataPrivate->windows.contains(w))
        gDuiComponentDataPrivate->windows.append(w);
}

void DuiComponentData::unregisterWindow(DuiWindow *w)
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::unregisterWindow() - DuiComponentData instance not yet created.");
    }
    gDuiComponentDataPrivate->windows.removeAll(w);
}

DuiFeedbackPlayer *DuiComponentData::feedbackPlayer()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::feedbackPlayer() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->feedbackPlayer;
}


bool DuiComponentData::isLoadDuiInputContextEnabled()
{
    return g_loadDuiInputContext;
}


void DuiComponentData::setLoadDuiInputContext(bool enable)
{
    g_loadDuiInputContext = enable;
}

QString DuiComponentData::appName()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::appName() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->appName;
}

QString DuiComponentData::binaryName()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::binaryName() - DuiComponentData instance not yet created.");
    }
    return gDuiComponentDataPrivate->binaryName;
}

QString DuiComponentData::serviceName()
{
    if (!gDuiComponentDataPrivate) {
        qFatal("DuiComponentData::serviceName() - DuiComponentData instance not yet created.");
    } else if (!gDuiComponentDataPrivate->service) {
        qFatal("DuiComponentData::serviceName() - DuiComponentData->service not yet created.");
    }

    return gDuiComponentDataPrivate->service->registeredName();
}
