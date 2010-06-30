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

#include <QMetaType>
#include <QEventLoop>
#include <QTimer>

#include <MSceneManager>
#include <MSceneWindow>

#include "ut_mscenemanager.h"

#include <MApplicationPage>
#include <MWindow>
#include <MNavigationBar>
#include <MMessageBox>
#include <MOverlay>
#include <MNavigationBar>
#include "mdockwidget.h"
#include <MScene>
#include "mscenemanager_p.h"

#include <MComponentData>
#include <mapplicationwindow.h>

bool gMWindowIsOnDisplay = false;
bool MWindow::isOnDisplay() const
{
    return gMWindowIsOnDisplay;
}

// Test class implementation

void Ut_MSceneManager::initTestCase()
{
    if(MComponentData::instance() == 0) {
        int argc = 1;
        char *argv[ 1 ];
        argv[ 0 ] = (char*)"./ut_mwindow";
        m_componentData = new MComponentData(argc, argv);
    }
    qRegisterMetaType<M::Orientation>("M::Orientation");
    qRegisterMetaType<M::Orientation>("M::OrientationAngle");
    mWindow = new MWindow;
}

void Ut_MSceneManager::cleanupTestCase()
{
    delete mWindow;
}

void Ut_MSceneManager::init()
{
    sm = new MSceneManager;
    mWindow->setSceneManager(sm);
}

void Ut_MSceneManager::cleanup()
{
    mWindow->setSceneManager(0);
}

void Ut_MSceneManager::testConstructorWithNoScene()
{
    QVERIFY(sm->scene());
}

void Ut_MSceneManager::testConstructorWithSceneSpecified()
{
    MScene *scene = new MScene;
    MSceneManager *manager = new MSceneManager(scene);
    QCOMPARE(manager->scene(), scene);
    delete manager;
}

void Ut_MSceneManager::testAddSceneWindow()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    testBridge.addSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);
    //QCOMPARE(sm->d_ptr->windows.size(), 1);
}

void Ut_MSceneManager::testRemoveSceneWindow()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    testBridge.addSceneWindow(sceneWindow);

    testBridge.removeSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->scene(), static_cast<const QGraphicsScene *>(0));
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    delete sceneWindow;
}

void Ut_MSceneManager::testSceneWindowAppearNow()
{
    MSceneWindow *sceneWindow = new MSceneWindow;

    gMWindowIsOnDisplay = true;
    mWindow->show();

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindowNow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(sceneWindow->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(sceneWindow->isOnDisplay(), true);
}

void Ut_MSceneManager::testSceneWindowAppear()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;
    mWindow->show();

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(sceneWindow->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(sceneWindow->isVisible(), true);

    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(sceneWindow->isOnDisplay(), true);
}

void Ut_MSceneManager::testSceneWindowDisappearNow()
{
    MNavigationBar *p = new MNavigationBar();

    sm->appearSceneWindowNow(p);
    sm->disappearSceneWindowNow(p);

    //QCOMPARE(sm->d_ptr->navBar, (MNavigationBar *)0);
    QCOMPARE(p->isVisible(), false);

    delete p;
}

void Ut_MSceneManager::testSceneWindowDisappear()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(sceneWindow->isOnDisplay(), true);
    QCOMPARE(sceneWindow->isVisible(), true);

    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(sceneWindow->isVisible(), true);

    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(sceneWindow->isVisible(), false);
    QCOMPARE(sceneWindow->isOnDisplay(), false);
}

void Ut_MSceneManager::testSceneLayerEffect()
{
    MDialog *m = new MMessageBox("test");
    MNavigationBar *n = new MNavigationBar();

    sm->appearSceneWindowNow(m);
    sm->appearSceneWindowNow(n);

    QVERIFY(m->parentItem() != 0);
    //QVERIFY(n->parentItem() == sm->d_ptr->rootElement);

    QCOMPARE((int)m->parentItem()->zValue(), (int)MSceneManagerPrivate::MessageBox);

    sm->disappearSceneWindowNow(m);
    sm->disappearSceneWindowNow(n);

    delete m;
    delete n;
}

void Ut_MSceneManager::testInitialOrientation()
{
    M::Orientation initialOrientation = sm->orientation();
    QCOMPARE(initialOrientation, M::Portrait);
}

