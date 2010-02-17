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
    : controller(0), list(0), itemModel(0), selectionModel(0)
{
}

DuiPopupListViewPrivate::~DuiPopupListViewPrivate()
{
}

void DuiPopupListViewPrivate::init()
{
    Q_Q(DuiPopupListView);

    // container which contain the gridItems
    list = new DuiList();
    list->setCellCreator(this);
    list->setSelectionMode(DuiList::SingleSelection);
    q->contentsLayout()->insertItem(0, list);

    QObject::connect(list, SIGNAL(itemClicked(QModelIndex)),
                     controller, SLOT(click(QModelIndex)));

    QObject::connect(controller, SIGNAL(itemModelChanged(QAbstractItemModel *)),
                     q, SLOT(setItemModel(QAbstractItemModel *)));
    QObject::connect(controller, SIGNAL(selectionModelChanged(QItemSelectionModel *)),
                     q, SLOT(setSelectionModel(QItemSelectionModel *)));
    QObject::connect(controller, SIGNAL(scrollToIndex(QModelIndex)),
                     q, SLOT(scrollTo(QModelIndex)));

    // just for preserving ABI
    QObject::connect(q->contentsViewport(), SIGNAL(sizePosChanged(QSizeF, QRectF, QPointF)),
                     q, SLOT(sizePosChanged(QSizeF, QRectF, QPointF)));

    if (controller->itemModel() != NULL) {
        q->setItemModel(controller->itemModel());
        q->setSelectionModel(controller->selectionModel());
    }

}

void DuiPopupListViewPrivate::updateCell(const QModelIndex& index, DuiWidget * cell) const
{
    DuiPopupListItem* item = static_cast<DuiPopupListItem*>(cell);

    QString title;
    QPixmap pixmap;
    QVariant value;

    value = itemModel->data(index, Qt::DisplayRole);
    if (value != QVariant())
        title = value.toString();

    value = itemModel->data(index, Qt::DecorationRole);
    if (value != QVariant())
        pixmap = *DuiTheme::pixmap(value.toString());

    item->setTitle(title);
    item->setPixmap(pixmap);

    if (selectionModel->isSelected(index))
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

void DuiPopupListView::setItemModel(QAbstractItemModel *itemModel)
{
    Q_D(DuiPopupListView);

    // just for preserving ABI
    if (d->itemModel) {
        disconnect(d->itemModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                   this, SLOT(dataChanged(QModelIndex, QModelIndex)));

        disconnect(d->itemModel, SIGNAL(rowsInserted(QModelIndex, int, int)),
                   this, SLOT(rowsInserted(QModelIndex, int, int)));

        disconnect(d->itemModel, SIGNAL(rowsRemoved(QModelIndex, int, int)),
                   this, SLOT(rowsRemoved(QModelIndex, int, int)));
    }

    d->itemModel = itemModel;
    d->list->setItemModel(itemModel);

    // just for preserving ABI
    if (d->itemModel) {
        connect(d->itemModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                this, SLOT(dataChanged(QModelIndex, QModelIndex)));

        connect(d->itemModel, SIGNAL(rowsInserted(QModelIndex, int, int)),
                this, SLOT(rowsInserted(QModelIndex, int, int)));

        connect(d->itemModel, SIGNAL(rowsRemoved(QModelIndex, int, int)),
                this, SLOT(rowsRemoved(QModelIndex, int, int)));
    }
}

void DuiPopupListView::setSelectionModel(QItemSelectionModel *selectionModel)
{
    Q_D(DuiPopupListView);

    // just for preserving ABI
    if (d->selectionModel) {
        disconnect(d->selectionModel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                   this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    }

    d->selectionModel = selectionModel;
    d->list->setSelectionModel(selectionModel);

    // just for preserving ABI
    if (d->selectionModel) {
        connect(d->selectionModel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
    }
}

void DuiPopupListView::sizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos)
{
    Q_UNUSED(viewportSize)
    Q_UNUSED(pannedRange)
    Q_UNUSED(pannedPos)
}

void DuiPopupListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)
}

void DuiPopupListView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
}

void DuiPopupListView::rowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
}

void DuiPopupListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)
}

void DuiPopupListView::scrollTo(const QModelIndex &index) const
{
    Q_D(const DuiPopupListView);
    d->list->scrollTo(index, DuiList::EnsureVisibleHint);
}

DUI_REGISTER_VIEW_NEW(DuiPopupListView, DuiPopupList)

#include "moc_duipopuplistview.cpp"

