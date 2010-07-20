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

#include "mbanner.h"
#include "mbannerview.h"
#include "mbannerview_p.h"

#include "mviewcreator.h"
#include "mimagewidget.h"
#include "mlabel.h"

#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mgridlayoutpolicy.h"

#include <QGraphicsSceneMouseEvent>

MBannerViewPrivate::MBannerViewPrivate() :
    layout(0),
    landscapePolicy(0),
    portraitPolicy(0),
    icon(0),
    title(0),
    subtitle(0)
{
}

MBannerViewPrivate::~MBannerViewPrivate()
{
}

void MBannerViewPrivate::initDynamicLayout()
{
    layout = new MLayout();

    landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    landscapePolicy->setSpacing(0);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);

    portraitPolicy = new MGridLayoutPolicy(layout);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);

    Q_Q(MBannerView);

    if (!q->model()->iconID().isEmpty() && !q->model()->title().isEmpty()
        && !q->model()->subtitle().isEmpty()) {

        icon = new MImageWidget();
        icon->setObjectName("CommonBannerMainIcon");
        landscapePolicy->addItem(icon,Qt::AlignVCenter);
        portraitPolicy->addItem(icon,0,0,2,1, Qt::AlignVCenter );
        
        title = new MLabel();
        title->setObjectName("CommonBannerSingleTitle");
        title->setAlignment(Qt::AlignVCenter);
        title->setWordWrap(true);
        landscapePolicy->addItem(title);
        portraitPolicy->addItem(title, 0,1,Qt::AlignLeft);
        
        subtitle = new MLabel();
        subtitle->setObjectName("CommonBannerSingleSubTitle");
        subtitle->setAlignment(Qt::AlignVCenter);
        subtitle->setWordWrap(true);
        landscapePolicy->addItem(subtitle);
        portraitPolicy->addItem(subtitle,1,1,Qt::AlignVCenter);
        
    } else if (q->model()->iconID().isEmpty() && !q->model()->subtitle().isEmpty()) {

        subtitle = new MLabel();
        subtitle->setObjectName("CommonBannerSingleSubTitle");
        subtitle->setAlignment(Qt::AlignVCenter);
        subtitle->setWordWrap(true);
        landscapePolicy->addItem(subtitle);
        portraitPolicy->addItem(subtitle,0,0,Qt::AlignVCenter);

    } else if (!q->model()->iconID().isEmpty() && !q->model()->subtitle().isEmpty()) {

        icon = new MImageWidget();
        icon->setObjectName("CommonBannerMainIcon");
        landscapePolicy->addItem(icon,Qt::AlignVCenter);
        portraitPolicy->addItem(icon,0,0,2,1, Qt::AlignVCenter );

        subtitle = new MLabel();
        subtitle->setObjectName("CommonBannerSingleSubTitle");
        subtitle->setAlignment(Qt::AlignVCenter);
        subtitle->setWordWrap(true);
        landscapePolicy->addItem(subtitle);
        portraitPolicy->addItem(subtitle,1,1,Qt::AlignVCenter);

    } else {
        /*These conditionals are necessary for cases not contemplated
        in the common layouts */

        if (!q->model()->iconID().isEmpty()) {
            icon = new MImageWidget();
            icon->setObjectName("CommonBannerMainIcon");
            landscapePolicy->addItem(icon, Qt::AlignVCenter);
            portraitPolicy->addItem(icon, 0, 0, 2, 1, Qt::AlignVCenter);
        }

        if (!q->model()->title().isEmpty()) {
            title = new MLabel();
            title->setObjectName("CommonBannerSingleTitle");
            title->setAlignment(Qt::AlignVCenter);
            title->setWordWrap(true);
            landscapePolicy->addItem(title);
            portraitPolicy->addItem(title, 0, 1, Qt::AlignLeft);
        }

        if (!q->model()->subtitle().isEmpty()) {
            subtitle = new MLabel();
            subtitle->setObjectName("CommonBannerSingleSubTitle");
            subtitle->setAlignment(Qt::AlignVCenter);
            subtitle->setWordWrap(true);
            landscapePolicy->addItem(subtitle);
            portraitPolicy->addItem(subtitle, 0, 0, Qt::AlignVCenter);
        }
    }

    landscapePolicy->addStretch();

    layout->setPortraitPolicy(portraitPolicy);
    layout->setLandscapePolicy(landscapePolicy);

    controller->setLayout(layout);

    QObject::connect(controller, SIGNAL(clicked()), controller, SLOT(dismiss()));
}

MBannerView::MBannerView(MBanner *controller) :
    MSceneWindowView(controller),
    d_ptr(new MBannerViewPrivate)
{
    Q_D(MBannerView);
    d->q_ptr = this;
    d->controller = controller;
}

MBannerView::MBannerView(MBannerViewPrivate &dd, MBanner *controller) :
    MSceneWindowView(controller),
    d_ptr(&dd)
{
    Q_D(MBannerView);
    d->q_ptr = this;
    d->controller = controller;
}

MBannerView::~MBannerView()
{
    delete d_ptr;
}

void MBannerView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MBannerView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MBannerView);

     QPointF touch = event->scenePos();
     QRectF rect  = d->controller->sceneBoundingRect();

     if (rect.contains(touch)) {
         d->controller->click();
     }
}

void MBannerView::setupModel()
{
    MSceneWindowView::setupModel();

    Q_D(MBannerView);

    d->initDynamicLayout();

    if (!model()->title().isEmpty())
        d->title->setText(model()->title());
    if (!model()->subtitle().isEmpty())
        d->subtitle->setText(model()->subtitle());
    if (!model()->iconID().isEmpty())
        d->icon->setImage(model()->iconID(), style()->iconSize());
}

void MBannerView::applyStyle()
{
    MSceneWindowView::applyStyle();
}

void MBannerView::updateData(const QList<const char *>& modifications)
{
    MSceneWindowView::updateData(modifications);

    Q_D(MBannerView);

    const char *member;

    foreach(member, modifications) {
        if (member == MBannerModel::Title) {
            d->title->setText(model()->title());
        } else if (member == MBannerModel::Subtitle) {
            d->subtitle->setText(model()->subtitle());
        } else if (member == MBannerModel::IconID) {
            d->icon->setImage(model()->iconID(), style()->iconSize());
        }
    }
}

M_REGISTER_VIEW_NEW(MBannerView, MBanner)
