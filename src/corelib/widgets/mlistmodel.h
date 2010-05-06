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

#ifndef MLISTMODEL_H
#define MLISTMODEL_H

#include <QHash>
#include <QAbstractItemModel>
#include <QRectF>
#include <QModelIndexList>
#include <QItemSelectionModel>

#include <mwidgetmodel.h>
#include "mabstractcellcreator.h"

class M_EXPORT MListModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MListModel)

private:

    M_MODEL_PTR_PROPERTY(QAbstractItemModel *, itemModel, ItemModel, true, 0)
    M_MODEL_PTR_PROPERTY(MCellCreator *, cellCreator, CellCreator, true, 0)
    M_MODEL_PROPERTY(QModelIndex, firstVisibleItem, FirstVisibleItem, false, QModelIndex())
    M_MODEL_PROPERTY(QModelIndex, lastVisibleItem, LastVisibleItem, false, QModelIndex())
    M_MODEL_PROPERTY(bool, showGroups, ShowGroups, true, false)
    M_MODEL_PROPERTY(int, columns, Columns, true, 1)
    M_MODEL_PTR_PROPERTY(QItemSelectionModel *, selectionModel, SelectionModel, true, NULL)
    M_MODEL_PROPERTY(bool, listIsMoving, ListIsMoving, true, false)
    M_MODEL_PROPERTY(QModelIndex, scrollToIndex, ScrollToIndex, false, QModelIndex())
    M_MODEL_PROPERTY(int, scrollHint, ScrollHint, true, 0)
};

#endif

