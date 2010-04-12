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

#ifndef MLISTVIEW_H__
#define MLISTVIEW_H__

#include <MWidgetView>
#include <mlist.h>
#include <mliststyle.h>
#include <mlistmodel.h>

class MController;
class MList;
class MListModel;
class MListViewPrivate;
class MPlainMultiColumnListViewPrivate;
class MGroupHeaderListViewPrivate;
class MMultiColumnListViewPrivate;

class M_EXPORT MListView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MListModel, MListStyle)

public:
    MListView(MWidgetController *controller);
    virtual ~MListView();

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
    void scrollTo(const QModelIndex &index, MList::ScrollHint hint);

private:
    MListViewPrivate *d_ptr;
    MList *controller;
    friend class MListViewPrivate;
    friend class MPlainMultiColumnListViewPrivate;
    friend class MGroupHeaderListViewPrivate;
    friend class MMultiColumnListViewPrivate;
};

#endif
