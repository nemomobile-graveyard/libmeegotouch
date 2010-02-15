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

#include "ut_duinotification.h"
#include "duinotification.h"
#include <duinotificationmanager_stub.h>
#include <duiremoteaction_stub.h>
#include <duiaction_stub.h>
#include <duinotificationgroup_stub.h>

void Ut_DuiNotification::initTestCase()
{
}

void Ut_DuiNotification::cleanupTestCase()
{
}

void Ut_DuiNotification::init()
{
}

void Ut_DuiNotification::cleanup()
{
}

void Ut_DuiNotification::testGettingAllNotifications()
{
    QList<uint> idList;
    idList << 1 << 5 << 3 << 42 << 100;
    gDefaultDuiNotificationManagerStub.stubSetReturnValue("notificationIdList", idList);

    QList<DuiNotification *> notificationList = DuiNotification::notifications();
    QCOMPARE(notificationList.count(), idList.count());
    foreach(DuiNotification * notification, notificationList) {
        QVERIFY(idList.contains(notification->id()));
    }
}

QTEST_APPLESS_MAIN(Ut_DuiNotification)
