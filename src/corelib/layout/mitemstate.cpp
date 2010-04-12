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

#include "mitemstate.h"
#include "mitemstate_p.h"
#include <QGraphicsWidget>

MItemStatePrivate::MItemStatePrivate(QGraphicsWidget *item) :
    item(item),
    sourceOpacity(-1),
    targetOpacity(-1),
    flags(MItemState::STATE_FLAG_NONE),
    geometryProgress(1),
    opacityProgress(1),
    isAnimationDone(true)
{
}

MItemState::MItemState()
    : d_ptr(new MItemStatePrivate(NULL))
{
}

MItemState::MItemState(QGraphicsWidget *i)
    : d_ptr(new MItemStatePrivate(i))
{
}

MItemState::MItemState(const MItemState &i)
    : d_ptr(new MItemStatePrivate(*i.d_ptr))
{ }

MItemState *MItemState::operator =(const MItemState &i)
{
    Q_D(MItemState);
    *d = *i.d_ptr;  //do a shallow copy
    return this;
}

bool MItemState::operator ==(const MItemState &i) const
{
    Q_D(const MItemState);
    // Identity is independent of positions...
    return (d->item == i.d_ptr->item);
}

MItemState::~MItemState()
{
    delete d_ptr;
}

bool MItemState::isNull() const
{
    Q_D(const MItemState);
    return (0 == d->item);
}

QGraphicsWidget *MItemState::item() const
{
    Q_D(const MItemState);
    return d->item;
}

QRectF MItemState::sourceGeometry() const
{
    Q_D(const MItemState);
    return d->sourceGeometry;
}

QRectF MItemState::targetGeometry() const
{
    Q_D(const MItemState);
    return d->targetGeometry;
}

MItemState::Flags MItemState::flags() const
{
    Q_D(const MItemState);
    return d->flags;
}

void MItemState::setGeometry(const QRectF &p)
{
    Q_D(MItemState);
    d->item->setGeometry(p);
    d->sourceGeometry = p;
    d->isAnimationDone = false;
    if (d->sourceGeometry != d->targetGeometry)
        d->geometryProgress = 0;
    else
        d->geometryProgress = 1;
}
void MItemState::setTargetGeometry(const QRectF &p)
{
    Q_D(MItemState);
    if (isNull()) {
        return;
    }
    QSizeF maximum  = d->item->effectiveSizeHint(Qt::MaximumSize, p.size());
    QSizeF minimum  = d->item->effectiveSizeHint(Qt::MinimumSize, p.size());
    QRectF target = p;
    target.setSize(target.size().boundedTo(maximum).expandedTo(minimum));
    if (target == d->targetGeometry && (isSet(STATE_FLAG_SHOWING) || isSet(STATE_FLAG_TO_BE_SHOWN)) && !isSet(STATE_FLAG_TO_BE_HIDDEN))
        return; //Nothing has changed and item is already being shown or is already showing.  If the item is hiding, then we need to reset the source and show the item anyway
    d->targetGeometry = target;
    d->sourceGeometry = d->item->geometry();

    if (!isSet(STATE_FLAG_SHOWING) || isSet(STATE_FLAG_TO_BE_HIDDEN)) //If it's not being shown or we are in the middle of a showing animation
        addFlags(STATE_FLAG_TO_BE_SHOWN);
    removeFlags(STATE_FLAG_TO_BE_HIDDEN);

    d->isAnimationDone = false;
    if (d->sourceGeometry != d->targetGeometry)
        d->geometryProgress = 0;
}

void MItemState::setFlags(Flags new_flags)
{
    Q_D(MItemState);
    if (isNull()) {
        return;
    }
    const Flags old_flags(d->flags);
    d->flags = new_flags;
}

bool MItemState::isSet(Flag flag) const
{
    Q_D(const MItemState);
    return (d->flags & flag) == flag;
}

void MItemState::addFlags(Flags additional_flags)
{
    Q_D(MItemState);
    if (isNull()) {
        return;
    }
    const Flags old_flags(d->flags);
    d->flags |= additional_flags;
}

void MItemState::removeFlags(Flags flags_to_remove)
{
    Q_D(MItemState);
    if (isNull()) {
        return;
    }
    d->flags &= ~flags_to_remove;
}

bool MItemState::hasIdenticalLayout(const MItemState &i) const
{
    Q_D(const MItemState);
    return (i.d_ptr->targetGeometry   == d->targetGeometry &&
            i.d_ptr->targetOpacity    == d->targetOpacity &&
            i.d_ptr->opacityProgress  == d->opacityProgress &&
            i.d_ptr->geometryProgress == d->geometryProgress &&
            i.d_ptr->flags            == d->flags);
}

bool MItemState::isAnimationDone() const
{
    Q_D(const MItemState);
    return d->isAnimationDone;
}

void MItemState::animationDone()
{
    Q_D(MItemState);
    d->isAnimationDone = true;
    d->opacityProgress = 1;
    d->geometryProgress = 1;
    d->sourceGeometry = d->targetGeometry;
    d->sourceOpacity = d->targetOpacity = -1;
    //Keep the deleted flag so that it can be deleted
    if (d->flags & STATE_FLAG_TO_BE_SHOWN) {
        d->flags &= ~STATE_FLAG_TO_BE_SHOWN;
        d->flags |= STATE_FLAG_SHOWING;
    }
    if (d->flags & STATE_FLAG_TO_BE_HIDDEN) {
        d->flags &= ~STATE_FLAG_TO_BE_HIDDEN;
        d->flags &= ~STATE_FLAG_SHOWING;
    }
}

void MItemState::setGeometryProgress(qreal progress)
{
    Q_D(MItemState);
    d->geometryProgress = progress;
    if (progress != 1)
        d->isAnimationDone = false;
}
void MItemState::setOpacityProgress(qreal progress)
{
    Q_D(MItemState);
    d->opacityProgress = progress;
    if (progress != 1)
        d->isAnimationDone = false;
}
qreal MItemState::geometryProgress() const
{
    Q_D(const MItemState);
    return d->geometryProgress;
}
qreal MItemState::opacityProgress() const
{
    Q_D(const MItemState);
    return d->opacityProgress;
}

void MItemState::setTargetOpacity(qreal opacity)
{
    Q_D(MItemState);
    if (isNull())
        return;
    d->targetOpacity = opacity;
    if (d->targetOpacity == -1)
        return;

    d->sourceOpacity = d->item->opacity();
    if (d->sourceOpacity != -1 && d->targetOpacity != d->sourceOpacity) {
        d->opacityProgress = 0;
        d->isAnimationDone = false;
    }
}
qreal MItemState::targetOpacity() const
{
    Q_D(const MItemState);
    return d->targetOpacity;
}
qreal MItemState::sourceOpacity() const
{
    Q_D(const MItemState);
    return d->sourceOpacity;
}
void MItemState::hide()
{
    Q_D(MItemState);
    removeFlags(STATE_FLAG_TO_BE_SHOWN);
    d->isAnimationDone = false;
    if (isSet(STATE_FLAG_SHOWING)) //if it's not already hiding
        addFlags(STATE_FLAG_TO_BE_HIDDEN);
}
