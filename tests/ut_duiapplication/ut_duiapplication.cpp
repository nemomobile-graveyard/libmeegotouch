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

#include <DuiLocale>
#include "ut_duiapplication.h"

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiApplicationService>

#include <duitheme.h>
#include <duifeedbackplayer_stub.h>
#include <duifeedbackplayerprivate_stub.h>
#include <duifeedback_stub.h>

class MyApplication: public DuiApplication
{
public:
    MyApplication(int &argc, char **argv);
    MyApplication(int &argc, char **argv, DuiApplicationService *service);
    virtual void releasePrestart();
    virtual void restorePrestart();
};


MyApplication::MyApplication(int &argc, char **argv)
    : DuiApplication(argc, argv)
{
}

MyApplication::MyApplication(int &argc, char **argv, DuiApplicationService *service)
    : DuiApplication(argc, argv, service)
{
}

bool Ut_DuiApplication::m_releasePrestart = false;
void MyApplication::releasePrestart()
{
    Ut_DuiApplication::m_releasePrestart = true;
}

bool Ut_DuiApplication::m_restorePrestart = false;
void MyApplication::restorePrestart()
{
    Ut_DuiApplication::m_restorePrestart = true;
}

void Ut_DuiApplication::init()
{
    m_prestartReleased = false;
    m_prestartRestored = false;
    m_releasePrestart = false;
    m_restorePrestart = false;

    registeredService.clear();
}

void Ut_DuiApplication::cleanup()
{
}

void Ut_DuiApplication::duiApplicationConstructor_data()
{
    QTest::addColumn<int>("count");
    QTest::addColumn<QString>("params");
    QTest::newRow("hw") << 2 <<  QString("-show-br -show-fps");
    QTest::newRow("hw-fullscreen") << 3 <<  QString("-show-br -show-fps -fullscreen");
    QTest::newRow("sw") << 2 <<  QString("-software -show-br");
    QTest::newRow("sw-fullscreen") << 3 <<  QString("-software -show-br -fullscreen");
}

DuiApplication *Ut_DuiApplication::buildApp(int count, const QString &params, DuiApplicationService *service)
{
    return buildApp(count, params, QString(), service);
}

DuiApplication *Ut_DuiApplication::buildApp(int count, const QString &params, const QString &appId, DuiApplicationService *service)
{
    DuiApplication *retVal = 0;

    QChar sep(' ');
    char *argv[MAX_PARAMS];
    int x = 0;

    QStringList list = params.split(sep);
    QStringListIterator it(list);
    while (it.hasNext() && x < MAX_PARAMS)  {
        argv[x++] = strdup(it.next().toLocal8Bit().constData());
    }

    // try to call the constructor with the correct signature
    if (service) {
        if (appId.isEmpty()) {
            retVal = new DuiApplication(count, argv, service);
        } else {
            retVal = new DuiApplication(count, argv, appId, service);
        }
    } else {
        if (appId.isEmpty()) {
            retVal = new DuiApplication(count, argv);
        } else {
            retVal = new DuiApplication(count, argv, appId);
        }
    }

    return retVal;
}

DuiApplication *Ut_DuiApplication::buildPrestartApp(int count, const QString &params, DuiApplicationService *service)
{
    DuiApplication *retVal = 0;

    QChar sep(' ');
    char *argv[MAX_PARAMS];
    int x = 0;

    QStringList list = params.split(sep);
    QStringListIterator it(list);
    while (it.hasNext() && x < MAX_PARAMS)  {
        argv[x++] = strdup(it.next().toLocal8Bit().constData());
    }

    // try to call the constructor with the correct signature
    if (service) {
        retVal = new MyApplication(count, argv, service);
    } else {
        retVal = new MyApplication(count, argv);
    }

    return retVal;
}

QDir Ut_DuiApplication::generateThemeDirectory()
{
    QDir themeDir(THEMEDIR);
    return themeDir;
}

QDir Ut_DuiApplication::generateApplicationThemeDirectory(const QString &applicationName)
{
    QDir themeDir = generateThemeDirectory();
    themeDir.setPath(themeDir.absolutePath() + QDir::separator() + applicationName);
    return themeDir;
}

