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

#ifndef MBUTTONVIEW_P_H
#define MBUTTONVIEW_P_H

#include "mdialogview_p.h"
#include <QPointer>
#include <QModelIndex>
#include "mabstractcellcreator.h"
#include "mbasiclistitem.h"

class MPopupList;
class QAbstractItemModel;
class QItemSelectionModel;
class MList;

class MPopupListItem : public MBasicListItem {
public:
    explicit MPopupListItem(QGraphicsItem *parent = NULL)
        : MBasicListItem(MBasicListItem::IconWithTitle, parent)
    {
        setObjectName("PopupListItem");
    }
};

class MPopupListViewPrivate : public MDialogViewPrivate, public MAbstractCellCreator<MPopupListItem>
{
    Q_DECLARE_PUBLIC(MPopupListView)

public:
    MPopupListViewPrivate();
    virtual ~MPopupListViewPrivate();

    virtual void init();

    virtual void updateCell(const QModelIndex& index, MWidget * cell) const;
    virtual void _q_scrollOnFirstAppearance();

    MPopupList*   controller;

    MList*    list;
};
#endif
