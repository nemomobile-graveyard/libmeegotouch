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

#ifndef MLISTVIEWPRIVATE_H__
#define MLISTVIEWPRIVATE_H__

#include <QObject>
#include <QHash>
#include <QModelIndex>
#include <QVector>
#include <QRectF>
#include <QTimer>
#include <QSet>
#include <MLabel>
#include <MList>
#include "mabstractcellcreator.h"
#include "private/mwidgetview_p.h"

class MWidget;
class MListView;
class MListIndex;
class MPannableViewport;
class MSideBar;
class MWidgetRecycler;
class QAbstractItemModel;
class QItemSelectionModel;
class QItemSelection;
class QPropertyAnimation;

namespace MListViewPrivateNamespace
{
    template<class T>
    T *findParentWidgetOfType(QGraphicsItem *widget)
    {
        T *targetWidget = NULL;
        QGraphicsWidget *parentWidget = widget->parentWidget();
        while (parentWidget && targetWidget == NULL) {
            targetWidget = qobject_cast<T *>(parentWidget);
            parentWidget = parentWidget->parentWidget();
        }

        return targetWidget;
    }

    template <typename T>
    inline int dFindLowerIndex(const QVector<T> &vec, const T &item)
    {
        int start = 0, end = vec.size() - 1;
        int i = (start + end + 1) >> 1;
        while (end - start > 0) {
            if (vec.at(i) > item)
                end = i - 1;
            else
                start = i;
            i = (start + end + 1) >> 1;
        }
        return i;
    }
}

class MListViewPrivate : public QObject
{
    Q_OBJECT

public:
    MListViewPrivate();
    virtual ~MListViewPrivate();

    void updateItemHeight();

    MWidget *createCell(int row);
    void deleteItem(MWidget *widget);

    void setSeparator(MWidget *separator);
    void setHeadersCreator(MCellCreator *headersCreator);

    MWidget *findCellAtRow(int row);

    void viewportRectChanged(const QRectF &viewportRect);
    void connectPannableViewport();
    void updateViewportRect(const QPointF &position, const QSizeF &size);

    void updatePannableViewportPosition();
    QPointF calculatePannableViewportOffset(const QPointF &listPosition);

    void updateFirstVisibleRow(const QModelIndex &index);
    void updateLastVisibleRow(const QModelIndex &index);

    void drawHorizontalSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);

    QPointF locateScrollToPosition(const QModelIndex &index, MList::ScrollHint hint);
    void updateScrollToTargetPosition();

    void scrollToPos(const QPointF &targetPosition);

    void deleteVisibleItemsArray();

public:
    virtual void createSeparators();
    virtual void updateSeparators();
    virtual void updateHeaders();
    virtual void updateHeaderHeight();

    virtual void createVisibleItems();
    virtual void clearVisibleItemsArray();
    virtual void clearFirstAndLastVisibleRows();
    virtual void removeInvisibleItems(const QPoint &firstVisibleItemCoord, const QPoint &lastVisibleItemCoord);
    virtual void cellClicked(MWidget *source);
    virtual void cellLongTapped(const QModelIndex &index, const QPointF &position);
    virtual void selectionChange(const QItemSelection &selected, const QItemSelection &deselected);
    virtual void updateItemSize();
    virtual void updateSeparatorSize();
    virtual QSizeF cellSize(int row) const;
    virtual void resetModel(MListModel *controllerModel);
    virtual int locateVisibleRowAt(int y, int x = 0) = 0;

    virtual int locatePosOfItem(int row) = 0;
    virtual int locatePosOfItem(const QModelIndex &index) = 0;
    virtual int itemsCount() const = 0;
    virtual QModelIndex flatRowToIndex(int row) const = 0;
    virtual int indexToFlatRow(const QModelIndex &index) const = 0;
    virtual void createVisibleItems(int firstVisibleRow, int lastVisibleRow);
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow) = 0;

    virtual MWidget *createItem(int row) = 0;
    virtual int totalHeight() = 0;
    virtual int hseparatorsCount() const = 0;
    virtual QModelIndex locateVisibleIndexAt(int pos) = 0;
    virtual QModelIndex locateLastVisibleIndexInRowAt(int pos);
    virtual bool isGroupHeader(const QModelIndex &index);
    virtual void layoutChanged();

    virtual void drawSeparators(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);

    virtual void updateListIndexVisibility();

    virtual void disconnectSignalsFromModelToListView();
    virtual void connectSignalsFromModelToListView();

    virtual void updateItemConnections();
    virtual void updateItemLongTapConnection(MWidget *cell);

