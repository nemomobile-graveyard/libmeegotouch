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

#include <MWidgetRecycler>
#include <MSeparator>
#include <MList>
#include <MPannableViewport>
#include <MAbstractItemModel>

#include <QItemSelectionModel>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "mcontentitem.h"
#include "mlistindex.h"
#include "mabstractcellcreator.h"
#include "mlistview_p.h"
#include "mapplicationpageview_p.h"

#include "mlistview.h"
#include "animations/mbasiclistitemdeletionanimation.h"

using namespace MListViewPrivateNamespace;

static const int MOVINGDETECTORTIMEOUT = 500;
static const int SCROLLTOANIMATIONSNAPDISTANCE = 100;

MListViewPrivate::MListViewPrivate() : recycler(new MWidgetRecycler)
{
    itemHeight = 0;
    rowCount = 0;
    viewWidth = 0;
    model = NULL;
    selectionModel = NULL;
    moving = false;
    hseparator = NULL;
    headersCreator = NULL;
    hdrHeight = 0;
    forceRepaint = false;
    viewportTopLeft = QPointF();
    viewportVisibleHeight = 0;
    hseparatorHeight = 0;
    pannableViewport = NULL;
    clearVisibleOnRelayout = false;

    scrollToAnimation = new QPropertyAnimation(this);
    lastScrolledToFlatRow = -1;
    lastGeometrySize = QSizeF();
    isDeleted = false;

    itemDeletionAnimation = NULL;

    movingDetectorTimer.setSingleShot(true);
    connect(&movingDetectorTimer, SIGNAL(timeout()), this, SLOT(movingDetectionTimerTimeout()));
}

MListViewPrivate::~MListViewPrivate()
{
    deleteVisibleItemsArray();

    if(controllerModel)
        clearFirstAndLastVisibleRows();

    movingDetectorTimer.stop();

    delete hseparator;
    delete recycler;
    delete itemDeletionAnimation;
}

void MListViewPrivate::setSeparator(MWidget *separator)
{
    if(hseparator)
        delete hseparator;

    hseparator = separator;
    if(separator)
        hseparatorHeight = separator->preferredHeight();
    else
        hseparatorHeight = 0;
}

void MListViewPrivate::createSeparators()
{
    setSeparator(new MSeparator);
}

void MListViewPrivate::updateSeparators()
{
    if (hseparator)
        hseparator->setObjectName(q_ptr->style()->horizontalSeparatorObjectName());
}

void MListViewPrivate::updateHeaders()
{
    MDefaultHeadersCreator *defaultCreator = dynamic_cast<MDefaultHeadersCreator*>(headersCreator);
    if (defaultCreator)
        defaultCreator->setHeaderStyleName(q_ptr->style()->groupHeaderObjectName());
}

void MListViewPrivate::updateHeaderHeight()
{
}

void MListViewPrivate::updateRecyclerMaxItemsCount()
{
    if (itemHeight > 0)
        recycler->setMaxItemsPerClass(viewportVisibleHeight / itemHeight + 2);
}

void MListViewPrivate::setHeadersCreator(MCellCreator *creator)
{
    delete headersCreator;
    hdrHeight = 0;
    headersCreator = creator;
}

void MListViewPrivate::movingDetectionTimerTimeout()
{
    if (isAnimating())
        return;

    moving = false;
    controllerModel->setListIsMoving(false);
    movingDetectorTimer.stop();
}

void MListViewPrivate::clearVisibleItemsArray()
{
    foreach(MWidget * item, visibleItems) {
        deleteItem(item);
    }

    visibleItems.clear();
}

void MListViewPrivate::deleteVisibleItemsArray()
{
    qDeleteAll(visibleItems);

    visibleItems.clear();
}

void MListViewPrivate::destroy()
{
    isDeleted = true;
    deleteLater();
}

void MListViewPrivate::clearFirstAndLastVisibleRows()
{
    updateFirstVisibleRow(QModelIndex());
    updateLastVisibleRow(QModelIndex());
}

void MListViewPrivate::cellClicked(MWidget *source)
{
    MWidget *widget = source;
    QModelIndex cellIndex(locateVisibleIndexAt(widget->pos().y()));
    controller->selectItem(cellIndex);
}

void MListViewPrivate::cellLongTapped(const QModelIndex &index, const QPointF &position)
{
    controller->longTapItem(index, position);
}

void MListViewPrivate::selectionChange(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (clearVisibleOnRelayout)
        return;

    for (QHash<QModelIndex, MWidget *>::iterator iter = visibleItems.begin(); iter != visibleItems.end(); ++iter) {
        if (selected.contains(iter.key()))
            iter.value()->setSelected(true);
        if (deselected.contains(iter.key()))
            iter.value()->setSelected(false);
    }
}

bool MListViewPrivate::isAnimating()
{
    return (itemDeletionAnimation && itemDeletionAnimation->state() == QParallelAnimationGroup::Running);
}

void MListViewPrivate::removeRows(const QModelIndex &parent, int start, int end, bool animated)
{
    if (!animated || isAnimating() || !itemDeletionAnimation)
        return;

    int first = indexToFlatRow(controllerModel->firstVisibleItem());
    int last = indexToFlatRow(controllerModel->lastVisibleItem());

    if (parent.isValid()) {
        int parentFlatRow = indexToFlatRow(parent);
        start += parentFlatRow + 1;
        end += parentFlatRow + 1;
    }

    if (start > last || !controller->isVisible())
        return;

    start = first > start ? first : start;
    end = end > last ? last : end;
    // Set targets for deletion animation
    appendTargetsToDeleteAnimation(start, end, first, last);

    // Start item deletion animation
    itemDeletionAnimation->start();
}

void MListViewPrivate::appendTargetsToDeleteAnimation(int start, int end, int first, int last)
{
    QPointF offset(0,0);
    for (int flatRow = first; flatRow <= last; flatRow ++) {
        MWidget *cell = findCellAtRow(flatRow);
        if (cell) {
            if (flatRow < start)
                itemDeletionAnimation->appendBeforeTarget(cell);
            else if (flatRow > end) {
                itemDeletionAnimation->appendAfterTarget(cell, cell->pos() - offset);
            }
            else {
                itemDeletionAnimation->appendDeleteTarget(cell);
                offset += QPointF(0, cellSize(flatRow).height() + hseparatorHeight);
            }
        }
    }
    animatingItems = visibleItems.values().toVector();
    visibleItems.clear();
}

void MListViewPrivate::resetAnimatedWidgets()
{
    while (!animatingItems.isEmpty()) {
        delete animatingItems.front();
        animatingItems.pop_front();
    }

    q_ptr->layoutChanged();
    _q_relayoutItemsIfNeeded();
}

void MListViewPrivate::deleteItem(MWidget *widget)
{
    recycler->recycle(widget);
}

MWidget *MListViewPrivate::createCell(int row)
{
    QModelIndex index(flatRowToIndex(row));
    MWidget *cell = controllerModel->cellCreator()->createCell(index, *recycler);
    cell->setParent(NULL);
    cell->setParentItem(controller);
    cell->setVisible(true);

    if(cell->maximumHeight() < itemHeight)
    {
        cell->setMaximumSize(cell->maximumWidth(), itemHeight);
    } else if(cell->minimumHeight() > itemHeight)
    {
        cell->setMinimumSize(cell->minimumWidth(), itemHeight);
    }

    cell->resize(cellSize(row));

    // TODO this is not optimal, I'm pretty sure, need to find better way to keep
    // selection. Refactor into its own function.
    QItemSelectionModel *selectionModel = controllerModel->selectionModel();
    cell->setSelected(selectionModel->isSelected(index));

    // TODO this code can be executed only when panning is stopped. Refactor into
    // its own function
    if (cell->metaObject()->indexOfSignal("clicked()") != -1) {
        QObject::connect(cell, SIGNAL(clicked()), q_ptr, SLOT(itemClick()), Qt::UniqueConnection);
    }

    updateItemLongTapConnection(cell);

    return cell;
}

