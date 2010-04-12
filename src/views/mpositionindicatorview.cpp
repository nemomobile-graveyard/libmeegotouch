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

#include "mpositionindicatorview.h"
#include "mpositionindicatorview_p.h"

#include <QString>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <QPropertyAnimation>

#include "mtheme.h"
#include "mviewcreator.h"
#include "mpositionindicator.h"

MPositionIndicatorViewPrivate::MPositionIndicatorViewPrivate()
    : controller(0),
      hideTimer(new QTimer()),
      visible(false)
{
}

MPositionIndicatorViewPrivate::~MPositionIndicatorViewPrivate()
{
    delete this->hideTimer;
}

MPositionIndicatorView::MPositionIndicatorView(MPositionIndicator *controller) :
    MWidgetView(* new MPositionIndicatorViewPrivate, controller)
{
    Q_D(MPositionIndicatorView);
    d->controller = controller;

    connect(d->hideTimer, SIGNAL(timeout()), this, SLOT(hide()));
    d->hideTimer->setSingleShot(true);

    d->fadeAnimation = new QPropertyAnimation(controller, "opacity", this);
}

MPositionIndicatorView::~MPositionIndicatorView()
{
    Q_D(MPositionIndicatorView);
    d->controller = 0;
}

void MPositionIndicatorView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    /*
        Q_D(const MPositionIndicatorView);
    */
    Q_UNUSED(option);
    /*
        if(d->visible) {
    */
    int dotDist = style()->pixmapDistance();
    QSize dotSize = style()->onPixmap()->size();
    QSizeF vpSize = model()->viewportSize();
    QRectF pRange = model()->range();
    QPointF pPos = model()->position();
    const QPixmap *pixOff = style()->offPixmap();
    const QPixmap *pixOn = style()->onPixmap();

    int dotsX = size().width() / dotDist;
    int onDotsX = (2 * dotsX * vpSize.width() + pRange.width()) / pRange.width() / 2;
    onDotsX = qMax(onDotsX,
                   style()->minIndicatorDots());
    int onPosX = (2 * dotsX * pPos.x() + pRange.width()) / pRange.width() / 2;
    QPoint barPosH = QPoint(size().width() - dotsX * dotDist + (dotDist - dotSize.width()) / 2,
                            size().height() - dotDist + (dotDist - dotSize.height()) / 2);

    int dotsY = size().height() / dotDist;
    int onDotsY = (2 * dotsY * vpSize.height() + pRange.height()) / pRange.height() / 2;
    onDotsY = qMax(onDotsY,
                   style()->minIndicatorDots());
    int onPosY = (2 * dotsY * pPos.y() + pRange.height()) / pRange.height() / 2;
    QPoint barPosV = QPoint(size().width() - dotDist + (dotDist - dotSize.width()) / 2,
                            size().height() - dotsY * dotDist + (dotDist - dotSize.height()) / 2);

    int effPosX = qMax(0, onPosX);
    int effPosY = qMax(0, onPosY);

    if (pRange.width() > vpSize.width()) {
        QPointF currPos(barPosH);
        QPointF posAdd(dotDist, 0);
        bool pixState;

        for (int i = 0; i < dotsX; ++i) {
            pixState = (i < effPosX || i >= onPosX + onDotsX) ? false : true;
            painter->drawPixmap(currPos, pixState ? *pixOn : *pixOff);

            currPos += posAdd;
        }
    }

    if (pRange.height() > vpSize.height()) {
        QPointF currPos(barPosV);
        QPointF posAdd(0, dotDist);
        bool pixState;

        for (int i = 0; i < dotsY; ++i) {
            pixState = (i < effPosY || i >= onPosY + onDotsY) ? false : true;
            painter->drawPixmap(currPos, pixState ? *pixOn : *pixOff);

            currPos += posAdd;
        }
    }
    /*
        }
    */
}

void MPositionIndicatorView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    resetHideTimer();
    update();
}

void MPositionIndicatorView::setupModel()
{
    MWidgetView::setupModel();

    resetHideTimer();
    update();
}

void MPositionIndicatorView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MWidgetView::resizeEvent(event);
    update();
}

void MPositionIndicatorView::changeEvent(QEvent *event)
{
    Q_D(MPositionIndicatorView);

    MWidgetView::changeEvent(event);

    if (event->type() == QEvent::EnabledChange) {
        if (d->controller->isEnabled()) {
            d->controller->setVisible(true);
        } else {
            d->controller->setVisible(false);
        }
    }
}

void MPositionIndicatorView::hide()
{
    Q_D(MPositionIndicatorView);
    d->visible = false;
    d->fadeAnimation->stop();
    d->fadeAnimation->setEndValue(0.0f);
    d->fadeAnimation->start();
    update();
}

void MPositionIndicatorView::resetHideTimer()
{
    Q_D(MPositionIndicatorView);
    if (!d->visible) {
        d->fadeAnimation->stop();
        d->fadeAnimation->setEndValue(1.0f);
        d->fadeAnimation->start();
        d->visible = true;
    }
    d->hideTimer->stop();
    d->hideTimer->start(style()->hideTimeout());
}

M_REGISTER_VIEW_NEW(MPositionIndicatorView, MPositionIndicator)
