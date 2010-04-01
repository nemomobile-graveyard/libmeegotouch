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

#ifndef DUILISTVIEWPRIVATE_H__
#define DUILISTVIEWPRIVATE_H__

#include <QObject>
#include <QHash>
#include <QModelIndex>
#include <QVector>
#include <QRectF>
#include <QTimer>
#include <QSet>
#include <DuiLabel>
#include "duiabstractcellcreator.h"
#include "private/duiwidgetview_p.h"

class DuiWidget;
class DuiListView;
class DuiList;
class DuiListModel;
class DuiWidgetRecycler;
class QAbstractItemModel;
class QItemSelectionModel;
class QItemSelection;

namespace DuiListViewPrivateNamespace
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

class DuiListViewPrivate : public QObject
{
    Q_OBJECT

public:
    DuiListViewPrivate();
    virtual ~DuiListViewPrivate();

    void updateItemHeight();

    DuiWidget *createCell(int row);
    void deleteItem(DuiWidget *widget);

    void setSeparator(DuiWidget *separator);
    void setHeadersCreator(DuiCellCreator *headersCreator);

    DuiWidget *findCellAtRow(int row);

    void exposedRectChanged(const QRectF &exposedRect);

    void updateFirstVisibleRow(const QModelIndex &index);
    void updateLastVisibleRow(const QModelIndex &index);

    void drawHorizontalSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);

public:
    virtual void createSeparators();
    virtual void updateSeparators();

    virtual void createVisibleItems();
    virtual void clearVisibleItemsArray();
    virtual void removeInvisibleItems(const QPoint &firstVisibleItemCoord, const QPoint &lastVisibleItemCoord);
    virtual void cellClicked(DuiWidget *source);
    virtual void selectionChange(const QItemSelection &selected, const QItemSelection &deselected);
    virtual void updateItemSize();
    virtual void updateSeparatorSize();
    virtual QSizeF cellSize(int row) const;
    virtual void resetModel(DuiListModel *controllerModel);
    virtual int locateVisibleRowAt(int y, int x = 0) = 0;

    virtual int locatePosOfItem(int row) = 0;
    virtual int locatePosOfItem(const QModelIndex &index) = 0;
    virtual int itemsCount() const = 0;
    virtual QModelIndex flatRowToIndex(int row) const = 0;
    virtual int indexToFlatRow(const QModelIndex &index) const = 0;
    virtual void createVisibleItems(int firstVisibleRow, int lastVisibleRow);
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow) = 0;

    virtual DuiWidget *createItem(int row) = 0;
    virtual int totalHeight() = 0;
    virtual int separatorsCount() const = 0;
    virtual QModelIndex locateVisibleIndexAt(int pos) = 0;
    virtual bool isGroupHeader(const QModelIndex &index);
    virtual void layoutChanged();

    virtual void drawSeparators(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);

public slots:
    void movingDetectionTimerTimeout();

public:
    DuiListView *q_ptr;
    DuiList *controller;
    DuiListModel *controllerModel;
    QAbstractItemModel *model;
    DuiWidgetRecycler *recycler;
    DuiWidget *hseparator;
    int hseparatorHeight;
    DuiCellCreator *headersCreator;

    QVector<DuiWidget *> visibleItems;
    QPointF viewportTopLeft;
    int viewportVisibleHeight;

    int itemHeight;
    int viewWidth;
    int hdrHeight;

    bool forceRepaint;

    // Section for panning detection
    QPointF oldExposedRectPosition;
    bool moving;
    QTimer movingDetectorTimer;
};

class DuiPlainListViewPrivate : public DuiListViewPrivate
{
public:
    DuiPlainListViewPrivate();
    virtual ~DuiPlainListViewPrivate();

public:
    virtual int totalHeight();

    virtual QModelIndex locateVisibleIndexAt(int pos);
    virtual int locateVisibleRowAt(int y, int x = 0);

    virtual int locatePosOfItem(int row);
    virtual int locatePosOfItem(const QModelIndex &index);
    virtual int separatorsCount() const;
    virtual int itemsCount() const;
    virtual int indexToFlatRow(const QModelIndex &index) const;
    virtual QModelIndex flatRowToIndex(int row) const;
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow);
    virtual DuiWidget *createItem(int row);
};

class DuiPlainMultiColumnListViewPrivate : public DuiPlainListViewPrivate
{
public:
    DuiPlainMultiColumnListViewPrivate();
    virtual ~DuiPlainMultiColumnListViewPrivate();

public:
    void setVerticalSeparator(DuiWidget *separator);
    int itemsToRows(int items) const;
    int flatRowToColumn(int row) const;

public:
    virtual void createSeparators();
    virtual void updateSeparators();