void MListViewPrivate::viewportRectChanged(const QRectF &viewportRect)
{
    if (isDeleted)
        return;

    if (viewportRect.topLeft() != oldViewportRectPosition) {
        movingDetectorTimer.start(MOVINGDETECTORTIMEOUT);

        if (!moving) {
            moving = true;
            controllerModel->setListIsMoving(true);
            lastScrolledToFlatRow = -1;
        }

        oldViewportRectPosition = viewportRect.topLeft();
    }
}

void MListViewPrivate::viewportPositionChanged(const QPointF &position)
{
    if (isDeleted)
        return;

    updateViewportRect(position - listPosition, QSizeF(viewWidth, pannableViewport->size().height()));
}

void MListViewPrivate::viewportSizeChanged(const QSizeF &size)
{
    if (isDeleted)
        return;

    updateViewportRect(viewportTopLeft, QSizeF(viewWidth, size.height()));
    updateScrollToTargetPosition();
    updateRecyclerMaxItemsCount();
}

void MListViewPrivate::viewportRangeChanged(const QRectF &range)
{
    Q_UNUSED(range);

    if (isDeleted)
        return;

    updateScrollToTargetPosition();
}

void MListViewPrivate::connectPannableViewport()
{
    disconnect(controller, SIGNAL(parentChanged()), this, SLOT(controllerParentChanged()));

    if (pannableViewport)
        pannableViewport->disconnect(this);

    connect(controller, SIGNAL(parentChanged()), SLOT(controllerParentChanged()));

    pannableViewport = MListViewPrivateNamespace::findParentWidgetOfType<MPannableViewport>(controller);
    if(pannableViewport) {
        updatePannableViewportPosition();

        connect(pannableViewport, SIGNAL(positionChanged(QPointF)), SLOT(viewportPositionChanged(QPointF)));
        connect(pannableViewport, SIGNAL(viewportSizeChanged(QSizeF)), SLOT(viewportSizeChanged(QSizeF)));
        connect(pannableViewport, SIGNAL(rangeChanged(QRectF)), SLOT(viewportRangeChanged(QRectF)));

        viewportTopLeft = pannableViewport->position() - listPosition;
        viewportVisibleHeight = pannableViewport->size().height();

        scrollToAnimation->setTargetObject(pannableViewport);
        scrollToAnimation->setPropertyName("position");
    }
}

void MListViewPrivate::controllerParentChanged()
{
    disconnect(controller, SIGNAL(parentChanged()), this, SLOT(controllerParentChanged()));
    connectPannableViewport();
}

void MListViewPrivate::updateListGeometry()
{
    if(q_ptr)
        q_ptr->updateGeometry();
}

void MListViewPrivate::updateViewportRect(const QPointF &position, const QSizeF &size)
{
    if (isDeleted)
        return;

    if ((viewportTopLeft != position) || (viewportVisibleHeight < size.height()) || (forceRepaint)) {
        forceRepaint = false;

        viewportTopLeft = position;
        viewportVisibleHeight = size.height();

        viewportRectChanged(QRectF(position, size));

        _q_relayoutItemsIfNeeded();
    }
}

void MListViewPrivate::updatePannableViewportPosition()
{
    if(!pannableViewport)
        connectPannableViewport();

    if(pannableViewport && controller) {
        QPointF oldListPosition = listPosition;

        listPosition = controller->mapToItem(pannableViewport->widget(), 0, 0);        
        viewportTopLeft += (oldListPosition - listPosition);

        listOffset = calculatePannableViewportOffset(listPosition);
        updateListIndexOffset();
    }
    else
        listPosition = QPointF(0,0);
}

QPointF MListViewPrivate::calculatePannableViewportOffset(const QPointF &listPosition)
{
    QPointF listOffset = listPosition;

    if (pannableViewport->widget() && pannableViewport->widget() != controller) {
        QList<QGraphicsItem *> pannableViewportWidgetChildren = pannableViewport->widget()->childItems();
        foreach (QGraphicsItem *item, pannableViewportWidgetChildren) {
            if (item->isWidget()) {
                MWidget *widget = dynamic_cast<MWidget *>(item);
                if (widget && widget->objectName() == MApplicationPageViewPrivate::TopSpacerName) {
                    listOffset.setY(listPosition.y() - widget->size().height());
                    break;
                }
            }
        }
    }

    return listOffset;
}

void MListViewPrivate::updateAnimations()
{
    delete itemDeletionAnimation;
    itemDeletionAnimation = 0;
    if (q_ptr->style()->deleteItemAnimation().isEmpty())
        return;

    itemDeletionAnimation = qobject_cast<MBasicListItemDeletionAnimation*>(MTheme::animation(q_ptr->style()->deleteItemAnimation()));
    if (itemDeletionAnimation)
        connect(itemDeletionAnimation, SIGNAL(finished()), this, SLOT(resetAnimatedWidgets()));
}

void MListViewPrivate::updateItemHeight()
{
    if (controllerModel->cellCreator()) {
        itemHeight = controllerModel->cellCreator()->cellSize().height();
        updateRecyclerMaxItemsCount();
    }
}

void MListViewPrivate::removeInvisibleItems(const QPoint &firstVisibleItemCoord,
        const QPoint &lastVisibleItemCoord)
{
    for (ModelIndexWidgetHash::iterator iter = visibleItems.begin(); iter != visibleItems.end();) {
        MWidget *cell = *iter;
        qreal cellPosY = cell->pos().y();

        if (cellPosY < firstVisibleItemCoord.y() || cellPosY > lastVisibleItemCoord.y()) {
            deleteItem(*iter);
            iter = visibleItems.erase(iter);
        } else {
            ++iter;
        }
    }
}

MWidget *MListViewPrivate::findCellAtRow(int row)
{
    foreach (MWidget * widget, visibleItems) {
        QPointF pos(widget->pos());
        int widgetRow = locateVisibleRowAt(pos.y(), pos.x());
        if (widgetRow == row)
            return widget;
    }

    return NULL;
}

void MListViewPrivate::createVisibleItems(int firstVisibleRow, int lastVisibleRow)
{
    for (int currentRow = firstVisibleRow; currentRow <= lastVisibleRow; currentRow++) {
        QModelIndex index = flatRowToIndex(currentRow);
        MWidget *cell = visibleItems.value(index, NULL);
        if (!cell) {
            cell = createItem(currentRow);
            visibleItems[index] = cell;
        }
        cell->setPos(QPointF(0, locatePosOfItem(currentRow)));
    }
}

void MListViewPrivate::disconnectSignalsFromModelToListView()
{
    if (model)
        model->disconnect(q_ptr);
    disconnect(controller, SIGNAL(visibleChanged()), q_ptr, SLOT(_q_relayoutItemsIfNeeded()));
}

