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
#include "duiremoteaction.h"

DuiNotificationGroupPrivate::DuiNotificationGroupPrivate() :
    _id(0)
{
}

DuiNotificationGroupPrivate::~DuiNotificationGroupPrivate()
{
}

DuiNotificationGroup::DuiNotificationGroup(DuiNotificationGroupPrivate &dd) :
    d_ptr(&dd)
{
}

DuiNotificationGroup::DuiNotificationGroup(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, const DuiRemoteAction &action, uint count, bool persistent) :
    d_ptr(new DuiNotificationGroupPrivate)
{
    Q_D(DuiNotificationGroup);
    if (!summary.isNull() || !body.isNull() || !imageURI.isNull() || !action.toString().isNull())
        d->_id = DuiNotificationManager::instance()->addGroup(eventType, summary, body, action.toString(), imageURI, count, persistent);
    else
        d->_id = DuiNotificationManager::instance()->addGroup(eventType, persistent);
}

DuiNotificationGroup::DuiNotificationGroup(unsigned int id) :
    d_ptr(new DuiNotificationGroupPrivate)
{
    Q_D(DuiNotificationGroup);
    d->_id = id;
}

DuiNotificationGroup::~DuiNotificationGroup()
{
    delete d_ptr;
}

uint DuiNotificationGroup::id() const
{
    Q_D(const DuiNotificationGroup);
    return d->_id;
}

bool DuiNotificationGroup::update(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, uint count, const DuiRemoteAction &action)
{
    Q_D(DuiNotificationGroup);
    if (!summary.isNull() || !body.isNull() || !imageURI.isNull() || !action.toString().isNull())
        return DuiNotificationManager::instance()->updateGroup(d->_id, eventType, summary, body, action.toString(), imageURI, count);
    else
        return DuiNotificationManager::instance()->updateGroup(d->_id, eventType);
}

bool DuiNotificationGroup::remove()
{
    if (!isValid()) {
        return false;
    } else {
        Q_D(DuiNotificationGroup);
        uint id = d->_id;
        d->_id = 0;
        return DuiNotificationManager::instance()->removeGroup(id);
    }
}

bool DuiNotificationGroup::isValid() const
{
    Q_D(const DuiNotificationGroup);
    return d->_id != 0;
}
