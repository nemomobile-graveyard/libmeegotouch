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
#include "duifastlistview_p.h"

#include "duifastlistview.h"

using namespace DuiFastListViewPrivateNamespace;

static const int MOVINGDETECTORTIMEOUT = 500;

DuiFastListViewPrivate::DuiFastListViewPrivate() : recycler(new DuiWidgetRecycler)
{
    itemHeight = 0;
    viewWidth = 0;
    model = NULL;
    moving = false;
    separator = NULL;
    headersCreator = NULL;
    hdrHeight = NULL;
    forceRepaint = false;
    viewportTopLeft = QPointF();
    viewportVisibleHeight = 0;
    movingDetectorTimer.setSingleShot(true);
    connect(&movingDetectorTimer, SIGNAL(timeout()), this, SLOT(movingDetectionTimerTimeout()));
}

DuiFastListViewPrivate::~DuiFastListViewPrivate()
{
    clearVisibleItemsArray();
    movingDetectorTimer.stop();
    delete headersCreator;
    delete separator;
    delete recycler;
}

void DuiFastListViewPrivate::setSeparator(DuiWidget *separator)
{
    this->separator = separator;
}

void DuiFastListViewPrivate::setHeadersCreator(DuiCellCreator *creator)
{
    delete headersCreator;
    headersCreator = creator;
}

void DuiFastListViewPrivate::movingDetectionTimerTimeout()
{
    moving = false;
    controllerModel->setListIsMoving(false);
    movingDetectorTimer.stop();
}

void DuiFastListViewPrivate::clearVisibleItemsArray()
{
    foreach(DuiWidget * item, visibleItems) {
        deleteItem(item);
    }

    visibleItems.clear();
}

void DuiFastListViewPrivate::cellClicked(DuiWidget *source)
{
    DuiWidget *widget = source;
    QModelIndex cellIndex(locateVisibleIndexAt(widget->pos().y()));
    controller->selectItem(cellIndex);
}

void DuiFastListViewPrivate::selectionChange(const QItemSelection &selected, const QItemSelection &deselected)
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

void DuiFastListViewPrivate::deleteItem(DuiWidget *widget)
{
    recycler->recycle(widget);
}

DuiWidget *DuiFastListViewPrivate::createCell(int row)
{
    const DuiCellCreator *cellCreator = controllerModel->cellCreator();
    QModelIndex index(flatRowToIndex(row));
    DuiWidget *cell = cellCreator->createCell(index, *recycler);
    cell->setParent(NULL);
    cell->setParentItem(controller);
    cell->setVisible(true);
    cell->resize(viewWidth, cell->preferredHeight());

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

void DuiFastListViewPrivate::exposedRectChanged(const QRectF &exposedRect)
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

void DuiFastListViewPrivate::updateItemHeight()
{
    const DuiCellCreator *cellCreator = controller->cellCreator();
    if (cellCreator)
        itemHeight = cellCreator->cellSize().height();
}

void DuiFastListViewPrivate::removeInvisibleItems(const QPoint &firstVisibleItemCoord,
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

DuiWidget *DuiFastListViewPrivate::findCellAtRow(int row)
{
    foreach(DuiWidget * widget, visibleItems) {
        QPointF pos(widget->pos());
        int widgetRow = locateVisibleRowAt(pos.y(), pos.x());
        if (widgetRow == row)
            return widget;
    }

    return NULL;
}

void DuiFastListViewPrivate::createVisibleItems(int firstVisibleRow, int lastVisibleRow)
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

void DuiFastListViewPrivate::resetModel(DuiListModel *duiListModel)
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

void DuiFastListViewPrivate::updateItemSize()
{
    foreach(DuiWidget * cell, visibleItems) {
        cell->resize(viewWidth, cell->preferredHeight());
    }
}

void DuiFastListViewPrivate::updateSeparatorSize()
{
    QRectF separatorBoundingRect(separator->boundingRect());
    separator->setGeometry(QRectF(QPointF(0, 0), QSizeF(viewWidth, separatorBoundingRect.height())));
}

void DuiFastListViewPrivate::updateFirstVisibleRow(const QModelIndex &index)
{
    controllerModel->setFirstVisibleItem(index);
}

void DuiFastListViewPrivate::updateLastVisibleRow(const QModelIndex &index)
{
    controllerModel->setLastVisibleItem(index);
}

void DuiFastListViewPrivate::createVisibleItems()
{
    QModelIndex firstVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y()));
    int firstVisibleRow = indexToFlatRow(firstVisibleIndex);

    QModelIndex lastVisibleIndex(locateVisibleIndexAt(viewportTopLeft.y() + viewportVisibleHeight));
    int lastVisibleRow = indexToFlatRow(lastVisibleIndex);

    createVisibleItems(firstVisibleRow, lastVisibleRow);
}