    virtual void createVisibleItems();
    virtual void clearVisibleItemsArray();
    virtual void removeInvisibleItems(const QPoint &firstVisibleItemCoord, const QPoint &lastVisibleItemCoord);
    virtual int locatePosOfItem(int row);
    virtual int totalHeight();
    virtual DuiWidget *createItem(int row);
    virtual int locateVisibleRowAt(int y, int x = 0);
    virtual void updateItemSize();
    virtual void updateSeparatorSize();
    virtual QSizeF cellSize(int row) const;
    virtual void cellClicked(DuiWidget *source);
    virtual void selectionChange(const QItemSelection &selected, const QItemSelection &deselected);
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow);

    virtual void drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawVerticalSeparator(int row, int column, QPainter *painter, const QStyleOptionGraphicsItem *option);
public:
    QHash<DuiWidget *, int> widgetFlatRows;
    DuiWidget *vseparator;
    int vseparatorWidth;
};

class DuiGroupHeaderListViewPrivate : public DuiListViewPrivate
{
public:
    DuiGroupHeaderListViewPrivate();
    virtual ~DuiGroupHeaderListViewPrivate();

public:
    DuiWidget *createHeader(int headerIndex);
    int headerHeight();
    int headersCount() const;
    int itemsCount(int index) const;
    void updateHeadersPositions();
    void updateHeadersRows();

public:
    virtual int indexToFlatRow(const QModelIndex &index) const;
    virtual int locatePosOfItem(int headerIndex, int itemIndex); // groups only
    virtual int groupSize(int headerIndex) const;
    virtual void resetModel(DuiListModel *controllerModel);
    virtual int totalHeight();
    virtual QSizeF cellSize(int row) const;

    virtual QModelIndex locateVisibleIndexAt(int pos);
    virtual int locateVisibleRowAt(int y, int x = 0);

    virtual int locatePosOfItem(int row);
    virtual int locatePosOfItem(const QModelIndex &index);
    virtual int separatorsCount() const;
    virtual int itemsCount() const;
    virtual QModelIndex flatRowToIndex(int row) const;

    void createVisibleItems(int firstVisibleRow, int lastVisibleRow);
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow);
    virtual DuiWidget *createItem(int row);
    virtual bool isGroupHeader(const QModelIndex &index);
    virtual void layoutChanged();

    virtual void drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);
public:
    QVector<int> headersPositions;
    QVector<int> headersRows;
};

class DuiMultiColumnListViewPrivate : public DuiGroupHeaderListViewPrivate
{
public:
    DuiMultiColumnListViewPrivate();
    virtual ~DuiMultiColumnListViewPrivate();

public:
    void setVerticalSeparator(DuiWidget *separator);
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
    virtual void cellClicked(DuiWidget *source);
    virtual void selectionChange(const QItemSelection &selected, const QItemSelection &deselected);
    virtual int locatePosOfItem(int headerIndex, int itemIndex);
    virtual int locatePosOfItem(const QModelIndex &index);
    virtual int groupSize(int headerIndex) const;
    virtual int totalHeight();
    virtual int locateVisibleRowAt(int y, int x = 0);
    virtual int locatePosOfItem(int row);
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow);
    virtual DuiWidget *createItem(int row);

    virtual void drawSeparator(const int row, QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawVerticalSeparator(int row, int column, QPainter *painter, const QStyleOptionGraphicsItem *option);
public:
    QHash<DuiWidget *, int> widgetFlatRows;
    DuiWidget *vseparator;
    int vseparatorWidth;
};

class DuiDefaultHeadersCreator : public DuiAbstractCellCreator<DuiLabel>
{
public:
    DuiDefaultHeadersCreator(const QString &headerObjectName) {
        this->headerObjectName = headerObjectName;
    }

    virtual ~DuiDefaultHeadersCreator() {

    }

    virtual DuiWidget *createCell(const QModelIndex &index, DuiWidgetRecycler &recycler) const {
        DuiLabel *header = dynamic_cast<DuiLabel *>(recycler.take(DuiLabel::staticMetaObject.className()));
        if (header == NULL) {
            header = new DuiLabel;
            header->setObjectName(headerObjectName);
        }
        updateCell(index, header);

        return header;
    }

    virtual void updateCell(const QModelIndex &index, DuiWidget *cell) const {
        DuiLabel *header = dynamic_cast<DuiLabel *>(cell);
        QString title = index.data(Qt::DisplayRole).toString();
        header->setText(title);
    }

private:
    QString headerObjectName;
};

#endif
