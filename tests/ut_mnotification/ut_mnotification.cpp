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

#include "ut_mnotification.h"
#include "mnotification.h"
#include "mnotificationmanager_stub.h"
#include <mremoteaction_stub.h>
#include <maction_stub.h>
#include <mnotificationgroup_stub.h>

// MNotificationManager stub
const QString MNotificationManager::eventTypeKey = "eventType";
const QString MNotificationManager::summaryKey = "summary";
const QString MNotificationManager::bodyKey = "body";
const QString MNotificationManager::actionKey = "action";
const QString MNotificationManager::imageKey = "imageId";
const QString MNotificationManager::countKey = "count";
const QString MNotificationManager::identifierKey = "identifier";
const QString MNotificationManager::timestampKey = "timestamp";


QDateTime gCurrentDateTimeUtc = QDateTime();
QDateTime QDateTime::currentDateTimeUtc()
{
    return gCurrentDateTimeUtc;
}

QDBusArgument &operator<<(QDBusArgument &argument, const QVariantHash &parameters)
{
    Q_UNUSED(parameters);
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QVariantHash &parameters)
{
    Q_UNUSED(parameters);
    return argument;
}

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
    gCurrentDateTimeUtc = QDateTime();
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
    gDefaultMNotificationManagerStub.stubSetReturnValue("notificationList", notifications);

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

void Ut_MNotification::testThatTimestampOfNotificationIsUpdatedInPublish()
{
    MNotification notification;
    uint timestamp = 1234567890;
    gCurrentDateTimeUtc = QDateTime::fromTime_t(123456);

    QCOMPARE(notification.timestamp().isNull(), true);

    // Timestamp is not set and addNotification is successfull
    gDefaultMNotificationManagerStub.stubSetReturnValue("addNotification", (uint)1);
    notification.publish();
    QCOMPARE(notification.timestamp(), gCurrentDateTimeUtc);

    // Timestamp is set and updateNotification is successfull
    gDefaultMNotificationManagerStub.stubSetReturnValue("updateNotification", true);
    notification.setTimestamp(QDateTime::fromTime_t(timestamp));
    notification.publish();
    QCOMPARE(notification.timestamp(), QDateTime::fromTime_t(timestamp));

    // updateNotification is not succesfull, so timestamp is previously updated timestamp
    gDefaultMNotificationManagerStub.stubSetReturnValue("updateNotification", false);
    notification.publish();
    QCOMPARE(notification.timestamp(), QDateTime::fromTime_t(timestamp));

    // Timestamp set is null QDateTime and updateNotification is successfull
    gDefaultMNotificationManagerStub.stubSetReturnValue("updateNotification", true);
    notification.setTimestamp(QDateTime());
    notification.publish();
    QCOMPARE(notification.timestamp(), gCurrentDateTimeUtc);
}

QTEST_APPLESS_MAIN(Ut_MNotification)
