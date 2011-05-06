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

#ifndef UT_MSCENEMANAGER_H
#define UT_MSCENEMANAGER_H

#include <QtTest/QtTest>
#include <QObject>

#include <MSceneManager>
#include <MSceneWindow>
#include <MNamespace>
#include <MComponentData>

class MWindow;

#define MAX_PARAMS 10
class Ut_MSceneManager: public QObject
{
    Q_OBJECT

private:
    MSceneManager *sm;
    MWindow *mWindow;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructorWithNoScene();
    void testConstructorWithSceneSpecified();
    void testAddSceneWindow();
    void testRemoveSceneWindow();
    void testSceneWindowAppearNow();
    void testSceneWindowAppear();
    void testSceneWindowDisappearNow();
    void testSceneWindowDisappear();
    void testSceneLayerEffect();
    void testInitialOrientation();
    void testOrientationChangedSignal();
    void testNoOrientationChangedSignalWhenRotatingBy180Degrees();
    void testOrientationChangeVisibility();
    //void testNavBarDockWidgetVisibility();
    void testAngleBoundaryCases();
    void testSceneSizes();
    void testPageSwitchingOnAppearNow();
    void testPageSwitchingOnAppear();
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
    void testTransitionModeWhenOffDisplay();
    void testTransitionModeWhenOnDisplay();
    void testTransitionModeWhenNoWindow();
    void testPageSwitchQueue();
    void testSceneWindowTransitionQueue_disappearWhileAppearing();
    void testSceneWindowTransitionQueue_disappearAndAppearWhileAppearing();
    void testSceneWindowTransitionQueue_disappearAndAppearWhileDisappearing_ApplicationPage();
    void testSceneWindowTransitionQueue_appearWhileDisappearing();
    void testSceneWindowTransitionQueue_appearAndDisappearWhileDisappearing();
    void testSceneWindowTransitionQueue_appearAndDisappearWhileDisappearing_ApplicationPage();
    void testCallingAppearOnAppearingPageWhenTransitionQueueHasOtherPages();
    void testDismissPageThatIsReappearing();
    void testDeletePageThatIsAppearing();
    void testDeletePageThatIsDisappearing();
#ifdef Q_WS_X11
    void testStatusBarGeometryProperty();
#endif
    void testSceneWindowsBehindSheetGetHiddenUponSheetAppearance();
    void testSceneWindowsBehindSheetAreShownWhenSheetAboutToDisappear();
    void testSceneWindowsBehindFirstSheetAreKeptHiddenWhenSecondSheetDisappears();
    void testDisappearingFirstSheetDoesNotAffectOthersVisibility();

    // Tests that disappeared scene windows do not get MOrientationChange events.
    // That's an optimization since it's a waste of resources to keep up to date
    // scene windows that are not being displayed anyway.
    void testDisappearedSceneWindowsDoNotGetOrientationChangeEvents();
private:
    MComponentData* m_componentData;
};

class EventSpy : public QObject
{
    Q_OBJECT
public:
    EventSpy() {}
    virtual ~EventSpy() {}

    virtual bool eventFilter (QObject *watched, QEvent *event) {
        Q_UNUSED(watched)
        eventTypesReceived << event->type();
        return false;
    }

    QList<QEvent::Type> eventTypesReceived;
};

#endif
