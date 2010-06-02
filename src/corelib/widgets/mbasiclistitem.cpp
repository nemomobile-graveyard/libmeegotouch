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
    image(NULL),
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
    setLayout(createLayout());
}

QGraphicsLayout *MBasicListItem::createLayout()
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

            layoutGrid->addItem(titleLabelWidget(), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
            break;
        }
    case MBasicListItem::TitleWithSubtitle: {
            setObjectName("BasicListItemTitleWithSubtitle");

            layoutGrid->addItem(titleLabelWidget(), 0, 0, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabelWidget(), 1, 0, Qt::AlignLeft | Qt::AlignBottom);
            break;
        }
    case MBasicListItem::IconWithTitle: {
            setObjectName("BasicListItemIconWithTitle");

            layoutGrid->addItem(imageWidget(), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
            break;
        }
    case MBasicListItem::IconWithTitleAndSubtitle: {
            setObjectName("BasicListItemIconWithTitleAndSubtitle");

            layoutGrid->addItem(imageWidget(), 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(subtitleLabelWidget(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            
            break;
        }
    default:
        break;
    }

    return layoutGrid;
}

void MBasicListItem::clearLayout()
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

void MBasicListItem::setImageWidget(MImageWidget *imageWidget)
{
    for (int i = 0; i < layoutGrid->count(); i++) {
        if (layoutGrid->itemAt(i) == image) {
            layoutGrid->removeAt(i);
            delete image;
            image = NULL;
            break;
        }
    }

    if (imageWidget) {
        image = imageWidget;
        if (listItemStyle == MBasicListItem::IconWithTitle)
            layoutGrid->addItem(image, 0, 0);
        else if (listItemStyle == MBasicListItem::IconWithTitleAndSubtitle)
            layoutGrid->addItem(image, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
    }
}

MImageWidget *MBasicListItem::imageWidget()
{
    if (!image) {
        image = new MImageWidget(this);
        image->setObjectName("CommonMainIcon");
    }
    return image;
}

MLabel *MBasicListItem::titleLabelWidget()
{
    if (!titleLabel) {
        titleLabel = new MLabel(this);
        if (listItemStyle == SingleTitle)
            titleLabel->setObjectName("CommonSingleTitle");
        else if (listItemStyle == TitleWithSubtitle)
            titleLabel->setObjectName("CommonTitle");
        else
            titleLabel->setObjectName("CommonTitleWithLeftMargin");
    }

    return titleLabel;
}

MLabel *MBasicListItem::subtitleLabelWidget()
{
    if (!subtitleLabel) {
        subtitleLabel = new MLabel(this);
        if (listItemStyle == TitleWithSubtitle)
            subtitleLabel->setObjectName("CommonSubTitle");
        else
            subtitleLabel->setObjectName("CommonSubTitleWithLeftMargin");
    }

    return subtitleLabel;
}

void MBasicListItem::setTitle(const QString &title)
{
    titleLabelWidget()->setText(title);
}

QString MBasicListItem::title()
{
    return titleLabelWidget()->text();
}

void MBasicListItem::setSubtitle(const QString &subtitle)
{
    subtitleLabelWidget()->setText(subtitle);
}

QString MBasicListItem::subtitle()
{
    return subtitleLabelWidget()->text();
}

