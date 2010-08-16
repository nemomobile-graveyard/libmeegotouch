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

#ifndef MDETAILEDLISTITEM_P_H
#define MDETAILEDLISTITEM_P_H

#include <QObject>
#include "mdetailedlistitem.h"

class MDetailedListItemPrivate
{
public:
    MDetailedListItemPrivate(MDetailedListItem::ItemStyle style);
    virtual ~MDetailedListItemPrivate();

    virtual void createLayout();
    virtual void clearLayout();

    QGraphicsGridLayout *layout();

    void applyIconStyle();

private:
    Q_DECLARE_PUBLIC(MDetailedListItem)

    MDetailedListItem *q_ptr;

private:
    QGraphicsGridLayout *layoutGrid;

    MImageWidget *image;
    MImageWidget *sideTopImage;
    MImageWidget *sideBottomImage;

    MLabel *titleLabel;
    MLabel *subtitleLabel;
    MLabel *sideBottomLabel;

    bool isLayoutInitialized;
    MDetailedListItem::ItemStyle listItemStyle;
    MDetailedListItem::IconStyle iconStyle;
};

#endif // MDETAILEDLISTITEM_P_H
