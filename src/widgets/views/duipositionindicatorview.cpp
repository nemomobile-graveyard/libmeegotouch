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

#include "duipositionindicatorview.h"
#include "duipositionindicatorview_p.h"

#include <QString>
#include <QTimer>
#include <QStyleOptionGraphicsItem>

#include "duipositionindicator_p.h"  // For the member indexes of the model
#include "duitheme.h"
#include "duiviewcreator.h"
#include "duipositionindicator.h"
#include "duiabstractwidgetanimation.h"
#define NODEBUG


DuiPositionIndicatorViewPrivate::DuiPositionIndicatorViewPrivate()
    : controller(0),
      hideTimer(new QTimer()),
      visible(false)
{
}

DuiPositionIndicatorViewPrivate::~DuiPositionIndicatorViewPrivate()
{
    delete this->hideTimer;
}

DuiPositionIndicatorView::DuiPositionIndicatorView(DuiPositionIndicator *controller) :
    DuiWidgetView(* new DuiPositionIndicatorViewPrivate, controller)
{
    Q_D(DuiPositionIndicatorView);
    d->controller = controller;

    connect(d->hideTimer, SIGNAL(timeout()), this, SLOT(hide()));
    d->hideTimer->setSingleShot(true);
}

DuiPositionIndicatorView::~DuiPositionIndicatorView()
{
    Q_D(DuiPositionIndicatorView);
    d->controller = 0;
}

void DuiPositionIndicatorView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    /*
        Q_D(const DuiPositionIndicatorView);
    */
    Q_UNUSED(option);
    Q_UNUSED(painter);

    /*
        if(d->visible) {
    */
    /*
    int dotDist = style()->pixmapDistance();
    QSize dotSize = style()->onPixmap()->size();
    QSizeF vpSize = model()->viewportSize();
    QRectF pRange = model()->pannedRange();
    QPointF pPos = model()->pannedPos();
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
    */
    /*
        }
    */
}

void DuiPositionIndicatorView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);

    resetHideTimer();
    update();
}

void DuiPositionIndicatorView::setupModel()
{
    DuiWidgetView::setupModel();

    resetHideTimer();
    update();
}

void DuiPositionIndicatorView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    DuiWidgetView::resizeEvent(event);
    update();
}

void DuiPositionIndicatorView::hide()
{
    Q_D(DuiPositionIndicatorView);
    d->visible = false;
    if (showAnimation())
        showAnimation()->stop();
    if (hideAnimation())
        hideAnimation()->start();
    update();
}

void DuiPositionIndicatorView::resetHideTimer()
{
    Q_D(DuiPositionIndicatorView);
    if (!d->visible) {
        if (hideAnimation())
            hideAnimation()->stop();
        if (showAnimation())
            showAnimation()->start();
        d->visible = true;
    }
    d->hideTimer->stop();
    d->hideTimer->start(style()->hideTimeout());
}

DUI_REGISTER_VIEW_NEW(DuiPositionIndicatorView, DuiPositionIndicator)
