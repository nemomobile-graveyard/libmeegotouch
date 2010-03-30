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

#include "duinotification.h"
#include "duinotification_p.h"
#include "duinotificationmanager.h"
#include "duinotificationgroup.h"
#include "duiremoteaction.h"

const QString DuiNotification::DeviceEvent = "device";
const QString DuiNotification::DeviceAddedEvent = "device.added";
const QString DuiNotification::DeviceErrorEvent = "device.error";
const QString DuiNotification::DeviceRemovedEvent = "device.removed";
const QString DuiNotification::EmailEvent = "email";
const QString DuiNotification::EmailArrivedEvent = "email.arrived";
const QString DuiNotification::EmailBouncedEvent = "email.bounced";
const QString DuiNotification::ImEvent = "im";
const QString DuiNotification::ImErrorEvent = "im.error";
const QString DuiNotification::ImReceivedEvent = "im.received";
const QString DuiNotification::NetworkEvent = "network";
const QString DuiNotification::NetworkConnectedEvent = "network.connected";
const QString DuiNotification::NetworkDisconnectedEvent = "network.disconnected";
const QString DuiNotification::NetworkErrorEvent = "network.error";
const QString DuiNotification::PresenceEvent = "presence";
const QString DuiNotification::PresenceOfflineEvent = "presence.offline";
const QString DuiNotification::PresenceOnlineEvent = "presence.online";
const QString DuiNotification::TransferEvent = "transfer";
const QString DuiNotification::TransferCompleteEvent = "transfer.complete";
const QString DuiNotification::TransferErrorEvent = "transfer.error";
const QString DuiNotification::MessageEvent = "x-nokia.message";
const QString DuiNotification::MessageArrivedEvent = "x-nokia.message.arrived";

DuiNotificationPrivate::DuiNotificationPrivate() :
    id(0),
    groupId(0),
    count(1)
{
}

DuiNotificationPrivate::~DuiNotificationPrivate()
{
}


DuiNotification::DuiNotification(DuiNotificationPrivate &dd) :
    d_ptr(&dd)
{
}

DuiNotification::DuiNotification(const QString &eventType, const QString &summary, const QString &body) :
    d_ptr(new DuiNotificationPrivate)
{
    Q_D(DuiNotification);
    d->eventType = eventType;
    d->summary = summary;
    d->body = body;
}

DuiNotification::DuiNotification(uint id) :
    d_ptr(new DuiNotificationPrivate)
{
    Q_D(DuiNotification);
    d->id = id;
}

DuiNotification::~DuiNotification()
{
    delete d_ptr;
}

uint DuiNotification::id() const
{
    Q_D(const DuiNotification);
    return d->id;
}

void DuiNotification::setGroup(const DuiNotificationGroup &group)
{
    Q_D(DuiNotification);
    d->groupId = group.id();
}

void DuiNotification::setEventType(const QString &eventType)
{
    Q_D(DuiNotification);
    d->eventType = eventType;
}

void DuiNotification::setSummary(const QString &summary)
{
    Q_D(DuiNotification);
    d->summary = summary;
}

QString DuiNotification::summary() const
{
    Q_D(const DuiNotification);
    return d->summary;
}

void DuiNotification::setBody(const QString &body)
{
    Q_D(DuiNotification);
    d->body = body;
}

QString DuiNotification::body() const
{
    Q_D(const DuiNotification);
    return d->body;
}

void DuiNotification::setImage(const QString &image)
{
    Q_D(DuiNotification);
    d->image = image;
}

QString DuiNotification::image() const
{
    Q_D(const DuiNotification);
    return d->image;
}

void DuiNotification::setAction(const DuiRemoteAction &action)
{
    Q_D(DuiNotification);
    d->action = action.toString();
}

void DuiNotification::setCount(uint count)
{
    Q_D(DuiNotification);
    d->count = count;
}

uint DuiNotification::count() const
{
    Q_D(const DuiNotification);
    return d->count;
}

bool DuiNotification::publish()
{
    Q_D(DuiNotification);

    bool success = false;
    if (d->id == 0) {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull()) {
            d->id = DuiNotificationManager::instance()->addNotification(d->groupId, d->eventType, d->summary, d->body, d->action, d->image, d->count);
        } else {
            d->id = DuiNotificationManager::instance()->addNotification(d->groupId, d->eventType);
        }

        success = d->id != 0;
    } else {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull()) {
            success = DuiNotificationManager::instance()->updateNotification(d->id, d->eventType, d->summary, d->body, d->action, d->image, d->count);
        } else {
            success = DuiNotificationManager::instance()->updateNotification(d->id, d->eventType);
        }
    }

    return success;
}

bool DuiNotification::remove()
{
    bool success = false;

    if (isPublished()) {
        Q_D(DuiNotification);
        uint id = d->id;
        d->id = 0;
        success = DuiNotificationManager::instance()->removeNotification(id);
    }

    return success;
}

bool DuiNotification::isPublished() const
{
    Q_D(const DuiNotification);
    return d->id != 0;
}

QList<DuiNotification *> DuiNotification::notifications()
{
    QList<uint> idList = DuiNotificationManager::instance()->notificationIdList();
    QList<DuiNotification *> notifications;
    foreach(uint i, idList) {
        DuiNotification *notification = new DuiNotification(i);
        notifications.append(notification);
    }
    return notifications;
}
