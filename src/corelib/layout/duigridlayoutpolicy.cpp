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

#include "duigridlayoutpolicy.h"
#include "duigridlayoutpolicy_p.h"
#include "duiabstractlayoutpolicystyle.h"
#include "duidebug.h"

#include "duilayouthelper_p.h"

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsGridLayout>
DuiGridLayoutPolicy::DuiGridLayoutPolicy(DuiLayout *layout) :
    DuiAbstractLayoutPolicy(*(new DuiGridLayoutPolicyPrivate(layout)))
{ }

DuiGridLayoutPolicy::~DuiGridLayoutPolicy()
{ }

int DuiGridLayoutPolicy::rowCount() const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->rowCount();
}

int DuiGridLayoutPolicy::columnCount() const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->columnCount();
}


QGraphicsLayoutItem *DuiGridLayoutPolicy::itemAt(int row, int col) const
{
    Q_D(const DuiGridLayoutPolicy);
    ProxyItem *item = static_cast<ProxyItem *>(d->engine->itemAt(row, col));
    if (!item)
        return NULL;

    return item->proxiedItem();
}

void DuiGridLayoutPolicy::addItem(QGraphicsLayoutItem *item, int row, int column,
                                  int rowSpan, int columnSpan, Qt::Alignment alignment)
{
    Q_D(DuiGridLayoutPolicy);

    if (row < 0 || column < 0) {
        duiWarning("DuiGridLayoutPolicy") << Q_FUNC_INFO << "invalid row/column:" << (row < 0 ? row : column);
        return;
    }
    if (columnSpan < 1 || rowSpan < 1) {
        duiWarning("DuiGridLayoutPolicy") << Q_FUNC_INFO << "invalid row span/column span:" << (rowSpan < 1 ? rowSpan : columnSpan);
        return;
    }
    if (!item) {
        duiWarning("DuiGridLayoutPolicy") << Q_FUNC_INFO << "cannot set data for null item";
        return;
    }
    ProxyItem *proxyItem = new ProxyItem(item);
    d->engine->addItem(proxyItem, row, column, rowSpan, columnSpan, alignment);

    // Register new item with the policy and layout
    DuiAbstractLayoutPolicy::addItem(item);
}

void DuiGridLayoutPolicy::setRowSpacing(int row, qreal spacing)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setRowSpacing(row, spacing);
    invalidatePolicyAndLayout();
}

qreal DuiGridLayoutPolicy::rowSpacing(int row) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->rowSpacing(row);
}

void DuiGridLayoutPolicy::setColumnSpacing(int column, qreal spacing)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setColumnSpacing(column, spacing);
    invalidatePolicyAndLayout();
}

qreal DuiGridLayoutPolicy::columnSpacing(int column) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->columnSpacing(column);
}

void DuiGridLayoutPolicy::setRowAlignment(int row, Qt::Alignment alignment)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setRowAlignment(row, alignment);
    invalidatePolicyAndLayout();
}

Qt::Alignment DuiGridLayoutPolicy::rowAlignment(int row) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->rowAlignment(row);
}

void DuiGridLayoutPolicy::setColumnAlignment(int column, Qt::Alignment alignment)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setColumnAlignment(column, alignment);
    invalidatePolicyAndLayout();
}

Qt::Alignment DuiGridLayoutPolicy::columnAlignment(int column) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->columnAlignment(column);
}

void DuiGridLayoutPolicy::setRowStretchFactor(int row, int stretch)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setRowStretchFactor(row, stretch);
    invalidatePolicyAndLayout();
}

int DuiGridLayoutPolicy::rowStretchFactor(int row) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->rowStretchFactor(row);
}

void DuiGridLayoutPolicy::setColumnStretchFactor(int column, int stretch)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setColumnStretchFactor(column, stretch);
    invalidatePolicyAndLayout();
}

int DuiGridLayoutPolicy::columnStretchFactor(int column) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->columnStretchFactor(column);
}

void DuiGridLayoutPolicy::setRowMinimumHeight(int row, qreal height)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setRowMinimumHeight(row, height);
    invalidatePolicyAndLayout();
}

qreal DuiGridLayoutPolicy::rowMinimumHeight(int row) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->rowMinimumHeight(row);
}

void DuiGridLayoutPolicy::setRowPreferredHeight(int row, qreal height)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setRowPreferredHeight(row, height);
    invalidatePolicyAndLayout();
}

qreal DuiGridLayoutPolicy::rowPreferredHeight(int row) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->rowPreferredHeight(row);
}