void MListViewPrivate::connectSignalsFromModelToListView()
{
    if (model) {
        connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), q_ptr, SLOT(dataChanged(QModelIndex, QModelIndex)));

        if (model->inherits("MAbstractItemModel") || model->inherits("MSortFilterProxyModel")) {
            connect(model, SIGNAL(rowsInserted(QModelIndex, int, int, bool)), q_ptr, SLOT(rowsInserted(QModelIndex, int, int, bool)));
            connect(model, SIGNAL(rowsRemoved(QModelIndex, int, int, bool)), q_ptr, SLOT(rowsRemoved(QModelIndex, int, int, bool)));
        } else {
            connect(model, SIGNAL(rowsInserted(QModelIndex, int, int)), q_ptr, SLOT(rowsInserted(QModelIndex, int, int)));
            connect(model, SIGNAL(rowsRemoved(QModelIndex, int, int)), q_ptr, SLOT(rowsRemoved(QModelIndex, int, int)));
        }

        connect(model, SIGNAL(layoutChanged()), q_ptr, SLOT(layoutChanged()));
        connect(model, SIGNAL(modelReset()), q_ptr, SLOT(modelReset()));
        connect(model, SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)), q_ptr, SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));

        connect(controller, SIGNAL(visibleChanged()), q_ptr, SLOT(_q_relayoutItemsIfNeeded()));
    }
}

void MListViewPrivate::updateItemConnections()
{
    foreach (MWidget *cell, visibleItems) {
        updateItemLongTapConnection(cell);
    }
}

void MListViewPrivate::updateItemLongTapConnection(MWidget *cell)
{
    if (cell->metaObject()->indexOfSignal("longTapped(QPointF)") != -1) {
        if (controllerModel->longTapEnabled())
            QObject::connect(cell, SIGNAL(longTapped(QPointF)), q_ptr, SLOT(_q_itemLongTapped(QPointF)), Qt::UniqueConnection);
        else
            QObject::disconnect(cell, SIGNAL(longTapped(QPointF)), q_ptr, SLOT(_q_itemLongTapped(QPointF)));
    }
}

void MListViewPrivate::resetModel(MListModel *mListModel)
{
    forceRepaint = true;
    rowCount = 0;

    disconnectSignalsFromModelToListView();

    controllerModel = mListModel;
    model = mListModel->itemModel();
    if(model)
        rowCount = model->rowCount();
    clearVisibleItemsArray();
    updateItemHeight();
    clearFirstAndLastVisibleRows();

    connectSignalsFromModelToListView();
}

void MListViewPrivate::updateItemSize()
{
    foreach(MWidget * cell, visibleItems) {
        cell->resize(cellSize(0));
    }
}

QSizeF MListViewPrivate::cellSize(int row) const
{
    Q_UNUSED(row);
    return QSizeF(viewWidth, itemHeight);
}

void MListViewPrivate::updateSeparatorSize()
{
    if (hseparator) {
        hseparatorHeight = hseparator->preferredHeight();
        hseparator->setGeometry(QRectF(QPointF(0, 0), QSizeF(viewWidth, hseparatorHeight)));
    }
}

void MListViewPrivate::updateFirstVisibleRow(const QModelIndex &index)
{
    if (isDeleted)
        return;

    q_ptr->model()->setFirstVisibleItem(index);
}

void MListViewPrivate::updateLastVisibleRow(const QModelIndex &index)
{
    if (isDeleted)
        return;

    q_ptr->model()->setLastVisibleItem(index);
}

void MListViewPrivate::createVisibleItems()
{
    QModelIndex firstVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y()));
    int firstVisibleRow = indexToFlatRow(firstVisibleIndex);

    QModelIndex lastVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y() + viewportVisibleHeight));
    int lastVisibleRow = indexToFlatRow(lastVisibleIndex);

    createVisibleItems(firstVisibleRow, lastVisibleRow);
}

QModelIndex MListViewPrivate::locateLastVisibleIndexInRowAt(int pos)
{
    return locateVisibleIndexAt(pos);
}

void MListViewPrivate::replaceItem(MWidget* item, MWidget* newItem)
{
    newItem->setPos(item->pos());
    visibleItems[visibleItems.key(item)] = newItem;
    deleteItem(item);
}

bool MListViewPrivate::isGroupHeader(const QModelIndex &index)
{
    Q_UNUSED(index);

    return false;
}

void MListViewPrivate::layoutChanged()
{
    if(model)
        rowCount = model->rowCount();
    else
        rowCount = 0;
}

void MListViewPrivate::drawSeparators(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if (!controllerModel->firstVisibleItem().isValid() || !controllerModel->lastVisibleItem().isValid() || isAnimating())
        return;

    int firstRow = indexToFlatRow(controllerModel->firstVisibleItem());
    int lastRow = indexToFlatRow(controllerModel->lastVisibleItem());

    for (int currentRow = firstRow; currentRow <= lastRow; currentRow++) {
        drawSeparator(currentRow, painter, option);
    }
}

void MListViewPrivate::drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    drawHorizontalSeparator(row, painter, option);
}

void MListViewPrivate::drawHorizontalSeparator(int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(row == 0 || hseparatorHeight == 0)
        return;

    QPointF pos(-q_ptr->marginLeft(), locatePosOfItem(row) - hseparatorHeight - q_ptr->marginTop());

    painter->translate(pos.x(), pos.y());
    hseparator->paint(painter, option);
    painter->translate(-pos.x(), -pos.y());
}

QPointF MListViewPrivate::locateScrollToPosition(int row, MList::ScrollHint hint)
{
    if (!pannableViewport)
        return QPointF(0,0);

    int cellPosition = locatePosOfItem(row);
    QPointF targetPosition = pannableViewport->position();

    qreal pannableViewportHeight = pannableViewport->boundingRect().height() - listOffset.y();
    switch (hint) {
    case MList::PositionAtTopHint:
        targetPosition.setY(listOffset.y() + cellPosition);
        break;

    case MList::PositionAtBottomHint:
        targetPosition.setY(cellPosition + itemHeight + listOffset.y() - pannableViewportHeight);
        break;

    case MList::PositionAtCenterHint:
        targetPosition.setY(listOffset.y() + cellPosition + itemHeight / 2 - pannableViewportHeight / 2);
        break;

    case MList::EnsureVisibleHint:
        if (cellPosition <= pannableViewport->position().y()) {
           targetPosition.setY(listOffset.y() + cellPosition);
        } else if (cellPosition + itemHeight > pannableViewport->position().y() + pannableViewportHeight) {
           targetPosition.setY(cellPosition + itemHeight + listOffset.y() - pannableViewportHeight);
        }
        break;
    }

    int pannableWidgetBoundingHeight = pannableViewport->widget()->boundingRect().height();
    targetPosition.setY(qMax(targetPosition.y(), (qreal)0));
    if (pannableWidgetBoundingHeight > pannableViewportHeight)
        targetPosition.setY(qMin(targetPosition.y(), pannableWidgetBoundingHeight - pannableViewportHeight));
    else
        targetPosition = pannableViewport->position();

    return targetPosition;
}

void MListViewPrivate::_q_itemLongTapped(const QPointF &pos)
{
    q_ptr->longTap(pos);
}

void MListViewPrivate::_q_relayoutItemsIfNeeded()
{
    if (isDeleted)
        return;

    if (controller->isVisible())
        q_ptr->relayoutItemsInViewportRect();
}

