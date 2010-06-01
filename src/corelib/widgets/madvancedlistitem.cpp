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

#include 
#include 

#include 
#include 
#include 
#include 

#include "madvancedlistitem.h"

MAdvancedListItem::MAdvancedListItem(QGraphicsItem *parent)
    : MListItem(parent), layout(NULL), progress(NULL), titleLabel(NULL), image(NULL)
    , rightTopImage(NULL), rightBottomImage(NULL)
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
    if(!image)
    {
        image = new MImageWidget(this);
        image->setObjectName("CommonMainIcon");
    }

    return image;
}

void MAdvancedListItem::setImageWidget(MImageWidget * imageWidget)
{
    delete image;
    imageWidget->setParentItem(this);
    image = imageWidget;
}

MProgressIndicator * MAdvancedListItem::progressIndicator()
{
    if(!progress) {
        progress = new MProgressIndicator(this, MProgressIndicator::barType);
        progress->setObjectName("CommonProgressBar");
    }

    return progress;
}

MLabel * MAdvancedListItem::titleLabelWidget()
{
    if(!titleLabel) {
        titleLabel = new MLabel(this);
        titleLabel->setObjectName("CommonTitle");
    }

    return titleLabel;
}

MImageWidget * MAdvancedListItem::rightTopImageWidget()
{
    if(!rightTopImage) {
        rightTopImage = new MImageWidget(this);
        rightTopImage->setObjectName("CommonSubItem");
    }

    return rightTopImage;
}

MImageWidget * MAdvancedListItem::rightBottomImageWidget()
{
    if(!rightBottomImage) {
        rightBottomImage = new MImageWidget(this);
        rightBottomImage->setObjectName("CommonSubItem");
    }

    return rightBottomImage;
}

QGraphicsLayout *MAdvancedListItem::createLayout()
{
    delete layout;
    layout = new QGraphicsGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(imageWidget(), 0, 0, 3, 1);
    layout->addItem(titleLabelWidget(), 0, 1);
    layout->addItem(progressIndicator(), 1, 1);
    layout->addItem(new QGraphicsWidget(this), 2, 1);
    layout->addItem(rightTopImageWidget(), 0, 2);
    layout->addItem(rightBottomImageWidget(), 0, 2);
    return layout;
}


