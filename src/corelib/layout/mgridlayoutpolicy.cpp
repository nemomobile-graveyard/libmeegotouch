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

#include "mgridlayoutpolicy.h"
#include "mgridlayoutpolicy_p.h"
#include "mabstractlayoutpolicystyle.h"
#include "mlayout.h"
#include "mdebug.h"

#include "mlayouthelper_p.h"

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsGridLayout>
MGridLayoutPolicy::MGridLayoutPolicy(MLayout *layout) :
    MAbstractLayoutPolicy(*(new MGridLayoutPolicyPrivate(layout)))
{
    Q_D(MGridLayoutPolicy);
    d->engine->setHorizontalSpacing(horizontalSpacing());
    d->engine->setVerticalSpacing(verticalSpacing());
}


MGridLayoutPolicy::~MGridLayoutPolicy()
{ }

int MGridLayoutPolicy::rowCount() const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->rowCount();
}

int MGridLayoutPolicy::columnCount() const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->columnCount();
}


QGraphicsLayoutItem *MGridLayoutPolicy::itemAt(int row, int col) const
{
    Q_D(const MGridLayoutPolicy);
    ProxyItem *item = static_cast<ProxyItem *>(d->engine->itemAt(row, col));
    if (!item)
        return NULL;

    return item->proxiedItem();
}

void MGridLayoutPolicy::addItem(QGraphicsLayoutItem *item, int row, int column,
                                  int rowSpan, int columnSpan, Qt::Alignment alignment)
{
    Q_D(MGridLayoutPolicy);

    if (row < 0 || column < 0) {
        mWarning("MGridLayoutPolicy") << Q_FUNC_INFO << "invalid row/column:" << (row < 0 ? row : column);
        return;
    }
    if (columnSpan < 1 || rowSpan < 1) {
        mWarning("MGridLayoutPolicy") << Q_FUNC_INFO << "invalid row span/column span:" << (rowSpan < 1 ? rowSpan : columnSpan);
        return;
    }
    if (!item) {
        mWarning("MGridLayoutPolicy") << Q_FUNC_INFO << "cannot set data for null item";
        return;
    }
    ProxyItem *proxyItem = new ProxyItem(item);
    d->engine->addItem(proxyItem, row, column, rowSpan, columnSpan, alignment);

    // Register new item with the policy and layout
    MAbstractLayoutPolicy::addItem(item);
}

void MGridLayoutPolicy::setRowSpacing(int row, qreal spacing)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setRowSpacing(row, spacing);
    invalidatePolicyAndLayout();
}

qreal MGridLayoutPolicy::rowSpacing(int row) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->rowSpacing(row);
}

void MGridLayoutPolicy::setColumnSpacing(int column, qreal spacing)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setColumnSpacing(column, spacing);
    invalidatePolicyAndLayout();
}

qreal MGridLayoutPolicy::columnSpacing(int column) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->columnSpacing(column);
}

void MGridLayoutPolicy::setRowAlignment(int row, Qt::Alignment alignment)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setRowAlignment(row, alignment);
    invalidatePolicyAndLayout();
}

Qt::Alignment MGridLayoutPolicy::rowAlignment(int row) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->rowAlignment(row);
}

void MGridLayoutPolicy::setColumnAlignment(int column, Qt::Alignment alignment)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setColumnAlignment(column, alignment);
    invalidatePolicyAndLayout();
}

Qt::Alignment MGridLayoutPolicy::columnAlignment(int column) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->columnAlignment(column);
}

void MGridLayoutPolicy::setRowStretchFactor(int row, int stretch)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setRowStretchFactor(row, stretch);
    invalidatePolicyAndLayout();
}

int MGridLayoutPolicy::rowStretchFactor(int row) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->rowStretchFactor(row);
}

void MGridLayoutPolicy::setColumnStretchFactor(int column, int stretch)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setColumnStretchFactor(column, stretch);
    invalidatePolicyAndLayout();
}

int MGridLayoutPolicy::columnStretchFactor(int column) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->columnStretchFactor(column);
}

void MGridLayoutPolicy::setRowMinimumHeight(int row, qreal height)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setRowMinimumHeight(row, height);
    invalidatePolicyAndLayout();
}

