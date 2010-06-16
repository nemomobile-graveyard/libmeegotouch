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

void Ut_MSceneManager::testSceneWindowAttaching()
{
    MOverlay *p = new MOverlay();
    sm->appearSceneWindowNow(p);

    QCOMPARE(sm->d_ptr->windows.size(), (int)1);
    delete p;
}

void Ut_MSceneManager::testSceneWindowDetaching()
{
    MOverlay *p = new MOverlay();
    sm->appearSceneWindowNow(p);

    QCOMPARE(sm->d_ptr->windows.size(), (int)1);
    delete p;
    QCOMPARE(sm->d_ptr->windows.size(), (int)0);
}

void Ut_MSceneManager::testSceneWindowAppearNow()
{
    MOverlay *p = new MOverlay();
    sm->appearSceneWindowNow(p);

    QCOMPARE((int)p->zValue(), (int)MSceneManagerPrivate::Overlay);
    QCOMPARE(p->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(p->isVisible(), true);

    delete p;
}

void Ut_MSceneManager::testSceneWindowAppear()
{
    MNavigationBar *p = new MNavigationBar();
    sm->appearSceneWindow(p);

    QCOMPARE((int)p->zValue(), (int)MSceneManagerPrivate::NavigationBar);
    QCOMPARE(p->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(p->isVisible(), true);

    delete p;
}

void Ut_MSceneManager::testSceneWindowDisappearNow()
{
    MNavigationBar *p = new MNavigationBar();

    sm->appearSceneWindowNow(p);
    sm->disappearSceneWindowNow(p);

    QCOMPARE(sm->d_ptr->navBar, (MNavigationBar *)0);
    QCOMPARE(p->isVisible(), false);

    delete p;
}

void Ut_MSceneManager::testSceneWindowDisappear()
{
    MNavigationBar *p = new MNavigationBar();
    sm->appearSceneWindow(p);
    sm->disappearSceneWindow(p);

    QCOMPARE(sm->d_ptr->navBar, (MNavigationBar *)0);
    QCOMPARE(p->isVisible(), false);

    delete p;
}

void Ut_MSceneManager::testSceneLayerEffect()
{
    MDialog *m = new MMessageBox("test");
    MNavigationBar *n = new MNavigationBar();

    sm->appearSceneWindowNow(m);
    sm->appearSceneWindowNow(n);

    QVERIFY(m->parentItem() != 0);
    QVERIFY(n->parentItem() == sm->d_ptr->rootElement);

    QCOMPARE((int)m->parentItem()->zValue(), (int)MSceneManagerPrivate::MessageBox);

    sm->disappearSceneWindowNow(m);
    sm->disappearSceneWindowNow(n);

    delete m;
    delete n;
}

void Ut_MSceneManager::testInitialOrientation()
{
    M::Orientation initialOrientation = sm->orientation();
    QCOMPARE(initialOrientation, M::Landscape);
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
    QObject debugInterface;
    MSceneWindow sceneWindow;

    debugInterface.setObjectName("debugInterface");
    debugInterface.setParent(sm);

    gMWindowIsOnDisplay = false;

    sm->appearSceneWindow(&sceneWindow);

    QCOMPARE((MSceneManager::TransitionMode)(debugInterface.property("transitionMode").toInt()),
             MSceneManager::ImmediateTransition);

    // clean up
    sm->disappearSceneWindowNow(&sceneWindow);
    if (sceneWindow.scene())
        sceneWindow.scene()->removeItem(&sceneWindow);
}

void Ut_MSceneManager::testTransitionModeWhenOnDisplay()
{
    QObject debugInterface;
    MSceneWindow sceneWindow;

    debugInterface.setObjectName("debugInterface");
    debugInterface.setParent(sm);

    gMWindowIsOnDisplay = true;

    sm->appearSceneWindow(&sceneWindow);

    QCOMPARE((MSceneManager::TransitionMode)(debugInterface.property("transitionMode").toInt()),
             MSceneManager::AnimatedTransition);

    // clean up
    sm->disappearSceneWindowNow(&sceneWindow);
    if (sceneWindow.scene())
        sceneWindow.scene()->removeItem(&sceneWindow);
}

void Ut_MSceneManager::testTransitionModeWhenNoWindow()
{
    QObject debugInterface;
    MSceneWindow sceneWindow;
    MSceneManager sceneManager;

    debugInterface.setObjectName("debugInterface");
    debugInterface.setParent(&sceneManager);

    gMWindowIsOnDisplay = true;

    sceneManager.appearSceneWindow(&sceneWindow);

    QCOMPARE((MSceneManager::TransitionMode)(debugInterface.property("transitionMode").toInt()),
             MSceneManager::ImmediateTransition);

    // clean up
    debugInterface.setParent(0); // don't get deleted by sceneManager
    sceneManager.disappearSceneWindowNow(&sceneWindow);
    if (sceneWindow.scene()) {
         // don't get deleted by QGraphicsScene
        sceneWindow.scene()->removeItem(&sceneWindow);
    }
}

QTEST_MAIN(Ut_MSceneManager);