bool DuiFastListViewPrivate::isGroupHeader(const QModelIndex &index)
{
    Q_UNUSED(index);

    return false;
}

void DuiFastListViewPrivate::layoutChanged()
{

}

////////////
// Plain list
////////////
DuiFastPlainListViewPrivate::DuiFastPlainListViewPrivate()
{

}

DuiFastPlainListViewPrivate::~DuiFastPlainListViewPrivate()
{

}

int DuiFastPlainListViewPrivate::separatorsCount() const
{
    return itemsCount() - 1;
}

int DuiFastPlainListViewPrivate::totalHeight()
{
    int itemsCount = this->itemsCount();
    int separatorHeight = separator->boundingRect().height();
    int separatorsCount = this->separatorsCount();
    int totalHeight = itemsCount * itemHeight + separatorsCount * separatorHeight;
    return totalHeight;
}

int DuiFastPlainListViewPrivate::itemsCount() const
{
    if (model == 0)
        return NULL;

    return model->rowCount();
}

DuiWidget *DuiFastPlainListViewPrivate::createItem(int row)
{
    return createCell(row);
}

int DuiFastPlainListViewPrivate::indexToFlatRow(const QModelIndex &index) const
{
    return index.row();
}

QModelIndex DuiFastPlainListViewPrivate::flatRowToIndex(int row) const
{
    return model->index(row, 0);
}

int DuiFastPlainListViewPrivate::locateVisibleRowAt(int y, int x)
{
    Q_UNUSED(x);
    // Formula for calculating position of specific row is following:
    // row * itemHeight + (row - 1) * separatorHeight = pos
    // to calculate row lets do basic math:
    // row = (pos + separatorHeight) / (itemHeight + separatorHeight)
    // since row is integer, we need to round it properly, so adding half height of item
    int separatorHeight = separator->boundingRect().height();
    int row = (y + (itemHeight / 2) + separatorHeight) / (itemHeight + separatorHeight);

    int modelRowCount = model->rowCount();
    if (row >= modelRowCount)
        row = modelRowCount - 1;

    return row;
}

// TODO write unit test for this
int DuiFastPlainListViewPrivate::locatePosOfItem(int row)
{
    int itemHeights = row * itemHeight;
    int separatorHeights = 0;
    if (row > 0)
        separatorHeights = row * separator->boundingRect().height();

    return itemHeights + separatorHeights;
}

int DuiFastPlainListViewPrivate::locatePosOfItem(const QModelIndex &index)
{
    return locatePosOfItem(index.row());
}

QModelIndex DuiFastPlainListViewPrivate::locateVisibleIndexAt(int pos)
{
    int row = locateVisibleRowAt(pos);
    if (row < 0)
        return model->index(0, 0);

    return model->index(row, 0);
}

void DuiFastPlainListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    DuiFastListViewPrivate::createVisibleItems(firstVisibleRow.row(), lastVisibleRow.row());
}

////////////
// MultiColumn without group headers
////////////
DuiFastPlainMultiColumnListViewPrivate::DuiFastPlainMultiColumnListViewPrivate()
{
}

DuiFastPlainMultiColumnListViewPrivate::~DuiFastPlainMultiColumnListViewPrivate()
{
}

int DuiFastPlainMultiColumnListViewPrivate::itemsToRows(int items) const
{
    int columns = controllerModel->columns();
    int rows = items / columns;
    if (items > rows * columns)
        rows++;

    return rows;
}

