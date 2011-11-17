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
#include <MWindow>
#include <MSceneManager>
#include <QTimerEvent>
#include <MDeviceProfile>
#include "mondisplaychangeevent.h"
#include "corelib/widgets/mwindow_p.h"

#include "ut_mwindow.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif //Q_WS_X11

Q_DECLARE_METATYPE(MSceneManager::TransitionMode)

class MyApplication : public MApplication
{
public:
    MyApplication(int &argc, char **argv) : MApplication(argc,argv) {}
#ifdef Q_WS_X11
    // make x11EventFilter public
    using MApplication::x11EventFilter;
#endif Q_WS_X11
};

bool MDeviceProfile::orientationAngleIsSupported(M::OrientationAngle angle, bool isKeyboardOpen) const
{
    Q_UNUSED(isKeyboardOpen);
    bool screenIsPortrait = MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Portrait;

    // We support only standard portrait orientation angle
    return (angle == (screenIsPortrait ? M::Angle0 : M::Angle270));
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
void Ut_MWindow::initTestCase()
{
    static int argc = 1;
    static char *argv[ 1 ];
    argv[ 0 ] = (char*)"./ut_mwindow";
    app = new MyApplication(argc, argv);

    m_portraitAngle = (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Portrait) ? M::Angle0 : M::Angle270;
    m_invertedPortraitAngle = (M::OrientationAngle)((m_portraitAngle + 180)%360);

    m_landscapeAngle = (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Portrait) ? M::Angle270 : M::Angle0;
    m_invertedLandscapeAngle = (M::OrientationAngle)((m_landscapeAngle + 180)%360);
}

void Ut_MWindow::init()
{
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
    QCOMPARE(win->sceneRect(), QRectF(QPoint(0,0), win->visibleSceneSize(MDeviceProfile::instance()->orientationFromAngle(M::Angle0))));
}

void Ut_MWindow::testOrientation_data()
{
    QTest::addColumn<M::OrientationAngle>("newAngle");
    QTest::newRow("Angle0")   << M::Angle0;
    QTest::newRow("Angle90")  << M::Angle90;
    QTest::newRow("Angle180") << M::Angle180;
    QTest::newRow("Angle270") << M::Angle270;
    QTest::newRow("Angle360") << M::Angle0;
}

void Ut_MWindow::testOrientation()
{
    QFETCH(M::OrientationAngle, newAngle);

    win->setOrientationAngle(newAngle);
    QCOMPARE(win->orientationAngle(), newAngle);
    QCOMPARE(win->orientation(), MDeviceProfile::instance()->orientationFromAngle(newAngle));
}

void Ut_MWindow::testSetOrientation()
{
    win->setLandscapeOrientation();
    QVERIFY(win->orientationAngle() == m_landscapeAngle || win->orientationAngle() == m_invertedLandscapeAngle);

    win->setPortraitOrientation();
    QVERIFY(win->orientationAngle() == m_portraitAngle || win->orientationAngle() == m_invertedPortraitAngle);

    win->setLandscapeOrientation();
    QVERIFY(win->orientationAngle() == m_landscapeAngle || win->orientationAngle() == m_invertedLandscapeAngle);
}

void Ut_MWindow::testSetOrientationAngleCalledFromSceneManager()
{
    win->setSceneManager(new MSceneManager);
    QCOMPARE(win->orientationAngle(), win->sceneManager()->orientationAngle());

    win->sceneManager()->setOrientationAngle(m_portraitAngle, MSceneManager::ImmediateTransition);
    win->setOrientationAngle(m_landscapeAngle);
    QCOMPARE(win->orientationAngle(), m_landscapeAngle);
    QCOMPARE(win->sceneManager()->orientationAngle(), m_landscapeAngle);
}

void Ut_MWindow::testVisibleSceneSize_data()
{
    QTest::addColumn<M::OrientationAngle>("newAngle");
    QTest::addColumn<M::Orientation>("newOrientation");
    QTest::newRow("Angle0")   << m_landscapeAngle   << M::Landscape;
    QTest::newRow("Angle90")  << m_portraitAngle  << M::Portrait;
    QTest::newRow("Angle180") << m_invertedLandscapeAngle << M::Landscape;
    QTest::newRow("Angle270") << m_invertedPortraitAngle << M::Portrait;
}

void Ut_MWindow::testVisibleSceneSize()
{
    QFETCH(M::OrientationAngle, newAngle);
    QFETCH(M::Orientation, newOrientation);

    win->setOrientationAngle(newAngle);

    QSize vss = win->visibleSceneSize();

    if (newOrientation == MDeviceProfile::instance()->orientationFromAngle(M::Angle90))
        vss.transpose();

    QCOMPARE(vss, MDeviceProfile::instance()->resolution());
}

void Ut_MWindow::testOrientationChangedSignalPropagationFromSceneManager()
{
    // create the scene manager
    win->sceneManager();

    win->sceneManager()->setOrientationAngle(m_landscapeAngle, MSceneManager::ImmediateTransition);

    QSignalSpy orientationSpy(win, SIGNAL(orientationChanged(M::Orientation)));
    QSignalSpy angleSpy(win, SIGNAL(orientationAngleChanged(M::OrientationAngle)));

    win->sceneManager()->setOrientationAngle(m_portraitAngle, MSceneManager::ImmediateTransition);

    QCOMPARE(orientationSpy.count(), 1);
    QCOMPARE(angleSpy.count(), 1);
}

void Ut_MWindow::testNoOrientationChangedSignalWhenRotatingBy180Degrees()
{
    // create the scene manager
    win->sceneManager();
    win->setOrientationAngle(m_landscapeAngle);

    QSignalSpy orientationSpy(win, SIGNAL(orientationChanged(M::Orientation)));
    QSignalSpy angleSpy(win, SIGNAL(orientationAngleChanged(M::OrientationAngle)));

    win->setOrientationAngle(m_invertedLandscapeAngle);

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

    // The emission of displayExited is delayed
    // Force a timeout on MWindow's internal timer so that the display change event
    // gets processed immediately.
    QTimerEvent timerEvent(win->d_func()->displayExitedTimer.timerId());
    win->d_func()->displayExitedTimer.event(&timerEvent);
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
    QTest::newRow("landscape->landscape")   << m_landscapeAngle   << m_landscapeAngle;
    QTest::newRow("inv. portrait->landscape")   << m_invertedPortraitAngle  << m_landscapeAngle;
    QTest::newRow("inv. landscape->inv. landscape") << m_invertedLandscapeAngle << m_invertedLandscapeAngle;
    QTest::newRow("portrait->landscape") << m_portraitAngle << m_landscapeAngle;
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
    QTest::newRow("landscape->portrait")   << m_landscapeAngle   << m_portraitAngle;
    QTest::newRow("inv. portrait->inv. portrait")   << m_invertedPortraitAngle  << m_invertedPortraitAngle;
    QTest::newRow("inv. landscape->portrait") << m_invertedLandscapeAngle << m_portraitAngle;
    QTest::newRow("portrait->portrait") << m_portraitAngle << m_portraitAngle;
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

void Ut_MWindow::testKeepOffDisplayWhenShownAfterReceivingFullyOffDisplayEvent()
{
    win->hide();

    // Tell the window that it is off display
    MOnDisplayChangeEvent event(MOnDisplayChangeEvent::FullyOffDisplay, QRectF());
    win->event(&event);

    QSignalSpy spyDisplayEntered(win, SIGNAL(displayEntered()));

    win->show();

    // The show() call shouldn't cause the window to presume it's (or will pretty
    // soon be) on display.
    //
    // A simple example is when window->show() is called while the physical display
    // is turned off.
    //
    // Even if the screen is on there could be other windows stacked higher therefore
    // only after a raise() it could go to the top. And in some cases, depending on the
    // window type and policy, even after a raise() the window might not end up being
    // the top most.
    QCOMPARE(spyDisplayEntered.count(), 0);
    QCOMPARE(win->isOnDisplay(), false);
}

// OBS: Here we want to test it without an underlying a scene manager.
void Ut_MWindow::testInvalidOrientationAngle()
{
    win->setOrientationAngle(M::Angle90);

    QCOMPARE(static_cast<int>(win->orientationAngle()), 90);

    win->setOrientationAngle(static_cast<M::OrientationAngle>(15));

    // Nothing should happen. Angle remains unchanged.
    QCOMPARE(static_cast<int>(win->orientationAngle()), 90);
}

#ifdef Q_WS_X11
void Ut_MWindow::testDisplayExitedWithoutDelayWhenWindowIsCoveredOnStartup()
{
    win->show();

    XEvent event;
    memset(&event, 0, sizeof(event));
    event.xvisibility.type = VisibilityNotify;
    event.xvisibility.state = VisibilityFullyObscured;
    event.xvisibility.window = win->winId();
    event.xvisibility.send_event = true;

    app->x11EventFilter(&event);

    QCOMPARE(win->isOnDisplay(), false);
}
#endif //Q_WS_X11

QTEST_APPLESS_MAIN(Ut_MWindow);