public Q_SLOTS:
    void movingDetectionTimerTimeout();
    void viewportPositionChanged(const QPointF &pos);
    void viewportSizeChanged(const QSizeF &size);
    void viewportRangeChanged(const QRectF &range);
    void controllerParentChanged();
    void updateListGeometry();

    void _q_itemLongTapped(const QPointF &pos);
    void _q_relayoutItemsIfNeeded();

public:
    MListView *q_ptr;
    MList *controller;
    MListModel *controllerModel;
    QAbstractItemModel *model;
    QItemSelectionModel *selectionModel;
    MWidgetRecycler *recycler;
    MWidget *hseparator;
    int hseparatorHeight;
    MCellCreator *headersCreator;

    QVector<MWidget *> visibleItems;
    QPointF viewportTopLeft;
    int viewportVisibleHeight;

    int itemHeight;
    int rowCount;
    int viewWidth;
    int hdrHeight;

    bool forceRepaint;

    bool clearVisibleOnRelayout;

    // Section for panning detection
    QPointF oldViewportRectPosition;
    bool moving;
    QTimer movingDetectorTimer;
    MPannableViewport *pannableViewport;

    QPointF listPosition;
    QPointF listOffset;

    // Scroll animation
    QPropertyAnimation *scrollToAnimation;
};

class MPlainListViewPrivate : public MListViewPrivate
{
public:
    MPlainListViewPrivate();
    virtual ~MPlainListViewPrivate();

public:
    virtual int totalHeight();

    virtual QModelIndex locateVisibleIndexAt(int pos);
    virtual int locateVisibleRowAt(int y, int x = 0);

    virtual int locatePosOfItem(int row);
    virtual int locatePosOfItem(const QModelIndex &index);
    virtual int hseparatorsCount() const;
    virtual int itemsCount() const;
    virtual int indexToFlatRow(const QModelIndex &index) const;
    virtual QModelIndex flatRowToIndex(int row) const;
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow);
    virtual MWidget *createItem(int row);
};

class MPlainMultiColumnListViewPrivate : public MPlainListViewPrivate
{
public:
    MPlainMultiColumnListViewPrivate();
    virtual ~MPlainMultiColumnListViewPrivate();

public:
    void setVerticalSeparator(MWidget *separator);
    int itemsToRows(int items) const;
    int flatRowToColumn(int row) const;

public:
    virtual void createSeparators();
    virtual void updateSeparators();

    virtual void createVisibleItems();
    virtual void clearVisibleItemsArray();
    virtual void removeInvisibleItems(const QPoint &firstVisibleItemCoord, const QPoint &lastVisibleItemCoord);
    virtual int locatePosOfItem(int row);
    virtual int hseparatorsCount() const;
    virtual int totalHeight();
    virtual MWidget *createItem(int row);
    virtual QModelIndex locateLastVisibleIndexInRowAt(int pos);
    virtual int locateVisibleRowAt(int y, int x = 0);
    virtual void updateItemSize();
    virtual void updateSeparatorSize();
    virtual QSizeF cellSize(int row) const;
    virtual void cellClicked(MWidget *source);
    virtual void selectionChange(const QItemSelection &selected, const QItemSelection &deselected);
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow);

    virtual void drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawVerticalSeparator(int row, int column, QPainter *painter, const QStyleOptionGraphicsItem *option);
public:
    QHash<MWidget *, int> widgetFlatRows;
    MWidget *vseparator;
    int vseparatorWidth;
};

class MGroupHeaderListViewPrivate : public MListViewPrivate
{
public:
    MGroupHeaderListViewPrivate();
    virtual ~MGroupHeaderListViewPrivate();

public:
    MWidget *createHeader(int headerIndex);
    int headerHeight();
    int headersCount() const;
    int itemsCount(int index) const;
    void updateHeadersPositions();
    void updateHeadersRows();
    void updateHeadersIndexes();

