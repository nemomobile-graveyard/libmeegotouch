/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <MLocale>
#include <mwindow_p.h>
#include "ut_mapplication.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationService>
#include <MComponentData>

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#endif //Q_WS_X11

#include <mtheme.h>
#include <mwindow_stub.h>
//#include <mfeedbackplayer_stub.h>
//#include <mfeedbackplayerprivate_stub.h>
#include <mfeedback_stub.h>

//todo: mapplication.cpp - these functions should not be called this way: window->d_ptr->sendDelayedExitDisplayEvent();
//when we fix that we can get rid off these 3 stubs
void MWindowPrivate::sendDelayedExitDisplayEvent()
{
}
void MWindowPrivate::setX11PrestartProperty(bool)
{
}
void MWindowPrivate::resolveOrientationRules()
{
}

const char* MWindowPrivate::LogicallyClosedPropertyName = "_m_logicallyClosed";

class MyApplication: public MApplication
{
public:
    MyApplication(int &argc, char **argv);
    MyApplication(int &argc, char **argv, MApplicationService *service);
    virtual void releasePrestart();
    virtual void restorePrestart();
};


MyApplication::MyApplication(int &argc, char **argv)
    : MApplication(argc, argv)
{
}

MyApplication::MyApplication(int &argc, char **argv, MApplicationService *service)
    : MApplication(argc, argv, service)
{
}

bool Ut_MApplication::m_releasePrestart = false;
void MyApplication::releasePrestart()
{
    Ut_MApplication::m_releasePrestart = true;
}

bool Ut_MApplication::m_restorePrestart = false;
void MyApplication::restorePrestart()
{
    Ut_MApplication::m_restorePrestart = true;
}

void Ut_MApplication::init()
{
    app = NULL;
    m_prestartReleased = false;
    m_prestartRestored = false;
    m_releasePrestart = false;
    m_restorePrestart = false;

    registeredService.clear();
}

void Ut_MApplication::cleanup()
{
}

void Ut_MApplication::mApplicationConstructor_data()
{
    QTest::addColumn<int>("count");
    QTest::addColumn<QString>("params");
    QTest::newRow("hw") << 2 <<  QString("-show-br -show-fps");
    QTest::newRow("hw-fullscreen") << 3 <<  QString("-show-br -show-fps -fullscreen");
    QTest::newRow("sw") << 2 <<  QString("-software -show-br");
    QTest::newRow("sw-fullscreen") << 3 <<  QString("-software -show-br -fullscreen");
}

MApplication *Ut_MApplication::buildApp(int count, const QString &params, MApplicationService *service)
{
    return buildApp(count, params, QString(), service);
}

MApplication *Ut_MApplication::buildApp(int count, const QString &params, const QString &appId, MApplicationService *service)
{
    MApplication *retVal = 0;

    QChar sep(' ');
    static char *argv[MAX_PARAMS] = {'\0'};
    static int x = 0;
    x = 0; //we have to always reset it

    QStringList list = params.split(sep);
    QStringListIterator it(list);
    while (it.hasNext() && x < MAX_PARAMS)  {
        argv[x++] = strdup(it.next().toLocal8Bit().constData());
    }
    Q_ASSERT(count == x);

    // try to call the constructor with the correct signature
    if (service) {
        if (appId.isEmpty()) {
            retVal = new MApplication(x, argv, service);
        } else {
            retVal = new MApplication(x, argv, appId, service);
        }
    } else {
        if (appId.isEmpty()) {
            retVal = new MApplication(x, argv);
        } else {
            retVal = new MApplication(x, argv, appId);
        }
    }

    return retVal;
}

