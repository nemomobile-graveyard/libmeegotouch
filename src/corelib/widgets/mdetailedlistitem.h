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
 
#ifndef MDETAILEDLISTITEM_H
#define MDETAILEDLISTITEM_H

#include "mlistitem.h"

class MImageWidget;
class MLabel;
class MStylableWidget;

class QGraphicsGridLayout;

class M_EXPORT MDetailedListItem : public MListItem
{
public:
    enum ItemStyle {
        IconTitleSubtitleAndTwoSideIcons = 0x0,
        IconTitleSubtitleAndSideIconWithLabel,
        ThumbnailTitleSubtitleAndTwoSideIcons,
        ThumbnailTitleAndTwoSideIcons
    };
    
    enum IconStyle {
        Icon = 0x0,
        Thumbnail
    };

public:
    MDetailedListItem(MDetailedListItem::ItemStyle style = IconTitleSubtitleAndTwoSideIcons, QGraphicsItem *parent = NULL);
    virtual ~MDetailedListItem();
    
    void initLayout();
    void clearLayout();
    
    void setItemStyle(ItemStyle itemStyle);
    ItemStyle itemStyle() const;
    
    void setIconStyle(IconStyle style);
    IconStyle iconStyle() const;
    
    void setIcon(MImageWidget *icon);
    MImageWidget *icon() const;
    
    void setThumbnail(MImageWidget *thumbnail);
    MImageWidget *thumbnail() const;
    
    void setSideTopImageWidget(MImageWidget *image);
    MImageWidget *sideTopImageWidget() const;
    
    void setSideBottomImageWidget(MImageWidget *image);
    MImageWidget *sideBottomImageWidget() const;
    
    void setTitle(const QString &title);
    QString title() const;
    
    void setSubtitle(const QString &subtitle);
    QString subtitle() const;
    
    void setSideBottomTitle(const QString &text);
    QString sideBottomTitle() const;
    
private:
    QGraphicsGridLayout *layoutGrid;
    QGraphicsGridLayout *contentLayoutGrid;
    MStylableWidget *contentPanel;

    MImageWidget *iconImage;    
    MImageWidget *sideTopImage;
    MImageWidget *sideBottomImage;
    
    MLabel *titleLabel; 
    MLabel *subtitleLabel;
    MLabel *sideBottomLabel;
    
    ItemStyle listItemStyle;
};


#endif // MDETAILEDLISTITEM_H