    void setGroupSeparator(MWidget *separator);

public:
    virtual void createSeparators();
    virtual void updateSeparators();
    virtual void updateSeparatorSize();

    virtual void updateHeaderHeight();

    virtual int indexToFlatRow(const QModelIndex &index) const;
    virtual int locatePosOfItem(int headerIndex, int itemIndex); // groups only
    virtual int groupSize(int headerIndex) const;
    virtual void resetModel(MListModel *controllerModel);
    virtual int totalHeight();
    virtual QSizeF cellSize(int row) const;

    virtual QModelIndex locateVisibleIndexAt(int pos);
    virtual int locateVisibleRowAt(int y, int x = 0);

    virtual int locatePosOfItem(int row);
    virtual int locatePosOfItem(const QModelIndex &index);
    virtual int hseparatorsCount() const;
    virtual int itemsCount() const;
    virtual QModelIndex flatRowToIndex(int row) const;

    void createVisibleItems(int firstVisibleRow, int lastVisibleRow);
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow);
    virtual MWidget *createItem(int row);
    virtual bool isGroupHeader(const QModelIndex &index);
    virtual void layoutChanged();

    virtual void drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawGroupSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);

    virtual void updateListIndexVisibility();

public:
    QVector<int> headersPositions;
    QVector<int> headersRows;

    MListIndex *listIndexWidget;
    MWidget *gseparator;
    int gseparatorHeight;
};

class MMultiColumnListViewPrivate : public MGroupHeaderListViewPrivate
{
public:
    MMultiColumnListViewPrivate();
    virtual ~MMultiColumnListViewPrivate();

public:
    void setVerticalSeparator(MWidget *separator);
    int itemsToRows(int items) const;
    int rowsInGroup(int headerIndex) const;
    int flatRowToColumn(int row) const;

public:
    virtual void createSeparators();
    virtual void updateSeparators();

    virtual void createVisibleItems();
    virtual void clearVisibleItemsArray();
    virtual void removeInvisibleItems(const QPoint &firstVisibleItemCoord, const QPoint &lastVisibleItemCoord);
    virtual void updateItemSize();
    virtual void updateSeparatorSize();
    virtual QSizeF cellSize(int row) const;
    virtual void cellClicked(MWidget *source);
    virtual void selectionChange(const QItemSelection &selected, const QItemSelection &deselected);
    virtual int locatePosOfItem(int headerIndex, int itemIndex);
    virtual int locatePosOfItem(const QModelIndex &index);
    virtual int hseparatorsCount() const;
    virtual int groupSize(int headerIndex) const;
    virtual int totalHeight();
    virtual QModelIndex locateLastVisibleIndexInRowAt(int pos);
    virtual int locateVisibleRowAt(int y, int x = 0);
    virtual int locatePosOfItem(int row);
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow);
    virtual MWidget *createItem(int row);

    virtual void drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawVerticalSeparator(int row, int column, QPainter *painter, const QStyleOptionGraphicsItem *option);
public:
    QHash<MWidget *, int> widgetFlatRows;
    MWidget *vseparator;
    int vseparatorWidth;
};

class MDefaultHeadersCreator : public MAbstractCellCreator<MLabel>
{
public:
    MDefaultHeadersCreator(const QString &headerObjectName) {
        setHeaderObjectName(headerObjectName);
    }

    virtual ~MDefaultHeadersCreator() {

    }

    virtual MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        MLabel *header = dynamic_cast<MLabel *>(recycler.take(MLabel::staticMetaObject.className()));
        if (header == NULL) {
            header = new MLabel;
            header->setObjectName(headerObjectName);
        }
        updateCell(index, header);

        return header;
    }

    virtual void updateCell(const QModelIndex &index, MWidget *cell) const {
        MLabel *header = dynamic_cast<MLabel *>(cell);
        QString title = index.data(Qt::DisplayRole).toString();
        header->setText(title);
    }

    virtual void setHeaderObjectName(const QString &headerObjectName) {
        this->headerObjectName = headerObjectName;
    }

private:
    QString headerObjectName;
};

#endif
