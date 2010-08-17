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

MSortFilterProxyModel::MSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

MSortFilterProxyModel::~MSortFilterProxyModel()
{
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