MApplication *Ut_MApplication::buildPrestartApp(int count, const QString &params, MApplicationService *service)
{
    MApplication *retVal = 0;

    QChar sep(' ');
    static char *argv[MAX_PARAMS] = {'\0'};;
    static int x = 0;
    x = 0; //we have to always reset it

    QStringList list = params.split(sep);
    QStringListIterator it(list);
    while (it.hasNext() && x < MAX_PARAMS)  {
        argv[x++] = strdup(it.next().toLocal8Bit().constData());
    }
    Q_ASSERT(count == x);

    // try to call the constructor with the correct signature
    if (service) {
        retVal = new MyApplication(x, argv, service);
    } else {
        retVal = new MyApplication(x, argv);
    }

    return retVal;
}

QDir Ut_MApplication::generateThemeDirectory()
{
    QDir themeDir(THEMEDIR);
    return themeDir;
}

QDir Ut_MApplication::generateApplicationThemeDirectory(const QString &applicationName)
{
    QDir themeDir = generateThemeDirectory();
    themeDir.setPath(themeDir.absolutePath() + QDir::separator() + applicationName);
    return themeDir;
}

void Ut_MApplication::mApplicationConstructor()
{
    QFETCH(int, count);
    QFETCH(QString, params);

    app = buildApp(count, params);
    QVERIFY(app != NULL);
    QVERIFY(!registeredService.isEmpty());
    delete app;
}

QString Ut_MApplication::registeredService = QString();

bool MApplicationService::registerService()
{
    Ut_MApplication::registeredService = registeredName();

    return true;
}

void Ut_MApplication::mApplicationConstructorWithAppId()
{
    QString serviceBasename = "com.nokia.";
    QString appName;
    QString argv0;

    argv0 = "appName";
    app = buildApp(1, argv0);
    delete app;
    QCOMPARE(registeredService, serviceBasename + argv0);

    appName = "xY0_-";
    argv0 = "appName";
    app = buildApp(1, argv0, appName);
    delete app;
    QCOMPARE(registeredService, serviceBasename + appName);

    // A whitespace as app identifier is invalid and not used
    appName = ' ';
    argv0 = "appName";
    app = buildApp(1, argv0, appName);
    delete app;
    QCOMPARE(registeredService, serviceBasename + argv0);
}

class MyApplicationService : public MApplicationService
{
public:
    MyApplicationService(const QString &newServiceName);
    virtual ~MyApplicationService();
};

MyApplicationService::MyApplicationService(const QString &newServiceName) :
    MApplicationService(newServiceName)
{
}

MyApplicationService::~MyApplicationService()
{
}

void Ut_MApplication::mApplicationConstructorWithService()
{
    MyApplicationService *myService = new MyApplicationService("com.nokia.Ut_MApplicationService");
    app = buildApp(1, "appName", myService);
    QString thisRegisteredName = myService->registeredName();
    delete app;
    QCOMPARE(thisRegisteredName, QString("com.nokia.Ut_MApplicationService"));
}


void Ut_MApplication::mAppName()
{
    app = buildApp(2, "appName1 -foobar");
    QCOMPARE(app->appName(), QString("appName1"));
    delete app;

    app = buildApp(2, "./appName2 -foobar");
    QCOMPARE(app->appName(), QString("appName2"));
    delete app;

    app = buildApp(2, "/foo/bar/appName3 -foobar");
    QCOMPARE(app->appName(), QString("appName3"));
    delete app;
}
#if 0
void Ut_MApplication::themeFileLoadingFromCurrentPath()
{
    // Setup the testing environment
    // Both svg and css files are found from current directory.
    gFileSystemVerifier.addValidFile("m.css");
    gFileSystemVerifier.addValidFile("m.svg");
    // Create application
    app = buildApp(1, "appName -foobar");
    QCOMPARE(app->applicationName(), QString("appName"));
    delete app;

    // Verify that correct svg and css files were loaded
    MyMThemeStub *themeStub = dynamic_cast<MyMThemeStub *>(gMThemeStub);
    Q_ASSERT(themeStub);
    QCOMPARE(themeStub->loadedSVG, QString("m.svg"));
    QCOMPARE(themeStub->loadedCSS, QString("m.css"));
}

