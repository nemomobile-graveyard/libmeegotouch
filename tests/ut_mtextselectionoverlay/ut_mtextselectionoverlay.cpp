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
    qRegisterMetaType<M::OrientationAngle>("M::OrientationAngle");
    qRegisterMetaType<const MTextSelectionHandle*>("const MTextSelectionHandle*");
    qRegisterMetaType<MTextSelectionOverlay::HandleType>("MTextSelectionOverlay::HandleType");

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
    press.setScenePos(QPointF(300, 60));
    press.setPos(QPointF(300, 60));
    QVERIFY(subject->handleA.sceneBoundingRect().contains(press.scenePos()));
    subject->mousePressEvent(&press);
    QCOMPARE(press.isAccepted(), true);
    QTest::qWait(60);
    QCOMPARE(subject->handleA.isVisible(), false);
    QCOMPARE(subject->handleA.isPressed(), true);
    QCOMPARE(subject->handleB.isPressed(), false);
    QCOMPARE(subject->handleB.disabled, true);
    QCOMPARE(selectionHanldeAPressedSpy.count(), 1);

    // move handleA
    move1.setScenePos(QPointF(400, 60));
    move1.setPos(QPointF(400, 60));
    subject->mouseMoveEvent(&move1);
    QCOMPARE(move1.isAccepted(), true);
    QCOMPARE(selectionHanldeMovedSpy.count(), 1);

    move2.setScenePos(QPointF(500, 60));
    move2.setPos(QPointF(500, 60));
    subject->mouseMoveEvent(&move2);
    QCOMPARE(move2.isAccepted(), true);
    QCOMPARE(selectionHanldeMovedSpy.count(), 2);

    // release handleA
    release.setScenePos(QPointF(500, 60));
    release.setPos(QPointF(500, 60));
    subject->mouseReleaseEvent(&release);
    QCOMPARE(release.isAccepted(), true);
    QTest::qWait(60);
    QCOMPARE(subject->handleA.isVisible(), true);
    QCOMPARE(subject->handleA.isPressed(), false);
    QCOMPARE(subject->handleB.isPressed(), false);
    QCOMPARE(subject->handleB.disabled, false);
    QCOMPARE(subject->handleB.isVisible(), true);
    QCOMPARE(selectionHanldeAReleasedSpy.count(), 1);

    // press handleB
    press.setScenePos(QPointF(600, 60));
    press.setPos(QPointF(600, 60));
    QVERIFY(subject->handleB.sceneBoundingRect().contains(press.scenePos()));
    subject->mousePressEvent(&press);
    QCOMPARE(press.isAccepted(), true);
    QTest::qWait(60);
    QCOMPARE(subject->handleA.isPressed(), false);
    QCOMPARE(subject->handleA.disabled, true);
    QCOMPARE(subject->handleB.isVisible(), false);
    QCOMPARE(subject->handleB.isPressed(), true);
    QCOMPARE(selectionHanldeBPressedSpy.count(), 1);

    selectionHanldeMovedSpy.clear();
    // move handleA
    move1.setScenePos(QPointF(700, 60));
    move1.setPos(QPointF(700, 60));
    subject->mouseMoveEvent(&move1);
    QCOMPARE(move1.isAccepted(), true);
    QCOMPARE(selectionHanldeMovedSpy.count(), 1);

    move2.setScenePos(QPointF(800, 60));
    move2.setPos(QPointF(800, 60));
    subject->mouseMoveEvent(&move2);
    QCOMPARE(move2.isAccepted(), true);
    QCOMPARE(selectionHanldeMovedSpy.count(), 2);

    // release handleB
    release.setScenePos(QPointF(800, 60));
    release.setPos(QPointF(800, 60));
    subject->mouseReleaseEvent(&release);
    QCOMPARE(release.isAccepted(), true);
    QTest::qWait(60);
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

QTEST_APPLESS_MAIN(Ut_MTextSelectionOverlay)

