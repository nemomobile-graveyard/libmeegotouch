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
 
#include "mbasiclistitem.h"

#include <MLabel>
#include <MImageWidget>

#include <MWidgetStyle>

#include <QGraphicsGridLayout>

MBasicListItem::MBasicListItem(MBasicListItem::ItemStyle style, QGraphicsItem *parent)
    : MListItem(parent),
    layoutGrid(NULL),
    iconImage(NULL),
    titleLabel(NULL),
    subtitleLabel(NULL),
    listItemStyle(style)
{
    setItemStyle(style);
}

MBasicListItem::~MBasicListItem()
{
}

void MBasicListItem::initLayout()
{
    clearLayout();
    
    if (!layoutGrid) {
        layoutGrid = new QGraphicsGridLayout(this);
        layoutGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        layoutGrid->setContentsMargins(0, 0, 0, 0);
        layoutGrid->setSpacing(0);
    }
    
    switch (listItemStyle) {
    case MBasicListItem::SingleTitle: {
            setObjectName("BasicListItemSingleTitle");

            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonSingleTitle");
            
            layoutGrid->addItem(titleLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
            break;
        }
    case MBasicListItem::TitleWithSubtitle: {
            setObjectName("BasicListItemTitleWithSubtitle");

            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonTitle");
            
            subtitleLabel = new MLabel(this);
            subtitleLabel->setObjectName("CommonSubTitle");
            
            layoutGrid->addItem(titleLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabel, 1, 0, Qt::AlignLeft | Qt::AlignBottom);
            break;
        }
    case MBasicListItem::IconWithTitle: {
            setObjectName("BasicListItemIconWithTitle");

            iconImage = new MImageWidget(this);
            iconImage->setObjectName("CommonMainIcon");
            
            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonTitleWithLeftMargin");

            layoutGrid->addItem(iconImage, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabel, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
            break;
        }
    case MBasicListItem::IconWithTitleAndSubtitle: {
            setObjectName("BasicListItemIconWithTitleAndSubtitle");

            iconImage = new MImageWidget(this);
            iconImage->setObjectName("CommonMainIcon");
            
            titleLabel = new MLabel(this);
            titleLabel->setObjectName("CommonTitleWithLeftMargin");
            
            subtitleLabel = new MLabel(this);
            subtitleLabel->setObjectName("CommonSubTitleWithLeftMargin");

            layoutGrid->addItem(iconImage, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabel, 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabel, 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            
            break;
        }
    default:
        break;
    }
}

void MBasicListItem::clearLayout()
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
    }
}

void MBasicListItem::setItemStyle(ItemStyle itemStyle)
{
    if (itemStyle == listItemStyle)
        return;
    
    initLayout();
}

MBasicListItem::ItemStyle MBasicListItem::itemStyle() const
{
    return listItemStyle;
}

void MBasicListItem::setIcon(MImageWidget *icon)
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
        if (listItemStyle == MBasicListItem::IconWithTitle)
            layoutGrid->addItem(iconImage, 0, 0);
        else if (listItemStyle == MBasicListItem::IconWithTitleAndSubtitle)
            layoutGrid->addItem(iconImage, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
    }
}

MImageWidget *MBasicListItem::icon() const
{
    return iconImage;
}

void MBasicListItem::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

QString MBasicListItem::title() const
{
    return titleLabel->text();
}

void MBasicListItem::setSubtitle(const QString &subtitle)
{
    subtitleLabel->setText(subtitle);    
}

QString MBasicListItem::subtitle() const
{
    return subtitleLabel->text();
}

