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

#include "ut_duinotificationmanager.h"
#include "duinotificationmanager.h"
#include "duinotification.h"
#include "duinotificationgroup.h"
#include "duiremoteaction.h"
#include "duifiledatastore.h"
#include <DuiApplication>

bool Ut_DuiNotificationManager::captureCalls = false;
QList<QString> Ut_DuiNotificationManager::asyncCallMethods;
QList< QList<QVariant> > Ut_DuiNotificationManager::asyncCallArguments;
QHash<QString, QVariant> Ut_DuiNotificationManager::mockStore;

DuiApplication *DuiApplication::instance()
{
    return reinterpret_cast<DuiApplication *>(1);
}

QString DuiApplication::appName()
{
    static QString name("ut_duinotificationmanager");
    return name;
}


DuiFileDataStore::DuiFileDataStore(const QString &) :
    d_ptr(0)
{
}

DuiFileDataStore::~DuiFileDataStore()
{
}

bool DuiFileDataStore::isReadable() const
{
    return true;
}

bool DuiFileDataStore::isWritable() const
{
    return true;
}

bool DuiFileDataStore::contains(const QString &key) const
{
    return Ut_DuiNotificationManager::mockStore.contains(key);
}

bool DuiFileDataStore::createValue(const QString &key, const QVariant &value)
{
    Ut_DuiNotificationManager::mockStore.insert(key, value);
    return true;
}

QVariant DuiFileDataStore::value(const QString &key) const
{
    return Ut_DuiNotificationManager::mockStore.value(key);
}

bool QDir::mkdir(const QString &) const
{
    return true;
}

bool QDir::exists(const QString &) const
{
    return true;
}

class TestRemoteAction : public DuiRemoteAction
{
public:
    TestRemoteAction(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent = NULL);
};

TestRemoteAction::TestRemoteAction(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent) : DuiRemoteAction(serviceName, objectPath, interface, methodName, arguments, parent)
{
}

// QDBusInterface stubs (used by DuiRemoteAction)
QDBusInterface::QDBusInterface(const QString &service, const QString &path, const QString &interface, const QDBusConnection &connection, QObject *parent) : QDBusAbstractInterface(service, path, interface.toUtf8().constData(), connection, parent)
{
}

QDBusInterface::~QDBusInterface()
{
}

// DuiNotificationManagerProxy stubs (used by DuiNotificationManager)
DuiNotificationManagerProxy::DuiNotificationManagerProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

DuiNotificationManagerProxy::~DuiNotificationManagerProxy()
{
}

// QDBusAbstractInterface stubs (used by DuiNotificationManagerProxy)
QDBusPendingCall QDBusAbstractInterface::asyncCallWithArgumentList(const QString &method, const QList<QVariant> & args)
{
    if (Ut_DuiNotificationManager::captureCalls) {
        Ut_DuiNotificationManager::asyncCallMethods.append(method);
        Ut_DuiNotificationManager::asyncCallArguments.append(args);
    }

    QDBusPendingReply<bool> reply;
    return reply;
}

QDBusMessage QDBusAbstractInterface::callWithArgumentList(QDBus::CallMode, const QString &, const QList<QVariant> &)
{
    return QDBusMessage();
}

// QDBusPendingReplyData stubs (used by DuiNotificationManagerProxy)
QVariant QDBusPendingReplyData::argumentAt(int) const
{
    return QVariant(1);
}

void QDBusPendingReplyData::setMetaTypes(int, const int *)
{
}

void Ut_DuiNotificationManager::initTestCase()
{
    // Clear the static members
    captureCalls = true;
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    // The manager should ask for the notification user ID (which will be stubbed to be 1)
    DuiNotification notification1("event");
    QCOMPARE(asyncCallMethods.count(), 2);
    QCOMPARE(asyncCallMethods.at(0), QString("notificationUserId"));
    QCOMPARE(asyncCallArguments.count(), 2);
    QCOMPARE(asyncCallArguments.at(0).count(), 0);

    // Check that the new user id was saved correctly
    QCOMPARE(mockStore.value("id/ut_duinotificationmanager").toUInt(), (uint)1);

    notificationUserId = 1;
}

void Ut_DuiNotificationManager::cleanupTestCase()
{
}

void Ut_DuiNotificationManager::init()
{
    // Clear the static members
    captureCalls = true;
    asyncCallMethods.clear();
    asyncCallArguments.clear();
}

