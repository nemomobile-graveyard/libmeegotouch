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

#ifndef UT_STATUSBARVIEW_H
#define UT_STATUSBARVIEW_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;

// the real unit/MStatusBarView class declaration
#include <mstatusbarview.h>

Q_DECLARE_METATYPE(MStatusBarView *);

class TestMStatusBarView : public MStatusBarView
{
    M_VIEW(MSceneWindowModel, MStatusBarStyle)

public:
    TestMStatusBarView(MStatusBar *controller)
        : MStatusBarView(controller)
    {}

    MStatusBarStyle *modifiableStyle() {
        MStatusBarStyleContainer &sc = style();
        const MStatusBarStyle *const_s = sc.operator ->();
        MStatusBarStyle *s = const_cast<MStatusBarStyle *>(const_s);
        return s;
    }
};

class Ut_MStatusBarView : public QObject
{
    Q_OBJECT
public:
    Ut_MStatusBarView();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test when mouse is moved over a certain threshold then status indicator menu appears.
    void testWhenMouseMovesAboveThresholdStatusIndicatorMenuAppears();
    // Test when swipe less than a particular amount, status indicator will not be triggered.
    void testWhenSwipeLessThanThresholdStatusIndicatorMenuDoesNotAppear();
    // Test when mouse is moved over a certain threshold then haptics done.
    void testWhenMousePressHapticsDone();
    // Thest when using swipe functionality, tapping the statusbar has no effect
    void testWhenUsingSwipeTapDoesNothing();
    // Thest when using tap functionality, swiping the statusbar has no effect
    void testWhenUsingTapSwipeDoesNothing();
    // Thest that the tap functionality works
    void testTapFunctionality();
    // Test that the status bar is dimmed when pressed
    void testPressDimming();
    // Test that the status indicator menu does not appear if it has been disabled from the style
    void testStatusIndicatorMenuDisabling();

#ifdef Q_WS_X11
    void testGettingPropertyWindowId();
    void testGettingSharedPixmapHandle_data();
    void testGettingSharedPixmapHandle();
    void testThatPropertyNewValueOfXPropertyNotifyEventFetchesPixmapHandleAndPropertyDeleteRemovesPixmapHandle();
    void testSharedPixmapHandleProviderOffline();
    void testEventFilterIsSetCorrectly();
    void testEventFilterWhenNoStatusBarInstance();
#endif

#ifdef HAVE_XDAMAGE
    void testTrackXDamageEventsForSharedPixmapWhenEnterDisplay();
    void testDestroyXDamageTrackingForSharedPixmapWhenExitDisplay();
    void testDestroyXDamageTrackingForSharedPixmapWhenEnterSwitcher();
#endif //HAVE_XDAMAGE


private:
    void mouseDownWorker(QPointF downAt);
    void mouseMoveWorker(QPointF moveTo);
    void mouseUpWorker(QPointF upAt);
    void initHelper();

#ifdef Q_WS_X11
    void createX11SharedPixmap();
    void freeX11SharedPixmap();
#endif

    TestMStatusBarView *m_subject;
    MStatusBar *m_statusbar;
    MApplication* app;
};

#endif