void Ut_MSceneManager::testOrientationChangedSignal()
{
    QSignalSpy changedSpy(sm, SIGNAL(orientationChanged(M::Orientation)));
    QSignalSpy finishedSpy(sm, SIGNAL(orientationChangeFinished(M::Orientation)));

    int newAngle = sm->orientationAngle() + M::Angle90;
    newAngle %= 360;

    sm->setOrientationAngle((M::OrientationAngle) newAngle,
                            MSceneManager::ImmediateTransition);
    M::Orientation newOrientation = (newAngle == M::Angle0 || newAngle == M::Angle180)
                                      ? M::Landscape
                                      : M::Portrait;

    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(changedSpy.at(0).at(0).value<M::Orientation>(), newOrientation);
    QCOMPARE(finishedSpy.count(), 1);
    QCOMPARE(finishedSpy.at(0).at(0).value<M::Orientation>(), newOrientation);
}

void Ut_MSceneManager::testNoOrientationChangedSignalWhenRotatingBy180Degrees()
{
    QSignalSpy orientationChangedSpy(sm, SIGNAL(orientationChanged(M::Orientation)));
    QSignalSpy angleChangedSpy(sm, SIGNAL(orientationAngleChanged(M::OrientationAngle)));
    QSignalSpy finishedSpy(sm, SIGNAL(orientationChangeFinished(M::Orientation)));

    int newAngle = sm->orientationAngle() + M::Angle180;
    newAngle %= 360;

    sm->setOrientationAngle((M::OrientationAngle) newAngle,
                            MSceneManager::ImmediateTransition);

    QCOMPARE(orientationChangedSpy.count(), 0);
    QCOMPARE(angleChangedSpy.count(), 1);
    QCOMPARE(finishedSpy.count(), 1);
}

// Test uses non-exported symbol "MDockWidget".
/*
void Ut_MSceneManager::testNavBarDockWidgetVisibility()
{
    MNavigationBar *p = new MNavigationBar();
    MDockWidget *d = new MDockWidget();

    sm->appearSceneWindowNow(p);
    sm->appearSceneWindowNow(d);

    int newAngle = sm->orientationAngle() + M::Angle90;
    newAngle %= 360;

    sm->setOrientationAngle( (M::OrientationAngle) newAngle );

    QCOMPARE(p->isVisible(), true);
    QCOMPARE(d->isVisible(), true);

    sm->disappearSceneWindowNow(p);
    sm->disappearSceneWindowNow(d);

    delete p;
    delete d;
}
*/

void Ut_MSceneManager::testAngleBoundaryCases()
{
    QSignalSpy spyChanged(sm, SIGNAL(orientationChanged(M::Orientation)));

    int newAngle = sm->orientationAngle() + M::Angle270;
    newAngle %= 360;
    sm->setOrientationAngle((M::OrientationAngle) newAngle,
                            MSceneManager::ImmediateTransition);

    newAngle = sm->orientationAngle() + M::Angle90;
    newAngle %= 360;
    sm->setOrientationAngle((M::OrientationAngle) newAngle,
                            MSceneManager::ImmediateTransition);

    QCOMPARE(spyChanged.count(), 2);

}

void Ut_MSceneManager::testSceneSizes()
{
    QSize vSR = sm->visibleSceneSize();

    // check scene rects are non-zero
    QVERIFY(vSR.width() > 0);
    QVERIFY(vSR.height() > 0);

    sm->setOrientationAngle(M::Angle90, MSceneManager::ImmediateTransition);

    // check scenerects are in correct orientation
    // what about square?
    QCOMPARE(sm->orientation(), M::Portrait);
    QCOMPARE(sm->visibleSceneSize(), sm->visibleSceneSize(M::Portrait));
    vSR = sm->visibleSceneSize();
    QVERIFY(vSR.width() < vSR.height());

    // test other orientation
    vSR = sm->visibleSceneSize(M::Landscape);
    QVERIFY(vSR.width() > vSR.height());

    sm->setOrientationAngle(M::Angle180, MSceneManager::ImmediateTransition);

    QCOMPARE(sm->orientation(), M::Landscape);
    QCOMPARE(sm->visibleSceneSize(), sm->visibleSceneSize(M::Landscape));
    vSR = sm->visibleSceneSize();
    QVERIFY(vSR.width() > vSR.height());

    // test other orientation
    vSR = sm->visibleSceneSize(M::Portrait);
    QVERIFY(vSR.width() < vSR.height());
}

