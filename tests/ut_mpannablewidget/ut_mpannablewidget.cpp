/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ut_mpannablewidget.h"
#include "mpannablewidget.h"
#include "mpannablewidget_p.h"
#include "mphysics2dpanning_p.h"
#include "mcancelevent.h"

#include <QGestureEvent>
#include <QPanGesture>

#include <QDebug>

class DummyGraphicsItem : public MWidget
{
public:
    DummyGraphicsItem() {
        mousePressReceived = false;
        mouseMoveReceived = false;
        mouseReleaseReceived = false;
        cancelReceived = false;
    }

    bool mousePressReceived;
    bool mouseMoveReceived;
    bool mouseReleaseReceived;
    bool cancelReceived;
};
DummyGraphicsItem *dummyItem = 0;

struct PhysicsState {

    PhysicsState() {
        pointerPressed = false;
        pointerMoved = false;
        pointerReleased = false;
        physicsStopped = false;
        physicsIsMoving = false;
    }

    bool pointerPressed;
    bool pointerMoved;
    bool pointerReleased;
    bool physicsStopped;
    bool physicsIsMoving;
};
PhysicsState *physicsState = 0;

void MPhysics2DPanning::pointerMove(const QPointF& /*pos*/)
{
    physicsState->pointerMoved = true;
}

 Qt::GestureState currentPanState = Qt::NoGesture;
 Qt::GestureState QGesture::state() const
 {
     return currentPanState;
 }

 // End of stubs.

void Ut_MPannableWidget::initTestCase()
{
}

void Ut_MPannableWidget::cleanupTestCase()
{
}

void Ut_MPannableWidget::init()
{
    dummyItem = new DummyGraphicsItem;
    physicsState = new PhysicsState;
    widget = new MPannableWidget;
    widgetGlass = new MPannableWidgetGlass(widget);
}

void Ut_MPannableWidget::cleanup()
{
    delete widgetGlass;
    delete widget;
    delete physicsState;
    delete dummyItem;
}

void Ut_MPannableWidget::panGestureMovesPhysicsPointer()
{
    QPanGesture panGesture;

    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent event(gestureList);

    currentPanState = Qt::GestureStarted;

    QSignalSpy spyPress(widget->d_func()->physics, SIGNAL(pointerPressed()));
    QSignalSpy spyRelease(widget->d_func()->physics, SIGNAL(pointerReleased()));
    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(spyPress.count(), 1);
    QCOMPARE(physicsState->pointerMoved, true);
    QCOMPARE(spyRelease.count(), 0);

    physicsState->pointerMoved = false;

    currentPanState = Qt::GestureUpdated;

    QSignalSpy spyPress2(widget->d_func()->physics, SIGNAL(pointerPressed()));
    QSignalSpy spyRelease2(widget->d_func()->physics, SIGNAL(pointerReleased()));
    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(spyPress2.count(), 0);
    QCOMPARE(physicsState->pointerMoved, true);
    QCOMPARE(spyRelease2.count(), 0);

    physicsState->pointerMoved = false;

    currentPanState = Qt::GestureFinished;

    QSignalSpy spyPress3(widget->d_func()->physics, SIGNAL(pointerPressed()));
    QSignalSpy spyRelease3(widget->d_func()->physics, SIGNAL(pointerReleased()));
    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(spyPress3.count(), 0);
    QCOMPARE(physicsState->pointerMoved, false);
    QCOMPARE(spyRelease3.count(), 1);
}

void Ut_MPannableWidget::panGestureAgainstPanningDirectionStopsPanning()
{
    QPanGesture panGesture;

    QList<QGesture*> gestureList;
    gestureList.append(&panGesture);
    QGestureEvent event(gestureList);

    currentPanState = Qt::GestureStarted;
    panGesture.setOffset(QPointF(100,0));

    QSignalSpy spyPress(widget->d_func()->physics, SIGNAL(pointerPressed()));
    QSignalSpy spyRelease(widget->d_func()->physics, SIGNAL(pointerReleased()));
    widget->panGestureEvent(&event, &panGesture);
    QCOMPARE(spyPress.count(), 0);
    QCOMPARE(physicsState->pointerMoved, false);
    QCOMPARE(spyRelease.count(), 1);
    QCOMPARE(event.isAccepted(&panGesture), false);
}

