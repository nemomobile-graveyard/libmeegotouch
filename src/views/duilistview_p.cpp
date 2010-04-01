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

#include <DuiWidgetRecycler>
#include <DuiSeparator>
#include <DuiList>

#include <QItemSelectionModel>

#include "duicontentitem.h"
#include "duiabstractcellcreator.h"
#include "duilistview_p.h"

#include "duilistview.h"

using namespace DuiListViewPrivateNamespace;

static const int MOVINGDETECTORTIMEOUT = 500;

DuiListViewPrivate::DuiListViewPrivate() : recycler(new DuiWidgetRecycler)
{
    itemHeight = 0;
    viewWidth = 0;
    model = NULL;
    moving = false;
    hseparator = NULL;
    headersCreator = NULL;
    hdrHeight = NULL;
    forceRepaint = false;
    viewportTopLeft = QPointF();
    viewportVisibleHeight = 0;
    hseparatorHeight = 0;
    movingDetectorTimer.setSingleShot(true);
    connect(&movingDetectorTimer, SIGNAL(timeout()), this, SLOT(movingDetectionTimerTimeout()));
}

DuiListViewPrivate::~DuiListViewPrivate()
{
    clearVisibleItemsArray();
    movingDetectorTimer.stop();
    delete headersCreator;
    delete hseparator;
    delete recycler;
}

void DuiListViewPrivate::setSeparator(DuiWidget *separator)
{
    if(hseparator)
        delete hseparator;

    hseparator = separator;
    if(separator)
        hseparatorHeight = separator->preferredHeight();
    else
        hseparatorHeight = 0;
}

void DuiListViewPrivate::createSeparators()
{
    setSeparator(new DuiSeparator);
}

void DuiListViewPrivate::updateSeparators()
{
    if(hseparator)
        hseparator->setObjectName(q_ptr->style()->horizontalSeparatorObjectName());
}

void DuiListViewPrivate::setHeadersCreator(DuiCellCreator *creator)
{
    delete headersCreator;
    headersCreator = creator;
}

void DuiListViewPrivate::movingDetectionTimerTimeout()
{
    moving = false;
    controllerModel->setListIsMoving(false);
    movingDetectorTimer.stop();
}

void DuiListViewPrivate::clearVisibleItemsArray()
{
    foreach(DuiWidget * item, visibleItems) {
        deleteItem(item);
    }

    visibleItems.clear();

    updateFirstVisibleRow(QModelIndex());
    updateLastVisibleRow(QModelIndex());
}

void DuiListViewPrivate::cellClicked(DuiWidget *source)
{
    DuiWidget *widget = source;
    QModelIndex cellIndex(locateVisibleIndexAt(widget->pos().y()));
    controller->selectItem(cellIndex);
}

void DuiListViewPrivate::selectionChange(const QItemSelection &selected, const QItemSelection &deselected)
{
    foreach(DuiWidget * widget, visibleItems) {
        // TODO convert into hashmap
        QModelIndex widgetIndex(locateVisibleIndexAt(widget->y()));
        if (selected.contains(widgetIndex))
            widget->setSelected(true);
        if (deselected.contains(widgetIndex))
            widget->setSelected(false);
    }
}

void DuiListViewPrivate::deleteItem(DuiWidget *widget)
{
    recycler->recycle(widget);
}

DuiWidget *DuiListViewPrivate::createCell(int row)
{
    const DuiCellCreator *cellCreator = controllerModel->cellCreator();
    QModelIndex index(flatRowToIndex(row));
    DuiWidget *cell = cellCreator->createCell(index, *recycler);
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
    // selection. Refactor into it's own function.
    QItemSelectionModel *selectionModel = controllerModel->selectionModel();
    cell->setSelected(selectionModel->isSelected(index));

    // TODO this code can be executed only when panning is stopped. Refactor into
    // it's own function
    if (cell->metaObject()->indexOfSignal("clicked()") != -1) {
        QObject::connect(cell, SIGNAL(clicked()), q_ptr, SLOT(itemClick()), Qt::UniqueConnection);
    }

    return cell;
}

void DuiListViewPrivate::exposedRectChanged(const QRectF &exposedRect)
{
    if (exposedRect.topLeft() != oldExposedRectPosition) {
        movingDetectorTimer.start(MOVINGDETECTORTIMEOUT);

        if (!moving) {
            moving = true;
            controllerModel->setListIsMoving(true);
        }

        oldExposedRectPosition = exposedRect.topLeft();
    }
}