void Ut_MSceneManager::testPageSwitchingOnAppearNow()
{
    MApplicationPage firstPage;
    MApplicationPage secondPage;

    QSignalSpy firstPageAppeared(&firstPage, SIGNAL(appeared()));
    QSignalSpy firstPageDisappeared(&firstPage, SIGNAL(disappeared()));
    QSignalSpy secondPageAppeared(&secondPage, SIGNAL(appeared()));

    sm->appearSceneWindowNow(&firstPage);

    QCOMPARE(firstPageAppeared.count(), 1);
    QCOMPARE(firstPageDisappeared.count(), 0);
    QCOMPARE(secondPageAppeared.count(), 0);
    firstPageAppeared.clear();

    sm->appearSceneWindowNow(&secondPage);

    QCOMPARE(firstPageAppeared.count(), 0);
    QCOMPARE(firstPageDisappeared.count(), 1);
    QCOMPARE(secondPageAppeared.count(), 1);
}

void Ut_MSceneManager::testPageSwitchingOnAppear()
{
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;

    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;

    QSignalSpy firstPageAppearing(firstPage, SIGNAL(appearing()));
    QSignalSpy firstPageAppeared(firstPage, SIGNAL(appeared()));
    QSignalSpy firstPageDisappearing(firstPage, SIGNAL(disappearing()));
    QSignalSpy firstPageDisappeared(firstPage, SIGNAL(disappeared()));
    QSignalSpy secondPageAppearing(secondPage, SIGNAL(appearing()));
    QSignalSpy secondPageAppeared(secondPage, SIGNAL(appeared()));

    sm->appearSceneWindow(firstPage);

    QCOMPARE(firstPageAppearing.count(), 1);
    QCOMPARE(firstPageAppeared.count(), 0);
    QCOMPARE(firstPageDisappearing.count(), 0);
    QCOMPARE(firstPageDisappeared.count(), 0);
    QCOMPARE(secondPageAppearing.count(), 0);
    QCOMPARE(secondPageAppeared.count(), 0);
    firstPageAppearing.clear();

    testBridge.fastForwardPageSwitchAnimation();

    QCOMPARE(firstPageAppearing.count(), 0);
    QCOMPARE(firstPageAppeared.count(), 1);
    QCOMPARE(firstPageDisappearing.count(), 0);
    QCOMPARE(firstPageDisappeared.count(), 0);
    QCOMPARE(secondPageAppearing.count(), 0);
    QCOMPARE(secondPageAppeared.count(), 0);
    firstPageAppeared.clear();

    sm->appearSceneWindow(secondPage);

    QCOMPARE(firstPageAppearing.count(), 0);
    QCOMPARE(firstPageAppeared.count(), 0);
    QCOMPARE(firstPageDisappearing.count(), 1);
    QCOMPARE(firstPageDisappeared.count(), 0);
    QCOMPARE(secondPageAppearing.count(), 1);
    QCOMPARE(secondPageAppeared.count(), 0);
    firstPageDisappearing.clear();
    secondPageAppearing.clear();

    testBridge.fastForwardPageSwitchAnimation();

    QCOMPARE(firstPageAppearing.count(), 0);
    QCOMPARE(firstPageAppeared.count(), 0);
    QCOMPARE(firstPageDisappearing.count(), 0);
    QCOMPARE(firstPageDisappeared.count(), 1);
    QCOMPARE(secondPageAppearing.count(), 0);
    QCOMPARE(secondPageAppeared.count(), 1);
}

