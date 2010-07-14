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

#include <QApplication>
#include <QPainter>
#include <QPropertyAnimation>

#include "mspinnerview.h"
#include "mspinnerview_p.h"

#include "mprogressindicator.h"

#include "mtheme.h"

MSpinnerViewPrivate::MSpinnerViewPrivate()
    :  q_ptr(0),
       controller(0),
       pieBrush(Qt::NoBrush),
       piePen(Qt::NoPen),
       positionAnimation(0)
{
}

MSpinnerViewPrivate::~MSpinnerViewPrivate()
{
    delete positionAnimation;
}

void MSpinnerViewPrivate::checkAnimationStatus()
{
    if (positionAnimation) {
        if (controller->isVisible() && controller->isOnDisplay())
            positionAnimation->start();
        else
            positionAnimation->stop();
    }
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

    if (model()->unknownDuration()) {
        if (!d->positionAnimation) {
            d->controller->setRange(0, 360);
            d->positionAnimation = new QPropertyAnimation(d->controller, "value", 0);
            d->positionAnimation->setStartValue(0);
            d->positionAnimation->setEndValue(360);
            d->positionAnimation->setStartValue(0);
            d->positionAnimation->setLoopCount(-1);
            d->positionAnimation->setDuration(style()->period());
            d->positionAnimation->start();
        }
    } else {
        if (d->positionAnimation) {
            d->positionAnimation->stop();
            delete d->positionAnimation;
            d->positionAnimation = 0;
        }
    }
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

     if (d->positionAnimation) {
         d->positionAnimation->setDuration(style()->period());
         d->positionAnimation->start();
     }

     d->pieBrush.setStyle(Qt::NoBrush);

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

    bool reverse = qApp->isRightToLeft();

    //drawing bg
    if (style()->bgPixmap() && !style()->bgPixmap()->isNull())
        painter->drawPixmap(0, 0, *style()->bgPixmap());

    // calculated values input into rendering
    qreal startAngle = 0.0;
    qreal endAngle = 0.0;

    // calculate values depending on mode
    // (from spinner ring is visible that part where is no pie)
    if (model()->unknownDuration()) {
        if (!reverse) {
            startAngle = model()->value();
            endAngle = startAngle +  90;
        } else {
            startAngle = -model()->value();
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

    if (style()->progressPixmap() && !style()->progressPixmap()->isNull()) {
        if (d->pieBrush.style() == Qt::NoBrush)
            d->pieBrush.setTexture(*style()->progressPixmap());
    }

    if (!d->pieBrush.texture().isNull()) {
       painter->setBrush(d->pieBrush);
       painter->setPen(d->piePen);
       painter->drawPie(0, 0,
                        d->pieBrush.texture().width(), d->pieBrush.texture().height(),
                        (90 - endAngle)*16, (endAngle - startAngle)*16);
    }
}

void MSpinnerView::visibilityChangedSlot()
{
    Q_D(MSpinnerView);
    d->checkAnimationStatus();
}

void MSpinnerView::displayEnteredSlot()
{
    Q_D(MSpinnerView);
    d->checkAnimationStatus();
}

void MSpinnerView::displayExitedSlot()
{
    Q_D(MSpinnerView);
    d->checkAnimationStatus();
}

#include "moc_mspinnerview.cpp"

// bind controller widget and view widget together by registration macro
M_REGISTER_VIEW_NEW(MSpinnerView, MProgressIndicator)