void MListViewPrivate::updateScrollToTargetPosition()
{
    if (scrollToAnimation->state() == QPropertyAnimation::Running &&
            lastScrolledToFlatRow >= 0) {
        QPointF targetPosition = locateScrollToPosition(lastScrolledToFlatRow,
                                                        static_cast<MList::ScrollHint>(controllerModel->scrollHint()));
        if (targetPosition != scrollToAnimation->endValue().toPointF()) {
            if (targetPosition.y() > pannableViewport->position().y())
                scrollToAnimation->setStartValue(targetPosition + QPointF(0, SCROLLTOANIMATIONSNAPDISTANCE));
            else
                scrollToAnimation->setStartValue(targetPosition - QPointF(0, SCROLLTOANIMATIONSNAPDISTANCE));
            scrollToAnimation->setEndValue(targetPosition);
        }
    }
}

void MListViewPrivate::scrollToPos(const QPointF &targetPosition, MList::AnimationMode mode)
{
    if (scrollToAnimation->state() == QPropertyAnimation::Running) {
        updateScrollToTargetPosition();
        return;
    }
    if (mode == MList::Animated) {
        if (targetPosition.y() > pannableViewport->position().y())
            scrollToAnimation->setStartValue(targetPosition + QPointF(0, SCROLLTOANIMATIONSNAPDISTANCE));
        else
            scrollToAnimation->setStartValue(targetPosition - QPointF(0, SCROLLTOANIMATIONSNAPDISTANCE));
        scrollToAnimation->setEndValue(targetPosition);
        scrollToAnimation->setEasingCurve(QEasingCurve::OutCubic);
        scrollToAnimation->setDuration(100);
        scrollToAnimation->start();
    }
    else
        pannableViewport->setPosition(targetPosition);
}

void MListViewPrivate::scrollToLastIndex()
{
    if (isDeleted)
        return;

    q_ptr->scrollTo(lastScrolledToFlatRow,
                    static_cast<MList::ScrollHint>(controllerModel->scrollHint()),
                    static_cast<MList::AnimationMode>(controllerModel->animationMode()));
}

void MListViewPrivate::updateListIndexVisibility()
{
}

void MListViewPrivate::updateListIndexOffset()
{
}

void MListViewPrivate::updateListIndexStyle()
{
}

////////////
// Plain list
////////////
MPlainListViewPrivate::MPlainListViewPrivate()
{

}

MPlainListViewPrivate::~MPlainListViewPrivate()
{

}

int MPlainListViewPrivate::hseparatorsCount() const
{
    return itemsCount() - 1;
}

int MPlainListViewPrivate::totalHeight()
{
    int itemsCount = this->itemsCount();
    int separatorsCount = this->hseparatorsCount();
    int totalHeight = itemsCount * itemHeight + separatorsCount * hseparatorHeight;
    return totalHeight;
}

int MPlainListViewPrivate::itemsCount() const
{
    if (model == 0)
        return 0;

    return rowCount;
}

MWidget *MPlainListViewPrivate::createItem(int row)
{
    return createCell(row);
}

int MPlainListViewPrivate::indexToFlatRow(const QModelIndex &index) const
{
    return index.row();
}

QModelIndex MPlainListViewPrivate::flatRowToIndex(int row) const
{
    return model->index(row, 0);
}

int MPlainListViewPrivate::locateVisibleRowAt(int y, int x)
{
    Q_UNUSED(x);

    if (itemHeight == 0)
        return 0;

    // Formula for calculating position of specific row is following:
    // row * itemHeight + row * hseparatorHeight = pos
    // to calculate row lets do basic math:
    // row = pos / (hseparatorHeight + itemHeight)
    int row = y / (hseparatorHeight + itemHeight);

    int modelRowCount = rowCount;
    if (row >= modelRowCount)
        row = modelRowCount - 1;

    return row;
}

// TODO write unit test for this
int MPlainListViewPrivate::locatePosOfItem(int row)
{
    int itemHeights = row * itemHeight;
    int hseparatorHeights = 0;
    if (row > 0)
        hseparatorHeights = row * hseparatorHeight;

    return itemHeights + hseparatorHeights;
}

int MPlainListViewPrivate::locatePosOfItem(const QModelIndex &index)
{
    return locatePosOfItem(index.row());
}

QModelIndex MPlainListViewPrivate::locateVisibleIndexAt(int pos)
{
    int row = locateVisibleRowAt(pos);
    if (row < 0)
        return model->index(0, 0);

    return model->index(row, 0);
}

void MPlainListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    MListViewPrivate::createVisibleItems(firstVisibleRow.row(), lastVisibleRow.row());
}

////////////
// Plain list MultiColumn
////////////
MPlainMultiColumnListViewPrivate::MPlainMultiColumnListViewPrivate()
{
    vseparatorWidth = 0;
    vseparator = NULL;
}

MPlainMultiColumnListViewPrivate::~MPlainMultiColumnListViewPrivate()
{
    if(vseparator)
        delete vseparator;
}

void MPlainMultiColumnListViewPrivate::createSeparators()
{
    MListViewPrivate::createSeparators();

    //create vertical separators
    setVerticalSeparator(new MSeparator(NULL, Qt::Vertical));
}

void MPlainMultiColumnListViewPrivate::updateSeparators()
{
    MListViewPrivate::updateSeparators();
    if (vseparator)
        vseparator->setObjectName(q_ptr->style()->verticalSeparatorObjectName());
}

void MPlainMultiColumnListViewPrivate::updateRecyclerMaxItemsCount()
{
    if (itemHeight > 0)
        recycler->setMaxItemsPerClass((viewportVisibleHeight / itemHeight + 2) * controllerModel->columns());
}

void MPlainMultiColumnListViewPrivate::setVerticalSeparator(MWidget *separator)
{
    if(vseparator)
        delete vseparator;

    vseparator = separator;
    if(vseparator)
        vseparatorWidth = vseparator->preferredWidth();
    else
        vseparatorWidth = 0;
}

int MPlainMultiColumnListViewPrivate::itemsToRows(int items) const
{
    int columns = controllerModel->columns();
    int rows = items / columns;
    if (items > rows * columns)
        rows++;

    return rows;
}

int MPlainMultiColumnListViewPrivate::flatRowToColumn(int row) const
{
    int columns = controllerModel->columns();
    int itemRow = row / columns;
    int flatRowColumn = row - itemRow * columns;

    return flatRowColumn;
}

int MPlainMultiColumnListViewPrivate::locatePosOfItem(int row)
{
    int columns = controllerModel->columns();
    int rows = row / columns;
    int itemHeights = rows * itemHeight;
    int hseparatorHeights = 0;
    if (rows > 0)
        hseparatorHeights = rows * hseparatorHeight;

    return itemHeights + hseparatorHeights;
}


int MPlainMultiColumnListViewPrivate::hseparatorsCount() const
{
    return itemsToRows(itemsCount()) - 1;
}

int MPlainMultiColumnListViewPrivate::totalHeight()
{
    int rowsCount = itemsToRows(itemsCount());
    int hseparatorsCount = this->hseparatorsCount();
    int totalHeight = rowsCount * itemHeight + hseparatorsCount * hseparatorHeight;
    return totalHeight;
}

MWidget *MPlainMultiColumnListViewPrivate::createItem(int row)
{
    MWidget *cell = createCell(row);

    return cell;
}

QModelIndex MPlainMultiColumnListViewPrivate::locateLastVisibleIndexInRowAt(int pos)
{
    int lastVisibleFlatRow = locateVisibleRowAt(pos, viewWidth-1);
    return flatRowToIndex(lastVisibleFlatRow);
}

void MPlainMultiColumnListViewPrivate::replaceItem(MWidget* item, MWidget* newItem)
{
    MListViewPrivate::replaceItem(item, newItem);
    widgetFlatRows[newItem] = widgetFlatRows[item];
    widgetFlatRows[item] = 0;
}

