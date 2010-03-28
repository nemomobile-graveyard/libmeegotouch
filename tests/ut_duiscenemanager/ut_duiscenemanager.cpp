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

#include <QMetaType>
#include <QEventLoop>
#include <QTimer>

#include <DuiSceneManager>
#include <DuiSceneWindow>

#include "ut_duiscenemanager.h"

#include <DuiApplicationPage>
#include <DuiWindow>
#include <DuiNavigationBar>
#include <DuiMessageBox>
#include <DuiOverlay>
#include <DuiNavigationBar>
#include "duidockwidget.h"
#include <DuiScene>
#include "duiscenemanager_p.h"

#include <DuiComponentData>
#include <duiapplicationwindow.h>

// DuiWidgetController stubs
const DuiWidgetView *DuiWidgetController::view() const
{
    return 0;
}

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
DuiWindow *gActiveWindow = 0;
bool DuiComponentData::softwareRendering()
{
    return true;
}

bool DuiComponentData::fullScreen()
{
    return false;
}

void DuiComponentData::setActiveWindow(DuiWindow *window)
{
    gActiveWindow = window;
}

QList<DuiWindow *> DuiComponentData::windows()
{
    QList<DuiWindow *> windowList;
    if (gActiveWindow)
       windowList.append(gActiveWindow);
    return windowList;
}

DuiWindow *DuiComponentData::activeWindow()
{
    return gActiveWindow;
}

DuiApplicationWindow *DuiComponentData::activeApplicationWindow()
{
    return qobject_cast<DuiApplicationWindow *>(gActiveWindow);
}

void DuiComponentData::registerWindow(DuiWindow *window)
{
    if (gActiveWindow == 0)
        setActiveWindow(window);
}

void DuiComponentData::unregisterWindow(DuiWindow *window)
{
    if (gActiveWindow == window)
        setActiveWindow(0);
}

bool DuiComponentData::emulateTwoFingerGestures()
{
    return false;
}

// DuiWindow stubs

// Avoid creating unnecessary OpenGL stuff
void DuiWindow::setTranslucentBackground(bool enable)
{
    Q_UNUSED(enable);
}

bool gDuiWindowIsOnDisplay = false;
bool DuiWindow::isOnDisplay() const
{
    return gDuiWindowIsOnDisplay;
}

// Test class implementation

void Ut_DuiSceneManager::initTestCase()
{
    qRegisterMetaType<Dui::Orientation>("Dui::Orientation");
    qRegisterMetaType<Dui::Orientation>("Dui::OrientationAngle");
    duiWindow = new DuiWindow;
}

void Ut_DuiSceneManager::cleanupTestCase()
{
    delete duiWindow;
}

void Ut_DuiSceneManager::init()
{
    sm = new DuiSceneManager;
    duiWindow->setSceneManager(sm);
}

void Ut_DuiSceneManager::cleanup()
{
    duiWindow->setSceneManager(0);
}

void Ut_DuiSceneManager::testConstructorWithNoScene()
{
    QVERIFY(sm->scene());
}

void Ut_DuiSceneManager::testConstructorWithSceneSpecified()
{
    DuiScene *scene = new DuiScene;
    DuiSceneManager *manager = new DuiSceneManager(scene);
    QCOMPARE(manager->scene(), scene);
    delete manager;
}

void Ut_DuiSceneManager::testSceneWindowAttaching()
{
    DuiOverlay *p = new DuiOverlay();
    sm->appearSceneWindowNow(p);

    QCOMPARE(sm->d_ptr->windows->size(), 1);
    delete p;
}

void Ut_DuiSceneManager::testSceneWindowDetaching()
{
    DuiOverlay *p = new DuiOverlay();
    sm->appearSceneWindowNow(p);

    QCOMPARE(sm->d_ptr->windows->size(), 1);
    delete p;
    QCOMPARE(sm->d_ptr->windows->size(), 0);
}