int DuiFastPlainMultiColumnListViewPrivate::flatRowToColumn(int row) const
{
    int columns = controllerModel->columns();
    int itemRow = row / columns;
    int flatRowColumn = row - itemRow * columns;

    return flatRowColumn;
}

int DuiFastPlainMultiColumnListViewPrivate::locatePosOfItem(int row)
{
    int columns = controllerModel->columns();
    int rows = row / columns;
    int itemHeights = rows * itemHeight;
    int separatorHeights = 0;
    if (rows > 0)
        separatorHeights = rows * separator->boundingRect().height();

    return itemHeights + separatorHeights;
}

int DuiFastPlainMultiColumnListViewPrivate::totalHeight()
{
    int rowsCount = itemsToRows(itemsCount());
    int separatorHeight = separator->boundingRect().height();
    int separatorsCount = this->separatorsCount();
    int totalHeight = rowsCount * itemHeight + separatorsCount * separatorHeight;
    return totalHeight;
}

DuiWidget *DuiFastPlainMultiColumnListViewPrivate::createItem(int row)
{
    DuiWidget *cell = createCell(row);
    cell->resize(viewWidth / controllerModel->columns(), cell->preferredHeight());

    return cell;
}

int DuiFastPlainMultiColumnListViewPrivate::locateVisibleRowAt(int y, int x)
{
    int separatorHeight = separator->boundingRect().height();
    int columns = controllerModel->columns();
    int row = y * columns / (separatorHeight + itemHeight);

    if (row >= itemsCount())
        row = itemsCount() - 1;

    int column = 0;
    if (viewWidth)
        column = x / (viewWidth / columns);

    return row + column;
}

void DuiFastPlainMultiColumnListViewPrivate::updateItemSize()
{
    int columns = controllerModel->columns();
    int columnWidth = viewWidth / columns;

    foreach(DuiWidget * cell, visibleItems) {
        int cellFlatRow = widgetFlatRows[cell];
        int cellColumn = flatRowToColumn(cellFlatRow);
        cell->resize(columnWidth, cell->preferredHeight());
        cell->setPos(QPointF(cellColumn * columnWidth, cell->pos().y()));
    }
}

void DuiFastPlainMultiColumnListViewPrivate::cellClicked(DuiWidget *source)
{
    int clickedFlatRow = widgetFlatRows.value(source);
    QModelIndex cellIndex(flatRowToIndex(clickedFlatRow));
    controller->selectItem(cellIndex);
}

void DuiFastPlainMultiColumnListViewPrivate::selectionChange(const QItemSelection &selected, const QItemSelection &deselected)
{
    foreach(DuiWidget * widget, visibleItems) {
        int widgetFlatRow = widgetFlatRows.value(widget);
        QModelIndex widgetIndex(flatRowToIndex(widgetFlatRow));
        if (selected.contains(widgetIndex))
            widget->setSelected(true);
        if (deselected.contains(widgetIndex))
            widget->setSelected(false);
    }
}

void DuiFastPlainMultiColumnListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    if (!viewWidth) // required for x position
        return;

    int firstRow = firstVisibleRow.row();
    int lastRow = lastVisibleRow.row();

    for (int currentRow = firstRow; currentRow <= lastRow; currentRow++) {
        DuiWidget *cell = findCellAtRow(currentRow);
        if (!cell && flatRowToColumn(currentRow) == 0) {

            // Create widgets to all columns in this row
            for (int column = 0; column < controllerModel->columns(); ++column) {
                cell = createItem(currentRow + column);
                visibleItems.append(cell);
                widgetFlatRows[cell] = currentRow + column;
                cell->setPos(QPointF(column*(viewWidth / controllerModel->columns()), locatePosOfItem(currentRow + column)));
                if (currentRow + column + 1 == itemsCount() || flatRowToColumn(currentRow + column + 1) == 0)
                    break;
            }
        }
    }

    // Remove multicolumn specific visual items that are no longer visible
    // After ABI-unfreeze this should be handled in multicolumn's removeInvisibleItems()
    for(int visibleIndex = visibleItems.count()-1; visibleIndex >= 0; --visibleIndex) {
        int visibleWidgetFlatRow =  widgetFlatRows[visibleItems[visibleIndex]];
        if(visibleWidgetFlatRow < firstRow || visibleWidgetFlatRow > lastRow) {
            deleteItem(visibleItems[visibleIndex]);
            visibleItems.remove(visibleIndex);
        }
    }
}

