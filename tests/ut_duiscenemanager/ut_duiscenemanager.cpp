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

// Avoid creating unnecessary OpenGL stuff
void DuiWindow::setTranslucentBackground(bool enable)
{
    Q_UNUSED(enable);
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
    sm->showWindowNow(p);

    QCOMPARE(sm->d_ptr->windows->size(), 1);
    delete p;
}

void Ut_DuiSceneManager::testSceneWindowDetaching()
{
    DuiOverlay *p = new DuiOverlay();
    sm->showWindowNow(p);

    QCOMPARE(sm->d_ptr->windows->size(), 1);
    delete p;
    QCOMPARE(sm->d_ptr->windows->size(), 0);
}

void Ut_DuiSceneManager::testSceneWindowShowNow()
{
    DuiOverlay *p = new DuiOverlay();
    sm->showWindowNow(p);

    QCOMPARE((int)p->zValue(), (int)DuiSceneManagerPrivate::Overlay);
    QCOMPARE(p->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(p->isVisible(), true);

    delete p;
}

void Ut_DuiSceneManager::testSceneWindowShow()
{
    DuiNavigationBar *p = new DuiNavigationBar();
    sm->showWindow(p);

    QCOMPARE((int)p->zValue(), (int)DuiSceneManagerPrivate::NavigationBar);
    QCOMPARE(p->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(p->isVisible(), true);

    delete p;
}

void Ut_DuiSceneManager::testSceneWindowHideNow()
{
    DuiNavigationBar *p = new DuiNavigationBar();

    sm->showWindowNow(p);
    sm->hideWindowNow(p);

    QCOMPARE(sm->d_ptr->navBar, (DuiNavigationBar *)0);
    QCOMPARE(p->isVisible(), false);

    delete p;
}

void Ut_DuiSceneManager::testSceneWindowHide()
{
    DuiNavigationBar *p = new DuiNavigationBar();
    sm->showWindow(p);
    sm->hideWindow(p);

    QCOMPARE(sm->d_ptr->navBar, (DuiNavigationBar *)0);
    QCOMPARE(p->isVisible(), false);

    delete p;
}

void Ut_DuiSceneManager::testSceneLayerEffect()
{
    DuiDialog *m = new DuiMessageBox("test");
    DuiNavigationBar *n = new DuiNavigationBar();

    m->appearNow();
    n->appearNow();

    QVERIFY(m->parentItem() != 0);
    QVERIFY(n->parentItem() == sm->d_ptr->rootElement);

    QCOMPARE((int)m->parentItem()->zValue(), (int)DuiSceneManagerPrivate::MessageBox);

    sm->hideWindowNow(m);
    sm->hideWindowNow(n);

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
                            Dui::ImmediateOrientationChange);
    Dui::Orientation newOrientation = (newAngle == Dui::Angle0 || newAngle == Dui::Angle180)
                                      ? Dui::Landscape
                                      : Dui::Portrait;

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<Dui::Orientation>(), newOrientation);
}

