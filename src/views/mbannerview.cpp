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
#include <QGraphicsGridLayout>
#include <QColor>
#include <QPalette>
#include <QDebug>

MBannerViewPrivate::MBannerViewPrivate() :
    layout(0),
    gridBanner(0),
    iconId(NULL),
    titleLabel(NULL),
    subtitleLabel(NULL),
    bannerTimeStampLabel(NULL),
    prefixTimeStampLabel(NULL),
    bannerTimeStampData(NULL)
{
}

MBannerViewPrivate::~MBannerViewPrivate()
{
}

MLabel *MBannerViewPrivate::title()
{
    if (!titleLabel) {
        titleLabel = new MLabel(controller);
        //Banners can't have html
        titleLabel->setTextFormat(Qt::PlainText);
    }
    return titleLabel;
}

MLabel *MBannerViewPrivate::subtitle()
{
    if (!subtitleLabel) {
        subtitleLabel = new MLabel(controller);
        //Banners can't have html
        subtitleLabel->setTextFormat(Qt::PlainText);
        subtitleLabel->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        subtitleLabel->setWordWrap(true);
        subtitleLabel->setTextElide(true);
    }
    return subtitleLabel;
}

MImageWidget *MBannerViewPrivate::icon()
{
    if (!iconId) {
        iconId = new MImageWidget(controller);
        iconId->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    return iconId;
}

MLabel *MBannerViewPrivate::bannerTimeStamp()
{
    if (!bannerTimeStampLabel) {
        bannerTimeStampLabel = new MLabel(controller);
        bannerTimeStampLabel->setTextFormat(Qt::PlainText);
    }

    if (bannerTimeStampData){
        bannerTimeStampLabel->setText(bannerTimeStampData->toString("hh:mm ap"));
    }

    return bannerTimeStampLabel;
}

MLabel *MBannerViewPrivate::prefixTimeStamp()
{
    if (!prefixTimeStampLabel) {
        prefixTimeStampLabel = new MLabel(controller);
        //Banners can't have html
        prefixTimeStampLabel->setTextFormat(Qt::PlainText);
        prefixTimeStampLabel->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        prefixTimeStampLabel->setTextElide(true);
    }
    return prefixTimeStampLabel;
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

void MBannerViewPrivate::setBannerTimeStamp(const QDateTime &date)
{
    bannerTimeStampData = new QDateTime(date);
}

void MBannerViewPrivate::setPrefixTimeStamp(const QString &string)
{
    prefixTimeStamp()->setText(string);
}

MLayout *MBannerViewPrivate::createLayout()
{
    layout = new MLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    landscapePolicy->setSpacing(0);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);

    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);
    layout->setPortraitPolicy(portraitPolicy);
    layout->setLandscapePolicy(landscapePolicy);
    controller->setLayout(layout);

    return layout;
}

QGraphicsGridLayout *MBannerViewPrivate::createGrid()
{
    gridBanner = new QGraphicsGridLayout();
    gridBanner->setContentsMargins(0, 0, 0, 0);
    gridBanner->setSpacing(0);
    controller->setLayout(gridBanner);

    return gridBanner;
}

void MBannerViewPrivate::layoutEventBanner()
{
    Q_Q(MBannerView);

    layout = createLayout();

    if (!q->model()->iconID().isEmpty()) {
        icon()->setStyleName("MBannerIconEvent");
        landscapePolicy->addItem(icon(), Qt::AlignTop);
        portraitPolicy->addItem(icon(), Qt::AlignTop);
    }

    if (!q->model()->title().isEmpty()) {
        title()->setAlignment(Qt::AlignTop);
        title()->setStyleName("MBannerTitleEvent");
        landscapePolicy->addItem(title(), Qt::AlignTop);
        portraitPolicy->addItem(title(), Qt::AlignTop);
    }

    if (!q->model()->subtitle().isEmpty()) {
        subtitle()->setAlignment(Qt::AlignTop);
        subtitle()->setStyleName("MBannerSubtitleEvent");
        landscapePolicy->addItem(subtitle(), Qt::AlignTop);
        portraitPolicy->addItem(subtitle(), Qt::AlignTop);
    }
    portraitPolicy->addStretch();
    landscapePolicy->addStretch();
}

void MBannerViewPrivate::layoutInformationBanner()
{
    Q_Q(MBannerView);

    layout = createLayout();

    if (!q->model()->iconID().isEmpty()) {
        icon()->setStyleName("MBannerIconInformation");
        landscapePolicy->addItem(icon(), Qt::AlignTop);
        portraitPolicy->addItem(icon(), Qt::AlignTop);
    }
    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("MBannerTitleInformation");
        landscapePolicy->addItem(title(), Qt::AlignTop);
        portraitPolicy->addItem(title(), Qt::AlignTop);
    }
    landscapePolicy->addStretch();
    portraitPolicy->addStretch();
}

void MBannerViewPrivate::layoutSystemBanner()
{
    Q_Q(MBannerView);

    layout = createLayout();

    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("MBannerTitleSystem");
        landscapePolicy->addItem(title(), Qt::AlignTop);
        portraitPolicy->addItem(title(), Qt::AlignTop);
    }
    landscapePolicy->addStretch();
    portraitPolicy->addStretch();
}

