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
 
#include "mdetailedlistitem.h"

#include <MLabel>
#include <MImageWidget>

#include <QGraphicsGridLayout>

MDetailedListItem::MDetailedListItem(MDetailedListItem::ItemStyle style, QGraphicsItem *parent)
    : MListItem(parent),
    layoutGrid(NULL),
    iconImage(NULL),
    titleLabel(NULL),
    subtitleLabel(NULL),
    listItemStyle(style)
{
    setItemStyle(style);
}

MDetailedListItem::~MDetailedListItem()
{
}

void MDetailedListItem::initLayout()
{
    clearLayout();
    
    if (!layoutGrid) {
        layoutGrid = new QGraphicsGridLayout(this);
        layoutGrid->setContentsMargins(0, 0, 0, 0);
        layoutGrid->setSpacing(0);
    }
    
    switch (listItemStyle) {
    case MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons :
    case MDetailedListItem::IconTitleSubtitleAndTwoSideIcons: {
            iconImage = new MImageWidget(this);
            iconImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            if (listItemStyle == MDetailedListItem::IconTitleSubtitleAndTwoSideIcons)
                setIconStyle(Icon);
            else
                setIconStyle(Thumbnail);
            
            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonTitle");
            
            subtitleLabel = new MLabel(this);
            subtitleLabel->setObjectName("CommonSubTitle");
            
            sideTopImage = new MImageWidget(this);
            sideTopImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            sideTopImage->setObjectName("CommonSubIcon");
            
            sideBottomImage = new MImageWidget(this);
            sideBottomImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            sideBottomImage->setObjectName("CommonSubIcon");

            layoutGrid->addItem(iconImage, 0, 0, 4, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabel, 1, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabel, 2, 1, Qt::AlignLeft | Qt::AlignBottom);
            layoutGrid->addItem(sideTopImage, 1, 2, Qt::AlignRight | Qt::AlignTop);
            layoutGrid->addItem(sideBottomImage, 2, 2, Qt::AlignRight | Qt::AlignBottom);
            layoutGrid->addItem(new QGraphicsWidget, 3, 1, 1, 2);
            
            break;
        }
    case MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel: {
            iconImage = new MImageWidget(this);
            iconImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            iconImage->setObjectName("CommonMainIcon");
            
            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonTitle");
            
            subtitleLabel = new MLabel(this);
            subtitleLabel->setObjectName("CommonSubTitle");
            
            sideTopImage = new MImageWidget(this);
            sideTopImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            sideTopImage->setObjectName("CommonSubIcon");
            
            sideBottomLabel = new MLabel(this);
            sideBottomLabel->setObjectName("CommonSubText");

            layoutGrid->addItem(iconImage, 0, 0, 4, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabel, 1, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabel, 2, 1, Qt::AlignLeft | Qt::AlignBottom);
            layoutGrid->addItem(sideTopImage, 1, 2, Qt::AlignRight | Qt::AlignTop);
            layoutGrid->addItem(sideBottomLabel, 2, 2, Qt::AlignRight | Qt::AlignBottom);
            layoutGrid->addItem(new QGraphicsWidget, 3, 1, 1, 2);
            
            break;
        }
    case MDetailedListItem::ThumbnailSmallTitleAndTwoSideIcons: {
            iconImage = new MImageWidget(this);
            iconImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            setIconStyle(Thumbnail);
            
            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonSubText");
            
            sideTopImage = new MImageWidget(this);
            sideTopImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            sideTopImage->setObjectName("CommonSubIcon");
            
            sideBottomImage = new MImageWidget(this);
            sideBottomImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            sideBottomImage->setObjectName("CommonSubIcon");

            layoutGrid->addItem(iconImage, 0, 0, 4, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabel, 1, 1, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(sideTopImage, 1, 2, Qt::AlignRight | Qt::AlignTop);
            layoutGrid->addItem(sideBottomImage, 2, 2, Qt::AlignRight | Qt::AlignBottom);
            layoutGrid->addItem(new QGraphicsWidget, 3, 1, 1, 2);
            
            break;
        }
    default:
        break;
    }
}

void MDetailedListItem::clearLayout()
{
    if (layoutGrid) {
        for (int i = 0; i < layoutGrid->count(); i++) {
            QGraphicsLayoutItem *item = layoutGrid->itemAt(0);
            layoutGrid->removeAt(0);
            delete item;
        }
        iconImage = NULL;
        titleLabel = NULL;
        subtitleLabel = NULL;
        sideTopImage = NULL;
        sideBottomImage = NULL;
        sideBottomLabel = NULL;
    }
}

void MDetailedListItem::setItemStyle(ItemStyle itemStyle)
{
    if (itemStyle == listItemStyle)
        return;
    
    initLayout();
}

MDetailedListItem::ItemStyle MDetailedListItem::itemStyle() const
{
    return listItemStyle;
}

void MDetailedListItem::setIconStyle(IconStyle style)
{
    if (style == Thumbnail)
        iconImage->setObjectName("CommonThumbnail");
    else if (style == Icon)
        iconImage->setObjectName("CommonMainIcon");
}

void MDetailedListItem::setIcon(MImageWidget *icon)
{
    for (int i = 0; i < layoutGrid->count(); i++) {
        if (layoutGrid->itemAt(i) == iconImage) {
            layoutGrid->removeAt(i);
            delete iconImage;
            iconImage = NULL;
            break;
        }
    }

    if (icon) {
        iconImage = icon;
        if (listItemStyle == MDetailedListItem::IconTitleSubtitleAndTwoSideIcons)
            layoutGrid->addItem(iconImage, 0, 0, 4, 1, Qt::AlignLeft | Qt::AlignVCenter);
    }
}

MImageWidget *MDetailedListItem::icon() const
{
    return iconImage;
}


void MDetailedListItem::setThumbnail(MImageWidget *thumbnail)
{
    setIcon(thumbnail);
}

MImageWidget *MDetailedListItem::thumbnail() const
{
    return icon();
}

void MDetailedListItem::setSideTopImageWidget(MImageWidget *image)
{
    for (int i = 0; i < layoutGrid->count(); i++) {
        if (layoutGrid->itemAt(i) == sideTopImage) {
            layoutGrid->removeAt(i);
            delete sideTopImage;
            sideTopImage = NULL;
            break;
        }
    }

    if (image) {
        sideTopImage = image;
        layoutGrid->addItem(sideTopImage, 1, 2, Qt::AlignRight | Qt::AlignTop);
    }
}

MImageWidget *MDetailedListItem::sideTopImageWidget() const
{
    return sideTopImage;
}

void MDetailedListItem::setSideBottomImageWidget(MImageWidget *image)
{
    for (int i = 0; i < layoutGrid->count(); i++) {
        if (layoutGrid->itemAt(i) == sideBottomImage) {
            layoutGrid->removeAt(i);
            delete sideBottomImage;
            sideBottomImage = NULL;
            break;
        }
    }

    if (image) {
        sideBottomImage = image;
        if (listItemStyle == MDetailedListItem::IconTitleSubtitleAndTwoSideIcons ||
            listItemStyle == MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons)
            layoutGrid->addItem(sideBottomImage, 2, 2, Qt::AlignRight | Qt::AlignBottom);
    }
}

MImageWidget *MDetailedListItem::sideBottomImageWidget() const
{
    return sideBottomImage;
}

void MDetailedListItem::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

QString MDetailedListItem::title() const
{
    return titleLabel->text();
}

void MDetailedListItem::setSubtitle(const QString &subtitle)
{
    subtitleLabel->setText(subtitle);    
}

QString MDetailedListItem::subtitle() const
{
    return subtitleLabel->text();
}

void MDetailedListItem::setSideBottomTitle(const QString &text)
{
    sideBottomLabel->setText(text);
}

QString MDetailedListItem::sideBottomTitle() const
{
    return sideBottomLabel->text();
}

