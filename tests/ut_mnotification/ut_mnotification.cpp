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
#include "metatypedeclarations.h"
#include <mremoteaction_stub.h>
#include <maction_stub.h>
#include <mnotificationgroup_stub.h>
#include <mnotificationmanagerproxy_stub.h>

// QDBusAbstractInterface stubs (used by MNotificationManagerProxy)
bool qDBusAbstractInterfaceCaptureCalls = false;
QList<QString> qDBusAbstractInterfaceAsyncCallMethods;
QList<QVariantList> qDBusAbstractInterfaceAsyncCallArguments;
QStringList qDBusPendingReplyArgumentsGetCapabilities;
QList<MNotification> qDBusPendingReplyArgumentsGetNotifications;
uint qDBusPendingReplyArgumentsNotify;
QVariantList qDBusPendingReplyArguments;
QDBusPendingCall QDBusAbstractInterface::asyncCallWithArgumentList(const QString &method, const QList<QVariant> &args)
{
    if (qDBusAbstractInterfaceCaptureCalls) {
        qDBusAbstractInterfaceAsyncCallMethods.append(method);
        qDBusAbstractInterfaceAsyncCallArguments.append(args);
    }

    qDBusPendingReplyArguments.clear();
    if (method == "GetCapabilities") {
        qDBusPendingReplyArguments.append(qDBusPendingReplyArgumentsGetCapabilities);
        return QDBusPendingReply<QStringList>();
    } else if (method == "GetNotifications") {
        qDBusPendingReplyArguments.append(QVariant::fromValue(qDBusPendingReplyArgumentsGetNotifications));
        return QDBusPendingReply<QList<MNotification> >();
    } else if (method == "Notify") {
        qDBusPendingReplyArguments.append(qDBusPendingReplyArgumentsNotify);
        return QDBusPendingReply<uint>();
    } else {
        return QDBusPendingReply<void>();
    }
}

QList<QString> qDBusAbstractInterfaceCallMethods;
QList<QVariantList> qDBusAbstractInterfaceCallArguments;
QDBusMessage QDBusAbstractInterface::callWithArgumentList(QDBus::CallMode, const QString &method, const QList<QVariant> &args)
{
    if (qDBusAbstractInterfaceCaptureCalls) {
        qDBusAbstractInterfaceCallMethods.append(method);
        qDBusAbstractInterfaceCallArguments.append(args);
    }

    return QDBusMessage();
}

// QDBusPendingReplyData stubs (used by MNotificationManagerProxy)
QVariant QDBusPendingReplyData::argumentAt(int index) const
{
    return qDBusPendingReplyArguments.at(index);
}

void QDBusPendingReplyData::setMetaTypes(int, const int *)
{
}

QDateTime gCurrentDateTimeUtc = QDateTime();
QDateTime QDateTime::currentDateTimeUtc()
{
    return gCurrentDateTimeUtc;
}

#if QT_VERSION < QT_VERSION_CHECK(4,8,0)
QDBusArgument &operator<<(QDBusArgument &argument, const QVariantHash &parameters)
{
    Q_UNUSED(parameters);
    return argument;
}
#endif //QT_VERSION < QT_VERSION_CHECK(4,8,0)

const QDBusArgument &operator>>(const QDBusArgument &argument, QVariantHash &parameters)
{
    Q_UNUSED(parameters);
    return argument;
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
    qDBusAbstractInterfaceCaptureCalls = true;
    qDBusAbstractInterfaceAsyncCallMethods.clear();
    qDBusAbstractInterfaceAsyncCallArguments.clear();
    qDBusAbstractInterfaceCallMethods.clear();
    qDBusAbstractInterfaceCallArguments.clear();
    qDBusPendingReplyArgumentsGetCapabilities.clear();
    qDBusPendingReplyArgumentsGetNotifications.clear();
    qDBusPendingReplyArgumentsNotify = 0;
}

void Ut_MNotification::cleanup()
{
}

void Ut_MNotification::testGettersAndSetters()
{
    QDateTime dateTime = QDateTime::fromTime_t(123456);
    MNotification notification("eventType", "summary", "body");
    QCOMPARE(notification.eventType(), QString("eventType"));
    QCOMPARE(notification.summary(), QString("summary"));
    QCOMPARE(notification.body(), QString("body"));
    notification.setSummary("summary2");
    notification.setBody("body2");
    QCOMPARE(notification.summary(), QString("summary2"));
    QCOMPARE(notification.body(), QString("body2"));
    notification.setImage("image");
    QCOMPARE(notification.image(), QString("image"));
    notification.setCount(5);
    QCOMPARE(notification.count(), (uint)5);
    notification.setIdentifier("identifier");
    QCOMPARE(notification.identifier(), QString("identifier"));
}