void Ut_DuiApplication::duiApplicationConstructor()
{
    QFETCH(int, count);
    QFETCH(QString, params);

    DuiApplication *app = NULL;

    app = buildApp(count, params);
    QVERIFY(app != NULL);
    QVERIFY(!registeredService.isEmpty());
    delete app;
}

QString Ut_DuiApplication::registeredService = QString();

bool DuiApplicationService::registerService()
{
    Ut_DuiApplication::registeredService = registeredName();

    return true;
}

void Ut_DuiApplication::duiApplicationConstructorWithAppId()
{
    QString serviceBasename = "com.nokia.";
    QString appName;
    QString argv0;

    argv0 = "appName";
    DuiApplication *app = buildApp(1, argv0);
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

class MyApplicationService : public DuiApplicationService
{
public:
    MyApplicationService(const QString &newServiceName);
    virtual ~MyApplicationService();
};

MyApplicationService::MyApplicationService(const QString &newServiceName) :
    DuiApplicationService(newServiceName)
{
}

MyApplicationService::~MyApplicationService()
{
}

void Ut_DuiApplication::duiApplicationConstructorWithService()
{
    MyApplicationService *myService = new MyApplicationService("com.nokia.Ut_DuiApplicationService");
    DuiApplication *app = buildApp(1, "appName", myService);
    QString thisRegisteredName = myService->registeredName();
    delete app;
    QCOMPARE(thisRegisteredName, QString("com.nokia.Ut_DuiApplicationService"));
}

#if 0
void Ut_DuiApplication::duiApplicationName()
{
    DuiApplication *app = buildApp(1, "appName -foobar");
    QCOMPARE(app->applicationName(), QString("appName"));
    delete app;

    app = buildApp(1, "./appName -foobar");
    QCOMPARE(app->applicationName(), QString("appName"));
    delete app;

    app = buildApp(1, "/foo/bar/appName -foobar");
    QCOMPARE(app->applicationName(), QString("appName"));
    delete app;
}

void Ut_DuiApplication::themeFileLoadingFromCurrentPath()
{
    // Setup the testing environment
    // Both svg and css files are found from current directory.
    gFileSystemVerifier.addValidFile("dui.css");
    gFileSystemVerifier.addValidFile("dui.svg");
    // Create application
    DuiApplication *app = buildApp(1, "appName -foobar");
    QCOMPARE(app->applicationName(), QString("appName"));
    delete app;

    // Verify that correct svg and css files were loaded
    MyDuiThemeStub *themeStub = dynamic_cast<MyDuiThemeStub *>(gDuiThemeStub);
    Q_ASSERT(themeStub);
    QCOMPARE(themeStub->loadedSVG, QString("dui.svg"));
    QCOMPARE(themeStub->loadedCSS, QString("dui.css"));
}

void Ut_DuiApplication::themeFileLoadingFromApplicationThemePath()
{
    // Setup the testing environment
    gFileSystemVerifier.addValidPath(generateThemeDirectory().path());
    gFileSystemVerifier.addValidPath(generateApplicationThemeDirectory("appName").path());
    // Create application
    DuiApplication *app = buildApp(1, "appName -foobar");
    QCOMPARE(app->applicationName(), QString("appName"));
    delete app;

    // Verify that correct svg file was loaded
    MyDuiThemeStub *themeStub = dynamic_cast<MyDuiThemeStub *>(gDuiThemeStub);
    Q_ASSERT(themeStub);
    QFileInfo loadedSVG(generateApplicationThemeDirectory(QString("appName")), "dui.svg");
    QCOMPARE(themeStub->loadedSVG, loadedSVG.absoluteFilePath());
    QFileInfo loadedCSS(generateApplicationThemeDirectory(QString("appName")), "dui.css");
    QCOMPARE(themeStub->loadedCSS, loadedCSS.absoluteFilePath());
}

void Ut_DuiApplication::themeFileLoadingFromGlobalThemePath()
{
    // Setup the testing environment
    gFileSystemVerifier.addValidPath(generateThemeDirectory().path());
    // Create application
    DuiApplication *app = buildApp(1, "appName -foobar");
    QCOMPARE(app->applicationName(), QString("appName"));
    delete app;

    // Verify that correct svg file was loaded
    MyDuiThemeStub *themeStub = dynamic_cast<MyDuiThemeStub *>(gDuiThemeStub);
    Q_ASSERT(themeStub);
    QFileInfo loadedSVG(generateThemeDirectory(), "dui.svg");
    QCOMPARE(themeStub->loadedSVG, loadedSVG.absoluteFilePath());
    QFileInfo loadedCSS(generateThemeDirectory(), "dui.css");
    QCOMPARE(themeStub->loadedCSS, loadedCSS.absoluteFilePath());
}
#endif

void Ut_DuiApplication::duiApplicationTranslationPath()
{
    DuiApplication *app = buildApp(1, "appName");
    DuiLocale l;

    QStringList list = l.translationPaths();

    QVERIFY(list.size() == 1);
    if (list.size() == 1) {
        QVERIFY(list.at(0) == TRANSLATION_DIR);
    }

    delete app;
}

void Ut_DuiApplication::duiApplicationNoFeedback()
{
    gDuiFeedbackPlayerPrivateStub->stubSetReturnValue("init", false);
    DuiApplication *app = buildApp(1, "appName");
    QVERIFY(app->feedbackPlayer() == 0);
    delete app;

    gDuiFeedbackPlayerPrivateStub->stubSetReturnValue("init", true);
    app = buildApp(1, "appName");
    QVERIFY(app->feedbackPlayer() != 0);
    delete app;
}

/*
void Ut_DuiApplication::testActiveApplicationWindow()
{
    app = buildApp( 2, "appName -software" );
    QVERIFY( app->activeApplicationWindow() == 0 );

    DuiApplicationWindow *appWin = new DuiApplicationWindow;
    QVERIFY( app->activeApplicationWindow() == appWin );

    delete appWin;
    QVERIFY( app->activeApplicationWindow() == 0 );

    delete app;
}

void Ut_DuiApplication::testApplicationWindows()
{
    app = buildApp( 2, "appName -software" );
    DuiApplicationWindow *appWin1 = new DuiApplicationWindow;
    Q_UNUSED(appWin1);
    DuiApplicationWindow *appWin2 = new DuiApplicationWindow;
    DuiApplicationWindow *appWin3 = new DuiApplicationWindow;

    DuiApplicationWindow *appWin4 = new DuiApplicationWindow;
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
*/

void Ut_DuiApplication::testPrestartMode()
{
    // Test that mode is set when started with -prestart
    app = buildApp(2, "appName -prestart");
    app->setPrestartMode(Dui::TerminateOnClose);
    QCOMPARE(app->prestartMode(), Dui::TerminateOnClose);
    app->setPrestartMode(Dui::LazyShutdown);
    QCOMPARE(app->prestartMode(), Dui::LazyShutdown);
    app->setPrestartMode(Dui::TerminateOnCloseMultiWindow);
    QCOMPARE(app->prestartMode(), Dui::TerminateOnCloseMultiWindow);
    app->setPrestartMode(Dui::LazyShutdownMultiWindow);
    QCOMPARE(app->prestartMode(), Dui::LazyShutdownMultiWindow);
    delete app;

    // Test that mode is not set when not started with -prestart
    app = buildApp(1, "appName");
    app->setPrestartMode(Dui::TerminateOnClose);
    QCOMPARE(app->prestartMode(), Dui::NoPrestart);
    delete app;
}

void Ut_DuiApplication::testReleasePrestartModeTerminateOnClose()
{
    // Test that app releases correctly from prestart
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartReleased()), this, SLOT(prestartReleased()));
    app->setPrestartMode(Dui::TerminateOnClose);
    service->launch();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was sent
    QCOMPARE(m_prestartReleased, true);
    // Test that the handler is called
    QCOMPARE(m_releasePrestart, true);
    delete app;
}

