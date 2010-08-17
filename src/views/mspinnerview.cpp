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

#include <QPainter>
#include <QPropertyAnimation>

#include "mspinnerview.h"
#include "mspinnerview_p.h"

#include "mprogressindicator.h"

#include "mtheme.h"

MSpinnerViewPrivate::MSpinnerViewPrivate()
    :  q_ptr(NULL),
       controller(NULL),
       pieBrush(Qt::NoBrush),
       piePen(Qt::NoPen),
       backgroundPixmap(NULL),
       progressPixmap(NULL),
       positionAnimation(NULL),
       angle(0)
{
}

MSpinnerViewPrivate::~MSpinnerViewPrivate()
{
    delete positionAnimation;
}

void MSpinnerViewPrivate::resumeAnimation()
{
    if (controller->unknownDuration()) {
        if (positionAnimation->state() == QPropertyAnimation::Paused)
            positionAnimation->resume();
    }
}

void MSpinnerViewPrivate::pauseAnimation()
{
    if (positionAnimation->state() == QPropertyAnimation::Running)
        positionAnimation->pause();
}

MSpinnerView::MSpinnerView(MProgressIndicator *controller) :
    MWidgetView(controller),
    d_ptr(new MSpinnerViewPrivate)
{
    Q_D(MSpinnerView);
    d->q_ptr = this;
    d->controller = controller;

    connect(controller, SIGNAL(visibleChanged()), this, SLOT(visibilityChangedSlot()));

    connect(controller, SIGNAL(displayEntered()), this, SLOT(displayEnteredSlot()));
    connect(controller, SIGNAL(displayExited()), this, SLOT(displayExitedSlot()));
}


MSpinnerView::~MSpinnerView()
{
    delete d_ptr;
}

void MSpinnerView::setupAnimation()
{
    Q_D(MSpinnerView);

    if (!d->positionAnimation) {
        d->positionAnimation = new QPropertyAnimation(this, "angle", 0);
        d->positionAnimation->setDuration(style()->period());
        d->positionAnimation->setStartValue(0);
        d->positionAnimation->setEndValue(360);
        d->positionAnimation->setLoopCount(-1);
    }

    if (model()->unknownDuration()) {
        if (d->positionAnimation->state() == QPropertyAnimation::Paused)
            d->positionAnimation->resume();
        else
            d->positionAnimation->start();
    } else {
        if (d->positionAnimation->state() == QPropertyAnimation::Running)
            d->positionAnimation->pause();
    }
}

int MSpinnerView::angle() const
{
    Q_D(const MSpinnerView);

    return d->angle;
}

void MSpinnerView::setAngle(int angle)
{
    Q_D(MSpinnerView);

    d->angle = angle;
    update();
}

void MSpinnerView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    foreach(const char * member, modifications) {
        if (member == MProgressIndicatorModel::UnknownDuration)
            setupAnimation();
    }

    update();
}

void MSpinnerView::applyStyle()
{
     MWidgetView::applyStyle();

     Q_D(MSpinnerView);

     if (d->positionAnimation)
         d->positionAnimation->setDuration(style()->period());

     if (style()->progressPixmap() && !style()->progressPixmap()->isNull())
         d->progressPixmap = style()->progressPixmap();

     if (style()->bgPixmap() && !style()->bgPixmap()->isNull())
         d->backgroundPixmap = style()->bgPixmap();

     update();
}

void MSpinnerView::setupModel()
{
    MWidgetView::setupModel();
                    
    setupAnimation();

    update();
}

void MSpinnerView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const MSpinnerView);

    bool reverse = (d->controller->layoutDirection() == Qt::RightToLeft);

    //drawing background
    if (d->backgroundPixmap && !d->backgroundPixmap->isNull())
        painter->drawPixmap(0, 0, *d->backgroundPixmap);

    // calculated values input into rendering
    qreal startAngle = 0.0;
    qreal endAngle = 0.0;

    // calculate values depending on mode
    // (from spinner ring is visible that part where is no pie)
    if (model()->unknownDuration()) {
        if (!reverse) {
            startAngle = angle();
            endAngle = startAngle + 90;
        } else {
            startAngle = -angle();
            endAngle = startAngle - 90;
        }
    } else {
        if (!reverse) {
            startAngle = 0.0;
            endAngle = 360 * (model()->value() - model()->minimum()) / (model()->maximum() - model()->minimum());
        } else {
            startAngle = 0.0;
            endAngle =  -360 * (model()->value() - model()->minimum()) / (model()->maximum() - model()->minimum());
        }
    }

    if (d->progressPixmap && !d->progressPixmap->isNull()) {
        d->pieBrush.setTexture(*d->progressPixmap);

        painter->setBrush(d->pieBrush);
        painter->setPen(d->piePen);
        painter->drawPie(QRect(0, 0, d->progressPixmap->width(), d->progressPixmap->height()),
                        (90 - endAngle) * 16,
                        (endAngle - startAngle) * 16);
    }
}

void MSpinnerView::visibilityChangedSlot()
{
    Q_D(MSpinnerView);
    if (d->controller->isVisible() && d->controller->isOnDisplay())
        d->resumeAnimation();
    else
        d->pauseAnimation();
}

void MSpinnerView::displayEnteredSlot()
{
    Q_D(MSpinnerView);

    if (d->controller->isVisible())
        d->resumeAnimation();
}

void MSpinnerView::displayExitedSlot()
{
    Q_D(MSpinnerView);
    d->pauseAnimation();
}

#include "moc_mspinnerview.cpp"

// bind controller widget and view widget together by registration macro
M_REGISTER_VIEW_NEW(MSpinnerView, MProgressIndicator)
