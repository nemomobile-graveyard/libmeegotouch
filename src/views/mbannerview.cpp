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

#include "mbanner.h"
#include "mbannerview.h"
#include "mbannerview_p.h"
#include "mviewcreator.h"
#include "mimagewidget.h"
#include "mlabel.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mgridlayoutpolicy.h"
#include "mviewconstants.h"

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
    bannerTimeStampData(QDateTime()),
    isDownOpacityEnabled(false),
    pixmapBanner(NULL),
    dayPassed(false),
    controller(0),
    isBeingPressed(false)
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
	//Banners can't have html
        bannerTimeStampLabel->setTextFormat(Qt::PlainText);
    }

    return bannerTimeStampLabel;
}

MLabel *MBannerViewPrivate::prefixTimeStamp()
{
    if (!prefixTimeStampLabel) {
        prefixTimeStampLabel = new MLabel(controller);
        //Banners can't have html
        prefixTimeStampLabel->setTextFormat(Qt::PlainText);
        prefixTimeStampLabel->setTextElide(true);
    }

    return prefixTimeStampLabel;
}

MImageWidget *MBannerViewPrivate::pixmap()
{

    if (!pixmapBanner) {
        Q_Q(MBannerView);
        pixmapBanner = new MImageWidget(controller);
        QSizePolicy policy(pixmapBanner->sizePolicy());
        policy.setHorizontalPolicy(QSizePolicy::Fixed);
        policy.setVerticalPolicy(QSizePolicy::Fixed);
        pixmapBanner->setSizePolicy(policy);
        pixmapBanner->setPreferredSize(q->style()->iconSize());
    }

    return pixmapBanner;
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
    Q_Q(MBannerView);
    if (date.isValid()) {
        //By default Date Time format is TimeShort (show hours & minutes + prefix)
        bannerTimeStamp();

        if (q->model()->bannerTimeStamp() != bannerTimeStampData) {
            bannerTimeStampData = q->model()->bannerTimeStamp();
            updateDateFormat();
        }
    }
}

void MBannerViewPrivate::setPrefixTimeStamp(const QString &string)
{
    prefixTimeStamp()->setText(string);
}

void MBannerViewPrivate::setPixmap(const QPixmap &pixmapTemp)
{
    pixmap()->setPixmap(pixmapTemp);
}

void MBannerViewPrivate::refreshStyleMode()
{
    Q_Q(MBannerView);

    if (q->model()->down()) {
        q->style().setModePressed();
    } else {
        q->style().setModeDefault();
    }

    q->applyStyle();
    q->update();
}

void MBannerViewPrivate::updateDateFormat() const
{
    Q_Q(const MBannerView);

    //If the datetime is more than 24 hours old, change the format to DateShort
    if (bannerTimeStampData.daysTo(QDateTime::currentDateTime()) >= 1) {
        QString formattedDateTime;
        dayPassed = true;
        if (q->style()->timestampSeparator().isEmpty()) {
            formattedDateTime = QString("%1 %2")
                .arg(MLocale().formatDateTime(q->model()->bannerTimeStamp().toLocalTime(), MLocale::DateShort, MLocale::TimeNone))
                .arg(MLocale().formatDateTime(q->model()->bannerTimeStamp().toLocalTime(), MLocale::DateNone, MLocale::TimeShort));
        } else {
            formattedDateTime = QString("%1 %2 %3")
                .arg(MLocale().formatDateTime(q->model()->bannerTimeStamp().toLocalTime(), MLocale::DateShort, MLocale::TimeNone))
                .arg(q->style()->timestampSeparator())
                .arg(MLocale().formatDateTime(q->model()->bannerTimeStamp().toLocalTime(), MLocale::DateNone, MLocale::TimeShort));
        }
        bannerTimeStampLabel->setText(formattedDateTime);
    } else {
        dayPassed = false;
        bannerTimeStampLabel->setText(MLocale().formatDateTime(
                    q->model()->bannerTimeStamp(), MLocale::DateNone,
                    MLocale::TimeShort));
    }
}