void Ut_MApplication::themeFileLoadingFromApplicationThemePath()
{
    // Setup the testing environment
    gFileSystemVerifier.addValidPath(generateThemeDirectory().path());
    gFileSystemVerifier.addValidPath(generateApplicationThemeDirectory("appName").path());
    // Create application
    app = buildApp(1, "appName -foobar");
    QCOMPARE(app->applicationName(), QString("appName"));
    delete app;

    // Verify that correct svg file was loaded
    MyMThemeStub *themeStub = dynamic_cast<MyMThemeStub *>(gMThemeStub);
    Q_ASSERT(themeStub);
    QFileInfo loadedSVG(generateApplicationThemeDirectory(QString("appName")), "m.svg");
    QCOMPARE(themeStub->loadedSVG, loadedSVG.absoluteFilePath());
    QFileInfo loadedCSS(generateApplicationThemeDirectory(QString("appName")), "m.css");
    QCOMPARE(themeStub->loadedCSS, loadedCSS.absoluteFilePath());
}

void Ut_MApplication::themeFileLoadingFromGlobalThemePath()
{
    // Setup the testing environment
    gFileSystemVerifier.addValidPath(generateThemeDirectory().path());
    // Create application
    app = buildApp(1, "appName -foobar");
    QCOMPARE(app->applicationName(), QString("appName"));
    delete app;

    // Verify that correct svg file was loaded
    MyMThemeStub *themeStub = dynamic_cast<MyMThemeStub *>(gMThemeStub);
    Q_ASSERT(themeStub);
    QFileInfo loadedSVG(generateThemeDirectory(), "m.svg");
    QCOMPARE(themeStub->loadedSVG, loadedSVG.absoluteFilePath());
    QFileInfo loadedCSS(generateThemeDirectory(), "m.css");
    QCOMPARE(themeStub->loadedCSS, loadedCSS.absoluteFilePath());
}
#endif

void Ut_MApplication::mApplicationTranslationPath()
{
    app = buildApp(1, "appName");
    MLocale l;

    QStringList list = l.translationPaths();

    QVERIFY(list.size() == 1);
    if (list.size() == 1) {
        QVERIFY(list.at(0) == TRANSLATION_DIR);
    }

    delete app;
}

/*
void Ut_MApplication::mApplicationNoFeedback()
{
    gMFeedbackPlayerPrivateStub->stubSetReturnValue("init", false);
    app = buildApp(1, "appName");
    QVERIFY(app->feedbackPlayer() == 0);
    delete app;

    gMFeedbackPlayerPrivateStub->stubSetReturnValue("init", true);
    app = buildApp(1, "appName");
    QVERIFY(app->feedbackPlayer() != 0);
    delete app;
}
*/

//these 2 tests request graphcical assets from themedaemon which is running on device
//in meego graphicssystem so we need to use raster and local daemon instance.
//anyway these 2 tests are a bit fragile so in case they can be disabled..
void Ut_MApplication::testActiveApplicationWindow()
{
    //we have to avoid using meego graphicssystem
    app = buildApp( 4, "appName -graphicssystem raster -local-theme" );
    QVERIFY( app->activeApplicationWindow() == 0 );

    MApplicationWindow *appWin = new MApplicationWindow;
    QVERIFY( app->activeApplicationWindow() == appWin );

    delete appWin;
    QVERIFY( app->activeApplicationWindow() == 0 );

    delete app;
}

