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

// TODO: Consider calculating size hint from the content

#include "minfobannereventview.h"
#include "minfobannereventview_p.h"

#include "minfobanner.h"
#include "mviewcreator.h"
#include "mimagewidget.h"
#include "mlabel.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

MInfoBannerEventViewPrivate::MInfoBannerEventViewPrivate(MInfoBanner *controller) :
    q_ptr(0), banner(controller), label(0), image(0), icon(0), layout(0),
    controller(controller)
{
}

MInfoBannerEventViewPrivate::~MInfoBannerEventViewPrivate()
{
}

void MInfoBannerEventViewPrivate::init()
{
    label = new MLabel();
    label->setAlignment(Qt::AlignCenter);
    label->setTextElide(true);
    label->setObjectName("EventInfoBannerBody");

    image = new MImageWidget();
    image->setObjectName("EventInfoBannerImage");
    icon = new MImageWidget();
    icon->setObjectName("EventInfoBannerIcon");

    layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    layout->addItem(image);
    layout->addItem(label);
    layout->addItem(icon);
    layout->setAlignment(label, Qt::AlignCenter);

    QObject::connect(banner, SIGNAL(clicked()), controller, SLOT(dismiss()));
}

MInfoBannerEventView::MInfoBannerEventView(MInfoBanner *controller) :
    MSceneWindowView(controller),
    d_ptr(new MInfoBannerEventViewPrivate(controller))
{
    Q_D(MInfoBannerEventView);
    d->q_ptr = this;
    d->init();
}

MInfoBannerEventView::MInfoBannerEventView(MInfoBannerEventViewPrivate &dd, MInfoBanner *controller) :
    MSceneWindowView(controller),
    d_ptr(&dd)
{
    Q_D(MInfoBannerEventView);
    d->q_ptr = this;
    d->init();
}

MInfoBannerEventView::~MInfoBannerEventView()
{
    delete d_ptr;
}

void MInfoBannerEventView::updateData(const QList<const char *>& modifications)
{
    MSceneWindowView::updateData(modifications);

    Q_D(MInfoBannerEventView);
    const char *member;

    foreach(member, modifications) {
        if (member == MInfoBannerModel::BodyText) {
            d->label->setText(model()->bodyText());
        } else if (member == MInfoBannerModel::ImageID) {
            d->image->setImage(model()->imageID(), style()->imageSize());
        } else if (member == MInfoBannerModel::Pixmap) {
            d->image->setPixmap(model()->pixmap());
        } else if (member == MInfoBannerModel::IconID) {
            d->icon->setImage(model()->iconID(), style()->iconSize());
        }
    }
}

void MInfoBannerEventView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MInfoBannerEventView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MInfoBannerEventView);

    QPointF touch = event->scenePos();
    QRectF rect  = d->controller->sceneBoundingRect();
    if (rect.contains(touch)) {
        d->banner->click();
    }
}

void MInfoBannerEventView::setupModel()
{
    MSceneWindowView::setupModel();

    Q_D(MInfoBannerEventView);

    d->label->setText(model()->bodyText());
    d->icon->setImage(model()->iconID(), style()->iconSize());

    if (!model()->imageID().isEmpty()) {
        d->image->setImage(model()->imageID(), style()->imageSize());
    } else {
        d->image->setPixmap(model()->pixmap());
    }
}

void MInfoBannerEventView::applyStyle()
{
    MSceneWindowView::applyStyle();

    Q_D(MInfoBannerEventView);

    if (!model()->imageID().isEmpty()) {
        d->image->setImage(model()->imageID(), style()->imageSize());
    } else {
        d->image->setPixmap(model()->pixmap());
    }

    d->icon->setImage(model()->iconID(), style()->iconSize());
}

M_REGISTER_VIEW_NEW(MInfoBannerEventView, MInfoBanner)
