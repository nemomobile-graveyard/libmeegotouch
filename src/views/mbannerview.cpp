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
    iconId(NULL),
    titleLabel(NULL),
    subtitleLabel(NULL)
{
}

MBannerViewPrivate::~MBannerViewPrivate()
{
}


MLabel *MBannerViewPrivate::title()
{
    if (!titleLabel) {
        titleLabel = new MLabel(controller);
        titleLabel->setObjectName("MBannerTitle");
        titleLabel->setAlignment(Qt::AlignVCenter);
        titleLabel->setWordWrap(true);
    }
    return titleLabel;
}

MLabel *MBannerViewPrivate::subtitle()
{
    if (!subtitleLabel) {
        subtitleLabel = new MLabel(controller);
        subtitleLabel->setObjectName("MBannerSubtitle");
        subtitleLabel->setAlignment(Qt::AlignVCenter);
        subtitleLabel->setWordWrap(true);
    }
    return subtitleLabel;
}

MImageWidget *MBannerViewPrivate::icon()
{
    if (!iconId) {
        iconId = new MImageWidget(controller);
        iconId->setObjectName("MBannerIcon");
    }
    return iconId;
}

void MBannerViewPrivate::setTitle(const QString &string)
{
    title()->setText(string);
}

void MBannerViewPrivate::setSubtitle(const QString &string)
{
    subtitle()->setText(string);
}

void MBannerViewPrivate::setIcon(const QString &i, const QSize &s)
{
    icon()->setImage(i,s);
}

void MBannerViewPrivate::initDynamicLayout()
{
    layout = new MLayout();

    landscapePolicy = new MGridLayoutPolicy(layout);
    landscapePolicy->setSpacing(0);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);

    portraitPolicy = new MGridLayoutPolicy(layout);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);

    Q_Q(MBannerView);

    if (!q->model()->iconID().isEmpty()&& !q->model()->title().isEmpty()
        && !q->model()->subtitle().isEmpty()) {

        landscapePolicy->addItem(icon(),0,0,2,1, Qt::AlignVCenter);
        portraitPolicy->addItem(icon(),0,0,2,1, Qt::AlignVCenter);

        landscapePolicy->addItem(title(), 0,1,Qt::AlignLeft);
        portraitPolicy->addItem(title(), 0,1,Qt::AlignLeft);
        
        landscapePolicy->addItem(subtitle(),1,1,Qt::AlignLeft);
        portraitPolicy->addItem(subtitle(),1,1,Qt::AlignLeft);
        
    } else if (q->model()->iconID().isEmpty() && !q->model()->subtitle().isEmpty()) {
        subtitle()->setObjectName("MBannerSubtitleSingle");
        landscapePolicy->addItem(subtitle(),0,0,Qt::AlignVCenter);
        portraitPolicy->addItem(subtitle(),0,0,Qt::AlignVCenter);

    } else if (!q->model()->iconID().isEmpty() && !q->model()->subtitle().isEmpty()) {
        subtitle()->setObjectName("MBannerSubtitleSingle");
        subtitle()->setWordWrap(false);
        landscapePolicy->addItem(icon(),0,0,2,1, Qt::AlignVCenter);
        portraitPolicy->addItem(icon(),0,0,2,1, Qt::AlignVCenter);

        landscapePolicy->addItem(subtitle(),1,1,Qt::AlignVCenter);
        portraitPolicy->addItem(subtitle(),1,1,Qt::AlignVCenter);
    } else {
        /*These conditionals are necessary for cases not contemplated
        in the common layouts */

        if (!q->model()->iconID().isEmpty()) {
            landscapePolicy->addItem(icon(), 0, 0, 2, 1, Qt::AlignVCenter);
            portraitPolicy->addItem(icon(), 0, 0, 2, 1, Qt::AlignVCenter);
        }

        if (!q->model()->title().isEmpty()) {
            landscapePolicy->addItem(title(), 0, 1, Qt::AlignLeft);
            portraitPolicy->addItem(title(), 0, 1, Qt::AlignLeft);
        }

        if (!q->model()->subtitle().isEmpty()) {
            landscapePolicy->addItem(subtitle(), 0, 0, Qt::AlignVCenter);
            portraitPolicy->addItem(subtitle(), 0, 0, Qt::AlignVCenter);
        }
    }

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


    if (!model()->title().isEmpty())
        d->setTitle(model()->title());
    if (!model()->subtitle().isEmpty())
        d->setSubtitle(model()->subtitle());
    if (!model()->iconID().isEmpty())
        d->setIcon(model()->iconID(), style()->iconSize());

    d->initDynamicLayout();
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
            d->setTitle(model()->title());
        } else if (member == MBannerModel::Subtitle) {
            d->setSubtitle(model()->subtitle());
        } else if (member == MBannerModel::IconID) {
            d->setIcon(model()->iconID(), style()->iconSize());
        }
    }
    d->initDynamicLayout();
}

M_REGISTER_VIEW_NEW(MBannerView, MBanner)
