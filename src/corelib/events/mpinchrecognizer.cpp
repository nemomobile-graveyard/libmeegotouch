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

#include "mpinchrecognizer.h"

#include "stdlib.h"

#include <QPinchGesture>
#include <QTouchEvent>

namespace {
    const qreal defaultResizeThreshold = 20;
}

MPinchRecognizer::MPinchRecognizer()
{
}

MPinchRecognizer::~MPinchRecognizer()
{
}

QGesture* MPinchRecognizer::create(QObject* target)
{
    Q_UNUSED(target);
    return new QPinchGesture();
}

QGestureRecognizer::Result MPinchRecognizer::recognize(QGesture* gesture,
                                                           QObject* watched,
                                                           QEvent* event)
{
    Q_UNUSED(watched);

    const QTouchEvent *ev = static_cast<const QTouchEvent*>(event);
    QPinchGesture *pinchGesture = static_cast<QPinchGesture*>(gesture);
    if (event->type() == QEvent::TouchBegin) {

        QTouchEvent::TouchPoint p1 = ev->touchPoints().at(0);
        QTouchEvent::TouchPoint p2 = p1;
        if (ev->touchPoints().size() > 1) {
            p2 = ev->touchPoints().at(1);
        }
        pinchGesture->setHotSpot(p1.pos() + (p2.pos() - p1.pos())/2);
        event->setAccepted(true);

        return QFlag(QGestureRecognizer::MayBeGesture | QGestureRecognizer::ConsumeEventHint);

    } else if (event->type() == QEvent::TouchUpdate) {

        if ( ev->touchPoints().size() < 2 ) {
            if (pinchGesture->state() != Qt::NoGesture)
                return QGestureRecognizer::FinishGesture;
            else
                return QGestureRecognizer::MayBeGesture;
        }

        QTouchEvent::TouchPoint p1 = ev->touchPoints().at(0);
        QTouchEvent::TouchPoint p2 = ev->touchPoints().at(1);

        if ( p1.state() == Qt::TouchPointReleased || p2.state() == Qt::TouchPointReleased ) {
            if (pinchGesture->state() != Qt::NoGesture)
                return QGestureRecognizer::FinishGesture;
            else
                return QGestureRecognizer::MayBeGesture;
        }

        QSizeF resizeValue;
        resizeValue.setWidth( abs(p2.pos().x()     - p1.pos().x()) -
                              abs(p2.lastPos().x() - p1.lastPos().x()) );

        resizeValue.setHeight( abs(p2.pos().y()     - p1.pos().y()) -
                               abs(p2.lastPos().y() - p1.lastPos().y()) );

        // 0.7*(x + y) approximates sqrt(x^2 + y^2)
        qreal scaleFactor = 0.7 * (resizeValue.width() + resizeValue.height());

        pinchGesture->setScaleFactor(scaleFactor);
        pinchGesture->setTotalScaleFactor( pinchGesture->totalScaleFactor() + scaleFactor);
        pinchGesture->setHotSpot(p1.pos() + (p2.pos() - p1.pos())/2);

        event->accept();

        qreal absoluteTotalResize;
        absoluteTotalResize = abs(pinchGesture->totalScaleFactor());

        if (pinchGesture->state() != Qt::NoGesture || defaultResizeThreshold < absoluteTotalResize)
        {
            return QGestureRecognizer::TriggerGesture;
        } else {
            return QGestureRecognizer::MayBeGesture;
        }
    } else if (event->type() == QEvent::TouchEnd) {
        if (pinchGesture->state() != Qt::NoGesture)
            return QGestureRecognizer::FinishGesture;
        else
            return QGestureRecognizer::CancelGesture;

    }
    return 0;
}

void MPinchRecognizer::reset(QGesture* gesture)
{
    QPinchGesture *pinchGesture = static_cast<QPinchGesture*>(gesture);
    pinchGesture->setTotalScaleFactor(0);
    pinchGesture->setLastScaleFactor(0);

    QGestureRecognizer::reset(gesture);
}
