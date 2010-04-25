/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
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

#include "minputmethodstate.h"
#include "mtheme.h"
#include "mcomponentdata.h"
#include "mscenemanager.h"
#include "mdeviceprofile.h"
#include "mlocale.h"
#include "mfeedbackplayer.h"
#include "mfeedbackplayer_p.h"
#include "mapplicationifproxy.h"
#include "mapplicationifadaptor.h"
#include "mapplicationservice.h"
#include "mcomponentdata_p.h"
#include "mwindow.h"
#include "mapplicationwindow.h"
#include "mtapandholdrecognizer.h"
#include "mpanrecognizer.h"
#include <MDebug>

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
#ifdef __arm__
    QtMsgType g_debug_level(QtCriticalMsg);
#else
    QtMsgType g_debug_level(QtWarningMsg);
#endif
    bool g_loadMInputContext(true);
    FILE *debugingOutput;
}


bool mRedirectOutput(const QString &filename)
{
    if (debugingOutput) {
        fclose(debugingOutput);
    }

    debugingOutput = fopen(filename.toUtf8(), "w");
    return debugingOutput != 0;
}

void mMessageHandler(QtMsgType type, const char *msg)
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

MComponentData *MComponentData::self = 0;

MComponentDataPrivate::MComponentDataPrivate()
    :
// for some reason in windows 7 only software rendering works
#if defined(QT_OPENGL_LIB) && !defined(Q_OS_WIN)
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
    logFps(false),
#ifdef __arm__
    showCursor(false),
#else
    showCursor(true),
#endif
    reverseLayout(false),
    prestarted(false),
    emulateTwoFingerGestures(false),
    prestartMode(M::NoPrestart),
    theme(0),
    deviceProfile(0),
    windows(),
    feedbackPlayer(0),
    locale(),
    imglistFilename(),
    appName(),
    binaryName(),
    deviceName(),
    service(0)
#ifdef TESTABLE
    ,
    testabilityInterface(0)
#endif
    ,
    q_ptr(NULL)
{

#ifdef TESTABLE
    // Activate testability plugin if exists
    QString testabilityPluginPostfix = ".so";
    QString testabilityPlugin = "testability/libtestability";

    testabilityPlugin = QLibraryInfo::location(QLibraryInfo::PluginsPath) + QDir::separator() + testabilityPlugin + testabilityPluginPostfix;
    QPluginLoader loader(testabilityPlugin.toLatin1().data());

    QObject *plugin = loader.instance();
    if (plugin) {
        mDebug("MComponentData") << "Testability plugin loaded successfully!";
        testabilityInterface = qobject_cast<TestabilityInterface *>(plugin);

        if (testabilityInterface) {
            mDebug("MComponentData") << "Testability interface obtained!";
            testabilityInterface->Initialize();
        } else {
            mDebug("MComponentData") << "Failed to get testability interface!";
        }
    } else {
        mDebug("MComponentData") << QString("Testability plugin %1 load failed with error: %2")
                                     .arg(testabilityPlugin).arg(loader.errorString());
    }
//end testability
#endif
}

MComponentDataPrivate::~MComponentDataPrivate()
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

