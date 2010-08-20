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

#include "ut_mswiperecognizer.h"
#include "mswiperecognizer.h"
#include "mswiperecognizer_p.h"

#include "mswipegesture_p.h"

#include <QGraphicsSceneMouseEvent>
#include <QtTest/QtTest>


Qt::GestureState currentGestureState = Qt::NoGesture;
Qt::GestureState QGesture::state() const
{
    return currentGestureState;
}

void Ut_MSwipeRecognizer::init()
{
    recognizer = new MSwipeRecognizer();
}

void Ut_MSwipeRecognizer::cleanup()
{
    delete recognizer;
}

void Ut_MSwipeRecognizer::testCreateGesture()
{
    swipeGesture = static_cast<MSwipeGesture*>(recognizer->create(this));
}

void Ut_MSwipeRecognizer::testRecognize()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(0,0));
    pressEvent.setScenePos(QPointF(0,0));
    pressEvent.setScreenPos(QPoint(0,0));

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setPos(QPointF(55,0));
    moveEvent.setScenePos(QPointF(55,0));
    moveEvent.setScreenPos(QPoint(55,0));

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

}

void Ut_MSwipeRecognizer::testFastTap()
{

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(0,0));
    pressEvent.setScenePos(QPointF(0,0));
    pressEvent.setScreenPos(QPoint(0,0));

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setPos(QPointF(0,0));
    releaseEvent.setScenePos(QPointF(0,0));
    releaseEvent.setScreenPos(QPoint(0,0));

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &releaseEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);
}

void Ut_MSwipeRecognizer::testTimedout()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(0,0));
    pressEvent.setScenePos(QPointF(0,0));
    pressEvent.setScreenPos(QPoint(0,0));

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setPos(QPointF(30,0));
    moveEvent.setScenePos(QPointF(30,0));
    moveEvent.setScreenPos(QPoint(30,0));

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    swipeGesture->time = QTime().currentTime().addSecs(-3);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);

}

void Ut_MSwipeRecognizer::testZigzagged()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(0,0));
    pressEvent.setScenePos(QPointF(0,0));
    pressEvent.setScreenPos(QPoint(0,0));

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setPos(QPointF(55,0));
    moveEvent.setScenePos(QPointF(55,0));
    moveEvent.setScreenPos(QPoint(55,0));

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    moveEvent.setPos(QPointF(-100,0));
    moveEvent.setScenePos(QPointF(-100,0));
    moveEvent.setScreenPos(QPoint(-100,0));

    currentGestureState = Qt::GestureStarted;

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);

}

void Ut_MSwipeRecognizer::testSnappingToRightAngles()
{

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(0,0));
    pressEvent.setScenePos(QPointF(0,0));
    pressEvent.setScreenPos(QPoint(0,0));

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setPos(QPointF(100,10));
    moveEvent.setScenePos(QPointF(100,10));
    moveEvent.setScreenPos(QPoint(100,10));

    currentGestureState = Qt::NoGesture;

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE( swipeGesture->swipeAngle(), 0.0);

    recognizer->reset(swipeGesture);

    currentGestureState = Qt::NoGesture;

    moveEvent.setPos(QPointF(10,-100));
    moveEvent.setScenePos(QPointF(10,-100));
    moveEvent.setScreenPos(QPoint(10,-100));

    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE( swipeGesture->swipeAngle(), 90.0);

    recognizer->reset(swipeGesture);

    currentGestureState = Qt::NoGesture;

    moveEvent.setPos(QPointF(-100,-10));
    moveEvent.setScenePos(QPointF(-100,-10));
    moveEvent.setScreenPos(QPoint(-100,-10));

    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE( swipeGesture->swipeAngle(), 180.0);

    recognizer->reset(swipeGesture);

    currentGestureState = Qt::NoGesture;

    moveEvent.setPos(QPointF(10,100));
    moveEvent.setScenePos(QPointF(10,100));
    moveEvent.setScreenPos(QPoint(10,100));

    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE( swipeGesture->swipeAngle(), 270.0);
}

QTEST_APPLESS_MAIN(Ut_MSwipeRecognizer)

