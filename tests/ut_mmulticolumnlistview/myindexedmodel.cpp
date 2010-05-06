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

#include "myindexedmodel.h"

MyIndexedModel::MyIndexedModel(QObject *r) : root(r)
{
}

MyIndexedModel::~MyIndexedModel()
{
    delete root;
}

QModelIndex MyIndexedModel::index(int row, int column, const QModelIndex &parent) const
{
    QObject *parentObject;

    if (!parent.isValid())
        parentObject = root;
    else
        parentObject = static_cast<QObject *>(parent.internalPointer());

    if (row >= 0 && row < parentObject->children().size())
        return createIndex(row, column, parentObject->children().at(row));
    else
        return QModelIndex();
}

QModelIndex MyIndexedModel::parent(const QModelIndex &child) const
{
    if (child == QModelIndex() || !child.isValid())
        return QModelIndex();

    QObject *object = static_cast<QObject *>(child.internalPointer());
    QObject *parent = object->parent();

    if (parent == root)
        return QModelIndex();

    QObject *grandParent = parent->parent();

    return createIndex(grandParent->children().indexOf(parent), 0, parent);
}

int MyIndexedModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return root->children().size();

    QObject *parentObject = static_cast<QObject *>(parent.internalPointer());
    return parentObject->children().size();
}

int MyIndexedModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant MyIndexedModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return (static_cast<QObject *>(index.internalPointer()))->objectName();

    return QVariant();
}
