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

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QDBusConnection>
#include <QDBusPendingReply>
#include <QProcess>
#include <QDebug>

#include <MApplication>
#include <MApplicationService>
#include "mapplicationservice_p.h"
#include <MApplicationWindow>
#include <MApplicationIfProxy>

#include "ft_prestart.h"

// has to be included last
#include "mapplication_p.h"

char   *Ft_Prestart::argv[MAX_PARAMS];
int     Ft_Prestart::argc;
bool    Ft_Prestart::programArgsCorrect = false;
bool    Ft_Prestart::programStarted = false;
bool    Ft_Prestart::allowRegisterService  = true;
bool    Ft_Prestart::allowRegisterService2 = true;
int     Ft_Prestart::failRegisterServiceTimes = 1;
int     Ft_Prestart::failRegisterServiceCounter = 0;
QString Ft_Prestart::registeredService("notSet");
bool    Ft_Prestart::applicationExited = false;
bool    Ft_Prestart::activeWindowSet = false;
bool    Ft_Prestart::windowActivated = false;
bool    Ft_Prestart::windowRaised = false;
bool    Ft_Prestart::windowClosed = false;
bool    Ft_Prestart::windowShown = false;

QDBusPendingReply<> MApplicationIfProxy::launch()
{
    return QDBusPendingReply<>();
}

bool MApplicationServicePrivate::registerService(const QString &name)
{
    bool retVal = false;

    if (Ft_Prestart::allowRegisterService) {
        retVal = true;
        Ft_Prestart::registeredService = name;
    } else {
        if (Ft_Prestart::allowRegisterService2) {
            if (Ft_Prestart::failRegisterServiceCounter <
                    Ft_Prestart::failRegisterServiceTimes) {
                Ft_Prestart::failRegisterServiceCounter++;
            } else {
                retVal = true;
                Ft_Prestart::registeredService = name;
                Ft_Prestart::failRegisterServiceCounter = 0;
            }
        }
    }

    qDebug() << "MApplicationServicePrivate::registerService() returning " << retVal;
    return retVal;
}

void QWidget::activateWindow()
{
    Ft_Prestart::windowActivated = true;
}

void QWidget::raise()
{
    Ft_Prestart::windowRaised = true;
}

void QApplication::closeAllWindows()
{
    Ft_Prestart::windowClosed = true;
}

void QWidget::setVisible(bool visible)
{
    Ft_Prestart::windowShown = visible;
}

void MApplicationPrivate::stdExit(int status)
{
    Ft_Prestart::applicationExited = true;
    Q_UNUSED(status);
    // don't exit - need to carry on to complete other tests
}

void Ft_Prestart::initGlobals()
{
    programArgsCorrect = false;
    programStarted = false;
    allowRegisterService  = true;
    allowRegisterService2 = true;
    failRegisterServiceTimes = 1;
    failRegisterServiceCounter = 0;
    registeredService = QString("notSet");
    applicationExited = false;
    activeWindowSet = false;
    windowActivated = false;
    windowRaised = false;
    windowClosed = false;
    windowShown = false;
}

void Ft_Prestart::init()
{
    argc = 2;
    argv[0] =  strdup("ft_prestart");
    argv[1] =  strdup("-software");
    m_mApp = 0;
    initGlobals();
}

void Ft_Prestart::cleanup()
{
    delete m_mApp;
    m_mApp = 0;
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
}

void Ft_Prestart::initTestCase()
{
}

void Ft_Prestart::cleanupTestCase()
{
}

void Ft_Prestart::buildPrestartedApp()
{
    m_subject = new MApplicationService("ft_prestart");
    if (argc < MAX_PARAMS) {
        argv[argc] = strdup("-prestart");
        argc++;
    }
    m_mApp = new MApplication(argc, argv, m_subject);
    windowRaised = false;
    windowActivated = false;
    windowShown = false;
    windowClosed = false;
    applicationExited = false;
}

