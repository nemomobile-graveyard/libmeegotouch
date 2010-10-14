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

#include "mswiperecognizer.h"
#include "mswiperecognizer_p.h"

#include "mswipegesture_p.h"

#include "mnamespace.h"
#include "mtheme.h"

#include <QEvent>
#include <QBasicTimer>
#include <QGraphicsSceneMouseEvent>
#include <QTouchEvent>
#include <QLineF>

MSwipeRecognizerPrivate::MSwipeRecognizerPrivate()
  : style( 0 ),
    q_ptr( 0 )
{}

MSwipeRecognizerPrivate::~MSwipeRecognizerPrivate()
{}

qreal MSwipeRecognizerPrivate::vectorLength(const QPointF &vector)
{
    return 0.7 *(vector.x()*vector.x() + vector.y()*vector.y());
}

bool MSwipeRecognizerPrivate::isAngleDeltaBelowThreshold(qreal angleDelta, qreal threshold)
{
    if (angleDelta < 0)
        angleDelta += 360;

    return angleDelta < threshold || (360 - angleDelta) < threshold;
}

void MSwipeRecognizerPrivate::snapToRightAngle(MSwipeGesture *swipeGesture)
{
    if (isAngleDeltaBelowThreshold(0 - swipeGesture->swipeAngle(), style->angleSnappingThreshold()))
        swipeGesture->setSwipeAngle(0);

    else if (isAngleDeltaBelowThreshold(90 - swipeGesture->swipeAngle(), style->angleSnappingThreshold()))
        swipeGesture->setSwipeAngle(90);

    else if (isAngleDeltaBelowThreshold(180 - swipeGesture->swipeAngle(), style->angleSnappingThreshold()))
        swipeGesture->setSwipeAngle(180);

    else if (isAngleDeltaBelowThreshold(270 - swipeGesture->swipeAngle(), style->angleSnappingThreshold()))
        swipeGesture->setSwipeAngle(270);
}

QGestureRecognizer::Result MSwipeRecognizerPrivate::startRecognition(MSwipeGesture *swipeGesture, const QGraphicsSceneMouseEvent *mouseEvent)
{
    swipeGesture->time = QTime::currentTime();
    swipeGesture->startPosition = mouseEvent->scenePos();
    swipeGesture->setHotSpot(mouseEvent->screenPos());

    return QGestureRecognizer::MayBeGesture;
}

QGestureRecognizer::Result MSwipeRecognizerPrivate::updateRecognition(MSwipeGesture *swipeGesture, const QGraphicsSceneMouseEvent *mouseEvent)
{
    QGestureRecognizer::Result result = QGestureRecognizer::CancelGesture;
    qreal currentDistance = vectorLength(mouseEvent->scenePos() - swipeGesture->startPosition);

    //Swipe angle is equal to the angle of a line between starting position and current position.
    swipeGesture->setSwipeAngle(QLineF(swipeGesture->startPosition, mouseEvent->scenePos()).angle());
    int elapsedTime = swipeGesture->time.msecsTo(QTime::currentTime());

    if (swipeGesture->state() != Qt::NoGesture) {
        //The gesture has already been recognized, check if the user didn't change direction.
        qreal angleDelta = swipeGesture->recognizedAngle - swipeGesture->swipeAngle();

        if ( isAngleDeltaBelowThreshold(angleDelta, style->angleThreshold()) &&
             currentDistance + style->jitterThreshold() > swipeGesture->prevDistance )
            result = QGestureRecognizer::TriggerGesture;

        swipeGesture->prevDistance = currentDistance;

    } else if (elapsedTime > style->timeout()) {
        // Timeout! This movement is too slow to be a swipe;
        result = QGestureRecognizer::CancelGesture;

    } else if (currentDistance > style->distanceThreshold()) {
        swipeGesture->recognizedAngle = swipeGesture->swipeAngle();
        swipeGesture->prevDistance = currentDistance;
        result = QGestureRecognizer::TriggerGesture;

    } else {
        //The gesture was not yet recognized, keep checking.
        result = QGestureRecognizer::MayBeGesture;
    }

    snapToRightAngle(swipeGesture);

    return result;
}

QGestureRecognizer::Result MSwipeRecognizerPrivate::finishRecognition(MSwipeGesture *swipeGesture)
{
    return swipeGesture->state() != Qt::NoGesture ? QGestureRecognizer::FinishGesture : QGestureRecognizer::CancelGesture;
}

MSwipeRecognizer::MSwipeRecognizer() :
        d_ptr(new MSwipeRecognizerPrivate())
{
    d_ptr->q_ptr = this;

    Q_D(MSwipeRecognizer);

    d->style = static_cast<const MSwipeRecognizerStyle *>(MTheme::style("MSwipeRecognizerStyle", ""));
}

MSwipeRecognizer::~MSwipeRecognizer()
{
    Q_D(MSwipeRecognizer);

    MTheme::releaseStyle(d->style);
    delete d_ptr;
}

QGesture* MSwipeRecognizer::create(QObject* /*target*/)
{
    return new MSwipeGesture();
}

QGestureRecognizer::Result MSwipeRecognizer::recognize(QGesture *state, QObject */*watched*/, QEvent *event)
{
    Q_D(MSwipeRecognizer);

    const QGraphicsSceneMouseEvent *mouseEvent = static_cast<const QGraphicsSceneMouseEvent*>(event);
    const QTouchEvent *touchEvent = static_cast<const QTouchEvent *>(event);
    QGestureRecognizer::Result result;

    MSwipeGesture *swipeGesture = static_cast<MSwipeGesture *>(state);

    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:

        // The swipe gesture is a one finger gesture. If we receive information that
        // there are two or more fingers pressed, we need to cancel our gesture.
        result = swipeGesture->state() != Qt::NoGesture && touchEvent->touchPoints().count() > 1 ? CancelGesture : Ignore;
        break;

    case QEvent::GraphicsSceneMousePress:
        result = d->startRecognition(swipeGesture, mouseEvent);
        break;

    case QEvent::GraphicsSceneMouseMove:
        result = d->updateRecognition(swipeGesture, mouseEvent);
        break;

    case QEvent::GraphicsSceneMouseRelease:
        result = d->finishRecognition(swipeGesture);
        break;

    default:
        result = QGestureRecognizer::Ignore;
        break;
    }
    return result;
}

void MSwipeRecognizer::reset(QGesture* state)
{
    MSwipeGesture *swipeGesture = static_cast<MSwipeGesture *>(state);

    swipeGesture->setSwipeAngle(0);
    swipeGesture->recognizedAngle = 0;
    swipeGesture->prevDistance = 0;
    swipeGesture->startPosition = QPointF();
    swipeGesture->time = QTime();

    QGestureRecognizer::reset(swipeGesture);
}
