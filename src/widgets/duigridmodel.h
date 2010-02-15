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

#ifndef DUIGRIDMODEL_H
#define DUIGRIDMODEL_H

#include <duiwidgetmodel.h>

#include <QRectF>
#include <QHash>
#include <QString>

class QAbstractItemModel;
class QGraphicsItem;

class DUI_EXPORT DuiGridModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiGridModel)

public:

    typedef QHash<int, QString> GroupHeaderHash;

    /*!
      \brief Are all grid items created at once
    */
    enum GridLayoutMode {
        SinglePass,
        Batched
    };

private:

    DUI_MODEL_PROPERTY(int, rowHeight, RowHeight, true, 0)
    DUI_MODEL_PROPERTY(int, spacing, Spacing, true, 5)
    DUI_MODEL_PTR_PROPERTY(QAbstractItemModel *, itemModel, ItemModel, true, 0)
    DUI_MODEL_PROPERTY(int, batchSize, BatchSize, true, 100)
    DUI_MODEL_PROPERTY(DuiGridModel::GridLayoutMode, layoutMode, LayoutMode, true, DuiGridModel::SinglePass)
    DUI_MODEL_PROPERTY(QRectF, internalMargin, InternalMargin, true, QRectF(0, 0, 0, 0))
    DUI_MODEL_PROPERTY(DuiGridModel::GroupHeaderHash, groupHeaders, GroupHeaders, true, DuiGridModel::GroupHeaderHash())
    DUI_MODEL_PTR_PROPERTY(QGraphicsItem *, panningPositionSource, PanningPositionSource, true, 0)
    DUI_MODEL_PROPERTY(bool, itemSelection, ItemSelection, true, false)
};

#endif