void Ut_DuiApplication::testReleasePrestartModeLazyShutdown()
{
    // Test that app releases correctly from prestart
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartReleased()), this, SLOT(prestartReleased()));
    app->setPrestartMode(Dui::LazyShutdown);
    service->launch();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was sent
    QCOMPARE(m_prestartReleased, true);
    // Test that the handler is called
    QCOMPARE(m_releasePrestart, true);
    delete app;
}

void Ut_DuiApplication::testReleasePrestartModeTerminateOnCloseMultiWindow()
{
    // Test that app releases correctly from prestart
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartReleased()), this, SLOT(prestartReleased()));
    app->setPrestartMode(Dui::TerminateOnCloseMultiWindow);
    service->launch();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was sent
    QCOMPARE(m_prestartReleased, true);
    // Test that the handler is called
    QCOMPARE(m_releasePrestart, true);
    delete app;
}

void Ut_DuiApplication::testReleasePrestartModeLazyShutdownMultiWindow()
{
    // Test that app releases correctly from prestart
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartReleased()), this, SLOT(prestartReleased()));
    app->setPrestartMode(Dui::LazyShutdownMultiWindow);
    service->launch();
    // Test that we have the correct mode set
    QCOMPARE(app->isPrestarted(), false);
    // Test that the signal was sent
    QCOMPARE(m_prestartReleased, true);
    // Test that the handler is called
    QCOMPARE(m_releasePrestart, true);
    delete app;
}

