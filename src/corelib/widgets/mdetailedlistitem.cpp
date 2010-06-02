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
    image(NULL),
    sideTopImage(NULL),
    sideBottomImage(NULL),
    titleLabel(NULL),
    subtitleLabel(NULL),
    sideBottomLabel(NULL),
    listItemStyle(style)
{
    setItemStyle(style);
}

MDetailedListItem::~MDetailedListItem()
{
}

void MDetailedListItem::initLayout()
{
    setLayout(createLayout());
}

QGraphicsLayout *MDetailedListItem::createLayout()
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
            setObjectName("DetailedListItemIconTitleSubtitleAndTwoSideIcons");
            setIconStyle(Icon);
            
            layoutGrid->addItem(imageWidget(), 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabelWidget(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            layoutGrid->addItem(sideTopImageWidget(), 0, 2, Qt::AlignRight | Qt::AlignTop);
            layoutGrid->addItem(sideBottomImageWidget(), 1, 2, Qt::AlignRight | Qt::AlignBottom);
            
            break;
        }
    case MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel: {
            setObjectName("DetailedListItemIconTitleSubtitleAndSideIconWithLabel");
            setIconStyle(Icon);

            layoutGrid->addItem(imageWidget(), 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabelWidget(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            layoutGrid->addItem(sideTopImageWidget(), 0, 2, Qt::AlignRight | Qt::AlignTop);
            layoutGrid->addItem(sideBottomLabelWidget(), 1, 2, Qt::AlignRight | Qt::AlignBottom);
            
            break;
        }
    case MDetailedListItem::ThumbnailTitleAndTwoSideIcons: {
            contentPanel = new MStylableWidget(this);
            contentLayoutGrid = new QGraphicsGridLayout(contentPanel);
            contentLayoutGrid->setContentsMargins(0, 0, 0, 0);
            contentLayoutGrid->setSpacing(0);
            contentLayoutGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

            contentPanel->setObjectName("DetailedListItemThumbnailTitleAndTwoSideIcons");
            contentPanel->setContentsMargins(0, 8, 16, 8);

            setIconStyle(Thumbnail);

            layoutGrid->addItem(imageWidget(), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

            contentLayoutGrid->addItem(titleLabelWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            contentLayoutGrid->addItem(sideTopImageWidget(), 0, 1, Qt::AlignRight | Qt::AlignTop);
            contentLayoutGrid->addItem(new QGraphicsWidget, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
            contentLayoutGrid->addItem(sideBottomImageWidget(), 2, 1, Qt::AlignRight | Qt::AlignBottom);
            
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

            contentPanel->setObjectName("DetailedListItemThumbnailTitleSubtitleAndTwoSideIcons");
            contentPanel->setContentsMargins(0, 8, 16, 8);

            setIconStyle(Thumbnail);

            layoutGrid->addItem(imageWidget(), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

            contentLayoutGrid->addItem(titleLabelWidget(), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
            contentLayoutGrid->addItem(subtitleLabelWidget(), 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
            contentLayoutGrid->addItem(sideTopImageWidget(), 0, 1, Qt::AlignRight | Qt::AlignTop);
            contentLayoutGrid->addItem(sideBottomImageWidget(), 1, 1, Qt::AlignRight | Qt::AlignBottom);

            contentPanel->setLayout(contentLayoutGrid);
            layoutGrid->addItem(contentPanel, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);

            break;
        }
    default:
        break;
    }

    return layoutGrid;
}

void MDetailedListItem::clearLayout()
{
    if (layoutGrid) {
        for (int i = 0; i < layoutGrid->count(); i++) {
            QGraphicsLayoutItem *item = layoutGrid->itemAt(0);
            layoutGrid->removeAt(0);
            delete item;
        }
        image = NULL;
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
        imageWidget()->setObjectName("CommonThumbnail");
    else if (style == Icon)
        imageWidget()->setObjectName("CommonMainIcon");
}

void MDetailedListItem::setImageWidget(MImageWidget *icon)
{
    for (int i = 0; i < layoutGrid->count(); i++) {
        if (layoutGrid->itemAt(i) == image) {
            layoutGrid->removeAt(i);
            delete image;
            image = NULL;
            break;
        }
    }

    if (icon) {
        image = icon;
        if (listItemStyle == MDetailedListItem::IconTitleSubtitleAndTwoSideIcons)
            layoutGrid->addItem(image, 0, 0, 4, 1, Qt::AlignLeft | Qt::AlignVCenter);
    }
}

MImageWidget *MDetailedListItem::imageWidget()
{
    if (!image) {
        image = new MImageWidget(this);
    }
    return image;
}

MImageWidget *MDetailedListItem::sideTopImageWidget()
{
    if (!sideTopImage) {
        sideTopImage = new MImageWidget(this);
        sideTopImage->setObjectName("CommonTopSideIcon");
    }
    return sideTopImage;
}

MImageWidget *MDetailedListItem::sideBottomImageWidget()
{
    if (!sideBottomImage) {
        sideBottomImage = new MImageWidget(this);
        sideBottomImage->setObjectName("CommonBottomSideIcon");
    }
    return sideBottomImage;
}

MLabel *MDetailedListItem::titleLabelWidget()
{
    if (!titleLabel) {
        titleLabel = new MLabel(this);
        titleLabel->setObjectName("CommonTitleWithLeftMargin");
    }

    return titleLabel;
}

void MDetailedListItem::setTitle(const QString &title)
{
    titleLabelWidget()->setText(title);
}

QString MDetailedListItem::title()
{
    return titleLabelWidget()->text();
}

MLabel *MDetailedListItem::subtitleLabelWidget()
{
    if (!subtitleLabel) {
        subtitleLabel = new MLabel(this);
        subtitleLabel->setObjectName("CommonSubTitleWithLeftMargin");
    }

    return subtitleLabel;
}

void MDetailedListItem::setSubtitle(const QString &subtitle)
{
    subtitleLabelWidget()->setText(subtitle);
}

QString MDetailedListItem::subtitle()
{
    return subtitleLabelWidget()->text();
}

MLabel *MDetailedListItem::sideBottomLabelWidget()
{
    if (!sideBottomLabel) {
        sideBottomLabel = new MLabel(this);
        sideBottomLabel->setObjectName("CommonBottomSideSubTitle");
    }

    return sideBottomLabel;
}

void MDetailedListItem::setSideBottomTitle(const QString &text)
{
    sideBottomLabelWidget()->setText(text);
}

QString MDetailedListItem::sideBottomTitle()
{
    return sideBottomLabelWidget()->text();
}

