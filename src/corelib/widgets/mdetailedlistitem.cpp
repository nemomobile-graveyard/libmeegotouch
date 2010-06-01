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

#include <MImageWidget>
#include <MLabel>
#include <MStylableWidget>

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
        layoutGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        layoutGrid->setContentsMargins(0, 0, 0, 0);
        layoutGrid->setSpacing(0);
    }
    
    switch (listItemStyle) {
    case MDetailedListItem::IconTitleSubtitleAndTwoSideIcons: {
            iconImage = new MImageWidget(this);
            if (listItemStyle == MDetailedListItem::IconTitleSubtitleAndTwoSideIcons) {
                setObjectName("DetailedItemIconTitleSubtitleAndTwoSideIcons");
                setIconStyle(Icon);
            } else {
                setObjectName("DetailedItemThumbnailTitleSubtitleAndTwoSideIcons");
                setIconStyle(Thumbnail);
            }
            
            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonTitleWithLeftMargin");
            
            subtitleLabel = new MLabel(this);
            subtitleLabel->setObjectName("CommonSubTitleWithLeftMargin");
            
            sideTopImage = new MImageWidget(this);
            sideTopImage->setObjectName("CommonTopSideIcon");
            
            sideBottomImage = new MImageWidget(this);
            sideBottomImage->setObjectName("CommonBottomSideIcon");

            layoutGrid->addItem(iconImage, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabel, 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabel, 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            layoutGrid->addItem(sideTopImage, 0, 2, Qt::AlignRight | Qt::AlignTop);
            layoutGrid->addItem(sideBottomImage, 1, 2, Qt::AlignRight | Qt::AlignBottom);
            
            break;
        }
    case MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel: {
            setObjectName("DetailedItemIconTitleSubtitleAndSideIconWithLabel");

            iconImage = new MImageWidget(this);
            iconImage->setObjectName("CommonMainIcon");
            
            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonTitleWithLeftMargin");
            
            subtitleLabel = new MLabel(this);
            subtitleLabel->setObjectName("CommonSubTitleWithLeftMargin");
            
            sideTopImage = new MImageWidget(this);
            sideTopImage->setObjectName("CommonTopSideIcon");
            
            sideBottomLabel = new MLabel(this);
            sideBottomLabel->setObjectName("CommonBottomSideSubTitle");

            layoutGrid->addItem(iconImage, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabel, 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabel, 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            layoutGrid->addItem(sideTopImage, 0, 2, Qt::AlignRight | Qt::AlignTop);
            layoutGrid->addItem(sideBottomLabel, 1, 2, Qt::AlignRight | Qt::AlignBottom);
            
            break;
        }
    case MDetailedListItem::ThumbnailTitleAndTwoSideIcons: {
            contentPanel = new MStylableWidget(this);
            contentLayoutGrid = new QGraphicsGridLayout(contentPanel);
            contentLayoutGrid->setContentsMargins(0, 0, 0, 0);
            contentLayoutGrid->setSpacing(0);
            contentLayoutGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

            contentPanel->setObjectName("DetailedItemThumbnailTitleAndTwoSideIcons");
            contentPanel->setContentsMargins(0, 8, 0, 8);

            iconImage = new MImageWidget(this);
            setIconStyle(Thumbnail);
            
            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonTitleWithLeftMargin");
            
            sideTopImage = new MImageWidget(this);
            sideTopImage->setObjectName("CommonTopSideIcon");
            
            sideBottomImage = new MImageWidget(this);
            sideBottomImage->setObjectName("CommonBottomSideIcon");

            layoutGrid->addItem(iconImage, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

            contentLayoutGrid->addItem(titleLabel, 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            contentLayoutGrid->addItem(sideTopImage, 0, 1, Qt::AlignRight | Qt::AlignTop);
            contentLayoutGrid->addItem(new QGraphicsWidget, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
            contentLayoutGrid->addItem(sideBottomImage, 2, 1, Qt::AlignRight | Qt::AlignBottom);
            
            contentPanel->setLayout(contentLayoutGrid);
            layoutGrid->addItem(contentPanel, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);

            break;
        }
    case MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons: {
            contentPanel = new MStylableWidget(this);
            contentLayoutGrid = new QGraphicsGridLayout(contentPanel);
            contentLayoutGrid->setContentsMargins(0, 0, 0, 0);
            contentLayoutGrid->setSpacing(0);
            contentLayoutGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

            contentPanel->setObjectName("DetailedItemThumbnailTitleSubtitleAndTwoSideIcons");
            contentPanel->setContentsMargins(0, 8, 0, 8);

            iconImage = new MImageWidget(this);
            setIconStyle(Thumbnail);

            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonTitleWithLeftMargin");

            subtitleLabel = new MLabel(this);
            subtitleLabel->setObjectName("CommonSubTitleWithLeftMargin");

            sideTopImage = new MImageWidget(this);
            sideTopImage->setObjectName("CommonTopSideIcon");

            sideBottomImage = new MImageWidget(this);
            sideBottomImage->setObjectName("CommonBottomSideIcon");

            layoutGrid->addItem(iconImage, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

            contentLayoutGrid->addItem(titleLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
            contentLayoutGrid->addItem(subtitleLabel, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
            contentLayoutGrid->addItem(sideTopImage, 0, 1, Qt::AlignRight | Qt::AlignTop);
            contentLayoutGrid->addItem(sideBottomImage, 1, 1, Qt::AlignRight | Qt::AlignBottom);

            contentPanel->setLayout(contentLayoutGrid);
            layoutGrid->addItem(contentPanel, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);

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

        if (contentLayoutGrid) {
            for (int i = 0; i < contentLayoutGrid->count(); i++) {
                QGraphicsLayoutItem *item = contentLayoutGrid->itemAt(0);
                contentLayoutGrid->removeAt(0);
                delete item;
            }
            delete contentLayoutGrid;
            contentLayoutGrid = NULL;
            delete contentPanel;
            contentPanel = NULL;
        }
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

