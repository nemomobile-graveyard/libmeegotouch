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

#include <QDBusConnection>
#include <QCoreApplication>
#include <QFileInfo>
#include <QScopedPointer>
#include "mnotification.h"
#include "mnotification_p.h"
#include "mnotificationgroup.h"
#include "mnotificationmanagerproxy.h"
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

//! A proxy for accessing the notification manager
static QScopedPointer<MNotificationManagerProxy> notificationManagerProxy;

MNotificationManagerProxy *notificationManager()
{
    if (notificationManagerProxy.isNull()) {
        qDBusRegisterMetaType<MNotification>();
        qDBusRegisterMetaType<QList<MNotification> >();
        notificationManagerProxy.reset(new MNotificationManagerProxy("org.freedesktop.Notifications", "/org/freedesktop/Notifications", QDBusConnection::sessionBus()));
    }
    return notificationManagerProxy.data();
}

MNotificationPrivate::MNotificationPrivate() :
    id(0),
    groupId(0),
    count(0)
{
}

MNotificationPrivate::~MNotificationPrivate()
{
}

QVariantHash MNotificationPrivate::hints() const
{
    QVariantHash hints;
    hints.insert("category", eventType);
    hints.insert("x-nemo-item-count", count);
    hints.insert("x-nemo-timestamp", userSetTimestamp);
    hints.insert("x-nemo-preview-summary", summary);
    hints.insert("x-nemo-preview-body", body);
    hints.insert("x-nemo-legacy-type", "MNotification");
    if (groupId > 0) {
        hints.insert("x-nemo-legacy-group-id", groupId);
    }
    if (!identifier.isEmpty()) {
        hints.insert("x-nemo-legacy-identifier", identifier);
    }
    if (!action.isEmpty()) {
        hints.insert("x-nemo-remote-action-default", action);
    }
    return hints;
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

uint MNotification::groupId() const
{
    Q_D(const MNotification);
    return d->groupId;
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

void MNotification::setTimestamp(const QDateTime &timestamp)
{
    Q_D(MNotification);
    d->userSetTimestamp = timestamp;
}

const QDateTime MNotification::timestamp() const
{
    Q_D(const MNotification);
    return d->publishedTimestamp;
}

bool MNotification::publish()
{
    Q_D(MNotification);

    if (d->userSetTimestamp.isNull()) {
        d->userSetTimestamp = QDateTime::currentDateTimeUtc();
    }

    QString summary;
    QString body;
    if (d->groupId == 0) {
        // Standalone notifications use the same summary and body for the preview banner and the lock screen; for grouped notifications only the preview banner has them
        summary = d->summary;
        body = d->body;
    }

    d->id = notificationManager()->Notify(QFileInfo(QCoreApplication::arguments()[0]).fileName(), d->id, d->image, summary, body, QStringList(), d->hints(), -1);

    if (d->id != 0) {
        d->publishedTimestamp = d->userSetTimestamp;
    }

    d->userSetTimestamp = QDateTime();

    return d->id != 0;
}

bool MNotification::remove()
{
    bool success = false;

    if (isPublished()) {
        Q_D(MNotification);
        notificationManager()->CloseNotification(d->id);
        d->id = 0;
        success = true;
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
    QList<MNotification *> notificationList;
    if (notificationManager()->GetCapabilities().value().contains("x-nemo-get-notifications")) {
        QList<MNotification> list = notificationManager()->GetNotifications(QFileInfo(QCoreApplication::arguments()[0]).fileName());
        foreach(const MNotification &notification, list) {
            if (notification.property("legacyType").toString() == "MNotification") {
                notificationList.append(new MNotification(notification));
            }
        }
    } else {
        qWarning("Notification manager does not support GetNotifications(). The application may misbehave.");
    }
    return notificationList;
}

QDBusArgument &operator<<(QDBusArgument &argument, const MNotification &)
{
    argument.beginStructure();
    argument << QString();
    argument << (uint)0;
    argument << QString();
    argument << QString();
    argument << QString();
    argument << QStringList();
    argument << QVariantHash();
    argument << -1;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, MNotification &notification)
{
    QString appName;
    QStringList actions;
    QVariantHash hints;
    int expireTimeout;
    argument.beginStructure();
    argument >> appName;
    argument >> notification.d_ptr->id;
    argument >> notification.d_ptr->image;
    argument >> notification.d_ptr->summary;
    argument >> notification.d_ptr->body;
    argument >> actions;
    argument >> hints;
    argument >> expireTimeout;
    argument.endStructure();

    if (hints.contains("x-nemo-preview-summary")) {
        notification.d_ptr->summary = hints.value("x-nemo-preview-summary").toString();
    }
    if (hints.contains("x-nemo-preview-body")) {
        notification.d_ptr->body = hints.value("x-nemo-preview-body").toString();
    }
    notification.d_ptr->eventType = hints.value("category").toString();
    notification.d_ptr->count = hints.value("x-nemo-item-count").toUInt();
    notification.d_ptr->userSetTimestamp = hints.value("x-nemo-timestamp").toDateTime();
    notification.d_ptr->action = hints.value("x-nemo-remote-action-default").toString();
    notification.d_ptr->identifier = hints.value("x-nemo-legacy-identifier").toString();
    notification.d_ptr->groupId = hints.value("x-nemo-legacy-group-id").toUInt();
    notification.setProperty("legacyType", hints.value("x-nemo-legacy-type"));

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
    d->userSetTimestamp = dn->userSetTimestamp;
    d->publishedTimestamp = dn->publishedTimestamp;
    setProperty("legacyType", notification.property("legacyType"));
    return *this;
}
