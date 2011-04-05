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

#include <QList>
#include <QVector>
#include <QPropertyAnimation>
#include <QStyleOptionGraphicsItem>

#include <QGestureEvent>
#include <QTapAndHoldGesture>

#include <MList>
#include <MWidgetView>
#include <MSeparator>
#include <MPannableViewport>

#include "mlistview.h"
#include "mlistview_p.h"

//// MListView /////
MListView::MListView(MWidgetController *widgetController)
    : MWidgetView(widgetController),
    d_ptr(NULL)
{
    controller = dynamic_cast<MList *>(widgetController);
}

MListView::~MListView()
{
    if (d_ptr) {
        if (!model()->headerCreator())
            delete d_ptr->headersCreator;

        delete d_ptr;
    }
}

void MListView::init()
{
    Q_ASSERT(controller);

    MCellCreator *headersCreator = NULL;

    if(d_ptr) {
        d_ptr->disconnectSignalsFromModelToListView();
        headersCreator = d_ptr->headersCreator;

        // Schedule deletion as we might have some events pending for delivery.
        d_ptr->destroy();
    }

    if (model()->columns() > 1) {
        if (model()->showGroups())
            d_ptr = new MMultiColumnListViewPrivate;
        else
            d_ptr = new MPlainMultiColumnListViewPrivate;
    } else {
        if (model()->showGroups())
            d_ptr = new MGroupHeaderListViewPrivate;
        else
            d_ptr = new MPlainListViewPrivate;
    }

    if (model()->headerCreator()) {
        d_ptr->setHeadersCreator(model()->headerCreator());
        if (model()->headerCreator() != headersCreator)
            delete headersCreator;
    } else
        d_ptr->setHeadersCreator(headersCreator);

    d_ptr->q_ptr = this;
    d_ptr->controller = dynamic_cast<MList *>(controller);
    d_ptr->createSeparators();
    d_ptr->updateSeparators();
    d_ptr->updateSeparatorSize();
    d_ptr->updateAnimations();

    connectSelectionModel();
    d_ptr->connectPannableViewport();

    d_ptr->resetModel(model());
}

void MListView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];

        if (member == MListModel::ItemModel || member == MListModel::ShowGroups || member == MListModel::Columns || member == MListModel::CellCreator ||
            member == MListModel::HeaderCreator) {
            if (model()->itemModel()) {
                init();
                if (d_ptr->pannableViewport) {
                    d_ptr->updateListGeometry();
                } else {
                    // Postpone the update of the list geometry to the next
                    // event loop, otherwise too many cells get created without
                    // having a pannable viewport.
                    QTimer::singleShot(0, d_ptr, SLOT(updateListGeometry()));
                }
            }
        } else if (member == MListModel::SelectionModel) {
            connectSelectionModel();
        } else if (member == MListModel::ScrollToIndex) {
            scrollTo(model()->scrollToIndex(),
                     static_cast<MList::ScrollHint>(model()->scrollHint()),
                     static_cast<MList::AnimationMode>(model()->animationMode()));
        } else if (member == MListModel::LongTap) {
            longTap(model()->longTap());
        } else if (member == MListModel::ListIndexDisplayMode) {
            d_ptr->updateListIndexVisibility();
        } else if (member == MListModel::LongTapEnabled) {
            d_ptr->updateItemConnections();
        }
    }
}

void MListView::connectSelectionModel()
{
    if (d_ptr->selectionModel)
        d_ptr->selectionModel->disconnect(this);

    if (model()->selectionModel()) {
        connect(model()->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    }

    d_ptr->selectionModel = model()->selectionModel();
}

void MListView::setupModel()
{
    MWidgetView::setupModel();

    init();
    updateGeometry();
}

void MListView::applyStyle()
{
    MWidgetView::applyStyle();

    if (d_ptr) {        
        d_ptr->clearVisibleItemsArray();
        d_ptr->updateAnimations();
        d_ptr->updateItemHeight();
        d_ptr->updateSeparators();
        d_ptr->updateSeparatorSize();
        d_ptr->updateHeaders();
        d_ptr->updateHeaderHeight();
        d_ptr->updateListIndexStyle();

        relayoutItemsInViewportRect();
    }
}

void MListView::notifyItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    MWidgetView::notifyItemChange(change, value);

    if(change == QGraphicsItem::ItemSceneHasChanged)
        emit controller->parentChanged();
}

QSizeF MListView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if (!d_ptr)
        return MWidgetView::sizeHint(which, constraint);

    return QSizeF(-1, d_ptr->totalHeight());
}

void MListView::setGeometry(const QRectF &rect)
{
    if (d_ptr) {
        if (!d_ptr->pannableViewport)
            d_ptr->viewportVisibleHeight = d_ptr->totalHeight();

        if (d_ptr->lastGeometrySize != size()) {
            d_ptr->viewWidth = rect.width();
            d_ptr->updatePannableViewportPosition();
            d_ptr->updateItemSize();
            d_ptr->updateSeparatorSize();
            relayoutItemsInViewportRect();

            d_ptr->scrollToLastIndex();
            d_ptr->lastGeometrySize = size();
        }
    }

    MWidgetView::setGeometry(rect);
}

