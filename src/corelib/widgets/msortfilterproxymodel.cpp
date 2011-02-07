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

#include "msortfilterproxymodel.h"
#include "msortfilterproxymodel_p.h"

#include "mabstractitemmodel.h"

MSortFilterProxyModelPrivate::MSortFilterProxyModelPrivate()
    : q_ptr(0),
    isSupportedModel(false),
    isAnimatedChange(false)
{
}

MSortFilterProxyModelPrivate::~MSortFilterProxyModelPrivate()
{
}

void MSortFilterProxyModelPrivate::init()
{
    Q_Q(MSortFilterProxyModel);

    q->connect(q, SIGNAL(layoutChanged()), q, SLOT(_q_resetAnimatedChange()));

    q->connect(q, SIGNAL(rowsInserted(QModelIndex,int,int)), q, SLOT(_q_rowsInserted(QModelIndex,int,int)));
    q->connect(q, SIGNAL(rowsRemoved(QModelIndex,int,int)), q, SLOT(_q_rowsRemoved(QModelIndex,int,int)));
}

void MSortFilterProxyModelPrivate::connectToSourceModel(QAbstractItemModel *sourceModel)
{
    Q_Q(MSortFilterProxyModel);

    q->disconnect(q, SLOT(_q_layoutAboutToBeAnimated()));

    isSupportedModel = sourceModel && (sourceModel->inherits(MAbstractItemModel::staticMetaObject.className()) ||
                     sourceModel->inherits(MSortFilterProxyModel::staticMetaObject.className()));

    if (isSupportedModel) {
        q->connect(sourceModel, SIGNAL(layoutAboutToBeAnimated()), q, SLOT(_q_layoutAboutToBeAnimated()));
    }
}

void MSortFilterProxyModelPrivate::_q_rowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_Q(MSortFilterProxyModel);
    emit q->rowsInserted(parent, first, last, isAnimatedChange);
    _q_resetAnimatedChange();
}

void MSortFilterProxyModelPrivate::_q_rowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_Q(MSortFilterProxyModel);
    emit q->rowsRemoved(parent, first, last, isAnimatedChange);
    _q_resetAnimatedChange();
}

void MSortFilterProxyModelPrivate::_q_layoutAboutToBeAnimated()
{
    Q_Q(MSortFilterProxyModel);
    emit q->layoutAboutToBeAnimated();
    isAnimatedChange = true;
}

void MSortFilterProxyModelPrivate::_q_resetAnimatedChange()
{
    isAnimatedChange = false;
}

MSortFilterProxyModel::MSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent),
    d_ptr(new MSortFilterProxyModelPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->init();
}

MSortFilterProxyModel::~MSortFilterProxyModel()
{
    delete d_ptr;
}

void MSortFilterProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    Q_D(MSortFilterProxyModel);
    QSortFilterProxyModel::setSourceModel(sourceModel);

    d->connectToSourceModel(sourceModel);
}

bool MSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (!source_parent.isValid()) {
        if (sourceModel()->hasChildren(sourceModel()->index(source_row, 0, QModelIndex())))
            return filterAcceptsGroup(sourceModel()->index(source_row, 0));
    }
   
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool MSortFilterProxyModel::filterAcceptsGroup(const QModelIndex &source_index) const
{
    for (int i = 0; i < sourceModel()->rowCount(source_index); i++)
        if (filterAcceptsRow(i, source_index))
            return true;

    return false;
}

#include "moc_msortfilterproxymodel.cpp"
