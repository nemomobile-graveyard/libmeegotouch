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

#include "mnotificationgroup.h"
#include "mnotificationgroup_p.h"
#include "mnotificationmanager.h"

MNotificationGroupPrivate::MNotificationGroupPrivate() : MNotificationPrivate()
{
}

MNotificationGroupPrivate::~MNotificationGroupPrivate()
{
}

MNotificationGroup::MNotificationGroup() :
    MNotification(*new MNotificationGroupPrivate)
{
}

MNotificationGroup::MNotificationGroup(const QString &eventType, const QString &summary, const QString &body) :
    MNotification(*new MNotificationGroupPrivate)
{
    Q_D(MNotificationGroup);
    d->eventType = eventType;
    d->summary = summary;
    d->body = body;
}

MNotificationGroup::MNotificationGroup(const MNotificationGroup &group) :
    MNotification(*new MNotificationGroupPrivate)
{
    *this = group;
}

MNotificationGroup::~MNotificationGroup()
{
}

MNotificationGroup::MNotificationGroup(uint id) : MNotification(id)
{
}

bool MNotificationGroup::publish()
{
    Q_D(MNotificationGroup);

    bool success = false;

    if (d->id == 0) {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull() || !d->identifier.isNull()) {
            d->id = MNotificationManager::instance()->addGroup(d->eventType, d->summary, d->body, d->action, d->image, d->count, d->identifier);
        } else {
            d->id = MNotificationManager::instance()->addGroup(d->eventType);
        }

        success = d->id != 0;
    } else {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull() || !d->identifier.isNull()) {
            success = MNotificationManager::instance()->updateGroup(d->id, d->eventType, d->summary, d->body, d->action, d->image, d->count, d->identifier);
        } else {
            success = MNotificationManager::instance()->updateGroup(d->id, d->eventType);
        }
    }

    return success;
}

uint MNotificationGroup::notificationCount()
{
    if (!isPublished()) {
        return 0;
    } else {
        Q_D(MNotificationGroup);
        return MNotificationManager::instance()->notificationCountInGroup(d->id);
    }
}

bool MNotificationGroup::remove()
{
    if (!isPublished()) {
        return false;
    } else {
        Q_D(MNotificationGroup);
        uint id = d->id;
        d->id = 0;
        return MNotificationManager::instance()->removeGroup(id);
    }
}

QList<MNotificationGroup *> MNotificationGroup::notificationGroups()
{
    QList<MNotificationGroup> list = MNotificationManager::instance()->notificationGroupListWithIdentifiers();
    QList<MNotificationGroup *> notificationGroups;
    foreach(const MNotificationGroup &group, list) {
        notificationGroups.append(new MNotificationGroup(group));
    }
    return notificationGroups;
}

QDBusArgument &operator<<(QDBusArgument &argument, const MNotificationGroup &group)
{
    const MNotificationGroupPrivate *d = group.d_func();
    argument.beginStructure();
    argument << d->id;
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

const QDBusArgument &operator>>(const QDBusArgument &argument, MNotificationGroup &group)
{
    MNotificationGroupPrivate *d = group.d_func();
    argument.beginStructure();
    argument >> d->id;
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
