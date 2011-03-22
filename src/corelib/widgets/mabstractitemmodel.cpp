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

#include "mabstractitemmodel.h"
#include "mabstractitemmodel_p.h"

MAbstractItemModelPrivate::MAbstractItemModelPrivate()
    : q_ptr(0),
    groupMode(false),
    isAnimatedChange(false)
{
}

MAbstractItemModelPrivate::~MAbstractItemModelPrivate()
{
}

void MAbstractItemModelPrivate::connectModel()
{
    Q_Q(MAbstractItemModel);

    q->connect(q, SIGNAL(rowsInserted(QModelIndex,int,int)), q, SLOT(_q_rowsInsertAnimated(QModelIndex,int,int)));
    q->connect(q, SIGNAL(rowsRemoved(QModelIndex,int,int)), q, SLOT(_q_rowsRemoveAnimated(QModelIndex,int,int)));

    q->connect(q, SIGNAL(layoutChanged()), q, SLOT(_q_layoutChanged()));
}

void MAbstractItemModelPrivate::setGrouped(bool mode)
{
    groupMode = mode;
}

bool MAbstractItemModelPrivate::isGrouped() const
{
    return groupMode;
}

void MAbstractItemModelPrivate::_q_rowsInsertAnimated(const QModelIndex &parent, int first, int last)
{
    Q_Q(MAbstractItemModel);

    emit q->rowsInserted(parent, first, last, isAnimatedChange);
}

void MAbstractItemModelPrivate::_q_rowsRemoveAnimated(const QModelIndex &parent, int first, int last)
{
    Q_Q(MAbstractItemModel);

    emit q->rowsRemoved(parent, first, last, isAnimatedChange);
}

void MAbstractItemModelPrivate::_q_layoutChanged()
{
    isAnimatedChange = false;
}

MAbstractItemModel::MAbstractItemModel(QObject *parent)
    : QAbstractItemModel(parent),
    d_ptr(new MAbstractItemModelPrivate)
{
    Q_D(MAbstractItemModel);

    d->q_ptr = this;
    d->connectModel();
}

MAbstractItemModel::~MAbstractItemModel()
{
    delete d_ptr;
}

void MAbstractItemModel::setGrouped(bool mode)
{
    Q_D(MAbstractItemModel);

    emit layoutAboutToBeChanged();

    if (d->isGrouped() == mode)
        return;

    d->setGrouped(mode);
    emit layoutChanged();
}

bool MAbstractItemModel::isGrouped() const
{
    Q_D(const MAbstractItemModel);

    return d->isGrouped();
}

int MAbstractItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    if (isGrouped()) {
        if (parent.isValid()) {
            if (parent.row() < groupCount() && !parent.parent().isValid())
                return rowCountInGroup(parent.row());
            else
                return 0;
        }
        else
            return groupCount();
    } else {
        if (parent.isValid())
            return 0;
        else
            return rowCountInGroup(-1);
    }
}

QModelIndex MAbstractItemModel::index(int row, int column, const QModelIndex &parent) const
{
    return ((row >= 0 && column >= 0) && row < rowCount(parent)) ? createIndex(row, column, parent.row()) : QModelIndex();
}

QModelIndex MAbstractItemModel::parent(const QModelIndex &child) const
{
    if (isGrouped() && child.isValid()) {
        return index(child.internalId(), 0);
    }

    return QModelIndex();
}

int MAbstractItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

QVariant MAbstractItemModel::data(const QModelIndex &index, int role) const
{
    if (isGrouped() && index.isValid()) {
        if (!index.parent().isValid() && role == Qt::DisplayRole)
            return groupTitle(index.row());
        else
            return itemData(index.row(), index.parent().row(), role);
    } else {
        if (index.isValid())
            return itemData(index.row(), -1, role);
    }

    return QVariant();
}

void MAbstractItemModel::beginInsertRows(const QModelIndex &parent, int first, int last, bool animated)
{
    Q_D(MAbstractItemModel);

    d->isAnimatedChange = animated;
    if (animated)
        emit layoutAboutToBeAnimated();

    QAbstractItemModel::beginInsertRows(parent, first, last);
}

void MAbstractItemModel::beginRemoveRows(const QModelIndex &parent, int first, int last, bool animated)
{
    Q_D(MAbstractItemModel);

    d->isAnimatedChange = animated;
    if (animated)
        emit layoutAboutToBeAnimated();

    QAbstractItemModel::beginRemoveRows(parent, first, last);
}

#include "moc_mabstractitemmodel.cpp"