MLayout *MBannerViewPrivate::createLayout()
{
    layout = new MLayout(controller);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
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
    gridBanner = new QGraphicsGridLayout(controller);
    gridBanner->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    gridBanner->setContentsMargins(0, 0, 0, 0);
    gridBanner->setSpacing(0);
    controller->setLayout(gridBanner);

    return gridBanner;
}

void MBannerViewPrivate::layoutShortEventBanner()
{
    Q_Q(MBannerView);

    layout = createLayout();

    if (!q->model()->pixmap().isNull() && q->model()->iconID().isEmpty()) {
        pixmap()->setStyleName("ShortEventBannerIcon");
        icon()->setVisible(false);
        pixmap()->setVisible(true);
        landscapePolicy->addItem(pixmap(), Qt::AlignTop);
        portraitPolicy->addItem(pixmap(), Qt::AlignTop);
    } else if (!q->model()->iconID().isEmpty()) {
        icon()->setStyleName("ShortEventBannerIcon");
        icon()->setVisible(true);
        pixmap()->setVisible(false);
        landscapePolicy->addItem(icon(), Qt::AlignTop);
        portraitPolicy->addItem(icon(), Qt::AlignTop);
    } else {
        icon()->setVisible(false);
        pixmap()->setVisible(false);
    }

    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("ShortEventBannerTitle");
        title()->setVisible(true);
        title()->setAlignment(Qt::AlignTop);
        title()->setTextElide(true);
        title()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        landscapePolicy->addItem(title(), Qt::AlignTop);
        portraitPolicy->addItem(title(), Qt::AlignTop);
    }

    if (!q->model()->subtitle().isEmpty()) {
        subtitle()->setStyleName("ShortEventBannerSubtitle");
        subtitle()->setVisible(true);
        subtitle()->setWordWrap(false);
        subtitle()->setAlignment(Qt::AlignTop);
        subtitle()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
        landscapePolicy->addItem(subtitle(), Qt::AlignTop);
        portraitPolicy->addItem(subtitle(), Qt::AlignTop);
        portraitPolicy->setStretchFactor(subtitle(), 1);
        landscapePolicy->setStretchFactor(subtitle(), 1);
    } else {
        landscapePolicy->addStretch();
        portraitPolicy->addStretch();
    }

    prefixTimeStamp()->setVisible(false);
    bannerTimeStamp()->setVisible(false);

    isDownOpacityEnabled = true;
}

void MBannerViewPrivate::layoutInformationBanner()
{
    Q_Q(MBannerView);

    layout = createLayout();

    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("InformationBannerTitle");
        title()->setVisible(true);
        title()->setWordWrap(true);
        title()->setTextElide(true);
        landscapePolicy->addItem(title(), Qt::AlignTop);
        portraitPolicy->addItem(title(), Qt::AlignTop);
        if (MLocale::directionForText(q->model()->title()) == Qt::RightToLeft) {
            title()->setAlignment(Qt::AlignAbsolute | Qt::AlignRight | Qt::AlignVCenter);
        }
    }

    icon()->setVisible(false);
    subtitle()->setVisible(false);
    pixmap()->setVisible(false);
    prefixTimeStamp()->setVisible(false);
    bannerTimeStamp()->setVisible(false);
}

