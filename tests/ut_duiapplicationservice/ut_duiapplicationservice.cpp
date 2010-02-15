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

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QDBusConnection>
#include <QDBusPendingReply>
#include <QProcess>
#include <QDebug>

#include <DuiApplication>
#include <DuiApplicationService>
#include "duiapplicationservice_p.h"
#include <DuiApplicationWindow>
#include <DuiApplicationIfProxy>
#include <DuiComponentData>

#include "ut_duiapplicationservice.h"

// has to be included last
#include "duiapplication_p.h"

QString     Ut_DuiApplicationService::appName;
QStringList Ut_DuiApplicationService::arguments;
char   *Ut_DuiApplicationService::argv[MAX_PARAMS];
int     Ut_DuiApplicationService::argc;
bool    Ut_DuiApplicationService::programArgsCorrect = false;
bool    Ut_DuiApplicationService::programStarted = false;
bool    Ut_DuiApplicationService::allowRegisterService  = true;
bool    Ut_DuiApplicationService::allowRegisterService2 = true;
int     Ut_DuiApplicationService::failRegisterServiceTimes = 1;
int     Ut_DuiApplicationService::failRegisterServiceCounter = 0;
QString Ut_DuiApplicationService::registeredService("notSet");
bool    Ut_DuiApplicationService::applicationExited = false;
bool    Ut_DuiApplicationService::activeWindowSet = false;
bool    Ut_DuiApplicationService::windowActivated = false;
bool    Ut_DuiApplicationService::windowRaised = false;
bool    Ut_DuiApplicationService::windowClosed = false;
bool    Ut_DuiApplicationService::windowShown = false;

DuiWindow::DuiWindow(QWidget *):
    d_ptr(0)
{
}

DuiWindow *DuiComponentData::activeWindow()
{
    QWidget *qw = new QWidget();
    return new DuiWindow(qw);
}

Dui::PrestartMode DuiComponentData::prestartMode()
{
    return Dui::NoPrestart;
}

QString DuiComponentData::appName()
{
    return "ut_duiapplicationservice";
}

QString DuiComponentData::binaryName()
{
    return "ut_duiapplicationservice";
}

QDBusPendingReply<> DuiApplicationIfProxy::launch()
{
    return QDBusPendingReply<>();
}

void DuiApplicationPrivate::stdExit(int status)
{
    Ut_DuiApplicationService::applicationExited = true;
    Q_UNUSED(status);
    // don't exit - need to carry on to complete other tests
}

QString DuiApplicationServicePrivate::appName()
{
    return QString(Ut_DuiApplicationService::argv[0]);
}

bool DuiApplicationServicePrivate::thisAppRunWithDBus()
{
    return true;
}

bool Ut_DuiApplicationService::isPrestartedReturnValue = false;
bool DuiApplicationServicePrivate::isPrestarted()
{
    return Ut_DuiApplicationService::isPrestartedReturnValue;
}

bool Ut_DuiApplicationService::isLazyShutdownReturnValue = false;
bool DuiApplicationServicePrivate::prestartModeIsLazyShutdown()
{
    return Ut_DuiApplicationService::isLazyShutdownReturnValue;
}

bool Ut_DuiApplicationService::prestartReleased = false;
void DuiApplicationServicePrivate::releasePrestart()
{
    Ut_DuiApplicationService::prestartReleased = true;
}

bool Ut_DuiApplicationService::prestartRestored = false;
void DuiApplicationServicePrivate::restorePrestart()
{
    Ut_DuiApplicationService::prestartRestored = true;
}

bool DuiApplicationServicePrivate::activeWindowSet()
{
    return Ut_DuiApplicationService::activeWindowSet;
}

void DuiApplicationServicePrivate::activateActiveWindow()
{
    Ut_DuiApplicationService::windowActivated = true;
}

void DuiApplicationServicePrivate::raiseActiveWindow()
{
    Ut_DuiApplicationService::windowRaised = true;
}

QStringList DuiApplicationServicePrivate::arguments()
{
    return Ut_DuiApplicationService::arguments;
}

QString DuiApplicationServicePrivate::binaryName()
{
    return Ut_DuiApplicationService::argv[ 0 ];
}

void DuiApplicationServicePrivate::launchNewProcess(const QString &binaryName, const QStringList &arguments)
{
    Ut_DuiApplicationService::programStarted = (binaryName == Ut_DuiApplicationService::argv[0]);

    // check argc is correct
    int argumentCountShouldBe = Ut_DuiApplicationService::argc - 1;
    int argumentCountIs = arguments.count();
    Ut_DuiApplicationService::programArgsCorrect = (argumentCountIs == argumentCountShouldBe);

    // check argvs are correct
    for (int thisArg = 0; thisArg < arguments.count(); ++thisArg) {
        QString isThis       = arguments.at(thisArg);
        QString shouldBeThis = QString(Ut_DuiApplicationService::argv[ thisArg+1 ]);
        Ut_DuiApplicationService::programArgsCorrect &= (isThis == shouldBeThis);
    }
}

bool DuiApplicationServicePrivate::registerService(const QString &name)
{
    bool retVal = false;

    if (Ut_DuiApplicationService::allowRegisterService) {
        retVal = true;
        Ut_DuiApplicationService::registeredService = name;
    } else {
        if (Ut_DuiApplicationService::allowRegisterService2) {
            if (Ut_DuiApplicationService::failRegisterServiceCounter <
                    Ut_DuiApplicationService::failRegisterServiceTimes) {
                Ut_DuiApplicationService::failRegisterServiceCounter++;
            } else {
                retVal = true;
                Ut_DuiApplicationService::registeredService = name;
                Ut_DuiApplicationService::failRegisterServiceCounter = 0;
            }
        }
    }

    qDebug() << "DuiApplicationServicePrivate::registerService() returning " << retVal;
    return retVal;
}

