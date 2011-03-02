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

#include "mnotificationmanager.h"
#include "mfiledatastore.h"
#include <QCoreApplication>
#include <QDir>

static const QString DATA_PATH = QDir::homePath() + QString("/.config/libmeegotouch/notifications/");

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

    QFileInfo appFileInfo(QCoreApplication::applicationFilePath());
    MFileDataStore userIdStore(DATA_PATH + appFileInfo.fileName() + ".data");
    if (!userIdStore.isReadable()) {
        return;
    }

    QString appId = QString("id/") + appFileInfo.fileName();

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
}

MNotificationManager::~MNotificationManager()
{
}

MNotificationManager *MNotificationManager::instance()
{
    static MNotificationManager notificationManagerInstance;
    return &notificationManagerInstance;
}

uint MNotificationManager::addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier)
{
    return proxy.addGroup(userId, eventType, summary, body, action, imageURI, count, identifier);
}

uint MNotificationManager::addGroup(const QString &eventType)
{
    return proxy.addGroup(userId, eventType);
}

uint MNotificationManager::addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier)
{
    return proxy.addNotification(userId, groupId, eventType, summary, body, action, imageURI, count, identifier);
}

uint MNotificationManager::addNotification(uint groupId, const QString &eventType)
{
    return proxy.addNotification(userId, groupId, eventType);
}

bool MNotificationManager::removeGroup(uint groupId)
{
    return proxy.removeGroup(userId, groupId);
}

bool MNotificationManager::removeNotification(uint notificationId)
{
    return proxy.removeNotification(userId, notificationId);
}

bool MNotificationManager::updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier)
{
    return proxy.updateGroup(userId, groupId, eventType, summary, body, action, imageURI, count, identifier);
}

bool MNotificationManager::updateGroup(uint groupId, const QString &eventType)
{
    return proxy.updateGroup(userId, groupId, eventType);
}

bool MNotificationManager::updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier)
{
    return proxy.updateNotification(userId, notificationId, eventType, summary, body, action, imageURI, count, identifier);
}

bool MNotificationManager::updateNotification(uint notificationId, const QString &eventType)
{
    return proxy.updateNotification(userId, notificationId, eventType);
}

QList<uint> MNotificationManager::notificationIdList()
{
    return proxy.notificationIdList(userId);
}

QList<MNotification> MNotificationManager::notificationListWithIdentifiers()
{
    return proxy.notificationListWithIdentifiers(userId);
}

QList<MNotificationGroup> MNotificationManager::notificationGroupListWithIdentifiers()
{
    return proxy.notificationGroupListWithIdentifiers(userId);
}

uint MNotificationManager::notificationCountInGroup(uint groupId)
{
    return proxy.notificationCountInGroup(userId, groupId);
}