void MBannerViewPrivate::layoutSystemBanner()
{
    Q_Q(MBannerView);

    layout = createLayout();

    if (!q->model()->pixmap().isNull() && q->model()->iconID().isEmpty()) {
        pixmap()->setStyleName("SystemBannerTitleIcon");
        icon()->setVisible(false);
        pixmap()->setVisible(true);
        landscapePolicy->addItem(pixmap(), Qt::AlignTop);
        portraitPolicy->addItem(pixmap(), Qt::AlignTop);
    } else if (!q->model()->iconID().isEmpty()) {
        icon()->setStyleName("SystemBannerTitleIcon");
        icon()->setVisible(true);
        pixmap()->setVisible(false);
        landscapePolicy->addItem(icon(), Qt::AlignTop);
        portraitPolicy->addItem(icon(), Qt::AlignTop);
    } else {
        icon()->setVisible(false);
        pixmap()->setVisible(false);
    }

    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("SystemBannerTitle");
        title()->setVisible(true);
        title()->setWordWrap(true);
        title()->setTextElide(true);
        if (MLocale::directionForText(q->model()->title()) == Qt::RightToLeft) {
            title()->setAlignment(Qt::AlignAbsolute | Qt::AlignRight | Qt::AlignVCenter);
        }
        landscapePolicy->addItem(title(), Qt::AlignTop);
        portraitPolicy->addItem(title(), Qt::AlignTop);
    }

    subtitle()->setVisible(false);
    prefixTimeStamp()->setVisible(false);
    bannerTimeStamp()->setVisible(false);
}

void MBannerViewPrivate::layoutGenericBanner(){

    Q_Q(MBannerView);

    q->model()->setStyleName("GenericBanner");

    layout = createLayout();


    if (!q->model()->pixmap().isNull() && q->model()->iconID().isEmpty()) {
        pixmap()->setStyleName("GenericBannerIcon");
        icon()->setVisible(false);
        pixmap()->setVisible(true);
        landscapePolicy->addItem(pixmap(), Qt::AlignTop);
        portraitPolicy->addItem(pixmap(), Qt::AlignTop);
    } else if (!q->model()->iconID().isEmpty()) {
        icon()->setStyleName("GenericBannerIcon");
        icon()->setVisible(true);
        pixmap()->setVisible(false);
        landscapePolicy->addItem(icon(), Qt::AlignTop);
        portraitPolicy->addItem(icon(), Qt::AlignTop);
    } else {
        icon()->setVisible(false);
        pixmap()->setVisible(false);
    }

    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("GenericBannerTitle");
        title()->setVisible(true);
        title()->setWordWrap(true);
        title()->setTextElide(true);
        landscapePolicy->addItem(title(), Qt::AlignTop);
        portraitPolicy->addItem(title(), Qt::AlignTop);
    }

    if (!q->model()->subtitle().isEmpty()) {
        subtitle()->setStyleName("GenericBannerSubtitle");
        subtitle()->setVisible(true);
        subtitle()->setWordWrap(true);
        subtitle()->setTextElide(true);
        landscapePolicy->addItem(subtitle(), Qt::AlignTop);
        portraitPolicy->addItem(subtitle(), Qt::AlignTop);
    }

    if (!q->model()->prefixTimeStamp().isEmpty()) {
        prefixTimeStamp()->setStyleName("GenericBannerTimeStamp");
        prefixTimeStamp()->setVisible(true);
        landscapePolicy->addItem(prefixTimeStamp(), Qt::AlignTop);
        portraitPolicy->addItem(prefixTimeStamp(), Qt::AlignTop);
    } else {
        prefixTimeStamp()->setVisible(false);
    }

    if (q->model()->bannerTimeStamp().isValid()) {
        bannerTimeStamp()->setStyleName("GenericBannerTimeStamp");
        bannerTimeStamp()->setVisible(true);
        landscapePolicy->addItem(bannerTimeStamp(), Qt::AlignTop);
        portraitPolicy->addItem(bannerTimeStamp(), Qt::AlignTop);
    } else {
        bannerTimeStamp()->setVisible(false);
    }

    landscapePolicy->addStretch();
    portraitPolicy->addStretch();
    qWarning() << "You are using a generic MBanner. Consider use System Banner,Information Banner or Event Banner";
}