void Ut_DuiSceneManager::testSceneWindowAppearNow()
{
    DuiOverlay *p = new DuiOverlay();
    sm->appearSceneWindowNow(p);

    QCOMPARE((int)p->zValue(), (int)DuiSceneManagerPrivate::Overlay);
    QCOMPARE(p->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(p->isVisible(), true);

    delete p;
}

void Ut_DuiSceneManager::testSceneWindowAppear()
{
    DuiNavigationBar *p = new DuiNavigationBar();
    sm->appearSceneWindow(p);

    QCOMPARE((int)p->zValue(), (int)DuiSceneManagerPrivate::NavigationBar);
    QCOMPARE(p->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(p->isVisible(), true);

    delete p;
}

void Ut_DuiSceneManager::testSceneWindowDisappearNow()
{
    DuiNavigationBar *p = new DuiNavigationBar();

    sm->appearSceneWindowNow(p);
    sm->disappearSceneWindowNow(p);

    QCOMPARE(sm->d_ptr->navBar, (DuiNavigationBar *)0);
    QCOMPARE(p->isVisible(), false);

    delete p;
}

void Ut_DuiSceneManager::testSceneWindowDisappear()
{
    DuiNavigationBar *p = new DuiNavigationBar();
    sm->appearSceneWindow(p);
    sm->disappearSceneWindow(p);

    QCOMPARE(sm->d_ptr->navBar, (DuiNavigationBar *)0);
    QCOMPARE(p->isVisible(), false);

    delete p;
}

void Ut_DuiSceneManager::testSceneLayerEffect()
{
    DuiDialog *m = new DuiMessageBox("test");
    DuiNavigationBar *n = new DuiNavigationBar();

    sm->appearSceneWindowNow(m);
    sm->appearSceneWindowNow(n);

    QVERIFY(m->parentItem() != 0);
    QVERIFY(n->parentItem() == sm->d_ptr->rootElement);

    QCOMPARE((int)m->parentItem()->zValue(), (int)DuiSceneManagerPrivate::MessageBox);

    sm->disappearSceneWindowNow(m);
    sm->disappearSceneWindowNow(n);

    delete m;
    delete n;
}

void Ut_DuiSceneManager::testInitialOrientation()
{
    Dui::Orientation initialOrientation = sm->orientation();
    QCOMPARE(initialOrientation, Dui::Landscape);
}

void Ut_DuiSceneManager::testOrientationChangedSignal()
{
    QSignalSpy spy(sm, SIGNAL(orientationChanged(Dui::Orientation)));

    int newAngle = sm->orientationAngle() + Dui::Angle90;
    newAngle %= 360;

    sm->setOrientationAngle((Dui::OrientationAngle) newAngle,
                            DuiSceneManager::ImmediateTransition);
    Dui::Orientation newOrientation = (newAngle == Dui::Angle0 || newAngle == Dui::Angle180)
                                      ? Dui::Landscape
                                      : Dui::Portrait;

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Dui::Orientation>(), newOrientation);
}

void Ut_DuiSceneManager::testNoOrientationChangedSignalWhenRotatingBy180Degrees()
{
    QSignalSpy spy(sm, SIGNAL(orientationChanged(Dui::Orientation)));

    int newAngle = sm->orientationAngle() + Dui::Angle180;
    newAngle %= 360;

    sm->setOrientationAngle((Dui::OrientationAngle) newAngle,
                            DuiSceneManager::ImmediateTransition);

    QCOMPARE(spy.count(), 0);
}

// Test uses non-exported symbol "DuiDockWidget".
/*
void Ut_DuiSceneManager::testNavBarDockWidgetVisibility()
{
    DuiNavigationBar *p = new DuiNavigationBar();
    DuiDockWidget *d = new DuiDockWidget();

    sm->appearSceneWindowNow(p);
    sm->appearSceneWindowNow(d);

    int newAngle = sm->orientationAngle() + Dui::Angle90;
    newAngle %= 360;

    sm->setOrientationAngle( (Dui::OrientationAngle) newAngle );

    QCOMPARE(p->isVisible(), true);
    QCOMPARE(d->isVisible(), true);

    sm->disappearSceneWindowNow(p);
    sm->disappearSceneWindowNow(d);

    delete p;
    delete d;
}
*/

void Ut_DuiSceneManager::testAngleBoundaryCases()
{
    QSignalSpy spyChanged(sm, SIGNAL(orientationChanged(Dui::Orientation)));

    int newAngle = sm->orientationAngle() + Dui::Angle270;
    newAngle %= 360;
    sm->setOrientationAngle((Dui::OrientationAngle) newAngle,
                            DuiSceneManager::ImmediateTransition);

    newAngle = sm->orientationAngle() + Dui::Angle90;
    newAngle %= 360;
    sm->setOrientationAngle((Dui::OrientationAngle) newAngle,
                            DuiSceneManager::ImmediateTransition);

    QCOMPARE(spyChanged.count(), 2);

}

void Ut_DuiSceneManager::testSceneSizes()
{
    QSize vSR = sm->visibleSceneSize();

    // check scene rects are non-zero
    QVERIFY(vSR.width() > 0);
    QVERIFY(vSR.height() > 0);

    sm->setOrientationAngle(Dui::Angle90, DuiSceneManager::ImmediateTransition);

    // check scenerects are in correct orientation
    // what about square?
    QCOMPARE(sm->orientation(), Dui::Portrait);
    QCOMPARE(sm->visibleSceneSize(), sm->visibleSceneSize(Dui::Portrait));
    vSR = sm->visibleSceneSize();
    QVERIFY(vSR.width() < vSR.height());

    // test other orientation
    vSR = sm->visibleSceneSize(Dui::Landscape);
    QVERIFY(vSR.width() > vSR.height());

    sm->setOrientationAngle(Dui::Angle180, DuiSceneManager::ImmediateTransition);

    QCOMPARE(sm->orientation(), Dui::Landscape);
    QCOMPARE(sm->visibleSceneSize(), sm->visibleSceneSize(Dui::Landscape));
    vSR = sm->visibleSceneSize();
    QVERIFY(vSR.width() > vSR.height());

    // test other orientation
    vSR = sm->visibleSceneSize(Dui::Portrait);
    QVERIFY(vSR.width() < vSR.height());
}

void Ut_DuiSceneManager::testPageSwitchingOnAppearNow()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;

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

void Ut_DuiSceneManager::testPageSwitchingOnDismissNow()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;

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

void Ut_DuiSceneManager::testPageHistoryPushing()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;
    DuiApplicationPage thirdPage;
    QList<DuiSceneWindow *> pageHistory;

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

void Ut_DuiSceneManager::testPageHistoryPopping()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;
    DuiApplicationPage thirdPage;
    QList<DuiSceneWindow *> pageHistory;

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

void Ut_DuiSceneManager::testSettingPageHistory()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;
    DuiApplicationPage thirdPage;
    QList<DuiSceneWindow *> pageHistory;

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

void Ut_DuiSceneManager::testDeletePageInPageHistory()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiApplicationPage *thirdPage = new DuiApplicationPage;

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

void Ut_DuiSceneManager::testPageHistoryAfterPageDeletion()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiApplicationPage *thirdPage = new DuiApplicationPage;
    QList<DuiSceneWindow *> pageHistory;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindowNow(secondPage);
    sm->appearSceneWindowNow(thirdPage);

    delete secondPage;
    secondPage = 0;

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 1);
    QCOMPARE(pageHistory.at(0), firstPage);
}