void DuiListViewPrivate::updateItemHeight()
{
    const DuiCellCreator *cellCreator = controller->cellCreator();
    if (cellCreator)
        itemHeight = cellCreator->cellSize().height();
}

void DuiListViewPrivate::removeInvisibleItems(const QPoint &firstVisibleItemCoord,
        const QPoint &lastVisibleItemCoord)
{
    for (QVector<DuiWidget *>::iterator iter = visibleItems.begin(); iter != visibleItems.end();) {
        DuiWidget *cell = *iter;
        qreal cellPosY = cell->pos().y();

        if (cellPosY < firstVisibleItemCoord.y() || cellPosY > lastVisibleItemCoord.y()) {
            deleteItem(*iter);
            iter = visibleItems.erase(iter);
        } else {
            ++iter;
        }
    }
}

DuiWidget *DuiListViewPrivate::findCellAtRow(int row)
{
    foreach(DuiWidget * widget, visibleItems) {
        QPointF pos(widget->pos());
        int widgetRow = locateVisibleRowAt(pos.y(), pos.x());
        if (widgetRow == row)
            return widget;
    }

    return NULL;
}

void DuiListViewPrivate::createVisibleItems(int firstVisibleRow, int lastVisibleRow)
{
    for (int currentRow = firstVisibleRow; currentRow <= lastVisibleRow; currentRow++) {
        DuiWidget *existingCell = findCellAtRow(currentRow);
        if (existingCell != NULL)
            continue;

        DuiWidget *cell = createItem(currentRow);
        visibleItems.append(cell);
        cell->setPos(QPointF(0, locatePosOfItem(currentRow)));
    }
}

void DuiListViewPrivate::resetModel(DuiListModel *duiListModel)
{
    forceRepaint = true;

    disconnect(q_ptr, SLOT(dataChanged(QModelIndex, QModelIndex)));
    disconnect(q_ptr, SLOT(rowsInserted(QModelIndex, int, int)));
    disconnect(q_ptr, SLOT(rowsRemoved(QModelIndex, int, int)));
    disconnect(q_ptr, SLOT(layoutChanged()));
    disconnect(q_ptr, SLOT(modelReset()));
    disconnect(q_ptr, SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));

    controllerModel = duiListModel;
    model = duiListModel->itemModel();
    clearVisibleItemsArray();
    updateItemHeight();
    updateFirstVisibleRow(QModelIndex());
    updateLastVisibleRow(QModelIndex());

    if (model) {
        connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), q_ptr, SLOT(dataChanged(QModelIndex, QModelIndex)));
        connect(model, SIGNAL(rowsInserted(QModelIndex, int, int)), q_ptr, SLOT(rowsInserted(QModelIndex, int, int)));
        connect(model, SIGNAL(rowsRemoved(QModelIndex, int, int)), q_ptr, SLOT(rowsRemoved(QModelIndex, int, int)));
        connect(model, SIGNAL(layoutChanged()), q_ptr, SLOT(layoutChanged()));
        connect(model, SIGNAL(modelReset()), q_ptr, SLOT(modelReset()));
        connect(model, SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)), q_ptr, SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));
    }
}

void DuiListViewPrivate::updateItemSize()
{
    foreach(DuiWidget * cell, visibleItems) {
        cell->resize(cellSize(0));
    }
}

QSizeF DuiListViewPrivate::cellSize(int row) const
{
    Q_UNUSED(row);
    return QSizeF(viewWidth, itemHeight);
}

void DuiListViewPrivate::updateSeparatorSize()
{
    if(hseparator) {
        hseparatorHeight = hseparator->preferredHeight();
        hseparator->setGeometry(QRectF(QPointF(0, 0), QSizeF(viewWidth, hseparatorHeight)));
    }
}

void DuiListViewPrivate::updateFirstVisibleRow(const QModelIndex &index)
{
    controllerModel->setFirstVisibleItem(index);
}

void DuiListViewPrivate::updateLastVisibleRow(const QModelIndex &index)
{
    controllerModel->setLastVisibleItem(index);
}