class CustomPhysics : public MPhysics2DPanning
{
public:
    CustomPhysics(MPannableWidget *parent) : MPhysics2DPanning(parent)
    {
    }
};

void Ut_MPannableWidget::usingCustomPhysics()
{
    CustomPhysics *customPhysics = new CustomPhysics(widget);
    widget->setPhysics(customPhysics);

    QVERIFY(static_cast<MPannableWidgetPrivate *>(widget->d_ptr)->physics == customPhysics);

    widget->setPhysics(new MPhysics2DPanning(widget));
}

void Ut_MPannableWidget::settingNewPhysicsShouldEmitPhysicsChangeSignal()
{
    CustomPhysics *customPhysics = new CustomPhysics(widget);

    QSignalSpy spyPhysicsChange(widget, SIGNAL(physicsChanged()));

    widget->setPhysics(customPhysics);

    QCOMPARE(spyPhysicsChange.count(),1);

    widget->setPhysics(new MPhysics2DPanning(widget));
}

void Ut_MPannableWidget::settingPhysicsToNULLShouldNotBreakTheWidget()
{
    widget->setPhysics(NULL);
    QVERIFY(static_cast<MPannableWidgetPrivate *>(widget->d_ptr)->physics != NULL);
}

void Ut_MPannableWidget::testGlassMousePressEvent()
{
    QGraphicsSceneMouseEvent event;
    //physics not in motion, ignore expected
    widgetGlass->mousePressEvent(&event);
    QVERIFY(!event.isAccepted());

    widget->d_func()->physics->setPosition(QPointF(100, 100));
    widget->d_func()->physics->setMaximumVelocity(10.0);
    //add some positions samples to calculate velocity
    widget->d_func()->physics->d_ptr->positions.push_front(QPointF(40, 40));
    widget->d_func()->physics->d_ptr->positions.push_front(QPointF(30, 30));
    widget->d_func()->physics->d_ptr->positions.push_front(QPointF(20, 20));
    widget->d_func()->physics->pointerRelease();
    //physics in motion
    widgetGlass->mousePressEvent(&event);
    QVERIFY(event.isAccepted());
}

void Ut_MPannableWidget::testGlassTapAndHoldGestureEvent()
{
    QList<QGesture*> gestures;

    QTapAndHoldGesture *gesture1 = new QTapAndHoldGesture();
    gesture1->setPosition(QPointF(20, 20));
    gestures.append(gesture1);
    QGestureEvent event(gestures);

    widgetGlass->tapAndHoldGestureEvent(&event, gesture1);
    //widget is not in motion, so event won't be accepted
    QVERIFY(!event.isAccepted(gesture1));

    widget->d_func()->physics->setPosition(QPointF(20, 20));
    widget->d_func()->physics->setMaximumVelocity(10.0);
    //add some positions samples to calculate velocity
    widget->d_func()->physics->d_ptr->positions.push_front(QPointF(40, 40));
    widget->d_func()->physics->d_ptr->positions.push_front(QPointF(30, 30));
    widget->d_func()->physics->d_ptr->positions.push_front(QPointF(20, 20));
    widget->d_func()->physics->pointerRelease();
    widgetGlass->tapAndHoldGestureEvent(&event, gesture1);
    QVERIFY(event.isAccepted(gesture1));
    gestures.clear();
}

void Ut_MPannableWidget::testEnabled()
{
    widget->setEnabled(true);

    QVERIFY(widget->isEnabled());
}

void Ut_MPannableWidget::testUpdatePosition()
{
    QSignalSpy spy(widget->d_func()->physics, SIGNAL(positionChanged(const QPointF)));
    widget->d_func()->physics->setPosition(QPointF(20, 20));

    QCOMPARE(spy.count(), 1);

    //nothing more to test
}

void Ut_MPannableWidget::testPanThreshold()
{
    //nothing to test as MPannableWidget::panThreshold()
    //and MPannableWidget::setPanThreshold() is not implemented

    widget->setPanThreshold(0);
    widget->panThreshold();
}

QTEST_APPLESS_MAIN(Ut_MPannableWidget);
