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

#include <QEvent>
#include <QBasicTimer>
#include <QGraphicsSceneMouseEvent>
#include <QTouchEvent>
#include <QLineF>

/* Recognizer default settings */
static const int MSwipeTimeout = 300; /* miliseconds */
static const int MSwipeMovementThreshold = 50; /* pixels */
static const qreal MSwipeAngleThreshold = 15; /* degrees */

MSwipeRecognizerPrivate::MSwipeRecognizerPrivate()
  : q_ptr( 0 )
{
}

MSwipeRecognizerPrivate::~MSwipeRecognizerPrivate()
{
}

MSwipeRecognizer::MSwipeRecognizer() :
        d_ptr(new MSwipeRecognizerPrivate())
{
    d_ptr->q_ptr = this;
}

MSwipeRecognizer::~MSwipeRecognizer()
{
    delete d_ptr;
}

QGesture* MSwipeRecognizer::create(QObject* target)
{
    Q_UNUSED(target);

    return new MSwipeGesture();
}

QGestureRecognizer::Result MSwipeRecognizer::recognize(QGesture *state, QObject */*watched*/, QEvent *event)
{

    MSwipeGesture *swipeGesture = static_cast<MSwipeGesture *>(state);
    const QGraphicsSceneMouseEvent *mouseEvent = static_cast<const QGraphicsSceneMouseEvent*>(event);
    const QTouchEvent *touchEvent = static_cast<const QTouchEvent *>(event);
    QGestureRecognizer::Result result;

    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:

        if (swipeGesture->state() != Qt::NoGesture && touchEvent->touchPoints().count() > 1) {
            result = QGestureRecognizer::CancelGesture;
        } else {
            result = QGestureRecognizer::Ignore;
        }
        break;

    case QEvent::GraphicsSceneMousePress:
        swipeGesture->time = QTime::currentTime();
        swipeGesture->startPosition = mouseEvent->scenePos();
        swipeGesture->setHotSpot(mouseEvent->screenPos());

        result = QGestureRecognizer::MayBeGesture;
        break;

    case QEvent::GraphicsSceneMouseMove: {
        int xDistance = qAbs(mouseEvent->scenePos().x() - swipeGesture->startPosition.x());
        int yDistance = qAbs(mouseEvent->scenePos().y() - swipeGesture->startPosition.y());

        swipeGesture->setSwipeAngle(QLineF(swipeGesture->startPosition, mouseEvent->scenePos()).angle());
        int elapsedTime = swipeGesture->time.msecsTo(QTime::currentTime());

        if (swipeGesture->state() == Qt::NoGesture && elapsedTime > MSwipeTimeout) {

            //Timeout! This movement is too slow to be a swipe;
            result = QGestureRecognizer::CancelGesture;

        } else if (swipeGesture->state() == Qt::NoGesture &&
                   (xDistance > MSwipeMovementThreshold || yDistance > MSwipeMovementThreshold)) {

            //The gesture has just been recognized, store the current angle to validate with further events.
            swipeGesture->recognizedAngle = QLineF(swipeGesture->startPosition, mouseEvent->scenePos()).angle();
            QPointF gestureVector = mouseEvent->scenePos() - swipeGesture->startPosition;
            swipeGesture->prevDistance = 0.7 *(gestureVector.x()*gestureVector.x() + gestureVector.y()*gestureVector.y());
            result = QGestureRecognizer::TriggerGesture;

        } else if (swipeGesture->state() != Qt::NoGesture) {

            //The gesture was already recognized, check if the user didn't zigzagged.
            qreal angleDelta = swipeGesture->recognizedAngle - swipeGesture->swipeAngle();
            if (angleDelta < 0)
                angleDelta += 360;

            QPointF gestureVector = mouseEvent->scenePos() - swipeGesture->startPosition;
            qreal currentDistance = 0.7 *(gestureVector.x()*gestureVector.x() + gestureVector.y()*gestureVector.y());

            if ( (angleDelta < MSwipeAngleThreshold || (360 - angleDelta ) < MSwipeAngleThreshold) &&
                  currentDistance > swipeGesture->prevDistance )
                result = QGestureRecognizer::TriggerGesture;
            else
                result = QGestureRecognizer::CancelGesture;

            swipeGesture->prevDistance = currentDistance;


        } else {
            //The gesture was not yet recognized, keep checking.
            result = QGestureRecognizer::MayBeGesture;
        }

        break;
    }

    case QEvent::GraphicsSceneMouseRelease: {
        if (swipeGesture->state() != Qt::NoGesture) {
            result = QGestureRecognizer::FinishGesture;
        } else {
            result = QGestureRecognizer::CancelGesture;
        }
        break;
    }
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