void DuiListViewPrivate::createVisibleItems()
{
    QModelIndex firstVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y()));
    int firstVisibleRow = indexToFlatRow(firstVisibleIndex);

    QModelIndex lastVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y() + viewportVisibleHeight));
    int lastVisibleRow = indexToFlatRow(lastVisibleIndex);

    createVisibleItems(firstVisibleRow, lastVisibleRow);
}

bool DuiListViewPrivate::isGroupHeader(const QModelIndex &index)
{
    Q_UNUSED(index);

    return false;
}

void DuiListViewPrivate::layoutChanged()
{

}

void DuiListViewPrivate::drawSeparators(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(!controllerModel->firstVisibleItem().isValid() || !controllerModel->lastVisibleItem().isValid())
        return;

    int firstRow = indexToFlatRow(controllerModel->firstVisibleItem());
    int lastRow = indexToFlatRow(controllerModel->lastVisibleItem());

    for (int currentRow = firstRow; currentRow <= lastRow; currentRow++) {
        drawSeparator(currentRow, painter, option);
    }
}

void DuiListViewPrivate::drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    drawHorizontalSeparator(row, painter, option);
}

void DuiListViewPrivate::drawHorizontalSeparator(int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(row == 0 || hseparatorHeight == 0)
        return;

    QPointF pos(-q_ptr->marginLeft(), locatePosOfItem(row) - hseparatorHeight - q_ptr->marginTop());

    painter->translate(pos.x(), pos.y());
    hseparator->paint(painter, option);
    painter->translate(-pos.x(), -pos.y());
}

////////////
// Plain list
////////////
DuiPlainListViewPrivate::DuiPlainListViewPrivate()
{

}

DuiPlainListViewPrivate::~DuiPlainListViewPrivate()
{

}

int DuiPlainListViewPrivate::separatorsCount() const
{
    return itemsCount() - 1;
}

int DuiPlainListViewPrivate::totalHeight()
{
    int itemsCount = this->itemsCount();
    int separatorsCount = this->separatorsCount();
    int totalHeight = itemsCount * itemHeight + separatorsCount * hseparatorHeight;
    return totalHeight;
}

int DuiPlainListViewPrivate::itemsCount() const
{
    if (model == 0)
        return NULL;

    return model->rowCount();
}

DuiWidget *DuiPlainListViewPrivate::createItem(int row)
{
    return createCell(row);
}

int DuiPlainListViewPrivate::indexToFlatRow(const QModelIndex &index) const
{
    return index.row();
}

QModelIndex DuiPlainListViewPrivate::flatRowToIndex(int row) const
{
    return model->index(row, 0);
}

int DuiPlainListViewPrivate::locateVisibleRowAt(int y, int x)
{
    Q_UNUSED(x);
    // Formula for calculating position of specific row is following:
    // row * itemHeight + row * hseparatorHeight = pos
    // to calculate row lets do basic math:
    // row = pos / (hseparatorHeight + itemHeight)
    int row = y / (hseparatorHeight + itemHeight);

    int modelRowCount = model->rowCount();
    if (row >= modelRowCount)
        row = modelRowCount - 1;

    return row;
}

// TODO write unit test for this
int DuiPlainListViewPrivate::locatePosOfItem(int row)
{
    int itemHeights = row * itemHeight;
    int hseparatorHeights = 0;
    if (row > 0)
        hseparatorHeights = row * hseparatorHeight;

    return itemHeights + hseparatorHeights;
}

int DuiPlainListViewPrivate::locatePosOfItem(const QModelIndex &index)
{
    return locatePosOfItem(index.row());
}

QModelIndex DuiPlainListViewPrivate::locateVisibleIndexAt(int pos)
{
    int row = locateVisibleRowAt(pos);
    if (row < 0)
        return model->index(0, 0);

    return model->index(row, 0);
}

void DuiPlainListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    DuiListViewPrivate::createVisibleItems(firstVisibleRow.row(), lastVisibleRow.row());
}

////////////
// Plain list MultiColumn
////////////
DuiPlainMultiColumnListViewPrivate::DuiPlainMultiColumnListViewPrivate()
{
    vseparatorWidth = 0;
    vseparator = NULL;
}

DuiPlainMultiColumnListViewPrivate::~DuiPlainMultiColumnListViewPrivate()
{
    if(vseparator)
        delete vseparator;
}

void DuiPlainMultiColumnListViewPrivate::createSeparators()
{
    DuiListViewPrivate::createSeparators();

    //create vertical separators
    setVerticalSeparator(new DuiSeparator(NULL, Qt::Vertical));
}

