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

#include "duiitemstate.h"
#include "duiitemstate_p.h"
#include <QGraphicsWidget>

DuiItemStatePrivate::DuiItemStatePrivate(QGraphicsWidget *item) :
    item(item),
    sourceOpacity(-1),
    targetOpacity(-1),
    flags(DuiItemState::STATE_FLAG_NONE),
    geometryProgress(1),
    opacityProgress(1),
    isAnimationDone(true)
{
}

DuiItemState::DuiItemState()
    : d_ptr(new DuiItemStatePrivate(NULL))
{
}

DuiItemState::DuiItemState(QGraphicsWidget *i)
    : d_ptr(new DuiItemStatePrivate(i))
{
}

DuiItemState::DuiItemState(const DuiItemState &i)
    : d_ptr(new DuiItemStatePrivate(*i.d_ptr))
{ }

DuiItemState *DuiItemState::operator =(const DuiItemState &i)
{
    Q_D(DuiItemState);
    *d = *i.d_ptr;  //do a shallow copy
    return this;
}

bool DuiItemState::operator ==(const DuiItemState &i) const
{
    Q_D(const DuiItemState);
    // Identity is independent of positions...
    return (d->item == i.d_ptr->item);
}

DuiItemState::~DuiItemState()
{
    delete d_ptr;
}

bool DuiItemState::isNull() const
{
    Q_D(const DuiItemState);
    return (0 == d->item);
}

QGraphicsWidget *DuiItemState::item() const
{
    Q_D(const DuiItemState);
    return d->item;
}

QRectF DuiItemState::sourceGeometry() const
{
    Q_D(const DuiItemState);
    return d->sourceGeometry;
}

QRectF DuiItemState::targetGeometry() const
{
    Q_D(const DuiItemState);
    return d->targetGeometry;
}

DuiItemState::Flags DuiItemState::flags() const
{
    Q_D(const DuiItemState);
    return d->flags;
}

void DuiItemState::setGeometry(const QRectF &p)
{
    Q_D(DuiItemState);
    d->item->setGeometry(p);
    d->sourceGeometry = p;
    d->isAnimationDone = false;
    if (d->sourceGeometry != d->targetGeometry)
        d->geometryProgress = 0;
    else
        d->geometryProgress = 1;
}
void DuiItemState::setTargetGeometry(const QRectF &p)
{
    Q_D(DuiItemState);
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

void DuiItemState::setFlags(Flags new_flags)
{
    Q_D(DuiItemState);
    if (isNull()) {
        return;
    }
    const Flags old_flags(d->flags);
    d->flags = new_flags;
}

bool DuiItemState::isSet(Flag flag) const
{
    Q_D(const DuiItemState);
    return (d->flags & flag) == flag;
}

void DuiItemState::addFlags(Flags additional_flags)
{
    Q_D(DuiItemState);
    if (isNull()) {
        return;
    }
    const Flags old_flags(d->flags);
    d->flags |= additional_flags;
}

void DuiItemState::removeFlags(Flags flags_to_remove)
{
    Q_D(DuiItemState);
    if (isNull()) {
        return;
    }
    d->flags &= ~flags_to_remove;
}

bool DuiItemState::hasIdenticalLayout(const DuiItemState &i) const
{
    Q_D(const DuiItemState);
    return (i.d_ptr->targetGeometry   == d->targetGeometry &&
            i.d_ptr->targetOpacity    == d->targetOpacity &&
            i.d_ptr->opacityProgress  == d->opacityProgress &&
            i.d_ptr->geometryProgress == d->geometryProgress &&
            i.d_ptr->flags            == d->flags);
}

bool DuiItemState::isAnimationDone() const
{
    Q_D(const DuiItemState);
    return d->isAnimationDone;
}

void DuiItemState::animationDone()
{
    Q_D(DuiItemState);
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

void DuiItemState::setGeometryProgress(qreal progress)
{
    Q_D(DuiItemState);
    d->geometryProgress = progress;
    if (progress != 1)
        d->isAnimationDone = false;
}
void DuiItemState::setOpacityProgress(qreal progress)
{
    Q_D(DuiItemState);
    d->opacityProgress = progress;
    if (progress != 1)
        d->isAnimationDone = false;
}
qreal DuiItemState::geometryProgress() const
{
    Q_D(const DuiItemState);
    return d->geometryProgress;
}
qreal DuiItemState::opacityProgress() const
{
    Q_D(const DuiItemState);
    return d->opacityProgress;
}

void DuiItemState::setTargetOpacity(qreal opacity)
{
    Q_D(DuiItemState);
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
qreal DuiItemState::targetOpacity() const
{
    Q_D(const DuiItemState);
    return d->targetOpacity;
}
qreal DuiItemState::sourceOpacity() const
{
    Q_D(const DuiItemState);
    return d->sourceOpacity;
}
void DuiItemState::hide()
{
    Q_D(DuiItemState);
    removeFlags(STATE_FLAG_TO_BE_SHOWN);
    d->isAnimationDone = false;
    if (isSet(STATE_FLAG_SHOWING)) //if it's not already hiding
        addFlags(STATE_FLAG_TO_BE_HIDDEN);
}