void MListView::relayoutItemsInViewportRect()
{
    if (d_ptr->clearVisibleOnRelayout) {
        d_ptr->clearVisibleItemsArray();
        d_ptr->clearFirstAndLastVisibleRows();
        d_ptr->clearVisibleOnRelayout = false;
    }

    if (d_ptr->model && model()->cellCreator()) {
        int rowCount = d_ptr->rowCount;

        if (rowCount) {
            QModelIndex firstVisibleIndex = d_ptr->locateVisibleIndexAt(d_ptr->viewportTopLeft.y());
            d_ptr->updateFirstVisibleRow(firstVisibleIndex);
            QModelIndex lastVisibleIndex = d_ptr->locateLastVisibleIndexInRowAt(d_ptr->viewportTopLeft.y() + d_ptr->viewportVisibleHeight);
            d_ptr->updateLastVisibleRow(lastVisibleIndex);

            QPoint firstVisibleItemPos(0, d_ptr->locatePosOfItem(firstVisibleIndex));
            QPoint lastVisibleItemPos(0, d_ptr->locatePosOfItem(lastVisibleIndex));
            d_ptr->removeInvisibleItems(firstVisibleItemPos, lastVisibleItemPos);

            d_ptr->createVisibleItems(firstVisibleIndex, lastVisibleIndex);
        } else {
            d_ptr->clearVisibleItemsArray();
            d_ptr->clearFirstAndLastVisibleRows();
        }
    }
}

void MListView::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MListView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    if (!d_ptr->clearVisibleOnRelayout)
        d_ptr->drawSeparators(painter, option);
}

void MListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (!model()->firstVisibleItem().isValid() && !model()->lastVisibleItem().isValid())
        return;

    if (d_ptr->controller->isVisible()) {
        const MCellCreator *cellCreator = model()->cellCreator();

        int firstVisibleRow = d_ptr->indexToFlatRow(model()->firstVisibleItem());
        int lastVisibleRow = d_ptr->indexToFlatRow(model()->lastVisibleItem());
        int topLeftRow = d_ptr->indexToFlatRow(topLeft);
        int bottomRightRow = d_ptr->indexToFlatRow(bottomRight);

        int top = qMax(topLeftRow, firstVisibleRow);
        int lastCellInLastVisibleRow = lastVisibleRow + model()->columns() - lastVisibleRow % model()->columns() - 1;
        int bottom = qMin(bottomRightRow, lastCellInLastVisibleRow);

        for (int i = top; i <= bottom; i++) {
            QModelIndex cellIndex = d_ptr->flatRowToIndex(i);
            if (!d_ptr->isGroupHeader(cellIndex)) {
                MWidget *cell = d_ptr->findCellAtRow(i);
                if( (controller->optimizationFlags() & MList::DontCallCreateCellDuringUpdate) == MList::DontCallCreateCellDuringUpdate) {
                    cellCreator->updateCell(cellIndex, cell);
                } else {
                    MWidget* newCell = d_ptr->createCell(i);
                    d_ptr->replaceItem(cell, newCell);
                }
            }
        }
    } else {
        d_ptr->clearVisibleOnRelayout = true;
    }
}

/*!
 * This slot is called when items are inserted under the given \a parent.
 * The changed items are those from \a start to \a end inclusive.
 *
 * \sa QAbstractItemView::rowsInserted(), exposedRectUpdated()
 */
void MListView::rowsInserted(const QModelIndex &parent, int start, int end, bool animated)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
    Q_UNUSED(animated);

    layoutChanged();
}

/*!
 * This slot is called when items under the given \a parent are removed.
 * The removed items are those from \a start to \a end inclusive.
 *
 * \sa rowsInserted(), exposedRectUpdated()
 */
void MListView::rowsRemoved(const QModelIndex &parent, int start, int end, bool animated)
{
    d_ptr->removeRows(parent, start, end, animated);

    if (!animated)
        layoutChanged();
}

void MListView::layoutChanged()
{
    if (!d_ptr->isAnimating()) {
        d_ptr->layoutChanged();

        updateGeometry();
        d_ptr->clearVisibleOnRelayout = true;
        d_ptr->lastScrolledToFlatRow = -1;
    }
}

void MListView::modelReset()
{
    layoutChanged();
}

void MListView::rowsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd,
                                const QModelIndex &destinationParent, int destinationRow)
{
    Q_UNUSED(sourceParent);
    Q_UNUSED(sourceStart);
    Q_UNUSED(sourceEnd);
    Q_UNUSED(destinationParent);
    Q_UNUSED(destinationRow);

    layoutChanged();
}

void MListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    d_ptr->selectionChange(selected, deselected);
}

void MListView::itemClick()
{
    QObject *s = sender();
    MWidget *senderWidget = qobject_cast<MWidget *>(s);
    if (senderWidget)
        d_ptr->cellClicked(senderWidget);
}

void MListView::scrollTo(const QModelIndex &index, MList::ScrollHint hint, MList::AnimationMode mode)
{
    if (index.isValid()) {
        int row = d_ptr->indexToFlatRow(index);
        scrollTo(row, hint, mode);
    }
}

void MListView::scrollTo(int row, MList::ScrollHint hint, MList::AnimationMode mode)
{
    if (row >= 0) {
        d_ptr->lastScrolledToFlatRow = row;
        if (d_ptr->pannableViewport) {
            QPointF targetPosition = d_ptr->locateScrollToPosition(row, hint);

            if (targetPosition != d_ptr->pannableViewport->position())
                d_ptr->scrollToPos(targetPosition, mode);
        }
    }
}

void MListView::longTap(const QPointF &pos)
{
    QPointF relativePos = d_ptr->controller->mapFromScene(pos);
    QModelIndex index = d_ptr->flatRowToIndex(d_ptr->locateVisibleRowAt(relativePos.y(), relativePos.x()));
    d_ptr->cellLongTapped(index, pos);
}

#include "moc_mlistview.cpp"

M_REGISTER_VIEW_NEW(MListView, MList)
