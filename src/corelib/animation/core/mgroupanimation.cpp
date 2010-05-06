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

#include "mgroupanimation.h"
#include "mgroupanimation_p.h"

#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

void MGroupAnimationPrivate::init(MGroupAnimation::Type type)
{
    Q_Q(MGroupAnimation);

    if (type == MGroupAnimation::Parallel) {
        group = new QParallelAnimationGroup(q);
    } else {
        group = new QSequentialAnimationGroup(q);
    }
}

// protected constructor
MGroupAnimation::MGroupAnimation(MGroupAnimationPrivate *dd, MGroupAnimation::Type type, QObject *parent) :
    MAnimation(dd, parent)
{
    Q_D(MGroupAnimation);
    d->init(type);
}

// public constructor
MGroupAnimation::MGroupAnimation(MGroupAnimation::Type type, QObject *parent) :
    MAnimation(new MGroupAnimationPrivate, parent)
{
    Q_D(MGroupAnimation);
    d->init(type);
}

// destructor
MGroupAnimation::~MGroupAnimation()
{
    // no need to delete d->group (Qt's parent-child relationship)
}

int MGroupAnimation::duration() const
{
    Q_D(const MGroupAnimation);
    return d->group->duration();
}

QAnimationGroup *MGroupAnimation::group()
{
    Q_D(MGroupAnimation);
    return d->group;
}

const QAnimationGroup *MGroupAnimation::group() const
{
    Q_D(const MGroupAnimation);
    return d->group;
}

void MGroupAnimation::updateCurrentTime(int msecs)
{
    Q_D(MGroupAnimation);
    d->group->setCurrentTime(msecs);
}

void MGroupAnimation::updateState(QAbstractAnimation::State oldState,
                                    QAbstractAnimation::State newState)
{
    Q_UNUSED(oldState);
    Q_D(MGroupAnimation);
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

void MGroupAnimation::updateDirection(QAbstractAnimation::Direction direction)
{
    Q_D(MGroupAnimation);
    d->group->setDirection(direction);
}
