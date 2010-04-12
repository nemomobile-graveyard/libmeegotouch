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

#include "mnotification.h"
#include "mnotification_p.h"
#include "mnotificationmanager.h"
#include "mnotificationgroup.h"
#include "mremoteaction.h"

const QString MNotification::DeviceEvent = "device";
const QString MNotification::DeviceAddedEvent = "device.added";
const QString MNotification::DeviceErrorEvent = "device.error";
const QString MNotification::DeviceRemovedEvent = "device.removed";
const QString MNotification::EmailEvent = "email";
const QString MNotification::EmailArrivedEvent = "email.arrived";
const QString MNotification::EmailBouncedEvent = "email.bounced";
const QString MNotification::ImEvent = "im";
const QString MNotification::ImErrorEvent = "im.error";
const QString MNotification::ImReceivedEvent = "im.received";
const QString MNotification::NetworkEvent = "network";
const QString MNotification::NetworkConnectedEvent = "network.connected";
const QString MNotification::NetworkDisconnectedEvent = "network.disconnected";
const QString MNotification::NetworkErrorEvent = "network.error";
const QString MNotification::PresenceEvent = "presence";
const QString MNotification::PresenceOfflineEvent = "presence.offline";
const QString MNotification::PresenceOnlineEvent = "presence.online";
const QString MNotification::TransferEvent = "transfer";
const QString MNotification::TransferCompleteEvent = "transfer.complete";
const QString MNotification::TransferErrorEvent = "transfer.error";
const QString MNotification::MessageEvent = "x-nokia.message";
const QString MNotification::MessageArrivedEvent = "x-nokia.message.arrived";

MNotificationPrivate::MNotificationPrivate() :
    id(0),
    groupId(0),
    count(1)
{
}

MNotificationPrivate::~MNotificationPrivate()
{
}


MNotification::MNotification(MNotificationPrivate &dd) :
    d_ptr(&dd)
{
}

MNotification::MNotification(const QString &eventType, const QString &summary, const QString &body) :
    d_ptr(new MNotificationPrivate)
{
    Q_D(MNotification);
    d->eventType = eventType;
    d->summary = summary;
    d->body = body;
}

MNotification::MNotification(uint id) :
    d_ptr(new MNotificationPrivate)
{
    Q_D(MNotification);
    d->id = id;
}

MNotification::~MNotification()
{
    delete d_ptr;
}

uint MNotification::id() const
{
    Q_D(const MNotification);
    return d->id;
}

void MNotification::setGroup(const MNotificationGroup &group)
{
    Q_D(MNotification);
    d->groupId = group.id();
}

void MNotification::setEventType(const QString &eventType)
{
    Q_D(MNotification);
    d->eventType = eventType;
}

void MNotification::setSummary(const QString &summary)
{
    Q_D(MNotification);
    d->summary = summary;
}

QString MNotification::summary() const
{
    Q_D(const MNotification);
    return d->summary;
}

void MNotification::setBody(const QString &body)
{
    Q_D(MNotification);
    d->body = body;
}

QString MNotification::body() const
{
    Q_D(const MNotification);
    return d->body;
}

void MNotification::setImage(const QString &image)
{
    Q_D(MNotification);
    d->image = image;
}

QString MNotification::image() const
{
    Q_D(const MNotification);
    return d->image;
}

void MNotification::setAction(const MRemoteAction &action)
{
    Q_D(MNotification);
    d->action = action.toString();
}

void MNotification::setCount(uint count)
{
    Q_D(MNotification);
    d->count = count;
}

uint MNotification::count() const
{
    Q_D(const MNotification);
    return d->count;
}

bool MNotification::publish()
{
    Q_D(MNotification);

    bool success = false;
    if (d->id == 0) {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull()) {
            d->id = MNotificationManager::instance()->addNotification(d->groupId, d->eventType, d->summary, d->body, d->action, d->image, d->count);
        } else {
            d->id = MNotificationManager::instance()->addNotification(d->groupId, d->eventType);
        }

        success = d->id != 0;
    } else {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull()) {
            success = MNotificationManager::instance()->updateNotification(d->id, d->eventType, d->summary, d->body, d->action, d->image, d->count);
        } else {
            success = MNotificationManager::instance()->updateNotification(d->id, d->eventType);
        }
    }

    return success;
}

bool MNotification::remove()
{
    bool success = false;

    if (isPublished()) {
        Q_D(MNotification);
        uint id = d->id;
        d->id = 0;
        success = MNotificationManager::instance()->removeNotification(id);
    }

    return success;
}

bool MNotification::isPublished() const
{
    Q_D(const MNotification);
    return d->id != 0;
}

QList<MNotification *> MNotification::notifications()
{
    QList<uint> idList = MNotificationManager::instance()->notificationIdList();
    QList<MNotification *> notifications;
    foreach(uint i, idList) {
        MNotification *notification = new MNotification(i);
        notifications.append(notification);
    }
    return notifications;
}