void Ut_MSceneManager::testPageSwitchingOnDismissNow()
{
    MApplicationPage firstPage;
    MApplicationPage secondPage;

    sm->appearSceneWindowNow(&firstPage);
    sm->appearSceneWindowNow(&secondPage);

    QSignalSpy firstPageAppeared(&firstPage, SIGNAL(appeared()));
    QSignalSpy firstPageDisappeared(&firstPage, SIGNAL(disappeared()));
    QSignalSpy secondPageAppeared(&secondPage, SIGNAL(appeared()));
    QSignalSpy secondPageDisappeared(&secondPage, SIGNAL(disappeared()));

    sm->dismissSceneWindowNow(&secondPage);

    QCOMPARE(firstPageAppeared.count(), 1);
    QCOMPARE(firstPageDisappeared.count(), 0);
    QCOMPARE(secondPageAppeared.count(), 0);
    QCOMPARE(secondPageDisappeared.count(), 1);
}

void Ut_MSceneManager::testPageHistoryPushing()
{
    MApplicationPage firstPage;
    MApplicationPage secondPage;
    MApplicationPage thirdPage;
    QList<MSceneWindow *> pageHistory;

    QCOMPARE(sm->pageHistory().count(), 0);

    sm->appearSceneWindowNow(&firstPage);

    QCOMPARE(sm->pageHistory().count(), 0);

    sm->appearSceneWindowNow(&secondPage);

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 1);
    QCOMPARE(pageHistory.at(0), &firstPage);

    sm->appearSceneWindowNow(&thirdPage);

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 2);
    QCOMPARE(pageHistory.at(0), &firstPage);
    QCOMPARE(pageHistory.at(1), &secondPage);
}

void Ut_MSceneManager::testPageHistoryPopping()
{
    MApplicationPage firstPage;
    MApplicationPage secondPage;
    MApplicationPage thirdPage;
    QList<MSceneWindow *> pageHistory;

    sm->appearSceneWindowNow(&firstPage);
    sm->appearSceneWindowNow(&secondPage);
    sm->appearSceneWindowNow(&thirdPage);

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 2);
    QCOMPARE(pageHistory.at(0), &firstPage);
    QCOMPARE(pageHistory.at(1), &secondPage);

    sm->dismissSceneWindowNow(&thirdPage);

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 1);
    QCOMPARE(pageHistory.at(0), &firstPage);

    sm->dismissSceneWindowNow(&secondPage);

    QCOMPARE(sm->pageHistory().count(), 0);

    sm->dismissSceneWindowNow(&firstPage);

    QCOMPARE(sm->pageHistory().count(), 0);
}

void Ut_MSceneManager::testSettingPageHistory()
{
    MApplicationPage firstPage;
    MApplicationPage secondPage;
    MApplicationPage thirdPage;
    QList<MSceneWindow *> pageHistory;

    sm->appearSceneWindowNow(&thirdPage);

    pageHistory.append(&firstPage);
    pageHistory.append(&secondPage);

    sm->setPageHistory(pageHistory);

    QCOMPARE(sm->pageHistory(), pageHistory);

    // Now check that the history set above will actually be followed
    // by the scene manager

    QSignalSpy firstPageAppeared(&firstPage, SIGNAL(appeared()));
    QSignalSpy firstPageDisappeared(&firstPage, SIGNAL(disappeared()));
    QSignalSpy secondPageAppeared(&secondPage, SIGNAL(appeared()));
    QSignalSpy secondPageDisappeared(&secondPage, SIGNAL(disappeared()));
    QSignalSpy thirdPageAppeared(&thirdPage, SIGNAL(appeared()));
    QSignalSpy thirdPageDisappeared(&thirdPage, SIGNAL(disappeared()));

    sm->dismissSceneWindowNow(&thirdPage);

    QCOMPARE(secondPageAppeared.count(), 1);
    QCOMPARE(secondPageDisappeared.count(), 0);
    QCOMPARE(thirdPageDisappeared.count(), 1);
    secondPageAppeared.clear();
    secondPageDisappeared.clear();

    sm->dismissSceneWindowNow(&secondPage);

    QCOMPARE(firstPageAppeared.count(), 1);
    QCOMPARE(firstPageDisappeared.count(), 0);
    QCOMPARE(secondPageAppeared.count(), 0);
    QCOMPARE(secondPageDisappeared.count(), 1);

    sm->dismissSceneWindowNow(&firstPage);

    QCOMPARE(firstPageDisappeared.count(), 1);
}

