/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <MApplication>
#include <MLabel>
#include <MLayout>

#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MImageWidget>
#include <MProgressIndicator>
#include <MSceneManager>

PhoneBookCell::PhoneBookCell(QGraphicsItem *parent)
    : MListItem(parent),
    layout(NULL),
    landscapePolicy(NULL),
    portraitPolicy(NULL),
    landscapeTitleLabel(NULL),
    portraitTitleLabel(NULL),
    subtitleLabel(NULL),
    icon(NULL)
{
    setStyleName(inv("CommonPanel"));
}

PhoneBookCell::~PhoneBookCell()
{
    if (icon)
        delete icon;
}

MLayout *PhoneBookCell::createLayout()
{
    layout = new MLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    landscapePolicy = new MGridLayoutPolicy(layout);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);

    // add to layout
    landscapePolicy->addItem(imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
    landscapePolicy->addItem(landscapeTitleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
    landscapePolicy->addItem(subtitleLabelWidget(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
    landscapePolicy->addItem(new QGraphicsWidget(this), 2, 1);

    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);

    portraitPolicy->addItem(imageWidget(), Qt::AlignLeft | Qt::AlignVCenter);
    portraitPolicy->addItem(portraitTitleLabelWidget(), Qt::AlignLeft | Qt::AlignVCenter);

    layout->setPortraitPolicy(portraitPolicy);
    layout->setLandscapePolicy(landscapePolicy);

    return layout;
}

MLabel *PhoneBookCell::landscapeTitleLabelWidget()
{
    if (!landscapeTitleLabel) {
        landscapeTitleLabel = new MLabel(this);
        landscapeTitleLabel->setObjectName("phoneBookListItemLandscapeTitleLabel");
        landscapeTitleLabel->setTextElide(true);
        landscapeTitleLabel->setStyleName(inv("CommonTitle"));
    }
    return landscapeTitleLabel;
}

MLabel *PhoneBookCell::portraitTitleLabelWidget()
{
    if (!portraitTitleLabel) {
        // In portrait we have only one title, also object name should be different
        portraitTitleLabel = new MLabel(this);
        portraitTitleLabel->setObjectName("phoneBookListItemPortraitTitleLabel");
        portraitTitleLabel->setTextElide(true);
        portraitTitleLabel->setStyleName(inv("CommonSingleTitle"));
    }
    return portraitTitleLabel;
}

MLabel *PhoneBookCell::subtitleLabelWidget()
{
    if (!subtitleLabel) {
        subtitleLabel = new MLabel(this);
        subtitleLabel->setObjectName("phoneBookListItemSubtitleLabel");
        subtitleLabel->setTextElide(true);
        subtitleLabel->setStyleName(inv("CommonSubTitle"));
    }
    return subtitleLabel;
}

MImageWidget *PhoneBookCell::imageWidget()
{
    if (!icon) {
        // icon
        icon = new MImageWidget(this);
        icon->setObjectName("phoneBookListItemIcon");
        icon->setStyleName("CommonMainIcon");
        icon->setImage("icon-m-content-avatar-placeholder");
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
    if (!iconImage.isNull())
        imageWidget()->setImage(iconImage);
    else
        imageWidget()->setImage("icon-m-content-avatar-placeholder");
}

QString PhoneBookCell::inv(QString stylename)
{
    if (MApplication::instance()->objectName() == "widgetsgallery") {
        return stylename;
    } else {
        return stylename.append("Inverted");
    }
}

