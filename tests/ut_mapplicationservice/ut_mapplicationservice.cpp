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
#include <MComponentData>

#include "ut_mapplicationservice.h"

// has to be included last
#include "mapplication_p.h"

QString     Ut_MApplicationService::appName;
QStringList Ut_MApplicationService::arguments;
char   *Ut_MApplicationService::argv[MAX_PARAMS];
int     Ut_MApplicationService::argc;
bool    Ut_MApplicationService::programArgsCorrect = false;
bool    Ut_MApplicationService::programStarted = false;
bool    Ut_MApplicationService::allowRegisterService  = true;
bool    Ut_MApplicationService::allowRegisterService2 = true;
int     Ut_MApplicationService::failRegisterServiceTimes = 1;
int     Ut_MApplicationService::failRegisterServiceCounter = 0;
QString Ut_MApplicationService::registeredService("notSet");
bool    Ut_MApplicationService::applicationExited = false;
bool    Ut_MApplicationService::activeWindowSet = false;
bool    Ut_MApplicationService::windowActivated = false;
bool    Ut_MApplicationService::windowRaised = false;
bool    Ut_MApplicationService::windowClosed = false;
bool    Ut_MApplicationService::windowShown = false;

M::PrestartMode MComponentData::prestartMode()
{
    return M::NoPrestart;
}

QString MComponentData::appName()
{
    return "ut_mapplicationservice";
}

QString MComponentData::binaryName()
{
    return "ut_mapplicationservice";
}

QDBusPendingReply<> MApplicationIfProxy::launch()
{
    return QDBusPendingReply<>();
}

void MApplicationPrivate::stdExit(int status)
{
    Ut_MApplicationService::applicationExited = true;
    Q_UNUSED(status);
    // don't exit - need to carry on to complete other tests
}

QString MApplicationServicePrivate::appName()
{
    return QString(Ut_MApplicationService::argv[0]);
}

bool MApplicationServicePrivate::thisAppRunWithDBus()
{
    return true;
}

bool Ut_MApplicationService::isPrestartedReturnValue = false;
bool MApplicationServicePrivate::isPrestarted()
{
    return Ut_MApplicationService::isPrestartedReturnValue;
}

bool Ut_MApplicationService::isLazyShutdownReturnValue = false;
bool MApplicationServicePrivate::prestartModeIsLazyShutdown()
{
    return Ut_MApplicationService::isLazyShutdownReturnValue;
}

bool Ut_MApplicationService::prestartReleased = false;
void MApplicationServicePrivate::releasePrestart()
{
    Ut_MApplicationService::prestartReleased = true;
}

bool Ut_MApplicationService::prestartRestored = false;
void MApplicationServicePrivate::restorePrestart()
{
    Ut_MApplicationService::prestartRestored = true;
}

bool MApplicationServicePrivate::activeWindowSet()
{
    return Ut_MApplicationService::activeWindowSet;
}

void MApplicationServicePrivate::activateActiveWindow()
{
    Ut_MApplicationService::windowActivated = true;
}

void MApplicationServicePrivate::raiseActiveWindow()
{
    Ut_MApplicationService::windowRaised = true;
}

QStringList MApplicationServicePrivate::arguments()
{
    return Ut_MApplicationService::arguments;
}

QString MApplicationServicePrivate::binaryName()
{
    return Ut_MApplicationService::argv[ 0 ];
}

void MApplicationServicePrivate::launchNewProcess(const QString &binaryName, const QStringList &arguments)
{
    Ut_MApplicationService::programStarted = (binaryName == Ut_MApplicationService::argv[0]);

    // check argc is correct
    int argumentCountShouldBe = Ut_MApplicationService::argc - 1;
    int argumentCountIs = arguments.count();
    Ut_MApplicationService::programArgsCorrect = (argumentCountIs == argumentCountShouldBe);

    // check argvs are correct
    for (int thisArg = 0; thisArg < arguments.count(); ++thisArg) {
        QString isThis       = arguments.at(thisArg);
        QString shouldBeThis = QString(Ut_MApplicationService::argv[ thisArg+1 ]);
        Ut_MApplicationService::programArgsCorrect &= (isThis == shouldBeThis);
    }
}

bool MApplicationServicePrivate::registerService(const QString &name)
{
    bool retVal = false;

    if (Ut_MApplicationService::allowRegisterService) {
        retVal = true;
        Ut_MApplicationService::registeredService = name;
    } else {
        if (Ut_MApplicationService::allowRegisterService2) {
            if (Ut_MApplicationService::failRegisterServiceCounter <
                    Ut_MApplicationService::failRegisterServiceTimes) {
                Ut_MApplicationService::failRegisterServiceCounter++;
            } else {
                retVal = true;
                Ut_MApplicationService::registeredService = name;
                Ut_MApplicationService::failRegisterServiceCounter = 0;
            }
        }
    }

    qDebug() << "MApplicationServicePrivate::registerService() returning " << retVal;
    return retVal;
}

void MApplicationServicePrivate::registerObject(const QString &path, QObject *object)
{
    Q_UNUSED(path);
    Q_UNUSED(object);
}

void MApplicationServicePrivate::unregisterObject(const QString &path)
{
    Q_UNUSED(path);
}
void MApplicationServicePrivate::stdExit(int exitValue)
{
    MApplicationPrivate::stdExit(exitValue);
}
void MApplicationServicePrivate::closeAllWindows()
{
    mApp->closeAllWindows();
}

