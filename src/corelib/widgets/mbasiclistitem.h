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
 
#ifndef MBASICLISTITEM_H
#define MBASICLISTITEM_H

#include "mlistitem.h"

class MImageWidget;
class MLabel;
class QGraphicsGridLayout;

class M_EXPORT MBasicListItem : public MListItem
{
public:
    enum ItemStyle {
        SingleTitle = 0x0,
        TitleWithSubtitle,
        IconWithTitle,
        IconWithTitleAndSubtitle
    };

public:
    MBasicListItem(MBasicListItem::ItemStyle style = SingleTitle, QGraphicsItem *parent = NULL);
    virtual ~MBasicListItem();
    
    void initLayout();
    void clearLayout();
    
    void setItemStyle(ItemStyle itemStyle);
    ItemStyle itemStyle() const;
    
    void setIcon(MImageWidget *icon);
    MImageWidget *icon() const;
    
    void setTitle(const QString &title);
    QString title() const;
    
    void setSubtitle(const QString &subtitle);
    QString subtitle() const;
    
private:
    QGraphicsGridLayout *layoutGrid;

    MImageWidget *iconImage;    
    MLabel *titleLabel;
    MLabel *subtitleLabel;
    
    ItemStyle listItemStyle;
};

#endif // MBASICLISTITEM_H
