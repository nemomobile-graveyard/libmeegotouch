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
#include "mlistfilter.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MList)

MListPrivate::MListPrivate() : selectionMode(MList::NoSelection), listFilter(0)
{
}

MListPrivate::~MListPrivate()
{
    delete listFilter;
}

void MListPrivate::init()
{
    Q_Q(MList);

    q->setOptimizationFlags(MList::DontCallCreateCellDuringUpdate);
    q->setSelectionMode(MList::NoSelection);
    q->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding)); //grow to available space in both directions
}

void MListPrivate::updateLongTapConnections()
{
    Q_Q(MList);

    if (q->receivers(SIGNAL(itemLongTapped(QModelIndex))) > 0  ||
        q->receivers(SIGNAL(itemLongTapped(QModelIndex,QPointF))) > 0) {
        q->model()->setLongTapEnabled(true);
    } else {
        q->model()->setLongTapEnabled(false);
    }
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
    delete model()->cellCreator();
    delete model()->headerCreator();
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
    MWidgetController::contextMenuEvent(event);
}

void MList::setItemModel(QAbstractItemModel *itemModel)
{
    Q_D(MList);
    if (d->listFilter && d->listFilter->enabled())
        itemModel = d->listFilter->updateItemModel(itemModel);

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
    scrollTo(index, MList::EnsureVisibleHint, MList::Animated);
}

void MList::scrollTo(const QModelIndex &index, AnimationMode mode)
{
    scrollTo(index, MList::EnsureVisibleHint, mode);
}

void MList::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    scrollTo(index, hint, MList::Animated);
}

void MList::scrollTo(const QModelIndex &index, ScrollHint hint, AnimationMode mode)
{
    emit scrollToIndex(index);

    model()->beginTransaction();
    model()->setScrollHint(hint);
    model()->setScrollToIndex(index);
    model()->setAnimationMode(mode);
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
            sModel->select(index, QItemSelectionModel::ClearAndSelect);
        }
    }

    emit itemClicked(index);
}

void MList::longTapItem(const QModelIndex &index)
{
    longTapItem(index, QPointF());
}

void MList::longTapItem(const QModelIndex &index, const QPointF &position)
{
    emit itemLongTapped(index);
    emit itemLongTapped(index, position);
}

void MList::setCellCreator(MCellCreator *itemCreator)
{
    delete model()->cellCreator();

    model()->setCellCreator(itemCreator);
}

const MCellCreator *MList::cellCreator() const
{
    return model()->cellCreator();
}

void MList::setHeaderCreator(MCellCreator *headerCreator)
{
    model()->setHeaderCreator(headerCreator);
}

const MCellCreator *MList::headerCreator() const
{
    return model()->headerCreator();
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

void MList::setIndexVisible(bool visible)
{
    if (visible)
        setIndexDisplayMode(MList::Show);
    else
        setIndexDisplayMode(MList::Hide);
}

void MList::setIndexDisplayMode(MList::DisplayMode displayMode)
{
    model()->setListIndexDisplayMode(displayMode);

    if(displayMode == MList::Hide)
        model()->setListIndexVisible(false);
    else
        model()->setListIndexVisible(true);
}

MList::DisplayMode MList::indexDisplayMode() const
{
    return static_cast<MList::DisplayMode>(model()->listIndexDisplayMode());
}

bool MList::indexVisible()
{
    return model()->listIndexVisible();
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

MListFilter *MList::filtering() const
{
    MListPrivate* d = static_cast<MListPrivate*>(d_ptr);
    if (d->listFilter == 0) {
        // Postpone the creation of the list filter until it is requested
        // to speedup the startup performance
        d->listFilter = new MListFilter(const_cast<MList*>(this));
        d->listFilter->setEnabled(false);
    }
    return d->listFilter;
}

void MList::keyPressEvent(QKeyEvent *event)
{
    Q_D(MList);

    if (d->listFilter && d->listFilter->enabled())
        d->listFilter->keyPressEvent(event);
}

void MList::connectNotify(const char *signal)
{
    Q_D(MList);
    if (QLatin1String(signal) == SIGNAL(itemLongTapped(QModelIndex)) ||
        QLatin1String(signal) == SIGNAL(itemLongTapped(QModelIndex,QPointF))) {
        d->updateLongTapConnections();
    }
}

void MList::disconnectNotify(const char *signal)
{
    Q_D(MList);
    if (QLatin1String(signal) == SIGNAL(itemLongTapped(QModelIndex)) ||
        QLatin1String(signal) == SIGNAL(itemLongTapped(QModelIndex,QPointF))) {
        d->updateLongTapConnections();
    }
}

MList::ListOptimizationFlags MList::optimizationFlags() const
{
    Q_D(const MList);
    return ListOptimizationFlags(d->optimizationFlags);
}

void MList::setOptimizationFlag(ListOptimizationFlag optimizationFlag, bool enabled)
{
    Q_D(MList);
    if (enabled)
        setOptimizationFlags(ListOptimizationFlags(d->optimizationFlags) | optimizationFlag);
    else
        setOptimizationFlags(ListOptimizationFlags(d->optimizationFlags) & ~optimizationFlag);
}

void MList::setOptimizationFlags(ListOptimizationFlags optimizationFlags)
{
    Q_D(MList);
    d->optimizationFlags = optimizationFlags;
}
