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

#ifndef UT_MWINDOW_H
#define UT_MWINDOW_H

#include <QtTest>
#include <QObject>
#include <MNamespace>
#include <MComponentData>
#include <MSceneManager>

class MWindow;
class MApplication;

#define MAX_PARAMS 10
class Ut_MWindow: public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testNoSceneManager();
    void testNoScene();
    void testSceneManagerAutocreation();
    void testIfSceneExistsWhenSceneManagerAutocreated();
    void testConstructorWithSceneManager();
    void testSetSceneManager();
    void testSceneRect();
    void testOrientation_data();
    void testOrientation();
    void testSetOrientationAngle_data();
    void testSetOrientationAngle();
    void testSetOrientation();
    void testSetOrientationAngleCalledFromSceneManager();
    void testVisibleSceneSize_data();
    void testVisibleSceneSize();
    void testOrientationChangedSignalPropagationFromSceneManager();
    void testNoOrientationChangedSignalWhenRotatingBy180Degrees();
    void testOrientationAngleLock();
    void testOrientationLock();
    void testIsOnDisplay();
    void testEnterDisplayEventHandler();
    void testExitDisplayEventHandler();
    void testDisplayEnteredSignal();
    void testDisplayExitedSignal();
    void testDisplayExitedOnClose();
    void testDisplayExitedOnCloseLazyShutdownApp();
    void testSwitcherExitedOnClose();
    void testIsInSwitcher();
    void testCloseOnLazyShutdown();
    void testGlobalAlpha();
    void testVideoGlobalAlpha();

    void testSetLandscapeOrientation_data();
    void testSetLandscapeOrientation();
    void testSetPortraitOrientation_data();
    void testSetPortraitOrientation();

    void testAnimatedOrientationChangeProperty_data();
    void testAnimatedOrientationChangeProperty();

    void testActiveWindow();

    void testNotificationPreviewsDisabled();

public slots:
    void onDisplayTestSlot();

public:
    static bool m_onDisplayHandlerCalled;
    static bool m_onDisplaySignalSent;
private:
    MComponentData* m_componentData;
};

#endif
