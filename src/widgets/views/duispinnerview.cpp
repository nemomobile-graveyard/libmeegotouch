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
#include <math.h>

#include "duispinnerview.h"
#include "duispinnerview_p.h"

#include "duiprogressindicator.h"
#include "duiprogressindicator_p.h"
#include "duiviewcreator.h"

#include "duitheme.h"
#include "duiscalableimage.h"
#include "duidebug.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int SpinnerRefreshRate = 30;

DuiSpinnerViewPrivate::DuiSpinnerViewPrivate()
    :  controller(0),
       inactiveElement(0),
       activeElement(0),
       position(0),
       elapsed(0),
       timer(0)
{
}


DuiSpinnerViewPrivate::~DuiSpinnerViewPrivate()
{
    DuiTheme::releasePixmap(activeElement);
    DuiTheme::releasePixmap(inactiveElement);

    delete timer;
}


DuiSpinnerView::DuiSpinnerView(DuiProgressIndicator *controller) :
    DuiWidgetView(* new DuiSpinnerViewPrivate, controller)
{
    Q_D(DuiSpinnerView);
    d->controller = controller;
    connect(controller, SIGNAL(visibleChanged()), this, SLOT(visibilityChanged()));
}


DuiSpinnerView::~DuiSpinnerView()
{
}

void DuiSpinnerViewPrivate::animationTimeout()
{
    Q_Q(DuiSpinnerView);

    if (q->model()->unknownDuration()) {

        // calculate interval in secs and add it to elapsed time
        qreal interval = (qreal) timer->interval() / 1000.0;
        elapsed += interval;

        // calculate how many steps we should take
        int steps = (int)(elapsed * (qreal) q->style()->speed());
        if (steps > 0) {
            // subtract the amount we will step from the elapsed time
            elapsed -= steps * (1.0 / (qreal) q->style()->speed());
            // and perform the stepping
            position = (position + steps) % elements.count();
            // redraw
            q->update();
        }
    }
}

void DuiSpinnerView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);

    Q_D(DuiSpinnerView);

    foreach(const char * member, modifications) {
        if (member == DuiProgressIndicatorModel::UnknownDuration) {
            if (model()->unknownDuration()) {
                if (!d->timer) {
                    d->timer = new QTimer(this);
                    connect(d->timer, SIGNAL(timeout()), this, SLOT(animationTimeout()));
                }
                d->timer->start(SpinnerRefreshRate);
            } else {
                delete d->timer;
                d->timer = NULL;
            }
        }
    }

    update();
}

void DuiSpinnerView::setupModel()
{
    DuiWidgetView::setupModel();

    Q_D(DuiSpinnerView);

    if (model()->unknownDuration()) {
        if (!d->timer) {
            d->timer = new QTimer(this);
            connect(d->timer, SIGNAL(timeout()), this, SLOT(animationTimeout()));
        }
        d->timer->start(SpinnerRefreshRate);
    } else {
        delete d->timer;
        d->timer = NULL;
    }

    update();
}


void DuiSpinnerView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    Q_D(const DuiSpinnerView);

    const int elementsCount = d->elements.count();
    if (style()->inactiveImage() && style()->activeImage()) {

        // size of an element
        QSize size(style()->elementSize(), style()->elementSize());

        if (model()->unknownDuration()) {
            // every n'th should be active
            int span = (d->elements.count() / style()->activeElementCount());
            // this tells the active index within the span
            int activeIndexInSpan = d->position % span;

            // draw all elements with proper image
            for (int i = 0; i < elementsCount; ++i) {
                if ((i % span) == activeIndexInSpan) {
                    painter->drawPixmap(QRect(d->elements[i], size), *style()->activeImage());
                } else {
                    painter->drawPixmap(QRect(d->elements[i], size), *style()->inactiveImage());
                }
            }
        } else {
            // active element count
            int active = (model()->value() - model()->minimum()) * d->elements.count() /
                         qMax(model()->maximum() - model()->minimum(), 1);

            // draw active elements
            for (int i = 0; i < active; ++i) {
                painter->drawPixmap(QRect(d->elements[i], size), *style()->activeImage());
            }

            // draw inactive elements
            for (int i = active; i < elementsCount; ++i) {
                painter->drawPixmap(QRect(d->elements[i], size), *style()->inactiveImage());
            }
        }
    }
}

void DuiSpinnerViewPrivate::calculateShape(QSizeF size)
{
    Q_Q(DuiSpinnerView);

    QSizeF s = size - QSizeF(q->style()->elementSize(), q->style()->elementSize());

    qreal diameter = qMin(s.width(), s.height());

    // clear existing elements
    elements.clear();

    if (q->style()->elementCount() > 0) {
        // center point
        QPoint center(size.width() * 0.5, size.height() * 0.5);

        // radius for the spinner
        qreal radius = diameter * 0.5f;

        // half element size
        qreal halfElementSize = q->style()->elementSize() * 0.5;

        qreal angle = 0;
        qreal span = (M_PI * 2.0) / q->style()->elementCount();

        // calculate spherical shape and store points where the item should be drawn
        const int elementsCount = q->style()->elementCount();
        for (int i = 0; i < elementsCount; ++i) {

            QPoint position;
            position.setX(center.x() + (sinf(angle) * radius) - halfElementSize);
            position.setY(center.y() - (cosf(angle) * radius) - halfElementSize);

            elements.append(position);

            angle += span;
        }
    }
}

void DuiSpinnerView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(DuiSpinnerView);
    d->calculateShape(event->newSize());
}

void DuiSpinnerView::applyStyle()
{
    Q_D(DuiSpinnerView);
    DuiWidgetView::applyStyle();
    d->calculateShape(size());
}


void DuiSpinnerViewPrivate::visibilityChanged()
{
    if (timer) {
        if (controller->isVisible()) {
            timer->start(SpinnerRefreshRate);
        } else {
            timer->stop();
        }
    }
}

#include "moc_duispinnerview.cpp"

// bind controller widget and view widget together by registration macro
DUI_REGISTER_VIEW_NEW(DuiSpinnerView, DuiProgressIndicator)
