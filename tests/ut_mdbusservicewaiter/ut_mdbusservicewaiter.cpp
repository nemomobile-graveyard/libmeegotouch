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
#include <QDBusConnection>

#include "mdbusservicewaiter_p.h"
#include "ut_mdbusservicewaiter.h"

void Ut_MDBusServiceWaiter::initTestCase()
{
}

void Ut_MDBusServiceWaiter::cleanupTestCase()
{
}

void Ut_MDBusServiceWaiter::testFailure()
{
    QString invalidService("com.nokia.not*valid");

    MDBusServiceWaiter serviceWaiter(invalidService);
    serviceWaiter.setTimeout(1000);
    QDBusConnection::sessionBus().registerService(invalidService);

    QVERIFY(!serviceWaiter.waitForRegistration());
}

void Ut_MDBusServiceWaiter::testSuccess()
{
    QString validService("com.nokia.valid");

    MDBusServiceWaiter serviceWaiter(validService);
    serviceWaiter.setTimeout(1000);
    QDBusConnection::sessionBus().registerService(validService);

    QVERIFY(serviceWaiter.waitForRegistration());

    QDBusConnection::sessionBus().unregisterService(validService);
}

QTEST_MAIN(Ut_MDBusServiceWaiter)