void Ut_DuiSceneManager::testPageHistoryChangedWhenPushing()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiApplicationPage *thirdPage = new DuiApplicationPage;
    QSignalSpy pageHistoryChanged(sm, SIGNAL(pageHistoryChanged()));

    sm->appearSceneWindowNow(firstPage);

    QCOMPARE(pageHistoryChanged.count(), 0);

    sm->appearSceneWindowNow(secondPage);

    QCOMPARE(pageHistoryChanged.count(), 1);

    sm->appearSceneWindowNow(thirdPage);

    QCOMPARE(pageHistoryChanged.count(), 2);
}

void Ut_DuiSceneManager::testPageHistoryChangedWhenPopping()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiApplicationPage *thirdPage = new DuiApplicationPage;

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

void Ut_DuiSceneManager::testPageHistoryChangedWhenSettingPageHistory()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;
    DuiApplicationPage thirdPage;
    QList<DuiSceneWindow *> pageHistory;

    sm->appearSceneWindowNow(&thirdPage);

    pageHistory.append(&firstPage);
    pageHistory.append(&secondPage);

    QSignalSpy pageHistoryChanged(sm, SIGNAL(pageHistoryChanged()));
    sm->setPageHistory(pageHistory);

    QCOMPARE(pageHistoryChanged.count(), 1);
}

