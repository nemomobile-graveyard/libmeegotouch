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

#ifndef UT_DUISCENEMANAGER_H
#define UT_DUISCENEMANAGER_H

#include <QtTest/QtTest>
#include <QObject>

#include <DuiSceneManager>
#include <DuiSceneWindow>
#include <DuiNamespace>

class DuiWindow;

#define MAX_PARAMS 10
class Ut_DuiSceneManager: public QObject
{
    Q_OBJECT

private:
    DuiSceneManager *sm;
    DuiWindow *duiWindow;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructorWithNoScene();
    void testConstructorWithSceneSpecified();
    void testSceneWindowAttaching();
    void testSceneWindowDetaching();
    void testSceneWindowAppearNow();
    void testSceneWindowAppear();
    void testSceneWindowDisappearNow();
    void testSceneWindowDisappear();
    void testSceneLayerEffect();
    void testInitialOrientation();
    void testOrientationChangedSignal();
    void testNoOrientationChangedSignalWhenRotatingBy180Degrees();
    //void testNavBarDockWidgetVisibility();
    void testAngleBoundaryCases();
    void testSceneSizes();
    void testPageSwitchingOnAppearNow();
    void testPageSwitchingOnDismissNow();
    void testPageHistoryPushing();
    void testPageHistoryPopping();
    void testSettingPageHistory();
    void testDeletePageInPageHistory();
    void testPageHistoryAfterPageDeletion();
    void testPageHistoryChangedWhenPushing();
    void testPageHistoryChangedWhenPopping();
    void testPageHistoryChangedWhenSettingPageHistory();
    void testPageHistoryChangedWhenSettingSamePageHistory();
};

Q_DECLARE_METATYPE(Dui::Orientation)

#endif
