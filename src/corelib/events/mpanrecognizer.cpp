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

#include <QGraphicsSceneMouseEvent>

//Hardcoded movement threshold used to recognize the panning gesture.
static const int defaultPanThreshold = 10;

MPanRecognizerPrivate::MPanRecognizerPrivate()
  : movementThreshold( 0 ),
    q_ptr( 0 )
{
}

MPanRecognizerPrivate::~MPanRecognizerPrivate()
{
}

MPanRecognizer::MPanRecognizer() :
        d_ptr(new MPanRecognizerPrivate())
{
    Q_D(MPanRecognizer);
    d->movementThreshold = defaultPanThreshold;
}

MPanRecognizer::~MPanRecognizer()
{
    delete d_ptr;
}

QGesture* MPanRecognizer::create(QObject* target)
{
    Q_UNUSED(target);
    return new MPanGesture;
}

QGestureRecognizer::Result MPanRecognizer::recognize(  QGesture* gesture,
                                                         QObject* watched,
                                                         QEvent* event)
{
    Q_UNUSED(watched);
    Q_D(MPanRecognizer);

    MPanGesture *panGesture = static_cast<MPanGesture*>(gesture);
    const QGraphicsSceneMouseEvent *ev = static_cast<const QGraphicsSceneMouseEvent *>(event);
    QGestureRecognizer::Result result = QGestureRecognizer::CancelGesture;
    qreal distX, distY;

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress:

        panGesture->startPos = ev->scenePos();
        panGesture->setHotSpot(ev->screenPos());

        if (panGesture->state() != Qt::NoGesture) {
            result = QGestureRecognizer::TriggerGesture;
        } else {
            result = QGestureRecognizer::MayBeGesture;
        }
        
        break;

    case QEvent::GraphicsSceneMouseRelease:
        if (panGesture->state() != Qt::NoGesture) {
            result = QGestureRecognizer::FinishGesture;
        } else {
            result = QGestureRecognizer::CancelGesture;
        }

        break;

    case QEvent::GraphicsSceneMouseMove:

        panGesture->setLastOffset(panGesture->offset());
        panGesture->setOffset(ev->scenePos() - panGesture->startPos);

        distX = abs(panGesture->offset().x());
        distY = abs(panGesture->offset().y());

        if (panGesture->state() == Qt::NoGesture) {

            if (distX > d->movementThreshold || distY > d->movementThreshold) {
                panGesture->panDirection = (distX > distY ? Qt::Horizontal : Qt::Vertical);
                result = QGestureRecognizer::TriggerGesture;
            } else {
                result = QGestureRecognizer::MayBeGesture;
            }

        } else {
            result = QGestureRecognizer::TriggerGesture;
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

    QGestureRecognizer::reset(gesture);
}