void MComponentDataPrivate::debugInit(bool levelSet)
{
    //Command line variables override environment variables
    if (!g_has_debug_whitelist && !g_has_debug_blacklist) {
        // Check whether we got environment variables
        QByteArray list;
        list = qgetenv("M_OUTPUT_PREFIX");
        if (!list.isEmpty()) {
            g_has_debug_whitelist = true;
        }
        list = qgetenv("M_NO_OUTPUT_PREFIX");
        if (!list.isEmpty()) {
            if (g_has_debug_whitelist) {
                qCritical("Error: Please export either M_OUTPUT_PREFIX or M_NO_OUTPUT_PREFIX, not both");
                exit(EXIT_FAILURE);
            }
            g_has_debug_blacklist = true;
        }
        g_debug_prefixes = QString(list).split(':');
    }

    // check environment variable for debug level if no command line overrides it
    if (!levelSet) {
        QByteArray level = qgetenv("M_OUTPUT_LEVEL");

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

MComponentData *MComponentData::instance()
{
    return self;
}

static MComponentDataPrivate *gMComponentDataPrivate = 0;

MComponentData::MComponentData(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
    : QObject(),
      d_ptr(new MComponentDataPrivate)
{
    Q_D(MComponentData);
    d->q_ptr = this;

    Q_ASSERT_X(!self, "MComponentData", "There should be one component data object only.");
    MComponentData::self = this;

    d->init(argc, argv, appIdentifier, service);
}

MComponentData::MComponentData(MApplicationService *service) :
    QObject(),
    d_ptr(new MComponentDataPrivate)
{
    Q_D(MComponentData);
    d->q_ptr = this;

    Q_ASSERT_X(!self, "MComponentData", "There should be one component data object only.");
    MComponentData::self = this;

    int argc = 0;
    char *argv = 0;

    d->init(argc, &argv, QString(), service);
}

void MComponentDataPrivate::init(int &argc, char **argv, const QString &appIdentifier, MApplicationService *newService)
{
    Q_Q(MComponentData);

    gMComponentDataPrivate = this;

    // Remember if we were given the -reverse parameter
    // (we have to do it this way, because the QApplication
    // constructor "eats" the "-reverse" command line parameter.
    // Therefore, we can not parse it here.)
    reverseLayout = qApp->layoutDirection() == Qt::RightToLeft;

    MTheme::ThemeService themeService = MTheme::AnyTheme;
    bool outputLevelSet = false;

#ifdef HAVE_N900
    //#MS - default commadline: /usr/bin/widgetsgallery -target N900 -fullscreen
    // -graphicssystem native  this we have to set in themedaemon or before MApplication()
    fullScreen = true;
    deviceName = "N900";
#endif //HAVE_N900

    // Assume every argument is used and mark those that are not
    QVector<bool> usedArguments(argc, true);

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
        else if (s == "-log-fps")
            logFps = true;
        else if (s == "-show-size")
            showSize = true;
        else if (s == "-show-position")
            showPosition = true;
        else if (s == "-show-cursor")
            showCursor = true;
        else if (s == "-show-object-names")
            q->setShowObjectNames(true);
        else if (s == "-emulate-two-finger-gestures")
            q->setEmulateTwoFingerGestures(true);
        else if (s == "-dev") {
            showSize = true;
            showPosition = true;
        } else if (s == "-genimglist") {
            if (i < (argc - 1)) {
                imglistFilename = argv[i+1];
                i++;
            }
        } else if (s == "-remote-theme") {
            themeService = MTheme::RemoteTheme;
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

                bool ok = mRedirectOutput(argv[ i ]);

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
        } else if (s == "-disable-m-input-context") {
            g_loadMInputContext = false;
        } else if (s == "-target") {
            if (i < (argc - 1)) {
                i++;
                deviceName = argv[i];

            } else {
                qCritical("%s: Error: Provide a device name to -target",
                          argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (s == "-v" || s.startsWith("-version") || s.startsWith("--version")) {
            mDebug("MComponentData") << "Version info: " <<  "\n"
                                         << "M_VERSION :" << M_VERSION << "\n" 
                                         << "Compile time:" <<  __DATE__  << __TIME__ << "\n"
                                         << "QT verison  :" << QT_VERSION_STR << "\n";
            exit(EXIT_SUCCESS);
        } else if (s == "-h" || s.startsWith("-help") || s.startsWith("--help")) {
            mDebug("MComponentData") << "Usage: " << argv[0] << "\n"
                                         << "  [-software] Enable software rendering\n"
                                         << "  [-fullscreen] Make the application fullscreen\n"
                                         << "  [-show-br] Show the bounding rectangle for all scene items\n"
                                         << "  [-show-fps] Show the FPS for the view (only with OpenGL rendering)\n"
                                         << "  [-log-fps] Log the FPS for the application\n"
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
                                         << "  [-emulate-two-finger-gestures] Emulate pinch gesture (alt + mid mouse button + mouse movement)\n"
                                         << "                                 and two finger pan gesture (ctrl + mid mouse button + mouse movement)\n"
                                         << "\n";
            exit(0);

        } else if (s == "-prestart") {
            prestarted = true;
        } else {
            usedArguments[i] = false;
        }
    }

    // remove used arguments from argc/argv by overwriting the used ones
    unsigned int currentTgt = 1;

    for (int currentSrc = 1; currentSrc < usedArguments.size(); ++currentSrc) {
        argv[currentTgt] = argv[currentSrc];

        if (!usedArguments[currentSrc]) {
            ++currentTgt;
        } else {
            --argc;
        }
    }

    // and cleaning up the rest, just in case
    for (int i = currentTgt+1; i < usedArguments.size(); ++i) {
        argv[i] = 0;
    }

    //Setup mDebug()
    debugInit(outputLevelSet);

    // If there was already a message handler, remove
    // own message handler again.
    QtMsgHandler handler(qInstallMsgHandler(mMessageHandler));
    if (handler != 0) {
        qInstallMsgHandler(handler);
    }

    if (themeService == MTheme::RemoteTheme && !imglistFilename.isEmpty()) {
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
    theme = new MTheme(themeIdentifier, imglistFilename, themeService);
    deviceProfile = new MDeviceProfile();

    QString catalog = appIdentifier;
    if (catalog.isEmpty())
        catalog = fileInfo.fileName();

    MLocale systemLocale; // gets the current system locale, creating it if necessary.
#ifdef Q_OS_WIN
    // walk to translation dir relative to bin dir
    QDir appDir(QCoreApplication::applicationDirPath());

    appDir.cdUp();
    appDir.cd("share");
    appDir.cd("l10n");
    appDir.cd("meegotouch");

    systemLocale.addTranslationPath(appDir.absolutePath());
#else
    systemLocale.addTranslationPath(TRANSLATION_DIR);
#endif
    // installs the libmeegotouch translations
    systemLocale.installTrCatalog("libmeegotouch");
    // installs the common translation catalog:
    systemLocale.installTrCatalog("common");
    // installs the translationi catalog of the application:
    systemLocale.installTrCatalog(catalog);
    MLocale::setDefault(systemLocale);

    // MLocale::setDefault(locale) also sets the
    // layoutDirection(). This overrides the effects of the -reverse
    // command line switch. Therefore, if the -reverse command line switch
    // was used, we have to set the RTL direction again here:
    if (reverseLayout)
        qApp->setLayoutDirection(Qt::RightToLeft);

    // set the input context
    if (g_loadMInputContext == true) {
        // QApplication::setInputContext takes ownership of this input context,
        // but it does not call setParent( qApp ); on the input context yet.
        // a bug for this is filed to qt software.
        QInputContext *ic = QInputContextFactory::create("MInputContext", 0);

        if (ic != 0) {
            qApp->setInputContext(ic);
        }
    }

    feedbackPlayer = new MFeedbackPlayer();
    if (!feedbackPlayer->d_ptr->init(themeIdentifier)) {
        delete feedbackPlayer;
        feedbackPlayer = 0;
    }

    // register dbus service
    appName = themeIdentifier;
    binaryName = argv[0];

    //appName cannot begin with number
    if (appName[0].isDigit()) {
        qFatal("MComponentData - application identifier must not begin with a digit.");
    }

#ifdef HAVE_DBUS
    // here we check for a working dbus session bus and give a big
    // fat error message when it does not exist.

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (connection.isConnected() == false) {
        qFatal("ERROR: No DBUS session bus found. Exiting now. Please make sure that a dbus session bus\n"
               "is running. In Scratchbox you should execute m-sb-session start. On the target device\n"
               "it should already be running. You should also make sure that the DBUS_SESSION_BUS_ADDRESS\n"
               "environment variable is set correctly. For that you can execute the following command:\n"
               "source /tmp/session_bus_address.user\n");

        qApp->exit(-1);
    }


    if (newService == 0) {
        QString serviceName = "com.nokia." + appName;
        mDebug("MComponentData") << "creating MComponentDataService with name:" << serviceName;
        service = new MApplicationService(serviceName);
    } else {
        service = newService;
    }

    service->registerService();
#else
    Q_UNUSED(newService);
#endif

    QGestureRecognizer::unregisterRecognizer(Qt::TapAndHoldGesture);
    QGestureRecognizer::registerRecognizer(new MTapAndHoldRecognizer());

    QGestureRecognizer::unregisterRecognizer(Qt::PanGesture);
    QGestureRecognizer::registerRecognizer(new MPanRecognizer());

    q->setShowCursor(showCursor);
}

MComponentData::~MComponentData()
{
    delete d_ptr;
    gMComponentDataPrivate = 0;
    self = 0;
}

QString MComponentData::deviceName()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::deviceName() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->deviceName;
}

bool MComponentData::softwareRendering()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::softwareRendering() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->softwareRendering;
}

bool MComponentData::fullScreen()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::fullScreen() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->fullScreen;
}

bool MComponentData::prestarted()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::prestarted() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->prestarted;
}

M::PrestartMode MComponentData::prestartMode()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::prestartMode() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->prestartMode;
}

bool MComponentData::showBoundingRect()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::showBoundingRect() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->showBoundingRect;
}