void MBannerViewPrivate::layoutFullEventBanner()
{
    Q_Q(MBannerView);

    gridBanner = createGrid();
    //Icon-ID has preference over setPixmap
    if (!q->model()->pixmap().isNull() && q->model()->iconID().isEmpty()) {
        pixmap()->setStyleName("FullEventBannerIcon");
        icon()->setVisible(false);
        pixmap()->setVisible(true);
        gridBanner->addItem(pixmap(), 0, 0, 3, 1, Qt::AlignTop);
    } else if (!q->model()->iconID().isEmpty()) {
        icon()->setStyleName("FullEventBannerIcon");
        icon()->setVisible(true);
        pixmap()->setVisible(false);
        gridBanner->addItem(icon(), 0, 0, 3, 1, Qt::AlignTop);
    } else {
        icon()->setVisible(false);
        pixmap()->setVisible(false);
    }

    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("FullEventBannerTitle");
        title()->setVisible(true);
        title()->setWordWrap(true);
        title()->setTextElide(true);
	title()->setAlignment(Qt::AlignTop);
        gridBanner->addItem(title(), 0, 1, Qt::AlignTop);
    } else {
        title()->setVisible(false);
    }

    if (!q->model()->subtitle().isEmpty()) {
        subtitle()->setStyleName("FullEventBannerSubtitle");
        subtitle()->setVisible(true);
        subtitle()->setWordWrap(false);
	subtitle()->setAlignment(Qt::AlignTop);
        gridBanner->addItem(subtitle(), 1, 1, Qt::AlignTop);
    } else {
        subtitle()->setVisible(false);
    }
    //Prefix is not shown if it doesn't come with a valid datetime
    if (!q->model()->prefixTimeStamp().isEmpty() && q->model()->bannerTimeStamp().isValid()) {
        QGraphicsLinearLayout *layoutStamp = new QGraphicsLinearLayout(gridBanner);
        layoutStamp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        layoutStamp->setContentsMargins(0,0,0,0);
        layoutStamp->setSpacing(0);
        prefixTimeStamp()->setStyleName("FullEventBannerPrefixTimeStamp");
	prefixTimeStamp()->setAlignment(Qt::AlignTop);
        prefixTimeStamp()->setVisible(true);
        bannerTimeStamp()->setStyleName("FullEventBannerTimeStamp");
	bannerTimeStamp()->setAlignment(Qt::AlignTop);
        bannerTimeStamp()->setVisible(true);
        layoutStamp->addItem(prefixTimeStamp());
        layoutStamp->addItem(bannerTimeStamp());
        layoutStamp->addStretch();
        //If the subtitle is not there timestamp & prefix go one row up
        if (q->model()->subtitle().isEmpty()) {
            gridBanner->addItem(layoutStamp, 1, 1, Qt::AlignTop);
        } else {
            gridBanner->addItem(layoutStamp, 2, 1, Qt::AlignTop);
        }
    } else if (q->model()->bannerTimeStamp().isValid()) {
        prefixTimeStamp()->setVisible(false);
        bannerTimeStamp()->setStyleName("FullEventBannerTimeStampIsolated");
	bannerTimeStamp()->setAlignment(Qt::AlignTop);
        if (q->model()->subtitle().isEmpty()) {
            gridBanner->addItem(bannerTimeStamp(), 1, 1, Qt::AlignTop);
        } else {
            gridBanner->addItem(bannerTimeStamp(), 2, 1, Qt::AlignTop);
        }
    } else {
        prefixTimeStamp()->setVisible(false);
        bannerTimeStamp()->setVisible(false);
    }
    isDownOpacityEnabled = false;
}

