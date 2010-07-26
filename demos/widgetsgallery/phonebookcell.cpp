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
    landscapePolicy(NULL),
    portraitPolicy(NULL),
    spinner(NULL),
    landscapeTitleLabel(NULL),
    portraitTitleLabel(NULL),
    subtitleLabel(NULL),
    icon(NULL)
{
}

PhoneBookCell::~PhoneBookCell()
{
    if (spinner)
        delete spinner;

    if (icon)
        delete icon;
}

MLayout *PhoneBookCell::createLayout()
{
    setObjectName("BasicListItemIconWithTitleAndSubtitle");
    layout = new MLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    landscapePolicy = new MGridLayoutPolicy(layout);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);

    // add to layout
    if (!imageWidget()->pixmap())
        landscapePolicy->addItem(spinnerWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
    else
        landscapePolicy->addItem(imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);


    landscapePolicy->addItem(landscapeTitleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
    landscapePolicy->addItem(subtitleLabelWidget(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
    landscapePolicy->addItem(new QGraphicsWidget(this), 2, 1);

    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);

    if (!imageWidget()->pixmap())
        portraitPolicy->addItem(spinnerWidget(), Qt::AlignLeft | Qt::AlignVCenter);
    else
        portraitPolicy->addItem(imageWidget(), Qt::AlignLeft | Qt::AlignVCenter);

    portraitPolicy->addItem(portraitTitleLabelWidget(), Qt::AlignLeft | Qt::AlignVCenter);

    layout->setPortraitPolicy(portraitPolicy);
    layout->setLandscapePolicy(landscapePolicy);

    return layout;
}

MProgressIndicator *PhoneBookCell::spinnerWidget()
{
    if (!spinner) {
        // spinner
        spinner = new MProgressIndicator(NULL, MProgressIndicator::spinnerType);
        spinner->setUnknownDuration(true);
        spinner->setObjectName("CommonSpinner");
    }
    return spinner;
}

MLabel *PhoneBookCell::landscapeTitleLabelWidget()
{
    if (!landscapeTitleLabel) {
        landscapeTitleLabel = new MLabel(this);
        landscapeTitleLabel->setTextElide(true);
        landscapeTitleLabel->setObjectName("CommonTitle");
    }
    return landscapeTitleLabel;
}

MLabel *PhoneBookCell::portraitTitleLabelWidget()
{
    if (!portraitTitleLabel) {
        // In portrait we have only one title, also object name should be different
        portraitTitleLabel = new MLabel(this);
        portraitTitleLabel->setTextElide(true);
        portraitTitleLabel->setObjectName("CommonSingleTitle");
    }
    return portraitTitleLabel;
}

MLabel *PhoneBookCell::subtitleLabelWidget()
{
    if (!subtitleLabel) {
        subtitleLabel = new MLabel(this);
        subtitleLabel->setTextElide(true);
        subtitleLabel->setObjectName("CommonSubTitle");
    }
    return subtitleLabel;
}

MImageWidget *PhoneBookCell::imageWidget()
{
    if (!icon) {
        // icon
        icon = new MImageWidget();
        icon->setObjectName("CommonMainIcon");
        icon->setVisible(false);
    }
    return icon;
}


void PhoneBookCell::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MListItem::resizeEvent(event);

    if (!layout)
        setLayout(createLayout());
}

QString PhoneBookCell::title()
{
    return landscapeTitleLabelWidget()->text();
}

void PhoneBookCell::setTitle(const QString &title)
{
    landscapeTitleLabelWidget()->setText(title);
    portraitTitleLabelWidget()->setText(title);
}

QString PhoneBookCell::subtitle()
{
    return subtitleLabelWidget()->text();
}

void PhoneBookCell::setSubtitle(const QString &subtitle)
{
    subtitleLabelWidget()->setText(subtitle);
}

QImage PhoneBookCell::image()
{
    return imageWidget()->pixmap()->toImage();
}

void PhoneBookCell::setImage(const QImage &iconImage)
{
    if (layout && (iconImage.isNull() || imageWidget()->image().isEmpty())) {
        if (layout->policy() == landscapePolicy) {
            if (landscapePolicy->itemAt(0, 0) == spinnerWidget() && !iconImage.isNull()) {
                landscapePolicy->removeItem(spinnerWidget());
                landscapePolicy->addItem(imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            } else if (landscapePolicy->itemAt(0, 0) == imageWidget() && iconImage.isNull()) {
                landscapePolicy->removeItem(imageWidget());
                landscapePolicy->addItem(spinnerWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            }
        } else if (layout->policy() == portraitPolicy) {
            if (portraitPolicy->itemAt(0) == imageWidget() && iconImage.isNull()) {
                portraitPolicy->removeAt(0);
                portraitPolicy->insertItem(0, spinnerWidget(), Qt::AlignLeft | Qt::AlignVCenter);
            } else if (portraitPolicy->itemAt(0) == spinnerWidget() && !iconImage.isNull()) {
                portraitPolicy->removeAt(0);
                portraitPolicy->insertItem(0, imageWidget(), Qt::AlignLeft | Qt::AlignVCenter);
            }
        }
    }

    if (!iconImage.isNull()) {
        imageWidget()->setImage(iconImage);
        imageWidget()->setVisible(true);
    }
}