void Ft_Prestart::PrestartLaunchTerminateOnClose()
{
    Ft_Prestart::allowRegisterService = true;
    Ft_Prestart::allowRegisterService2 = true;
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::TerminateOnClose);

    MApplicationWindow *window = new MApplicationWindow();

    // window.show should not have any effect in prestarted mode
    window->show();

    // Application in prestarted state, no window visible
    QCOMPARE(Ft_Prestart::windowRaised, false);
    QCOMPARE(Ft_Prestart::windowActivated, false);
    QCOMPARE(Ft_Prestart::windowShown, false);

    // dbus launch, window should be activated
    m_subject->launch();
    QCOMPARE(Ft_Prestart::windowRaised, true);
    QCOMPARE(Ft_Prestart::windowActivated, true);
    QCOMPARE(Ft_Prestart::windowShown, true);
}

void Ft_Prestart::closePrestartedLazyShutdownApplication()
{
    Ft_Prestart::allowRegisterService = false;
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::LazyShutdown);

    MApplicationWindow *window = new MApplicationWindow();
    window->show();

    // Launch prestarted application
    m_subject->launch();
    QCOMPARE(Ft_Prestart::windowShown, true);

    // Close prestarted application, window should be hidden and application still in prestarted-mode
    m_subject->close();
    QCOMPARE(Ft_Prestart::windowShown, false);
    QCOMPARE(m_mApp->isPrestarted(), true);

    // Re-launch application, window should be shown
    m_subject->launch();
    QCOMPARE(Ft_Prestart::windowShown, true);
}

void Ft_Prestart::exitPrestartedLazyShutdownApplication()
{
    Ft_Prestart::allowRegisterService = false;
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::LazyShutdown);

    MApplicationWindow *window = new MApplicationWindow();
    window->show();

    // Launch prestarted application
    m_subject->launch();
    QCOMPARE(Ft_Prestart::windowShown, true);

    // Close prestarted application, window should be hidden and application still in prestarted-mode
    m_subject->exit();
    QCOMPARE(Ft_Prestart::windowShown, false);
    QCOMPARE(m_mApp->isPrestarted(), true);

    // Re-launch application, window should be shown
    m_subject->launch();
    QCOMPARE(Ft_Prestart::windowShown, true);
}

void Ft_Prestart::closePrestartedNoPrestartApplication()
{
    Ft_Prestart::allowRegisterService = false;
    buildPrestartedApp();
    QCOMPARE(m_mApp->prestartMode(), M::NoPrestart);

    MApplicationWindow window;
    window.show();
    // Launch prestarted application without prestartmode set
    m_subject->launch();

    QCOMPARE(Ft_Prestart::windowRaised, true);
    QCOMPARE(Ft_Prestart::windowShown, true);
    QCOMPARE(Ft_Prestart::windowClosed, false);

    // Close prestarted application, window should be hidden and application still in prestarted-mode
    m_subject->close();
    QCOMPARE(Ft_Prestart::windowClosed, true);
    QCOMPARE(m_mApp->isPrestarted(), false);
}

void Ft_Prestart::exitPrestartedNoPrestartApplication()
{
    Ft_Prestart::allowRegisterService = false;
    buildPrestartedApp();
    QCOMPARE(m_mApp->prestartMode(), M::NoPrestart);

    MApplicationWindow window;
    window.show();
    // Launch prestarted application without prestartmode set
    m_subject->launch();

    QCOMPARE(Ft_Prestart::windowRaised, true);
    QCOMPARE(Ft_Prestart::windowShown, true);
    QCOMPARE(Ft_Prestart::applicationExited, false);

    // Exit prestarted application, window should be hidden and application still in prestarted-mode
    m_subject->exit();
    QCOMPARE(Ft_Prestart::applicationExited, true);
    QCOMPARE(m_mApp->isPrestarted(), false);
}