void MBannerViewPrivate::layoutGenericBanner(){

    Q_Q(MBannerView);

    q->model()->setStyleName("GenericBanner");

    layout = createLayout();

    if (!q->model()->iconID().isEmpty()) {
        icon()->setStyleName("MBannerIconGeneric");
        landscapePolicy->addItem(icon(), Qt::AlignTop);
        portraitPolicy->addItem(icon(), Qt::AlignTop);
    }
    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("MBannerTitleGeneric");
        landscapePolicy->addItem(title(), Qt::AlignTop);
        portraitPolicy->addItem(title(), Qt::AlignTop);
    }
    if (!q->model()->subtitle().isEmpty()) {
        subtitle()->setStyleName("MBannerSubTitleGeneric");
        landscapePolicy->addItem(subtitle(), Qt::AlignTop);
        portraitPolicy->addItem(subtitle(), Qt::AlignTop);
    }
    if (q->model()->bannerTimeStamp().isValid()) {
        bannerTimeStamp()->setStyleName("MBannerTimeStampGeneric");
        landscapePolicy->addItem(bannerTimeStamp(), Qt::AlignTop);
        portraitPolicy->addItem(bannerTimeStamp(), Qt::AlignTop);
    }
    landscapePolicy->addStretch();
    portraitPolicy->addStretch();
    qWarning() << "You are using a generic MBanner. Consider use System Banner,Information Banner or Event Banner";
}

void MBannerViewPrivate::layoutEventLockScreen()
{
    Q_Q(MBannerView);

    gridBanner = createGrid();

    icon()->setStyleName("Icon");
    title()->setStyleName("HeadLine");
    title()->setTextElide(true);
    subtitle()->setStyleName("Body");
    bannerTimeStamp()->setStyleName("TimeStamp");
    prefixTimeStamp()->setStyleName("PrefixTimeStamp");

    gridBanner->addItem(icon(), 0, 0, 3, 1, Qt::AlignTop);
    gridBanner->addItem(title(), 0 , 1, Qt::AlignTop);
    gridBanner->addItem(subtitle(), 1, 1, Qt::AlignTop);

    if (!q->model()->prefixTimeStamp().isEmpty()) {
        QGraphicsLinearLayout *layoutStamp = new QGraphicsLinearLayout(gridBanner);
        layoutStamp->addItem(prefixTimeStamp());
        layoutStamp->addItem(bannerTimeStamp());
        layoutStamp->addStretch();
        gridBanner->addItem(layoutStamp, 2, 1,Qt::AlignTop);
    } else {
        gridBanner->addItem(bannerTimeStamp(), 2, 1, Qt::AlignTop);
    }
}

void MBannerViewPrivate::layoutEventScreen()
{
    Q_Q(MBannerView);

    gridBanner = createGrid();

    icon()->setStyleName("IconScreen");
    title()->setStyleName("HeadLine");
    subtitle()->setStyleName("Body");
    prefixTimeStamp()->setStyleName("PrefixTimeStamp");
    bannerTimeStamp()->setStyleName("TimeStamp");

    gridBanner->addItem(icon(), 0, 0, 3, 1, Qt::AlignTop);
    gridBanner->addItem(title(), 0, 1, Qt::AlignTop);
    gridBanner->addItem(subtitle(), 1, 1, Qt::AlignTop);

    if (!q->model()->prefixTimeStamp().isEmpty()) {
        QGraphicsLinearLayout *layoutStamp = new QGraphicsLinearLayout(gridBanner);
        layoutStamp->addItem(prefixTimeStamp());
        layoutStamp->addItem(bannerTimeStamp());
        layoutStamp->addStretch();
        gridBanner->addItem(layoutStamp, 2, 1,Qt::AlignTop);
    } else {
        gridBanner->addItem(bannerTimeStamp(), 2, 1, Qt::AlignTop);
    }
}

void MBannerViewPrivate::initDynamicLayout()
{
    Q_Q(MBannerView);

    if (q->model()->styleName()=="EventBanner") {
        layoutEventBanner();
    } else if (q->model()->styleName()=="EventScreen") {
        layoutEventScreen();
    } else if (q->model()->styleName()=="LockScreen") {
        //Until new designs same layout as event screen
        layoutEventScreen();
        //layoutEventLockScreen();
    } else if (q->model()->styleName()=="SystemBanner") {
        layoutSystemBanner();
    } else if (q->model()->styleName()=="InformationBanner") {
        layoutInformationBanner();
    } else {
        //If wrong or empty styleName == generic banner
        layoutGenericBanner();
    }
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
        d->setIcon(model()->iconID(),style()->iconSize());
    if (!model()->bannerTimeStamp().isNull())
        d->setBannerTimeStamp(model()->bannerTimeStamp());
    if(!model()->prefixTimeStamp().isEmpty()){
        d->setPrefixTimeStamp(model()->prefixTimeStamp());
    }
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
    bool updateLayout=false;
    foreach(member, modifications) {
        if (member == MBannerModel::Title) {
            d->setTitle(model()->title());
            updateLayout=true;
        } else if (member == MBannerModel::Subtitle) {
            d->setSubtitle(model()->subtitle());
            updateLayout=true;
        } else if (member == MBannerModel::IconID) {
            d->setIcon(model()->iconID(),style()->iconSize());
            updateLayout=true;
        } else if (member == MBannerModel::BannerTimeStamp) {
            d->setBannerTimeStamp(model()->bannerTimeStamp());
            updateLayout=true;
        } else if (member == MBannerModel::PrefixTimeStamp) {
            d->setPrefixTimeStamp(model()->prefixTimeStamp());
            updateLayout=true;
        }
    }
    /*! Due to the dynamic method for the layouts
    is necessary to check every widget to avoid unnecessary updates
      */
    if (updateLayout == true) {
        d->initDynamicLayout();
    }
}

M_REGISTER_VIEW_NEW(MBannerView, MBanner)
