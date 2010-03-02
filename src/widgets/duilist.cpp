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

#include <QVector>
#include <QGraphicsSceneEvent>
#include <QGraphicsGridLayout>
#include <QItemSelectionModel>

#include <DuiDebug>
#include "duilist.h"
#include "duitheme.h"
#include "duibutton.h"
#include "duilist_p.h"
#include "duilabel.h"
#include "duipannableviewport.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiList)

DuiListPrivate::DuiListPrivate()
{
}

DuiListPrivate::~DuiListPrivate()
{
}

void DuiListPrivate::init()
{
    Q_Q(DuiList);

    q->setSelectionMode(DuiList::NoSelection);
    q->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding)); //grow to available space in both directions
}

DuiList::DuiList(DuiListPrivate *dd, DuiListModel *model, QGraphicsItem *parent)
    : DuiWidgetController(dd, model, parent)
{
    Q_D(DuiList);
    d->init();
}

DuiList::DuiList(QGraphicsItem *parent)
    : DuiWidgetController(new DuiListPrivate, new DuiListModel, parent)

{
    Q_D(DuiList);
    d->init();
}

DuiList::~DuiList()
{
}

void DuiList::updateData(const QList<const char *>& modifications)
{
    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];
        if (member == DuiListModel::ListIsMoving) {
            model()->listIsMoving() ? emit panningStarted() : emit panningStopped();
        }
    }
}

void DuiList::setItemModel(QAbstractItemModel *itemModel)
{
    setSelectionModel(NULL);

    if (itemModel)
        setSelectionModel(new QItemSelectionModel(itemModel));

    model()->setItemModel(itemModel);
}


QAbstractItemModel *DuiList::itemModel() const
{
    return model()->itemModel();
}

void DuiList::scrollTo(const QModelIndex &index)
{
    scrollTo(index, DuiList::EnsureVisibleHint);
}

void DuiList::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    emit scrollToIndex(index);

    model()->beginTransaction();
    model()->setScrollHint(hint);
    model()->setScrollToIndex(index);
    model()->commitTransaction();
}

QItemSelectionModel *DuiList::selectionModel() const
{
    return model()->selectionModel();
}

void DuiList::setSelectionModel(QItemSelectionModel *selectionModel)
{
    if (selectionModel == this->selectionModel()) {
        return;
    }

    model()->setSelectionModel(selectionModel);
    emit selectionModelChanged(selectionModel);
}

void DuiList::selectItem(const QModelIndex &index)
{
    QItemSelectionModel *sModel = selectionModel();

    if (index.isValid() && sModel->model() != index.model()) {
        qWarning("DuiList::selectItem() failed: "
                 "Trying to select an item that is for"
                 " a different model than the view ");
        return;
    }

    if (sModel != NULL) {
        if (selectionMode() == DuiList::MultiSelection) {
            if (sModel->isSelected(index)) {
                sModel->select(index, QItemSelectionModel::Deselect);
            } else {
                sModel->select(index, QItemSelectionModel::Select);
            }
        } else if (selectionMode() == DuiList::SingleSelection) {
            sModel->select(index, QItemSelectionModel::SelectCurrent);
        }
    }

    emit itemClicked(index);
}

void DuiList::setCellCreator(DuiCellCreator *itemCreator)
{
    model()->setCellCreator(itemCreator);
}

const DuiCellCreator *DuiList::cellCreator() const
{
    return model()->cellCreator();
}

const QModelIndex DuiList::firstVisibleItem() const
{
    return model()->firstVisibleItem();
}

const QModelIndex DuiList::lastVisibleItem() const
{
    return model()->lastVisibleItem();
}

bool DuiList::showGroups() const
{
    return model()->showGroups();
}

void DuiList::setShowGroups(bool showGroups)
{
    model()->setShowGroups(showGroups);
}

int DuiList::columns() const
{
    return model()->columns();
}

void DuiList::setColumns(int columns)
{
    model()->setColumns(columns);
}

void DuiList::setSelectionMode(DuiList::SelectionMode mode)
{
    Q_D(DuiList);
    d->selectionMode = mode;

    QItemSelectionModel *sModel = selectionModel();

    if (sModel)
        sModel->clearSelection();
}

DuiList::SelectionMode DuiList::selectionMode() const
{
    Q_D(const DuiList);
    return d->selectionMode;
}
