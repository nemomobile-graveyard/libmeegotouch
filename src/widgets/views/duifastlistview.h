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

#ifndef DUIFASTLISTVIEW_H__
#define DUIFASTLISTVIEW_H__

#include <DuiWidgetView>
#include <duilist.h>
#include <duiliststyle.h>
#include <duilistmodel.h>

class DuiController;
class DuiList;
class DuiListModel;
class DuiFastListViewPrivate;

class DUI_EXPORT DuiFastListView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiListModel, DuiListStyle)

public:
    DuiFastListView(DuiWidgetController *controller);
    virtual ~DuiFastListView();

    virtual void setGeometry(const QRectF &rect);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

public slots:
    void relayoutItemsInExposedRect();

protected:
    virtual void drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

protected slots:
    virtual void updateData(const QList<const char *>& modifications);
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsRemoved(const QModelIndex &parent, int start, int end);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void itemClick();
    void layoutChanged();
    void modelReset();
    void rowsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd,
                   const QModelIndex &destinationParent, int destinationRow);

protected:
    virtual void setupModel();
    virtual void applyStyle();

private:
    void init();
    void connectSelectionModel();
    void scrollTo(const QModelIndex &index, DuiList::ScrollHint hint);

private:
    DuiFastListViewPrivate *d_ptr;
    DuiList *controller;
    friend class DuiFastListViewPrivate;
};

#endif
