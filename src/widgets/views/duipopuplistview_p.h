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
#include "duiabstractcellcreator.h"
#include "duicontentitem.h"

class DuiPopupList;
class QAbstractItemModel;
class QItemSelectionModel;
class DuiList;

class DuiPopupListItem : public DuiContentItem {
public:
    explicit DuiPopupListItem(QGraphicsItem *parent = NULL)
        : DuiContentItem(DuiContentItem::IconAndSingleTextLabel, parent)
    {
        setObjectName("PopupListItem");
    }
};

class DuiPopupListViewPrivate : public DuiDialogViewPrivate, public DuiAbstractCellCreator<DuiPopupListItem>
{
    Q_DECLARE_PUBLIC(DuiPopupListView)

public:
    DuiPopupListViewPrivate();
    virtual ~DuiPopupListViewPrivate();

    virtual void init();

    virtual void updateCell(const QModelIndex& index, DuiWidget * cell) const;

    DuiPopupList*   controller;

    DuiList*    list;
};
#endif