void DuiPlainMultiColumnListViewPrivate::updateSeparators()
{
    DuiListViewPrivate::updateSeparators();
    if(vseparator)
        vseparator->setObjectName(q_ptr->style()->verticalSeparatorObjectName());
}

void DuiPlainMultiColumnListViewPrivate::setVerticalSeparator(DuiWidget *separator)
{
    if(vseparator)
        delete vseparator;

    vseparator = separator;
    if(vseparator)
        vseparatorWidth = vseparator->preferredWidth();
    else
        vseparatorWidth = 0;
}

int DuiPlainMultiColumnListViewPrivate::itemsToRows(int items) const
{
    int columns = controllerModel->columns();
    int rows = items / columns;
    if (items > rows * columns)
        rows++;

    return rows;
}

int DuiPlainMultiColumnListViewPrivate::flatRowToColumn(int row) const
{
    int columns = controllerModel->columns();
    int itemRow = row / columns;
    int flatRowColumn = row - itemRow * columns;

    return flatRowColumn;
}

int DuiPlainMultiColumnListViewPrivate::locatePosOfItem(int row)
{
    int columns = controllerModel->columns();
    int rows = row / columns;
    int itemHeights = rows * itemHeight;
    int hseparatorHeights = 0;
    if (rows > 0)
        hseparatorHeights = rows * hseparatorHeight;

    return itemHeights + hseparatorHeights;
}

int DuiPlainMultiColumnListViewPrivate::totalHeight()
{
    int rowsCount = itemsToRows(itemsCount());
    int separatorsCount = this->separatorsCount();
    int totalHeight = rowsCount * itemHeight + separatorsCount * hseparatorHeight;
    return totalHeight;
}

DuiWidget *DuiPlainMultiColumnListViewPrivate::createItem(int row)
{
    DuiWidget *cell = createCell(row);

    return cell;
}

int DuiPlainMultiColumnListViewPrivate::locateVisibleRowAt(int y, int x)
{
    int columns = controllerModel->columns();
    int row = y * columns / (hseparatorHeight + itemHeight);

    if (row >= itemsCount())
        row = itemsCount() - 1;

    int column = 0;
    if (viewWidth)
        column = x / (viewWidth / columns);

    return row + column;
}

void DuiPlainMultiColumnListViewPrivate::updateItemSize()
{
    foreach(DuiWidget * cell, visibleItems) {
        int cellRow = widgetFlatRows[cell] - 1;
        int cellColumn = flatRowToColumn(cellRow);
        cell->resize(cellSize(cellRow));
        cell->setPos(QPointF(cellColumn * viewWidth / controllerModel->columns(), cell->pos().y()));
    }
}

void DuiPlainMultiColumnListViewPrivate::updateSeparatorSize()
{
    DuiListViewPrivate::updateSeparatorSize();

    //Update vertical separator
    if(vseparator) {
        vseparatorWidth = vseparator->preferredWidth();
        vseparator->setGeometry(QRectF(QPointF(0, 0), QSizeF(vseparatorWidth, itemHeight)));
    }
}

QSizeF DuiPlainMultiColumnListViewPrivate::cellSize(int row) const
{
    Q_UNUSED(row);
    int columns = controllerModel->columns();
    return QSizeF(viewWidth / columns - vseparatorWidth, itemHeight);
}

void DuiPlainMultiColumnListViewPrivate::cellClicked(DuiWidget *source)
{
    int clickedFlatRow = widgetFlatRows.value(source) - 1;
    QModelIndex cellIndex(flatRowToIndex(clickedFlatRow));
    controller->selectItem(cellIndex);
}

void DuiPlainMultiColumnListViewPrivate::selectionChange(const QItemSelection &selected, const QItemSelection &deselected)
{
    foreach(DuiWidget * widget, visibleItems) {
        int widgetFlatRow = widgetFlatRows.value(widget) - 1;
        QModelIndex widgetIndex(flatRowToIndex(widgetFlatRow));
        if (selected.contains(widgetIndex))
            widget->setSelected(true);
        if (deselected.contains(widgetIndex))
            widget->setSelected(false);
    }
}

void DuiPlainMultiColumnListViewPrivate::createVisibleItems()
{
    QModelIndex firstVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y()));
    QModelIndex lastVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y() + viewportVisibleHeight));
    createVisibleItems(firstVisibleIndex, lastVisibleIndex);
}