void DuiGridLayoutPolicy::setRowMaximumHeight(int row, qreal height)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setRowMaximumHeight(row, height);
    invalidatePolicyAndLayout();
}

qreal DuiGridLayoutPolicy::rowMaximumHeight(int row) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->rowMaximumHeight(row);
}

void DuiGridLayoutPolicy::setRowFixedHeight(int row, qreal height)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setRowFixedHeight(row, height);
    invalidatePolicyAndLayout();
}

void DuiGridLayoutPolicy::setColumnMinimumWidth(int column, qreal width)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setColumnMinimumWidth(column, width);
    invalidatePolicyAndLayout();
}

qreal DuiGridLayoutPolicy::columnMinimumWidth(int column) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->columnMinimumWidth(column);
}

void DuiGridLayoutPolicy::setColumnPreferredWidth(int column, qreal width)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setColumnPreferredWidth(column, width);
    invalidatePolicyAndLayout();
}

qreal DuiGridLayoutPolicy::columnPreferredWidth(int column) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->columnPreferredWidth(column);
}

void DuiGridLayoutPolicy::setColumnMaximumWidth(int column, qreal width)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setColumnMaximumWidth(column, width);
    invalidatePolicyAndLayout();
}

qreal DuiGridLayoutPolicy::columnMaximumWidth(int column) const
{
    Q_D(const DuiGridLayoutPolicy);
    return d->engine->columnMaximumWidth(column);
}

void DuiGridLayoutPolicy::setColumnFixedWidth(int column, qreal width)
{
    Q_D(DuiGridLayoutPolicy);
    d->engine->setColumnFixedWidth(column, width);
    invalidatePolicyAndLayout();
}

void DuiGridLayoutPolicy::removeAt(int index)
{
    Q_D(DuiGridLayoutPolicy);
    if (index < 0 || index >= d->engine->count()) {
        duiWarning("DuiGridLayoutPolicy") << Q_FUNC_INFO << "invalid index" << index;
        return;
    }

    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(index);
    d->engine->removeAt(index);
    delete proxyItem;

    DuiAbstractLayoutPolicy::removeAt(index);
}
QSizeF DuiGridLayoutPolicy::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiGridLayoutPolicy);
    const_cast<DuiGridLayoutPolicyPrivate *>(d)->refreshEngine();
    return d->engine->sizeHint(which, constraint);
}

void DuiGridLayoutPolicy::relayout()
{
    Q_D(DuiGridLayoutPolicy);

    d->engine->invalidate();
    d->refreshEngine();
    d->engine->activate();
    if (!isActive())
        return;

    for (int i = d->engine->count() - 1; i >= 0; --i) {
        const QGraphicsLayoutItem *item = d->engine->itemAt(i);
        setItemGeometry(i, item->geometry());
    }
}

void DuiGridLayoutPolicy::invalidate()
{
    Q_D(DuiGridLayoutPolicy);
    //We need to invalidate the geometry of all the proxy items
    for (int i = d->engine->count() - 1; i >= 0; --i) {
        ProxyItem *item = static_cast<ProxyItem *>(d->engine->itemAt(i));
        item->updateGeometry();
    }
    d->engine->invalidate();
    DuiAbstractLayoutPolicy::invalidate();
}

void DuiGridLayoutPolicy::setHorizontalSpacing(qreal spacing)
{
    Q_D(DuiGridLayoutPolicy);
    DuiAbstractLayoutPolicy::setHorizontalSpacing(spacing);
    d->engine->setHorizontalSpacing(spacing);
}

void DuiGridLayoutPolicy::setVerticalSpacing(qreal spacing)
{
    Q_D(DuiGridLayoutPolicy);
    DuiAbstractLayoutPolicy::setVerticalSpacing(spacing);
    d->engine->setVerticalSpacing(spacing);
}

void DuiGridLayoutPolicy::setAlignment(QGraphicsLayoutItem *item, Qt::Alignment alignment)
{
    Q_D(DuiGridLayoutPolicy);
    //This will take O(N) time to find the index of the item
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));
    d->engine->setAlignment(proxyItem, alignment);
    invalidatePolicyAndLayout();
}

Qt::Alignment DuiGridLayoutPolicy::alignment(QGraphicsLayoutItem *item) const
{
    Q_D(const DuiGridLayoutPolicy);
    //This will take O(N) time to find the index of the item
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));
    return d->engine->alignment(proxyItem);
}
