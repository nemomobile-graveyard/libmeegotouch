/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mpanrecognizertouch.h"
#include "mpanrecognizertouch_p.h"
#include "mpangesturetouch.h"

#include <mdebug.h>
#include <mnamespace.h>
#include <mpanrecognizerstyle.h>
#include <mtheme.h>

MPanRecognizerTouchPrivate::MPanRecognizerTouchPrivate()
  : style(0),
    q_ptr(0)
{
}

MPanRecognizerTouchPrivate::~MPanRecognizerTouchPrivate()
{
}

void MPanRecognizerTouchPrivate::clearNotAlignedMovement(MPanGestureTouch *panGesture)
{
    if (panGesture->panDirection.testFlag(Qt::Vertical)) {
        QPointF zeroedOffset = panGesture->offset();
        zeroedOffset.setX(0);
        panGesture->setOffset(zeroedOffset);
    } else if (panGesture->panDirection.testFlag(Qt::Horizontal)) {
        QPointF zeroedOffset = panGesture->offset();
        zeroedOffset.setY(0);
        panGesture->setOffset(zeroedOffset);
    } else {
        panGesture->setOffset(QPointF());
    }
}

const QTouchEvent::TouchPoint *MPanRecognizerTouchPrivate::fetchPressedPoint(
    const QList<QTouchEvent::TouchPoint> &points)
{
    const QTouchEvent::TouchPoint *pressedPoint = 0;
    int i = 0;

    while (!pressedPoint && i < points.count()) {
        if (points.at(i).state() == Qt::TouchPointPressed) {
            pressedPoint = &points.at(i);
        }

        ++i;
    }

    return pressedPoint;
}

const QTouchEvent::TouchPoint *MPanRecognizerTouchPrivate::fetchTouchPointById(
    const QList<QTouchEvent::TouchPoint> &points, int wantedId)
{
    const QTouchEvent::TouchPoint *touchPoint = 0;
    int i = 0;

    while (!touchPoint && i < points.count()) {
        if (points.at(i).id() == wantedId) {
            touchPoint = &points.at(i);
        }

        ++i;
    }

    return touchPoint;
}

QGestureRecognizer::Result MPanRecognizerTouchPrivate::recognizePossibleNewGesture(
    MPanGestureTouch *panGesture, const QTouchEvent::TouchPoint *pressedPoint)
{
    Q_ASSERT(panGesture->state() == Qt::NoGesture);
    Q_ASSERT(panGesture->touchPointId == -1);

    panGesture->startScenePos = pressedPoint->scenePos();
    panGesture->setHotSpot(pressedPoint->screenPos());
    panGesture->touchPointId = pressedPoint->id();

    return QGestureRecognizer::MayBeGesture;
}


QGestureRecognizer::Result MPanRecognizerTouchPrivate::finishGesture(MPanGestureTouch *panGesture)
{
    panGesture->touchPointId = -1;

    if (panGesture->state() != Qt::NoGesture)
        return QGestureRecognizer::FinishGesture;
    else
        return QGestureRecognizer::CancelGesture;
}

QGestureRecognizer::Result MPanRecognizerTouchPrivate::updateGesture(
    MPanGestureTouch *panGesture, const QTouchEvent::TouchPoint *touchPoint)
{
    QGestureRecognizer::Result result = QGestureRecognizer::Ignore;
    Q_ASSERT(touchPoint->id() == panGesture->touchPointId);
    qreal distX, distY;

    panGesture->setLastOffset(panGesture->offset());
    panGesture->setOffset(touchPoint->scenePos() - panGesture->startScenePos);

    distX = abs(panGesture->offset().x());
    distY = abs(panGesture->offset().y());

    if (panGesture->state() != Qt::NoGesture) {
        // It has already been recognized as a pan gesture.
        // So let's just update it.
        result = QGestureRecognizer::TriggerGesture;
    } else {
        // Hasn't been yet recognized as a pan gesture
        // Check if it has moved enough to qualify as one
        if (distX > style->distanceThreshold() ||
                distY > style->distanceThreshold()) {
            panGesture->panDirection = (distX > distY ? Qt::Horizontal : Qt::Vertical);
            // It is a pan gesture
            result = QGestureRecognizer::TriggerGesture;
        } else {
            // Hasn't moved enough yet.
            result = QGestureRecognizer::MayBeGesture;
        }
    }

    clearNotAlignedMovement(panGesture);

    return result;
}

