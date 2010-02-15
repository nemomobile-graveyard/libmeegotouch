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

#include "duilinearlayoutpolicy.h"
#include "duilinearlayoutpolicy_p.h"

#include "duilayout.h"
#include "duilayouthelper_p.h"
#include "duidebug.h"

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsLayoutItem>
#include <QtGui/QGraphicsLinearLayout>
DuiLinearLayoutPolicy::DuiLinearLayoutPolicy(DuiLayout *l, Qt::Orientation o) :
    DuiAbstractLayoutPolicy(*(new DuiLinearLayoutPolicyPrivate(l, o)))
{ }

DuiLinearLayoutPolicy::~DuiLinearLayoutPolicy()
{ }

void DuiLinearLayoutPolicy::addItem(QGraphicsLayoutItem *item)
{
    insertItem(-1, item);
}
void DuiLinearLayoutPolicy::addItem(QGraphicsLayoutItem *item, Qt::Alignment alignment)
{
    insertItem(-1, item, alignment);
}

void DuiLinearLayoutPolicy::insertItem(int index, QGraphicsLayoutItem *item)
{
    Q_D(DuiLinearLayoutPolicy);
    if (!item) {
        duiWarning("DuiLinearLayoutPolicy") << Q_FUNC_INFO << "cannot insert null item";
        return;
    }
    if (item == d->layout) {
        duiWarning("DuiLinearLayoutPolicy") << Q_FUNC_INFO << "cannot insert self";
        return;
    }

    d->fixIndex(&index);
    d->rowCount++;
    ProxyItem *proxyItem = new ProxyItem(item);
    d->engine->insertItem(index, proxyItem);
    DuiAbstractLayoutPolicy::insertItem(index, item);
}

void DuiLinearLayoutPolicy::insertItem(int index, QGraphicsLayoutItem *item, Qt::Alignment alignment)
{
    insertItem(index, item);
    setAlignment(item, alignment);
}

qreal DuiLinearLayoutPolicy::spacing() const
{
    Q_D(const DuiLinearLayoutPolicy);
    (void)style();  //Make sure the style is loaded
    return d->engine->spacing();
}

void DuiLinearLayoutPolicy::setItemSpacing(int index, qreal spacing)
{
    Q_D(DuiLinearLayoutPolicy);
    if (itemSpacing(index) == spacing)
        return;
    d->engine->setItemSpacing(index, spacing);
    invalidatePolicyAndLayout();
}

qreal DuiLinearLayoutPolicy::itemSpacing(int index) const
{
    Q_D(const DuiLinearLayoutPolicy);
    return d->engine->itemSpacing(index);
}

void DuiLinearLayoutPolicy::insertStretch(int index, int stretch)
{
    Q_D(DuiLinearLayoutPolicy);
    //FIXME This is going to screw everything up since we don't insert the item to the base class..
    d->fixIndex(&index);
    d->rowCount++;
    d->engine->insertStretch(index, stretch);
    invalidatePolicyAndLayout();
}
void DuiLinearLayoutPolicy::setStretchFactor(QGraphicsLayoutItem *item, int stretch)
{
    Q_D(DuiLinearLayoutPolicy);
    if (!item) {
        duiWarning("DuiLinearLayoutPolicy") << Q_FUNC_INFO << "cannot assign a stretch factor to a null item";
        return;
    }
    if (stretchFactor(item) == stretch)
        return;
    d->engine->setStretchFactor(item, stretch);
    invalidatePolicyAndLayout();
}
int DuiLinearLayoutPolicy::stretchFactor(QGraphicsLayoutItem *item) const
{
    Q_D(const DuiLinearLayoutPolicy);
    if (!item) {
        duiWarning("DuiLinearLayoutPolicy") << Q_FUNC_INFO << "cannot return a stretch factor for a null item";
        return 0;
    }
    return d->engine->stretchFactor(item);
}

void DuiLinearLayoutPolicy::setOrientation(Qt::Orientation orientation)
{
    Q_D(DuiLinearLayoutPolicy);
    if (orientation != d->engine->orientation()) {
        d->engine->setOrientation(orientation);
        if (orientation == Qt::Horizontal)
            d->engine->setSpacing(horizontalSpacing());
        else
            d->engine->setSpacing(verticalSpacing());
        invalidatePolicyAndLayout();
    }
}

Qt::Orientation DuiLinearLayoutPolicy::orientation() const
{
    Q_D(const DuiLinearLayoutPolicy);
    return d->engine->orientation();
}

void DuiLinearLayoutPolicy::removeAt(int index)
{
    Q_D(DuiLinearLayoutPolicy);
    if (index < 0 || index >= d->engine->count()) {
        duiWarning("DuiLinearLayoutPolicy") << Q_FUNC_INFO << "invalid index" << index;
        return;
    }
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(index);
    d->rowCount--;
    d->engine->removeAt(index);
    delete proxyItem;
    DuiAbstractLayoutPolicy::removeAt(index);
}
QSizeF DuiLinearLayoutPolicy::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiLinearLayoutPolicy);

    d->engine->setMinimumSize(QSizeF(-1, -1));
    d->engine->setMaximumSize(QSizeF(-1, -1));
    const_cast<DuiLinearLayoutPolicyPrivate *>(d)->refreshEngine();
    return d->engine->sizeHint(which, constraint);
}

void DuiLinearLayoutPolicy::relayout()
{
    Q_D(DuiLinearLayoutPolicy);
    d->engine->setMinimumSize(layout()->minimumSize());
    d->engine->setMaximumSize(layout()->maximumSize());
    d->refreshEngine();
    d->engine->activate();

    if (!isActive())
        return;
    for (int i = d->engine->count() - 1; i >= 0; --i) {
        const QGraphicsLayoutItem *item = d->engine->itemAt(i);
        setItemGeometry(i, item->geometry());
    }
}

void DuiLinearLayoutPolicy::invalidate()
{
    Q_D(DuiLinearLayoutPolicy);
    d->engine->invalidate();
    DuiAbstractLayoutPolicy::invalidate();
}

void DuiLinearLayoutPolicy::setHorizontalSpacing(qreal spacing)
{
    Q_D(DuiLinearLayoutPolicy);
    DuiAbstractLayoutPolicy::setHorizontalSpacing(spacing);
    if (orientation() == Qt::Horizontal)
        d->engine->setSpacing(spacing);
}

void DuiLinearLayoutPolicy::setVerticalSpacing(qreal spacing)
{
    Q_D(DuiLinearLayoutPolicy);
    DuiAbstractLayoutPolicy::setVerticalSpacing(spacing);
    if (orientation() == Qt::Vertical)
        d->engine->setSpacing(spacing);
}
void DuiLinearLayoutPolicy::setSpacing(qreal spacing)
{
    Q_D(DuiLinearLayoutPolicy);
    DuiAbstractLayoutPolicy::setHorizontalSpacing(spacing);
    DuiAbstractLayoutPolicy::setVerticalSpacing(spacing);
    d->engine->setSpacing(spacing);
}

void DuiLinearLayoutPolicy::setAlignment(QGraphicsLayoutItem *item, Qt::Alignment alignment)
{
    Q_D(DuiLinearLayoutPolicy);
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));
    d->engine->setAlignment(proxyItem, alignment);
    invalidatePolicyAndLayout();
}

Qt::Alignment DuiLinearLayoutPolicy::alignment(QGraphicsLayoutItem *item) const
{
    Q_D(const DuiLinearLayoutPolicy);
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));
    return d->engine->alignment(proxyItem);
}