void MBannerViewPrivate::layoutLockScreenEventBanner()
{
    Q_Q(MBannerView);

    gridBanner = createGrid();

    //Icon-ID has preference over setPixmap
    if (!q->model()->pixmap().isNull() && q->model()->iconID().isEmpty()) {
        pixmap()->setStyleName("LockScreenEventBannerIcon");
        icon()->setVisible(false);
        pixmap()->setVisible(true);
        gridBanner->addItem(pixmap(), 0, 0, 3, 1, Qt::AlignTop);
    } else if (!q->model()->iconID().isEmpty()) {
        icon()->setStyleName("LockScreenEventBannerIcon");
        icon()->setVisible(true);
        pixmap()->setVisible(false);
        gridBanner->addItem(icon(), 0, 0, 3, 1, Qt::AlignTop);
    } else {
        icon()->setVisible(false);
        pixmap()->setVisible(false);
    }

    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("LockScreenEventBannerTitle");
        title()->setVisible(true);
        title()->setWordWrap(true);
        title()->setTextElide(true);
	title()->setAlignment(Qt::AlignTop);
        gridBanner->addItem(title(), 0, 1, Qt::AlignTop);
    } else {
        title()->setVisible(false);
    }

    if (!q->model()->subtitle().isEmpty()) {
        subtitle()->setStyleName("LockScreenEventBannerSubtitle");
        subtitle()->setVisible(true);
        subtitle()->setWordWrap(false);
	subtitle()->setAlignment(Qt::AlignTop);
        gridBanner->addItem(subtitle(), 1, 1, Qt::AlignTop);
    } else {
        subtitle()->setVisible(false);
    }
    //Prefix is not shown if it doesn't come with a valid datetime
    if (!q->model()->prefixTimeStamp().isEmpty() && q->model()->bannerTimeStamp().isValid()) {
        QGraphicsLinearLayout *layoutStamp = new QGraphicsLinearLayout(gridBanner);
        layoutStamp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        layoutStamp->setContentsMargins(0,0,0,0);
        layoutStamp->setSpacing(0);
        prefixTimeStamp()->setStyleName("LockScreenEventBannerPrefixTimeStamp");
	prefixTimeStamp()->setAlignment(Qt::AlignTop);
        prefixTimeStamp()->setVisible(true);
        bannerTimeStamp()->setStyleName("LockScreenEventBannerTimeStamp");
	bannerTimeStamp()->setAlignment(Qt::AlignTop);
        bannerTimeStamp()->setVisible(true);
        layoutStamp->addItem(prefixTimeStamp());
        layoutStamp->addItem(bannerTimeStamp());
        layoutStamp->addStretch();
        //If the subtitle is not there timestamp & prefix go one row up
        if (q->model()->subtitle().isEmpty()) {
            gridBanner->addItem(layoutStamp, 1, 1, Qt::AlignTop);
        } else {
            gridBanner->addItem(layoutStamp, 2, 1, Qt::AlignTop);
        }
    } else if (q->model()->bannerTimeStamp().isValid()) {
        prefixTimeStamp()->setVisible(false);
        bannerTimeStamp()->setStyleName("LockScreenEventBannerTimeStampIsolated");
	bannerTimeStamp()->setAlignment(Qt::AlignTop);
        if (q->model()->subtitle().isEmpty()) {
            gridBanner->addItem(bannerTimeStamp(), 1, 1, Qt::AlignTop);
        } else {
            gridBanner->addItem(bannerTimeStamp(), 2, 1, Qt::AlignTop);
        }
    } else {
        prefixTimeStamp()->setVisible(false);
        bannerTimeStamp()->setVisible(false);
    }
    isDownOpacityEnabled = false;
}

