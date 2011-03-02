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
#include "mlistitem.h"

class MPopupList;
class QAbstractItemModel;
class QItemSelectionModel;
class MList;
class MLinearLayoutPolicy;
class MImageWidget;

class MPopupListItem : public MListItem {
public:
    explicit MPopupListItem(QGraphicsItem *parent = NULL);
    void setTitle(const QString &title);
    void setWordWrap(bool wordWrap);
    void setIconID(const QString& id);
    void setPixmap(const QPixmap& pixmap);

private:
    void updateLayout();
    MImageWidget* iconWidget();

    enum ItemStyle {
        SingleTitle,
        IconTitle
    };

    MLinearLayoutPolicy* policy[2];
    MImageWidget* icon;
    MLabel* title;
    ItemStyle itemStyle;
#ifdef UNIT_TEST
    friend class Ut_MPopupList;
#endif
};

class MPopupListCellCreator : public MAbstractCellCreator<MPopupListItem>
{
public:
    MPopupListCellCreator(MList* list);
    virtual void updateCell(const QModelIndex& index, MWidget * cell) const;
    void setWordWrap(bool wordWrap);

private:
    MList* list;
    bool wordWrap;
};

class MPopupListViewPrivate : public MDialogViewPrivate
{
    Q_DECLARE_PUBLIC(MPopupListView)

public:
    MPopupListViewPrivate();
    virtual ~MPopupListViewPrivate();

    virtual void init();

    MPopupList*   controller;

    MList*    list;
    MPopupListCellCreator *cellCreator;
};
#endif
