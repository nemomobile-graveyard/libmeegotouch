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

#include <MApplication>

#include <QMouseEvent>
#include <QtTest/QtTest>


//Mocking MThemePrivate, because the style object is private to library
//and we need to compile some functionality into the unittest.
#include "mtheme_p.h"
void MThemePrivate::unregisterStyleContainer(MStyleContainer*)
{
}

void MThemePrivate::registerStyleContainer(MStyleContainer *)
{
}

#include "mtheme.h"
//Filling the values of the style object.
static const int MSwipeTimeout = 300; /* miliseconds */
static const int MSwipeMovementThreshold = 50; /* pixels */
static const qreal MSwipeAngleThreshold = 15; /* degrees */
static const qreal MSwipeAngleSnappingThreshold = 22.5; /* degrees */

MSwipeRecognizerStyle recognizerStyle;
const MStyle *MTheme::style(const char *,
                            const QString &) {
    recognizerStyle.setTimeout(MSwipeTimeout);
    recognizerStyle.setDistanceThreshold(MSwipeMovementThreshold);
    recognizerStyle.setangleThreshold(MSwipeAngleThreshold);
    recognizerStyle.setangleSnappingThreshold(MSwipeAngleSnappingThreshold);
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

void Ut_MSwipeRecognizer::init()
{
    recognizer = new MSwipeRecognizer();
}

MApplication *app;
void Ut_MSwipeRecognizer::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mswiperecognizer" };
    app = new MApplication(argc, app_name);
}

void Ut_MSwipeRecognizer::cleanupTestCase()
{
    delete app;
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
    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(0,0), QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent(QEvent::MouseMove, QPoint(100,0), QPoint(100,0), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);
}

void Ut_MSwipeRecognizer::testFastTap()
{
    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(0,0), QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(0,0), QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &releaseEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);
}

qint64 elapsedTime = 0;
qint64 QElapsedTimer::elapsed() const
{
    return elapsedTime;
}

void Ut_MSwipeRecognizer::testTimedout()
{
    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(0,0), QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent(QEvent::MouseMove, QPoint(30,0), QPoint(30,0), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    elapsedTime = 3000;

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);

    elapsedTime = 0;

}

void Ut_MSwipeRecognizer::testZigzagged()
{
    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(0,0), QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent1(QEvent::MouseMove, QPoint(55,0), QPoint(55,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent2(QEvent::MouseMove, QPoint(-100,0), QPoint(-100,0), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent2);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    currentGestureState = Qt::GestureStarted;

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent2);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);

}

void Ut_MSwipeRecognizer::testSnappingToRightAngles()
{
    QMouseEvent pressEvent(QEvent::MouseButtonPress,QPoint(0,0), QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent1(QEvent::MouseMove, QPoint(100,10), QPoint(100,10), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent2(QEvent::MouseMove, QPoint(10,-100), QPoint(10,-100), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent3(QEvent::MouseMove, QPoint(-100,-10), QPoint(-100,-10), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent4(QEvent::MouseMove, QPoint(10,100), QPoint(10,100), Qt::LeftButton, Qt::LeftButton, 0);

    currentGestureState = Qt::NoGesture;

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent1);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE( swipeGesture->swipeAngle(), 0.0);

    recognizer->reset(swipeGesture);

    currentGestureState = Qt::NoGesture;

    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent2);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE( swipeGesture->swipeAngle(), 90.0);

    recognizer->reset(swipeGesture);

    currentGestureState = Qt::NoGesture;

    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent3);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE( swipeGesture->swipeAngle(), 180.0);

    recognizer->reset(swipeGesture);

    currentGestureState = Qt::NoGesture;

    currentState = recognizer->recognize(swipeGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(swipeGesture, 0, &moveEvent4);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);
    QCOMPARE( swipeGesture->swipeAngle(), 270.0);
}

QTEST_APPLESS_MAIN(Ut_MSwipeRecognizer)

