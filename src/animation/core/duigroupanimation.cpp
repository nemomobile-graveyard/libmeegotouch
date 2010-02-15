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

#include "duigroupanimation.h"
#include "duigroupanimation_p.h"

#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

void DuiGroupAnimationPrivate::init(DuiGroupAnimation::Type type)
{
    Q_Q(DuiGroupAnimation);

    if (type == DuiGroupAnimation::Parallel) {
        group = new QParallelAnimationGroup(q);
    } else {
        group = new QSequentialAnimationGroup(q);
    }
}

// protected constructor
DuiGroupAnimation::DuiGroupAnimation(DuiGroupAnimationPrivate *dd, DuiGroupAnimation::Type type, QObject *parent) :
    DuiAnimation(dd, parent)
{
    Q_D(DuiGroupAnimation);
    d->init(type);
}

// public constructor
DuiGroupAnimation::DuiGroupAnimation(DuiGroupAnimation::Type type, QObject *parent) :
    DuiAnimation(new DuiGroupAnimationPrivate, parent)
{
    Q_D(DuiGroupAnimation);
    d->init(type);
}

// destructor
DuiGroupAnimation::~DuiGroupAnimation()
{
    // no need to delete d->group (Qt's parent-child relationship)
}

int DuiGroupAnimation::duration() const
{
    Q_D(const DuiGroupAnimation);
    return d->group->duration();
}

QAnimationGroup *DuiGroupAnimation::group()
{
    Q_D(DuiGroupAnimation);
    return d->group;
}

const QAnimationGroup *DuiGroupAnimation::group() const
{
    Q_D(const DuiGroupAnimation);
    return d->group;
}

void DuiGroupAnimation::updateCurrentTime(int msecs)
{
    Q_D(DuiGroupAnimation);
    d->group->setCurrentTime(msecs);
}

void DuiGroupAnimation::updateState(QAbstractAnimation::State oldState,
                                    QAbstractAnimation::State newState)
{
    Q_UNUSED(oldState);
    Q_D(DuiGroupAnimation);
    switch (newState) {
    case Stopped:
        d->group->stop();
        break;
    case Paused:
        d->group->pause();
        break;
    case Running:
        d->group->start();
        break;
    default:
        break;
    }
}

void DuiGroupAnimation::updateDirection(QAbstractAnimation::Direction direction)
{
    Q_D(DuiGroupAnimation);
    d->group->setDirection(direction);
}
