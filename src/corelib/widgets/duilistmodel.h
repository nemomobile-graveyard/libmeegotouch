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

#ifndef DUILISTMODEL_H
#define DUILISTMODEL_H

#include <QHash>
#include <QAbstractItemModel>
#include <QRectF>
#include <QModelIndexList>
#include <QItemSelectionModel>

#include <duiwidgetmodel.h>
#include "duiabstractcellcreator.h"

class DUI_EXPORT DuiListModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiListModel)

private:

    DUI_MODEL_PTR_PROPERTY(QAbstractItemModel *, itemModel, ItemModel, true, 0)
    DUI_MODEL_PTR_PROPERTY(DuiCellCreator *, cellCreator, CellCreator, true, 0)
    DUI_MODEL_PROPERTY(QModelIndex, firstVisibleItem, FirstVisibleItem, false, QModelIndex())
    DUI_MODEL_PROPERTY(QModelIndex, lastVisibleItem, LastVisibleItem, false, QModelIndex())
    DUI_MODEL_PROPERTY(bool, showGroups, ShowGroups, true, false)
    DUI_MODEL_PROPERTY(int, columns, Columns, true, 1)
    DUI_MODEL_PTR_PROPERTY(QItemSelectionModel *, selectionModel, SelectionModel, true, NULL)
    DUI_MODEL_PROPERTY(bool, listIsMoving, ListIsMoving, true, false)
    DUI_MODEL_PROPERTY(QModelIndex, scrollToIndex, ScrollToIndex, false, QModelIndex())
    DUI_MODEL_PROPERTY(int, scrollHint, ScrollHint, true, 0)
};

#endif

