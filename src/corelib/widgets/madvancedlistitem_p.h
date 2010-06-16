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

#ifndef MADVANCEDLISTITEM_P_H
#define MADVANCEDLISTITEM_P_H

#include "madvancedlistitem.h"

class MAdvancedListItemPrivate
{
public:
    MAdvancedListItemPrivate(MAdvancedListItem::ItemStyle style);
    virtual ~MAdvancedListItemPrivate();

    virtual void createLayout();
    virtual void clearLayout();

    QGraphicsGridLayout *layout();

private:
    Q_DECLARE_PUBLIC(MAdvancedListItem)

    MAdvancedListItem *q_ptr;

private:
    QGraphicsGridLayout *layoutGrid;

    MProgressIndicator *progress;
    MLabel *titleLabel;
    MImageWidget *image;
    MImageWidget *sideTopImage;
    MImageWidget *sideBottomImage;

    bool isLayoutInitialized;
    MAdvancedListItem::ItemStyle listItemStyle;
};

#endif // MADVANCEDLISTITEM_P_H