void Ut_MSceneManager::testDeletePageInPageHistory()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MApplicationPage *thirdPage = new MApplicationPage;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindowNow(secondPage);
    sm->appearSceneWindowNow(thirdPage);

    delete secondPage;
    secondPage = 0;

    QSignalSpy firstPageAppeared(firstPage, SIGNAL(appeared()));
    QSignalSpy firstPageDisappeared(firstPage, SIGNAL(disappeared()));
    QSignalSpy thirdPageDisappeared(thirdPage, SIGNAL(disappeared()));

    sm->dismissSceneWindowNow(thirdPage);

    // Should go back straight to the first page since the second one was deleted.
    QCOMPARE(firstPageAppeared.count(), 1);
    QCOMPARE(firstPageDisappeared.count(), 0);
    QCOMPARE(thirdPageDisappeared.count(), 1);

    delete thirdPage;
}

void Ut_MSceneManager::testPageHistoryAfterPageDeletion()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MApplicationPage *thirdPage = new MApplicationPage;
    QList<MSceneWindow *> pageHistory;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindowNow(secondPage);
    sm->appearSceneWindowNow(thirdPage);

    delete secondPage;
    secondPage = 0;

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 1);
    QCOMPARE(pageHistory.at(0), firstPage);
}

void Ut_MSceneManager::testPageHistoryChangedWhenPushing()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MApplicationPage *thirdPage = new MApplicationPage;
    QSignalSpy pageHistoryChanged(sm, SIGNAL(pageHistoryChanged()));

    sm->appearSceneWindowNow(firstPage);

    QCOMPARE(pageHistoryChanged.count(), 0);

    sm->appearSceneWindowNow(secondPage);

    QCOMPARE(pageHistoryChanged.count(), 1);

    sm->appearSceneWindowNow(thirdPage);

    QCOMPARE(pageHistoryChanged.count(), 2);
}

void Ut_MSceneManager::testPageHistoryChangedWhenPopping()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MApplicationPage *thirdPage = new MApplicationPage;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindowNow(secondPage);
    sm->appearSceneWindowNow(thirdPage);

    QSignalSpy pageHistoryChanged(sm, SIGNAL(pageHistoryChanged()));

    sm->dismissSceneWindowNow(thirdPage);

    QCOMPARE(pageHistoryChanged.count(), 1);

    sm->dismissSceneWindowNow(secondPage);

    QCOMPARE(pageHistoryChanged.count(), 2);

    sm->dismissSceneWindowNow(firstPage);

    // page history should not have been changed since it was already empty
    QCOMPARE(pageHistoryChanged.count(), 2);
}

void Ut_MSceneManager::testPageHistoryChangedWhenSettingPageHistory()
{
    MApplicationPage firstPage;
    MApplicationPage secondPage;
    MApplicationPage thirdPage;
    QList<MSceneWindow *> pageHistory;

    sm->appearSceneWindowNow(&thirdPage);

    pageHistory.append(&firstPage);
    pageHistory.append(&secondPage);

    QSignalSpy pageHistoryChanged(sm, SIGNAL(pageHistoryChanged()));
    sm->setPageHistory(pageHistory);

    QCOMPARE(pageHistoryChanged.count(), 1);
}

void Ut_MSceneManager::testPageHistoryChangedWhenSettingSamePageHistory()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MApplicationPage *thirdPage = new MApplicationPage;
    QList<MSceneWindow *> pageHistory;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindowNow(secondPage);
    sm->appearSceneWindowNow(thirdPage);

    pageHistory = sm->pageHistory();

    QSignalSpy pageHistoryChanged(sm, SIGNAL(pageHistoryChanged()));
    sm->setPageHistory(pageHistory);
    QCOMPARE(pageHistoryChanged.count(), 0);
}

void Ut_MSceneManager::testTransitionModeWhenOffDisplay()
{
    QObject testBridge;
    MSceneWindow *sceneWindow = new MSceneWindow;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = false;

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindow(sceneWindow);

    // Appearance won't be animated since the MWindow is off display
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testTransitionModeWhenOnDisplay()
{
    QObject testBridge;
    MSceneWindow *sceneWindow = new MSceneWindow;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindow(sceneWindow);

    // Appearance won't be animated since the MWindow is on display
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);
}