void Ut_DuiApplication::testRestorePrestartModeLazyShutdown()
{
    // Test that app restores correctly to prestart
    // if in LazyShutdown mode
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartRestored()), this, SLOT(prestartRestored()));
    app->setPrestartMode(Dui::LazyShutdown);
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

void Ut_DuiApplication::testRestorePrestartModeTerminateOnClose()
{
    // Test that app won't restore to prestart
    // if not in LazyShutdown mode
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartRestored()), this, SLOT(prestartRestored()));
    app->setPrestartMode(Dui::TerminateOnClose);
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

void Ut_DuiApplication::testRestorePrestartModeLazyShutdownMultiWindow()
{
    // Test that app restores correctly to prestart
    // if in LazyShutdown mode
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartRestored()), this, SLOT(prestartRestored()));
    app->setPrestartMode(Dui::LazyShutdownMultiWindow);
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

void Ut_DuiApplication::testRestorePrestartModeTerminateOnCloseMultiWindow()
{
    // Test that app won't restore to prestart
    // if not in LazyShutdown mode
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->connect(app, SIGNAL(prestartRestored()), this, SLOT(prestartRestored()));
    app->setPrestartMode(Dui::TerminateOnCloseMultiWindow);
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

void Ut_DuiApplication::testIsPrestartedLazyShutdown()
{
    // Test that app restores correctly to prestart
    // if in LazyShutdown mode
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(Dui::LazyShutdown);
    QCOMPARE(app->isPrestarted(), true);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), true);
    delete app;
}

void Ut_DuiApplication::testIsPrestartedTerminateOnClose()
{
    // Test that app won't restore to prestart
    // if not in LazyShutdown mode
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(Dui::TerminateOnClose);
    QCOMPARE(app->isPrestarted(), true);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), false);
    delete app;
}

void Ut_DuiApplication::testIsPrestartedLazyShutdownMultiWindow()
{
    // Test that app restores correctly to prestart
    // if in LazyShutdown mode
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(Dui::LazyShutdownMultiWindow);
    QCOMPARE(app->isPrestarted(), true);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), true);
    delete app;
}

void Ut_DuiApplication::testIsPrestartedTerminateOnCloseMultiWindow()
{
    // Test that app won't restore to prestart
    // if not in LazyShutdown mode
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(Dui::TerminateOnCloseMultiWindow);
    QCOMPARE(app->isPrestarted(), true);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), false);
    delete app;
}


void Ut_DuiApplication::testIsPrestartedNoPrestart()
{
    // Test that app won't enter prestart
    // if -prestart not set
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(1, "appName", service);
    QCOMPARE(app->isPrestarted(), false);
    app->setPrestartMode(Dui::TerminateOnClose);
    QCOMPARE(app->isPrestarted(), false);
    service->launch();
    QCOMPARE(app->isPrestarted(), false);
    service->close();
    QCOMPARE(app->isPrestarted(), false);
    delete app;
}

void Ut_DuiApplication::testPrestartedProperty()
{
    DuiApplicationService *service = new DuiApplicationService("com.nokia.appName");
    app = buildPrestartApp(2, "appName -prestart", service);
    app->setPrestartMode(Dui::LazyShutdown);
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


void Ut_DuiApplication::prestartReleased()
{
    m_prestartReleased = true;
}

void Ut_DuiApplication::prestartRestored()
{
    m_prestartRestored = true;
}

QTEST_APPLESS_MAIN(Ut_DuiApplication);