// Test uses non-exported symbol "DuiDockWidget".
/*
void Ut_DuiSceneManager::testNavBarDockWidgetVisibility()
{
    DuiNavigationBar *p = new DuiNavigationBar();
    DuiDockWidget *d = new DuiDockWidget();

    p->appearNow();
    d->appearNow();

    int newAngle = sm->orientationAngle() + Dui::Angle90;
    newAngle %= 360;

    sm->setOrientationAngle( (Dui::OrientationAngle) newAngle );

    QCOMPARE(p->isVisible(), true);
    QCOMPARE(d->isVisible(), true);

    sm->hideWindowNow(p);
    sm->hideWindowNow(d);

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
                            Dui::ImmediateOrientationChange);

    newAngle = sm->orientationAngle() + Dui::Angle90;
    newAngle %= 360;
    sm->setOrientationAngle((Dui::OrientationAngle) newAngle,
                            Dui::ImmediateOrientationChange);

    QCOMPARE(spyChanged.count(), 2);

}

void Ut_DuiSceneManager::testSceneSizes()
{
    QSize vSR = sm->visibleSceneSize();

    // check scene rects are non-zero
    QVERIFY(vSR.width() > 0);
    QVERIFY(vSR.height() > 0);

    sm->setOrientationAngle(Dui::Angle90);

    // check scenerects are in correct orientation
    // what about square?
    QCOMPARE(sm->orientation(), Dui::Portrait);
    QCOMPARE(sm->visibleSceneSize(), sm->visibleSceneSize(Dui::Portrait));
    vSR = sm->visibleSceneSize();
    QVERIFY(vSR.width() < vSR.height());

    // test other orientation
    vSR = sm->visibleSceneSize(Dui::Landscape);
    QVERIFY(vSR.width() > vSR.height());

    sm->setOrientationAngle(Dui::Angle180);

    QCOMPARE(sm->orientation(), Dui::Landscape);
    QCOMPARE(sm->visibleSceneSize(), sm->visibleSceneSize(Dui::Landscape));
    vSR = sm->visibleSceneSize();
    QVERIFY(vSR.width() > vSR.height());

    // test other orientation
    vSR = sm->visibleSceneSize(Dui::Portrait);
    QVERIFY(vSR.width() < vSR.height());
}

void Ut_DuiSceneManager::testWindowAnimationDone()
{
    DuiSceneWindow *window1 = new DuiOverlay;
    DuiSceneWindow *window2 = new DuiOverlay;
    QSignalSpy spyWindow1Shown(window1, SIGNAL(windowShown()));
    QSignalSpy spyWindow1Hidden(window1, SIGNAL(windowHidden()));
    QSignalSpy spyWindow2Shown(window2, SIGNAL(windowShown()));
    window1->appearNow();
    QCOMPARE(spyWindow1Shown.count(), 1);
    QCOMPARE(spyWindow1Hidden.count(), 0);
    QCOMPARE(spyWindow2Shown.count(), 0);
    sm->hideWindowNow(window1);
    QCOMPARE(spyWindow1Shown.count(), 1);
    QCOMPARE(spyWindow1Hidden.count(), 1);
    QCOMPARE(spyWindow2Shown.count(), 0);
    delete window1;
    delete window2;
}

void Ut_DuiSceneManager::testPageSwitchingOnAppearNow()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;

    QSignalSpy firstPageShown(&firstPage, SIGNAL(windowShown()));
    QSignalSpy firstPageHidden(&firstPage, SIGNAL(windowHidden()));
    QSignalSpy secondPageShown(&secondPage, SIGNAL(windowShown()));

    sm->showWindowNow(&firstPage);

    QCOMPARE(firstPageShown.count(), 1);
    QCOMPARE(firstPageHidden.count(), 0);
    QCOMPARE(secondPageShown.count(), 0);
    firstPageShown.clear();

    sm->showWindowNow(&secondPage);

    QCOMPARE(firstPageShown.count(), 0);
    QCOMPARE(firstPageHidden.count(), 1);
    QCOMPARE(secondPageShown.count(), 1);
}

void Ut_DuiSceneManager::testPageSwitchingOnDismissNow()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;

    sm->showWindowNow(&firstPage);
    sm->showWindowNow(&secondPage);

    QSignalSpy firstPageShown(&firstPage, SIGNAL(windowShown()));
    QSignalSpy firstPageHidden(&firstPage, SIGNAL(windowHidden()));
    QSignalSpy secondPageShown(&secondPage, SIGNAL(windowShown()));
    QSignalSpy secondPageHidden(&secondPage, SIGNAL(windowHidden()));

    sm->closeWindowNow(&secondPage);

    QCOMPARE(firstPageShown.count(), 1);
    QCOMPARE(firstPageHidden.count(), 0);
    QCOMPARE(secondPageShown.count(), 0);
    QCOMPARE(secondPageHidden.count(), 1);
}

void Ut_DuiSceneManager::testPageHistoryPushing()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;
    DuiApplicationPage thirdPage;
    QList<DuiSceneWindow *> pageHistory;

    QCOMPARE(sm->pageHistory().count(), 0);

    sm->showWindowNow(&firstPage);

    QCOMPARE(sm->pageHistory().count(), 0);

    sm->showWindowNow(&secondPage);

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 1);
    QCOMPARE(pageHistory.at(0), &firstPage);

    sm->showWindowNow(&thirdPage);

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

    sm->showWindowNow(&firstPage);
    sm->showWindowNow(&secondPage);
    sm->showWindowNow(&thirdPage);

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 2);
    QCOMPARE(pageHistory.at(0), &firstPage);
    QCOMPARE(pageHistory.at(1), &secondPage);

    sm->closeWindowNow(&thirdPage);

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 1);
    QCOMPARE(pageHistory.at(0), &firstPage);

    sm->closeWindowNow(&secondPage);

    QCOMPARE(sm->pageHistory().count(), 0);

    sm->closeWindowNow(&firstPage);

    QCOMPARE(sm->pageHistory().count(), 0);
}

void Ut_DuiSceneManager::testSettingPageHistory()
{
    DuiApplicationPage firstPage;
    DuiApplicationPage secondPage;
    DuiApplicationPage thirdPage;
    QList<DuiSceneWindow *> pageHistory;

    sm->showWindowNow(&thirdPage);

    pageHistory.append(&firstPage);
    pageHistory.append(&secondPage);

    sm->setPageHistory(pageHistory);

    QCOMPARE(sm->pageHistory(), pageHistory);

    // Now check that the history set above will actually be followed
    // by the scene manager

    QSignalSpy firstPageShown(&firstPage, SIGNAL(windowShown()));
    QSignalSpy firstPageHidden(&firstPage, SIGNAL(windowHidden()));
    QSignalSpy secondPageShown(&secondPage, SIGNAL(windowShown()));
    QSignalSpy secondPageHidden(&secondPage, SIGNAL(windowHidden()));
    QSignalSpy thirdPageShown(&thirdPage, SIGNAL(windowShown()));
    QSignalSpy thirdPageHidden(&thirdPage, SIGNAL(windowHidden()));

    sm->closeWindowNow(&thirdPage);

    QCOMPARE(secondPageShown.count(), 1);
    QCOMPARE(secondPageHidden.count(), 0);
    QCOMPARE(thirdPageHidden.count(), 1);
    secondPageShown.clear();
    secondPageHidden.clear();

    sm->closeWindowNow(&secondPage);

    QCOMPARE(firstPageShown.count(), 1);
    QCOMPARE(firstPageHidden.count(), 0);
    QCOMPARE(secondPageShown.count(), 0);
    QCOMPARE(secondPageHidden.count(), 1);

    sm->closeWindowNow(&firstPage);

    QCOMPARE(firstPageHidden.count(), 1);
}

void Ut_DuiSceneManager::testDeletePageInPageHistory()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiApplicationPage *thirdPage = new DuiApplicationPage;

    sm->showWindowNow(firstPage);
    sm->showWindowNow(secondPage);
    sm->showWindowNow(thirdPage);

    delete secondPage;
    secondPage = 0;

    QSignalSpy firstPageShown(firstPage, SIGNAL(windowShown()));
    QSignalSpy firstPageHidden(firstPage, SIGNAL(windowHidden()));
    QSignalSpy thirdPageHidden(thirdPage, SIGNAL(windowHidden()));

    sm->closeWindowNow(thirdPage);

    // Should go back straight to the first page since the second one was deleted.
    QCOMPARE(firstPageShown.count(), 1);
    QCOMPARE(firstPageHidden.count(), 0);
    QCOMPARE(thirdPageHidden.count(), 1);

    delete thirdPage;
}

void Ut_DuiSceneManager::testPageHistoryAfterPageDeletion()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiApplicationPage *thirdPage = new DuiApplicationPage;
    QList<DuiSceneWindow *> pageHistory;

    sm->showWindowNow(firstPage);
    sm->showWindowNow(secondPage);
    sm->showWindowNow(thirdPage);

    delete secondPage;
    secondPage = 0;

    pageHistory = sm->pageHistory();
    QCOMPARE(pageHistory.count(), 1);
    QCOMPARE(pageHistory.at(0), firstPage);
}

QTEST_MAIN(Ut_DuiSceneManager);