void Ut_MApplication::testApplicationWindows()
{
    //we have to avoid using meego graphicssystem
    app = buildApp( 4, "appName -graphicssystem raster -local-theme" );

    MApplicationWindow *appWin1 = new MApplicationWindow;
    MApplicationWindow *appWin2 = new MApplicationWindow;
    MApplicationWindow *appWin3 = new MApplicationWindow;

    MApplicationWindow *appWin4 = new MApplicationWindow;
    delete appWin4;

    QVERIFY( app->windows().size() == 3 );
    QVERIFY( app->windows().contains( appWin1 ) );
    QVERIFY( app->windows().contains( appWin2 ) );
    QVERIFY( app->windows().contains( appWin3 ) );

    QVERIFY( !app->windows().contains( appWin4 ) );

    delete appWin3;
    QVERIFY( !app->windows().contains( appWin3 ) );

    delete appWin2;
    delete appWin1;
    delete app;
}


void Ut_MApplication::testPrestartMode()
{
    // Test that mode is set when started with -prestart
    app = buildApp(2, "appName -prestart");
    app->setPrestartMode(M::TerminateOnClose);
    QCOMPARE(app->prestartMode(), M::TerminateOnClose);
    app->setPrestartMode(M::LazyShutdown);
    QCOMPARE(app->prestartMode(), M::LazyShutdown);
    app->setPrestartMode(M::TerminateOnCloseMultiWindow);
    QCOMPARE(app->prestartMode(), M::TerminateOnCloseMultiWindow);
    app->setPrestartMode(M::LazyShutdownMultiWindow);
    QCOMPARE(app->prestartMode(), M::LazyShutdownMultiWindow);
    delete app;

    // Test that mode is not set when not started with -prestart
    app = buildApp(1, "appName");
    app->setPrestartMode(M::TerminateOnClose);
    QCOMPARE(app->prestartMode(), M::NoPrestart);
    delete app;
}

void Ut_MApplication::testReleasePrestartModeTerminateOnClose()
{
    // Test that app releases correctly from prestart
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartReleased()), this, SLOT(prestartReleased()));
    app->setPrestartMode(M::TerminateOnClose);
    service->launch();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was sent
    QCOMPARE(m_prestartReleased, true);
    // Test that the handler is called
    QCOMPARE(m_releasePrestart, true);
    delete app;
}

void Ut_MApplication::testReleasePrestartModeLazyShutdown()
{
    // Test that app releases correctly from prestart
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartReleased()), this, SLOT(prestartReleased()));
    app->setPrestartMode(M::LazyShutdown);
    service->launch();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was sent
    QCOMPARE(m_prestartReleased, true);
    // Test that the handler is called
    QCOMPARE(m_releasePrestart, true);
    delete app;
}

void Ut_MApplication::testReleasePrestartModeTerminateOnCloseMultiWindow()
{
    // Test that app releases correctly from prestart
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartReleased()), this, SLOT(prestartReleased()));
    app->setPrestartMode(M::TerminateOnCloseMultiWindow);
    service->launch();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was sent
    QCOMPARE(m_prestartReleased, true);
    // Test that the handler is called
    QCOMPARE(m_releasePrestart, true);
    delete app;
}

void Ut_MApplication::testReleasePrestartModeLazyShutdownMultiWindow()
{
    // Test that app releases correctly from prestart
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartReleased()), this, SLOT(prestartReleased()));
    app->setPrestartMode(M::LazyShutdownMultiWindow);
    service->launch();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was sent
    QCOMPARE(m_prestartReleased, true);
    // Test that the handler is called
    QCOMPARE(m_releasePrestart, true);
    delete app;
}

void Ut_MApplication::testRestorePrestartModeLazyShutdown()
{
    // Test that app restores correctly to prestart
    // if in LazyShutdown mode
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartRestored()), this, SLOT(prestartRestored()));
    app->setPrestartMode(M::LazyShutdown);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), true);
    // Test that the signal was sent
    QCOMPARE(m_prestartRestored, true);
    // Test that the handler is called
    QCOMPARE(m_restorePrestart, true);
    delete app;
}