int MPlainMultiColumnListViewPrivate::locateVisibleRowAt(int y, int x)
{
    if (itemHeight == 0)
        return 0;

    int columns = controllerModel->columns();
    int row = y / (hseparatorHeight + itemHeight) * columns;

    if (row >= itemsCount())
        row = itemsCount() - 1;

    int column = 0;
    if (viewWidth)
        column = qMin(x / (viewWidth / columns), columns - 1);

    int flatRow = row + column;
    if (flatRow >= itemsCount())
        flatRow = itemsCount() - 1;

    return flatRow;
}

void MPlainMultiColumnListViewPrivate::updateItemSize()
{
    foreach(MWidget * cell, visibleItems) {
        int cellRow = widgetFlatRows[cell] - 1;
        int cellColumn = flatRowToColumn(cellRow);
        cell->resize(cellSize(cellRow));
        cell->setPos(QPointF(cellColumn * viewWidth / controllerModel->columns(), cell->pos().y()));
    }
}

void MPlainMultiColumnListViewPrivate::updateSeparatorSize()
{
    MListViewPrivate::updateSeparatorSize();

    if (vseparator) {
        vseparatorWidth = vseparator->preferredWidth();
        vseparator->setGeometry(QRectF(QPointF(0, 0), QSizeF(vseparatorWidth, itemHeight)));
    }
}

QSizeF MPlainMultiColumnListViewPrivate::cellSize(int row) const
{
    Q_UNUSED(row);
    int columns = controllerModel->columns();
    return QSizeF(viewWidth / columns - vseparatorWidth, itemHeight);
}

void MPlainMultiColumnListViewPrivate::cellClicked(MWidget *source)
{
    int clickedFlatRow = widgetFlatRows.value(source) - 1;
    QModelIndex cellIndex(flatRowToIndex(clickedFlatRow));
    controller->selectItem(cellIndex);
}

void MPlainMultiColumnListViewPrivate::selectionChange(const QItemSelection &selected, const QItemSelection &deselected)
{
    foreach(MWidget * widget, visibleItems) {
        int widgetFlatRow = widgetFlatRows.value(widget) - 1;
        QModelIndex widgetIndex(flatRowToIndex(widgetFlatRow));
        if (selected.contains(widgetIndex))
            widget->setSelected(true);
        if (deselected.contains(widgetIndex))
            widget->setSelected(false);
    }
}

void MPlainMultiColumnListViewPrivate::createVisibleItems()
{
    QModelIndex firstVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y()));
    QModelIndex lastVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y() + viewportVisibleHeight));
    createVisibleItems(firstVisibleIndex, lastVisibleIndex);
}

void MPlainMultiColumnListViewPrivate::clearVisibleItemsArray()
{
    MListViewPrivate::clearVisibleItemsArray();
    widgetFlatRows.clear();
}

void MPlainMultiColumnListViewPrivate::removeInvisibleItems(const QPoint &firstVisibleItemCoord,
        const QPoint &lastVisibleItemCoord)
{
    for (ModelIndexWidgetHash::iterator iter = visibleItems.begin(); iter != visibleItems.end();) {
        MWidget *cell = *iter;
        qreal cellPosY = cell->pos().y();

        if (cellPosY < firstVisibleItemCoord.y() || cellPosY > lastVisibleItemCoord.y() || widgetFlatRows[*iter] > itemsCount()) {
            widgetFlatRows[*iter] = 0;
            deleteItem(*iter);
            iter = visibleItems.erase(iter);
        } else {
            ++iter;
        }
    }
}

void MPlainMultiColumnListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    int firstRow = firstVisibleRow.row();
    int lastRow = lastVisibleRow.row();

    if (!viewWidth || (!firstRow&&!lastRow&&itemsCount()>1)) // required for x position
        return;

    for (int currentRow = firstRow; currentRow <= lastRow; currentRow++) {
        MWidget *cell = findCellAtRow(currentRow);
        if (!widgetFlatRows[cell] && flatRowToColumn(currentRow) == 0) {

            // Create widgets to all columns in this row
            for (int column = 0; column < controllerModel->columns(); ++column) {
                QModelIndex index = flatRowToIndex(currentRow + column);
                cell = visibleItems.value(index, NULL);
                if (!cell) {
                    cell = createItem(currentRow + column);
                    visibleItems[index] = cell;
                }
                widgetFlatRows[cell] = currentRow + column + 1;
                cell->setPos(QPointF(column*(viewWidth / controllerModel->columns()), locatePosOfItem(currentRow + column)));
                if (currentRow + column + 1 == itemsCount() || flatRowToColumn(currentRow + column + 1) == 0)
                    break;
            }
        }
    }
}

void MPlainMultiColumnListViewPrivate::drawSeparator(int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    int columns = controllerModel->columns();
    int column = flatRowToColumn(row);

    if(row >= columns && column == 0) {
        drawHorizontalSeparator(row, painter, option);
        return;
    }

    if(column > 0)
        drawVerticalSeparator(row, column, painter, option);
}

void MPlainMultiColumnListViewPrivate::drawVerticalSeparator(int row, int column, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(vseparatorWidth == 0)
        return;

    int itemWidth = viewWidth / controllerModel->columns();

    QPointF pos(column*itemWidth - q_ptr->marginLeft() - vseparatorWidth, locatePosOfItem(row) - q_ptr->marginTop());
    painter->translate(pos.x(), pos.y());
    vseparator->paint(painter, option);
    painter->translate(-pos.x(), -pos.y());
}

void MPlainMultiColumnListViewPrivate::appendTargetsToDeleteAnimation(int start, int end, int first, int last)
{
    QPointF destination = findCellAtRow(start)->pos();
    for (int flatRow = first; flatRow <= last; flatRow ++) {
        MWidget *cell = findCellAtRow(flatRow);
        if (cell) {
            if (flatRow < start)
                itemDeletionAnimation->appendBeforeTarget(cell);
            else if (flatRow > end) {
                itemDeletionAnimation->appendAfterTarget(cell, destination);
                destination = cell->pos();
            }
            else
                itemDeletionAnimation->appendDeleteTarget(cell);
        }
    }
    animatingItems = visibleItems.values().toVector();
    visibleItems.clear();
}

////////////
// Group Header
////////////
MGroupHeaderListViewPrivate::MGroupHeaderListViewPrivate()
{
    listIndexWidget = NULL;
    gseparator = NULL;

    gseparatorHeight = 0;
}

MGroupHeaderListViewPrivate::~MGroupHeaderListViewPrivate()
{
    delete listIndexWidget;

    if (gseparator)
        delete gseparator;
}

void MGroupHeaderListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    MListViewPrivate::createVisibleItems(indexToFlatRow(firstVisibleRow), indexToFlatRow(lastVisibleRow));
}

QModelIndex MGroupHeaderListViewPrivate::locateVisibleIndexAt(int pos)
{
    int row = locateVisibleRowAt(pos);
    if (row < 0)
        return model->index(0, 0);

    return flatRowToIndex(row);
}

int MGroupHeaderListViewPrivate::hseparatorsCount() const
{
    int itemsCount = this->headersCount();
    int hseparators = 0;
    for (int i = 0; i < itemsCount; i++) {
        int itemsCountInGroup = this->itemsCount(i);
        if (itemsCountInGroup == 0)
            continue;

        hseparators += itemsCountInGroup - 1;
    }

    return hseparators;
}

