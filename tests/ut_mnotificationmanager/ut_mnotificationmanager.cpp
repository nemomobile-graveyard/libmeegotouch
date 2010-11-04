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

void Ut_MNotificationManager::testAddNotification()
{
    MNotification notification1("event");
    QCOMPARE(asyncCallMethods.count(), 0);
    notification1.publish();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 3);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(0));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));

    asyncCallMethods.clear();
    asyncCallArguments.clear();

    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    MNotification notification2("event");
    notification2.setSummary("summary");
    QCOMPARE(asyncCallMethods.count(), 0);
    notification2.setBody("body");
    QCOMPARE(asyncCallMethods.count(), 0);
    notification2.setImage("image");
    QCOMPARE(asyncCallMethods.count(), 0);
    notification2.setAction(action);
    QCOMPARE(asyncCallMethods.count(), 0);
    notification2.setCount(42);
    QCOMPARE(asyncCallMethods.count(), 0);
    notification2.setIdentifier("testidentifier");
    QCOMPARE(asyncCallMethods.count(), 0);
    notification2.publish();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 9);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(0));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant("summary"));
    QCOMPARE(asyncCallArguments.at(0).at(4), QVariant("body"));
    QCOMPARE(asyncCallArguments.at(0).at(5), QVariant(action.toString()));
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("image"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant("42"));
    QCOMPARE(asyncCallArguments.at(0).at(8), QVariant("testidentifier"));
}

void Ut_MNotificationManager::testUpdateNotification()
{
    TestNotification notification("event");
    notification.publish();
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    notification.setSummary("summary");
    notification.setBody("body");
    notification.setImage("image");
    notification.setCount(42);
    notification.setAction(action);
    notification.setIdentifier("testidentifier");
    notification.publish();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("updateNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 9);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(notification.id()));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant("summary"));
    QCOMPARE(asyncCallArguments.at(0).at(4), QVariant("body"));
    QCOMPARE(asyncCallArguments.at(0).at(5), QVariant(action.toString()));
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("image"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant("42"));
    QCOMPARE(asyncCallArguments.at(0).at(8), QVariant("testidentifier"));
}

void Ut_MNotificationManager::testRemoveNotification()
{
    TestNotification notification("event");
    notification.publish();
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    int id = notification.id();
    notification.remove();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("removeNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 2);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(id));
}

void Ut_MNotificationManager::testAddGroup()
{
    MNotificationGroup group1("event");
    QCOMPARE(asyncCallMethods.count(), 0);
    group1.publish();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addGroup"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 2);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant("event"));

    asyncCallMethods.clear();
    asyncCallArguments.clear();

    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    TestNotificationGroup group2("event");
    group2.setSummary("summary");
    QCOMPARE(asyncCallMethods.count(), 0);
    group2.setBody("body");
    QCOMPARE(asyncCallMethods.count(), 0);
    group2.setImage("image");
    QCOMPARE(asyncCallMethods.count(), 0);
    group2.setAction(action);
    QCOMPARE(asyncCallMethods.count(), 0);
    group2.setCount(42);
    QCOMPARE(asyncCallMethods.count(), 0);
    group2.setIdentifier("testidentifier");
    QCOMPARE(asyncCallMethods.count(), 0);
    group2.publish();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addGroup"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 8);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("summary"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant("body"));
    QCOMPARE(asyncCallArguments.at(0).at(4), QVariant(action.toString()));
    QCOMPARE(asyncCallArguments.at(0).at(5), QVariant("image"));
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("42"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant("testidentifier"));
}

void Ut_MNotificationManager::testUpdateGroup()
{
    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    TestNotificationGroup group("event");
    group.publish();
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    group.setSummary("summary");
    group.setBody("body");
    group.setImage("image");
    group.setCount(42);
    group.setAction(action);
    group.setIdentifier("testidentifier");
    group.publish();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("updateGroup"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 9);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(group.id()));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant("summary"));
    QCOMPARE(asyncCallArguments.at(0).at(4), QVariant("body"));
    QCOMPARE(asyncCallArguments.at(0).at(5), QVariant(action.toString()));
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("image"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant("42"));
    QCOMPARE(asyncCallArguments.at(0).at(8), QVariant("testidentifier"));
}

void Ut_MNotificationManager::testRemoveGroup()
{
    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    TestNotificationGroup group("event");
    group.publish();
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    int id = group.id();
    group.remove();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("removeGroup"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 2);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(id));
}

void Ut_MNotificationManager::testAddToGroup()
{
    TestNotificationGroup group1("event");
    group1.publish();
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    TestNotification notification1("event");
    notification1.setGroup(group1);
    notification1.publish();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 3);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(group1.id()));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));

    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    TestNotificationGroup group2("event");
    group2.setSummary("summary");
    group2.setBody("body");
    group2.setImage("image");
    group2.setAction(action);
    group2.publish();
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    TestNotification notification2("event");
    notification2.setGroup(group2);
    notification2.setSummary("summary");
    notification2.setBody("body");
    notification2.setImage("image");
    notification2.setAction(action);
    notification2.publish();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 9);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(group2.id()));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant("summary"));
    QCOMPARE(asyncCallArguments.at(0).at(4), QVariant("body"));
    QCOMPARE(asyncCallArguments.at(0).at(5), QVariant(action.toString()));
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("image"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant("1"));
    QCOMPARE(asyncCallArguments.at(0).at(8), QVariant(""));
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
    MNotificationManager::instance()->notificationListWithIdentifiers();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("notificationListWithIdentifiers"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(1));
}

void Ut_MNotificationManager::testNotificationGroupList()
{
    MNotificationManager::instance()->notificationGroupListWithIdentifiers();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("notificationGroupListWithIdentifiers"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(1));
}

QTEST_MAIN(Ut_MNotificationManager)
