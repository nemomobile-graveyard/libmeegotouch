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

#include <MApplication>
#include <MWindow>
#include <MSceneManager>
#include <MDeviceProfile>
#include "mondisplaychangeevent.h"
#include "corelib/widgets/mwindow_p.h"

#include "ut_mwindow.h"

MWindow *win;

// Variables for onDisplay -tests
bool Ut_MWindow::m_onDisplayHandlerCalled = false;
bool Ut_MWindow::m_onDisplaySignalSent    = false;

// MWindows' visibility handler re-imps
void MWindow::enterDisplayEvent()
{
    Ut_MWindow::m_onDisplayHandlerCalled = true;
}

void MWindow::exitDisplayEvent()
{
    Ut_MWindow::m_onDisplayHandlerCalled = true;
}

// Test class implementation
void Ut_MWindow::init()
{
    if(MComponentData::instance() == 0) {
        int argc = 1;
        char *argv[ 1 ];
        argv[ 0 ] = (char*)"./ut_mwindow";
        m_componentData = new MComponentData(argc, argv);
    }
    win = new MWindow;
}

void Ut_MWindow::cleanup()
{
    delete win;
    win = 0;
}

void Ut_MWindow::testNoSceneManager()
{
    const MWindow *const_win = const_cast<MWindow *>(win);
    QCOMPARE((quintptr) const_win->sceneManager(), (quintptr) 0);
}

void Ut_MWindow::testNoScene()
{
    QCOMPARE((quintptr) win->scene(), (quintptr) 0);
}

void Ut_MWindow::testSceneManagerAutocreation()
{
    QVERIFY(win->sceneManager() != 0);
}

void Ut_MWindow::testIfSceneExistsWhenSceneManagerAutocreated()
{
    win->sceneManager();
    QCOMPARE((quintptr) win->scene(), (quintptr) win->sceneManager()->scene());
}

void Ut_MWindow::testConstructorWithSceneManager()
{
    MSceneManager *manager = new MSceneManager;
    const MWindow *const_win = new MWindow(manager);
    QCOMPARE((quintptr) const_win->sceneManager(), (quintptr) manager);
    delete const_win;
}

void Ut_MWindow::testSetSceneManager()
{
    MSceneManager *manager = new MSceneManager;
    win->setSceneManager(manager);

    QCOMPARE((quintptr) win->sceneManager(), (quintptr) manager);
}

void Ut_MWindow::testSceneRect()
{
    win->setSceneManager(new MSceneManager);
    QCOMPARE(win->sceneRect(), QRectF(QPoint(0,0), win->visibleSceneSize(M::Landscape)));
}

void Ut_MWindow::testOrientation_data()
{
    QTest::addColumn<M::OrientationAngle>("newAngle");
    QTest::addColumn<M::Orientation>("newOrientation");
    QTest::newRow("Angle0")   << M::Angle0   << M::Landscape;
    QTest::newRow("Angle90")  << M::Angle90  << M::Portrait;
    QTest::newRow("Angle180") << M::Angle180 << M::Landscape;
    QTest::newRow("Angle270") << M::Angle270 << M::Portrait;
    QTest::newRow("Angle360") << M::Angle0   << M::Landscape;
}

void Ut_MWindow::testOrientation()
{
    QFETCH(M::OrientationAngle, newAngle);
    QFETCH(M::Orientation, newOrientation);

    win->setOrientationAngle(newAngle);
    QCOMPARE(win->orientation(), newOrientation);
}

void Ut_MWindow::testSetOrientationAngle_data()
{
    QTest::addColumn<M::OrientationAngle>("newAngle");
    QTest::newRow("Angle0")   << M::Angle0;
    QTest::newRow("Angle90")  << M::Angle90;
    QTest::newRow("Angle180") << M::Angle180;
    QTest::newRow("Angle270") << M::Angle270;
    QTest::newRow("Angle360") << M::Angle0;
}

void Ut_MWindow::testSetOrientationAngle()
{
    QFETCH(M::OrientationAngle, newAngle);

    win->setOrientationAngle(newAngle);
    QCOMPARE(win->orientationAngle(), newAngle);
}

void Ut_MWindow::testSetOrientationAngleCalledFromSceneManager()
{
    win->setSceneManager(new MSceneManager);
    QCOMPARE(win->orientationAngle(), win->sceneManager()->orientationAngle());

    win->sceneManager()->setOrientationAngle(M::Angle90, MSceneManager::ImmediateTransition);
    win->setOrientationAngle(M::Angle0);
    QCOMPARE(win->orientationAngle(), M::Angle0);
    QCOMPARE(win->sceneManager()->orientationAngle(), M::Angle0);
}

void Ut_MWindow::testVisibleSceneSize_data()
{
    QTest::addColumn<M::OrientationAngle>("newAngle");
    QTest::addColumn<M::Orientation>("newOrientation");
    QTest::newRow("Angle0")   << M::Angle0   << M::Landscape;
    QTest::newRow("Angle90")  << M::Angle90  << M::Portrait;
    QTest::newRow("Angle180") << M::Angle180 << M::Landscape;
    QTest::newRow("Angle270") << M::Angle270 << M::Portrait;
    QTest::newRow("Angle360") << M::Angle0   << M::Landscape;
}