bool MComponentData::showFps()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::showFps() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->showFps;
}

bool MComponentData::logFps()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::logFps() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->logFps;
}

bool MComponentData::showSize()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::showSizes() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->showSize;
}

bool MComponentData::showPosition()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::showPositions() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->showPosition;
}

bool MComponentData::showMargins()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::showMargins() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->showMargins;
}

bool MComponentData::showObjectNames()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::showObject() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->showObjectNames;
}

bool MComponentData::showCursor()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::showCursor() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->showCursor;
}

bool MComponentData::emulateTwoFingerGestures()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::emulateTwoFingerGestures() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->emulateTwoFingerGestures;
}

void MComponentData::setShowPosition(bool show)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setPositions() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->showPosition = show;
}

void MComponentData::setShowSize(bool show)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setPositions() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->showSize = show;
}

void MComponentData::setShowMargins(bool show)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setMargins() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->showMargins = show;
}

void MComponentData::setShowObjectNames(bool show)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setShowObjectNames() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->showObjectNames = show;
}

void MComponentData::setShowBoundingRect(bool show)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setShowBoundingRect() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->showBoundingRect = show;
}

void MComponentData::setShowFps(bool show)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setShowFps() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->showFps = show;
}

void MComponentData::setLogFps(bool show)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setLogFps() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->logFps = show;
}

