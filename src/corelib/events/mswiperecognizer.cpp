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
{
}

MSwipeRecognizerPrivate::~MSwipeRecognizerPrivate()
{
}

bool MSwipeRecognizerPrivate::isAngleDeltaBelowThreshold(qreal angleDelta, qreal threshold)
{
   if (angleDelta < 0)
        angleDelta += 360;

    if (angleDelta < threshold || (360 - angleDelta ) < threshold)
        return true;

    return false;
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

QGesture* MSwipeRecognizer::create(QObject* target)
{
    Q_UNUSED(target);

    return new MSwipeGesture();
}

QGestureRecognizer::Result MSwipeRecognizer::recognize(QGesture *state, QObject */*watched*/, QEvent *event)
{
    Q_D(MSwipeRecognizer);

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

        if (swipeGesture->state() == Qt::NoGesture && elapsedTime > d->style->timeout()) {

            //Timeout! This movement is too slow to be a swipe;
            result = QGestureRecognizer::CancelGesture;

        } else if (swipeGesture->state() == Qt::NoGesture &&
                   (xDistance > d->style->distanceThreshold() || yDistance > d->style->distanceThreshold())) {

            //The gesture has just been recognized, store the current angle to validate with further events.
            swipeGesture->recognizedAngle = QLineF(swipeGesture->startPosition, mouseEvent->scenePos()).angle();
            QPointF gestureVector = mouseEvent->scenePos() - swipeGesture->startPosition;
            swipeGesture->prevDistance = 0.7 *(gestureVector.x()*gestureVector.x() + gestureVector.y()*gestureVector.y());
            result = QGestureRecognizer::TriggerGesture;

        } else if (swipeGesture->state() != Qt::NoGesture) {

            //The gesture was already recognized, check if the user didn't zigzagged.
            qreal angleDelta = swipeGesture->recognizedAngle - swipeGesture->swipeAngle();

            QPointF gestureVector = mouseEvent->scenePos() - swipeGesture->startPosition;
            qreal currentDistance = 0.7 *(gestureVector.x()*gestureVector.x() + gestureVector.y()*gestureVector.y());

            if ( d->isAngleDeltaBelowThreshold(angleDelta, d->style->angleThreshold()) && currentDistance > swipeGesture->prevDistance )
                result = QGestureRecognizer::TriggerGesture;
            else
                result = QGestureRecognizer::CancelGesture;

            swipeGesture->prevDistance = currentDistance;


        } else {
            //The gesture was not yet recognized, keep checking.
            result = QGestureRecognizer::MayBeGesture;
        }

        //Snapping to 0,90,180,270 degrees:
        if (d->isAngleDeltaBelowThreshold(0 - swipeGesture->swipeAngle(), d->style->angleSnappingThreshold()))
            swipeGesture->setSwipeAngle(0);
        else if (d->isAngleDeltaBelowThreshold(90 - swipeGesture->swipeAngle(), d->style->angleSnappingThreshold()))
            swipeGesture->setSwipeAngle(90);
        else if (d->isAngleDeltaBelowThreshold(180 - swipeGesture->swipeAngle(), d->style->angleSnappingThreshold()))
            swipeGesture->setSwipeAngle(180);
        else if (d->isAngleDeltaBelowThreshold(270 - swipeGesture->swipeAngle(), d->style->angleSnappingThreshold()))
            swipeGesture->setSwipeAngle(270);

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