void Ut_MWindow::testVisibleSceneSize()
{
    QFETCH(M::OrientationAngle, newAngle);
    QFETCH(M::Orientation, newOrientation);

    win->setOrientationAngle(newAngle);

    QSize vss = win->visibleSceneSize();

    if (newOrientation == M::Portrait)
        vss.transpose();

    QCOMPARE(vss, MDeviceProfile::instance()->resolution());
}

void Ut_MWindow::testOrientationChangedSignalPropagationFromSceneManager()
{
    QSignalSpy orientationSpy(win, SIGNAL(orientationChanged(M::Orientation)));
    QSignalSpy angleSpy(win, SIGNAL(orientationAngleChanged(M::OrientationAngle)));

    // create the scene manager
    win->sceneManager();
    M::OrientationAngle newAngle = (M::OrientationAngle)(win->orientationAngle() + 90);

    win->sceneManager()->setOrientationAngle(newAngle, MSceneManager::ImmediateTransition);

    QCOMPARE(orientationSpy.count(), 1);
    QCOMPARE(angleSpy.count(), 1);
}

void Ut_MWindow::testNoOrientationChangedSignalWhenRotatingBy180Degrees()
{
    QSignalSpy orientationSpy(win, SIGNAL(orientationChanged(M::Orientation)));
    QSignalSpy angleSpy(win, SIGNAL(orientationAngleChanged(M::OrientationAngle)));

    // create the scene manager
    win->sceneManager();
    M::OrientationAngle newAngle = (M::OrientationAngle)(win->orientationAngle() + 180);

    win->setOrientationAngle(newAngle);

    QCOMPARE(orientationSpy.count(), 0);
    QCOMPARE(angleSpy.count(), 1);
}

void Ut_MWindow::testIsOnDisplay()
{
    MOnDisplayChangeEvent ev1(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    QVERIFY(win->isOnDisplay() == false);

    MOnDisplayChangeEvent ev2(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(win->isOnDisplay() == true);
}

void Ut_MWindow::testEnterDisplayEventHandler()
{
    Ut_MWindow::m_onDisplayHandlerCalled = false;

    MOnDisplayChangeEvent ev1(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    MOnDisplayChangeEvent ev2(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(Ut_MWindow::m_onDisplayHandlerCalled == true);
}

void Ut_MWindow::testExitDisplayEventHandler()
{
    Ut_MWindow::m_onDisplayHandlerCalled = false;

    MOnDisplayChangeEvent ev1(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    MOnDisplayChangeEvent ev2(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(Ut_MWindow::m_onDisplayHandlerCalled == true);
}

void Ut_MWindow::testDisplayEnteredSignal()
{
    connect(win, SIGNAL(displayEntered()), this, SLOT(onDisplayTestSlot()));

    Ut_MWindow::m_onDisplaySignalSent = false;

    MOnDisplayChangeEvent ev1(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    MOnDisplayChangeEvent ev2(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(Ut_MWindow::m_onDisplaySignalSent == true);

    disconnect(win, SIGNAL(displayEntered()), this, SLOT(onDisplayTestSlot()));
}

void Ut_MWindow::testDisplayExitedSignal()
{
    connect(win, SIGNAL(displayExited()), this, SLOT(onDisplayTestSlot()));

    Ut_MWindow::m_onDisplaySignalSent = false;

    MOnDisplayChangeEvent ev1(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    MOnDisplayChangeEvent ev2(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(Ut_MWindow::m_onDisplaySignalSent == true);

    disconnect(win, SIGNAL(displayExited()), this, SLOT(onDisplayTestSlot()));
}

void Ut_MWindow::onDisplayTestSlot()
{
    Ut_MWindow::m_onDisplaySignalSent = true;
}

void Ut_MWindow::testDisplayExitedOnClose()
{
    win->show();
    QSignalSpy spy(win, SIGNAL(displayExited()));
    win->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_MWindow::testDisplayExitedOnCloseLazyShutdownApp()
{
    MApplication::setPrestartMode(M::LazyShutdown);
    win->show();
    QSignalSpy spy(win, SIGNAL(displayExited()));
    win->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_MWindow::testCloseOnLazyShutdown()
{
    win->setCloseOnLazyShutdown(true);
    QVERIFY(win->closeOnLazyShutdown() == true);
    win->setCloseOnLazyShutdown(false);
    QVERIFY(win->closeOnLazyShutdown() == false);
}

void Ut_MWindow::testGlobalAlpha()
{
    qreal alpha = win->globalAlpha();
    QCOMPARE(alpha, 1.0);

    win->setGlobalAlpha(0.5);
    alpha = win->globalAlpha();
    QVERIFY( qAbs(alpha - 0.5) < 0.001 );

    win->setGlobalAlpha(0.0);
    alpha = win->globalAlpha();
    QCOMPARE(alpha, 0.0);

    win->setGlobalAlpha(1.0);
    alpha = win->globalAlpha();
    QCOMPARE(alpha, 1.0);

    win->setGlobalAlpha(-2.0);
    alpha = win->globalAlpha();
    QCOMPARE(alpha, 1.0);
}

QTEST_MAIN(Ut_MWindow);
