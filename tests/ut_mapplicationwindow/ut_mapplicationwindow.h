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

#ifndef UT_MAPPLICATIONWINDOW
#define UT_MAPPLICATIONWINDOW

#include <QObject>
#include <QList>
#include <QtTest>
#include <MNamespace>
#include <MComponentData>

class QGraphicsItem;

class MApplicationWindow;
class MApplicationPage;
class MNavigationBar;
class MSceneWindow;

class Ut_MApplicationWindow : public QObject
{
    Q_OBJECT

public:
    static bool m_windowShown;
    static bool m_windowClosed;
    static bool m_prestartRestored;

private slots:
    void init();   // Executed before each test function
    void cleanup();  // Executed after each test function
    void initTestCase(); // Executed before all
    void cleanupTestCase(); // Executed after all tests have been run

    void testConstructorWithoutScene();
    void testConstructorWithScene();
    void testPageChanged();
    void testWindowActivate();

    void testSetWindowIconID();
    void testCurrentPage();
    void testMenu();
    void testIsMenuOpen();

    void testIsOnDisplay();
    void testPrestartNoPrestart();
    void testPrestartTerminateOnClose();
    void testPrestartLazyShutdown();

    void testDisplayExitedOnClose();
    void testDisplayExitedOnCloseLazyShutdownApp();

    void testPageEscape();
    void testPageEscapeAuto();
    void testPageEscapeAutoWhenAddingPageHistory();
    void testPageEscapeAutoWhenClearingPageHistory();

    void testComponentsDisplayMode();

    void testStatusBarVisibility_data();
    void testStatusBarVisibility();

    void testToolBarInLandscapeIsFloating();
    void testToolBarInPortraiIsInNavigationBar();
    void testToolBarMovesFromFloatingToNavigationBarWhenRotatedToPortrait();
    void testTabBarInLandscapeIsInNavigationBar();
    void testTabBarInPortraitIsFloating();
    void testTabBarMovesFromNavigationBarToFloatingWhenRotatedToPortrait();

    void testNavigationBarVisibility_data();
    void testNavigationBarVisibility();

    void testNavigationBarVisibilityFloatableTabbar();
    void testNavigationBarVisibilityDockedTabbar();
    void testNavigationBarVisibilityHideToolbarAction();

    void testFirstPageWithCustomNavigationBarContent();
    void testNavigationBarOpacity();

    void testGoingBackDoesntMakeNavigationBarDisappearAndReappear();

    void testNavigationBarInOutAnimations_data();
    void testNavigationBarInOutAnimations();

    void testNavigationBarShowAfterNonAnimatedPageAppearance();

private:
    MApplicationWindow *m_subject;
    MComponentData *m_componentData;

    void initToolbarLocationTC(M::OrientationAngle angle, const QString &viewType);
    bool isToolBarFloating();
    void processEventsAndFastForwardDisappearAppearAnimations(MSceneWindow* sceneWindow);
};


enum StatusBarTestOperation {
    MakeFullScreen,
    MakeNormal,
    MaximizePageArea,
    RestorePageArea
};

Q_DECLARE_METATYPE(MApplicationPage *);
Q_DECLARE_METATYPE(QList<StatusBarTestOperation>);

#endif
