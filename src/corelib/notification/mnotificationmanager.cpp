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
#include <MApplication>
#include <QDir>

static const QString DATA_PATH = QDir::homePath() + QString("/.config/libmeegotouch/notifications/");

MNotificationManager::MNotificationManager() :
    proxy("com.nokia.m.NotificationManager", "/notificationmanager", QDBusConnection::sessionBus()),
    userId(0)
{
    if (!MApplication::instance()) {
        qWarning("MApplication instance should be created before creating persistent notifications");
        return;
    }


    if (!QDir::root().exists(DATA_PATH) && !QDir::root().mkpath(DATA_PATH))
        return;

    MFileDataStore userIdStore(DATA_PATH + MApplication::appName() + ".data");

    if (!userIdStore.isReadable())
        return;

    QString appId = QString("id/") + MApplication::appName();

    // Check if a userId for an application with this name already exists
    if (userIdStore.contains(appId)) {
        userId = userIdStore.value(appId).toUInt();
    } else {
        if (!userIdStore.isWritable())
            return;
        // Fetch a new id from the notification manager over DBus
        userId = proxy.notificationUserId();
        userIdStore.createValue(appId, userId);
    }
}

MNotificationManager::~MNotificationManager()
{
}

MNotificationManager *MNotificationManager::instance()
{
    static MNotificationManager notificationManagerInstance;
    return &notificationManagerInstance;
}

uint MNotificationManager::addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return proxy.addGroup(userId, eventType, summary, body, action, imageURI, count);
}

uint MNotificationManager::addGroup(const QString &eventType)
{
    return proxy.addGroup(userId, eventType);
}

uint MNotificationManager::addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return proxy.addNotification(userId, groupId, eventType, summary, body, action, imageURI, count);
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

bool MNotificationManager::updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return proxy.updateGroup(userId, groupId, eventType, summary, body, action, imageURI, count);
}

bool MNotificationManager::updateGroup(uint groupId, const QString &eventType)
{
    return proxy.updateGroup(userId, groupId, eventType);
}

bool MNotificationManager::updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return proxy.updateNotification(userId, notificationId, eventType, summary, body, action, imageURI, count);
}

bool MNotificationManager::updateNotification(uint notificationId, const QString &eventType)
{
    return proxy.updateNotification(userId, notificationId, eventType);
}

QList<uint> MNotificationManager::notificationIdList()
{
    return proxy.notificationIdList(userId);
}
