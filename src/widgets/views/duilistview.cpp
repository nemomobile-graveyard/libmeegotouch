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

#include <QList>
#include <DuiDebug>
#include <QVector>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <DuiList>
#include <DuiWidgetView>
#include <DuiSeparator>
#include <DuiPannableViewport>

#include "duilistview.h"
#include "duilistview_p.h"

//// DuiListView /////
DuiListView::DuiListView(DuiWidgetController *widgetController) : DuiWidgetView(widgetController), d_ptr(NULL)
{
    controller = dynamic_cast<DuiList *>(widgetController);
}

DuiListView::~DuiListView()
{
    delete d_ptr;
}

void DuiListView::init()
{
    Q_ASSERT(controller);

    delete d_ptr;

    if (model()->columns() > 1) {
        if (model()->showGroups())
            d_ptr = new DuiMultiColumnListViewPrivate;
        else
            d_ptr = new DuiPlainMultiColumnListViewPrivate;
    } else {
        if (model()->showGroups())
            d_ptr = new DuiGroupHeaderListViewPrivate;
        else
            d_ptr = new DuiPlainListViewPrivate;
    }

    if (model()->showGroups())
        d_ptr->setHeadersCreator(new DuiDefaultHeadersCreator(style()->groupHeaderObjectName()));

    d_ptr->q_ptr = this;
    d_ptr->controller = dynamic_cast<DuiList *>(controller);
    d_ptr->setSeparator(new DuiSeparator);

    connectSelectionModel();

    d_ptr->resetModel(model());
}

void DuiListView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];

        if (member == DuiListModel::ItemModel || member == DuiListModel::ShowGroups || member == DuiListModel::Columns || member == DuiListModel::CellCreator) {
            if(model()->itemModel())
            {
                init();
                updateGeometry();
            }
        } else if (member == DuiListModel::SelectionModel) {
            connectSelectionModel();
        } else if (member == DuiListModel::ScrollToIndex) {
            scrollTo(model()->scrollToIndex(), static_cast<DuiList::ScrollHint>(model()->scrollHint()));
        }
    }
}

void DuiListView::connectSelectionModel()
{
    disconnect(this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    if (model()->selectionModel()) {
        connect(model()->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    }
}

void DuiListView::setupModel()
{
    DuiWidgetView::setupModel();

    init();
    updateGeometry();
}

void DuiListView::applyStyle()
{
    DuiWidgetView::applyStyle();

    if (d_ptr) {
        d_ptr->updateItemSize();
        d_ptr->updateSeparatorSize();
        d_ptr->setHeadersCreator(new DuiDefaultHeadersCreator(style()->groupHeaderObjectName()));
    }
}

QSizeF DuiListView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if (!d_ptr)
        return DuiWidgetView::sizeHint(which, constraint);

    return QSizeF(-1, d_ptr->totalHeight());
}

void DuiListView::setGeometry(const QRectF &rect)
{
    if (d_ptr) {
        d_ptr->viewWidth = rect.width();
        d_ptr->updateItemSize();
        d_ptr->updateSeparatorSize();
        relayoutItemsInExposedRect();
    }

    DuiWidgetView::setGeometry(rect);
}

void DuiListView::relayoutItemsInExposedRect()
{
    if (d_ptr->model && model()->cellCreator()) {
        int rowCount = d_ptr->model->rowCount();

        if(rowCount)
        {
            QModelIndex firstVisibleRow = d_ptr->locateVisibleIndexAt(d_ptr->viewportTopLeft.y());
            d_ptr->updateFirstVisibleRow(firstVisibleRow);
            QModelIndex lastVisibleRow = d_ptr->locateVisibleIndexAt(
                d_ptr->viewportTopLeft.y() + d_ptr->viewportVisibleHeight - 1); // - 1 to select from inside viewport
            d_ptr->updateLastVisibleRow(lastVisibleRow);

            QPoint firstVisibleItemPos(0, d_ptr->locatePosOfItem(firstVisibleRow));
            QPoint lastVisibleItemPos(0, d_ptr->locatePosOfItem(lastVisibleRow));
            d_ptr->removeInvisibleItems(firstVisibleItemPos, lastVisibleItemPos);

            d_ptr->createVisibleItems(firstVisibleRow, lastVisibleRow);
        } else {
            d_ptr->clearVisibleItemsArray();
        }
    }
}

void DuiListView::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);

    QRectF exposedRect(option->exposedRect);
    if ((d_ptr->viewportTopLeft != exposedRect.topLeft()) || (d_ptr->viewportVisibleHeight < exposedRect.height()) || (d_ptr->forceRepaint)) {
        d_ptr->forceRepaint = false;

        d_ptr->viewportTopLeft = exposedRect.topLeft();
        if (d_ptr->viewportVisibleHeight < exposedRect.height())
            d_ptr->viewportVisibleHeight = exposedRect.height();

        d_ptr->exposedRectChanged(exposedRect);
        const_cast<DuiListView *>(this)->relayoutItemsInExposedRect();
    }
}