void MGroupHeaderListViewPrivate::resetModel(MListModel *mListModel)
{
    MListViewPrivate::resetModel(mListModel);

    if (!listIndexWidget) {
        listIndexWidget = new MListIndex(controller);
        updateListIndexVisibility();
        updateListIndexStyle();
    }

    if (!controllerModel->headerCreator()) {
        if (!headersCreator)
            headersCreator = new MDefaultHeadersCreator(q_ptr->style()->groupHeaderObjectName());
        else
            updateHeaders();
    }

    headersPositions.resize(this->headersCount());
    updateHeaderHeight();
}

int MGroupHeaderListViewPrivate::locatePosOfItem(const QModelIndex &index)
{
    if (index.parent().isValid()) {
        return locatePosOfItem(index.parent().row(), index.row());
    } else {
        return locatePosOfItem(index.row(), -1);
    }
}

int MGroupHeaderListViewPrivate::locatePosOfItem(int headerIndex, int itemIndex)
{
    if (itemIndex == -1) {
        // we hitted header
        return headersPositions[headerIndex];
    }

    int pos = headersPositions[headerIndex] + headerHeight();
    if (itemIndex == 0)
        return pos;

    int itemHeights = itemIndex * itemHeight;
    int hseparatorHeights = 0;
    hseparatorHeights = itemIndex * hseparatorHeight;

    pos += hseparatorHeights + itemHeights;
    return pos;
}

int MGroupHeaderListViewPrivate::locatePosOfItem(int row)
{
    int headerIndex = dFindLowerIndex(headersRows, row);
    int relativeRow = row - headersRows[headerIndex] - 1; // we have to subtruct header
    return locatePosOfItem(headerIndex, relativeRow);
}

int MGroupHeaderListViewPrivate::locateVisibleRowAt(int y, int x)
{
    Q_UNUSED(x);

    if (itemHeight == 0)
        return 0;

    if (headersPositions.size() == 0)
        return 0;

    int headerIndex = dFindLowerIndex(headersPositions, y);

    int headerPosition = headersPositions[headerIndex];
    int headerRow = headersRows[headerIndex];
    int relativePos = y - headerPosition;
    int headerHeight = this->headerHeight();
    if (relativePos < headerHeight)
        return headerRow;

    int row = (relativePos - headerHeight) / (itemHeight + hseparatorHeight) + headerRow + 1;
    return row;
}

QModelIndex MGroupHeaderListViewPrivate::flatRowToIndex(int row) const
{
    if (!headersRows.contains(row)) {
        int headerIndex = dFindLowerIndex(headersRows, row);
        QModelIndex parent(model->index(headerIndex, 0));
        int relativeRow = row - headersRows[headerIndex] - 1;
        int itemsCount = this->itemsCount(headerIndex);

        // Check if header doesn't have any children
        if(itemsCount == 0)
            return parent;

        if (relativeRow >= itemsCount)
            relativeRow = itemsCount - 1;

        QModelIndex index(model->index(relativeRow, 0, parent));
        return index;
    }

    int headerIndex = headersRows.indexOf(row);
    return model->index(headerIndex, 0);
}

void MGroupHeaderListViewPrivate::updateHeadersPositions()
{
    int headersCount = this->headersCount();

    headersPositions.clear();
    if (headersCount == 0)
        return;

    int headerHeight = this->headerHeight();
    headersPositions << 0;
    int previousIndexPosition = 0;
    for (int i = 1; i < headersCount; i++) {
        int groupSize = this->groupSize(i - 1);
        headersPositions << previousIndexPosition + headerHeight + groupSize + gseparatorHeight;
        previousIndexPosition += headerHeight + groupSize + gseparatorHeight;
    }
}

void MGroupHeaderListViewPrivate::updateHeadersRows()
{
    int headersCount = this->headersCount();

    headersRows.clear();
    if (headersCount == 0)
        return;

    headersRows << 0;
    int prevGroupSize = 0;
    for (int i = 1; i < headersCount; i++) {
        prevGroupSize += itemsCount(i - 1);
        headersRows << (i + prevGroupSize);
    }
}

void MGroupHeaderListViewPrivate::updateHeadersIndexes()
{
    if(listIndexWidget) {
        QMap<QModelIndex, QString> shortcuts;
        for (int i = 0; i < headersCount(); i++) {
            QModelIndex headerRowIndex = flatRowToIndex(headersRows[i]);
            shortcuts[headerRowIndex] = model->data(headerRowIndex).toString();
        }
        listIndexWidget->setShortcuts(shortcuts);
    }
}

void MGroupHeaderListViewPrivate::setGroupSeparator(MWidget *separator)
{
    if(gseparator)
        delete gseparator;

    gseparator = separator;
    if(gseparator)
        gseparatorHeight = gseparator->preferredHeight();
    else
        gseparatorHeight = 0;
}

void MGroupHeaderListViewPrivate::createSeparators()
{
    MListViewPrivate::createSeparators();

    setGroupSeparator(new MSeparator);
}

void MGroupHeaderListViewPrivate::updateSeparators()
{
    MListViewPrivate::updateSeparators();

    if (gseparator)
        gseparator->setObjectName(q_ptr->style()->groupSeparatorObjectName());
}

void MGroupHeaderListViewPrivate::updateSeparatorSize()
{
    MListViewPrivate::updateSeparatorSize();

    if (gseparator) {
        gseparatorHeight = gseparator->preferredHeight();
        gseparator->setGeometry(QRectF(QPointF(0, 0), QSizeF(viewWidth, gseparatorHeight)));
    }
}

void MGroupHeaderListViewPrivate::updateHeaderHeight()
{
    updateHeadersPositions();
    updateHeadersRows();
    updateHeadersIndexes();
}

int MGroupHeaderListViewPrivate::indexToFlatRow(const QModelIndex &index) const
{
    if (!index.isValid())
        return -1;

    if (index.parent().isValid()) {
        // always need to add 1 because of parent.
        return headersRows[index.parent().row()] + index.row() + 1;
    }

    return headersRows[index.row()];
}

MWidget *MGroupHeaderListViewPrivate::createItem(int row)
{
    if (!headersRows.contains(row)) {
        return createCell(row);
    } else {
        int headerIndex = dFindLowerIndex(headersRows, row);
        return createHeader(headerIndex);
    }
}

MWidget *MGroupHeaderListViewPrivate::createHeader(int headerIndex)
{
    MWidget *header = headersCreator->createCell(model->index(headerIndex, 0), *recycler);
    header->setParent(NULL);
    header->setParentItem(controller);
    header->setVisible(true);
    header->resize(viewWidth, header->preferredHeight());
    return header;
}

int MGroupHeaderListViewPrivate::headerHeight()
{
    if (!hdrHeight) {
        MWidget *header = createHeader(0);
        hdrHeight = header->boundingRect().height();
        deleteItem(header);
    }
    return hdrHeight;
}

int MGroupHeaderListViewPrivate::headersCount() const
{
    return rowCount;
}

int MGroupHeaderListViewPrivate::itemsCount(int headerIndex) const
{
    QModelIndex index(model->index(headerIndex, 0));
    return model->rowCount(index);
}

int MGroupHeaderListViewPrivate::itemsCount() const
{
    if (!controllerModel->showGroups())
        return rowCount;

    int groupsCount = rowCount;
    int totalItemsCount = 0;
    for (int i = 0; i < groupsCount; i++) {
        totalItemsCount += itemsCount(i);
    }

    return totalItemsCount;
}

int MGroupHeaderListViewPrivate::groupSize(int headerIndex) const
{
    int itemsCount = this->itemsCount(headerIndex);
    return ((itemsCount * itemHeight) + (itemsCount - 1) * hseparatorHeight);
}