MApplicationServicePrivate::MApplicationServicePrivate(const QString &newServiceName):
    serviceName(newServiceName),
    registered(false),
    instanceCounter(0),
    q_ptr(0),
    pDBusConnection(0),
    mApp(MApplication::instance())
{
}

MApplicationServicePrivate::~MApplicationServicePrivate()
{
}

void Ut_MApplicationService::init()
{
    m_subject = new MApplicationService("ut_mapplicationservice");
    //new MComponentData( m_subject );
    isPrestartedReturnValue = false;
    isLazyShutdownReturnValue = false;
}

void Ut_MApplicationService::cleanup()
{
    delete m_subject;
}

void Ut_MApplicationService::initTestCase()
{
    argc = 2;
    argv[0] =  strdup("ut_mapplicationservice");
    argv[1] =  strdup("-software");
    appName = QString(argv[0]);
    for (int arg = 0; arg < argc; ++arg) {
        arguments << argv[ arg ];
    }
}

void Ut_MApplicationService::cleanupTestCase()
{
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
}

void Ut_MApplicationService::plainLaunch()
{
    Ut_MApplicationService::allowRegisterService = true;
    Ut_MApplicationService::allowRegisterService2 = true;
    windowRaised = false;
    windowActivated = false;

    // without window
    activeWindowSet = false;
    m_subject->launch();
    QCOMPARE(Ut_MApplicationService::windowRaised, false);
    QCOMPARE(Ut_MApplicationService::windowActivated, false);

    // with window
    activeWindowSet = true;
    m_subject->launch();
    QCOMPARE(Ut_MApplicationService::windowRaised, true);
    QCOMPARE(Ut_MApplicationService::windowActivated, true);
}

void Ut_MApplicationService::launchWithRegistrationFailure()
{
    Ut_MApplicationService::allowRegisterService = false;
    Ut_MApplicationService::allowRegisterService2 = false;
    windowRaised = false;
    windowActivated = false;

    // without window
    activeWindowSet = false;
    m_subject->launch();
    QCOMPARE(Ut_MApplicationService::windowRaised, false);
    QCOMPARE(Ut_MApplicationService::windowActivated, false);

    // with window
    activeWindowSet = true;
    m_subject->launch();
    QCOMPARE(Ut_MApplicationService::windowRaised, true);
    QCOMPARE(Ut_MApplicationService::windowActivated, true);
}

void Ut_MApplicationService::launchAnotherWithQProcess()
{
    Ut_MApplicationService::allowRegisterService = true;
    Ut_MApplicationService::allowRegisterService2 = true;

    // MApplication constructor calls this once
    m_subject->launchAnotherWithQProcess();
    QCOMPARE(Ut_MApplicationService::programArgsCorrect, false);
    QCOMPARE(Ut_MApplicationService::programStarted, false);

    // another called from launch
    m_subject->launchAnotherWithQProcess();
    QCOMPARE(Ut_MApplicationService::programArgsCorrect, true);
    QCOMPARE(Ut_MApplicationService::programStarted, true);
}

void Ut_MApplicationService::incrementAndRegisterOneFail()
{
    Ut_MApplicationService::allowRegisterService = false;
    Ut_MApplicationService::allowRegisterService2 = true;
    Ut_MApplicationService::failRegisterServiceTimes = 1;

    m_subject->incrementAndRegister();
    QCOMPARE(Ut_MApplicationService::registeredService, QString("com.nokia.ut_mapplicationservice2"));
}

void Ut_MApplicationService::incrementAndRegisterTenFail()
{
    Ut_MApplicationService::allowRegisterService = false;
    Ut_MApplicationService::allowRegisterService2 = true;
    Ut_MApplicationService::failRegisterServiceTimes = 10;

    m_subject->incrementAndRegister();
    QCOMPARE(Ut_MApplicationService::registeredService, QString("com.nokia.ut_mapplicationservice11"));
}

void Ut_MApplicationService::misc()
{
    Ut_MApplicationService::allowRegisterService = false;
    windowRaised = false;
    windowActivated = false;

    m_subject->close();
    m_subject->exit();
}

void Ut_MApplicationService::prestartLaunch()
{
    isPrestartedReturnValue = true;
    prestartReleased = false;
    m_subject->launch();
    QVERIFY(prestartReleased == true);
}

void Ut_MApplicationService::prestartLaunchNoPrestart()
{
    prestartReleased = false;
    m_subject->launch();
    QVERIFY(prestartReleased == false);
}

void Ut_MApplicationService::prestartCloseLazyShutdown()
{
    prestartRestored = false;
    isLazyShutdownReturnValue = true;
    m_subject->close();
    QVERIFY(prestartRestored == true);
}

void Ut_MApplicationService::prestartCloseNoLazyShutdown()
{
    prestartRestored = false;
    m_subject->close();
    QVERIFY(prestartRestored == false);
}

void Ut_MApplicationService::prestartExitLazyShutdown()
{
    prestartRestored = false;
    isLazyShutdownReturnValue = true;
    m_subject->exit();
    QVERIFY(prestartRestored == true);
}

void Ut_MApplicationService::prestartExitNoLazyShutdown()
{
    prestartRestored = false;
    m_subject->exit();
    QVERIFY(prestartRestored == false);
}


QTEST_MAIN(Ut_MApplicationService)
