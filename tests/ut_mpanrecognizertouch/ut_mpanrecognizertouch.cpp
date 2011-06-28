/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ut_mpanrecognizertouch.h"

#include <mpanrecognizertouch.h>
#include <mpanrecognizertouch_p.h>
#include <mpangesturetouch.h>
#include <mpanrecognizerstyle.h>

#include <QWidget>
#include <QTouchEvent>
#include <QtTest/QtTest>

#include <MApplication>

//Mocking MThemePrivate, because the style object is private to library
//and we need to compile some functionality into the unittest.
#include <mtheme_p.h>
void MThemePrivate::unregisterStyleContainer(MStyleContainer*)
{
}

void MThemePrivate::registerStyleContainer(MStyleContainer *)
{
}

#include <mtheme.h>
//Filling the values of the style object.
static const int defaultPanThreshold = 10;

MPanRecognizerStyle recognizerStyle;
const MStyle *MTheme::style(const char *,
                            const QString &) {
    recognizerStyle.setDistanceThreshold(defaultPanThreshold);
    return &recognizerStyle;
}

void MTheme::releaseStyle(const MStyle *)
{
}

// This methods mocks the behavior of QGesture class in order
// to set state of QGesture according to the needs of the unittest.
Qt::GestureState currentGestureState = Qt::NoGesture;
Qt::GestureState QGesture::state() const
{
    return currentGestureState;
}

MApplication *app;

Ut_MPanRecognizerTouch::Ut_MPanRecognizerTouch()
    : QObject(),
      recognizer(0)
{
}

void Ut_MPanRecognizerTouch::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mpanrecognizertouch" };
    app = new MApplication(argc, app_name);
}

void Ut_MPanRecognizerTouch::cleanupTestCase()
{
    delete app;
}

void Ut_MPanRecognizerTouch::init()
{
    currentGestureState = Qt::NoGesture;
    recognizer = new MPanRecognizerTouch;
}

void Ut_MPanRecognizerTouch::cleanup()
{
    delete recognizer;
    recognizer = 0;
}

