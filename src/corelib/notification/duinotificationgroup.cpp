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

#include "duinotificationgroup.h"
#include "duinotificationgroup_p.h"
#include "duinotificationmanager.h"

DuiNotificationGroupPrivate::DuiNotificationGroupPrivate() : DuiNotificationPrivate()
{
}

DuiNotificationGroupPrivate::~DuiNotificationGroupPrivate()
{
}

DuiNotificationGroup::DuiNotificationGroup(const QString &eventType, const QString &summary, const QString &body) :
    DuiNotification(*new DuiNotificationGroupPrivate)
{
    Q_D(DuiNotificationGroup);
    d->eventType = eventType;
    d->summary = summary;
    d->body = body;
}

DuiNotificationGroup::~DuiNotificationGroup()
{
}

DuiNotificationGroup::DuiNotificationGroup(uint id) : DuiNotification(id)
{
}

bool DuiNotificationGroup::publish()
{
    Q_D(DuiNotificationGroup);

    bool success = false;

    if (d->id == 0) {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull()) {
            d->id = DuiNotificationManager::instance()->addGroup(d->eventType, d->summary, d->body, d->action, d->image, d->count);
        } else {
            d->id = DuiNotificationManager::instance()->addGroup(d->eventType);
        }

        success = d->id != 0;
    } else {
        if (!d->summary.isNull() || !d->body.isNull() || !d->image.isNull() || !d->action.isNull()) {
            success = DuiNotificationManager::instance()->updateGroup(d->id, d->eventType, d->summary, d->body, d->action, d->image, d->count);
        } else {
            success = DuiNotificationManager::instance()->updateGroup(d->id, d->eventType);
        }
    }

    return success;
}

bool DuiNotificationGroup::remove()
{
    if (!isPublished()) {
        return false;
    } else {
        Q_D(DuiNotificationGroup);
        uint id = d->id;
        d->id = 0;
        return DuiNotificationManager::instance()->removeGroup(id);
    }
}
