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

#include <QObject>
#include <MDebug>
#include <MApplication>
#include <MApplicationService>
#include <MApplicationIfProxy>

#include "ut_mapplicationifproxy.h"

#define NUMBER_OF_PARAMETERS 2

class MyApplicationService: public MApplicationService
{
public:
    MyApplicationService(QObject *parent = 0) :
        MApplicationService("com.nokia.ut_mapplicationifproxy", parent) {
    }

    void launch() {
        mDebug("MyApplicationService::launch") << "launch call received";
        Ut_MApplicationIfProxy::launchCalled = true;
    }

    void launch(const QStringList &parameters) {
        Q_UNUSED(parameters);
        mDebug("MyApplicationService::launch") << "launch call received with parameters";
        if (parameters.count() == NUMBER_OF_PARAMETERS)
            Ut_MApplicationIfProxy::launchCalledWithParams = true;
    }

    void close() {
        mDebug("MyApplicationService::close") << "close call received";
        Ut_MApplicationIfProxy::closeCalled = true;
    }

    void exit() {
        mDebug("MyApplicationService::exit") << "exit call received";
        Ut_MApplicationIfProxy::exitCalled = true;
    }
};


bool    Ut_MApplicationIfProxy::launchCalled = false;
bool    Ut_MApplicationIfProxy::launchCalledWithParams = false;
bool    Ut_MApplicationIfProxy::exitCalled = false;
bool    Ut_MApplicationIfProxy::closeCalled = false;

void Ut_MApplicationIfProxy::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplicationifproxy" };
    app = new MApplication(argc, app_name, new MyApplicationService());

    subject = new MApplicationIfProxy("com.nokia.ut_mapplicationifproxy");
}

void Ut_MApplicationIfProxy::cleanupTestCase()
{
    delete subject;
    delete app;
}

void Ut_MApplicationIfProxy::testLaunch()
{
    subject->launch();
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 1000);

    QVERIFY(launchCalled == true);
}

void Ut_MApplicationIfProxy::testLaunchWithParams()
{
    QStringList params;
    for (int i = NUMBER_OF_PARAMETERS; i > 0; --i)
        params << QString("Param %1") + QString::number(i);

    subject->launch(params);
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 1000);

    QVERIFY(launchCalledWithParams == true);
}

void Ut_MApplicationIfProxy::testClose()
{
    subject->close();
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 1000);

    QVERIFY(closeCalled == true);
}

void Ut_MApplicationIfProxy::testExit()
{
    subject->exit();
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 1000);

    QVERIFY(exitCalled == true);
}

QTEST_APPLESS_MAIN(Ut_MApplicationIfProxy)
