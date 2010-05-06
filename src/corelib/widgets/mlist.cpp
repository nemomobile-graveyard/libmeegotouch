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

#include <QVector>
#include <QGraphicsSceneEvent>
#include <QGraphicsGridLayout>
#include <QItemSelectionModel>

#include <MDebug>
#include "mlist.h"
#include "mtheme.h"
#include "mbutton.h"
#include "mlist_p.h"
#include "mlabel.h"
#include "mpannableviewport.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MList)

        MListPrivate::MListPrivate() : selectionMode(MList::NoSelection)
{
}

MListPrivate::~MListPrivate()
{
}

void MListPrivate::init()
{
    Q_Q(MList);

    q->setSelectionMode(MList::NoSelection);
    q->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding)); //grow to available space in both directions
}

MList::MList(MListPrivate *dd, MListModel *model, QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    Q_D(MList);
    d->init();
}

MList::MList(QGraphicsItem *parent)
    : MWidgetController(new MListPrivate, new MListModel, parent)

{
    Q_D(MList);
    d->init();
}

MList::~MList()
{
}

void MList::updateData(const QList<const char *>& modifications)
{
    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];
        if (member == MListModel::ListIsMoving) {
            model()->listIsMoving() ? emit panningStarted() : emit panningStopped();
        }
    }
}

void MList::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    model()->setLongTap(event->pos());

    MWidgetController::contextMenuEvent(event);
}

void MList::setItemModel(QAbstractItemModel *itemModel)
{
    setSelectionModel(NULL);

    if (itemModel)
        setSelectionModel(new QItemSelectionModel(itemModel));

    model()->setItemModel(itemModel);
}


QAbstractItemModel *MList::itemModel() const
{
    return model()->itemModel();
}

void MList::scrollTo(const QModelIndex &index)
{
    scrollTo(index, MList::EnsureVisibleHint);
}

void MList::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    emit scrollToIndex(index);

    model()->beginTransaction();
    model()->setScrollHint(hint);
    model()->setScrollToIndex(index);
    model()->commitTransaction();
}

QItemSelectionModel *MList::selectionModel() const
{
    return model()->selectionModel();
}

void MList::setSelectionModel(QItemSelectionModel *selectionModel)
{
    if (selectionModel == this->selectionModel()) {
        return;
    }

    model()->setSelectionModel(selectionModel);
    emit selectionModelChanged(selectionModel);
}

void MList::selectItem(const QModelIndex &index)
{
    QItemSelectionModel *sModel = selectionModel();

    if (index.isValid() && sModel->model() != index.model()) {
        qWarning("MList::selectItem() failed: "
                 "Trying to select an item that is for"
                 " a different model than the view ");
        return;
    }

    if (sModel != NULL) {
        if (selectionMode() == MList::MultiSelection) {
            if (sModel->isSelected(index)) {
                sModel->select(index, QItemSelectionModel::Deselect);
            } else {
                sModel->select(index, QItemSelectionModel::Select);
            }
        } else if (selectionMode() == MList::SingleSelection) {
            sModel->select(index, QItemSelectionModel::SelectCurrent);
        }
    }

    emit itemClicked(index);
}

void MList::longTapItem(const QModelIndex &index)
{
    emit itemLongTapped(index);
}

void MList::setCellCreator(MCellCreator *itemCreator)
{
    model()->setCellCreator(itemCreator);
}

const MCellCreator *MList::cellCreator() const
{
    return model()->cellCreator();
}

const QModelIndex MList::firstVisibleItem() const
{
    return model()->firstVisibleItem();
}

const QModelIndex MList::lastVisibleItem() const
{
    return model()->lastVisibleItem();
}

bool MList::showGroups() const
{
    return model()->showGroups();
}

void MList::setShowGroups(bool showGroups)
{
    model()->setShowGroups(showGroups);
}

int MList::columns() const
{
    return model()->columns();
}

void MList::setColumns(int columns)
{
    model()->setColumns(columns);
}

void MList::setSelectionMode(MList::SelectionMode mode)
{
    Q_D(MList);
    d->selectionMode = mode;

    QItemSelectionModel *sModel = selectionModel();

    if (sModel)
        sModel->clearSelection();
}

MList::SelectionMode MList::selectionMode() const
{
    Q_D(const MList);
    return d->selectionMode;
}