void MBannerViewPrivate::layoutPrivateEventBanner()
{
    /*!
      Private banner is the same as full event banner except that doesn't show subtitle (body)
      due to privacy reasons.
    */

    Q_Q(MBannerView);

    gridBanner = createGrid();

    //Icon-ID has preference over setPixmap
    if (!q->model()->pixmap().isNull() && q->model()->iconID().isEmpty()) {
        pixmap()->setStyleName("PrivateEventBannerIcon");
        icon()->setVisible(false);
        pixmap()->setVisible(true);
        gridBanner->addItem(pixmap(), 0, 0, 3, 1, Qt::AlignTop);
    } else if (!q->model()->iconID().isEmpty()) {
        icon()->setStyleName("PrivateEventBannerIcon");
        icon()->setVisible(true);
        pixmap()->setVisible(false);
        gridBanner->addItem(icon(), 0, 0, 3, 1, Qt::AlignTop);
    } else {
        icon()->setVisible(false);
	pixmap()->setVisible(false);
    }

    if (!q->model()->title().isEmpty()) {
        title()->setStyleName("PrivateEventBannerTitle");
        title()->setVisible(true);
        title()->setWordWrap(true);
        title()->setTextElide(true);
        title()->setAlignment(Qt::AlignTop);
        gridBanner->addItem(title(), 0, 1, Qt::AlignTop);
    } else {
        title()->setVisible(false);
    }

    subtitle()->setVisible(false);
    subtitle()->setActive(false);

    //Prefix is not shown if it doesn't come with a valid datetime
    if (!q->model()->prefixTimeStamp().isEmpty() && q->model()->bannerTimeStamp().isValid()) {
        QGraphicsLinearLayout *layoutStamp = new QGraphicsLinearLayout(gridBanner);
        layoutStamp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        layoutStamp->setContentsMargins(0,0,0,0);
        layoutStamp->setSpacing(0);
        prefixTimeStamp()->setStyleName("PrivateEventBannerPrefixTimeStamp");
	prefixTimeStamp()->setAlignment(Qt::AlignTop);
        prefixTimeStamp()->setVisible(true);
        bannerTimeStamp()->setStyleName("PrivateEventBannerTimeStamp");
	bannerTimeStamp()->setAlignment(Qt::AlignTop);
        bannerTimeStamp()->setVisible(true);
        layoutStamp->addItem(prefixTimeStamp());
        layoutStamp->addItem(bannerTimeStamp());
        layoutStamp->addStretch();
        gridBanner->addItem(layoutStamp, 1, 1, Qt::AlignTop);
    } else if (q->model()->bannerTimeStamp().isValid()) {
        prefixTimeStamp()->setVisible(false);
        bannerTimeStamp()->setStyleName("PrivateEventBannerTimeStampIsolated");
	bannerTimeStamp()->setAlignment(Qt::AlignTop);
        gridBanner->addItem(bannerTimeStamp(), 1, 1, Qt::AlignTop);
    } else {
        prefixTimeStamp()->setVisible(false);
        bannerTimeStamp()->setVisible(false);
    }
    isDownOpacityEnabled = false;
}

void MBannerViewPrivate::initDynamicLayout()
{
    Q_Q(MBannerView);

    if (q->model()->styleName() == MBannerType::ShortEventBanner) {
        layoutShortEventBanner();
    } else if (q->model()->styleName() == MBannerType::FullEventBanner) {
        layoutFullEventBanner();
    } else if (q->model()->styleName() == MBannerType::LockScreenEventBanner) {
        layoutLockScreenEventBanner();
    } else if (q->model()->styleName() == MBannerType::PrivateEventBanner) {
        layoutPrivateEventBanner();
    } else if (q->model()->styleName() == MBannerType::SystemBanner) {
        layoutSystemBanner();
    } else if (q->model()->styleName() == MBannerType::InformationBanner) {
        layoutInformationBanner();
    } else {
        //If wrong or empty styleName == generic banner
        layoutGenericBanner();
    }

    QObject::connect(controller, SIGNAL(clicked()), controller, SLOT(dismiss()));
}

void MBannerViewPrivate::manageOpacities() const
{
    Q_Q(const MBannerView);

    if (q->model()->down() && isDownOpacityEnabled) {
        if (iconId)
            iconId->setOpacity(q->style()->pressDownBannerOpacity());
        if (titleLabel)
            titleLabel->setOpacity(q->style()->pressDownBannerOpacity());
        if (subtitleLabel)
            subtitleLabel->setOpacity(q->style()->pressDownBannerOpacity());
        if (prefixTimeStampLabel)
            prefixTimeStampLabel->setOpacity(q->style()->pressDownBannerOpacity());
        if (bannerTimeStampLabel)
            bannerTimeStampLabel->setOpacity(q->style()->pressDownBannerOpacity());
    } else if (!q->model()->down() && isDownOpacityEnabled) {
        if (iconId)
            iconId->setOpacity(controller->opacity());
        if (titleLabel)
            titleLabel->setOpacity(controller->opacity());
        if (subtitleLabel)
            subtitleLabel->setOpacity(controller->opacity());
        if (prefixTimeStampLabel)
            prefixTimeStampLabel->setOpacity(controller->opacity());
        if (bannerTimeStampLabel)
            bannerTimeStampLabel->setOpacity(controller->opacity());
    }
}

