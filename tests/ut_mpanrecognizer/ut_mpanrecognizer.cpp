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

#include <QGraphicsView>
#include <QMouseEvent>
#include <QtTest/QtTest>

#include <MApplication>

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
QGraphicsView *view;
void Ut_MPanRecognizer::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mpanrecognizer" };
    app = new MApplication(argc, app_name);

    view = new QGraphicsView();
    view->show();
}

void Ut_MPanRecognizer::cleanupTestCase()
{
    delete app;
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
    panGesture = static_cast<MPanGesture*>(recognizer->create(view));
}

void Ut_MPanRecognizer::testRecognize()
{
    QMouseEvent pressEvent(QEvent::MouseButtonPress,QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent(QEvent::MouseMove,QPoint(0,100), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease,QPoint(0,100), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(panGesture, 0, &moveEvent);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    //Artificially setting state of QGesture object.
    currentGestureState = Qt::GestureUpdated;

    currentState = recognizer->recognize(panGesture, 0, &releaseEvent);
    QCOMPARE( currentState, QGestureRecognizer::FinishGesture);
}

void Ut_MPanRecognizer::testTapIsNotRecognizedAsPan()
{
    QMouseEvent pressEvent(QEvent::MouseButtonPress,QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease,QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(panGesture, 0, &releaseEvent);
    QCOMPARE( currentState, QGestureRecognizer::CancelGesture);
}

void Ut_MPanRecognizer::testTheMovementInDirectionOtherThanRecognizedIsZeroed()
{
    QMouseEvent pressEvent(QEvent::MouseButtonPress,QPoint(0,0), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent1(QEvent::MouseMove,QPoint(0,100), Qt::LeftButton, Qt::LeftButton, 0);
    QMouseEvent moveEvent2(QEvent::MouseMove,QPoint(30,100), Qt::LeftButton, Qt::LeftButton, 0);

    QGestureRecognizer::Result currentState;
    currentState = recognizer->recognize(panGesture, 0, &pressEvent);
    QCOMPARE( currentState, QGestureRecognizer::MayBeGesture);

    currentState = recognizer->recognize(panGesture, 0, &moveEvent1);
    QCOMPARE( currentState, QGestureRecognizer::TriggerGesture);

    //Artificially setting state of QGesture object.
    currentGestureState = Qt::GestureUpdated;
    recognizer->recognize(panGesture, 0, &moveEvent2);
    QCOMPARE( panGesture->offset().x(), 0.0);
}

void Ut_MPanRecognizer::testReset()
{
    MPanGesture * panGesture = new MPanGesture(0);
    panGesture->startPos = QPointF(1,1);
    panGesture->panDirection = Qt::Vertical;
    panGesture->pressed = true;

    recognizer->reset(panGesture);
    QCOMPARE((int)panGesture->panDirection,0);
    QCOMPARE(panGesture->pressed, false);
    QVERIFY(qFuzzyCompare(panGesture->startPos.x(), 0));
    QVERIFY(qFuzzyCompare(panGesture->startPos.ry(),0));
}
QTEST_APPLESS_MAIN(Ut_MPanRecognizer)

