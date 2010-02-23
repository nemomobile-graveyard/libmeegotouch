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

#include <DuiApplication>
#include <DuiWindow>
#include <DuiComponentData>
#include <DuiSceneManager>
#include "duiondisplaychangeevent.h"

#include "ut_duiwindow.h"

DuiWindow *win;

// Variables for onDisplay -tests
bool Ut_DuiWindow::m_onDisplayHandlerCalled = false;
bool Ut_DuiWindow::m_onDisplaySignalSent    = false;

// DuiDeviceProfile stubs
class DuiDeviceProfile
{
public:
    static DuiDeviceProfile *instance();
    QSize resolution() const;
};

DuiDeviceProfile *DuiDeviceProfile::instance()
{
    static DuiDeviceProfile p;
    return &p;
}

QSize DuiDeviceProfile::resolution() const
{
    return QSize(1000, 500);
}

// DuiComponentData stubs
bool DuiComponentData::softwareRendering()
{
    return true;
}

bool DuiComponentData::fullScreen()
{
    return false;
}

void DuiComponentData::registerWindow(DuiWindow *window)
{
    Q_UNUSED(window);
}

void DuiComponentData::unregisterWindow(DuiWindow *window)
{
    Q_UNUSED(window);
}

void DuiComponentData::setActiveWindow(DuiWindow *window)
{
    Q_UNUSED(window);
}

void DuiComponentData::setPrestarted(bool flag)
{
    Q_UNUSED(flag);
}

QList<DuiWindow *> DuiComponentData::windows()
{
    QList<DuiWindow *> windowlist;
    return windowlist;
}

// DuiApplication stubs
DuiWindow *DuiApplication::activeWindow()
{
    return 0;
}

static Dui::PrestartMode fakeMode = Dui::LazyShutdown;

bool DuiApplication::isPrestarted()
{
    if ((fakeMode == Dui::LazyShutdown) || (fakeMode == Dui::TerminateOnClose))
        return true;

    return false;
}

void DuiApplication::setPrestartMode(Dui::PrestartMode mode)
{
    fakeMode = mode;
    return;
}

Dui::PrestartMode DuiApplication::prestartMode()
{
    return fakeMode;
}

bool DuiComponentData::emulateTwoFingerGestures()
{
    return false;
}

// DuiSceneManager stubs
Dui::OrientationAngle gOrientationAngle = Dui::Angle0;

Dui::OrientationAngle DuiSceneManager::orientationAngle() const
{
    return gOrientationAngle;
}

Dui::Orientation DuiSceneManager::orientation() const
{
    return (gOrientationAngle == Dui::Angle90 || gOrientationAngle == Dui::Angle270) ? Dui::Portrait : Dui::Landscape;
}

void DuiSceneManager::setOrientationAngle(Dui::OrientationAngle angle, DuiSceneManager::OrientationChangeMode mode)
{
    Q_UNUSED(mode);

    Dui::Orientation newOrientation = (angle == Dui::Angle90 || angle == Dui::Angle270)
                                      ? Dui::Portrait
                                      : Dui::Landscape;

    if (orientationAngle() != angle)
        emit orientationAngleChanged(angle);
    if (orientation() != newOrientation)
        emit orientationChanged(newOrientation);
    gOrientationAngle = angle;
}

// DuiWindows' visibility handler re-imps
void DuiWindow::enterDisplayEvent()
{
    Ut_DuiWindow::m_onDisplayHandlerCalled = true;
}

void DuiWindow::exitDisplayEvent()
{
    Ut_DuiWindow::m_onDisplayHandlerCalled = true;
}

// Test class implementation
void Ut_DuiWindow::init()
{
    win = new DuiWindow;
}

void Ut_DuiWindow::cleanup()
{
    delete win;
    win = 0;
}

void Ut_DuiWindow::testNoSceneManager()
{
    const DuiWindow *const_win = const_cast<DuiWindow *>(win);
    QCOMPARE((quintptr) const_win->sceneManager(), (quintptr) 0);
}

