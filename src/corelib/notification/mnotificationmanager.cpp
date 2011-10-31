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

#include "mnotificationmanager.h"
#include "mfiledatastore.h"
#include <QCoreApplication>
#include <QDir>

static const QString DATA_PATH = QDir::homePath() + QString("/.config/libmeegotouch/notifications/");

const QString MNotificationManager::eventTypeKey = "eventType";
const QString MNotificationManager::summaryKey = "summary";
const QString MNotificationManager::bodyKey = "body";
const QString MNotificationManager::actionKey = "action";
const QString MNotificationManager::imageKey = "imageId";
const QString MNotificationManager::countKey = "count";
const QString MNotificationManager::identifierKey = "identifier";
const QString MNotificationManager::timestampKey = "timestamp";

Q_DECLARE_METATYPE(QVariantHash)
Q_DECLARE_METATYPE(QList<QVariantHash>)

MNotificationManager::MNotificationManager() :
    proxy("com.meego.core.MNotificationManager", "/notificationmanager", QDBusConnection::sessionBus()),
    userId(0)
{
    if (!QCoreApplication::instance()) {
        qWarning("QCoreApplication instance should be created before creating notifications");
        return;
    }

    if (!QDir::root().exists(DATA_PATH) && !QDir::root().mkpath(DATA_PATH)) {
        return;
    }

    // Resolve application name from the application's path. If the application name is applauncherd.bin, resolve it from the first argument
    QString appName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    if (appName == "applauncherd.bin") {
        QStringList args = QCoreApplication::arguments();
        if (!args.isEmpty()) {
            appName = QFileInfo(args.first()).fileName();
        }
    }

    MFileDataStore userIdStore(DATA_PATH + appName + ".data");
    if (!userIdStore.isReadable()) {
        return;
    }

    QString appId = QString("id/") + appName;

    // Check if a userId for an application with this name already exists
    if (userIdStore.contains(appId)) {
        userId = userIdStore.value(appId).toUInt();
    } else {
        if (!userIdStore.isWritable()) {
            return;
        }
        // Fetch a new id from the notification manager over DBus
        userId = proxy.notificationUserId();
        userIdStore.createValue(appId, userId);
    }

    qDBusRegisterMetaType<MNotification>();
    qDBusRegisterMetaType<MNotificationGroup>();
    qDBusRegisterMetaType<QList<MNotification> >();
    qDBusRegisterMetaType<QList<MNotificationGroup> >();
    qDBusRegisterMetaType<QList<QVariantHash> >();
    qDBusRegisterMetaType<QVariantHash>();
}

MNotificationManager::~MNotificationManager()
{
}

MNotificationManager *MNotificationManager::instance()
{
    static MNotificationManager notificationManagerInstance;
    return &notificationManagerInstance;
}

QList<uint> MNotificationManager::notificationIdList()
{
    return proxy.notificationIdList(userId);
}

QList<MNotification> MNotificationManager::notificationList()
{
    return proxy.notificationListWithNotificationParameters(userId);
}

QList<MNotificationGroup> MNotificationManager::notificationGroupList()
{
    return proxy.notificationGroupListWithNotificationParameters(userId);
}

uint MNotificationManager::notificationCountInGroup(uint groupId)
{
    return proxy.notificationCountInGroup(userId, groupId);
}

uint MNotificationManager::addNotification(uint groupId, const QVariantHash  &parameters)
{
    return proxy.addNotification(userId, groupId, parameters);
}

bool MNotificationManager::updateNotification(uint notificationId, const QVariantHash  &parameters)
{
    return proxy.updateNotification(userId, notificationId, parameters);
}

bool MNotificationManager::removeNotification(uint notificationId)
{
    return proxy.removeNotification(userId, notificationId);
}

uint MNotificationManager::addGroup(const QVariantHash  &parameters)
{
    return proxy.addGroup(userId, parameters);
}

bool MNotificationManager::updateGroup(uint groupId, const QVariantHash  &parameters)
{
    return proxy.updateGroup(userId, groupId, parameters);
}

bool MNotificationManager::removeGroup(uint groupId)
{
    return proxy.removeGroup(userId, groupId);
}

#if QT_VERSION < QT_VERSION_CHECK(4,8,0)
QDBusArgument &operator<<(QDBusArgument &argument, const QVariantHash &parameters)
{
    QHashIterator<QString, QVariant> i(parameters);

    argument.beginMap(QMetaType::QString, qMetaTypeId<QDBusVariant>());
    while (i.hasNext()) {
        i.next();

        argument.beginMapEntry();
        argument << i.key();
        argument << QDBusVariant(i.value());
        argument.endMapEntry();
    }
    argument.endMap();
    return argument;
}
#endif //QT_VERSION < QT_VERSION_CHECK(4,8,0)

const QDBusArgument &operator>>(const QDBusArgument &argument, QVariantHash &parameters)
{
    argument.beginMap();
    while (!argument.atEnd()) {
        QString key;
        QDBusVariant value;

        argument.beginMapEntry ();
        argument >> key;
        argument >> value;
        argument.endMapEntry ();

        parameters[key] = value.variant();
    }
    argument.endMap();

    return argument;
}