void DuiApplicationServicePrivate::registerObject(const QString &path, QObject *object)
{
    Q_UNUSED(path);
    Q_UNUSED(object);
}

void DuiApplicationServicePrivate::unregisterObject(const QString &path)
{
    Q_UNUSED(path);
}

void Ut_DuiApplicationService::init()
{
    m_subject = new DuiApplicationService("ut_duiapplicationservice");
    //new DuiComponentData( m_subject );
    isPrestartedReturnValue = false;
    isLazyShutdownReturnValue = false;
}

void Ut_DuiApplicationService::cleanup()
{
    delete m_subject;
}

void Ut_DuiApplicationService::initTestCase()
{
    argc = 2;
    argv[0] =  strdup("ut_duiapplicationservice");
    argv[1] =  strdup("-software");
    appName = QString(argv[0]);
    for (int arg = 0; arg < argc; ++arg) {
        arguments << argv[ arg ];
    }
}

void Ut_DuiApplicationService::cleanupTestCase()
{
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
}

void Ut_DuiApplicationService::plainLaunch()
{
    Ut_DuiApplicationService::allowRegisterService = true;
    Ut_DuiApplicationService::allowRegisterService2 = true;
    windowRaised = false;
    windowActivated = false;

    // without window
    activeWindowSet = false;
    m_subject->launch();
    QCOMPARE(Ut_DuiApplicationService::windowRaised, false);
    QCOMPARE(Ut_DuiApplicationService::windowActivated, false);

    // with window
    activeWindowSet = true;
    m_subject->launch();
    QCOMPARE(Ut_DuiApplicationService::windowRaised, true);
    QCOMPARE(Ut_DuiApplicationService::windowActivated, true);
}

void Ut_DuiApplicationService::launchWithRegistrationFailure()
{
    Ut_DuiApplicationService::allowRegisterService = false;
    Ut_DuiApplicationService::allowRegisterService2 = false;
    windowRaised = false;
    windowActivated = false;

    // without window
    activeWindowSet = false;
    m_subject->launch();
    QCOMPARE(Ut_DuiApplicationService::windowRaised, false);
    QCOMPARE(Ut_DuiApplicationService::windowActivated, false);

    // with window
    activeWindowSet = true;
    m_subject->launch();
    QCOMPARE(Ut_DuiApplicationService::windowRaised, true);
    QCOMPARE(Ut_DuiApplicationService::windowActivated, true);
}

void Ut_DuiApplicationService::launchAnotherWithQProcess()
{
    Ut_DuiApplicationService::allowRegisterService = true;
    Ut_DuiApplicationService::allowRegisterService2 = true;

    // DuiApplication constructor calls this once
    m_subject->launchAnotherWithQProcess();
    QCOMPARE(Ut_DuiApplicationService::programArgsCorrect, false);
    QCOMPARE(Ut_DuiApplicationService::programStarted, false);

    // another called from launch
    m_subject->launchAnotherWithQProcess();
    QCOMPARE(Ut_DuiApplicationService::programArgsCorrect, true);
    QCOMPARE(Ut_DuiApplicationService::programStarted, true);
}

void Ut_DuiApplicationService::incrementAndRegisterOneFail()
{
    Ut_DuiApplicationService::allowRegisterService = false;
    Ut_DuiApplicationService::allowRegisterService2 = true;
    Ut_DuiApplicationService::failRegisterServiceTimes = 1;

    m_subject->incrementAndRegister();
    QCOMPARE(Ut_DuiApplicationService::registeredService, QString("com.nokia.ut_duiapplicationservice2"));
}

void Ut_DuiApplicationService::incrementAndRegisterTenFail()
{
    Ut_DuiApplicationService::allowRegisterService = false;
    Ut_DuiApplicationService::allowRegisterService2 = true;
    Ut_DuiApplicationService::failRegisterServiceTimes = 10;

    m_subject->incrementAndRegister();
    QCOMPARE(Ut_DuiApplicationService::registeredService, QString("com.nokia.ut_duiapplicationservice11"));
}

void Ut_DuiApplicationService::misc()
{
    Ut_DuiApplicationService::allowRegisterService = false;
    windowRaised = false;
    windowActivated = false;

    m_subject->close();
    m_subject->exit();
}

void Ut_DuiApplicationService::prestartLaunch()
{
    isPrestartedReturnValue = true;
    prestartReleased = false;
    m_subject->launch();
    QVERIFY(prestartReleased == true);
}

void Ut_DuiApplicationService::prestartLaunchNoPrestart()
{
    prestartReleased = false;
    m_subject->launch();
    QVERIFY(prestartReleased == false);
}

void Ut_DuiApplicationService::prestartCloseLazyShutdown()
{
    prestartRestored = false;
    isLazyShutdownReturnValue = true;
    m_subject->close();
    QVERIFY(prestartRestored == true);
}

void Ut_DuiApplicationService::prestartCloseNoLazyShutdown()
{
    prestartRestored = false;
    m_subject->close();
    QVERIFY(prestartRestored == false);
}

void Ut_DuiApplicationService::prestartExitLazyShutdown()
{
    prestartRestored = false;
    isLazyShutdownReturnValue = true;
    m_subject->exit();
    QVERIFY(prestartRestored == true);
}

void Ut_DuiApplicationService::prestartExitNoLazyShutdown()
{
    prestartRestored = false;
    m_subject->exit();
    QVERIFY(prestartRestored == false);
}


QTEST_MAIN(Ut_DuiApplicationService)