void DuiPlainMultiColumnListViewPrivate::clearVisibleItemsArray()
{
    DuiListViewPrivate::clearVisibleItemsArray();
    widgetFlatRows.clear();
}

void DuiPlainMultiColumnListViewPrivate::removeInvisibleItems(const QPoint &firstVisibleItemCoord,
        const QPoint &lastVisibleItemCoord)
{
    for (QVector<DuiWidget *>::iterator iter = visibleItems.begin(); iter != visibleItems.end();) {
        DuiWidget *cell = *iter;
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

void DuiPlainMultiColumnListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    int firstRow = firstVisibleRow.row();
    int lastRow = lastVisibleRow.row();

    if (!viewWidth || (!firstRow&&!lastRow&&itemsCount()>1)) // required for x position
        return;

    for (int currentRow = firstRow; currentRow <= lastRow; currentRow++) {
        DuiWidget *cell = findCellAtRow(currentRow);
        if (!widgetFlatRows[cell] && flatRowToColumn(currentRow) == 0) {

            // Create widgets to all columns in this row
            for (int column = 0; column < controllerModel->columns(); ++column) {
                cell = createItem(currentRow + column);
                visibleItems.append(cell);
                widgetFlatRows[cell] = currentRow + column + 1;
                cell->setPos(QPointF(column*(viewWidth / controllerModel->columns()), locatePosOfItem(currentRow + column)));
                if (currentRow + column + 1 == itemsCount() || flatRowToColumn(currentRow + column + 1) == 0)
                    break;
            }
        }
    }
}

void DuiPlainMultiColumnListViewPrivate::drawSeparator(int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
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

void DuiPlainMultiColumnListViewPrivate::drawVerticalSeparator(int row, int column, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(vseparatorWidth == 0)
        return;

    int itemWidth = viewWidth / controllerModel->columns();

    QPointF pos(column*itemWidth - q_ptr->marginLeft() - vseparatorWidth, locatePosOfItem(row) - q_ptr->marginTop());
    painter->translate(pos.x(), pos.y());
    vseparator->paint(painter, option);
    painter->translate(-pos.x(), -pos.y());
}

////////////
// Group Header
////////////
DuiGroupHeaderListViewPrivate::DuiGroupHeaderListViewPrivate()
{
}

DuiGroupHeaderListViewPrivate::~DuiGroupHeaderListViewPrivate()
{
}

void DuiGroupHeaderListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    DuiListViewPrivate::createVisibleItems(indexToFlatRow(firstVisibleRow), indexToFlatRow(lastVisibleRow));
}

QModelIndex DuiGroupHeaderListViewPrivate::locateVisibleIndexAt(int pos)
{
    int row = locateVisibleRowAt(pos);
    if (row < 0)
        return model->index(0, 0);

    return flatRowToIndex(row);
}

int DuiGroupHeaderListViewPrivate::separatorsCount() const
{
    int itemsCount = this->headersCount();
    int seperators = 0;
    for (int i = 0; i < itemsCount; i++) {
        int itemsCountInGroup = this->itemsCount(i);
        if (itemsCountInGroup == 0)
            continue;

        seperators += itemsCountInGroup - 1;
    }

    return seperators;
}

void DuiGroupHeaderListViewPrivate::resetModel(DuiListModel *duiListModel)
{
    DuiListViewPrivate::resetModel(duiListModel);

    headersPositions.resize(this->headersCount());
    updateHeadersPositions();
    updateHeadersRows();
}

int DuiGroupHeaderListViewPrivate::locatePosOfItem(const QModelIndex &index)
{
    if (index.parent().isValid()) {
        return locatePosOfItem(index.parent().row(), index.row());
    } else {
        return locatePosOfItem(index.row(), -1);
    }
}

int DuiGroupHeaderListViewPrivate::locatePosOfItem(int headerIndex, int itemIndex)
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

int DuiGroupHeaderListViewPrivate::locatePosOfItem(int row)
{
    int headerIndex = dFindLowerIndex(headersRows, row);
    int relativeRow = row - headersRows[headerIndex] - 1; // we have to subtruct header
    return locatePosOfItem(headerIndex, relativeRow);
}

int DuiGroupHeaderListViewPrivate::locateVisibleRowAt(int y, int x)
{
    Q_UNUSED(x);

    if(headersPositions.size() == 0)
        return 0;

    int headerIndex = dFindLowerIndex(headersPositions, y);

    int headerPosition = headersPositions[headerIndex];
    int headerRow = headersRows[headerIndex];
    int relativePos = y - headerPosition;
    int headerHeight = this->headerHeight();
    if (relativePos < headerHeight)
        return headerRow;

    int row = relativePos / (hseparatorHeight + itemHeight) + headerRow + 1;
    return row;
}

QModelIndex DuiGroupHeaderListViewPrivate::flatRowToIndex(int row) const
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

void DuiGroupHeaderListViewPrivate::updateHeadersPositions()
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
        headersPositions << previousIndexPosition + headerHeight + groupSize;
        previousIndexPosition += headerHeight + groupSize;
    }
}

