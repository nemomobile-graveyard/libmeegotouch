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

#ifndef DUIFASTLISTVIEWPRIVATE_H__
#define DUIFASTLISTVIEWPRIVATE_H__

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
class DuiFastListView;
class DuiList;
class DuiListModel;
class DuiWidgetRecycler;
class QAbstractItemModel;
class QItemSelectionModel;
class QItemSelection;

namespace DuiFastListViewPrivateNamespace
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

class DuiFastListViewPrivate : public QObject
{
    Q_OBJECT

public:
    DuiFastListViewPrivate();
    virtual ~DuiFastListViewPrivate();

    void updateItemHeight();

    DuiWidget *createCell(int row);
    void deleteItem(DuiWidget *widget);

    void setSeparator(DuiWidget *separator);
    void setHeadersCreator(DuiCellCreator *headersCreator);

    DuiWidget *findCellAtRow(int row);

    void updateSeparatorSize();

    void exposedRectChanged(const QRectF &exposedRect);

    void updateFirstVisibleRow(const QModelIndex &index);
    void updateLastVisibleRow(const QModelIndex &index);
    void createVisibleItems();

public:
    virtual void clearVisibleItemsArray();
    virtual void removeInvisibleItems(const QPoint &firstVisibleItemCoord, const QPoint &lastVisibleItemCoord);
    virtual void cellClicked(DuiWidget *source);
    virtual void selectionChange(const QItemSelection &selected, const QItemSelection &deselected);
    virtual void updateItemSize();
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

public slots:
    void movingDetectionTimerTimeout();

public:
    DuiFastListView *q_ptr;
    DuiList *controller;
    DuiListModel *controllerModel;
    QAbstractItemModel *model;
    DuiWidgetRecycler *recycler;
    DuiWidget *separator;
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

class DuiFastPlainListViewPrivate : public DuiFastListViewPrivate
{
public:
    DuiFastPlainListViewPrivate();
    virtual ~DuiFastPlainListViewPrivate();

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

class DuiFastPlainMultiColumnListViewPrivate : public DuiFastPlainListViewPrivate
{
public:
    DuiFastPlainMultiColumnListViewPrivate();
    virtual ~DuiFastPlainMultiColumnListViewPrivate();

public:
    int itemsToRows(int items) const;
    int flatRowToColumn(int row) const;

public:
    virtual void clearVisibleItemsArray();
    virtual void removeInvisibleItems(const QPoint &firstVisibleItemCoord, const QPoint &lastVisibleItemCoord);
    virtual int locatePosOfItem(int row);
    virtual int totalHeight();
    virtual DuiWidget *createItem(int row);
    virtual int locateVisibleRowAt(int y, int x = 0);
    virtual void updateItemSize();
    virtual void cellClicked(DuiWidget *source);
    virtual void selectionChange(const QItemSelection &selected, const QItemSelection &deselected);
    virtual void createVisibleItems(const QModelIndex &firstVisibleRow,
                                    const QModelIndex &lastVisibleRow);
public:
    QHash<DuiWidget *, int> widgetFlatRows;
    DuiWidget *hseparator;
};

class DuiFastGroupHeaderListViewPrivate : public DuiFastListViewPrivate
{
public:
    DuiFastGroupHeaderListViewPrivate();
    virtual ~DuiFastGroupHeaderListViewPrivate();

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

public:
    QVector<int> headersPositions;
    QVector<int> headersRows;
};

class DuiFastMultiColumnListViewPrivate : public DuiFastGroupHeaderListViewPrivate
{
public:
    DuiFastMultiColumnListViewPrivate();
    virtual ~DuiFastMultiColumnListViewPrivate();

public:
    int itemsToRows(int items) const;
    int rowsInGroup(int headerIndex) const;
    int flatRowToColumn(int row) const;

public:
    virtual void clearVisibleItemsArray();
    virtual void removeInvisibleItems(const QPoint &firstVisibleItemCoord, const QPoint &lastVisibleItemCoord);
    virtual void updateItemSize();
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

public:
    QHash<DuiWidget *, int> widgetFlatRows;
    DuiWidget *hseparator;
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
