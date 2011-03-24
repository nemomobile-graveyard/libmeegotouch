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

#ifndef UT_MSCENEWINDOW_H
#define UT_MSCENEWINDOW_H

#include <QtTest/QtTest>
#include <QObject>
#include <MSceneWindow>
#include <MComponentData>

class MWindow;

class Ut_MSceneWindow : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testAccessors();

    void testAppearOnWindow();
    void testAppearOnScene();
    void testAppearWithoutSceneManager();

    void testDismiss();

    void testDestroyWhenDoneCallingDisappear();
    void testDestroyWhenDoneCallingDismiss();
    void testDestroyWhenDismissedCallingDisappear();
    void testDestroyWhenDismissedCallingDismiss();
    void testDismissedStateReset();

    void testSceneWindowStateChanges();
    void testSettingSameSceneWindowState();

    void testFocusHandlingOnWindowBlocking();

private:
    void processPendingEvents();
    bool verifySceneWindowStateChange(QSignalSpy &spy,
            MSceneWindow::SceneWindowState newState,
            MSceneWindow::SceneWindowState oldState);

    MSceneWindow *m_subject;
    MWindow *window;
    MComponentData *m_componentData;
};

class TestBridge : public QObject
{
    Q_OBJECT
public:
    TestBridge(QObject *parent = 0);
    void setSceneWindowState(MSceneWindow::SceneWindowState newState);
};

Q_DECLARE_METATYPE(MSceneWindow::WindowType);
Q_DECLARE_METATYPE(MSceneWindow::DeletionPolicy);

#endif
