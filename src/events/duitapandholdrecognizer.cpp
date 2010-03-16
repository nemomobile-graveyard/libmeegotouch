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

#include "duitapandholdrecognizer.h"
#include "duitapandholdrecognizer_p.h"

#include "duitapandholdgesture_p.h"

#include <QEvent>
#include <QBasicTimer>
#include <QGraphicsSceneMouseEvent>

/* Recognizer default settings */
static const int DuiTapAndHoldTimeout = 500; /* miliseconds */
static const int DuiTapAndHoldMovementThreshold = 20; /* pixels */

DuiTapAndHoldRecognizerPrivate::DuiTapAndHoldRecognizerPrivate()
{
}

DuiTapAndHoldRecognizerPrivate::~DuiTapAndHoldRecognizerPrivate()
{
}

DuiTapAndHoldRecognizer::DuiTapAndHoldRecognizer() :
        d_ptr(new DuiTapAndHoldRecognizerPrivate())
{
    Q_D(DuiTapAndHoldRecognizer);

    d->timeoutValue = DuiTapAndHoldTimeout;
    d->movementThreshold = DuiTapAndHoldMovementThreshold;
}

DuiTapAndHoldRecognizer::~DuiTapAndHoldRecognizer()
{
    delete d_ptr;
}

QGesture* DuiTapAndHoldRecognizer::create(QObject* target)
{
    Q_UNUSED(target);

    return new DuiTapAndHoldGesture();
}

QGestureRecognizer::Result DuiTapAndHoldRecognizer::recognize(QGesture *state, QObject *watched, QEvent *event)
{
    Q_D(DuiTapAndHoldRecognizer);

    DuiTapAndHoldGesture *tapAndHoldState = static_cast<DuiTapAndHoldGesture*>(state);

    if (watched == state && event->type() == QEvent::Timer) {
        tapAndHoldState->killTimer(tapAndHoldState->timerId);
        tapAndHoldState->timerId = 0;
        return QGestureRecognizer::FinishGesture | QGestureRecognizer::ConsumeEventHint;
    }

    const QGraphicsSceneMouseEvent *ev = static_cast<const QGraphicsSceneMouseEvent *>(event);

    QGestureRecognizer::Result result = QGestureRecognizer::CancelGesture;

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress:
        tapAndHoldState->setPosition(ev->pos());
        tapAndHoldState->setHotSpot(ev->screenPos());

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
            QPoint delta = ev->pos().toPoint() - tapAndHoldState->position().toPoint();
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

void DuiTapAndHoldRecognizer::reset(QGesture* gesture)
{
    DuiTapAndHoldGesture *tapAndHoldState = static_cast<DuiTapAndHoldGesture*>(gesture);

    tapAndHoldState->setPosition(QPointF());

    if (tapAndHoldState->timerId)
        tapAndHoldState->killTimer(tapAndHoldState->timerId);
    tapAndHoldState->timerId = 0;

    QGestureRecognizer::reset(gesture);
}