void DuiListView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    // Temporarely disable separator drawing when group headers are shown
    if (d_ptr->separator->boundingRect().height() == 0 || model()->showGroups())
        return;

    // TODO move this code to private class
    for (int currentRow = model()->firstVisibleItem().row(); currentRow <= model()->lastVisibleItem().row(); currentRow++) {
        if (currentRow == 0)
            continue;

        QPointF itemPos(0, d_ptr->locatePosOfItem(currentRow));
        int separatorPos = itemPos.y() - d_ptr->separator->boundingRect().height();

        painter->translate(0, separatorPos);
        d_ptr->separator->paint(painter, option);
        painter->translate(0, -separatorPos);
    }
}

void DuiListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (!model()->firstVisibleItem().isValid() && !model()->lastVisibleItem().isValid())
        return;

    const DuiCellCreator *cellCreator = model()->cellCreator();

    int firstVisibleRow = d_ptr->indexToFlatRow(model()->firstVisibleItem());
    int lastVisibleRow = d_ptr->indexToFlatRow(model()->lastVisibleItem());
    int topLeftRow = d_ptr->indexToFlatRow(topLeft);
    int bottomRightRow = d_ptr->indexToFlatRow(bottomRight);

    int top = qMax(topLeftRow, firstVisibleRow);
    int bottom = qMin(bottomRightRow, lastVisibleRow);

    for (int i = top; i <= bottom; i++) {
        QModelIndex cellIndex = d_ptr->flatRowToIndex(i);
        if (!d_ptr->isGroupHeader(cellIndex)) {
            DuiWidget *cell = d_ptr->findCellAtRow(i);
            cellCreator->updateCell(cellIndex, cell);
        }
    }
}

/*!
 * This slot is called when items are inserted under the given \a parent.
 * The changed items are those from \a start to \a end inclusive.
 *
 * \sa QAbstractItemView::rowsInserted(), exposedRectUpdated()
 */
void DuiListView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

    layoutChanged();
}

/*!
 * This slot is called when items under the given \a parent are removed.
 * The removed items are those from \a start to \a end inclusive.
 *
 * \sa rowsInserted(), exposedRectUpdated()
 */
void DuiListView::rowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

    layoutChanged();
}

void DuiListView::layoutChanged()
{
    d_ptr->layoutChanged();

    updateGeometry();
    d_ptr->clearVisibleItemsArray();
}

void DuiListView::modelReset()
{
    layoutChanged();
}

void DuiListView::rowsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd,
                                const QModelIndex &destinationParent, int destinationRow)
{
    Q_UNUSED(sourceParent);
    Q_UNUSED(sourceStart);
    Q_UNUSED(sourceEnd);
    Q_UNUSED(destinationParent);
    Q_UNUSED(destinationRow);

    layoutChanged();
}

void DuiListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    d_ptr->selectionChange(selected, deselected);
}

void DuiListView::itemClick()
{
    QObject *s = sender();
    DuiWidget *senderWidget = qobject_cast<DuiWidget *>(s);
    if (senderWidget)
        d_ptr->cellClicked(senderWidget);
}

void DuiListView::scrollTo(const QModelIndex &index, DuiList::ScrollHint hint)
{
    if (index.isValid()) {
        int cellPosition = d_ptr->locatePosOfItem(index);

        DuiPannableViewport *pannableViewport =
            DuiListViewPrivateNamespace::findParentWidgetOfType<DuiPannableViewport>(controller);

        if (pannableViewport) {
            QPointF targetPosition = pannableViewport->position();
            QPointF listPosition(controller->mapToItem(pannableViewport->widget(), 0, 0));

            qreal pannableViewportHeight = pannableViewport->boundingRect().height();
            switch (hint) {
            case DuiList::PositionAtTopHint:
                targetPosition.setY(controller->pos().y() + cellPosition);
                break;

            case DuiList::PositionAtBottomHint:
                targetPosition.setY(cellPosition + d_ptr->itemHeight + listPosition.y() - pannableViewportHeight);
                break;

            case DuiList::PositionAtCenterHint:
                targetPosition.setY(listPosition.y() + cellPosition + d_ptr->itemHeight / 2 - pannableViewportHeight / 2);
                break;

            case DuiList::EnsureVisibleHint:
                if (cellPosition < d_ptr->viewportTopLeft.y()) {
                    targetPosition.setY(controller->pos().y() + cellPosition);
                } else if (cellPosition + d_ptr->itemHeight > d_ptr->viewportTopLeft.y() + pannableViewportHeight) {
                    targetPosition.setY(cellPosition + d_ptr->itemHeight + listPosition.y() - pannableViewportHeight);
                }

                break;
            }

            targetPosition.setY(qMax(targetPosition.y(), (qreal)0));
            targetPosition.setY(qMin(targetPosition.y(), pannableViewport->widget()->boundingRect().height() - pannableViewportHeight));

            pannableViewport->setPosition(targetPosition);
        }
    }
}

DUI_REGISTER_VIEW_NEW(DuiListView, DuiList)
