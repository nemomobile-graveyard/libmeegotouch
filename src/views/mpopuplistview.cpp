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

#include "mpopuplistview.h"
#include "mpopuplistview_p.h"

#include "mpopuplist.h"
#include "mlist.h"
#include "mgriditem.h"
#include "mpannableviewport.h"
#include "mimagewidget.h"

#include <QGraphicsLinearLayout>

MPopupListViewPrivate::MPopupListViewPrivate()
    : controller(0), list(0)
{
}

MPopupListViewPrivate::~MPopupListViewPrivate()
{
}

void MPopupListViewPrivate::init()
{
    Q_Q(MPopupListView);

    list = new MList();
    list->setCellCreator(this);
    list->setSelectionMode(MList::SingleSelection);
    q->contentsLayout()->insertItem(0, list);

    contentsViewport->setObjectName("MPopupListContentsViewport");

    QObject::connect(list, SIGNAL(itemClicked(QModelIndex)), controller, SLOT(click(QModelIndex)));
    QObject::connect(list, SIGNAL(displayEntered()), q, SLOT(_q_scrollOnFirstAppearance()));
}

void MPopupListViewPrivate::updateCell(const QModelIndex& index, MWidget * cell) const
{
    MPopupListItem* item = static_cast<MPopupListItem*>(cell);

    QString title;
    QString iconID;
    QVariant value;

    value = list->itemModel()->data(index, Qt::DisplayRole);
    if (value.isValid())
        title = value.toString();

    value = list->itemModel()->data(index, Qt::DecorationRole);
    if (value.isValid())
        iconID = value.toString();

    item->setTitle(title);
    item->imageWidget()->setImage(iconID);

    if (list->selectionModel()->isSelected(index))
        item->setSelected(true);

    if (index.row() == 0)
        item->setLayoutPosition(M::TopCenterPosition);
    else if (index.row() == index.model()->rowCount() - 1)
        item->setLayoutPosition(M::BottomCenterPosition);
    else
        item->setLayoutPosition(M::CenterPosition);
}

void MPopupListViewPrivate::_q_scrollOnFirstAppearance()
{
    Q_Q(MPopupListView);
    list->scrollTo(q->model()->scrollToIndex());
    QObject::disconnect(list, SIGNAL(displayEntered()), q, SLOT(_q_scrollOnFirstAppearance()));
}

MPopupListView::MPopupListView(MPopupList *controller)
    : MDialogView(* new MPopupListViewPrivate, controller)
{
    Q_D(MPopupListView);
    d->controller = controller;

    d->init();
}

MPopupListView::MPopupListView(MPopupListViewPrivate &dd, MPopupList *controller) :
    MDialogView(dd, controller)
{
    Q_D(MPopupListView);
    d->controller = controller;

    d->init();
}

MPopupListView::~MPopupListView()
{
}

void MPopupListView::updateData(const QList<const char *>& modifications)
{
    MDialogView::updateData(modifications);

    Q_D(MPopupListView);
    foreach(const char* member, modifications) {
        if (member == MPopupListModel::ItemModel)
            d->list->setItemModel(model()->itemModel());
        else if (member == MPopupListModel::SelectionModel)
            d->list->setSelectionModel(model()->selectionModel());
        else if (member == MPopupListModel::ScrollToIndex)
            d->list->scrollTo(model()->scrollToIndex());
    }
}

void MPopupListView::setupModel()
{
    MDialogView::setupModel();

    Q_D(MPopupListView);
    d->list->setItemModel(model()->itemModel());
    d->list->setSelectionModel(model()->selectionModel());
}

void MPopupListView::scrollTo(const QModelIndex &index) const
{
    const_cast<MPopupListModel*>(model())->setScrollToIndex(index);
}

M_REGISTER_VIEW_NEW(MPopupListView, MPopupList)

#include "moc_mpopuplistview.cpp"