void Ft_Prestart::closePrestartedTerminateOnCloseApplication()
{
    Ft_Prestart::allowRegisterService = false;
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::TerminateOnClose);

    MApplicationWindow window;
    window.show();
    // Launch prestarted application without prestartmode set
    m_subject->launch();

    QCOMPARE(Ft_Prestart::windowRaised, true);
    QCOMPARE(Ft_Prestart::windowShown, true);
    QCOMPARE(Ft_Prestart::windowClosed, false);

    // Close prestarted application, window should be hidden and application still in prestarted-mode
    m_subject->close();
    QCOMPARE(Ft_Prestart::windowClosed, true);
    QCOMPARE(m_mApp->isPrestarted(), false);
}

void Ft_Prestart::exitPrestartedTerminateOnCloseApplication()
{
    Ft_Prestart::allowRegisterService = false;
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::TerminateOnClose);

    MApplicationWindow window;
    window.show();
    // Launch prestarted application without prestartmode set
    m_subject->launch();

    QCOMPARE(Ft_Prestart::windowRaised, true);
    QCOMPARE(Ft_Prestart::windowShown, true);
    QCOMPARE(Ft_Prestart::applicationExited, false);

    // Exit prestarted application, window should be hidden and application still in prestarted-mode
    m_subject->exit();
    QCOMPARE(Ft_Prestart::applicationExited, true);
    QCOMPARE(m_mApp->isPrestarted(), false);
}



// IMPORTED FROM UT_MAPPLICATIONWINDOW

void Ft_Prestart::testPrestartedLazyShutdownShow()
{
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::LazyShutdown);
    QCOMPARE(Ft_Prestart::windowShown, false);
    m_mWin->show();
    // Show should not be enough to show application which is in prestarted state
    QCOMPARE(Ft_Prestart::windowShown, false);
}

void Ft_Prestart::testPrestartedTerminateOnCloseShow()
{
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::TerminateOnClose);
    QCOMPARE(Ft_Prestart::windowShown, false);
    m_mWin->show();
    // Show should not be enough to show application which is in prestarted state
    QCOMPARE(Ft_Prestart::windowShown, false);
}

void Ft_Prestart::testPrestartedNoPrestartShow()
{
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::NoPrestart);
    QCOMPARE(Ft_Prestart::windowShown, false);
    m_mWin->show();
    QCOMPARE(Ft_Prestart::windowShown, true);
}

void Ft_Prestart::testPrestartedNoPrestartClose()
{
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::NoPrestart);
    QCOMPARE(Ft_Prestart::windowShown, false);

    m_mWin->show();
    QCOMPARE(Ft_Prestart::windowShown, true);
    QCOMPARE(Ft_Prestart::windowClosed, false);

    m_subject->close();
    QCOMPARE(Ft_Prestart::windowClosed, true);
    QCOMPARE(m_mApp->isPrestarted(), false);
}

void Ft_Prestart::testPrestartedLazyShutdownClose()
{
    MApplicationService *m_subject = new MApplicationService("com.nokia.myApp");
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::LazyShutdown);

    // Simulate launch using D-Bus
    m_subject->launch();

    QCOMPARE(Ft_Prestart::windowShown, true);

    // Close the window
    m_subject->close();

    // Window should not be really closed after close if LazyShutdown
    QCOMPARE(Ft_Prestart::windowClosed, false);
    QCOMPARE(Ft_Prestart::windowShown, false);
    QCOMPARE(m_mApp->isPrestarted(), true);

    // Simulate launch using D-Bus
    m_subject->launch();

    QCOMPARE(Ft_Prestart::windowShown, true);
}

void Ft_Prestart::testPrestartedTerminateOnCloseClose()
{
    MApplicationService *m_subject = new MApplicationService("com.nokia.myApp");
    buildPrestartedApp();
    m_mApp->setPrestartMode(M::TerminateOnClose);

    // Simulate launch using D-Bus
    m_subject->launch();
    QCOMPARE(Ft_Prestart::windowShown, true);

    // Close the window
    m_subject->close();

    QCOMPARE(Ft_Prestart::windowClosed, true);
    QCOMPARE(m_mApp->isPrestarted(), false);
}



QTEST_APPLESS_MAIN(Ft_Prestart)