void DuiGroupHeaderListViewPrivate::updateHeadersRows()
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

int DuiGroupHeaderListViewPrivate::indexToFlatRow(const QModelIndex &index) const
{
    if (!index.isValid())
        return -1;

    if (index.parent().isValid()) {
        // always need to add 1 because of parent.
        return headersRows[index.parent().row()] + index.row() + 1;
    }

    return headersRows[index.row()];
}

DuiWidget *DuiGroupHeaderListViewPrivate::createItem(int row)
{
    if (!headersRows.contains(row)) {
        return createCell(row);
    } else {
        int headerIndex = dFindLowerIndex(headersRows, row);
        return createHeader(headerIndex);
    }
}

DuiWidget *DuiGroupHeaderListViewPrivate::createHeader(int headerIndex)
{
    DuiWidget *header = headersCreator->createCell(model->index(headerIndex, 0), *recycler);
    header->setParent(NULL);
    header->setParentItem(controller);
    header->setVisible(true);
    header->resize(viewWidth, header->preferredHeight());
    return header;
}

int DuiGroupHeaderListViewPrivate::headerHeight()
{
    if (!hdrHeight) {
        DuiWidget *header = createHeader(0);
        hdrHeight = header->boundingRect().height();
        deleteItem(header);
    }
    return hdrHeight;
}

int DuiGroupHeaderListViewPrivate::headersCount() const
{
    return model->rowCount();
}

int DuiGroupHeaderListViewPrivate::itemsCount(int headerIndex) const
{
    QModelIndex index(model->index(headerIndex, 0));
    return model->rowCount(index);
}

int DuiGroupHeaderListViewPrivate::itemsCount() const
{
    if (!controllerModel->showGroups())
        return model->rowCount();

    int groupsCount = model->rowCount();
    int totalItemsCount = 0;
    for (int i = 0; i < groupsCount; i++) {
        totalItemsCount += itemsCount(i);
    }

    return totalItemsCount;
}

int DuiGroupHeaderListViewPrivate::groupSize(int headerIndex) const
{
    int itemsCount = this->itemsCount(headerIndex);
    return ((itemsCount * itemHeight) + (itemsCount - 1) * hseparatorHeight);
}

int DuiGroupHeaderListViewPrivate::totalHeight()
{
    int headersCount = this->headersCount();
    int itemsCount = this->itemsCount();
    int separatorsCount = this->separatorsCount();
    int totalHeight = headersCount * headerHeight() + itemsCount * itemHeight + separatorsCount * hseparatorHeight;
    return totalHeight;
}

QSizeF DuiGroupHeaderListViewPrivate::cellSize(int row) const
{
    if(headersRows.contains(row)) {
        return QSizeF(viewWidth, hdrHeight);
    }
    return DuiListViewPrivate::cellSize(row);
}

bool DuiGroupHeaderListViewPrivate::isGroupHeader(const QModelIndex &index)
{
    return !index.parent().isValid();
}

void DuiGroupHeaderListViewPrivate::createVisibleItems(int firstVisibleRow, int lastVisibleRow)
{
    DuiListViewPrivate::createVisibleItems(firstVisibleRow, lastVisibleRow);
}

void DuiGroupHeaderListViewPrivate::layoutChanged()
{
    updateHeadersPositions();
    updateHeadersRows();
}

void DuiGroupHeaderListViewPrivate::drawSeparator(int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(headersRows.contains(row) || headersRows.contains(row - 1))
        return;

    drawHorizontalSeparator(row, painter, option);
}

