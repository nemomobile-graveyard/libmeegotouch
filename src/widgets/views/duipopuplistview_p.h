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

#ifndef DUIBUTTONVIEW_P_H
#define DUIBUTTONVIEW_P_H

#include "duidialogview_p.h"
#include <QPointer>
#include <QModelIndex>
#include <QPersistentModelIndex>

class DuiPopupList;
class DuiPannableViewport;
class QGraphicsWidget;
class QAbstractItemModel;
class QItemSelectionModel;
class QGraphicsLinearLayout;

class DuiPopupListViewPrivate : public DuiDialogViewPrivate
{
    Q_DECLARE_PUBLIC(DuiPopupListView)

public:
    DuiPopupListViewPrivate();
    ~DuiPopupListViewPrivate();

    virtual void init();

    void _q_modelDestroyed();
    void _q_modelReset();
    void _q_layoutChanged();
    void _q_arrangeWidget();

    void setLayoutDirty();

    void doItemsLayout() const;
    inline void setSpacerFixedHeight(qreal height) const;
    inline DuiWidget *buildItem(const QModelIndex &index) const;
    inline void recycleWidget(DuiWidget *widget) const;
    inline void setViewportRange(const QRectF &rect) const;

    QModelIndex indexAt(const QPointF &pos);
    DuiWidget  *indexWidget(const QModelIndex &index);

    DuiPopupList *controller;

    DuiPannableViewport    *viewport;
    QGraphicsLinearLayout  *layout;
    QGraphicsWidget        *container;
    QGraphicsWidget        *spacer;

    QAbstractItemModel             *itemModel;
    QPointer<QItemSelectionModel>   selectionModel;

    mutable int         itemDirty;
    mutable bool        singlePass;
    mutable int         batchSize;
    mutable bool        gridItemMode;

    mutable QPointF     lastPosition;
    mutable int         itemStartIndex;
    mutable qreal       itemHeight;
    mutable bool        arrangingWidget;

    QSizeF              viewportSize;
    QPointF             pannedPos;

    mutable QList<DuiWidget *>  widgetsList;
    mutable QList<DuiWidget *>  objectsPool;

    QPersistentModelIndex pressedIndex;
};

#endif