void Ut_DuiNotificationManager::cleanup()
{
}

void Ut_DuiNotificationManager::testAddNotification()
{
    DuiNotification notification1("event");
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 4);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(0));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant(false));

    asyncCallMethods.clear();
    asyncCallArguments.clear();

    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    DuiNotification notification2("event", "summary", "body", "imageURI", action, 42, true);
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
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("imageURI"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant("42"));
    QCOMPARE(asyncCallArguments.at(0).at(8), QVariant(true));
}

void Ut_DuiNotificationManager::testUpdateNotification()
{
    DuiNotification notification1("event");
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    notification1.update("event");
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("updateNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 3);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(notification1.id()));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));

    asyncCallMethods.clear();
    asyncCallArguments.clear();

    DuiNotification notification2("event");
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    notification2.update("event", "summary", "body", "imageURI", 42, action);
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("updateNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 8);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(notification2.id()));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant("summary"));
    QCOMPARE(asyncCallArguments.at(0).at(4), QVariant("body"));
    QCOMPARE(asyncCallArguments.at(0).at(5), QVariant(action.toString()));
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("imageURI"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant("42"));
}

void Ut_DuiNotificationManager::testRemoveNotification()
{
    DuiNotification notification("event");
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

void Ut_DuiNotificationManager::testAddGroup()
{
    DuiNotificationGroup group1("event");
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addGroup"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 3);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant(false));

    asyncCallMethods.clear();
    asyncCallArguments.clear();

    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    DuiNotificationGroup group2("event", "summary", "body", "imageURI", action, 42, false);
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addGroup"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 8);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("summary"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant("body"));
    QCOMPARE(asyncCallArguments.at(0).at(4), QVariant(action.toString()));
    QCOMPARE(asyncCallArguments.at(0).at(5), QVariant("imageURI"));
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("42"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant(false));
}

void Ut_DuiNotificationManager::testUpdateGroup()
{
    DuiNotificationGroup group1("event");
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    group1.update("event");
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("updateGroup"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 3);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(group1.id()));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));

    asyncCallMethods.clear();
    asyncCallArguments.clear();

    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    DuiNotificationGroup group2("event", "summary", "body", "imageURI", action);
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    group2.update("event", "summary", "body", "imageURI", 42, action);
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("updateGroup"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 8);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(group2.id()));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant("summary"));
    QCOMPARE(asyncCallArguments.at(0).at(4), QVariant("body"));
    QCOMPARE(asyncCallArguments.at(0).at(5), QVariant(action.toString()));
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("imageURI"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant("42"));
}

void Ut_DuiNotificationManager::testRemoveGroup()
{
    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    DuiNotificationGroup group("event", "summary", "body", "imageURI", action);
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

void Ut_DuiNotificationManager::testAddToGroup()
{
    DuiNotificationGroup group1("event");
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    DuiNotification notification1(group1, "event");
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("addNotification"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).count(), 4);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(notificationUserId));
    QCOMPARE(asyncCallArguments.at(0).at(1), QVariant(group1.id()));
    QCOMPARE(asyncCallArguments.at(0).at(2), QVariant("event"));
    QCOMPARE(asyncCallArguments.at(0).at(3), QVariant(false));

    QList<QVariant> arguments;
    TestRemoteAction action("serviceName", "objectPath", "interface", "methodName", arguments);
    DuiNotificationGroup group2("event", "summary", "body", "imageURI", action);
    asyncCallMethods.clear();
    asyncCallArguments.clear();

    DuiNotification notification2(group2, "event", "summary", "body", "imageURI", action);
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
    QCOMPARE(asyncCallArguments.at(0).at(6), QVariant("imageURI"));
    QCOMPARE(asyncCallArguments.at(0).at(7), QVariant("1"));
    QCOMPARE(asyncCallArguments.at(0).at(8), QVariant(false));
}

void Ut_DuiNotificationManager::testNotificationIdList()
{
    DuiNotificationManager::instance()->notificationIdList();
    QCOMPARE(asyncCallMethods.count(), 1);
    QCOMPARE(asyncCallMethods.at(0), QString("notificationIdList"));
    QCOMPARE(asyncCallArguments.count(), 1);
    QCOMPARE(asyncCallArguments.at(0).at(0), QVariant(1));
}

QTEST_APPLESS_MAIN(Ut_DuiNotificationManager)
