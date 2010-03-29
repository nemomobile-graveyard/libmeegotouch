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

#include <QPainter>
#include <QTimeLine>
#include <QTimer>
#include <QGraphicsSceneResizeEvent>
#include <QApplication>

#include "duiprogressindicatorbarview.h"
#include "duiprogressindicatorbarview_p.h"
#include "duiprogressindicator.h"
#include "duiviewcreator.h"
#include "duiprogressindicator_p.h" // For the member indexes of the model
#include "duiscalableimage.h"
#include "duitheme.h"
#include "duidebug.h"

// 30 fps
const int ProgressBarUpdateInterval = 1000 / 30;

DuiProgressIndicatorBarViewPrivate::DuiProgressIndicatorBarViewPrivate()
    :  controller(0),
       elementSize(0),
       activeElementCount(0),
       speed(0),
       position(0),
       timer(0)
{
}


DuiProgressIndicatorBarViewPrivate::~DuiProgressIndicatorBarViewPrivate()
{
    delete timer;
}


DuiProgressIndicatorBarView::DuiProgressIndicatorBarView(DuiProgressIndicator *controller) :
    DuiWidgetView(* new DuiProgressIndicatorBarViewPrivate, controller)
{
    Q_D(DuiProgressIndicatorBarView);

    d->controller = controller;
    connect(controller, SIGNAL(visibleChanged()), this, SLOT(visibilityChangedSlot()));

}


DuiProgressIndicatorBarView::~DuiProgressIndicatorBarView()
{
}

void DuiProgressIndicatorBarView::animationTimeout()
{
    Q_D(DuiProgressIndicatorBarView);

    if (model()->unknownDuration()) {

        // calculate interval in secs and add it to elapsed time
        qreal elapsed = (qreal) d->timer->interval() / 1000.0;

        // calculate how many pixels to move
        if (elapsed > 0) {
            qreal distance = elapsed * (qreal) style()->speed();

            // current position
            qreal width = rect().width();

            // increment
            qreal current = d->position * width;
            current += distance;

            // make sure it falls to range
            int count = (int)(current / width);
            current -= count * width;
            d->position = current / width;

            // redraw
            update();
        }
    }
}

void DuiProgressIndicatorBarView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);

    Q_D(DuiProgressIndicatorBarView);

    foreach(const char * member, modifications) {
        if (member == DuiProgressIndicatorModel::UnknownDuration) {
            if (model()->unknownDuration()) {
                if (!d->timer) {
                    d->timer = new QTimer(this);
                    connect(d->timer, SIGNAL(timeout()), this, SLOT(animationTimeout()));
                }
                d->timer->start(ProgressBarUpdateInterval);
            } else {
                delete d->timer;
                d->timer = NULL;
            }
        }
    }

    update();
}

void DuiProgressIndicatorBarView::setupModel()
{
    DuiWidgetView::setupModel();

    Q_D(DuiProgressIndicatorBarView);

    if (model()->unknownDuration()) {
        if (!d->timer) {
            d->timer = new QTimer(this);
            connect(d->timer, SIGNAL(timeout()), this, SLOT(animationTimeout()));
        }
        d->timer->start(ProgressBarUpdateInterval);
    } else {
        delete d->timer;
        d->timer = NULL;
    }

    update();
}

void DuiProgressIndicatorBarView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const DuiProgressIndicatorBarView);

    if (style()->activeImage() && style()->inactiveImage()) {

        bool reverse = qApp->isRightToLeft();

        QRect r(rect().toRect());
        style()->inactiveImage()->draw(r, painter);

        int left, right;
        style()->activeImage()->borders(&left, &right, NULL, NULL);
        int minimumScalableWidth = left + right;

        if (!model()->unknownDuration()) {
            qreal offset = (qreal)(model()->value() - model()->minimum()) / (qreal)(model()->maximum() - model()->minimum());
            if (offset > 0) {
                if (!reverse) {
                    r.setRight(offset * r.width());
                    if (r.width() < minimumScalableWidth)
                        r.setRight(r.left() + minimumScalableWidth);
                } else {
                    r.setLeft((1.0 - offset) * r.width());
                    if (r.width() < minimumScalableWidth)
                        r.setLeft(r.right() - minimumScalableWidth);
                }
                style()->activeImage()->draw(r, painter);
            }
        } else {

            qreal distance = d->position * (qreal) r.width();

            // need to draw in 1 or 2 parts, depending if the indicator element goes across the ends
            if ((distance + style()->elementSize()) > r.width()) {
                // two draw calls
                QRect r2(r);
                if (!reverse) {
                    r.setLeft(distance);
                    r2.setRight(style()->elementSize() - r.width());
                } else {
                    r.setLeft(0);
                    r.setRight(r.width() - distance);
                    r2.setLeft(r2.width() - (style()->elementSize() - r.width()));
                }

                if (r.width() >= minimumScalableWidth)
                    style()->activeImage()->draw(r, painter);
                if (r2.width() >= minimumScalableWidth)
                    style()->activeImage()->draw(r2, painter);
            } else {

                // one draw call
                if (!reverse) {
                    r.setLeft(distance);
                    r.setRight(distance + style()->elementSize());
                    if (r.width() < minimumScalableWidth)
                        r.setRight(r.left() + minimumScalableWidth);
                } else {
                    r.setRight(r.width() - distance);
                    r.setLeft(r.right() - style()->elementSize());
                    if (r.width() < minimumScalableWidth)
                        r.setLeft(r.right() - minimumScalableWidth);
                }
                style()->activeImage()->draw(r, painter);
            }
        }
    }
}

void DuiProgressIndicatorBarView::visibilityChangedSlot()
{
    Q_D(DuiProgressIndicatorBarView);
    if (d->timer) {
        if (d->controller->isVisible()) {
            d->timer->start(ProgressBarUpdateInterval);
        } else {
            d->timer->stop();
        }
    }
}


// bind controller widget and view widget together by registration macro
DUI_REGISTER_VIEW_NEW(DuiProgressIndicatorBarView, DuiProgressIndicator)

