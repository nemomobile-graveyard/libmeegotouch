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

#include "ut_mnotificationmanager.h"
#include "mnotificationmanager.h"
#include "mnotification.h"
#include "mnotificationgroup.h"
#include "mremoteaction.h"
#include "mfiledatastore.h"
#include <QCoreApplication>

// Subclasses to gain access to the IDs
class TestNotification : public MNotification
{
public:
    explicit TestNotification(const QString &eventType, const QString &summary = QString(), const QString &body = QString());
    explicit TestNotification(uint id);
    virtual ~TestNotification();
    uint id() const;
};

class TestNotificationGroup : public MNotificationGroup
{
public:
    explicit TestNotificationGroup(const QString &eventType, const QString &summary = QString(), const QString &body = QString());
    explicit TestNotificationGroup(uint id);
    virtual ~TestNotificationGroup();
    uint id() const;
};

TestNotification::TestNotification(const QString &eventType, const QString &summary, const QString &body) : MNotification(eventType, summary, body)
{
}

TestNotification::TestNotification(uint id) : MNotification(id)
{
}

TestNotification::~TestNotification()
{
}

uint TestNotification::id() const
{
    return MNotification::id();
}

TestNotificationGroup::TestNotificationGroup(const QString &eventType, const QString &summary, const QString &body) : MNotificationGroup(eventType, summary, body)
{
}

TestNotificationGroup::TestNotificationGroup(uint id) : MNotificationGroup(id)
{
}

TestNotificationGroup::~TestNotificationGroup()
{
}

uint TestNotificationGroup::id() const
{
    return MNotificationGroup::id();
}

bool Ut_MNotificationManager::captureCalls = false;
QList<QString> Ut_MNotificationManager::asyncCallMethods;
QList< QList<QVariant> > Ut_MNotificationManager::asyncCallArguments;
QHash<QString, QVariant> Ut_MNotificationManager::mockStore;

MFileDataStore::MFileDataStore(const QString &) :
    d_ptr(0)
{
}

MFileDataStore::~MFileDataStore()
{
}

bool MFileDataStore::isReadable() const
{
    return true;
}

bool MFileDataStore::isWritable() const
{
    return true;
}

bool MFileDataStore::contains(const QString &key) const
{
    return Ut_MNotificationManager::mockStore.contains(key);
}

bool MFileDataStore::createValue(const QString &key, const QVariant &value)
{
    Ut_MNotificationManager::mockStore.insert(key, value);
    return true;
}

QVariant MFileDataStore::value(const QString &key) const
{
    return Ut_MNotificationManager::mockStore.value(key);
}

bool QDir::mkdir(const QString &) const
{
    return true;
}

bool QDir::exists(const QString &) const
{
    return true;
}

class TestRemoteAction : public MRemoteAction
{
public:
    TestRemoteAction(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent = NULL);
};

TestRemoteAction::TestRemoteAction(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent) : MRemoteAction(serviceName, objectPath, interface, methodName, arguments, parent)
{
}

// QDBusInterface stubs (used by MRemoteAction)
QDBusInterface::QDBusInterface(const QString &service, const QString &path, const QString &interface, const QDBusConnection &connection, QObject *parent) : QDBusAbstractInterface(service, path, interface.toUtf8().constData(), connection, parent)
{
}

QDBusInterface::~QDBusInterface()
{
}

// MNotificationManagerProxy stubs (used by MNotificationManager)
MNotificationManagerProxy::MNotificationManagerProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

MNotificationManagerProxy::~MNotificationManagerProxy()
{
}

// QDBusAbstractInterface stubs (used by MNotificationManagerProxy)
QDBusPendingCall QDBusAbstractInterface::asyncCallWithArgumentList(const QString &method, const QList<QVariant> & args)
{
    if (Ut_MNotificationManager::captureCalls) {
        Ut_MNotificationManager::asyncCallMethods.append(method);
        Ut_MNotificationManager::asyncCallArguments.append(args);
    }

    QDBusPendingReply<bool> reply;
    return reply;
}

QDBusMessage QDBusAbstractInterface::callWithArgumentList(QDBus::CallMode, const QString &, const QList<QVariant> &)
{
    return QDBusMessage();
}

// QDBusPendingReplyData stubs (used by MNotificationManagerProxy)
QVariant QDBusPendingReplyData::argumentAt(int) const
{
    return QVariant(1);
}

void QDBusPendingReplyData::setMetaTypes(int, const int *)
{
}

void Ut_MNotificationManager::initTestCase()
{
    // Clear the static members
    captureCalls = true;
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    // The manager should ask for the notification user ID (which will be stubbed to be 1)
    MNotification notification1("event");
    notification1.publish();
    QCOMPARE(asyncCallMethods.count(), 2);
    QCOMPARE(asyncCallMethods.at(0), QString("notificationUserId"));
    QCOMPARE(asyncCallArguments.count(), 2);
    QCOMPARE(asyncCallArguments.at(0).count(), 0);

    // Check that the new user id was saved correctly
    QCOMPARE(mockStore.value("id/ut_mnotificationmanager").toUInt(), (uint)1);

    notificationUserId = 1;
}

void Ut_MNotificationManager::cleanupTestCase()
{
}

void Ut_MNotificationManager::init()
{
    // Clear the static members
    captureCalls = true;
    asyncCallMethods.clear();
    asyncCallArguments.clear();
}

void Ut_MNotificationManager::cleanup()
{
}

void Ut_MNotificationManager::testNotificationIdList()
{
    MNotificationManager::instance()->notificationIdList();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("notificationIdList"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(1));
}

