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
#include "views/mtopleveloverlay.h"

#include <QEvent>
#include <QMetaType>
#include <QPainter>
#include <QPanGesture>
#include <QSignalSpy>

#include <MApplication>
#include <MApplicationWindow>
#include <MCancelEvent>
#include <MNamespace>
#include <MScene>
#include <MSceneManager>
#include <MWidget>

namespace {
    Qt::GestureState gGestureState = Qt::GestureStarted;
}

Qt::GestureState QGesture::state() const
{
    return gGestureState;
}

void Ut_MTextMagnifier::initTestCase()
{
    qRegisterMetaType<M::OrientationAngle>("M::OrientationAngle");

    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "./ut_mtextmagnifier" };
    MApplication::setLoadMInputContext(false);
    m_app = new MApplication(dummyArgc, dummyArgv);
    m_appWindow = new MApplicationWindow;

    magnifiedWidget = new MWidget;
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
    subject = new MTextMagnifier(*magnifiedWidget, QSizeF());
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
    subject->overlay.panGestureEvent(&event, &panGesture);

    QCOMPARE(event.isAccepted(Qt::PanGesture), wasConsumed);
}

void Ut_MTextMagnifier::testOverlayGeometry_data()
{
    QTest::addColumn<M::OrientationAngle>("angle");
    QTest::addColumn<QRectF>("expectedSceneGeometry");

    const QRectF expectedSceneGeometry(
        QPointF(),
        m_appWindow->sceneManager()->visibleSceneSize(M::Landscape));

    QTest::newRow("angle 0") << M::Angle0 << expectedSceneGeometry;
    QTest::newRow("angle 90") << M::Angle90 << expectedSceneGeometry;
    QTest::newRow("angle 180") << M::Angle180 << expectedSceneGeometry;
    QTest::newRow("angle 270") << M::Angle270 << expectedSceneGeometry;
}

void Ut_MTextMagnifier::testOverlayGeometry()
{
    // Overlay should be fullscreen.

    QFETCH(M::OrientationAngle, angle);
    QFETCH(QRectF, expectedSceneGeometry);

    m_appWindow->sceneManager()->setOrientationAngle(
        angle, MSceneManager::ImmediateTransition);

    const QGraphicsWidget &overlay(subject->overlay);
    QCOMPARE(overlay.rotation(), static_cast<qreal>(angle));
    QCOMPARE(overlay.mapRectToScene(overlay.rect()), expectedSceneGeometry);
}

void Ut_MTextMagnifier::testOutOfScreen_data()
{
    QTest::addColumn<QSizeF>("magnifierSize");
    QTest::addColumn<QSizeF>("keepVisibleSize");
    QTest::addColumn<QPointF>("pos");
    // Bounding rect position given in parent coordinates.
    QTest::addColumn<QPointF>("boundingRectPos");

    // This test assumes magnifier offset value from style (stubbed) is (0,0).
    // Therefore bounding rectangle is centered at widget origin. Offset is
    // added if a part of bounding rectangle doesn't fit inside screen area.

    // Bounding rect sits in the corner.
    QTest::newRow("No offset required")
        << QSizeF(30.0f, 30.0f)
        << QSizeF(10.0f, 10.0f)
        << QPointF(15.0f, 15.0f)
        << QPointF(0.0f, 0.0f);

    // Allowed to go negative because of small visible area
    QTest::newRow("No offset required due to small visible area")
        << QSizeF(30.0f, 30.0f)
        << QSizeF(10.0f, 10.0f)
        << QPointF(10.0f, 10.0f)
        << QPointF(-5.0f, -5.0f);

    QTest::newRow("Offset required")
        << QSizeF(30.0f, 30.0f)
        << QSizeF(10.0f, 10.0f)
        << QPointF(0.0f, 0.0f)
        << QPointF(-10.0f, -10.0f); // -10 instead of -15

    QTest::newRow("Offset required because whole magnifier should be visible")
        << QSizeF(30.0f, 30.0f)
        << QSizeF(30.0f, 30.0f)
        << QPointF(10.0f, 10.0f)
        << QPointF(0.0f, 0.0f);

    const QSizeF sceneSize =
        m_appWindow->sceneManager()->visibleSceneSize(M::Landscape);
    const QPointF sceneTopRight = QPointF(sceneSize.width(), 0.0f);

    QTest::newRow("No offset required, top right corner")
        << QSizeF(30.0f, 30.0f)
        << QSizeF(10.0f, 10.0f)
        << sceneTopRight + QPointF(-15.0f, 15.0f)
        << sceneTopRight + QPointF(-30.0f, 0.0f);

    QTest::newRow("Offset required, top right corner")
        << QSizeF(30.0f, 30.0f)
        << QSizeF(10.0f, 10.0f)
        << sceneTopRight
        << sceneTopRight + QPointF(-20.0f, -10.0f);
}

void Ut_MTextMagnifier::testOutOfScreen()
{
    QFETCH(QSizeF, magnifierSize);
    QFETCH(QSizeF, keepVisibleSize);
    QFETCH(QPointF, pos);
    QFETCH(QPointF, boundingRectPos);

    // keepVisibleSize must be given to constructor
    delete subject;
    subject = new MTextMagnifier(*magnifiedWidget,
                                 keepVisibleSize);

    // For now, test only landscape. The testOverlayGeometry
    // test checks whether overlay is of the correct size in
    // every orientation. This is enough because moving the magnifier
    // is done in local item space of magnifier.
    m_appWindow->sceneManager()->setOrientationAngle(
        M::Angle0, MSceneManager::ImmediateTransition);

    subject->resize(magnifierSize);
    subject->setMagnifiedPosition(pos);

    QCOMPARE(subject->boundingRect().topLeft(),
             subject->parentItem()->mapToItem(subject, boundingRectPos));
}

QTEST_APPLESS_MAIN(Ut_MTextMagnifier)