////////////
// Group Header MultiColumn
////////////

DuiMultiColumnListViewPrivate::DuiMultiColumnListViewPrivate()
{
    vseparatorWidth = 0;
    vseparator = NULL;
}

DuiMultiColumnListViewPrivate::~DuiMultiColumnListViewPrivate()
{
    if(vseparator)
        delete vseparator;
}

void DuiMultiColumnListViewPrivate::setVerticalSeparator(DuiWidget *separator)
{
    if(vseparator)
        delete vseparator;

    vseparator = separator;
    if(vseparator)
        vseparatorWidth = vseparator->preferredWidth();
    else
        vseparatorWidth = 0;
}

void DuiMultiColumnListViewPrivate::createSeparators()
{
    DuiListViewPrivate::createSeparators();
    setVerticalSeparator(new DuiSeparator(NULL, Qt::Vertical));
}

void DuiMultiColumnListViewPrivate::updateSeparators()
{
    DuiListViewPrivate::updateSeparators();
    if(vseparator)
        vseparator->setObjectName(q_ptr->style()->verticalSeparatorObjectName());
}

int DuiMultiColumnListViewPrivate::itemsToRows(int items) const
{
    int columns = controllerModel->columns();
    int rows = items / columns;
    if (items > rows * columns)
        rows++;

    return rows;
}

int DuiMultiColumnListViewPrivate::rowsInGroup(int headerIndex) const
{
    int itemsInGroup = itemsCount(headerIndex);
    int rowsInGroup = itemsToRows(itemsInGroup);

    return rowsInGroup;
}

int DuiMultiColumnListViewPrivate::flatRowToColumn(int row) const
{
    if (headersRows.contains(row))
        return 0; // group headers are always in column 0

    int headerIndex = dFindLowerIndex(headersRows, row);
    int relativeRow = row - headersRows[headerIndex] - 1;
    int columns = controllerModel->columns();
    int itemRow = relativeRow / columns;
    int flatRowColumn = relativeRow - itemRow * columns;

    return flatRowColumn;
}

void DuiMultiColumnListViewPrivate::updateItemSize()
{
    foreach(DuiWidget * cell, visibleItems) {
        int cellFlatRow = widgetFlatRows[cell] - 1;
        int cellColumn = flatRowToColumn(cellFlatRow);
        cell->resize(cellSize(cellFlatRow));
        cell->setPos(QPointF(cellColumn * viewWidth / controllerModel->columns(), cell->pos().y()));
    }
}

void DuiMultiColumnListViewPrivate::updateSeparatorSize()
{
    DuiListViewPrivate::updateSeparatorSize();

    //Update vertical separator
    if(vseparator) {
        vseparatorWidth = vseparator->preferredWidth();
        vseparator->setGeometry(QRectF(QPointF(0, 0), QSizeF(vseparatorWidth, itemHeight)));
    }
}

QSizeF DuiMultiColumnListViewPrivate::cellSize(int row) const
{
    if(headersRows.contains(row)) {
        return DuiGroupHeaderListViewPrivate::cellSize(row);
    }

    int columns = controllerModel->columns();
    return QSizeF(viewWidth / columns - vseparatorWidth, itemHeight);
}

void DuiMultiColumnListViewPrivate::cellClicked(DuiWidget *source)
{
    int clickedFlatRow = widgetFlatRows.value(source) - 1;
    QModelIndex cellIndex(flatRowToIndex(clickedFlatRow));
    controller->selectItem(cellIndex);
}

void DuiMultiColumnListViewPrivate::selectionChange(const QItemSelection &selected, const QItemSelection &deselected)
{
    foreach(DuiWidget * widget, visibleItems) {
        int widgetFlatRow = widgetFlatRows.value(widget) - 1;
        QModelIndex widgetIndex(flatRowToIndex(widgetFlatRow));
        if (selected.contains(widgetIndex))
            widget->setSelected(true);
        if (deselected.contains(widgetIndex))
            widget->setSelected(false);
    }
}

void DuiMultiColumnListViewPrivate::createVisibleItems()
{
    QModelIndex firstVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y()));
    QModelIndex lastVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y() + viewportVisibleHeight));
    createVisibleItems(firstVisibleIndex, lastVisibleIndex);
}

