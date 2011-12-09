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

#include "ut_mtextselectionoverlay.h"
#include "testview.h"
#include "../utils.h"
#include "views/mtextselectionoverlay.h"
#include "views/mtextselectionhandle.h"

#include <QEvent>
#include <QMetaType>
#include <QPainter>
#include <QPanGesture>
#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>

#include <MApplication>
#include <MApplicationWindow>
#include <MCancelEvent>
#include <MNamespace>
#include <MScene>
#include <MSceneManager>
#include <MWidgetController>
#include <MWidgetView>

namespace {
    Qt::GestureState gGestureState = Qt::GestureStarted;
}

Q_DECLARE_METATYPE(const MTextSelectionHandle*)

Qt::GestureState QGesture::state() const
{
    return gGestureState;
}


void Ut_MTextSelectionOverlay::initTestCase()
{
#ifndef __arm__
    QApplication::setGraphicsSystem("raster");
#endif

    qRegisterMetaType<M::OrientationAngle>("M::OrientationAngle");
    qRegisterMetaType<const MTextSelectionHandle*>("const MTextSelectionHandle*");

    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "./ut_mtextselectionoverlay" };
    MApplication::setLoadMInputContext(false);
    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;

    selectedView = new TestView;
    selectedWidget = new MWidgetController;
    selectedWidget->setView(selectedView);
    m_appWindow->scene()->addItem(selectedWidget);
}

void Ut_MTextSelectionOverlay::cleanupTestCase()
{
    delete selectedWidget;
    selectedWidget = 0;
    delete m_appWindow;
    m_appWindow = 0;
    delete m_app;
    m_app = 0;
}


void Ut_MTextSelectionOverlay::init()
{
    subject = new MTextSelectionOverlay(selectedWidget, selectedView); 
}


void Ut_MTextSelectionOverlay::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_MTextSelectionOverlay::testSelectionChange_data()
{
    QTest::addColumn<int>("newAnchor");
    QTest::addColumn<QRectF>("newAnchorRect");
    QTest::addColumn<bool>("newAnchorVisible");

    QTest::addColumn<int>("newCursor");
    QTest::addColumn<QRectF>("newCursorRect");
    QTest::addColumn<bool>("newCursorVisible");

    QTest::addColumn<bool>("isOverlayVisible");

    QTest::newRow("selection start") << 0 << QRectF(50, 50, 1, 10) << true
        << 10 << QRectF(300, 50, 1, 10) << true << true;

    QTest::newRow("selection changing 1") << 3 << QRectF(100, 50, 1, 10) << false
        << 10 << QRectF(300, 50, 1, 10) << true << true;

    QTest::newRow("selection changing 2") << 5 << QRectF(150, 50, 1, 10) << false
        << 10 << QRectF(300, 50, 1, 10) << true << true;

    QTest::newRow("selection changed") << 5 << QRectF(100, 50, 1, 10) << true
        << 10 << QRectF(300, 50, 1, 10) << true << true;

    QTest::newRow("selection disappear") << 5 << QRectF(100, 50, 1, 10) << false
        << 5 << QRectF(100, 50, 1, 10) << false << false;
}

void Ut_MTextSelectionOverlay::testSelectionChange()
{
    QFETCH(int, newAnchor);
    QFETCH(QRectF, newAnchorRect);
    QFETCH(bool, newAnchorVisible);
    QFETCH(int, newCursor);
    QFETCH(QRectF, newCursorRect);
    QFETCH(bool, newCursorVisible);

    QFETCH(bool, isOverlayVisible);

    emit selectedView->selectionChanged(newAnchor, newAnchorRect, newAnchorVisible,
                                        newCursor, newCursorRect, newCursorVisible);

    QCOMPARE(subject->isVisible(), isOverlayVisible);

    QCOMPARE(subject->anchor, newAnchor);
    QCOMPARE(subject->anchorHandle->isVisible(), newAnchorVisible);
    QCOMPARE(subject->cursor, newCursor);
    QCOMPARE(subject->cursorHandle->isVisible(), newCursorVisible);

    // Create gesture object and put it inside an event.
    QPanGesture panGesture;
    QList<QGesture *> gestures;
    gestures << &panGesture;
    QGestureEvent event(gestures);
    event.setAccepted(Qt::PanGesture, false);
    // If the overlay is visible, it is actually overlay who gets the event.
    subject->panGestureEvent(&event, &panGesture);
    QCOMPARE(event.isAccepted(Qt::PanGesture), isOverlayVisible);
}

