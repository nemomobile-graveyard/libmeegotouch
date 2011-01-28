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

#include "mpanrecognizer.h"
#include "mpanrecognizer_p.h"

#include "mpangesture_p.h"

#include "mnamespace.h"
#include "mtheme.h"

#include <QGraphicsSceneMouseEvent>
#include <QTouchEvent>

MPanRecognizerPrivate::MPanRecognizerPrivate()
  : style( 0 ),
    q_ptr( 0 )
{
}

MPanRecognizerPrivate::~MPanRecognizerPrivate()
{
}

MPanRecognizer::MPanRecognizer() :
        d_ptr(new MPanRecognizerPrivate())
{
    d_ptr->q_ptr = this;

    Q_D(MPanRecognizer);

    d->style = static_cast<const MPanRecognizerStyle *>(MTheme::style("MPanRecognizerStyle", ""));
}

MPanRecognizer::~MPanRecognizer()
{
    Q_D(MPanRecognizer);

    MTheme::releaseStyle(d->style);
    delete d_ptr;
}

QGesture* MPanRecognizer::create(QObject* target)
{
    if (target && target->isWidgetType() == false) {
        return 0; // this assumes the target is a QGraphicsObject, so we return
                  // NULL to indicate that the recognizer doesn't support graphicsobject and
                  // graphicsscene events.
    }
    return new MPanGesture;
}

QGestureRecognizer::Result MPanRecognizer::recognize(  QGesture* gesture,
                                                         QObject* watched,
                                                         QEvent* event)
{
    Q_UNUSED(watched);
    Q_D(MPanRecognizer);

    MPanGesture *panGesture = static_cast<MPanGesture*>(gesture);
    const QMouseEvent *ev = static_cast<const QMouseEvent *>(event);
    const QTouchEvent *touchEvent = static_cast<const QTouchEvent *>(event);
    QGestureRecognizer::Result result = QGestureRecognizer::CancelGesture;
    qreal distX, distY;

    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:

        if (panGesture->state() != Qt::NoGesture && touchEvent->touchPoints().count() > 1) {
            result = QGestureRecognizer::CancelGesture;
        } else {
            result = QGestureRecognizer::Ignore;
        }
        break;

    case QEvent::MouseButtonPress:
        panGesture->startPos = ev->pos();
        panGesture->setHotSpot(ev->globalPos());
        panGesture->pressed = true;

        if (panGesture->state() != Qt::NoGesture) {
            result = QGestureRecognizer::TriggerGesture;
        } else {
            result = QGestureRecognizer::MayBeGesture;
        }
        break;

    case QEvent::MouseButtonRelease:
        panGesture->pressed = false;
        if (panGesture->state() != Qt::NoGesture) {
            result = QGestureRecognizer::FinishGesture;
        } else {
            result = QGestureRecognizer::CancelGesture;
        }
        break;

    case QEvent::MouseMove:

        if (panGesture->pressed == false) {
            //Stray mouse move received;
            result = QGestureRecognizer::Ignore;
            break;
        }

        panGesture->setLastOffset(panGesture->offset());
        panGesture->setOffset(ev->pos() - panGesture->startPos);

        distX = abs(panGesture->offset().x());
        distY = abs(panGesture->offset().y());

        if (panGesture->state() != Qt::NoGesture) {
            result = QGestureRecognizer::TriggerGesture;
        } else if (distX > d->style->distanceThreshold() ||
                   distY > d->style->distanceThreshold()) {
            panGesture->panDirection = (distX > distY ? Qt::Horizontal : Qt::Vertical);
            panGesture->startPos = ev->pos();
            panGesture->setLastOffset(QPointF());
            panGesture->setOffset(QPointF());
            result = QGestureRecognizer::TriggerGesture;
        } else {
            result = QGestureRecognizer::MayBeGesture;
        }

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

        break;

    default:
        result = QGestureRecognizer::Ignore;
        break;
    }
    return result;
}

void MPanRecognizer::reset(QGesture* gesture)
{
    MPanGesture *panGesture = static_cast<MPanGesture*>(gesture);
    panGesture->setOffset(QPointF());
    panGesture->setLastOffset(QPointF());
    panGesture->startPos = QPointF();
    panGesture->panDirection = 0;
    panGesture->pressed = false;

    QGestureRecognizer::reset(gesture);
}
