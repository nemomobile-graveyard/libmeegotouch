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

#include "ut_mpanrecognizer.h"
#include "mpanrecognizer.h"
#include "mpanrecognizer_p.h"

#include "mpangesture_p.h"

#include <QGraphicsSceneMouseEvent>
#include <QtTest/QtTest>

// QGesture stubs:
Qt::GestureState currentGestureState = Qt::NoGesture;
Qt::GestureState QGesture::state() const
{
    return currentGestureState;
}


void Ut_MPanRecognizer::init()
{
    currentGestureState = Qt::NoGesture;
    recognizer = new MPanRecognizer();
}

void Ut_MPanRecognizer::cleanup()
{
    delete recognizer;
}

void Ut_MPanRecognizer::testCreateGesture()
{
    PanGesture = static_cast<MPanGesture*>(recognizer->create(this));
}

void Ut_MPanRecognizer::testRecognize()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(0,0));
    pressEvent.setScenePos(QPointF(0,0));
    pressEvent.setScreenPos(QPoint(0,0));

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setPos(QPointF(0,100));
    moveEvent.setScenePos(QPointF(0,100));
    moveEvent.setScreenPos(QPoint(0,100));

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setPos(QPointF(0,100));
    releaseEvent.setScenePos(QPointF(0,100));
    releaseEvent.setScreenPos(QPoint(0,100));

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(PanGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(PanGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    //Artificially setting state of QGesture object.
    currentGestureState = Qt::GestureUpdated;

    currentState = recognizer->recognize(PanGesture, 0, &releaseEvent);
    QCOMPARE( currentState, QGestureRecognizer::FinishGesture);
}

void Ut_MPanRecognizer::testTapIsNotRecognizedAsPan()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(0,0));
    pressEvent.setScenePos(QPointF(0,0));
    pressEvent.setScreenPos(QPoint(0,0));

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    pressEvent.setPos(QPointF(0,0));
    pressEvent.setScenePos(QPointF(0,0));
    pressEvent.setScreenPos(QPoint(0,0));

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(PanGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(PanGesture, 0, &releaseEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);
}

QTEST_APPLESS_MAIN(Ut_MPanRecognizer)

