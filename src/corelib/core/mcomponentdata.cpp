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
#include <QBitArray>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <qglobal.h>
#include <QInputContextFactory>
#include <QInputContext>

#include "minputmethodstate.h"
#include "mtheme.h"
#include "mtheme_p.h"
#include "mthemedaemon.h"
#include "mcomponentdata.h"
#include "mscenemanager.h"
#include "mdeviceprofile.h"
#include "mlocale.h"
#include "mfeedbackplayer.h"
#include "mfeedbackplayer_p.h"
#ifdef HAVE_DBUS
#include "mapplicationifproxy.h"
#include "mapplicationifadaptor.h"
#include "mapplicationservice.h"
#endif //HAVE_DBUS

#ifndef Q_OS_WIN
#include "mcomponentcache.h"
#endif

#include "mcomponentdata_p.h"
#include "mwindow.h"
#include "mapplicationwindow.h"
#include "mtapandholdrecognizer.h"
#include "mpanrecognizer.h"
#include "mswiperecognizer.h"
#include "msyslogclient.h"
#include <MDebug>
#include <QtPlugin>
#include <QPluginLoader>
#include <QLibraryInfo>
#include "testabilityinterface.h"
#include "mgraphicssystemhelper.h"

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xatom.h>
#ifdef HAVE_XDAMAGE
#include <X11/extensions/Xfixes.h>
#endif // HAVE_XFIXES
#endif // Q_WS_X11

namespace
{
    QStringList g_debug_prefixes;
    bool g_has_debug_whitelist(false);
    bool g_has_debug_blacklist(false);
    bool g_show_performance_messages(false);
#ifdef __arm__
    QtMsgType g_debug_level(QtCriticalMsg);
#else
    QtMsgType g_debug_level(QtWarningMsg);
#endif
    bool g_loadMInputContext(false);
    FILE *debugingOutput;
    MSyslogClientSocket * g_syslogSocket = 0;
}

bool mInitSyslogConnection(const QUrl &url)
{
    if (g_syslogSocket) {
        g_syslogSocket->close();
        delete g_syslogSocket;
        g_syslogSocket = 0;
    }

    if(url.isEmpty())
        return true;

    MSyslogClientSocket *socket = new MSyslogClientSocket();

    if (socket->connectToServer(url)) {
        g_syslogSocket = socket;
        return true;
    } else {
        delete socket;
        return false;
    }
}

bool mRedirectOutput(const QString &filename)
{
    if (debugingOutput) {
        fclose(debugingOutput);
    }

    debugingOutput = fopen(filename.toUtf8(), "w");
    return debugingOutput != 0;
}

void m_output_message(const char* msg)
{
    FILE *out = debugingOutput;
    if (!out) {
        out = stderr;
    }
    fprintf(out, "%s\n", msg);
}