void Ut_DuiWindow::testNoScene()
{
    QCOMPARE((quintptr) win->scene(), (quintptr) 0);
}

void Ut_DuiWindow::testSceneManagerAutocreation()
{
    QVERIFY(win->sceneManager() != 0);
}

void Ut_DuiWindow::testIfSceneExistsWhenSceneManagerAutocreated()
{
    win->sceneManager();
    QCOMPARE((quintptr) win->scene(), (quintptr) win->sceneManager()->scene());
}

void Ut_DuiWindow::testConstructorWithSceneManager()
{
    DuiSceneManager *manager = new DuiSceneManager;
    const DuiWindow *const_win = new DuiWindow(manager);
    QCOMPARE((quintptr) const_win->sceneManager(), (quintptr) manager);
    delete const_win;
}

void Ut_DuiWindow::testSetSceneManager()
{
    DuiSceneManager *manager = new DuiSceneManager;
    win->setSceneManager(manager);

    QCOMPARE((quintptr) win->sceneManager(), (quintptr) manager);
}

void Ut_DuiWindow::testOrientation_data()
{
    QTest::addColumn<Dui::OrientationAngle>("newAngle");
    QTest::addColumn<Dui::Orientation>("newOrientation");
    QTest::newRow("Angle0")   << Dui::Angle0   << Dui::Landscape;
    QTest::newRow("Angle90")  << Dui::Angle90  << Dui::Portrait;
    QTest::newRow("Angle180") << Dui::Angle180 << Dui::Landscape;
    QTest::newRow("Angle270") << Dui::Angle270 << Dui::Portrait;
    QTest::newRow("Angle360") << Dui::Angle0   << Dui::Landscape;
}

void Ut_DuiWindow::testOrientation()
{
    QFETCH(Dui::OrientationAngle, newAngle);
    QFETCH(Dui::Orientation, newOrientation);

    win->setOrientationAngle(newAngle);
    QCOMPARE(win->orientation(), newOrientation);
}

void Ut_DuiWindow::testSetOrientationAngle_data()
{
    QTest::addColumn<Dui::OrientationAngle>("newAngle");
    QTest::newRow("Angle0")   << Dui::Angle0;
    QTest::newRow("Angle90")  << Dui::Angle90;
    QTest::newRow("Angle180") << Dui::Angle180;
    QTest::newRow("Angle270") << Dui::Angle270;
    QTest::newRow("Angle360") << Dui::Angle0;
}

void Ut_DuiWindow::testSetOrientationAngle()
{
    QFETCH(Dui::OrientationAngle, newAngle);

    win->setOrientationAngle(newAngle);
    QCOMPARE(win->orientationAngle(), newAngle);
}

void Ut_DuiWindow::testVisibleSceneSize_data()
{
    QTest::addColumn<Dui::OrientationAngle>("newAngle");
    QTest::addColumn<Dui::Orientation>("newOrientation");
    QTest::newRow("Angle0")   << Dui::Angle0   << Dui::Landscape;
    QTest::newRow("Angle90")  << Dui::Angle90  << Dui::Portrait;
    QTest::newRow("Angle180") << Dui::Angle180 << Dui::Landscape;
    QTest::newRow("Angle270") << Dui::Angle270 << Dui::Portrait;
    QTest::newRow("Angle360") << Dui::Angle0   << Dui::Landscape;
}

void Ut_DuiWindow::testVisibleSceneSize()
{
    QFETCH(Dui::OrientationAngle, newAngle);
    QFETCH(Dui::Orientation, newOrientation);

    win->setOrientationAngle(newAngle);

    QSize vss = win->visibleSceneSize();

    if (newOrientation == Dui::Portrait)
        vss.transpose();

    QCOMPARE(vss, DuiDeviceProfile::instance()->resolution());
}