void Ut_MPanRecognizerTouch::testSimplePanGestureRecognition()
{
    QList<QTouchEvent::TouchPoint> touchPoints;
    QTouchEvent::TouchPoint touchPoint;
    MPanGestureTouch *panGesture = 0;

    QGestureRecognizer::Result currentState;

    panGesture = static_cast<MPanGestureTouch*>(recognizer->create(0));
    QVERIFY(panGesture);

    currentGestureState = Qt::NoGesture;
    {
        touchPoint.setId(0);
        touchPoint.setScenePos(QPointF(0.0f, 0.0f));
        touchPoint.setState(Qt::TouchPointPressed);
        touchPoints.append(touchPoint);
        QTouchEvent pressEvent(QEvent::TouchBegin,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointPressed,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::MayBeGesture);

    {
        touchPoints.clear();
        touchPoint.setScenePos(QPointF(0.0f, 100.0f));
        touchPoint.setState(Qt::TouchPointMoved);
        touchPoints.append(touchPoint);
        QTouchEvent moveEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointMoved,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &moveEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::TriggerGesture);

    currentGestureState = Qt::GestureStarted;
    {
        touchPoints.clear();
        touchPoint.setScenePos(QPointF(0.0f, 100.0f));
        touchPoint.setState(Qt::TouchPointReleased);
        touchPoints.append(touchPoint);
        QTouchEvent releaseEvent(QEvent::TouchEnd,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointReleased,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &releaseEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::FinishGesture);

    delete panGesture;
}

void Ut_MPanRecognizerTouch::testTapIsNotRecognizedAsPan()
{
    QList<QTouchEvent::TouchPoint> touchPoints;
    QTouchEvent::TouchPoint touchPoint;
    MPanGestureTouch *panGesture = 0;

    QGestureRecognizer::Result currentState;

    panGesture = static_cast<MPanGestureTouch*>(recognizer->create(0));
    QVERIFY(panGesture);

    currentGestureState = Qt::NoGesture;
    {
        touchPoint.setId(0);
        touchPoint.setScenePos(QPointF(0.0f, 0.0f));
        touchPoint.setState(Qt::TouchPointPressed);
        touchPoints.append(touchPoint);
        QTouchEvent pressEvent(QEvent::TouchBegin,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointPressed,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::MayBeGesture);

    {
        touchPoints.clear();
        touchPoint.setScenePos(QPointF(0.0f, 0.0f));
        touchPoint.setState(Qt::TouchPointReleased);
        touchPoints.append(touchPoint);
        QTouchEvent releaseEvent(QEvent::TouchEnd,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointReleased,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &releaseEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::CancelGesture);

    delete panGesture;
}

void Ut_MPanRecognizerTouch::testTheMovementInDirectionOtherThanRecognizedIsZeroed()
{
    QList<QTouchEvent::TouchPoint> touchPoints;
    QTouchEvent::TouchPoint touchPoint;
    MPanGestureTouch *panGesture = 0;

    QGestureRecognizer::Result currentState;

    panGesture = static_cast<MPanGestureTouch*>(recognizer->create(0));
    QVERIFY(panGesture);

    currentGestureState = Qt::NoGesture;
    {
        touchPoint.setId(0);
        touchPoint.setScenePos(QPointF(0.0f, 0.0f));
        touchPoint.setState(Qt::TouchPointPressed);
        touchPoints.append(touchPoint);
        QTouchEvent pressEvent(QEvent::TouchBegin,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointPressed,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::MayBeGesture);

    {
        touchPoints.clear();
        touchPoint.setScenePos(QPointF(0.0f, 100.0f));
        touchPoint.setState(Qt::TouchPointMoved);
        touchPoints.append(touchPoint);
        QTouchEvent moveEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointMoved,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &moveEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::TriggerGesture);

    currentGestureState = Qt::GestureStarted;

    {
        touchPoints.clear();
        touchPoint.setScenePos(QPointF(30.0f, 100.0f));
        touchPoint.setState(Qt::TouchPointMoved);
        touchPoints.append(touchPoint);
        QTouchEvent moveEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointMoved,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &moveEvent);
    }
    QCOMPARE(panGesture->offset().x(), 0.0);


    delete panGesture;
}

void Ut_MPanRecognizerTouch::testSecondTouchPointHasNoEffectOnSequentialPannings()
{
    QList<QTouchEvent::TouchPoint> touchPoints;
    QTouchEvent::TouchPoint touchPoint;
    MPanGestureTouch *panGesture = 0;

    QGestureRecognizer::Result currentState;

    panGesture = static_cast<MPanGestureTouch*>(recognizer->create(0));
    QVERIFY(panGesture);

    // 1.a - User puts finger on touchscreen
    currentGestureState = Qt::NoGesture;
    {
        touchPoint.setId(0);
        touchPoint.setScenePos(QPointF(0.0f, 0.0f));
        touchPoint.setState(Qt::TouchPointPressed);
        touchPoints.append(touchPoint);
        QTouchEvent pressEvent(QEvent::TouchBegin,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointPressed,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::MayBeGesture);

    // 1.b - User starts panning
    {
        touchPoints.clear();
        touchPoint.setScenePos(QPointF(0.0f, 100.0f));
        touchPoint.setState(Qt::TouchPointMoved);
        touchPoints.append(touchPoint);
        QTouchEvent moveEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointMoved,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &moveEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE(panGesture->offset(), QPointF(0.0f, 100.0f));

    currentGestureState = Qt::GestureStarted;

    // 2.a - user puts a second finger on touchscreen (or touches the screen
    //   with his palm)
    {
        touchPoints.clear();

        touchPoint.setId(0);
        touchPoint.setScenePos(QPointF(0.0f, 100.0f));
        touchPoint.setState(Qt::TouchPointStationary);
        touchPoints.append(touchPoint);

        touchPoint.setId(1);
        touchPoint.setScenePos(QPointF(70.0f, 800.0f));
        touchPoint.setState(Qt::TouchPointPressed);
        touchPoints.append(touchPoint);

        QTouchEvent pressEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointPressed | Qt::TouchPointStationary,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::Ignore);
    QCOMPARE(panGesture->offset(), QPointF(0.0f, 100.0f));


    // 2.b - user moves his second touch point
    {
        touchPoints.clear();

        touchPoint.setId(0);
        touchPoint.setScenePos(QPointF(0.0f, 100.0f));
        touchPoint.setState(Qt::TouchPointStationary);
        touchPoints.append(touchPoint);

        touchPoint.setId(1);
        touchPoint.setScenePos(QPointF(140.0f, 800.0f));
        touchPoint.setState(Qt::TouchPointMoved);
        touchPoints.append(touchPoint);

        QTouchEvent pressEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointStationary | Qt::TouchPointMoved,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::Ignore);
    QCOMPARE(panGesture->offset(), QPointF(0.0f, 100.0f));

    // 3 - user keeps panning with his finger from step 1
    {
        touchPoints.clear();

        touchPoint.setId(0);
        touchPoint.setScenePos(QPointF(0.0f, 150.0f));
        touchPoint.setState(Qt::TouchPointMoved);
        touchPoints.append(touchPoint);

        touchPoint.setId(1);
        touchPoint.setScenePos(QPointF(140.0f, 800.0f));
        touchPoint.setState(Qt::TouchPointStationary);
        touchPoints.append(touchPoint);

        QTouchEvent pressEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointStationary | Qt::TouchPointMoved,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE(panGesture->lastOffset(), QPointF(0.0f, 100.0f));
    QCOMPARE(panGesture->offset(), QPointF(0.0f, 150.0f));

    currentGestureState = Qt::GestureUpdated;

    // 4 - user raises finger from step 1.
    {
        touchPoints.clear();

        touchPoint.setId(0);
        touchPoint.setScenePos(QPointF(0.0f, 150.0f));
        touchPoint.setState(Qt::TouchPointReleased);
        touchPoints.append(touchPoint);

        touchPoint.setId(1);
        touchPoint.setScenePos(QPointF(140.0f, 800.0f));
        touchPoint.setState(Qt::TouchPointStationary);
        touchPoints.append(touchPoint);

        QTouchEvent pressEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointStationary | Qt::TouchPointReleased,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::FinishGesture);
    QCOMPARE(panGesture->lastOffset(), QPointF(0.0f, 150.0f));
    QCOMPARE(panGesture->offset(), QPointF(0.0f, 150.0f));

    recognizer->reset(panGesture);
    currentGestureState = Qt::NoGesture;

    // 5 - user moves finger/contact point from step 2
    {
        touchPoints.clear();

        touchPoint.setId(1);
        touchPoint.setScenePos(QPointF(180.0f, 800.0f));
        touchPoint.setState(Qt::TouchPointMoved);
        touchPoints.append(touchPoint);

        QTouchEvent pressEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointMoved,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::Ignore);

    // 6.a - user puts another finger on touchscreen
    {
        touchPoints.clear();

        touchPoint.setId(1);
        touchPoint.setScenePos(QPointF(180.0f, 800.0f));
        touchPoint.setState(Qt::TouchPointStationary);
        touchPoints.append(touchPoint);

        touchPoint.setId(2);
        touchPoint.setScenePos(QPointF(20.0f, 20.0f));
        touchPoint.setState(Qt::TouchPointPressed);
        touchPoints.append(touchPoint);

        QTouchEvent pressEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointStationary | Qt::TouchPointPressed,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::MayBeGesture);

    // 6.b - user starts panning with that other finger
    {
        touchPoints.clear();

        touchPoint.setId(1);
        touchPoint.setScenePos(QPointF(180.0f, 800.0f));
        touchPoint.setState(Qt::TouchPointStationary);
        touchPoints.append(touchPoint);

        touchPoint.setId(2);
        touchPoint.setScenePos(QPointF(120.0f, 120.0f));
        touchPoint.setState(Qt::TouchPointMoved);
        touchPoints.append(touchPoint);

        QTouchEvent pressEvent(QEvent::TouchUpdate,
                               QTouchEvent::TouchScreen,
                               Qt::NoModifier,
                               Qt::TouchPointStationary | Qt::TouchPointMoved,
                               touchPoints);
        currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    }
    QCOMPARE(currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE(panGesture->offset(), QPointF(0.0f, 100.0f));
}

QTEST_APPLESS_MAIN(Ut_MPanRecognizerTouch)