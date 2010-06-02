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

#include "madvancedlistitem.h"

#include <MImageWidget>
#include <MLabel>
#include <MProgressIndicator>

#include <QGraphicsGridLayout>

MAdvancedListItem::MAdvancedListItem(MAdvancedListItem::ItemStyle itemStyle, QGraphicsItem *parent)
    : MListItem(parent), layoutGrid(NULL), progress(NULL), titleLabel(NULL), image(NULL),
    sideTopImage(NULL), sideBottomImage(NULL), listItemStyle(itemStyle)
{

}

MAdvancedListItem::~MAdvancedListItem()
{
}

void MAdvancedListItem::initLayout()
{
    setLayout(createLayout());
}

QString MAdvancedListItem::title() const
{
    if(titleLabel)
        return titleLabel->text();

    return QString();
}

void MAdvancedListItem::setTitle(const QString &title)
{
    titleLabelWidget()->setText(title);
}

MImageWidget * MAdvancedListItem::imageWidget()
{
    if (!image) {
        image = new MImageWidget(this);
        image->setObjectName("CommonMainIcon");
    }

    return image;
}

void MAdvancedListItem::setImageWidget(MImageWidget * imageWidget)
{
    if (image) {
        for (int i = 0; i < layoutGrid->count(); i++) {
            if (layoutGrid->itemAt(i) == image) {
                layoutGrid->removeAt(i);
                break;
            }
        }
        delete image;
        image = NULL;
    }
    image = imageWidget;
}

MProgressIndicator * MAdvancedListItem::progressIndicator()
{
    if (!progress) {
        progress = new MProgressIndicator(this, MProgressIndicator::barType);
        progress->setObjectName("CommonProgressIndicator");
    }

    return progress;
}

MLabel * MAdvancedListItem::titleLabelWidget()
{
    if (!titleLabel) {
        titleLabel = new MLabel(this);
        titleLabel->setObjectName("CommonTitleWithLeftMargin");
    }

    return titleLabel;
}

MImageWidget * MAdvancedListItem::sideTopImageWidget()
{
    if (!sideTopImage) {
        sideTopImage = new MImageWidget(this);
        sideTopImage->setObjectName("CommonTopSideIcon");
    }

    return sideTopImage;
}

MImageWidget * MAdvancedListItem::sideBottomImageWidget()
{
    if (!sideBottomImage) {
        sideBottomImage = new MImageWidget(this);
        sideBottomImage->setObjectName("CommonBottomSideIcon");
    }

    return sideBottomImage;
}

QGraphicsLayout *MAdvancedListItem::createLayout()
{
    delete layoutGrid;
    layoutGrid = new QGraphicsGridLayout(this);
    layoutGrid->setContentsMargins(0, 0, 0, 0);
    layoutGrid->setSpacing(0);

    switch (listItemStyle) {
    case IconWithTitleProgressIndicatorAndTwoSideIcons: {
            setObjectName("AdvancedListItemIconWithTitleProgressIndicatorAndTwoSideIcons");

            layoutGrid->addItem(imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(progressIndicator(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            layoutGrid->addItem(sideTopImageWidget(), 0, 2, Qt::AlignRight | Qt::AlignTop);
            layoutGrid->addItem(sideBottomImageWidget(), 1, 2, Qt::AlignRight | Qt::AlignBottom);
            break;
        }
    case IconWithTitleProgressIndicatorAndTopSideIcon: {
            setObjectName("AdvancedListItemIconWithTitleProgressIndicatorAndTopSideIcon");

            layoutGrid->addItem(imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layoutGrid->addItem(titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layoutGrid->addItem(progressIndicator(), 1, 1, 1, 2, Qt::AlignLeft | Qt::AlignBottom);
            layoutGrid->addItem(sideTopImageWidget(), 0, 2, Qt::AlignRight | Qt::AlignTop);
            break;
        }
    default:
        break;
    }

    return layoutGrid;
}