void Ut_MApplication::testRestorePrestartModeTerminateOnClose()
{
    // Test that app won't restore to prestart
    // if not in LazyShutdown mode
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartRestored()), this, SLOT(prestartRestored()));
    app->setPrestartMode(M::TerminateOnClose);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was not sent
    QCOMPARE(m_prestartRestored, false);
    // Test that the handler is called
    QCOMPARE(m_restorePrestart, false);
    delete app;
}

void Ut_MApplication::testRestorePrestartModeLazyShutdownMultiWindow()
{
    // Test that app restores correctly to prestart
    // if in LazyShutdown mode
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartRestored()), this, SLOT(prestartRestored()));
    app->setPrestartMode(M::LazyShutdownMultiWindow);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), true);
    // Test that the signal was sent
    QCOMPARE(m_prestartRestored, true);
    // Test that the handler is called
    QCOMPARE(m_restorePrestart, true);
    delete app;
}

void Ut_MApplication::testRestorePrestartModeTerminateOnCloseMultiWindow()
{
    // Test that app won't restore to prestart
    // if not in LazyShutdown mode
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartRestored()), this, SLOT(prestartRestored()));
    app->setPrestartMode(M::TerminateOnCloseMultiWindow);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was not sent
    QCOMPARE(m_prestartRestored, false);
    // Test that the handler is called
    QCOMPARE(m_restorePrestart, false);
    delete app;
}

void Ut_MApplication::testIsPrestartedLazyShutdown()
{
    // Test that app restores correctly to prestart
    // if in LazyShutdown mode
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(M::LazyShutdown);
    QCOMPARE(app->isPrestarted(), true);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), true);
    delete app;
}

void Ut_MApplication::testIsPrestartedTerminateOnClose()
{
    // Test that app won't restore to prestart
    // if not in LazyShutdown mode
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(M::TerminateOnClose);
    QCOMPARE(app->isPrestarted(), true);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), false);
    delete app;
}

void Ut_MApplication::testIsPrestartedLazyShutdownMultiWindow()
{
    // Test that app restores correctly to prestart
    // if in LazyShutdown mode
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(M::LazyShutdownMultiWindow);
    QCOMPARE(app->isPrestarted(), true);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), true);
    delete app;
}

void Ut_MApplication::testIsPrestartedTerminateOnCloseMultiWindow()
{
    // Test that app won't restore to prestart
    // if not in LazyShutdown mode
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(M::TerminateOnCloseMultiWindow);
    QCOMPARE(app->isPrestarted(), true);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), false);
    delete app;
}


void Ut_MApplication::testIsPrestartedNoPrestart()
{
    // Test that app won't enter prestart
    // if -prestart not set
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(1, "appName", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(M::TerminateOnClose);
    QCOMPARE(app->isPrestarted(), false);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), false);
    delete app;
}

void Ut_MApplication::testPrestartedProperty()
{
    MApplicationService *service = new MApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->setPrestartMode(M::LazyShutdown);
    app->connect(app, SIGNAL(prestartReleased()), this, SLOT(prestartReleased()));
    app->connect(app, SIGNAL(prestartRestored()), this, SLOT(prestartRestored()));

    QCOMPARE(app->isPrestarted(), true);

    app->setPrestarted(false);
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was sent
    QCOMPARE(m_prestartReleased, true);
    // Test that the handler is called
    QCOMPARE(m_releasePrestart, true);

    app->setPrestarted(true);
    QCOMPARE(app->isPrestarted(), true);
    // Test that the signal was sent
    QCOMPARE(m_prestartRestored, true);
    // Test that the handler is called
    QCOMPARE(m_restorePrestart, true);

    delete app;
}

#ifdef Q_WS_X11
void Ut_MApplication::testMinimizing_data()
{
    QTest::addColumn<int>("progress");
    QTest::addColumn<int>("minimizingCount");
    QTest::addColumn<int>("minimizedCount");
    QTest::addColumn<int>("minimizingCanceledCount");
    QTest::newRow("finish minimizing") << 100 << 1 << 1 << 0;
    QTest::newRow("cancel minimizing") <<   0 << 1 << 0 << 1;
    QTest::newRow("minimizing twice")  <<   1 << 2 << 0 << 0;
}

