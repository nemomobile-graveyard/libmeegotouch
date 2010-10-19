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

#include "mnamespace.h"
#include "mtheme.h"

#include <QEvent>
#include <QBasicTimer>
#include <QMouseEvent>
#include <QTouchEvent>

MTapAndHoldRecognizerPrivate::MTapAndHoldRecognizerPrivate()
  : style( 0 ),
    q_ptr( 0 )
{
}

MTapAndHoldRecognizerPrivate::~MTapAndHoldRecognizerPrivate()
{
}

MTapAndHoldRecognizer::MTapAndHoldRecognizer() :
        d_ptr(new MTapAndHoldRecognizerPrivate())
{
    d_ptr->q_ptr = this;

    Q_D(MTapAndHoldRecognizer);

    d->style = static_cast<const MTapAndHoldRecognizerStyle *>(MTheme::style("MTapAndHoldRecognizerStyle", ""));
}

MTapAndHoldRecognizer::~MTapAndHoldRecognizer()
{
    Q_D(MTapAndHoldRecognizer);

    MTheme::releaseStyle(d->style);
    delete d_ptr;
}

QGesture* MTapAndHoldRecognizer::create(QObject* target)
{
    if (target && target->isWidgetType() == false) {
        return 0; // this assumes the target is a QGraphicsObject, so we return
                  // NULL to indicate that the recognizer doesn't support graphicsobject and
                  // graphicsscene events.
    }

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

    const QMouseEvent *ev = static_cast<const QMouseEvent *>(event);
    const QTouchEvent *touchEvent = static_cast<const QTouchEvent *>(event);

    QGestureRecognizer::Result result = QGestureRecognizer::CancelGesture;

    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:

        if (tapAndHoldState->state() != Qt::NoGesture && touchEvent->touchPoints().count() > 1) {
            result = QGestureRecognizer::CancelGesture;
        } else {
            result = QGestureRecognizer::Ignore;
        }
        break;

    case QEvent::MouseButtonPress:
        tapAndHoldState->setPosition(ev->pos());
        tapAndHoldState->setHotSpot(ev->globalPos());

        if (tapAndHoldState->timerId)
            tapAndHoldState->killTimer(tapAndHoldState->timerId);
        tapAndHoldState->timerId = tapAndHoldState->startTimer(d->style->timeout());
        result = QGestureRecognizer::TriggerGesture;
        break;

    case QEvent::MouseButtonRelease:
        if (tapAndHoldState->timerId) {
            tapAndHoldState->killTimer(tapAndHoldState->timerId);
            tapAndHoldState->timerId = 0;
            result = QGestureRecognizer::CancelGesture;
        } else {
            result = QGestureRecognizer::Ignore;
        }

        break;

    case QEvent::MouseMove:
        if (tapAndHoldState->state() != Qt::NoGesture) {
            QPoint delta = ev->pos() - tapAndHoldState->position().toPoint();
            if (delta.manhattanLength() <= d->style->movementThreshold())
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
