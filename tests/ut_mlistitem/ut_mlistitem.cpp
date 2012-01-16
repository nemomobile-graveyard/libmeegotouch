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

#include <MApplication>
#include <QGraphicsSceneMouseEvent>
#include <QStateMachine>
#include <QState>
#include <QSignalSpy>
#include <QTapAndHoldGesture>
#include "ut_mlistitem.h"
#include "mlistitem.h"
#include "mlistitemview.h"
#include "mlistitemview_p.h"
#include "mtapstatemachine.h"
#include "mtapstatemachine_p.h"
#include "mviewconstants.h"

void Ut_MListItem::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mlistitem" };
    app = new MApplication(argc, app_name);
}

void Ut_MListItem::cleanupTestCase()
{
    delete app;
}

void Ut_MListItem::init()
{
    listItem = new MListItem();
}

void Ut_MListItem::cleanup()
{
    delete listItem;
}

void Ut_MListItem::testClick()
{
    QSignalSpy clickedSpy(listItem, SIGNAL(clicked()));
    listItem->click();
    QCOMPARE(clickedSpy.count(), 1);
}

void Ut_MListItem::testLongTap()
{
    QPointF point (0, 0);
    QSignalSpy longTappedSpy(listItem, SIGNAL(longTapped(QPointF)));
    listItem->longTap(point);
    QCOMPARE(longTappedSpy.count(), 1);
    QCOMPARE(longTappedSpy.last()[0].toPointF(), point);
}

void Ut_MListItem::testReleasedOnHide()
{
    MListItemView *view = new MListItemView(listItem);
    listItem->setView(view);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addItem(listItem);

    // needed for MTapStateMachine to start working
    QCoreApplication::processEvents();

    QStateMachine *machine = view->d_func()->tapStateMachine->d_func()->highlightMachine;;
    QState *unhighlighted = view->d_func()->tapStateMachine->d_func()->unhighlightedState;
    QState *highlighted = view->d_func()->tapStateMachine->d_func()->pressHighlightedState;

    // initial state:
    // view must not be down
    QCOMPARE(view->d_func()->down, false);
    // state machine must be in unhighlighted state
    QVERIFY(machine->configuration().contains(unhighlighted));

    // send press event and wait the for the initial press time
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    scene->sendEvent(listItem, &pressEvent);
    QTest::qWait(1000);

    // pressed state
    // view must be down
    QCOMPARE(view->d_func()->down, true);
    // state machine must be in highlighted state
    QVERIFY(machine->configuration().contains(highlighted));

    // now hide the item
    listItem->hide();

    // hidden item state
    // view must not be down
    QCOMPARE(view->d_func()->down, false);
    // state machine must be in unhighlighted state
    QVERIFY(machine->configuration().contains(unhighlighted));

    scene->removeItem(listItem);
    delete scene;
}

void Ut_MListItem::testMouseClick()
{
    QSignalSpy clickedSpy(listItem, SIGNAL(clicked()));
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    listItem->mousePressEvent(&pressEvent);

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setButton(Qt::LeftButton);
    listItem->mouseReleaseEvent(&releaseEvent);
    QCOMPARE(clickedSpy.count(), 1);
}

void Ut_MListItem::testMouseMoveOutside()
{
    MListItemView *view = const_cast<MListItemView*>(qobject_cast<const MListItemView*>(listItem->view()));
    MListItemStyle *style = const_cast<MListItemStyle*>(view->style().operator ->());
    int maxDistanceForClick = M_RELEASE_MISS_DELTA+100;
    style->setMaxDistanceForClick(maxDistanceForClick);

    listItem->setGeometry(QRectF(0, 0, 300, 100));

    QSignalSpy clickedSpy(listItem, SIGNAL(clicked()));
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    listItem->mousePressEvent(&pressEvent);

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setButton(Qt::LeftButton);
    moveEvent.setScenePos(QPointF(-(maxDistanceForClick-10), 0)); //within max distance from the press point but outside of the widget release area
    listItem->mouseMoveEvent(&moveEvent);

    QCoreApplication::processEvents();

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setButton(Qt::LeftButton);
    listItem->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(clickedSpy.count(), 0);
}

void Ut_MListItem::testMouseMoveWithinMaxDistance()
{
    MListItemView *view = const_cast<MListItemView*>(qobject_cast<const MListItemView*>(listItem->view()));
    MListItemStyle *style = const_cast<MListItemStyle*>(view->style().operator ->());
    int maxDistanceForClick = 100;
    style->setMaxDistanceForClick(maxDistanceForClick);

    listItem->setGeometry(QRectF(0, 0, maxDistanceForClick, 100));

    QSignalSpy clickedSpy(listItem, SIGNAL(clicked()));
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    listItem->mousePressEvent(&pressEvent);

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setButton(Qt::LeftButton);
    moveEvent.setScenePos(QPointF(maxDistanceForClick-10, 0)); //within max distance from the press point and widget area
    listItem->mouseMoveEvent(&moveEvent);

    QCoreApplication::processEvents();

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setButton(Qt::LeftButton);
    listItem->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(clickedSpy.count(), 1);
}

void Ut_MListItem::testMouseMoveBeyondMaxDistance()
{
    MListItemView *view = const_cast<MListItemView*>(qobject_cast<const MListItemView*>(listItem->view()));
    MListItemStyle *style = const_cast<MListItemStyle*>(view->style().operator ->());
    int maxDistanceForClick = 100;
    style->setMaxDistanceForClick(maxDistanceForClick);

    listItem->setGeometry(QRectF(0, 0, maxDistanceForClick+100, 100));

    QSignalSpy clickedSpy(listItem, SIGNAL(clicked()));
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    listItem->mousePressEvent(&pressEvent);

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setButton(Qt::LeftButton);
    moveEvent.setScenePos(QPointF(maxDistanceForClick+10, 0)); //Too far from the press point but within widget area
    listItem->mouseMoveEvent(&moveEvent);

    QCoreApplication::processEvents();

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setButton(Qt::LeftButton);
    listItem->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(clickedSpy.count(), 0);
}

void Ut_MListItem::testCancelEvent()
{
    QSignalSpy clickedSpy(listItem, SIGNAL(clicked()));
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    listItem->mousePressEvent(&pressEvent);

    MCancelEvent event;
    listItem->cancelEvent(&event);

    QCoreApplication::processEvents();

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setButton(Qt::LeftButton);
    listItem->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(clickedSpy.count(), 0);
}

Qt::GestureState QGesture::state() const
{
    return Qt::GestureFinished;
}

void Ut_MListItem::testTapAndHold()
{
    listItem->setGeometry(QRectF(0, 0, 300, 100));

    QSignalSpy longTappedSpy(listItem, SIGNAL(longTapped(QPointF)));

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    listItem->mousePressEvent(&pressEvent);

    QList<QGesture *> gestures;
    QTapAndHoldGesture gesture;
    QPointF point(10, 10);
    gesture.setPosition(point);
    gestures.append(&gesture);
    QGestureEvent event(gestures);

    listItem->tapAndHoldGestureEvent(&event, &gesture);

    QVERIFY(event.isAccepted(&gesture));
    QCOMPARE(longTappedSpy.count(), 1);
    QCOMPARE(longTappedSpy.last()[0].toPointF(), point);
}

QTEST_APPLESS_MAIN(Ut_MListItem)