////////////
// Group Header
////////////
DuiFastGroupHeaderListViewPrivate::DuiFastGroupHeaderListViewPrivate()
{

}

DuiFastGroupHeaderListViewPrivate::~DuiFastGroupHeaderListViewPrivate()
{

}

void DuiFastGroupHeaderListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    DuiFastListViewPrivate::createVisibleItems(indexToFlatRow(firstVisibleRow), indexToFlatRow(lastVisibleRow));
}

QModelIndex DuiFastGroupHeaderListViewPrivate::locateVisibleIndexAt(int pos)
{
    int row = locateVisibleRowAt(pos);
    if (row < 0)
        return model->index(0, 0);

    return flatRowToIndex(row);
}

int DuiFastGroupHeaderListViewPrivate::separatorsCount() const
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

void DuiFastGroupHeaderListViewPrivate::resetModel(DuiListModel *duiListModel)
{
    DuiFastListViewPrivate::resetModel(duiListModel);

    headersPositions.resize(this->headersCount());
    updateHeadersPositions();
    updateHeadersRows();
}

int DuiFastGroupHeaderListViewPrivate::locatePosOfItem(const QModelIndex &index)
{
    if (index.parent().isValid()) {
        return locatePosOfItem(index.parent().row(), index.row());
    } else {
        return locatePosOfItem(index.row(), -1);
    }
}

int DuiFastGroupHeaderListViewPrivate::locatePosOfItem(int headerIndex, int itemIndex)
{
    if (itemIndex == -1) {
        // we hitted header
        return headersPositions[headerIndex];
    }

    int pos = headersPositions[headerIndex] + headerHeight();
    if (itemIndex == 0)
        return pos;

    int itemHeights = itemIndex * itemHeight;
    int separatorHeight = separator->boundingRect().height();
    int separatorHeights = 0;
    separatorHeights = itemIndex * separatorHeight;

    pos += separatorHeights + itemHeights;
    return pos;
}

int DuiFastGroupHeaderListViewPrivate::locatePosOfItem(int row)
{
    int headerIndex = dFindLowerIndex(headersRows, row);
    int relativeRow = row - headersRows[headerIndex] - 1; // we have to subtruct header
    return locatePosOfItem(headerIndex, relativeRow);
}

int DuiFastGroupHeaderListViewPrivate::locateVisibleRowAt(int y, int x)
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

    int separatorHeight = separator->boundingRect().height();
    int row = (relativePos + (itemHeight / 2) + separatorHeight - headerHeight) / (itemHeight + separatorHeight);
    return headerRow + row + 1;
}

QModelIndex DuiFastGroupHeaderListViewPrivate::flatRowToIndex(int row) const
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

void DuiFastGroupHeaderListViewPrivate::updateHeadersPositions()
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

void DuiFastGroupHeaderListViewPrivate::updateHeadersRows()
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

int DuiFastGroupHeaderListViewPrivate::indexToFlatRow(const QModelIndex &index) const
{
    if (!index.isValid())
        return -1;

    if (index.parent().isValid()) {
        // always need to add 1 because of parent.
        return headersRows[index.parent().row()] + index.row() + 1;
    }

    return headersRows[index.row()];
}

DuiWidget *DuiFastGroupHeaderListViewPrivate::createItem(int row)
{
    if (!headersRows.contains(row)) {
        return createCell(row);
    } else {
        int headerIndex = dFindLowerIndex(headersRows, row);
        return createHeader(headerIndex);
    }
}

DuiWidget *DuiFastGroupHeaderListViewPrivate::createHeader(int headerIndex)
{
    DuiWidget *header = headersCreator->createCell(model->index(headerIndex, 0), *recycler);
    header->setParent(NULL);
    header->setParentItem(controller);
    header->setVisible(true);
    header->resize(viewWidth, header->preferredHeight());
    return header;
}