void Ut_DuiWindow::testOrientationChangedSignalPropagationFromSceneManager()
{
    QSignalSpy orientationSpy(win, SIGNAL(orientationChanged(Dui::Orientation)));
    QSignalSpy angleSpy(win, SIGNAL(orientationAngleChanged(Dui::OrientationAngle)));

    // create the scene manager
    win->sceneManager();
    Dui::OrientationAngle newAngle = (Dui::OrientationAngle)(win->orientationAngle() + 90);

    win->sceneManager()->setOrientationAngle(newAngle, DuiSceneManager::ImmediateOrientationChange);

    QCOMPARE(orientationSpy.count(), 1);
    QCOMPARE(angleSpy.count(), 1);
}

void Ut_DuiWindow::testNoOrientationChangedSignalWhenRotatingBy180Degrees()
{
    QSignalSpy orientationSpy(win, SIGNAL(orientationChanged(Dui::Orientation)));
    QSignalSpy angleSpy(win, SIGNAL(orientationAngleChanged(Dui::OrientationAngle)));

    // create the scene manager
    win->sceneManager();
    Dui::OrientationAngle newAngle = (Dui::OrientationAngle)(win->orientationAngle() + 180);

    win->setOrientationAngle(newAngle);

    QCOMPARE(orientationSpy.count(), 0);
    QCOMPARE(angleSpy.count(), 1);
}

void Ut_DuiWindow::testIsOnDisplay()
{
    DuiOnDisplayChangeEvent ev1(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    QVERIFY(win->isOnDisplay() == false);

    DuiOnDisplayChangeEvent ev2(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(win->isOnDisplay() == true);
}

void Ut_DuiWindow::testEnterDisplayEventHandler()
{
    Ut_DuiWindow::m_onDisplayHandlerCalled = false;

    DuiOnDisplayChangeEvent ev1(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    DuiOnDisplayChangeEvent ev2(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(Ut_DuiWindow::m_onDisplayHandlerCalled == true);
}

void Ut_DuiWindow::testExitDisplayEventHandler()
{
    Ut_DuiWindow::m_onDisplayHandlerCalled = false;

    DuiOnDisplayChangeEvent ev1(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    DuiOnDisplayChangeEvent ev2(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(Ut_DuiWindow::m_onDisplayHandlerCalled == true);
}

void Ut_DuiWindow::testDisplayEnteredSignal()
{
    connect(win, SIGNAL(displayEntered()), this, SLOT(onDisplayTestSlot()));

    Ut_DuiWindow::m_onDisplaySignalSent = false;

    DuiOnDisplayChangeEvent ev1(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    DuiOnDisplayChangeEvent ev2(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(Ut_DuiWindow::m_onDisplaySignalSent == true);

    disconnect(win, SIGNAL(displayEntered()), this, SLOT(onDisplayTestSlot()));
}

void Ut_DuiWindow::testDisplayExitedSignal()
{
    connect(win, SIGNAL(displayExited()), this, SLOT(onDisplayTestSlot()));

    Ut_DuiWindow::m_onDisplaySignalSent = false;

    DuiOnDisplayChangeEvent ev1(true, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev1);

    DuiOnDisplayChangeEvent ev2(false, QRectF(QPointF(0, 0), win->visibleSceneSize()));
    win->event(&ev2);

    QVERIFY(Ut_DuiWindow::m_onDisplaySignalSent == true);

    disconnect(win, SIGNAL(displayExited()), this, SLOT(onDisplayTestSlot()));
}

void Ut_DuiWindow::onDisplayTestSlot()
{
    Ut_DuiWindow::m_onDisplaySignalSent = true;
}

void Ut_DuiWindow::testDisplayExitedOnClose()
{
    win->show();
    QSignalSpy spy(win, SIGNAL(displayExited()));
    win->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_DuiWindow::testDisplayExitedOnCloseLazyShutdownApp()
{
    DuiApplication::setPrestartMode(Dui::LazyShutdown);
    win->show();
    QSignalSpy spy(win, SIGNAL(displayExited()));
    win->close();
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(Ut_DuiWindow);
