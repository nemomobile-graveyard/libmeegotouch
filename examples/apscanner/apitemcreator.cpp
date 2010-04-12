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

#include "apitemcreator.h"
#include "apitemmodel.h"

void APItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();
    contentItem->setTitle(rowData[0]);
    if (rowData.count() > 1)
        contentItem->setSubtitle(rowData[1]);
    contentItem->setPixmap(QPixmap(rowData[2]));
    contentItem->boundingRect();

    if (index.row() == 0)
        contentItem->setItemMode(MContentItem::SingleColumnTop);
    else if (index.sibling(index.row() + 1, 0).isValid())
        contentItem->setItemMode(MContentItem::SingleColumnCenter);
    else
        contentItem->setItemMode(MContentItem::SingleColumnBottom);
}
