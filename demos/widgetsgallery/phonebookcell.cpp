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
 

#include "phonebookcell.h"

#include <MLabel>
#include <MLayout>

#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MImageWidget>
#include <MProgressIndicator>

PhoneBookCell::PhoneBookCell()
    : MListItem(),
    layout(NULL),
    titleLabel(NULL),
    subtitleLabel(NULL),
    imageWidget(NULL)
{
}

PhoneBookCell::~PhoneBookCell()
{   
}

void PhoneBookCell::initLayout()
{
    setLayout(createLayout());
}

MLayout *PhoneBookCell::createLayout()
{
    layout = new MLayout(this);
    landscapePolicy = new MGridLayoutPolicy(layout);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);
    
    // title
    titleLabel = new MLabel(this);
    titleLabel->setTextElide(true);
    titleLabel->setObjectName("CommonTitle");

    // subtitle
    subtitleLabel = new MLabel(this);
    subtitleLabel->setTextElide(true);
    subtitleLabel->setObjectName("CommonSubTitle");    
    
    // icon
    imageWidget = new MImageWidget(this);
    imageWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    imageWidget->setObjectName("CommonMainIcon");
    imageWidget->setVisible(false);

    // spinner
    spinner = new MProgressIndicator(this, MProgressIndicator::spinnerType);
    spinner->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    spinner->setUnknownDuration(true);
    spinner->setObjectName("CommonMainIcon");

    // add to layout
    landscapePolicy->addItem(spinner, 0, 0, 4, 1);
    landscapePolicy->addItem(titleLabel, 1, 1, Qt::AlignTop);
    landscapePolicy->addItem(subtitleLabel, 2, 1);
    landscapePolicy->addItem(new QGraphicsWidget(), 3, 1);
    
    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);
    portraitPolicy->addItem(spinner);
    portraitPolicy->addItem(titleLabel);
    
    layout->setPortraitPolicy(portraitPolicy);
    layout->setLandscapePolicy(landscapePolicy);

    return layout;
}

QString PhoneBookCell::title() const
{
    return titleLabel->text();
}

void PhoneBookCell::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

QString PhoneBookCell::subtitle() const
{
    return subtitleLabel->text();
}

void PhoneBookCell::setSubtitle(const QString &subtitle)
{
    subtitleLabel->setText(subtitle);
}

QImage PhoneBookCell::image() const
{
    return imageWidget->pixmap()->toImage();
}

void PhoneBookCell::setImage(const QImage &image)
{
    imageWidget->setImage(image);
    imageWidget->setVisible(true);

    if (layout->policy() == landscapePolicy) {
        if (landscapePolicy->itemAt(0, 0) == spinner && !image.isNull()) {
            landscapePolicy->removeItem(spinner);
            landscapePolicy->addItem(imageWidget, 0, 0, 4, 1);
        } else if (landscapePolicy->itemAt(0, 0) == imageWidget && image.isNull()) {
            landscapePolicy->removeItem(imageWidget);
            landscapePolicy->addItem(spinner, 0, 0, 4, 1);
        }
    } else if (layout->policy() == portraitPolicy) {
        if (portraitPolicy->itemAt(0) == imageWidget && image.isNull()) {
            portraitPolicy->removeAt(0);
            portraitPolicy->insertItem(0, spinner, Qt::AlignLeft);
        } else if (portraitPolicy->itemAt(0) == spinner && !image.isNull()) {
            portraitPolicy->removeAt(0);
            portraitPolicy->insertItem(0, imageWidget, Qt::AlignLeft);
        }
    }
}
