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

#include "mlinearlayoutpolicy.h"
#include "mlinearlayoutpolicy_p.h"

#include "mlayout.h"
#include "mlayouthelper_p.h"
#include "mdebug.h"

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsLayoutItem>
#include <QtGui/QGraphicsLinearLayout>
MLinearLayoutPolicy::MLinearLayoutPolicy(MLayout *l, Qt::Orientation o) :
    MAbstractLayoutPolicy(*(new MLinearLayoutPolicyPrivate(l, o)))
{
    Q_D(MLinearLayoutPolicy);
    if (d->engine->orientation() == Qt::Horizontal)
        d->engine->setSpacing(horizontalSpacing());
    else
        d->engine->setSpacing(verticalSpacing());
}

MLinearLayoutPolicy::~MLinearLayoutPolicy()
{ }

void MLinearLayoutPolicy::addItem(QGraphicsLayoutItem *item)
{
    insertItem(-1, item);
}
void MLinearLayoutPolicy::addItem(QGraphicsLayoutItem *item, Qt::Alignment alignment)
{
    insertItem(-1, item, alignment);
}

void MLinearLayoutPolicy::insertItem(int index, QGraphicsLayoutItem *item)
{
    Q_D(MLinearLayoutPolicy);
    if (!item) {
        mWarning("MLinearLayoutPolicy") << Q_FUNC_INFO << "cannot insert null item";
        return;
    }
    if (item == d->layout) {
        mWarning("MLinearLayoutPolicy") << Q_FUNC_INFO << "cannot insert self";
        return;
    }

    d->fixIndex(&index);
    d->rowCount++;
    ProxyItem *proxyItem = new ProxyItem(item);
    d->engine->insertItem(index, proxyItem);
    MAbstractLayoutPolicy::insertItem(index, item);
    
    if( isActive() && d->notifyWidgetsOfLayoutPositionEnabled )
        d->notifyAffectedWidgetsOfLayoutPosition(index, true);
}

void MLinearLayoutPolicy::insertItem(int index, QGraphicsLayoutItem *item, Qt::Alignment alignment)
{
    insertItem(index, item);
    setAlignment(item, alignment);
}

qreal MLinearLayoutPolicy::spacing() const
{
    Q_D(const MLinearLayoutPolicy);
    (void)style();  //Make sure the style is loaded
    return d->engine->spacing();
}

void MLinearLayoutPolicy::setItemSpacing(int index, qreal spacing)
{
    Q_D(MLinearLayoutPolicy);
    if (itemSpacing(index) == spacing)
        return;
    d->engine->setItemSpacing(index, spacing);
    invalidatePolicyAndLayout();
}

qreal MLinearLayoutPolicy::itemSpacing(int index) const
{
    Q_D(const MLinearLayoutPolicy);
    return d->engine->itemSpacing(index);
}

void MLinearLayoutPolicy::insertStretch(int index, int stretch)
{
    Q_D(MLinearLayoutPolicy);
    //FIXME This is going to screw everything up since we don't insert the item to the base class..
    d->fixIndex(&index);
    d->rowCount++;
    d->engine->insertStretch(index, stretch);
    invalidatePolicyAndLayout();
}
void MLinearLayoutPolicy::setStretchFactor(QGraphicsLayoutItem *item, int stretch)
{
    Q_D(MLinearLayoutPolicy);
    if (!item) {
        mWarning("MLinearLayoutPolicy") << Q_FUNC_INFO << "cannot assign a stretch factor to a null item";
        return;
    }

    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));

    if (d->engine->stretchFactor(proxyItem) == stretch)
        return;

    d->engine->setStretchFactor(proxyItem, stretch);
    invalidatePolicyAndLayout();
}
int MLinearLayoutPolicy::stretchFactor(QGraphicsLayoutItem *item) const
{
    Q_D(const MLinearLayoutPolicy);
    if (!item) {
        mWarning("MLinearLayoutPolicy") << Q_FUNC_INFO << "cannot return a stretch factor for a null item";
        return 0;
    }
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));
    return d->engine->stretchFactor(proxyItem);
}

void MLinearLayoutPolicy::setOrientation(Qt::Orientation orientation)
{
    Q_D(MLinearLayoutPolicy);
    if (orientation != d->engine->orientation()) {
        d->engine->setOrientation(orientation);
        if (orientation == Qt::Horizontal)
            d->engine->setSpacing(horizontalSpacing());
        else
            d->engine->setSpacing(verticalSpacing());
        invalidatePolicyAndLayout();
        if( isActive() && d->notifyWidgetsOfLayoutPositionEnabled )
            d->notifyAllWidgetsOfLayoutPosition();
    }
}

