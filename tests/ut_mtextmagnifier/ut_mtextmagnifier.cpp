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

#include "ut_mtextmagnifier.h"
#include "views/mtextmagnifier.h"

#include <QEvent>
#include <QGraphicsWidget>
#include <QMetaType>
#include <QPainter>
#include <QPanGesture>
#include <QSignalSpy>

#include <MApplication>
#include <MApplicationWindow>
#include <MCancelEvent>
#include <MScene>

namespace {
    Qt::GestureState gGestureState = Qt::GestureStarted;
}

Qt::GestureState QGesture::state() const
{
    return gGestureState;
}

void Ut_MTextMagnifier::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "./ut_mtextmagnifier" };
    MApplication::setLoadMInputContext(false);
    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;

    magnifiedWidget = new QGraphicsWidget;
    m_appWindow->scene()->addItem(magnifiedWidget);
}

void Ut_MTextMagnifier::cleanupTestCase()
{
    delete magnifiedWidget;
    magnifiedWidget = 0;
    delete m_appWindow;
    m_appWindow = 0;
    delete m_app;
    m_app = 0;
}


void Ut_MTextMagnifier::init()
{
    subject = new MTextMagnifier(*magnifiedWidget);
}


void Ut_MTextMagnifier::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_MTextMagnifier::testMagnifierConsumesPanGesture_data()
{
    QTest::addColumn<Qt::GestureState>("gestureState");
    QTest::addColumn<bool>("wasConsumed");

    // This is needs to work to prevent panning while magnifier is shown.
    QTest::newRow("gesture started") << Qt::GestureStarted << true;

    // Other gesture states should be ignored.
    QTest::newRow("gesture canceled") << Qt::GestureCanceled << false;
    QTest::newRow("gesture updated") << Qt::GestureUpdated << false;
    QTest::newRow("gesture finished") << Qt::GestureFinished << false;
}

void Ut_MTextMagnifier::testMagnifierConsumesPanGesture()
{
    QFETCH(Qt::GestureState, gestureState);
    QFETCH(bool, wasConsumed);

    subject->appear();
    QVERIFY(subject->isAppeared());

    // Create gesture object and put it inside an event.
    QPanGesture panGesture;
    gGestureState = gestureState;

    QList<QGesture *> gestures;
    gestures << &panGesture;
    QGestureEvent event(gestures);
    event.setAccepted(Qt::PanGesture, false);

    // It is actually overlay who gets the event.
    subject->overlay.gestureEvent(&event);

    QCOMPARE(event.isAccepted(Qt::PanGesture), wasConsumed);
}

QTEST_APPLESS_MAIN(Ut_MTextMagnifier)