void Ut_MNotificationManager::testNotificationList()
{
    MNotificationManager::instance()->notificationList();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("notificationListWithNotificationParameters"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(1));
}

void Ut_MNotificationManager::testNotificationGroupList()
{
    MNotificationManager::instance()->notificationGroupList();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("notificationGroupListWithNotificationParameters"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(1));
}

void Ut_MNotificationManager::testAddNotificationWithParameters()
{
    TestNotification notification("Event", "Summary", "Body");
    TestRemoteAction action("service", "object", "interface", "method", QList<QVariant>());
    notification.setAction(action);
    QDateTime timestamp = QDateTime::currentDateTime();
    notification.setTimestamp(timestamp);
    TestNotificationGroup group("");
    uint groupId = group.id();
    notification.setGroup(group);
    notification.publish();

    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addNotification"));

    QCOMPARE(asyncCallArguments.at(0).at(0).toUInt(), notificationUserId);
    QCOMPARE(asyncCallArguments.at(0).at(1).toUInt(), groupId);

    QHash<QString, QVariant> varHash = asyncCallArguments.at(0).at(2).toHash();
    QCOMPARE(varHash.value(MNotificationManager::eventTypeKey).toString(), QString("Event"));
    QCOMPARE(varHash.value(MNotificationManager::summaryKey).toString(), QString("Summary"));
    QCOMPARE(varHash.value(MNotificationManager::bodyKey).toString(), QString("Body"));
    QCOMPARE(varHash.value(MNotificationManager::timestampKey), QVariant(timestamp.toTime_t()));
    QCOMPARE(varHash.value(MNotificationManager::actionKey).toString(), action.toString());

}

void Ut_MNotificationManager::testUpdateNotificationWithParameters()
{
    TestNotification notification("Event", "Summary", "Body");
    QDateTime timestamp = QDateTime::currentDateTime();
    notification.setTimestamp(timestamp);
    notification.publish();
    uint notificationId = notification.id();

    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addNotification"));

    asyncCallMethods.clear();
    asyncCallArguments.clear();

    notification.setSummary("Summary Updated");
    QDateTime timestampUpdated = timestamp.addDays(1);
    notification.setTimestamp(timestampUpdated);
    notification.publish();

    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("updateNotification"));
    QCOMPARE(asyncCallArguments.at(0).at(0).toUInt(), notificationUserId);
    QCOMPARE(asyncCallArguments.at(0).at(1).toUInt(), notificationId);

    QHash<QString, QVariant> varHash = asyncCallArguments.at(0).at(2).toHash();

    QCOMPARE(varHash.value(MNotificationManager::eventTypeKey).toString(), QString("Event"));
    QCOMPARE(varHash.value(MNotificationManager::summaryKey).toString(), QString("Summary Updated"));
    QCOMPARE(varHash.value(MNotificationManager::bodyKey).toString(), QString("Body"));
    QCOMPARE(varHash.value(MNotificationManager::timestampKey), QVariant(timestampUpdated.toTime_t()));

}

void Ut_MNotificationManager::testRemoveNotificationWithParameters()
{
    TestNotification notification("Event");
    notification.publish();
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    uint notificationId = notification.id();
    notification.remove();

    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("removeNotification"));
    QCOMPARE(asyncCallArguments.at(0).at(0).toUInt(), notificationUserId);
    QCOMPARE(asyncCallArguments.at(0).at(1).toUInt(), notificationId);
}

void Ut_MNotificationManager::testAddGroupWithParameters()
{
    TestNotificationGroup group("Event", "Summary", "Body");
    TestRemoteAction action("service", "object", "interface", "method", QList<QVariant>());
    group.setAction(action);
    group.publish();

    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addGroup"));
    QCOMPARE(asyncCallArguments.at(0).at(0).toUInt(), notificationUserId);

    QHash<QString, QVariant> varHash = asyncCallArguments.at(0).at(1).toHash();

    QCOMPARE(varHash.value(MNotificationManager::eventTypeKey).toString(), QString("Event"));
    QCOMPARE(varHash.value(MNotificationManager::summaryKey).toString(), QString("Summary"));
    QCOMPARE(varHash.value(MNotificationManager::bodyKey).toString(), QString("Body"));
    QCOMPARE(varHash.value(MNotificationManager::actionKey).toString(), action.toString());

}

void Ut_MNotificationManager::testUpdateGroupWithParameters()
{
    TestNotificationGroup group("Event", "Summary", "Body");
    group.publish();
    uint groupId = group.id();

    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addGroup"));

    asyncCallMethods.clear();
    asyncCallArguments.clear();

    group.setSummary("Summary Updated");
    group.publish();

    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("updateGroup"));
    QCOMPARE(asyncCallArguments.at(0).at(0).toUInt(), notificationUserId);
    QCOMPARE(asyncCallArguments.at(0).at(1).toUInt(), groupId);
    QHash<QString, QVariant> varHash = asyncCallArguments.at(0).at(2).toHash();

    QCOMPARE(varHash.value(MNotificationManager::eventTypeKey).toString(), QString("Event"));
    QCOMPARE(varHash.value(MNotificationManager::summaryKey).toString(), QString("Summary Updated"));
    QCOMPARE(varHash.value(MNotificationManager::bodyKey).toString(), QString("Body"));
}

void Ut_MNotificationManager::testRemoveGroupWithParameters()
{
    TestNotificationGroup group("Event");
    group.publish();
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    uint groupId = group.id();
    group.remove();

    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("removeGroup"));
    QCOMPARE(asyncCallArguments.at(0).at(0).toUInt(), notificationUserId);
    QCOMPARE(asyncCallArguments.at(0).at(1).toUInt(), groupId);
}

QTEST_MAIN(Ut_MNotificationManager)