Qt::Orientation MLinearLayoutPolicy::orientation() const
{
    Q_D(const MLinearLayoutPolicy);
    return d->engine->orientation();
}

void MLinearLayoutPolicy::removeAt(int index)
{
    Q_D(MLinearLayoutPolicy);
    if (index < 0 || index >= d->engine->count()) {
        mWarning("MLinearLayoutPolicy") << Q_FUNC_INFO << "invalid index" << index;
        return;
    }

    if( isActive() && d->notifyWidgetsOfLayoutPositionEnabled )
        d->notifyAffectedWidgetsOfLayoutPosition(index, false);

    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(index);
    d->rowCount--;
    d->engine->removeAt(index);
    delete proxyItem;
    MAbstractLayoutPolicy::removeAt(index);
}

QSizeF MLinearLayoutPolicy::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MLinearLayoutPolicy);

    d->engine->setMinimumSize(QSizeF(-1, -1));
    d->engine->setMaximumSize(QSizeF(-1, -1));
    const_cast<MLinearLayoutPolicyPrivate *>(d)->refreshEngine();
    return d->engine->sizeHint(which, constraint);
}

void MLinearLayoutPolicy::setNotifyWidgetsOfLayoutPositionEnabled(bool enabled)
{
    Q_D(MLinearLayoutPolicy);
    if( enabled != d->notifyWidgetsOfLayoutPositionEnabled ) {
        d->notifyWidgetsOfLayoutPositionEnabled = enabled;
        if( isActive() )
            d->notifyAllWidgetsOfLayoutPosition();
    }
}

bool MLinearLayoutPolicy::isNotifyWidgetsOfLayoutPositionEnabled() const
{
    Q_D(const MLinearLayoutPolicy);
    return d->notifyWidgetsOfLayoutPositionEnabled;
}


void MLinearLayoutPolicy::relayout()
{
    Q_D(MLinearLayoutPolicy);
    if (!isActive())
        return;

    d->engine->setMinimumSize(layout()->minimumSize());
    d->engine->setMaximumSize(layout()->maximumSize());
    d->refreshWidget();

    for (int i = d->engine->count() - 1; i >= 0; --i) {
        const QGraphicsLayoutItem *item = d->engine->itemAt(i);
        setItemGeometry(i, item->geometry());
    }
}

void MLinearLayoutPolicy::invalidate()
{
    Q_D(MLinearLayoutPolicy);
    d->engine->invalidate();

    MAbstractLayoutPolicy::invalidate();
}

void MLinearLayoutPolicy::setHorizontalSpacing(qreal spacing)
{
    Q_D(MLinearLayoutPolicy);
    MAbstractLayoutPolicy::setHorizontalSpacing(spacing);
    if (orientation() == Qt::Horizontal)
        d->engine->setSpacing(spacing);
}

void MLinearLayoutPolicy::setVerticalSpacing(qreal spacing)
{
    Q_D(MLinearLayoutPolicy);
    MAbstractLayoutPolicy::setVerticalSpacing(spacing);
    if (orientation() == Qt::Vertical)
        d->engine->setSpacing(spacing);
}
void MLinearLayoutPolicy::setSpacing(qreal spacing)
{
    Q_D(MLinearLayoutPolicy);
    MAbstractLayoutPolicy::setHorizontalSpacing(spacing);
    MAbstractLayoutPolicy::setVerticalSpacing(spacing);
    d->engine->setSpacing(spacing);
}

void MLinearLayoutPolicy::setAlignment(QGraphicsLayoutItem *item, Qt::Alignment alignment)
{
    Q_D(MLinearLayoutPolicy);
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));
    d->engine->setAlignment(proxyItem, alignment);
    invalidatePolicyAndLayout();
}

Qt::Alignment MLinearLayoutPolicy::alignment(QGraphicsLayoutItem *item) const
{
    Q_D(const MLinearLayoutPolicy);
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));
    return d->engine->alignment(proxyItem);
}
void MLinearLayoutPolicy::activated()
{
    Q_D(MLinearLayoutPolicy);
    MAbstractLayoutPolicy::activated();
    d->notifyAllWidgetsOfLayoutPosition();
}
