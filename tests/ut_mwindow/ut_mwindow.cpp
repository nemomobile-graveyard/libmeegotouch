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

Q_DECLARE_METATYPE(MSceneManager::TransitionMode)

bool MDeviceProfile::orientationAngleIsSupported(M::OrientationAngle angle, bool isKeyboardOpen) const
{
    Q_UNUSED(isKeyboardOpen);
    if (angle == M::Angle270)
        return true;
    return false;
}

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
        static int argc = 1;
        static char *argv[ 1 ];
        argv[ 0 ] = (char*)"./ut_mwindow";
        m_componentData = MComponentData::createInstance(argc, argv);
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

void Ut_MWindow::testSetOrientation()
{
    win->setLandscapeOrientation();
    QVERIFY(win->orientationAngle() == M::Angle0 || win->orientationAngle() == M::Angle180);

    win->setPortraitOrientation();
    QVERIFY(win->orientationAngle() == M::Angle90 || win->orientationAngle() == M::Angle270);

    win->setLandscapeOrientation();
    QVERIFY(win->orientationAngle() == M::Angle0 || win->orientationAngle() == M::Angle180);
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
    M::OrientationAngle newAngle = (M::OrientationAngle)(0);

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

void Ut_MWindow::testOrientationAngleLock()
{
    win->setOrientationAngleLocked(true);
    QCOMPARE(true, win->d_func()->orientationAngleLocked);
    QCOMPARE(true, win->isOrientationAngleLocked());

    win->setOrientationAngleLocked(false);
    QCOMPARE(false, win->d_func()->orientationAngleLocked);
    QCOMPARE(false, win->isOrientationAngleLocked());

    win->lockOrientationAngle();
    QCOMPARE(true, win->d_func()->orientationAngleLocked);
    QCOMPARE(true, win->isOrientationAngleLocked());

    win->unlockOrientationAngle();
    QCOMPARE(false, win->d_func()->orientationAngleLocked);
    QCOMPARE(false, win->isOrientationAngleLocked());
}

void Ut_MWindow::testOrientationLock()
{
    win->setOrientationLocked(true);
    QCOMPARE(true, win->d_func()->orientationLocked);
    QCOMPARE(true, win->isOrientationLocked());

    win->setOrientationLocked(false);
    QCOMPARE(false, win->d_func()->orientationLocked);
    QCOMPARE(false, win->isOrientationLocked());

    win->lockOrientation();
    QCOMPARE(true, win->d_func()->orientationLocked);
    QCOMPARE(true, win->isOrientationLocked());

    win->unlockOrientation();
    QCOMPARE(false, win->d_func()->orientationLocked);
    QCOMPARE(false, win->isOrientationLocked());
}

void Ut_MWindow::testIsOnDisplay()
{
    // Should be initially false
    QVERIFY(win->isOnDisplay() == false);

    MOnDisplayChangeEvent ev2(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(win->isOnDisplay() == true);

    MOnDisplayChangeEvent ev1(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    // Sending displayExited should be delayed, wait a bit
    sleep(1.2);
    MApplication::processEvents();

    QVERIFY(win->isOnDisplay() == false);
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

    // sending displayExited should not be delayed
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

void Ut_MWindow::testSwitcherExitedOnClose()
{
    // Test that switcherExited() is sent when
    // a minimized application is closed
    win->showMinimized();
    QSignalSpy spy(win, SIGNAL(switcherExited()));
    win->close();
    QCOMPARE(spy.count(), 1);
    QVERIFY(win->isInSwitcher() == false);

    // Test that switcherExited() is NOT sent when
    // a maximized application is closed
    win->show();
    QSignalSpy spy2(win, SIGNAL(switcherExited()));
    win->close();
    QCOMPARE(spy2.count(), 0);
}

void Ut_MWindow::testIsInSwitcher()
{
    win->show();

    QSignalSpy spy(win, SIGNAL(switcherEntered()));
    win->showMinimized();
    QCOMPARE(spy.count(), 1);
    QVERIFY(win->isInSwitcher() == true);

    QSignalSpy spy2(win, SIGNAL(switcherExited()));
    win->showMaximized();
    QCOMPARE(spy2.count(), 1);
    QVERIFY(win->isInSwitcher() == false);
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

void Ut_MWindow::testVideoGlobalAlpha()
{
    qreal alpha = win->videoGlobalAlpha();
    QCOMPARE(alpha, 1.0);

    win->setVideoGlobalAlpha(0.5);
    alpha = win->videoGlobalAlpha();
    QVERIFY( qAbs(alpha - 0.5) < 0.001 );

    win->setVideoGlobalAlpha(0.0);
    alpha = win->videoGlobalAlpha();
    QCOMPARE(alpha, 0.0);

    win->setVideoGlobalAlpha(1.0);
    alpha = win->videoGlobalAlpha();
    QCOMPARE(alpha, 1.0);

    win->setVideoGlobalAlpha(-2.0);
    alpha = win->videoGlobalAlpha();
    QCOMPARE(alpha, 1.0);
}

void Ut_MWindow::testSetLandscapeOrientation_data()
{
    QTest::addColumn<M::OrientationAngle>("initialAngle");
    QTest::addColumn<M::OrientationAngle>("landscapeAngle");
    QTest::newRow("0->0")     << M::Angle0   << M::Angle0;
    QTest::newRow("90->0")    << M::Angle90  << M::Angle0;
    QTest::newRow("180->180") << M::Angle180 << M::Angle180;
    QTest::newRow("270->0")   << M::Angle270 << M::Angle0;
}

void Ut_MWindow::testSetLandscapeOrientation()
{
    QFETCH(M::OrientationAngle, initialAngle);
    QFETCH(M::OrientationAngle, landscapeAngle);

    win->setOrientationAngle(initialAngle);
    win->setLandscapeOrientation();
    QCOMPARE(win->orientationAngle(), landscapeAngle);
}

void Ut_MWindow::testSetPortraitOrientation_data()
{
    QTest::addColumn<M::OrientationAngle>("initialAngle");
    QTest::addColumn<M::OrientationAngle>("portraitAngle");
    QTest::newRow("0->270")   << M::Angle0   << M::Angle270;
    QTest::newRow("90->90")   << M::Angle90  << M::Angle90;
    QTest::newRow("180->270") << M::Angle180 << M::Angle270;
    QTest::newRow("270->270") << M::Angle270 << M::Angle270;
}

void Ut_MWindow::testSetPortraitOrientation()
{
    QFETCH(M::OrientationAngle, initialAngle);
    QFETCH(M::OrientationAngle, portraitAngle);

    win->setOrientationAngle(initialAngle);
    win->setPortraitOrientation();
    QCOMPARE(win->orientationAngle(), portraitAngle);
}

void Ut_MWindow::testAnimatedOrientationChangeProperty_data()
{
    QTest::addColumn<bool>("defineAnimatedOrientationChangeProperty");
    QTest::addColumn<bool>("propertyValue");
    QTest::addColumn<bool>("setVisibleValue");
    QTest::addColumn<MSceneManager::TransitionMode>("expectedTransitionMode");

    QTest::newRow("invisible window, undefined property")
            << false << false << false << MSceneManager::ImmediateTransition;

    QTest::newRow("invisible window, property false")
            << true << false << false << MSceneManager::ImmediateTransition;

    QTest::newRow("invisible window, property true")
            << true << true << false << MSceneManager::ImmediateTransition;

    QTest::newRow("visible window, property true")
            << true << true << true << MSceneManager::AnimatedTransition;

    QTest::newRow("visible window, property false")
            << true << false << true << MSceneManager::ImmediateTransition;

    QTest::newRow("visible window, property undefined")
            << false << false << true << MSceneManager::AnimatedTransition;
}

void Ut_MWindow::testAnimatedOrientationChangeProperty()
{
    QFETCH(bool, defineAnimatedOrientationChangeProperty);
    QFETCH(bool, propertyValue);
    QFETCH(bool, setVisibleValue);
    QFETCH(MSceneManager::TransitionMode, expectedTransitionMode);

    if (defineAnimatedOrientationChangeProperty)
        win->setProperty("animatedOrientationChange", propertyValue);

    win->setVisible(setVisibleValue);

    MSceneManager::TransitionMode actualTransitionMode =
            static_cast<MSceneManager::TransitionMode>(win->orientationChangeTransitionMode());
    QCOMPARE(actualTransitionMode, expectedTransitionMode);
}

void  Ut_MWindow::testActiveWindow()
{
    QCOMPARE(MApplication::activeWindow(), win);
    MWindow *win2 = new MWindow;
    MWindow *win3 = new MWindow;

    win2->show();
    QCOMPARE(MApplication::activeWindow(), win2);

    win3->showMinimized();
    QCOMPARE(MApplication::activeWindow(), win2);

    delete win2;
    delete win3;
}

void Ut_MWindow::testNotificationPreviewsDisabled()
{
    bool visible = win->notificationPreviewsVisible();
    QVERIFY( visible );

    win->setNotificationPreviewsVisible(false);
    visible = win->notificationPreviewsVisible();
    QVERIFY( !visible );

    win->setNotificationPreviewsVisible(true);
    visible = win->notificationPreviewsVisible();
    QVERIFY( visible );
}

QTEST_MAIN(Ut_MWindow);