int MGroupHeaderListViewPrivate::totalHeight()
{
    int headersCount = this->headersCount();
    int itemsCount = this->itemsCount();
    int hseparatorsCount = this->hseparatorsCount();
    int totalHeight = headersCount * headerHeight() + itemsCount * itemHeight + hseparatorsCount * hseparatorHeight + gseparatorHeight * headersCount;
    return totalHeight;
}

QSizeF MGroupHeaderListViewPrivate::cellSize(int row) const
{
    if(headersRows.contains(row)) {
        return QSizeF(viewWidth, hdrHeight);
    }
    return MListViewPrivate::cellSize(row);
}

bool MGroupHeaderListViewPrivate::isGroupHeader(const QModelIndex &index)
{
    return !index.parent().isValid();
}

void MGroupHeaderListViewPrivate::createVisibleItems(int firstVisibleRow, int lastVisibleRow)
{
    MListViewPrivate::createVisibleItems(firstVisibleRow, lastVisibleRow);
}

void MGroupHeaderListViewPrivate::layoutChanged()
{
    MListViewPrivate::layoutChanged();

    updateHeaderHeight();
}

void MGroupHeaderListViewPrivate::drawSeparator(int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(headersRows.contains(row) || headersRows.contains(row - 1)) {
        if (headersRows.contains(row))
            drawGroupSeparator(row, painter, option);
        return;
    }

    drawHorizontalSeparator(row, painter, option);
}

void MGroupHeaderListViewPrivate::drawGroupSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(row == 0 || gseparatorHeight == 0)
        return;

    QPointF pos(-q_ptr->marginLeft(), locatePosOfItem(row) - gseparatorHeight - q_ptr->marginTop());

    painter->translate(pos.x(), pos.y());
    gseparator->paint(painter, option);
    painter->translate(-pos.x(), -pos.y());
}

void MGroupHeaderListViewPrivate::updateListIndexVisibility()
{
    if (listIndexWidget)
        listIndexWidget->setDisplayMode((MList::DisplayMode)controllerModel->listIndexDisplayMode());
}

void MGroupHeaderListViewPrivate::updateListIndexOffset()
{
    if (listIndexWidget)
        listIndexWidget->setOffset(pannableViewport->pos());
}

void MGroupHeaderListViewPrivate::appendTargetsToDeleteAnimation(int start, int end, int first, int last)
{
    MListViewPrivate::appendTargetsToDeleteAnimation(start, end, first, last);
}

void MGroupHeaderListViewPrivate::updateListIndexStyle()
{
    if (listIndexWidget)
        listIndexWidget->setStyleName(q_ptr->style()->listIndexStyleName());
}

////////////
// Group Header MultiColumn
////////////

MMultiColumnListViewPrivate::MMultiColumnListViewPrivate()
{
    vseparatorWidth = 0;
    vseparator = NULL;
}

MMultiColumnListViewPrivate::~MMultiColumnListViewPrivate()
{
    if(vseparator)
        delete vseparator;
}

void MMultiColumnListViewPrivate::setVerticalSeparator(MWidget *separator)
{
    if(vseparator)
        delete vseparator;

    vseparator = separator;
    if(vseparator)
        vseparatorWidth = vseparator->preferredWidth();
    else
        vseparatorWidth = 0;
}

void MMultiColumnListViewPrivate::createSeparators()
{
    MGroupHeaderListViewPrivate::createSeparators();
    setVerticalSeparator(new MSeparator(NULL, Qt::Vertical));
}

void MMultiColumnListViewPrivate::updateSeparators()
{
    MGroupHeaderListViewPrivate::updateSeparators();

    if (vseparator)
        vseparator->setObjectName(q_ptr->style()->verticalSeparatorObjectName());
}

void MMultiColumnListViewPrivate::updateRecyclerMaxItemsCount()
{
    if (itemHeight > 0)
        recycler->setMaxItemsPerClass((viewportVisibleHeight / itemHeight + 2) * controllerModel->columns());
}

int MMultiColumnListViewPrivate::itemsToRows(int items) const
{
    int columns = controllerModel->columns();
    int rows = items / columns;
    if (items > rows * columns)
        rows++;

    return rows;
}

int MMultiColumnListViewPrivate::rowsInGroup(int headerIndex) const
{
    int itemsInGroup = itemsCount(headerIndex);
    int rowsInGroup = itemsToRows(itemsInGroup);

    return rowsInGroup;
}

int MMultiColumnListViewPrivate::flatRowToColumn(int row) const
{
    if (headersRows.contains(row))
        return 0; // group headers are always in column 0

    int headerIndex = dFindLowerIndex(headersRows, row);

    if(headersRows.count() <= headerIndex)
        return 0;

    int relativeRow = row - headersRows[headerIndex] - 1;
    int columns = controllerModel->columns();
    int itemRow = relativeRow / columns;
    int flatRowColumn = relativeRow - itemRow * columns;

    return flatRowColumn;
}

void MMultiColumnListViewPrivate::updateItemSize()
{
    foreach(MWidget * cell, visibleItems) {
        int cellFlatRow = widgetFlatRows[cell] - 1;
        int cellColumn = flatRowToColumn(cellFlatRow);
        cell->resize(cellSize(cellFlatRow));
        cell->setPos(QPointF(cellColumn * viewWidth / controllerModel->columns(), cell->pos().y()));
    }
}

void MMultiColumnListViewPrivate::updateSeparatorSize()
{
    MGroupHeaderListViewPrivate::updateSeparatorSize();

    if (vseparator) {
        vseparatorWidth = vseparator->preferredWidth();
        vseparator->setGeometry(QRectF(QPointF(0, 0), QSizeF(vseparatorWidth, itemHeight)));
    }
}

QSizeF MMultiColumnListViewPrivate::cellSize(int row) const
{
    if(headersRows.contains(row)) {
        return MGroupHeaderListViewPrivate::cellSize(row);
    }

    int columns = controllerModel->columns();
    return QSizeF(viewWidth / columns - vseparatorWidth, itemHeight);
}

void MMultiColumnListViewPrivate::cellClicked(MWidget *source)
{
    int clickedFlatRow = widgetFlatRows.value(source) - 1;
    QModelIndex cellIndex(flatRowToIndex(clickedFlatRow));
    controller->selectItem(cellIndex);
}

void MMultiColumnListViewPrivate::selectionChange(const QItemSelection &selected, const QItemSelection &deselected)
{
    foreach(MWidget * widget, visibleItems) {
        int widgetFlatRow = widgetFlatRows.value(widget) - 1;
        QModelIndex widgetIndex(flatRowToIndex(widgetFlatRow));
        if (selected.contains(widgetIndex))
            widget->setSelected(true);
        if (deselected.contains(widgetIndex))
            widget->setSelected(false);
    }
}

void MMultiColumnListViewPrivate::createVisibleItems()
{
    QModelIndex firstVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y()));
    QModelIndex lastVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y() + viewportVisibleHeight));
    createVisibleItems(firstVisibleIndex, lastVisibleIndex);
}

void MMultiColumnListViewPrivate::clearVisibleItemsArray()
{
    MListViewPrivate::clearVisibleItemsArray();
    widgetFlatRows.clear();
}

