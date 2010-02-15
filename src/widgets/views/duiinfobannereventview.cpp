/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

// TODO: Consider calculating size hint from the content

#include "duiinfobannereventview.h"
#include "duiinfobannereventview_p.h"

#include "duiinfobanner.h"
#include "duiviewcreator.h"
#include "duiimagewidget.h"
#include "duilabel.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

DuiInfoBannerEventViewPrivate::DuiInfoBannerEventViewPrivate(DuiInfoBanner *controller) :
    DuiSceneWindowViewPrivate(),
    banner(controller), label(0), image(0), icon(0), layout(0)
{
}

DuiInfoBannerEventViewPrivate::~DuiInfoBannerEventViewPrivate()
{
}

void DuiInfoBannerEventViewPrivate::init()
{
    label = new DuiLabel();
    label->setAlignment(Qt::AlignCenter);
    label->setTextElide(true);
    label->setObjectName("EventInfoBannerBody");

    image = new DuiImageWidget();
    image->setObjectName("EventInfoBannerImage");
    icon = new DuiImageWidget();
    icon->setObjectName("EventInfoBannerIcon");

    layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    layout->addItem(image);
    layout->addItem(label);
    layout->addItem(icon);
    layout->setAlignment(label, Qt::AlignCenter);
}

DuiInfoBannerEventView::DuiInfoBannerEventView(DuiInfoBanner *controller) :
    DuiSceneWindowView(* new DuiInfoBannerEventViewPrivate(controller), controller)
{
    Q_D(DuiInfoBannerEventView);
    d->init();
}

DuiInfoBannerEventView::DuiInfoBannerEventView(DuiInfoBannerEventViewPrivate &dd, DuiInfoBanner *controller) :
    DuiSceneWindowView(dd, controller)
{
    Q_D(DuiInfoBannerEventView);
    d->init();
}

DuiInfoBannerEventView::~DuiInfoBannerEventView()
{
}

void DuiInfoBannerEventView::updateData(const QList<const char *>& modifications)
{
    DuiSceneWindowView::updateData(modifications);

    Q_D(DuiInfoBannerEventView);
    const char *member;

    foreach(member, modifications) {
        if (member == DuiInfoBannerModel::BodyText) {
            d->label->setText(model()->bodyText());
        } else if (member == DuiInfoBannerModel::ImageID) {
            d->image->setImage(model()->imageID(), style()->imageSize());
        } else if (member == DuiInfoBannerModel::Pixmap) {
            d->image->setPixmap(model()->pixmap());
        } else if (member == DuiInfoBannerModel::IconID) {
            d->icon->setImage(model()->iconID(), style()->iconSize());
        }
    }
}

void DuiInfoBannerEventView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void DuiInfoBannerEventView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiInfoBannerEventView);

    QPointF touch = event->scenePos();
    QRectF rect  = d->controller->sceneBoundingRect();
    if (rect.contains(touch)) {
        d->banner->click();
    }
}

void DuiInfoBannerEventView::setupModel()
{
    DuiSceneWindowView::setupModel();

    Q_D(DuiInfoBannerEventView);

    d->label->setText(model()->bodyText());
    d->icon->setImage(model()->iconID(), style()->iconSize());

    if (!model()->imageID().isEmpty()) {
        d->image->setImage(model()->imageID(), style()->imageSize());
    } else {
        d->image->setPixmap(model()->pixmap());
    }
}

void DuiInfoBannerEventView::applyStyle()
{
    DuiSceneWindowView::applyStyle();

    Q_D(DuiInfoBannerEventView);

    if (!model()->imageID().isEmpty()) {
        d->image->setImage(model()->imageID(), style()->imageSize());
    } else {
        d->image->setPixmap(model()->pixmap());
    }

    d->icon->setImage(model()->iconID(), style()->iconSize());
}

DUI_REGISTER_VIEW_NEW(DuiInfoBannerEventView, DuiInfoBanner)