void Ut_MSceneManager::testTransitionModeWhenNoWindow()
{
    QObject testBridge;
    MSceneWindow *sceneWindow = new MSceneWindow;
    MSceneManager sceneManager;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(&sceneManager);

    gMWindowIsOnDisplay = true;

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    sceneManager.appearSceneWindow(sceneWindow);

    // Appearance won't be animated since there's no MWindow rendering the scene
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);

    // clean up
    testBridge.setParent(0); // don't get deleted by sceneManager
}

void Ut_MSceneManager::testPageSwitchQueue()
{
    TestBridge testBridge;
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MApplicationPage *thirdPage = new MApplicationPage;
    QSignalSpy firstPageAppeared(firstPage, SIGNAL(appeared()));
    QSignalSpy secondPageAppeared(secondPage, SIGNAL(appeared()));

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindow(firstPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Appearing);

    sm->appearSceneWindow(secondPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindow(thirdPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappeared);

    testBridge.fastForwardPageSwitchAnimation();

    QCOMPARE(firstPageAppeared.count(), 1);
    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappeared);

    testBridge.fastForwardPageSwitchAnimation();

    QCOMPARE(secondPageAppeared.count(), 1);
    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Appearing);

    testBridge.fastForwardPageSwitchAnimation();

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_disappearWhileAppearing()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    QSignalSpy spyAppeared(sceneWindow, SIGNAL(appeared()));
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);

    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);

    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(spyAppeared.count(), 1);
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_disappearAndAppearWhileAppearing()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);

    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);

    // That cancels out the previous disappear() that was queued, resulting
    // in an empty transition queue.
    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);

    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_appearWhileDisappearing()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    QSignalSpy spyDisappeared(sceneWindow, SIGNAL(disappeared()));
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(sceneWindow);
    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(spyDisappeared.count(), 1);
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);

    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_appearAndDisappearWhileDisappearing()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(sceneWindow);
    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    // That cancels out the previous appear() that was queued, resulting
    // in an empty transition queue.
    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testDismissPageThatIsReappearing()
{
    TestBridge testBridge;
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MApplicationPage *thirdPage = new MApplicationPage;
    QSignalSpy firstPageAppeared(firstPage, SIGNAL(appeared()));
    QSignalSpy secondPageAppeared(secondPage, SIGNAL(appeared()));

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindowNow(secondPage);
    sm->appearSceneWindowNow(thirdPage);

    sm->dismissSceneWindow(thirdPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappearing);

    sm->dismissSceneWindow(secondPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappearing);

    testBridge.fastForwardPageSwitchAnimation();

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappeared);

    testBridge.fastForwardPageSwitchAnimation();

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testDeletePageThatIsAppearing()
{
    TestBridge testBridge;
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindow(secondPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);

    delete secondPage;
    secondPage = 0;

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappearing);

    // Animation should not cause a crash
    testBridge.fastForwardPageSwitchAnimation();

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testDeletePageThatIsDisappearing()
{
    TestBridge testBridge;
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(sm);

    gMWindowIsOnDisplay = true;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindow(secondPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);

    delete firstPage;
    firstPage = 0;

    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);

    // Animation should not cause a crash
    testBridge.fastForwardPageSwitchAnimation();

    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appeared);
}

TestBridge::TestBridge(QObject *parent)
    : QObject(parent)
{
}

void TestBridge::fastForwardPageSwitchAnimation()
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "fastForwardPageSwitchAnimation",
            Qt::DirectConnection);
}

void TestBridge::fastForwardSceneWindowTransitionAnimation(MSceneWindow *sceneWindow)
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "fastForwardSceneWindowTransitionAnimation",
            Qt::DirectConnection,
            Q_ARG(MSceneWindow*, sceneWindow));
}

void TestBridge::addSceneWindow(MSceneWindow *sceneWindow)
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "addSceneWindow",
            Qt::DirectConnection,
            Q_ARG(MSceneWindow*, sceneWindow));
}

void TestBridge::removeSceneWindow(MSceneWindow *sceneWindow)
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "removeSceneWindow",
            Qt::DirectConnection,
            Q_ARG(MSceneWindow*, sceneWindow));
}

QTEST_MAIN(Ut_MSceneManager);