void MMultiColumnListViewPrivate::removeInvisibleItems(const QPoint &firstVisibleItemCoord,
        const QPoint &lastVisibleItemCoord)
{
    for (ModelIndexWidgetHash::iterator iter = visibleItems.begin(); iter != visibleItems.end();) {
        MWidget *cell = *iter;
        qreal cellPosY = cell->pos().y();

        if (cellPosY < firstVisibleItemCoord.y() || cellPosY > lastVisibleItemCoord.y()/* || widgetFlatRows[*iter] > itemsCount()*/) {
            widgetFlatRows[*iter] = 0;
            deleteItem(*iter);
            iter = visibleItems.erase(iter);
        } else {
            ++iter;
        }
    }
}

void MMultiColumnListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    int firstRow = indexToFlatRow(firstVisibleRow);
    int lastRow = indexToFlatRow(lastVisibleRow);

    if (!viewWidth || (!firstRow&&!lastRow&&itemsCount()>1)) { // required for x position
        return;
    }

    for (int currentRow = firstRow; currentRow <= lastRow; currentRow++) {
        MWidget *cell = findCellAtRow(currentRow);
        if (!widgetFlatRows[cell] && flatRowToColumn(currentRow) == 0) {

            // Create widgets to all columns in this row
            for (int column = 0; column < controllerModel->columns(); ++column) {
                QModelIndex index = flatRowToIndex(currentRow + column);
                cell = visibleItems.value(index, NULL);
                if (!cell) {
                    cell = createItem(currentRow + column);
                    visibleItems[index] = cell;
                }
                widgetFlatRows[cell] = currentRow + column + 1;
                cell->setPos(QPointF(column*(viewWidth / controllerModel->columns()), locatePosOfItem(currentRow + column)));
                if (currentRow + column + 1 == itemsCount() + rowCount || flatRowToColumn(currentRow + column + 1) == 0)
                    break;
            }
        }
    }
}

int MMultiColumnListViewPrivate::locatePosOfItem(int headerIndex, int itemIndex)
{
    if (itemIndex == -1)
        return headersPositions[headerIndex];

    int pos = headersPositions[headerIndex] + headerHeight();
    if (itemIndex == 0)
        return pos;

    int rows = itemsToRows(itemIndex + 1) - 1; // rows after the 1st one
    int itemHeights = rows * itemHeight;
    int hseparatorHeights = rows * hseparatorHeight;
    pos += hseparatorHeights + itemHeights;

    return pos;
}

int MMultiColumnListViewPrivate::locatePosOfItem(int row)
{
    int headerIndex = dFindLowerIndex(headersRows, row);
    int relativeRow = row - headersRows[headerIndex] - 1;

    return locatePosOfItem(headerIndex, relativeRow);
}

int MMultiColumnListViewPrivate::locatePosOfItem(const QModelIndex &index)
{
    return MGroupHeaderListViewPrivate::locatePosOfItem(index);
}

int MMultiColumnListViewPrivate::hseparatorsCount() const
{
    int hseparatorsCount = 0;
    for (int i = 0; i < headersCount(); ++i) {
        hseparatorsCount += rowsInGroup(i) - 1;
    }

    return hseparatorsCount;
}

QModelIndex MMultiColumnListViewPrivate::locateLastVisibleIndexInRowAt(int pos)
{
    int lastVisibleFlatRow = locateVisibleRowAt(pos, viewWidth-1);
    return flatRowToIndex(lastVisibleFlatRow);
}

int MMultiColumnListViewPrivate::locateVisibleRowAt(int y, int x)
{
    Q_UNUSED(x);

    if (itemHeight == 0)
        return 0;

    int headerIndex = dFindLowerIndex(headersPositions, y);
    int headerRow = headersRows[headerIndex];
    int relativePos = y - headersPositions[headerIndex];
    int headerHeight = this->headerHeight();
    if (relativePos < headerHeight)
        return headerRow;

    relativePos -= headerHeight;
    int columns = controllerModel->columns();

    // row/columns * itemHeight + row/columns * hseparatorHeight = pos  ->  r/c*i + r/c*s = p  ->  r = p*c/(s+i)
    int row = relativePos / (hseparatorHeight + itemHeight)  * columns;
    int column = 0;
    if (viewWidth)
        column = qMin(x / (viewWidth / columns), columns - 1);
    return headerRow + row + column + 1;
}

MWidget *MMultiColumnListViewPrivate::createItem(int row)
{
    if (!headersRows.contains(row)) {
        MWidget *cell = createCell(row);
        return cell;
    } else {
        int headerIndex = dFindLowerIndex(headersRows, row);
        return createHeader(headerIndex);
    }
}

void MMultiColumnListViewPrivate::replaceItem(MWidget* item, MWidget* newItem)
{
    MListViewPrivate::replaceItem(item, newItem);
    widgetFlatRows[newItem] = widgetFlatRows[item];
    widgetFlatRows[item] = 0;
}

int MMultiColumnListViewPrivate::groupSize(int headerIndex) const
{
    int rows = rowsInGroup(headerIndex);
    int groupSize = rows * itemHeight + (rows - 1) * hseparatorHeight;

    return groupSize;
}

int MMultiColumnListViewPrivate::totalHeight()
{
    int totalHeight = 0;
    for (int i = 0; i < headersCount(); ++i) {
        totalHeight += headerHeight() + groupSize(i) + gseparatorHeight;
    }

    return totalHeight;
}

void MMultiColumnListViewPrivate::drawSeparator(int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(headersRows.contains(row)) {
        drawGroupSeparator(row, painter, option);
        return;
    }

    int columns = controllerModel->columns();
    int column = flatRowToColumn(row);
    if(row >= columns && column == 0) {
        drawHorizontalSeparator(row, painter, option);
        return;
    }

    if(column > 0) {
        drawVerticalSeparator(row, column, painter, option);
    }
}

void MMultiColumnListViewPrivate::drawVerticalSeparator(int row, int column, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(vseparatorWidth == 0)
        return;
    int itemWidth = viewWidth / controllerModel->columns();

    QPointF pos(column*itemWidth - vseparatorWidth - q_ptr->marginLeft(), locatePosOfItem(row) - q_ptr->marginTop());
    painter->translate(pos.x(), pos.y());
    vseparator->paint(painter, option);
    painter->translate(-pos.x(), -pos.y());
}

void MMultiColumnListViewPrivate::appendTargetsToDeleteAnimation(int start, int end, int first, int last)
{
    QPointF offset(0,0);
    QPointF destination = findCellAtRow(start)->pos();
    bool shifting = false;
    for (int flatRow = first; flatRow <= last; flatRow ++) {
        MWidget *cell = findCellAtRow(flatRow);
        if (cell) {
            QModelIndex index = flatRowToIndex(flatRow);
            if (flatRow < start)
                itemDeletionAnimation->appendBeforeTarget(cell);
            else if (flatRow > end) {
                if (isGroupHeader(index) && !shifting) {
                    int itemsInGroup = itemsCount(index.row() - 1);
                    int oldGroupRows = itemsToRows(itemsInGroup + (end - start + 1));
                    int newGroupRows = itemsToRows(itemsInGroup);
                    offset = QPointF(0, (oldGroupRows - newGroupRows) * (itemHeight + hseparatorHeight));
                    shifting = true;
                }
                if (!shifting) {
                    itemDeletionAnimation->appendAfterTarget(cell, destination);
                    destination = cell->pos();
                }
                else
                    itemDeletionAnimation->appendAfterTarget(cell, cell->pos() - offset);
            }
            else {
                itemDeletionAnimation->appendDeleteTarget(cell);
            }
        }
    }
    animatingItems = visibleItems.values().toVector();
    visibleItems.clear();
}
