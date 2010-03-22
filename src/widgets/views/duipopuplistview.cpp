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

#include "duipopuplistview.h"
#include "duipopuplistview_p.h"

#include "duipopuplist.h"
#include "duilist.h"
#include "duigriditem.h"
#include "duipannableviewport.h"

#include <QGraphicsLinearLayout>

DuiPopupListViewPrivate::DuiPopupListViewPrivate()
    : controller(0), list(0)
{
}

DuiPopupListViewPrivate::~DuiPopupListViewPrivate()
{
}

void DuiPopupListViewPrivate::init()
{
    Q_Q(DuiPopupListView);

    list = new DuiList();
    list->setCellCreator(this);
    list->setSelectionMode(DuiList::SingleSelection);
    q->contentsLayout()->insertItem(0, list);

    QObject::connect(list, SIGNAL(itemClicked(QModelIndex)), controller, SLOT(click(QModelIndex)));
    QObject::connect(controller, SIGNAL(scrollToIndex(QModelIndex)),  q, SLOT(scrollTo(QModelIndex)));
}

void DuiPopupListViewPrivate::updateCell(const QModelIndex& index, DuiWidget * cell) const
{
    DuiPopupListItem* item = static_cast<DuiPopupListItem*>(cell);

    QString title;
    QPixmap pixmap;
    QVariant value;

    value = list->itemModel()->data(index, Qt::DisplayRole);
    if (value != QVariant())
        title = value.toString();

    value = list->itemModel()->data(index, Qt::DecorationRole);
    if (value != QVariant())
        pixmap = *DuiTheme::pixmap(value.toString());

    item->setTitle(title);
    item->setPixmap(pixmap);

    if (list->selectionModel()->isSelected(index))
        item->setSelected(true);
}

DuiPopupListView::DuiPopupListView(DuiPopupList *controller)
    : DuiDialogView(* new DuiPopupListViewPrivate, controller)
{
    Q_D(DuiPopupListView);
    d->controller = controller;

    d->init();
}

DuiPopupListView::DuiPopupListView(DuiPopupListViewPrivate &dd, DuiPopupList *controller) :
    DuiDialogView(dd, controller)
{
    Q_D(DuiPopupListView);
    d->controller = controller;

    d->init();
}

DuiPopupListView::~DuiPopupListView()
{
}

void DuiPopupListView::updateData(const QList<const char *>& modifications)
{
    DuiDialogView::updateData(modifications);

    Q_D(DuiPopupListView);
    foreach(const char* member, modifications) {
        if (member == DuiPopupListModel::ItemModel)
            d->list->setItemModel(model()->itemModel());
        else if (member == DuiPopupListModel::SelectionModel)
            d->list->setSelectionModel(model()->selectionModel());
    }
}

void DuiPopupListView::setupModel()
{
    DuiDialogView::setupModel();

    Q_D(DuiPopupListView);
    d->list->setItemModel(model()->itemModel());
    d->list->setSelectionModel(model()->selectionModel());
}

void DuiPopupListView::scrollTo(const QModelIndex &index) const
{
    Q_D(const DuiPopupListView);
    d->list->scrollTo(index, DuiList::EnsureVisibleHint);
}

DUI_REGISTER_VIEW_NEW(DuiPopupListView, DuiPopupList)

#include "moc_duipopuplistview.cpp"

