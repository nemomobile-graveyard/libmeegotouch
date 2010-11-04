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

#include "ut_mnotification.h"
#include "mnotification.h"
#include <mnotificationmanager_stub.h>
#include <mremoteaction_stub.h>
#include <maction_stub.h>
#include <mnotificationgroup_stub.h>

// Subclasses to gain access to the IDs
class TestNotification : public MNotification
{
public:
    TestNotification(uint id);
    uint id() const;
};

TestNotification::TestNotification(uint id) : MNotification(id)
{
}

uint TestNotification::id() const
{
    return MNotification::id();
}

void Ut_MNotification::initTestCase()
{
}

void Ut_MNotification::cleanupTestCase()
{
}

void Ut_MNotification::init()
{
}

void Ut_MNotification::cleanup()
{
}

void Ut_MNotification::testGettingAllNotifications()
{
    QList<MNotification> notifications;
    notifications.append(TestNotification(1));
    notifications.append(TestNotification(5));
    notifications.append(TestNotification(3));
    notifications.append(TestNotification(42));
    notifications.append(TestNotification(100));
    gDefaultMNotificationManagerStub.stubSetReturnValue("notificationListWithIdentifiers", notifications);

    QList<MNotification *> notificationList = MNotification::notifications();
    QCOMPARE(notificationList.count(), notifications.count());
    foreach(MNotification *notification, notificationList) {
        bool found = false;
        foreach(const MNotification &n, notifications) {
            if (static_cast<TestNotification *>(notification)->id() == static_cast<const TestNotification &>(n).id()) {
                found = true;
                break;
            }
        }
        QVERIFY(found);
    }
}

QTEST_APPLESS_MAIN(Ut_MNotification)
