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

#include "mapplication.h"
#include <QGraphicsView>
#include <QMouseEvent>
#include <QtTest/QtTest>

Qt::GestureState currentGestureState = Qt::NoGesture;
Qt::GestureState QGesture::state() const
{
    return currentGestureState;
}

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
static const int MTapAndHoldTimeout = 500; /* miliseconds */
static const qreal MTapAndHoldMovementThreshold = 20; /* pixels */

MTapAndHoldRecognizerStyle recognizerStyle;
const MStyle *MTheme::style(const char *,
                            const QString &) {
    recognizerStyle.setTimeout(MTapAndHoldTimeout);
    recognizerStyle.setMovementThreshold(MTapAndHoldMovementThreshold);
    return &recognizerStyle;
}

void MTheme::releaseStyle(const MStyle *)
{
}

MApplication *app;
QGraphicsView *view;
void Ut_MTapAndHoldRecognizer::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mtapandholdrecognizer" };
    app = new MApplication(argc, app_name);

    view = new QGraphicsView();
    view->show();
}

void Ut_MTapAndHoldRecognizer::cleanupTestCase()
{
    delete app;
}

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
    tapAndHoldGesture = static_cast<MTapAndHoldGesture*>(recognizer->create(view));
}

void Ut_MTapAndHoldRecognizer::testRecognize()
{
    QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0,0), QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(tapAndHoldGesture, 0, pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    QTimerEvent fakeTimerEvent(tapAndHoldGesture->timerId);
    currentState = recognizer->recognize(tapAndHoldGesture, tapAndHoldGesture, &fakeTimerEvent);
    QCOMPARE( currentState, QGestureRecognizer::FinishGesture | QGestureRecognizer::ConsumeEventHint );

    delete pressEvent;
}

void Ut_MTapAndHoldRecognizer::testFastTap()
{
    QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease, QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(tapAndHoldGesture, 0, pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    //Setting timerId as would happen in real life scenario
    tapAndHoldGesture->timerId = 1;

    currentState = recognizer->recognize(tapAndHoldGesture, 0, releaseEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);

    delete pressEvent;
    delete releaseEvent;
}

void Ut_MTapAndHoldRecognizer::testMovePointerInsideThreshold()
{
    QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove, QPoint(3,0), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(tapAndHoldGesture, 0, pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    currentGestureState = Qt::GestureStarted;

    currentState = recognizer->recognize(tapAndHoldGesture, 0, moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    QTimerEvent fakeTimerEvent(tapAndHoldGesture->timerId);
    currentState = recognizer->recognize(tapAndHoldGesture, tapAndHoldGesture, &fakeTimerEvent);
    QCOMPARE( currentState, QGestureRecognizer::FinishGesture | QGestureRecognizer::ConsumeEventHint );

    delete pressEvent;
    delete moveEvent;
}

void Ut_MTapAndHoldRecognizer::testMovePointerBeyondThreshold()
{
    QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent *moveEvent = new QMouseEvent(QEvent::MouseMove, QPoint(50,0), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(tapAndHoldGesture, 0, pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    currentGestureState = Qt::GestureStarted;

    currentState = recognizer->recognize(tapAndHoldGesture, 0, moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);

    delete pressEvent;
    delete moveEvent;
}

QTEST_APPLESS_MAIN(Ut_MTapAndHoldRecognizer)