void Ut_MApplication::testMinimizing()
{
    QFETCH(int, progress);
    QFETCH(int, minimizingCount);
    QFETCH(int, minimizedCount);
    QFETCH(int, minimizingCanceledCount);
        
    app = buildApp(1, "appName");
    
    QSignalSpy minimizingSpy(app, SIGNAL(minimizing()));
    QSignalSpy minimizingCanceledSpy(app, SIGNAL(minimizingCanceled()));
    QSignalSpy minimizedSpy(app, SIGNAL(minimized()));
    
    MWindow *win = new MWindow();
    MComponentData::registerWindow(win);
    win->setProperty(MWindowPrivate::LogicallyClosedPropertyName, false);

    const Atom minimizeMsgAtom = XInternAtom(QX11Info::display(),
                                             "_MEEGOTOUCH_MINIMIZE_ANIMATION",
                                             false);
    XEvent event;
    memset(&event, 0, sizeof(event));
    event.xclient.type = ClientMessage;
    event.xclient.message_type = minimizeMsgAtom;
    event.xclient.window = win->winId();
    event.xclient.format = 32;
    event.xclient.data.l[0] = 1; // = start minimizing    
    
    app->x11EventFilter(&event);
    QCOMPARE(minimizingSpy.count(), 1);
    QCOMPARE(minimizingCanceledSpy.count(), 0);
    QCOMPARE(minimizedSpy.count(), 0);
    
    event.xclient.data.l[0] = progress;
    app->x11EventFilter(&event);
    QCOMPARE(minimizingSpy.count(), minimizingCount);
    QCOMPARE(minimizingCanceledSpy.count(), minimizingCanceledCount);
    QCOMPARE(minimizedSpy.count(), minimizedCount);

    MComponentData::unregisterWindow(win);
    delete win;
    delete app;
}

void Ut_MApplication::testMinimizingCanceledByClosedWindow()
{  
    const int windowsCount = 3;
    
    app = buildApp(1, "appName");
    QList<MWindow*> windows;
    for (int i = 0; i < windowsCount; ++i) {
        MWindow *win = new MWindow();
        MComponentData::registerWindow(win);
        win->setProperty(MWindowPrivate::LogicallyClosedPropertyName, false);
        windows.append(win);
    }
    
    const Atom minimizeMsgAtom = XInternAtom(QX11Info::display(),
                                             "_MEEGOTOUCH_MINIMIZE_ANIMATION",
                                             false);
    XEvent event;
    memset(&event, 0, sizeof(event));
    event.xclient.type = ClientMessage;
    event.xclient.message_type = minimizeMsgAtom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = 1; // 1 = start minimizing

    QSignalSpy minimizingSpy(app, SIGNAL(minimizing()));
    for (int i = 0; i < windowsCount; ++i) {
        event.xclient.window = windows.at(i)->winId();
        app->x11EventFilter(&event);
    }
    QCOMPARE(minimizingSpy.count(), windowsCount);
    
    QSignalSpy minimizingCanceledSpy(app, SIGNAL(minimizingCanceled()));
    for (int i = 0; i < windowsCount; ++i) {
        windows[i]->setProperty(MWindowPrivate::LogicallyClosedPropertyName, true);
    }
    QCOMPARE(minimizingCanceledSpy.count(), windowsCount);
    
    for (int i = 0; i < windowsCount; ++i) {
        MComponentData::registerWindow(windows.at(i));
    }
    qDeleteAll(windows);
    delete app;
}
#endif // Q_WS_X11

void Ut_MApplication::prestartReleased()
{
    m_prestartReleased = true;
}

void Ut_MApplication::prestartRestored()
{
    m_prestartRestored = true;
}

QTEST_APPLESS_MAIN(Ut_MApplication);
