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
#include "qapplication.h"

#include <QString>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <QPropertyAnimation>

#include "mtheme.h"
#include "mviewcreator.h"
#include "mpositionindicator.h"
#include "mscalableimage.h"

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

void MPositionIndicatorView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MPositionIndicatorView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    QSizeF  vpSize = model()->viewportSize();
    QRectF  pRange = model()->range();
    QPointF pPos   = model()->position();

    const MScalableImage *indicator = style()->indicatorImage();
    const MScalableImage *rail = style()->backgroundImage();


    if (pRange.height() > vpSize.height()) {

        int indicatorPixmapSizeX = indicator->pixmap()->width();
        int railPixmapSizeX = rail->pixmap()->width();

        int indicatorHeight = qMax(style()->minIndicatorSize(), int((vpSize.height()/pRange.height())*size().height()));
        int indicatorPositionY = (pPos.y()/pRange.height())*size().height();

        if (indicatorPositionY + indicatorHeight > size().height()) {
            indicatorHeight = size().height() - indicatorPositionY;
        }

        if (indicatorPositionY < 0) {
            indicatorHeight += indicatorPositionY;
            indicatorPositionY = 0;
        }

        int railPositionX = (qApp->layoutDirection() == Qt::RightToLeft ? 0 : size().width() - railPixmapSizeX);
        int indicatorPositionX = (qApp->layoutDirection() == Qt::RightToLeft ? 0 : size().width() - indicatorPixmapSizeX);

        rail->draw( railPositionX,
                    0,
                    railPixmapSizeX,
                    size().height(),
                    painter);



        indicator->draw(indicatorPositionX,
                        indicatorPositionY,
                        indicatorPixmapSizeX,
                        indicatorHeight,
                        painter);
    }

    if (pRange.width() > vpSize.width()) {

        int indicatorPixmapSizeY = indicator->pixmap()->height();
        int railPixmapSizeY = rail->pixmap()->height();
        int indicatorWidth = qMax(style()->minIndicatorSize(), int((vpSize.width()/pRange.width())*size().width()));
        int indicatorPositionX = (pPos.x()/pRange.width())*size().width();

        if (indicatorPositionX + indicatorWidth > size().width()) {
            indicatorWidth = size().width() - indicatorPositionX;
        }

        if (indicatorPositionX < 0) {
            indicatorWidth += indicatorPositionX;
            indicatorPositionX = 0;
        }

        rail->draw( 0,
                    size().height() - railPixmapSizeY,
                    size().width(),
                    railPixmapSizeY,
                    painter);

        indicator->draw(indicatorPositionX,
                        size().height() - indicatorPixmapSizeY,
                        indicatorWidth,
                        indicatorPixmapSizeY,
                        painter);
    }
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
