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
#include "duiremoteaction.h"

DuiNotificationPrivate::DuiNotificationPrivate() :
    _id(0)
{
}

DuiNotificationPrivate::~DuiNotificationPrivate()
{
}


DuiNotification::DuiNotification(DuiNotificationPrivate &dd) :
    d_ptr(&dd)
{
}

DuiNotification::DuiNotification(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, const DuiRemoteAction &action, uint count, bool persistent) :
    d_ptr(new DuiNotificationPrivate)
{
    Q_D(DuiNotification);
    if (!summary.isNull() || !body.isNull() || !imageURI.isNull() || !action.toString().isNull())
        d->_id = DuiNotificationManager::instance()->addNotification(0, eventType, summary, body, action.toString(), imageURI, count, persistent);
    else
        d->_id = DuiNotificationManager::instance()->addNotification(0, eventType, persistent);
}

DuiNotification::DuiNotification(const DuiNotificationGroup &group, const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, const DuiRemoteAction &action) :
    d_ptr(new DuiNotificationPrivate)
{
    Q_D(DuiNotification);
    if (!summary.isNull() || !body.isNull() || !imageURI.isNull() || !action.toString().isNull())
        d->_id = DuiNotificationManager::instance()->addNotification(group.id(), eventType, summary, body, action.toString(), imageURI);
    else
        d->_id = DuiNotificationManager::instance()->addNotification(group.id(), eventType);
}

DuiNotification::DuiNotification(uint id) :
    d_ptr(new DuiNotificationPrivate)
{
    Q_D(DuiNotification);
    d->_id = id;
}

DuiNotification::~DuiNotification()
{
    delete d_ptr;
}

uint DuiNotification::id() const
{
    Q_D(const DuiNotification);
    return d->_id;
}

bool DuiNotification::update(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, uint count, const DuiRemoteAction &action)
{
    Q_D(DuiNotification);
    if (!summary.isNull() || !body.isNull() || !imageURI.isNull() || !action.toString().isNull())
        return DuiNotificationManager::instance()->updateNotification(d->_id, eventType, summary, body, action.toString(), imageURI, count);
    else
        return DuiNotificationManager::instance()->updateNotification(d->_id, eventType);
}

bool DuiNotification::remove()
{
    if (!isValid()) {
        return false;
    } else {
        Q_D(DuiNotification);
        uint id = d->_id;
        d->_id = 0;
        return DuiNotificationManager::instance()->removeNotification(id);
    }
}

bool DuiNotification::isValid() const
{
    Q_D(const DuiNotification);
    return d->_id != 0;
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