MBannerView::MBannerView(MBanner *controller) :
    MSceneWindowView(controller),
    d_ptr(new MBannerViewPrivate)
{
    Q_D(MBannerView);
    d->q_ptr = this;
    d->controller = controller;

    bool ok = connect(&d_ptr->minimumTapFeedbackDurationTimer, SIGNAL(timeout()),
                      this, SLOT(onMinimumTapFeedbackDurationReached()));
    if (!ok) qFatal("Signal connection failed!");
    d_ptr->minimumTapFeedbackDurationTimer.setSingleShot(true);
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

    Q_D(MBannerView);

    if (model()->down())
        return;

    d->isBeingPressed = true;
    d->minimumTapFeedbackDurationTimer.start();

    model()->setDown(true);
    d->refreshStyleMode();
}

void MBannerView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MBannerView);

    if (!model()->down())
        return;

    if (!d->minimumTapFeedbackDurationTimer.isActive()) {
        model()->setDown(false);
        d->refreshStyleMode();
    }

    d->isBeingPressed = false;

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);

    if (rect.contains(touch)) {
        d->controller->click();
    }
}

void MBannerView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MBannerView);

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);

    if (rect.contains(touch)) {
        d->isBeingPressed = true;
        if (!model()->down()) {
            d->minimumTapFeedbackDurationTimer.start();
            model()->setDown(true);
            d->refreshStyleMode();
        }
    } else {
        d->isBeingPressed = false;
        if (model()->down() && !d->minimumTapFeedbackDurationTimer.isActive()) {
            model()->setDown(false);
            d->refreshStyleMode();
        }
    }
}

void MBannerView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MBannerView);

    if (!model()->down())
        return;

    if (!d->minimumTapFeedbackDurationTimer.isActive()) {
        model()->setDown(false);
        d->refreshStyleMode();
    }

    d->isBeingPressed = false;
}

void MBannerView::onMinimumTapFeedbackDurationReached()
{
    Q_D(MBannerView);
    if (model()->down() && !d->isBeingPressed) {
        model()->setDown(false);
        d->refreshStyleMode();
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
    if (!model()->prefixTimeStamp().isEmpty())
        d->setPrefixTimeStamp(model()->prefixTimeStamp());
    if (!model()->pixmap().isNull())
        d->setPixmap(model()->pixmap());

    d->initDynamicLayout();
}

void MBannerView::applyStyle()
{
    Q_D(MBannerView);
    MSceneWindowView::applyStyle();
    d->minimumTapFeedbackDurationTimer.setInterval(style()->minimumTapFeedbackDuration());
}

void MBannerView::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MBannerView);

    Q_UNUSED(painter);
    Q_UNUSED(option);

    if (d->bannerTimeStampLabel && d->dayPassed != (d->bannerTimeStampData.daysTo(QDateTime::currentDateTime()) >= 1)) {
        d->updateDateFormat();
    }
}

void MBannerView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_UNUSED(painter);

    Q_D(const MBannerView);

    d->manageOpacities();
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
        } else if (member == MBannerModel::Pixmap) {
            d->setPixmap(model()->pixmap());
            updateLayout=true;
        } else if (member == MBannerModel::StyleName) {
            updateLayout=true;
        }
    }
    /*! Due to the dynamic method for the layouts
    is necessary to check every widget to avoid unnecessary updates
      */
    if (updateLayout == true) {
        d->initDynamicLayout();
        update();
    }
}

M_REGISTER_VIEW_NEW(MBannerView, MBanner)