int DuiFastGroupHeaderListViewPrivate::headerHeight()
{
    if (!hdrHeight) {
        DuiWidget *header = createHeader(0);
        hdrHeight = header->boundingRect().height();
        deleteItem(header);
    }
    return hdrHeight;
}

int DuiFastGroupHeaderListViewPrivate::headersCount() const
{
    return model->rowCount();
}

int DuiFastGroupHeaderListViewPrivate::itemsCount(int headerIndex) const
{
    QModelIndex index(model->index(headerIndex, 0));
    return model->rowCount(index);
}

int DuiFastGroupHeaderListViewPrivate::itemsCount() const
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

int DuiFastGroupHeaderListViewPrivate::groupSize(int headerIndex) const
{
    int itemsCount = this->itemsCount(headerIndex);
    return ((itemsCount * itemHeight) + (itemsCount - 1) * separator->boundingRect().height());
}

int DuiFastGroupHeaderListViewPrivate::totalHeight()
{
    int headersCount = this->headersCount();
    int itemsCount = this->itemsCount();
    int separatorHeight = separator->boundingRect().height();
    int separatorsCount = this->separatorsCount();
    int totalHeight = headersCount * headerHeight() + itemsCount * itemHeight + separatorsCount * separatorHeight;
    return totalHeight;
}

bool DuiFastGroupHeaderListViewPrivate::isGroupHeader(const QModelIndex &index)
{
    return !index.parent().isValid();
}

void DuiFastGroupHeaderListViewPrivate::createVisibleItems(int firstVisibleRow, int lastVisibleRow)
{
    DuiFastListViewPrivate::createVisibleItems(firstVisibleRow, lastVisibleRow);
}

void DuiFastGroupHeaderListViewPrivate::layoutChanged()
{
    updateHeadersPositions();
    updateHeadersRows();
}

////////////
// MultiColumn with group headers
////////////

DuiFastMultiColumnListViewPrivate::DuiFastMultiColumnListViewPrivate()
{
}

DuiFastMultiColumnListViewPrivate::~DuiFastMultiColumnListViewPrivate()
{
}

int DuiFastMultiColumnListViewPrivate::itemsToRows(int items) const
{
    int columns = controllerModel->columns();
    int rows = items / columns;
    if (items > rows * columns)
        rows++;

    return rows;
}

int DuiFastMultiColumnListViewPrivate::rowsInGroup(int headerIndex) const
{
    int itemsInGroup = itemsCount(headerIndex);
    int rowsInGroup = itemsToRows(itemsInGroup);

    return rowsInGroup;
}

int DuiFastMultiColumnListViewPrivate::flatRowToColumn(int row) const
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

void DuiFastMultiColumnListViewPrivate::updateItemSize()
{
    int columns = controllerModel->columns();
    int columnWidth = viewWidth / columns;

    foreach(DuiWidget * cell, visibleItems) {
        int cellFlatRow = widgetFlatRows[cell];
        int cellColumn = flatRowToColumn(cellFlatRow);
        cell->resize(columnWidth, cell->preferredHeight());
        cell->setPos(QPointF(cellColumn * columnWidth, cell->pos().y()));
    }
}

void DuiFastMultiColumnListViewPrivate::cellClicked(DuiWidget *source)
{
    int clickedFlatRow = widgetFlatRows.value(source);
    QModelIndex cellIndex(flatRowToIndex(clickedFlatRow));
    controller->selectItem(cellIndex);
}

void DuiFastMultiColumnListViewPrivate::selectionChange(const QItemSelection &selected, const QItemSelection &deselected)
{
    foreach(DuiWidget * widget, visibleItems) {
        int widgetFlatRow = widgetFlatRows.value(widget);
        QModelIndex widgetIndex(flatRowToIndex(widgetFlatRow));
        if (selected.contains(widgetIndex))
            widget->setSelected(true);
        if (deselected.contains(widgetIndex))
            widget->setSelected(false);
    }
}