QGestureRecognizer::Result MPanRecognizerTouchPrivate::recognizeExistingGesture(
    MPanGestureTouch *panGesture, QTouchEvent *event)
{
    QGestureRecognizer::Result result = QGestureRecognizer::Ignore;

    const QTouchEvent::TouchPoint *touchPoint = fetchTouchPointById(event->touchPoints(), panGesture->touchPointId);

    if (touchPoint) {
        switch (touchPoint->state()) {
            case Qt::TouchPointPressed:
                mWarning("MPanRecognizerTouch") << "touch point pressed twice!?";
                // fall through...
            case Qt::TouchPointMoved:
                result = updateGesture(panGesture, touchPoint);
                break;
            case Qt::TouchPointStationary:
                // do nothing.
                break;
            case Qt::TouchPointReleased:
                updateGesture(panGesture, touchPoint);
                result = finishGesture(panGesture);
                break;
            default:
                mWarning("MPanRecognizerTouch") << "Unknown touch point state" << touchPoint->state();
        }
    } else {
        result = finishGesture(panGesture);
    }

    return result;
}

QGestureRecognizer::Result MPanRecognizerTouchPrivate::recognizeTouchEvent(
    MPanGestureTouch *panGesture, QTouchEvent *event)
{
    QGestureRecognizer::Result result = QGestureRecognizer::Ignore;

    if (panGesture->touchPointId == -1) {
        if (event->touchPointStates().testFlag(Qt::TouchPointPressed)) {
            const QTouchEvent::TouchPoint *pressedPoint = fetchPressedPoint(event->touchPoints());
            if (pressedPoint) {
                result = recognizePossibleNewGesture(panGesture, pressedPoint);
            }
        }
    } else {
        result = recognizeExistingGesture(panGesture, event);
    }

    return result;
}

MPanRecognizerTouch::MPanRecognizerTouch() :
        d_ptr(new MPanRecognizerTouchPrivate())
{
    d_ptr->q_ptr = this;

    Q_D(MPanRecognizerTouch);

    d->style = static_cast<const MPanRecognizerStyle *>(
                MTheme::style("MPanRecognizerStyle", ""));
}

MPanRecognizerTouch::~MPanRecognizerTouch()
{
    Q_D(MPanRecognizerTouch);

    MTheme::releaseStyle(d->style);
    delete d_ptr;
}

QGesture* MPanRecognizerTouch::create(QObject* target)
{
    if (target && target->isWidgetType() == false) {
        return 0; // this assumes the target is a QGraphicsObject, so we return
                  // NULL to indicate that the recognizer doesn't support graphicsobject and
                  // graphicsscene events.
    }
    return new MPanGestureTouch;
}

QGestureRecognizer::Result MPanRecognizerTouch::recognize(QGesture* gesture,
                                                          QObject* watched,
                                                          QEvent* event)
{
    Q_UNUSED(watched);
    Q_D(MPanRecognizerTouch);

    MPanGestureTouch *panGesture = static_cast<MPanGestureTouch*>(gesture);

    switch (event->type()) {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
            return d->recognizeTouchEvent(panGesture, static_cast<QTouchEvent *>(event));
        default:
            return QGestureRecognizer::Ignore;
    }
}

void MPanRecognizerTouch::reset(QGesture* gesture)
{
    MPanGestureTouch *panGesture = static_cast<MPanGestureTouch*>(gesture);
    panGesture->setOffset(QPointF());
    panGesture->setLastOffset(QPointF());
    panGesture->startScenePos = QPointF();
    panGesture->panDirection = 0;
    panGesture->touchPointId = -1;

    QGestureRecognizer::reset(gesture);
}