void mMessageHandler(int type, const char *msg)
{
    switch (type) {
    case MDebug::Performance:
        if (g_show_performance_messages)
            m_output_message(msg);
        break;
    default:
        break;
    };
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

    m_output_message(msg);

    if (g_syslogSocket) {
        g_syslogSocket->sendMsg(type, msg);
    }

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
    showStyleNames(false),
    showFps(false),
    logFps(false),
#ifdef __arm__
    showCursor(false),
#else
    showCursor(true),
#endif
    reverseLayout(false),
    prestarted(false),
    emulateTwoFingerGestures(true),
    prestartMode(M::NoPrestart),
    forcedOrientationAngle(M::Angle0),
    isOrientationForced(false),
    theme(0),
    deviceProfile(0),
    windows(),
    firstWindow(0),
    feedbackPlayer(0),
    locale(),
    appName(),
    binaryName(),
    deviceName(),
    syslogServer(),
    service(0),
    testabilityInterface(0),
#ifdef HAVE_GCONF
    debugOptions ("/meegotouch/debug/info_display"),
#endif
    q_ptr(NULL)
{
}

MComponentDataPrivate::~MComponentDataPrivate()
{
    //remove the testability plugin if it exists
    //makes sure that all resources used by the plugin
    //are free when the application exits
    if (testabilityInterface) {
        delete testabilityInterface;
    }

    delete theme;
    theme = 0;

    delete feedbackPlayer;
    feedbackPlayer = 0;

#ifdef HAVE_DBUS
    delete service;
    service = 0;
#endif

    delete deviceProfile;
    deviceProfile = 0;
}

void MComponentDataPrivate::testabilityInit()
{
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

    // If there was already a message handler, remove
    // own message handler again. If our message handler
    // was installed succesfully, initiate the connection to
    // syslog, if syslogServer is not the empty string.
    QtMsgHandler handler(qInstallMsgHandler(mMessageHandler));
    if (handler != 0) {
        qInstallMsgHandler(handler);
    } else if (!syslogServer.isEmpty()) {
        initSyslogConnection(syslogServer);
    }
    MDebug::installMessageHandler(mMessageHandler);
}

MComponentData *MComponentData::instance()
{
    return self;
}

MComponentData* MComponentData::createInstance(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
{
    if(!self) {
        self = new MComponentData(argc, argv, appIdentifier, service);
    }
    return self;
}

MComponentData* MComponentData::createInstance(MApplicationService *service)
{
    if(!self) {
        self = new MComponentData(service);
    }
    return self;
}


static MComponentDataPrivate *gMComponentDataPrivate = 0;

MComponentData::MComponentData(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
    : QObject(),
      d_ptr(new MComponentDataPrivate)
{
    Q_D(MComponentData);
    d->q_ptr = this;

    if(self)
        qWarning() << "MComponentData instantiated although there is already an instance!\n Maybe you called the MComponentData Contructor directly, what is deprecated. Use MComponentData::createInstance() instead.";

    self = this;

    d->init(argc, argv, appIdentifier, service);
}

MComponentData::MComponentData(MApplicationService *service) :
    QObject(),
    d_ptr(new MComponentDataPrivate)
{
    Q_D(MComponentData);
    d->q_ptr = this;

    if(self)
        qWarning() << "MComponentData instantiated although there is already an instance!\n Maybe you called the MComponentData Contructor directly, what is deprecated. Use MComponentData::createInstance() instead.";

    self = this;

    int argc = 0;
    char *argv[] = {0};

    d->init(argc, argv, QString(), service);
}

bool MComponentDataPrivate::initSyslogConnection(const QUrl &url)
{
    if (!mInitSyslogConnection(url)) {
        mWarning("MComponentDataPrivate::initSyslogConnection") <<
            "unable to establish connection to given syslog server url.";
        return false;
    }

    return true;
}

void MComponentDataPrivate::init(int &argc, char **argv, const QString &appIdentifier, MApplicationService *newService)
{
    Q_Q(MComponentData);

    gMComponentDataPrivate = this;

    QFileInfo fileInfo(argv[0]);
    QString themeIdentifier = fileInfo.fileName();
    if (!appIdentifier.isEmpty()) {
        QRegExp regExp("[0-9a-zA-Z_-]*");
        if (regExp.exactMatch(appIdentifier)) {
            themeIdentifier = appIdentifier;
        }
    }

    // register dbus service
    appName = themeIdentifier;
    binaryName = argv[0];

    //appName cannot begin with number
    if (appName[0].isDigit()) {
        qCritical("MComponentData - application identifier must not begin with a digit.");
        exit(EXIT_FAILURE);
    }

#ifdef HAVE_DBUS
    // here we check for a working dbus session bus and give a big
    // fat error message when it does not exist.

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        qCritical("ERROR: No DBUS session bus found. Exiting now. Please make sure that a dbus session bus\n"
               "is running. In Scratchbox you should execute meego-sb-session start. On the target device\n"
               "it should already be running. You should also make sure that the DBUS_SESSION_BUS_ADDRESS\n"
               "environment variable is set correctly. For that you can execute the following command:\n"
               "source /tmp/session_bus_address.user\n");

        exit(EXIT_FAILURE);
    }

    /* If cache is being populated, real name of the application to be
       executed is not known yet. Service registration will be skipped
       now and done in reinit(). */
#ifndef Q_OS_WIN
    if (!MComponentCache::populating())
#endif
    {
        if (newService == 0) {
            registerDefaultService(appName);
        } else {
            registerNewService(newService);
        }
    }
#else
    Q_UNUSED(newService);
#endif

    // Remember if we were given the -reverse parameter
    // (we have to do it this way, because the QApplication
    // constructor "eats" the "-reverse" command line parameter.
    // Therefore, we can not parse it here.)
    reverseLayout = qApp->layoutDirection() == Qt::RightToLeft;

#ifdef HAVE_GCONF
    QObject::connect(&debugOptions, SIGNAL(valueChanged()),
                     q_ptr, SLOT(_q_updateDebugOptionsFromGConfValues()));
#endif

#ifdef __arm__
    MTheme::ThemeService themeService = MTheme::RemoteTheme;
#elif defined Q_OS_WIN
    MTheme::ThemeService themeService = MTheme::LocalTheme;
#else
    MTheme::ThemeService themeService = MTheme::AnyTheme;
#endif

#ifdef M_OS_MAEMO5
    //#MS - default commandline: /usr/bin/widgetsgallery -target N900 -fullscreen
    // -graphicssystem native  this we have to set in themedaemon or before MApplication()
    fullScreen = true;
    deviceName = "N900";
    themeService = MTheme::LocalTheme;
#endif //M_OS_MAEMO5

    parseArguments(argc, argv, themeService);

#ifdef TESTABLE
#ifndef Q_OS_WIN
    if (!MComponentCache::populating())
#endif
        testabilityInit();
#endif //TESTABLE

    theme = new MTheme(themeIdentifier, QString(), themeService);
    deviceProfile = new MDeviceProfile();

    QString catalog = appIdentifier;
    if (catalog.isEmpty())
        catalog = fileInfo.fileName();

    // set the path for the icu extra data:
    MLocale::setDataPath(M_ICUEXTRADATA_DIR);

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
    // installs the translation catalog of the application if we
    // already know the application/catalog name (not just populate
    // cache)
#ifndef Q_OS_WIN
    if (!MComponentCache::populating())
#endif
    {
        systemLocale.installTrCatalog(catalog);
    }
    MLocale::setDefault(systemLocale);

    // MLocale::setDefault(locale) may also set the
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

#ifndef Q_OS_WIN
    feedbackPlayer = new MFeedbackPlayer();
    if (!feedbackPlayer->d_ptr->init(themeIdentifier)) {
        delete feedbackPlayer;
        feedbackPlayer = 0;
    }
#endif

    QGestureRecognizer::unregisterRecognizer(Qt::TapAndHoldGesture);
    QGestureRecognizer::registerRecognizer(new MTapAndHoldRecognizer());

    QGestureRecognizer::unregisterRecognizer(Qt::PanGesture);
    QGestureRecognizer::registerRecognizer(new MPanRecognizer());

    QGestureRecognizer::unregisterRecognizer(Qt::SwipeGesture);
    QGestureRecognizer::registerRecognizer(new MSwipeRecognizer());

    q->setShowCursor(showCursor);
}

void MComponentDataPrivate::parseArguments(int &argc, char **argv,
                                           MTheme::ThemeService &themeService)
// argc and argv (in and out): command line arguments, used ones are removed
// themeService (out): value changed if theme service is defined in arguments
{
    bool outputLevelSet = false;

    // Assume every argument is used and mark those that are not
    QVector<bool> usedArguments(argc, true);

    // Check environment variables
    QByteArray useSoftwareRendering = qgetenv("M_USE_SOFTWARE_RENDERING");
    if (!useSoftwareRendering.isNull())
        softwareRendering = true;

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
            showObjectNames=true;
        else if (s == "-show-style-names")
            showStyleNames=true;
        else if (s == "-dev") {
            showSize = true;
            showPosition = true;
        } else if (s == "-remote-theme") {
            themeService = MTheme::RemoteTheme;
        } else if (s == "-local-theme") {
            if (softwareRendering && MGraphicsSystemHelper::isRunningMeeGoCompatibleGraphicsSystem()) {
                qCritical("-local-theme in combination with -software is not supported with the MeeGo graphicssystem. Please either drop one of the parameters or use a different graphicssystem.");
                exit(EXIT_FAILURE);
            }
            themeService = MTheme::LocalTheme;
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
        } else if (s == "-performance-debug") {
            g_show_performance_messages = true;
        } else if (s == "-syslog-server") {
            if (i < (argc -1)) {
                i++;

                if (QString(argv[i]) == "local") {
                    syslogServer = "file:///dev/log";
                } else {
                    syslogServer = argv[i];
                }

                if (!syslogServer.isValid()) {
                    qCritical("%s: Error: Given syslog server URL is invalid",
                        argv[0]);
                    exit(EXIT_FAILURE);
                }

            } else {
                qCritical("%s: Error: Please specify the syslog server",
                          argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (s == "-target") {
            if (i < (argc - 1)) {
                i++;
                deviceName = argv[i];

            } else {
                qCritical("%s: Error: Provide a device name to -target",
                          argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (s == "-fixed-orientation") {
            if (i < (argc - 1)) {
                i++;
                QString selectedOrientationString = argv[i];

                if (selectedOrientationString == "0") {
                    forcedOrientationAngle = M::Angle0;
                    isOrientationForced = true;
                } else if (selectedOrientationString == "90") {
                    forcedOrientationAngle = M::Angle90;
                    isOrientationForced = true;
                } else if (selectedOrientationString == "180") {
                    forcedOrientationAngle = M::Angle180;
                    isOrientationForced = true;
                } else if (selectedOrientationString == "270") {
                    forcedOrientationAngle = M::Angle270;
                    isOrientationForced = true;
                } else {
                    qCritical("%s: Error: Provide one of 0, 90, 180, 270 to -fixed-orientation",
                              argv[0]);
                    exit(EXIT_FAILURE);
                }
            } else {
                qCritical("%s: Error: Provide one of 0, 90, 180, 270 to -fixed-orientation",
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
                                         << "  [-show-style-names] Show the names of the styles in the scene\n"
                                         << "  [-show-position] Show widget positions in the scene\n"
                                         << "  [-show-cursor] Force the cursor to be visible\n"
                                         << "  [-reverse] Change the layout direction to right-to-left direction\n"
                                         << "  [-dev] Enable development visualization mode\n"
                                         << "  [-genimglist filename] Generate list of requested images to filename\n"
                                         << "  [-remote-theme] Wait until remote theme daemon is available\n"
                                         << "  [-local-theme] Force usage of local theme processing instead of remote theme daemon\n"
                                         << "  [-output-level debug|warning|critical] Only show messages of given output level or above\n"
                                         << "  [-output-prefix <prefix>] Only show debug messages that start with the given prefix\n"
                                         << "  [-no-output-prefix <prefix>] Only show debug messages that do not start with the given prefix\n"
                                         << "  [-performance-debug] Show debug messages affecting performance\n"
                                         << "  [-syslog-server <server>] Log debug output to a local or remote syslog server instance.\n"
                                         << "      To send messages over the network, specify server as udp://hostname[:port].\n"
                                         << "      To log locally, use the keyword 'local', which is an alias for file:///dev/log,\n"
                                         << "      which is the domain socket on which the syslog daemon listens on Unix.\n"
                                         << "  [-target <name>] Use the target device profile\n"
                                         << "  [-prestart] Prestart the application (if supported)\n"
                                         << "  [-fixed-orientation 0|90|180|270] Start application in fixed orientation. \n "
                                         << "      This overrides keyboard state, as well as a device profile"
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
}

void MComponentDataPrivate::registerNewService(MApplicationService *newService)
{
#ifdef HAVE_DBUS
    service = newService;
    service->registerService();
#else
    Q_UNUSED(newService);
#endif
}

void MComponentDataPrivate::registerDefaultService(const QString &appIdentifier)
{
#ifdef HAVE_DBUS
    QString serviceName = "com.nokia." + appIdentifier;
    registerNewService(new MApplicationService(serviceName));
#else
    Q_UNUSED(appIdentifier);
#endif
}

void MComponentDataPrivate::_q_notifyInputMethodActiveWindowOrientationChangeStarted()
{
    MWindow *activeWindow = MComponentData::activeWindow();
    MSceneManager *sceneManager = (activeWindow) ? activeWindow->sceneManager() : NULL;
    if (sceneManager) {
        MInputMethodState::instance()->startActiveWindowOrientationAngleChange(sceneManager->orientationAngle());
    }
}

void MComponentDataPrivate::_q_notifyInputMethodActiveWindowOrientationChangeFinished()
{
    MWindow *activeWindow = MComponentData::activeWindow();
    if (activeWindow) {
        MInputMethodState::instance()->setActiveWindowOrientationAngle(activeWindow->orientationAngle());
    }
}

#ifdef HAVE_GCONF
void MComponentDataPrivate::_q_updateDebugOptionsFromGConfValues()
{
    Q_Q(MComponentData);

    QString gconfKey = debugOptions.value().toString();
    QBitArray debugOptionsBitArray;

    debugOptionsBitArray.resize(7);
    debugOptionsBitArray.fill(false);

    for (int i = 0; i < debugOptionsBitArray.size(); i++) {
        if (i >= gconfKey.size()) {
            debugOptionsBitArray.setBit(i, false);
        } else {
            if (gconfKey.at(i) == '1') {
                debugOptionsBitArray.setBit(i, true);
            } else {
                debugOptionsBitArray.setBit(i, false);
            }
        }
    }

    q->setShowStyleNames(debugOptionsBitArray.at(0));
    q->setShowObjectNames(debugOptionsBitArray.at(1));
    q->setShowSize(debugOptionsBitArray.at(2));
    q->setShowBoundingRect(debugOptionsBitArray.at(3));
    q->setShowPosition(debugOptionsBitArray.at(4));
    q->setShowMargins(debugOptionsBitArray.at(5));
    q->setShowFps(debugOptionsBitArray.at(6));

    // Necessary to immediately see the activated options
    Q_FOREACH(MWindow *window, q->windows()) {
        if (window->isOnDisplay() && !window->isInSwitcher()) {
            window->viewport()->update();
        }
    }
}
#endif

void MComponentData::reinit(int &argc, char **argv, const QString &appIdentifier, MApplicationService *newService)
{
    Q_D(MComponentData);
    MTheme::ThemeService themeService = MTheme::AnyTheme;

    d->parseArguments(argc, argv,
                      themeService);

    if (d->service) {
        qFatal("MComponentData::reinit() - Called but service already registered.");
    }

    if (!appIdentifier.isEmpty()) {
        d->appName = appIdentifier;
    } else {
        QFileInfo fileInfo(argv[0]);
        d->appName = fileInfo.fileName();
    }

    d->testabilityInit();

    MLocale systemLocale;
    systemLocale.installTrCatalog(d->appName);
    MLocale::setDefault(systemLocale);

    if (MTheme::instance()) {
        MTheme::instance()->d_func()->reinit(d->appName);
    }

    if (newService) {
        d->registerNewService(newService);
    } else {
        d->registerDefaultService(d->appName);
    }

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

bool MComponentData::showStyleNames()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::showStyleNames() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->showStyleNames;
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

void MComponentData::setShowStyleNames(bool show)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setShowStyleNames() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->showStyleNames = show;
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
#if defined(HAVE_XFIXES) && defined(Q_WS_X11)
        XFixesShowCursor(QX11Info::display(), QX11Info::appRootWindow());
#else
        qApp->restoreOverrideCursor();
#endif
    } else {
#if defined(HAVE_XFIXES) && defined(Q_WS_X11)
        XFixesHideCursor(QX11Info::display(), QX11Info::appRootWindow());
#else
        QPixmap cursor(QSize(1, 1));
        cursor.fill(Qt::transparent);
        qApp->setOverrideCursor(cursor);
#endif
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

bool MComponentData::setSyslogServer(const QUrl &url)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::setSyslogServer() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->initSyslogConnection(url);
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

    return gMComponentDataPrivate->firstWindow;
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
    MSceneManager *oldSceneManager = (activeWindow) ? activeWindow->sceneManager() : NULL;
    MSceneManager *newSceneManager = (w) ? w->sceneManager() : NULL;

    if (oldSceneManager) {
        QObject::disconnect(oldSceneManager,
                            SIGNAL(orientationAboutToChange(M::Orientation)),
                            MComponentData::instance(),
                            SLOT(_q_notifyInputMethodActiveWindowOrientationChangeStarted()));
        QObject::disconnect(oldSceneManager,
                            SIGNAL(orientationChangeFinished(M::Orientation)),
                            MComponentData::instance(),
                            SLOT(_q_notifyInputMethodActiveWindowOrientationChangeFinished()));
    }
    if (newSceneManager) {
        QObject::connect(newSceneManager,
                         SIGNAL(orientationAboutToChange(M::Orientation)),
                         MComponentData::instance(),
                         SLOT(_q_notifyInputMethodActiveWindowOrientationChangeStarted()),
                         Qt::DirectConnection);
        QObject::connect(newSceneManager,
                         SIGNAL(orientationChangeFinished(M::Orientation)),
                         MComponentData::instance(),
                         SLOT(_q_notifyInputMethodActiveWindowOrientationChangeFinished()),
                         Qt::DirectConnection);
        MInputMethodState::instance()->setActiveWindowOrientationAngle(w->orientationAngle());
    }
    int oldIndex = gMComponentDataPrivate->windows.indexOf(w);

    if (oldIndex == -1)
        mWarning("MComponentData::setActiveWindow()") << "attempting to activate unregistered window";
    else {
        gMComponentDataPrivate->windows.move(oldIndex, 0);
        gMComponentDataPrivate->firstWindow = w;
    }
}

void MComponentData::registerWindow(MWindow *w)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::registerWindow() - MComponentData instance not yet created.");
    }
    if (gMComponentDataPrivate->windows.isEmpty()) {
        gMComponentDataPrivate->windows.append(w);
        gMComponentDataPrivate->firstWindow = w;
    }
    else if (!gMComponentDataPrivate->windows.contains(w)) {
        gMComponentDataPrivate->windows.append(w);
    }
}

void MComponentData::unregisterWindow(MWindow *w)
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::unregisterWindow() - MComponentData instance not yet created.");
    }
    gMComponentDataPrivate->windows.removeAll(w);
    if (gMComponentDataPrivate->firstWindow == w) {
        if (gMComponentDataPrivate->windows.isEmpty()) {
            gMComponentDataPrivate->firstWindow = 0;
        }
        else {
            gMComponentDataPrivate->firstWindow = gMComponentDataPrivate->windows.at(0);
        }
    }
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
#ifdef HAVE_DBUS
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::serviceName() - MComponentData instance not yet created.");
    } else if (!gMComponentDataPrivate->service) {
        qFatal("MComponentData::serviceName() - MComponentData->service not yet created.");
    }

    return gMComponentDataPrivate->service->registeredName();
#else
    return QString();
#endif // HAVE_DBUS
}

M::OrientationAngle MComponentData::forcedOrientationAngle()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::orientationAngleIsSupported() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->forcedOrientationAngle;
}

bool MComponentData::isOrientationForced()
{
    if (!gMComponentDataPrivate) {
        qFatal("MComponentData::allowedOrientations() - MComponentData instance not yet created.");
    }
    return gMComponentDataPrivate->isOrientationForced;
}

#ifdef Q_WS_X11
static int handleXError(Display *, XErrorEvent *)
{
    return 0;
}
#endif // Q_WS_X11

bool MComponentData::isMeeGoWindowManagerRunning()
{
    bool retValue = false;

#ifdef Q_WS_X11

    Display       *dpy = QX11Info::display();
    Window         rootw = RootWindow(dpy, XDefaultScreen(dpy));
    Atom           wmSupportAtom = XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", False);
    Atom           type;
    int            format;
    unsigned long  numItems;
    unsigned long  bytesAfter;
    unsigned char *data = 0;

    if (XGetWindowProperty(dpy, rootw, wmSupportAtom, 0, 1, False, XA_WINDOW,
                           &type, &format, &numItems, &bytesAfter, &data) == Success) {
        if (data) {

            Window wid = *(reinterpret_cast<Window *>(data));
            XFree(data);
            data = 0;

            Atom wmNameAtom = XInternAtom(dpy, "WM_NAME", False);

            // Set error handler because window wid we got might not exist and
            // the following name query would fail.
            int (*previousHandler)(Display *, XErrorEvent *) = XSetErrorHandler(handleXError);

            if (XGetWindowProperty(dpy, wid, wmNameAtom, 0, 16, False, XA_STRING,
                                   &type, &format, &numItems, &bytesAfter, &data) == Success) {
                if (data) {
                    if (strcmp(reinterpret_cast<const char *>(data), "MCompositor") == 0) {
                        retValue = true;
                    }

                    XFree(data);
                    data = 0;
                }
            }

            XSetErrorHandler(previousHandler);
        }
    }
#endif  // Q_WS_X11

    return retValue;
}

#ifdef Q_WS_X11
QStack<MComponentData::ChainData> MComponentDataPrivate::chainData;

void MComponentData::pushChainData(const ChainData &newChainData)
{
    MComponentDataPrivate::chainData.push( newChainData );

    if ( MComponentData::instance() )
    {
        emit MComponentData::instance()->chainTaskDataChanged();
    }
}

MComponentData::ChainData MComponentData::popChainData()
{
    ChainData retVal = MComponentDataPrivate::chainData.pop();

    return retVal;
}

bool MComponentData::chainDataStackIsEmpty()
{
    bool retVal = MComponentDataPrivate::chainData.isEmpty();

    return retVal;
}

WId MComponentData::lastChainParentWinId()
{
    if ( MComponentDataPrivate::chainData.isEmpty() )
    {
        return 0;
    }

    return MComponentDataPrivate::chainData.top().first;
}
#endif // Q_WS_X11


#include "moc_mcomponentdata.cpp"