void DuiFastMultiColumnListViewPrivate::createVisibleItems(const QModelIndex &firstVisibleRow,
        const QModelIndex &lastVisibleRow)
{
    if (!viewWidth) // required for x position
        return;

    int firstRow = indexToFlatRow(firstVisibleRow);
    int lastRow = indexToFlatRow(lastVisibleRow);

    for (int currentRow = firstRow; currentRow <= lastRow; currentRow++) {
        DuiWidget *cell = findCellAtRow(currentRow);
        if (!cell && flatRowToColumn(currentRow) == 0) {

            // Create widgets to all columns in this row
            for (int column = 0; column < controllerModel->columns(); ++column) {
                cell = createItem(currentRow + column);
                visibleItems.append(cell);
                widgetFlatRows[cell] = currentRow + column;
                cell->setPos(QPointF(column*(viewWidth / controllerModel->columns()), locatePosOfItem(currentRow + column)));
                if (currentRow + column + 1 == itemsCount() + model->rowCount() || flatRowToColumn(currentRow + column + 1) == 0)
                    break;
            }
        }
    }

    // Remove multicolumn specific visual items that are no longer visible
    // After ABI-unfreeze this should be handled in multicolumn's removeInvisibleItems()
    for(int visibleIndex = visibleItems.count()-1; visibleIndex >= 0; --visibleIndex) {
        int visibleWidgetFlatRow =  widgetFlatRows[visibleItems[visibleIndex]];
        if(visibleWidgetFlatRow < firstRow || visibleWidgetFlatRow > lastRow) {
            deleteItem(visibleItems[visibleIndex]);
            visibleItems.remove(visibleIndex);
        }
    }
}

int DuiFastMultiColumnListViewPrivate::locatePosOfItem(int headerIndex, int itemIndex)
{
    if (itemIndex == -1)
        return headersPositions[headerIndex];

    int pos = headersPositions[headerIndex] + headerHeight();
    if (itemIndex == 0)
        return pos;

    int rows = itemsToRows(itemIndex + 1) - 1; // rows after the 1st one
    int itemHeights = rows * itemHeight;
    int separatorHeights = rows * separator->boundingRect().height();
    pos += separatorHeights + itemHeights;

    return pos;
}

int DuiFastMultiColumnListViewPrivate::locatePosOfItem(int row)
{
    int headerIndex = dFindLowerIndex(headersRows, row);
    int relativeRow = row - headersRows[headerIndex] - 1;

    return locatePosOfItem(headerIndex, relativeRow);
}

int DuiFastMultiColumnListViewPrivate::locatePosOfItem(const QModelIndex &index)
{
    return DuiFastGroupHeaderListViewPrivate::locatePosOfItem(index);
}

int DuiFastMultiColumnListViewPrivate::locateVisibleRowAt(int y, int x)
{
    Q_UNUSED(x);
    int headerIndex = dFindLowerIndex(headersPositions, y);
    int headerRow = headersRows[headerIndex];
    int relativePos = y - headersPositions[headerIndex];
    int headerHeight = this->headerHeight();
    if (relativePos < headerHeight)
        return headerRow;

    relativePos -= headerHeight;
    int separatorHeight = separator->boundingRect().height();
    int columns = controllerModel->columns();

    // row/columns * itemHeight + row/columns * separatorHeight = pos  ->  r/c*i + r/c*s = p  ->  r = p*c/(s+i)
    int row = relativePos * columns / (separatorHeight + itemHeight);
    int column = 0;
    if (viewWidth)
        column = x / (viewWidth / columns);
    return headerRow + row + column + 1;
}

DuiWidget *DuiFastMultiColumnListViewPrivate::createItem(int row)
{
    if (!headersRows.contains(row)) {
        DuiWidget *cell = createCell(row);
        cell->resize(viewWidth / controllerModel->columns(), cell->preferredHeight());
        return cell;
    } else {
        int headerIndex = dFindLowerIndex(headersRows, row);
        return createHeader(headerIndex);
    }
}

int DuiFastMultiColumnListViewPrivate::groupSize(int headerIndex) const
{
    int rows = rowsInGroup(headerIndex);
    int groupSize = rows * itemHeight + (rows - 1) * separator->boundingRect().height();

    return groupSize;
}

int DuiFastMultiColumnListViewPrivate::totalHeight()
{
    int totalHeight = 0;
    for (int i = 0; i < headersCount(); ++i) {
        totalHeight += headerHeight() + groupSize(i);
    }

    return totalHeight;
}
