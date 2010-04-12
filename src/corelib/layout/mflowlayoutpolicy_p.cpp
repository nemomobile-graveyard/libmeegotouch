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

#include "mflowlayoutpolicy_p.h"

MFlowLayoutPolicyPrivate::MFlowLayoutPolicyPrivate(MLayout *l) :
    MAbstractLayoutPolicyPrivate(l)
{
    numberOfRows = -1;
    rowLimit = -1;
}

MFlowLayoutPolicyPrivate::~MFlowLayoutPolicyPrivate()
{ }

bool MFlowLayoutPolicyPrivate::roomForItemOnCurrentRow(const QSizeF &item_size, const qreal &constraint_width, QPointF *position, qreal *row_height) const
{
    Q_ASSERT(position);
    if (constraint_width != -1 //no width constraint, so everything will go on the first row
            && (position->x() != 0)  //Force the first item on a row to always fit in that row
            && (position->x() + item_size.width() + horizontalSpacing > constraint_width)) // if the item doesn't fit into the current row, return false
        return false;

    else if (position->x() != 0) { //the item fits into the current row and is not the first item
        position->rx() += horizontalSpacing;
        if (row_height)
            *row_height = qMax(*row_height, item_size.height());
    } else if (row_height) //the item fits into the current row and is the first item
        *row_height = item_size.height();
    return true;
}