void MComponentData::setShowCursor(bool show)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setShowCursor() - MComponentData instance not yet created.");
    }

    if (show) {
        qApp->restoreOverrideCursor();
    } else {
        QPixmap cursor(QSize(1, 1));
        cursor.fill(Qt::transparent);
        qApp->setOverrideCursor(cursor);
    }

    gMComponentDataPrivate->showCursor = show;
}


void MComponentData::setPrestarted(bool flag)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setPrestarted() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->prestarted = flag;
}

void MComponentData::setPrestartMode(M::PrestartMode mode)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setPrestartMode() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->prestartMode = mode;
}

void MComponentData::setEmulateTwoFingerGestures(bool flag)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setEmulateTwoFingerGestures() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->emulateTwoFingerGestures = flag;
}

QList<MWindow *> MComponentData::windows()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::windows() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->windows;
}

MWindow *MComponentData::activeWindow()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::activeWindow() - MComponentData instance not yet created.");
    }

    if (gMComponentDataPrivate->windows.isEmpty())
        return 0;

    return gMComponentDataPrivate->windows.at(0);
}

MApplicationWindow *MComponentData::activeApplicationWindow()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::activeApplicationWindow() - MComponentData instance not yet created.");
    }

    return qobject_cast<MApplicationWindow *>(activeWindow());
}

void MComponentData::setActiveWindow(MWindow *w)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setActiveWindow() - MComponentData instance not yet created.");
    }

    MWindow *activeWindow = MComponentData::activeWindow();

    if (activeWindow) {
        QObject::disconnect(activeWindow,
                            SIGNAL(orientationAngleChanged(M::OrientationAngle)),
                            MInputMethodState::instance(),
                            SLOT(setActiveWindowOrientationAngle(M::OrientationAngle)));
    }
    if (w) {
        QObject::connect(w, SIGNAL(orientationAngleChanged(M::OrientationAngle)),
                         MInputMethodState::instance(),
                         SLOT(setActiveWindowOrientationAngle(M::OrientationAngle)));
        MInputMethodState::instance()->setActiveWindowOrientationAngle(w->orientationAngle());
    }
    int oldIndex = gMComponentDataPrivate->windows.indexOf(w);

    if (oldIndex == -1)
        mWarning("MComponentData::setActiveWindow()") << "attempting to activate unregistered window";
    else
        gMComponentDataPrivate->windows.move(oldIndex, 0);
}

void MComponentData::registerWindow(MWindow *w)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::registerWindow() - MComponentData instance not yet created.");
    }
    if (!gMComponentDataPrivate->windows.contains(w))
        gMComponentDataPrivate->windows.append(w);
}

void MComponentData::unregisterWindow(MWindow *w)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::unregisterWindow() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->windows.removeAll(w);
}

MFeedbackPlayer *MComponentData::feedbackPlayer()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::feedbackPlayer() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->feedbackPlayer;
}


bool MComponentData::isLoadMInputContextEnabled()
{
    return g_loadMInputContext;
}


void MComponentData::setLoadMInputContext(bool enable)
{
    g_loadMInputContext = enable;
}

QString MComponentData::appName()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::appName() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->appName;
}

QString MComponentData::binaryName()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::binaryName() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->binaryName;
}

QString MComponentData::serviceName()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::serviceName() - MComponentData instance not yet created.");
    } else if (!gMComponentDataPrivate->service) {
        qFatal("MComponentData::serviceName() - MComponentData->service not yet created.");
    }

    return gMComponentDataPrivate->service->registeredName();
}