void DuiMultiColumnListViewPrivate::clearVisibleItemsArray()
{
    DuiListViewPrivate::clearVisibleItemsArray();
    widgetFlatRows.clear();
}

void DuiMultiColumnListViewPrivate::removeInvisibleItems(const QPoint &firstVisibleItemCoord,
        const QPoint &lastVisibleItemCoord)
{
    for (QVector<DuiWidget *>::iterator iter = visibleItems.begin(); iter != visibleItems.end();) {
        DuiWidget *cell = *iter;
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

void DuiMultiColumnListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    int firstRow = indexToFlatRow(firstVisibleRow);
    int lastRow = indexToFlatRow(lastVisibleRow);

    if (!viewWidth || (!firstRow&&!lastRow&&itemsCount()>1)) { // required for x position
        return;
    }

    for (int currentRow = firstRow; currentRow <= lastRow; currentRow++) {
        DuiWidget *cell = findCellAtRow(currentRow);
        if (!widgetFlatRows[cell] && flatRowToColumn(currentRow) == 0) {

            // Create widgets to all columns in this row
            for (int column = 0; column < controllerModel->columns(); ++column) {
                cell = createItem(currentRow + column);
                visibleItems.append(cell);
                widgetFlatRows[cell] = currentRow + column + 1;
                cell->setPos(QPointF(column*(viewWidth / controllerModel->columns()), locatePosOfItem(currentRow + column)));
                if (currentRow + column + 1 == itemsCount() + model->rowCount() || flatRowToColumn(currentRow + column + 1) == 0)
                    break;
            }
        }
    }
}

int DuiMultiColumnListViewPrivate::locatePosOfItem(int headerIndex, int itemIndex)
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

int DuiMultiColumnListViewPrivate::locatePosOfItem(int row)
{
    int headerIndex = dFindLowerIndex(headersRows, row);
    int relativeRow = row - headersRows[headerIndex] - 1;

    return locatePosOfItem(headerIndex, relativeRow);
}

int DuiMultiColumnListViewPrivate::locatePosOfItem(const QModelIndex &index)
{
    return DuiGroupHeaderListViewPrivate::locatePosOfItem(index);
}

int DuiMultiColumnListViewPrivate::locateVisibleRowAt(int y, int x)
{
    Q_UNUSED(x);
    int headerIndex = dFindLowerIndex(headersPositions, y);
    int headerRow = headersRows[headerIndex];
    int relativePos = y - headersPositions[headerIndex];
    int headerHeight = this->headerHeight();
    if (relativePos < headerHeight)
        return headerRow;

    relativePos -= headerHeight;
    int columns = controllerModel->columns();

    // row/columns * itemHeight + row/columns * hseparatorHeight = pos  ->  r/c*i + r/c*s = p  ->  r = p*c/(s+i)
    int row = relativePos * columns / (hseparatorHeight + itemHeight);
    int column = 0;
    if (viewWidth)
        column = x / (viewWidth / columns);
    return headerRow + row + column + 1;
}

DuiWidget *DuiMultiColumnListViewPrivate::createItem(int row)
{
    if (!headersRows.contains(row)) {
        DuiWidget *cell = createCell(row);
        return cell;
    } else {
        int headerIndex = dFindLowerIndex(headersRows, row);
        return createHeader(headerIndex);
    }
}

int DuiMultiColumnListViewPrivate::groupSize(int headerIndex) const
{
    int rows = rowsInGroup(headerIndex);
    int groupSize = rows * itemHeight + (rows - 1) * hseparatorHeight;

    return groupSize;
}

int DuiMultiColumnListViewPrivate::totalHeight()
{
    int totalHeight = 0;
    for (int i = 0; i < headersCount(); ++i) {
        totalHeight += headerHeight() + groupSize(i);
    }

    return totalHeight;
}

void DuiMultiColumnListViewPrivate::drawSeparator(int row, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(headersRows.contains(row))
        return;

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

void DuiMultiColumnListViewPrivate::drawVerticalSeparator(int row, int column, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if(vseparatorWidth == 0)
        return;
    int itemWidth = viewWidth / controllerModel->columns();

    QPointF pos(column*itemWidth - vseparatorWidth - q_ptr->marginLeft(), locatePosOfItem(row) - q_ptr->marginTop());
    painter->translate(pos.x(), pos.y());
    vseparator->paint(painter, option);
    painter->translate(-pos.x(), -pos.y());
}
