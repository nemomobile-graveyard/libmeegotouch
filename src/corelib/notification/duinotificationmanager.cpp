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

#include "duinotificationmanager.h"
#include "duifiledatastore.h"
#include <DuiApplication>
#include <QDir>

static const QString DATA_PATH = QDir::homePath() + QString("/.config/libdui/notifications/");

DuiNotificationManager::DuiNotificationManager() :
    proxy("com.nokia.dui.NotificationManager", "/notificationmanager", QDBusConnection::sessionBus()),
    userId(0)
{
    if (!DuiApplication::instance()) {
        qWarning("DuiApplication instance should be created before creating persistent notifications");
        return;
    }


    if (!QDir::root().exists(DATA_PATH) && !QDir::root().mkpath(DATA_PATH))
        return;

    DuiFileDataStore userIdStore(DATA_PATH + DuiApplication::appName() + ".data");

    if (!userIdStore.isReadable())
        return;

    QString appId = QString("id/") + DuiApplication::appName();

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

DuiNotificationManager::~DuiNotificationManager()
{
}

DuiNotificationManager *DuiNotificationManager::instance()
{
    static DuiNotificationManager notificationManagerInstance;
    return &notificationManagerInstance;
}

uint DuiNotificationManager::addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return proxy.addGroup(userId, eventType, summary, body, action, imageURI, count);
}

uint DuiNotificationManager::addGroup(const QString &eventType)
{
    return proxy.addGroup(userId, eventType);
}

uint DuiNotificationManager::addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return proxy.addNotification(userId, groupId, eventType, summary, body, action, imageURI, count);
}

uint DuiNotificationManager::addNotification(uint groupId, const QString &eventType)
{
    return proxy.addNotification(userId, groupId, eventType);
}

bool DuiNotificationManager::removeGroup(uint groupId)
{
    return proxy.removeGroup(userId, groupId);
}

bool DuiNotificationManager::removeNotification(uint notificationId)
{
    return proxy.removeNotification(userId, notificationId);
}

bool DuiNotificationManager::updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return proxy.updateGroup(userId, groupId, eventType, summary, body, action, imageURI, count);
}

bool DuiNotificationManager::updateGroup(uint groupId, const QString &eventType)
{
    return proxy.updateGroup(userId, groupId, eventType);
}

bool DuiNotificationManager::updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return proxy.updateNotification(userId, notificationId, eventType, summary, body, action, imageURI, count);
}

bool DuiNotificationManager::updateNotification(uint notificationId, const QString &eventType)
{
    return proxy.updateNotification(userId, notificationId, eventType);
}

QList<uint> DuiNotificationManager::notificationIdList()
{
    return proxy.notificationIdList(userId);
}
