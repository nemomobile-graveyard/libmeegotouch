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

#ifndef MGRIDMODEL_H
#define MGRIDMODEL_H

#include <mwidgetmodel.h>

#include <QRectF>
#include <QHash>
#include <QString>

class QAbstractItemModel;
class QGraphicsItem;

class M_CORE_EXPORT MGridModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MGridModel)

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

    M_MODEL_PROPERTY(int, rowHeight, RowHeight, true, 0)
    M_MODEL_PROPERTY(int, spacing, Spacing, true, 5)
    M_MODEL_PTR_PROPERTY(QAbstractItemModel *, itemModel, ItemModel, true, 0)
    M_MODEL_PROPERTY(int, batchSize, BatchSize, true, 100)
    M_MODEL_PROPERTY(MGridModel::GridLayoutMode, layoutMode, LayoutMode, true, MGridModel::SinglePass)
    M_MODEL_PROPERTY(QRectF, internalMargin, InternalMargin, true, QRectF(0, 0, 0, 0))
    M_MODEL_PROPERTY(MGridModel::GroupHeaderHash, groupHeaders, GroupHeaders, true, MGridModel::GroupHeaderHash())
    M_MODEL_PTR_PROPERTY(QGraphicsItem *, panningPositionSource, PanningPositionSource, true, 0)
    M_MODEL_PROPERTY(bool, itemSelection, ItemSelection, true, false)
};

#endif