void Ut_MNotification::testPublish()
{
    qDBusPendingReplyArgumentsNotify = 1;
    QDateTime dateTime = QDateTime::fromTime_t(123456);
    MRemoteAction action("serviceName", "objectPath", "interface", "methodName", QVariantList() << 5 << 6);
    MNotification notification("eventType");
    notification.setImage("image");
    notification.setSummary("summary");
    notification.setBody("body");
    notification.setCount(10);
    notification.setTimestamp(dateTime);
    notification.setAction(action);
    notification.setIdentifier("identifier");
    notification.publish();

    QCOMPARE(qDBusAbstractInterfaceAsyncCallMethods.count(), 1);
    QCOMPARE(qDBusAbstractInterfaceAsyncCallMethods.at(0), QString("Notify"));
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(0).toString(), QFileInfo(QCoreApplication::arguments()[0]).fileName());
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(1).toUInt(), (uint)0);
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(2).toString(), notification.image());
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(3).toString(), notification.summary());
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(4).toString(), notification.body());
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(5).toStringList().isEmpty(), true);
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(6).toHash().value("category").toString(), QString("eventType"));
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(6).toHash().value("x-nemo-item-count").toUInt(), (uint)10);
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(6).toHash().value("x-nemo-timestamp").toDateTime(), dateTime);
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(6).toHash().value("x-nemo-preview-summary").toString(), QString("summary"));
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(6).toHash().value("x-nemo-preview-body").toString(), QString("body"));
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(6).toHash().value("x-nemo-legacy-type").toString(), QString("MNotification"));
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(6).toHash().value("x-nemo-legacy-identifier").toString(), QString("identifier"));
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(6).toHash().value("x-nemo-remote-action-default").toString(), action.toString());
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(0).at(7).toInt(), -1);

    notification.publish();
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(1).at(1).toUInt(), qDBusPendingReplyArgumentsNotify);
}

void Ut_MNotification::testIsPublished()
{
    MNotification notification;
    QCOMPARE(notification.isPublished(), false);

    qDBusPendingReplyArgumentsNotify = 1;
    notification.publish();
    QCOMPARE(notification.isPublished(), true);

    qDBusPendingReplyArgumentsNotify = 0;
    notification.publish();
    QCOMPARE(notification.isPublished(), false);
}

void Ut_MNotification::testRemove()
{
    MNotification notification;
    QCOMPARE(notification.remove(), false);

    qDBusPendingReplyArgumentsNotify = 1;
    notification.publish();
    QCOMPARE(notification.remove(), true);
    QCOMPARE(notification.isPublished(), false);
    QCOMPARE(qDBusAbstractInterfaceAsyncCallMethods.count(), 2);
    QCOMPARE(qDBusAbstractInterfaceAsyncCallMethods.at(1), QString("CloseNotification"));
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(1).at(0).toUInt(), qDBusPendingReplyArgumentsNotify);
}

void Ut_MNotification::testGettingAllNotificationsDoesNothingIfManagerDoesNotSupport()
{
    MNotification::notifications();
    QCOMPARE(qDBusAbstractInterfaceAsyncCallMethods.count(), 1);
    QCOMPARE(qDBusAbstractInterfaceAsyncCallMethods.at(0), QString("GetCapabilities"));
}

void Ut_MNotification::testGettingAllNotifications()
{
    qDBusPendingReplyArgumentsGetCapabilities << "x-nemo-get-notifications";
    MNotification notification1("eventType1", "summary1", "body1");
    MNotification notification2("eventType2", "summary2", "body2");
    notification2.setProperty("legacyType", "MNotification");
    qDBusPendingReplyArgumentsGetNotifications << notification1 << notification2;
    QList<MNotification *> notificationList = MNotification::notifications();
    QCOMPARE(qDBusAbstractInterfaceAsyncCallMethods.count(), 2);
    QCOMPARE(qDBusAbstractInterfaceAsyncCallMethods.at(0), QString("GetCapabilities"));
    QCOMPARE(qDBusAbstractInterfaceAsyncCallMethods.at(1), QString("GetNotifications"));
    QCOMPARE(qDBusAbstractInterfaceAsyncCallArguments.at(1).at(0).toString(), QFileInfo(QCoreApplication::arguments()[0]).fileName());
    QCOMPARE(notificationList.count(), 1);
    QCOMPARE(notificationList.at(0)->eventType(), QString("eventType2"));
    QCOMPARE(notificationList.at(0)->summary(), QString("summary2"));
    QCOMPARE(notificationList.at(0)->body(), QString("body2"));
}

void Ut_MNotification::testThatTimestampOfNotificationIsUpdatedInPublish()
{
    MNotification notification;
    uint timestamp = 1234567890;
    gCurrentDateTimeUtc = QDateTime::fromTime_t(123456);

    QCOMPARE(notification.timestamp().isNull(), true);

    // Timestamp is not set and addNotification is successful
    qDBusPendingReplyArgumentsNotify = 1;
    notification.publish();
    QCOMPARE(notification.timestamp(), gCurrentDateTimeUtc);

    // Timestamp is set and updateNotification is successful
    qDBusPendingReplyArgumentsNotify = 1;
    notification.setTimestamp(QDateTime::fromTime_t(timestamp));
    notification.publish();
    QCOMPARE(notification.timestamp(), QDateTime::fromTime_t(timestamp));

    // updateNotification is not succesfull, so timestamp is previously updated timestamp
    qDBusPendingReplyArgumentsNotify = 0;
    notification.publish();
    QCOMPARE(notification.timestamp(), QDateTime::fromTime_t(timestamp));

    // Timestamp set is null QDateTime and updateNotification is successful
    qDBusPendingReplyArgumentsNotify = 1;
    notification.setTimestamp(QDateTime());
    notification.publish();
    QCOMPARE(notification.timestamp(), gCurrentDateTimeUtc);
}

QTEST_MAIN(Ut_MNotification)
