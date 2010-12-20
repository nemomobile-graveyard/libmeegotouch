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

#ifndef MBASICLISTITEM_P_H
#define MBASICLISTITEM_P_H

#include <QObject>
#include "mbasiclistitem.h"

class MImageWidget;
class MLabel;
class QGraphicsGridLayout;

class MBasicListItemPrivate
{
public:
    MBasicListItemPrivate(MBasicListItem::ItemStyle style);
    virtual ~MBasicListItemPrivate();

    virtual void createLayout();
    virtual void clearLayout();

    QGraphicsGridLayout *layout();

    void updateWidgetVisiblity();

private:
    Q_DECLARE_PUBLIC(MBasicListItem)

    MBasicListItem *q_ptr;
    QGraphicsGridLayout *layoutGrid;

    MImageWidget *image;
    MLabel *titleLabel;
    MLabel *subtitleLabel;

    bool isLayoutInitialized;
    MBasicListItem::ItemStyle listItemStyle;
};

#endif // MBASICLISTITEM_P_H
