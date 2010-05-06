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

#include "ut_mtapandholdrecognizer.h"
#include "mtapandholdrecognizer.h"
#include "mtapandholdrecognizer_p.h"

#include "mtapandholdgesture_p.h"

#include <QGraphicsSceneMouseEvent>
#include <QtTest/QtTest>

void Ut_MTapAndHoldRecognizer::init()
{
    recognizer = new MTapAndHoldRecognizer();
}

void Ut_MTapAndHoldRecognizer::cleanup()
{
    delete recognizer;
}

void Ut_MTapAndHoldRecognizer::testCreateGesture()
{
    tapAndHoldGesture = static_cast<MTapAndHoldGesture*>(recognizer->create(this));
}

void Ut_MTapAndHoldRecognizer::testRecognize()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(0,0));
    pressEvent.setScenePos(QPointF(0,0));
    pressEvent.setScreenPos(QPoint(0,0));

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(tapAndHoldGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    QTimerEvent fakeTimerEvent(tapAndHoldGesture->timerId);
    currentState = recognizer->recognize(tapAndHoldGesture, tapAndHoldGesture, &fakeTimerEvent);
    QCOMPARE( currentState, QGestureRecognizer::FinishGesture | QGestureRecognizer::ConsumeEventHint );
}

void Ut_MTapAndHoldRecognizer::testFastTap()
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
    currentState = recognizer->recognize(tapAndHoldGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    //Setting timerId as would happen in real life scenario
    tapAndHoldGesture->timerId = 1;

    currentState = recognizer->recognize(tapAndHoldGesture, 0, &releaseEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);
}

QTEST_APPLESS_MAIN(Ut_MTapAndHoldRecognizer)