void Ut_MTextSelectionOverlay::testSelectionPressAndMove()
{
    QCOMPARE(subject->isVisible(), false);
    QCOMPARE(subject->handleA.isVisible(), false);
    QCOMPARE(subject->handleB.isVisible(), false);

    emit selectedView->selectionChanged(10, QRectF(300, 50, 1, 10), true,
                                        20, QRectF(600, 50, 1, 10), true);

    QCOMPARE(subject->isVisible(), true);
    QCOMPARE(subject->handleA.isVisible(), true);
    QCOMPARE(subject->handleB.isVisible(), true);

    QGraphicsSceneMouseEvent press(QEvent::GraphicsSceneMousePress);
    QGraphicsSceneMouseEvent move1(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent move2(QEvent::GraphicsSceneMouseMove);
    QGraphicsSceneMouseEvent release(QEvent::GraphicsSceneMouseRelease);

    QSignalSpy selectionHanldeAPressedSpy(&(subject->handleA),
                                          SIGNAL(pressed(const MTextSelectionHandle *)));
    QSignalSpy selectionHanldeBPressedSpy(&(subject->handleB),
                                          SIGNAL(pressed(const MTextSelectionHandle *)));

    QSignalSpy selectionHanldeAReleasedSpy(&(subject->handleA),
                                          SIGNAL(released(const MTextSelectionHandle *)));
    QSignalSpy selectionHanldeBReleasedSpy(&(subject->handleB),
                                          SIGNAL(released(const MTextSelectionHandle *)));

    QSignalSpy selectionHanldeMovedSpy(subject,
                                       SIGNAL(selectionHandleMoved(const QPointF &)));

    // press handleA
    QPointF pressScenePos = subject->handleA.sceneBoundingRect().center();
    press.setScenePos(pressScenePos);
    press.setPos(subject->mapFromScene(pressScenePos));
    QVERIFY(subject->handleA.sceneBoundingRect().contains(press.scenePos()));
    subject->mousePressEvent(&press);
    QCOMPARE(press.isAccepted(), true);
    waitForTransition(&subject->handleA);
    QCOMPARE(subject->handleA.isVisible(), false);
    QCOMPARE(subject->handleA.isPressed(), true);
    QCOMPARE(subject->handleB.isPressed(), false);
    QCOMPARE(subject->handleB.disabled, true);
    QCOMPARE(selectionHanldeAPressedSpy.count(), 1);

    // move handleA
    pressScenePos += QPointF(100, 0);
    move1.setScenePos(pressScenePos);
    move1.setPos(subject->mapFromScene(pressScenePos));
    subject->mouseMoveEvent(&move1);
    QCOMPARE(move1.isAccepted(), true);
    QCOMPARE(selectionHanldeMovedSpy.count(), 1);

    pressScenePos += QPointF(100, 0);
    move2.setScenePos(pressScenePos);
    move2.setPos(subject->mapFromScene(pressScenePos));
    subject->mouseMoveEvent(&move2);
    QCOMPARE(move2.isAccepted(), true);
    QCOMPARE(selectionHanldeMovedSpy.count(), 2);

    // release handleA
    release.setScenePos(pressScenePos);
    release.setPos(subject->mapFromScene(pressScenePos));
    subject->mouseReleaseEvent(&release);
    QCOMPARE(release.isAccepted(), true);
    waitForTransition(&subject->handleA);
    QCOMPARE(subject->handleA.isVisible(), true);
    QCOMPARE(subject->handleA.isPressed(), false);
    QCOMPARE(subject->handleB.isPressed(), false);
    QCOMPARE(subject->handleB.disabled, false);
    QCOMPARE(subject->handleB.isVisible(), true);
    QCOMPARE(selectionHanldeAReleasedSpy.count(), 1);

    // press handleB
    pressScenePos = subject->handleB.sceneBoundingRect().center();
    press.setScenePos(pressScenePos);
    press.setPos(subject->mapFromScene(pressScenePos));
    QVERIFY(subject->handleB.sceneBoundingRect().contains(press.scenePos()));
    subject->mousePressEvent(&press);
    QCOMPARE(press.isAccepted(), true);
    waitForTransition(&subject->handleB);
    QCOMPARE(subject->handleA.isPressed(), false);
    QCOMPARE(subject->handleA.disabled, true);
    QCOMPARE(subject->handleB.isVisible(), false);
    QCOMPARE(subject->handleB.isPressed(), true);
    QCOMPARE(selectionHanldeBPressedSpy.count(), 1);

    selectionHanldeMovedSpy.clear();
    // move handle
    pressScenePos += QPointF(0, 100);
    move1.setScenePos(pressScenePos);
    move1.setPos(subject->mapFromScene(pressScenePos));
    subject->mouseMoveEvent(&move1);
    QCOMPARE(move1.isAccepted(), true);
    QCOMPARE(selectionHanldeMovedSpy.count(), 1);

    pressScenePos += QPointF(0, 100);
    move2.setScenePos(pressScenePos);
    move2.setPos(subject->mapFromScene(pressScenePos));
    subject->mouseMoveEvent(&move2);
    QCOMPARE(move2.isAccepted(), true);
    QCOMPARE(selectionHanldeMovedSpy.count(), 2);

    // release handleB
    release.setScenePos(pressScenePos);
    release.setPos(subject->mapFromScene(pressScenePos));
    subject->mouseReleaseEvent(&release);
    QCOMPARE(release.isAccepted(), true);
    waitForTransition(&subject->handleB);
    QCOMPARE(subject->handleA.isVisible(), true);
    QCOMPARE(subject->handleA.disabled, false);
    QCOMPARE(subject->handleA.isPressed(), false);
    QCOMPARE(subject->handleB.isPressed(), false);
    QCOMPARE(subject->handleB.isVisible(), true);
    QCOMPARE(selectionHanldeBReleasedSpy.count(), 1);
}

void Ut_MTextSelectionOverlay::testRegion_data()
{
    QTest::addColumn<int>("newAnchor");
    QTest::addColumn<QRectF>("newAnchorRect");
    QTest::addColumn<bool>("newAnchorVisible");

    QTest::addColumn<int>("newCursor");
    QTest::addColumn<QRectF>("newCursorRect");
    QTest::addColumn<bool>("newCursorVisible");

    QTest::addColumn<bool>("isRegionEmpty");

    QTest::newRow("selection start") << 0 << QRectF(50, 50, 1, 10) << true
        << 10 << QRectF(300, 50, 1, 10) << true << false;

    QTest::newRow("selection changing 1") << 3 << QRectF(100, 50, 1, 10) << false
        << 10 << QRectF(300, 50, 1, 10) << true << false;

    QTest::newRow("selection changing 2") << 5 << QRectF(150, 50, 1, 10) << false
        << 10 << QRectF(300, 50, 1, 10) << true << false;

    QTest::newRow("selection changed") << 5 << QRectF(100, 50, 1, 10) << true
        << 10 << QRectF(300, 50, 1, 10) << true << false;

    QTest::newRow("selection disappear") << 5 << QRectF(100, 50, 1, 10) << false
        << 5 << QRectF(100, 50, 1, 10) << false << true;

    QTest::newRow("default values") << -1 << QRectF() << true
        << -1 << QRectF() << false << true;
}

void Ut_MTextSelectionOverlay::testRegion()
{
    QFETCH(int, newAnchor);
    QFETCH(QRectF, newAnchorRect);
    QFETCH(bool, newAnchorVisible);
    QFETCH(int, newCursor);
    QFETCH(QRectF, newCursorRect);
    QFETCH(bool, newCursorVisible);

    QFETCH(bool, isRegionEmpty);

    QVERIFY(subject->region().isEmpty());

    emit selectedView->selectionChanged(newAnchor, newAnchorRect, newAnchorVisible,
                                        newCursor, newCursorRect, newCursorVisible);

    QCOMPARE(subject->region().isEmpty(), isRegionEmpty);
}

void Ut_MTextSelectionOverlay::testMouseInteraction_data()
{
    QTest::addColumn<int>("angle");
    QTest::addColumn<bool>("suddenlyHidden");
    QTest::addColumn<int>("expectSignal");

    QTest::newRow("landscape, visible") << int(M::Angle0)  << false << 1;
    QTest::newRow("landscape, hiding")  << int(M::Angle0)  << true  << 0;
    QTest::newRow("portrait, visible")  << int(M::Angle90) << false << 1;
    QTest::newRow("portrait, hiding")   << int(M::Angle90) << true  << 0;
}

void Ut_MTextSelectionOverlay::testMouseInteraction()
{
    QFETCH(int, angle);
    QFETCH(bool, suddenlyHidden);
    QFETCH(int, expectSignal);

    MApplication::activeWindow()->setOrientationAngle((M::OrientationAngle)angle);

    QSignalSpy spyPressed(subject, SIGNAL(selectionHandlePressed(QPointF)));
    QVERIFY(spyPressed.isValid());
    QSignalSpy spyMoved(subject, SIGNAL(selectionHandleMoved(QPointF)));
    QVERIFY(spyMoved.isValid());
    QSignalSpy spyReleased(subject, SIGNAL(selectionHandleReleased(QPointF)));
    QVERIFY(spyReleased.isValid());

    // show overlay
    emit selectedView->selectionChanged(0, QRectF(50, 50, 1, 10), true,
                                        10, QRectF(300, 50, 1, 10), true);

    QCOMPARE(subject->isVisible(), true);
    waitForTransition(&subject->handleA); // wait until transition will be finished

    if (suddenlyHidden) {
        emit selectedView->selectionChanged(0, QRectF(), false,
                                            0, QRectF(), false);
    }

    const QPointF handleAOldPos = subject->handleA.scenePos();
    const QPointF handleBOldPos = subject->handleB.scenePos();
    const QPointF pressScenePos = subject->handleA.sceneBoundingRect().center();
    const QPointF pressPos = subject->mapFromScene(pressScenePos);
    QGraphicsSceneMouseEvent mousePressEvent(QEvent::GraphicsSceneMousePress);
    mousePressEvent.setScenePos(pressScenePos);
    mousePressEvent.setPos(pressPos);

    subject->mousePressEvent(&mousePressEvent);

    QCOMPARE(mousePressEvent.isAccepted(), !suddenlyHidden);
    QCOMPARE(spyPressed.count(), expectSignal);
    spyPressed.clear();
    QCOMPARE(subject->handleA.isPressed(), !suddenlyHidden);
    QVERIFY(!subject->handleB.isPressed());

    if (!mousePressEvent.isAccepted()) {
        //Nothing to test, because press event was ignored
        return;
    }

    const QPointF moveScenePos = QRectF(subject->handleA.sceneBoundingRect().topLeft(),
                                        subject->handleB.sceneBoundingRect().bottomRight()).center();
    const QPointF movePos = subject->mapFromScene(moveScenePos);
    const QPointF mouseDistance = moveScenePos - pressScenePos;

    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    mouseMoveEvent.setScenePos(moveScenePos);
    mouseMoveEvent.setPos(movePos);

    subject->mouseMoveEvent(&mouseMoveEvent);

    QCOMPARE(subject->handleA.scenePos() - handleAOldPos, mouseDistance);
    QCOMPARE(subject->handleB.scenePos(), handleBOldPos);
    QVERIFY(subject->handleA.isPressed());
    QVERIFY(!subject->handleB.isPressed());
    QVERIFY(spyPressed.isEmpty());
    QVERIFY(spyReleased.isEmpty());
    QCOMPARE(spyMoved.count(), 1);
    spyMoved.clear();

    QGraphicsSceneMouseEvent mouseReleaseEvent(QEvent::GraphicsSceneMouseRelease);
    subject->mouseReleaseEvent(&mouseReleaseEvent);

    // handle A jumps back to initial position,
    // because TestView does not update selecion when handle is moved
    QCOMPARE(subject->handleA.scenePos(), handleAOldPos);

    QCOMPARE(subject->handleB.scenePos(), handleBOldPos);
    QVERIFY(!subject->handleA.isPressed());
    QVERIFY(!subject->handleB.isPressed());

    QVERIFY(spyPressed.isEmpty());
    QVERIFY(spyMoved.isEmpty());
    QCOMPARE(spyReleased.count(), expectSignal);
    spyReleased.clear();
}

void Ut_MTextSelectionOverlay::waitForTransition(MTextSelectionHandle* handle)
{
    Ut_Utils::waitForSignal(&handle->opacityAnimation, SIGNAL(finished()), 1000);
}

QTEST_APPLESS_MAIN(Ut_MTextSelectionOverlay)

