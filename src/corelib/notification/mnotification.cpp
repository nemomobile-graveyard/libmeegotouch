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

MNotification::MNotification() :
    d_ptr(new MNotificationPrivate)
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

MNotification::MNotification(const MNotification &notification) :
    QObject(), d_ptr(new MNotificationPrivate)
{
    *this = notification;
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

QString MNotification::eventType() const
{
    Q_D(const MNotification);
    return d->eventType;
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

void MNotification::setIdentifier(const QString &identifier)
{
    Q_D(MNotification);
    d->identifier = identifier;
}

QString MNotification::identifier() const
{
    Q_D(const MNotification);
    return d->identifier;
}

bool MNotification::publish()
{
    Q_D(MNotification);

    bool success = false;
    if (d->id == 0) {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull() || !d->identifier.isNull()) {
            d->id = MNotificationManager::instance()->addNotification(d->groupId, d->eventType, d->summary, d->body, d->action, d->image, d->count, d->identifier);
        } else {
            d->id = MNotificationManager::instance()->addNotification(d->groupId, d->eventType);
        }

        success = d->id != 0;
    } else {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull() || !d->identifier.isNull()) {
            success = MNotificationManager::instance()->updateNotification(d->id, d->eventType, d->summary, d->body, d->action, d->image, d->count, d->identifier);
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
    QList<MNotification> list = MNotificationManager::instance()->notificationListWithIdentifiers();
    QList<MNotification *> notifications;
    foreach(const MNotification &notification, list) {
        notifications.append(new MNotification(notification));
    }
    return notifications;
}

QDBusArgument &operator<<(QDBusArgument &argument, const MNotification &notification)
{
    const MNotificationPrivate *d = notification.d_func();
    argument.beginStructure();
    argument << d->id;
    argument << d->groupId;
    argument << d->eventType;
    argument << d->summary;
    argument << d->body;
    argument << d->image;
    argument << d->action;
    argument << d->count;
    argument << d->identifier;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, MNotification &notification)
{
    MNotificationPrivate *d = notification.d_func();
    argument.beginStructure();
    argument >> d->id;
    argument >> d->groupId;
    argument >> d->eventType;
    argument >> d->summary;
    argument >> d->body;
    argument >> d->image;
    argument >> d->action;
    argument >> d->count;
    argument >> d->identifier;
    argument.endStructure();
    return argument;
}

MNotification &MNotification::operator=(const MNotification &notification)
{
    Q_D(MNotification);
    const MNotificationPrivate *dn = notification.d_func();
    d->id = dn->id;
    d->groupId = dn->groupId;
    d->eventType = dn->eventType;
    d->summary = dn->summary;
    d->body = dn->body;
    d->image = dn->image;
    d->action = dn->action;
    d->count = dn->count;
    d->identifier = dn->identifier;
    return *this;
}
