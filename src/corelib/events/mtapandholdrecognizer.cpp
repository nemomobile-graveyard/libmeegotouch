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

#include "mtapandholdrecognizer.h"
#include "mtapandholdrecognizer_p.h"

#include "mtapandholdgesture_p.h"

#include <QEvent>
#include <QBasicTimer>
#include <QGraphicsSceneMouseEvent>

/* Recognizer default settings */
static const int MTapAndHoldTimeout = 500; /* miliseconds */
static const int MTapAndHoldMovementThreshold = 20; /* pixels */

MTapAndHoldRecognizerPrivate::MTapAndHoldRecognizerPrivate()
{
}

MTapAndHoldRecognizerPrivate::~MTapAndHoldRecognizerPrivate()
{
}

MTapAndHoldRecognizer::MTapAndHoldRecognizer() :
        d_ptr(new MTapAndHoldRecognizerPrivate())
{
    Q_D(MTapAndHoldRecognizer);

    d->timeoutValue = MTapAndHoldTimeout;
    d->movementThreshold = MTapAndHoldMovementThreshold;
}

MTapAndHoldRecognizer::~MTapAndHoldRecognizer()
{
    delete d_ptr;
}

QGesture* MTapAndHoldRecognizer::create(QObject* target)
{
    Q_UNUSED(target);

    return new MTapAndHoldGesture();
}

QGestureRecognizer::Result MTapAndHoldRecognizer::recognize(QGesture *state, QObject *watched, QEvent *event)
{
    Q_D(MTapAndHoldRecognizer);

    MTapAndHoldGesture *tapAndHoldState = static_cast<MTapAndHoldGesture*>(state);

    if (watched == state && event->type() == QEvent::Timer) {
        tapAndHoldState->killTimer(tapAndHoldState->timerId);
        tapAndHoldState->timerId = 0;
        return QGestureRecognizer::FinishGesture | QGestureRecognizer::ConsumeEventHint;
    }

    const QGraphicsSceneMouseEvent *ev = static_cast<const QGraphicsSceneMouseEvent *>(event);

    QGestureRecognizer::Result result = QGestureRecognizer::CancelGesture;

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress:
        tapAndHoldState->setPosition(ev->scenePos());
        tapAndHoldState->setHotSpot(ev->scenePos());

        if (tapAndHoldState->timerId)
            tapAndHoldState->killTimer(tapAndHoldState->timerId);
        tapAndHoldState->timerId = tapAndHoldState->startTimer(d->timeoutValue);
        result = QGestureRecognizer::TriggerGesture;
        break;
    case QEvent::GraphicsSceneMouseRelease:

        if (tapAndHoldState->timerId) {
            tapAndHoldState->killTimer(tapAndHoldState->timerId);
            tapAndHoldState->timerId = 0;
            result = QGestureRecognizer::CancelGesture;
        } else {
            result = QGestureRecognizer::Ignore;
        }

        break;
    case QEvent::GraphicsSceneMouseMove:
        if (tapAndHoldState->state() != Qt::NoGesture) {
            QPoint delta = ev->scenePos().toPoint() - tapAndHoldState->position().toPoint();
            if (delta.manhattanLength() <= d->movementThreshold)
                result = QGestureRecognizer::TriggerGesture;
        }
        break;
    default:
        result = QGestureRecognizer::Ignore;
        break;
    }
    return result;
}

void MTapAndHoldRecognizer::reset(QGesture* gesture)
{
    MTapAndHoldGesture *tapAndHoldState = static_cast<MTapAndHoldGesture*>(gesture);

    tapAndHoldState->setPosition(QPointF());

    if (tapAndHoldState->timerId)
        tapAndHoldState->killTimer(tapAndHoldState->timerId);
    tapAndHoldState->timerId = 0;

    QGestureRecognizer::reset(gesture);
}