void Ut_DuiSceneManager::testPageHistoryChangedWhenSettingSamePageHistory()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiApplicationPage *thirdPage = new DuiApplicationPage;
    QList<DuiSceneWindow *> pageHistory;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindowNow(secondPage);
    sm->appearSceneWindowNow(thirdPage);

    pageHistory = sm->pageHistory();

    QSignalSpy pageHistoryChanged(sm, SIGNAL(pageHistoryChanged()));
    sm->setPageHistory(pageHistory);
    QCOMPARE(pageHistoryChanged.count(), 0);
}

void Ut_DuiSceneManager::testTransitionModeWhenOffDisplay()
{
    QObject debugInterface;
    DuiSceneWindow sceneWindow;

    debugInterface.setObjectName("debugInterface");
    debugInterface.setParent(sm);

    gDuiWindowIsOnDisplay = false;

    sm->appearSceneWindow(&sceneWindow);

    QCOMPARE((DuiSceneManager::TransitionMode)(debugInterface.property("transitionMode").toInt()),
             DuiSceneManager::ImmediateTransition);

    // clean up
    sm->disappearSceneWindowNow(&sceneWindow);
    if (sceneWindow.scene())
        sceneWindow.scene()->removeItem(&sceneWindow);
}

void Ut_DuiSceneManager::testTransitionModeWhenOnDisplay()
{
    QObject debugInterface;
    DuiSceneWindow sceneWindow;

    debugInterface.setObjectName("debugInterface");
    debugInterface.setParent(sm);

    gDuiWindowIsOnDisplay = true;

    sm->appearSceneWindow(&sceneWindow);

    QCOMPARE((DuiSceneManager::TransitionMode)(debugInterface.property("transitionMode").toInt()),
             DuiSceneManager::AnimatedTransition);

    // clean up
    sm->disappearSceneWindowNow(&sceneWindow);
    if (sceneWindow.scene())
        sceneWindow.scene()->removeItem(&sceneWindow);
}

void Ut_DuiSceneManager::testTransitionModeWhenNoWindow()
{
    QObject debugInterface;
    DuiSceneWindow sceneWindow;
    DuiSceneManager sceneManager;

    debugInterface.setObjectName("debugInterface");
    debugInterface.setParent(&sceneManager);

    gDuiWindowIsOnDisplay = true;

    sceneManager.appearSceneWindow(&sceneWindow);

    QCOMPARE((DuiSceneManager::TransitionMode)(debugInterface.property("transitionMode").toInt()),
             DuiSceneManager::ImmediateTransition);

    // clean up
    debugInterface.setParent(0); // don't get deleted by sceneManager
    sceneManager.disappearSceneWindowNow(&sceneWindow);
    if (sceneWindow.scene()) {
         // don't get deleted by QGraphicsScene
        sceneWindow.scene()->removeItem(&sceneWindow);
    }
}

QTEST_MAIN(Ut_DuiSceneManager);