qreal MGridLayoutPolicy::rowMinimumHeight(int row) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->rowMinimumHeight(row);
}

void MGridLayoutPolicy::setRowPreferredHeight(int row, qreal height)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setRowPreferredHeight(row, height);
    invalidatePolicyAndLayout();
}

qreal MGridLayoutPolicy::rowPreferredHeight(int row) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->rowPreferredHeight(row);
}

void MGridLayoutPolicy::setRowMaximumHeight(int row, qreal height)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setRowMaximumHeight(row, height);
    invalidatePolicyAndLayout();
}

qreal MGridLayoutPolicy::rowMaximumHeight(int row) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->rowMaximumHeight(row);
}

void MGridLayoutPolicy::setRowFixedHeight(int row, qreal height)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setRowFixedHeight(row, height);
    invalidatePolicyAndLayout();
}

void MGridLayoutPolicy::setColumnMinimumWidth(int column, qreal width)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setColumnMinimumWidth(column, width);
    invalidatePolicyAndLayout();
}

qreal MGridLayoutPolicy::columnMinimumWidth(int column) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->columnMinimumWidth(column);
}

void MGridLayoutPolicy::setColumnPreferredWidth(int column, qreal width)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setColumnPreferredWidth(column, width);
    invalidatePolicyAndLayout();
}

qreal MGridLayoutPolicy::columnPreferredWidth(int column) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->columnPreferredWidth(column);
}

void MGridLayoutPolicy::setColumnMaximumWidth(int column, qreal width)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setColumnMaximumWidth(column, width);
    invalidatePolicyAndLayout();
}

qreal MGridLayoutPolicy::columnMaximumWidth(int column) const
{
    Q_D(const MGridLayoutPolicy);
    return d->engine->columnMaximumWidth(column);
}

void MGridLayoutPolicy::setColumnFixedWidth(int column, qreal width)
{
    Q_D(MGridLayoutPolicy);
    d->engine->setColumnFixedWidth(column, width);
    invalidatePolicyAndLayout();
}

void MGridLayoutPolicy::removeAt(int index)
{
    Q_D(MGridLayoutPolicy);
    if (index < 0 || index >= d->engine->count()) {
        mWarning("MGridLayoutPolicy") << Q_FUNC_INFO << "invalid index" << index;
        return;
    }

    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(index);
    d->engine->removeAt(index);
    delete proxyItem;

    MAbstractLayoutPolicy::removeAt(index);
}
QSizeF MGridLayoutPolicy::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MGridLayoutPolicy);

    d->engine->setMinimumSize(QSizeF(-1, -1));
    d->engine->setMaximumSize(QSizeF(-1, -1));
    const_cast<MGridLayoutPolicyPrivate *>(d)->refreshEngine();
    return d->engine->sizeHint(which, constraint);
}

void MGridLayoutPolicy::relayout()
{
    Q_D(MGridLayoutPolicy);
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

void MGridLayoutPolicy::invalidate()
{
    Q_D(MGridLayoutPolicy);
    d->engine->invalidate();

    MAbstractLayoutPolicy::invalidate();
}

void MGridLayoutPolicy::setHorizontalSpacing(qreal spacing)
{
    Q_D(MGridLayoutPolicy);
    MAbstractLayoutPolicy::setHorizontalSpacing(spacing);
    d->engine->setHorizontalSpacing(spacing);
}

void MGridLayoutPolicy::setVerticalSpacing(qreal spacing)
{
    Q_D(MGridLayoutPolicy);
    MAbstractLayoutPolicy::setVerticalSpacing(spacing);
    d->engine->setVerticalSpacing(spacing);
}

void MGridLayoutPolicy::setAlignment(QGraphicsLayoutItem *item, Qt::Alignment alignment)
{
    Q_D(MGridLayoutPolicy);
    //This will take O(N) time to find the index of the item
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));
    d->engine->setAlignment(proxyItem, alignment);
    invalidatePolicyAndLayout();
}

Qt::Alignment MGridLayoutPolicy::alignment(QGraphicsLayoutItem *item) const
{
    Q_D(const MGridLayoutPolicy);
    //This will take O(N) time to find the index of the item
    QGraphicsLayoutItem *proxyItem = d->engine->itemAt(indexOf(item));
    return d->engine->alignment(proxyItem);
}
