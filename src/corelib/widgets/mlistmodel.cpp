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

#include "mlistmodel.h"

void MListModel::setScrollToIndex(const QModelIndex &index)
{
    _scrollToIndex() = index;
    memberModified(ScrollToIndex);
}

const QModelIndex &MListModel::scrollToIndex() const
{
    return _scrollToIndex();
}

const QModelIndex& MListModel::firstVisibleItem() const
{
    return _firstVisibleItem();
}

void MListModel::setFirstVisibleItem(const QModelIndex& firstVisibleItem)
{
    _firstVisibleItem() = firstVisibleItem;
    // Do not calling memberModified function to save CPU, controller doesn't need
    // to know when this value is changed, it will just read the current
}

const QModelIndex& MListModel::lastVisibleItem() const
{
    return _lastVisibleItem();
}

void MListModel::setLastVisibleItem(const QModelIndex& lastVisibleItem)
{
    _lastVisibleItem() = lastVisibleItem;
    // Do not calling memberModified function to save CPU, controller doesn't need
    // to know when this value is changed, it will just read the current
}
