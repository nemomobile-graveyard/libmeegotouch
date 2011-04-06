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
#include <mwindow_p.h>
#include <MNavigationBar>
#include <MMessageBox>
#include <MOverlay>
#include <MNavigationBar>
#include "mdockwidget.h"
#include <MScene>
#include "mscenemanager_p.h"
#include <MSheet>

#include <MComponentData>
#include <mapplicationwindow.h>
#include <MDeviceProfile>
#include "mstatusbar.h"

#include "mondisplaychangeevent.h"

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
#endif

bool MDeviceProfile::orientationAngleIsSupported(M::OrientationAngle angle, bool isKeyboardOpen) const
{
    Q_UNUSED(isKeyboardOpen);
    if (angle == M::Angle270)
        return true;
    return false;
}

bool gMWindowIsOnDisplay = false;
bool MWindow::isOnDisplay() const
{
    return gMWindowIsOnDisplay;
}

class EventTestPage : public MApplicationPage
{
public:
    QRectF latestViewRect;
    bool mustBeResolvedReceived;
protected:
    virtual void onDisplayChangeEvent(MOnDisplayChangeEvent * event) {
        if (event->state() == MOnDisplayChangeEvent::MustBeResolved) {
            mustBeResolvedReceived = true;
            latestViewRect = event->viewRect();
            MSceneWindow::onDisplayChangeEvent(event);
        }
    }
};

// Test class implementation

void Ut_MSceneManager::initTestCase()
{
    if(MComponentData::instance() == 0) {
        static int argc = 1;
        static char *argv[ 1 ];
        argv[ 0 ] = (char*)"./ut_mwindow";
        m_componentData = MComponentData::createInstance(argc, argv);
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

    sm->addSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);
    //QCOMPARE(sm->d_ptr->windows.size(), 1);
}

void Ut_MSceneManager::testRemoveSceneWindow()
{
    MSceneWindow *sceneWindow = new MSceneWindow;

    sm->addSceneWindow(sceneWindow);

    sm->removeSceneWindow(sceneWindow);

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

    gMWindowIsOnDisplay = true;
    mWindow->show();

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(sceneWindow->scene(), qobject_cast<const QGraphicsScene *>(sm->scene()));
    QCOMPARE(sceneWindow->isVisible(), true);

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(sceneWindow->isOnDisplay(), true);
}

void Ut_MSceneManager::testSceneWindowDisappearNow()
{
    MNavigationBar *p = new MNavigationBar();

    sm->appearSceneWindowNow(p);
    sm->disappearSceneWindowNow(p);

    QCOMPARE(p->isVisible(), false);

    delete p;
}

void Ut_MSceneManager::testSceneWindowDisappear()
{
    MSceneWindow *sceneWindow = new MSceneWindow;

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(sceneWindow->isOnDisplay(), true);
    QCOMPARE(sceneWindow->isVisible(), true);

    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(sceneWindow->isVisible(), true);

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

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

void Ut_MSceneManager::testOrientationChangeVisibility()
{
    gMWindowIsOnDisplay = true;

    // Create page to catch MOnDisplayChangeEvent's
    EventTestPage *win = new EventTestPage;
    sm->appearSceneWindowNow(win);

    // Init with landscape
    sm->setOrientationAngle(M::Angle0, MSceneManager::ImmediateTransition);

    const int height = win->geometry().height();
    const int width  = win->geometry().width();

    // Rotate to portrait, immediately
    win->mustBeResolvedReceived = false;
    sm->setOrientationAngle(M::Angle90, MSceneManager::ImmediateTransition);
    QVERIFY(win->mustBeResolvedReceived == true);
    QVERIFY(win->latestViewRect == QRectF(0, 0, height, width));

    // Rotate to landscape, immediately
    win->mustBeResolvedReceived = false;
    sm->setOrientationAngle(M::Angle0, MSceneManager::ImmediateTransition);
    QVERIFY(win->mustBeResolvedReceived == true);
    QVERIFY(win->latestViewRect == QRectF(0, 0, width, height));

    // Rotate back to portrait, immediately
    win->mustBeResolvedReceived = false;
    sm->setOrientationAngle(M::Angle90, MSceneManager::ImmediateTransition);
    QVERIFY(win->mustBeResolvedReceived == true);
    QVERIFY(win->latestViewRect == QRectF(0, 0, height, width));

    // Rotate to landscape, animatedly
    win->mustBeResolvedReceived = false;
    sm->setOrientationAngle(M::Angle0);
    QVERIFY(win->mustBeResolvedReceived == false);
    sm->fastForwardOrientationChangeAnimation();
    QVERIFY(win->mustBeResolvedReceived == true);
    QVERIFY(win->latestViewRect == QRectF(0, 0, width, height));

    // Rotate back to portrait, animatedly
    win->mustBeResolvedReceived = false;
    sm->setOrientationAngle(M::Angle90);
    QVERIFY(win->mustBeResolvedReceived == false);
    sm->fastForwardOrientationChangeAnimation();
    QVERIFY(win->mustBeResolvedReceived == true);
    QVERIFY(win->latestViewRect == QRectF(0, 0, height, width));
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

    sm->fastForwardPageSwitchAnimation();

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
    MSceneWindow *sceneWindow = new MSceneWindow;

    gMWindowIsOnDisplay = false;

    // Have to send a visibility event to MWindow so
    // that MWindowPrivate::onDisplaySet == true.
    MOnDisplayChangeEvent event(false, QRectF());
    QApplication::instance()->sendEvent(mWindow, &event);

    // Force a timeout on mwindows internal timer so that the display change event
    // gets processed immediately.
    QTimerEvent timerEvt(mWindow->d_func()->displayExitedTimer.timerId());
    QApplication::instance()->sendEvent(&(mWindow->d_func()->displayExitedTimer), &timerEvt);
    QApplication::processEvents();

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindow(sceneWindow);

    // Appearance won't be animated since the MWindow is off display
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testTransitionModeWhenOnDisplay()
{
    MSceneWindow *sceneWindow = new MSceneWindow;

    gMWindowIsOnDisplay = true;

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindow(sceneWindow);

    // Appearance won't be animated since the MWindow is on display
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);
}

void Ut_MSceneManager::testTransitionModeWhenNoWindow()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    MSceneManager sceneManager;

    gMWindowIsOnDisplay = true;

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);

    sceneManager.appearSceneWindow(sceneWindow);

    // Appearance won't be animated since there's no MWindow rendering the scene
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testPageSwitchQueue()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MApplicationPage *thirdPage = new MApplicationPage;
    QSignalSpy firstPageAppeared(firstPage, SIGNAL(appeared()));
    QSignalSpy secondPageAppeared(secondPage, SIGNAL(appeared()));

    gMWindowIsOnDisplay = true;

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappeared);

    sm->appearSceneWindow(firstPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(firstPageAppeared.count(), 1);

    sm->appearSceneWindow(secondPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);

    sm->appearSceneWindow(thirdPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappeared);

    sm->fastForwardPageSwitchAnimation();

    QCOMPARE(secondPageAppeared.count(), 1);
    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Appearing);

    sm->fastForwardPageSwitchAnimation();

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_disappearWhileAppearing()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    QSignalSpy spyAppeared(sceneWindow, SIGNAL(appeared()));

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);

    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(spyAppeared.count(), 1);
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_disappearAndAppearWhileAppearing()
{
    MSceneWindow *sceneWindow = new MSceneWindow;

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

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_disappearAndAppearWhileDisappearing_ApplicationPage()
{
    MSceneWindow *sceneWindow = new MApplicationPage;

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);

    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    // That cancels the previous appear() that was queued, resulting
    // in an empty transition queue.
    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_appearWhileDisappearing()
{
    MSceneWindow *sceneWindow = new MSceneWindow;
    QSignalSpy spyDisappeared(sceneWindow, SIGNAL(disappeared()));

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(sceneWindow);
    sm->disappearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    sm->appearSceneWindow(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappearing);

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(spyDisappeared.count(), 1);
    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appearing);

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_appearAndDisappearWhileDisappearing()
{
    MSceneWindow *sceneWindow = new MSceneWindow;

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

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testSceneWindowTransitionQueue_appearAndDisappearWhileDisappearing_ApplicationPage()
{
    MSceneWindow *sceneWindow = new MApplicationPage;

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

    sm->fastForwardSceneWindowTransitionAnimation(sceneWindow);

    QCOMPARE(sceneWindow->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testCallingAppearOnAppearingPageWhenTransitionQueueHasOtherPages()
{
    MSceneWindow *page1 = new MApplicationPage;
    MSceneWindow *page2 = new MApplicationPage;

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(page1);
    sm->appearSceneWindow(page2);

    QCOMPARE(page1->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(page2->sceneWindowState(), MSceneWindow::Appearing);

    sm->appearSceneWindow(page1);

    // Nothing changes. page1 appearance got queued.
    QCOMPARE(page1->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(page2->sceneWindowState(), MSceneWindow::Appearing);

    sm->appearSceneWindow(page2);

    // Nothing changes. page2 appearance got queued as well.
    QCOMPARE(page1->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(page2->sceneWindowState(), MSceneWindow::Appearing);

    sm->fastForwardPageSwitchAnimation();

    // queued page1 appearance got applied.
    QCOMPARE(page1->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(page2->sceneWindowState(), MSceneWindow::Disappearing);

    sm->fastForwardPageSwitchAnimation();

    // queued page2 appearance got applied.
    QCOMPARE(page1->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(page2->sceneWindowState(), MSceneWindow::Appearing);

    sm->fastForwardPageSwitchAnimation();

    // No more page transitions, queue is empty
    QCOMPARE(page1->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(page2->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MSceneManager::testDismissPageThatIsReappearing()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MApplicationPage *thirdPage = new MApplicationPage;
    QSignalSpy firstPageAppeared(firstPage, SIGNAL(appeared()));
    QSignalSpy secondPageAppeared(secondPage, SIGNAL(appeared()));

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

    sm->fastForwardPageSwitchAnimation();

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappeared);

    sm->fastForwardPageSwitchAnimation();

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(thirdPage->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testDeletePageThatIsAppearing()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;

    gMWindowIsOnDisplay = true;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindow(secondPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);

    delete secondPage;
    secondPage = 0;

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappearing);

    // Animation should not cause a crash
    sm->fastForwardPageSwitchAnimation();

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSceneManager::testDeletePageThatIsDisappearing()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;

    gMWindowIsOnDisplay = true;

    sm->appearSceneWindowNow(firstPage);
    sm->appearSceneWindow(secondPage);

    QCOMPARE(firstPage->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);

    delete firstPage;
    firstPage = 0;

    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appearing);

    // Animation should not cause a crash
    sm->fastForwardPageSwitchAnimation();

    QCOMPARE(secondPage->sceneWindowState(), MSceneWindow::Appeared);
}

#ifdef Q_WS_X11

static QVector<unsigned long> getX11Property(MWindow* window, const char *propertyName)
{
    QVector<unsigned long> result;
    Atom actualType = 0;
    int actualFormat = 0;
    unsigned long nitems = 0;
    unsigned long bytes = 0;
    unsigned long* data = 0;

    Atom propertyAtom = XInternAtom(QX11Info::display(), propertyName, False);

    int status = XGetWindowProperty(QX11Info::display(), window->effectiveWinId(), propertyAtom,
                                    0, 4, False, AnyPropertyType,
                                    &actualType, &actualFormat, &nitems,
                                    &bytes, reinterpret_cast<unsigned char**>(&data));

    if (status == Success && actualType == XA_CARDINAL && actualFormat == 32) {
        for (unsigned long i=0; i<nitems; i++)
            result.append(data[i]);
    }
    if (status == Success)
        XFree(data);
    return result;
}

void setX11Property(MWindow* window, const char *propertyName, QVector<unsigned long> value)
{
    Atom a = XInternAtom(QX11Info::display(), propertyName, False);
    XChangeProperty(QX11Info::display(), window->effectiveWinId(), a, XA_CARDINAL, 32, PropModeReplace,
                    (unsigned char*)value.data(), value.size());
}


static QVector<unsigned long> windowGeometry(MSceneWindow* window)
{
    QRectF rect = window->mapRectToScene(QRectF(QPointF(), window->geometry().size()));
    QVector<unsigned long> vector;
    vector << rect.x() << rect.y();
    vector << rect.width() << rect.height();
    return vector;
}

void Ut_MSceneManager::testStatusBarGeometryProperty()
{
    MSceneManager* sceneManager = new MSceneManager;
    MWindow *window = new MWindow(sceneManager);
    MStatusBar* statusbar = new MStatusBar;

    QCOMPARE(getX11Property(window, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY"), QVector<unsigned long>());

    sceneManager->setOrientationAngle(M::Angle0, MSceneManager::ImmediateTransition);

    sceneManager->appearSceneWindowNow(statusbar);

    QCOMPARE(getX11Property(window, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY"), windowGeometry(statusbar));

    sceneManager->setOrientationAngle(M::Angle270, MSceneManager::ImmediateTransition);

    QCOMPARE(getX11Property(window, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY"), windowGeometry(statusbar));

    sceneManager->disappearSceneWindowNow(statusbar);

    // no statusbar - the property is deleted
    QCOMPARE(getX11Property(window, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY"), QVector<unsigned long>());

    // set the property manually
    setX11Property(window, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY", QVector<unsigned long>() << 1 << 2 << 30 << 40);

    sceneManager->setOrientationAngle(M::Angle0, MSceneManager::ImmediateTransition);

    // changing an orientation shouldn't affect the property if the window has no statusbar
    QCOMPARE(getX11Property(window, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY"), QVector<unsigned long>() << 1 << 2 << 30 << 40);

    window->setSceneManager(0);
    delete window;
}

#endif

void Ut_MSceneManager::testSceneWindowsBehindSheetGetHiddenUponSheetAppearance()
{
    MSceneWindow *page = new MApplicationPage;
    MSceneWindow *sheet = new MSheet;

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(page);

    QCOMPARE(page->isVisible(), true);

    sm->appearSceneWindow(sheet);

    // Nothing changes. sheet is still animating its appearance
    QCOMPARE(page->isVisible(), true);

    sm->fastForwardSceneWindowTransitionAnimation(sheet);

    // Sheet has finished its appearance animation. It now covers the entire screen
    // but for the status bar part. Since we consider both status bar and sheet to be
    // fully opaque nothing behind them is visible to a QGraphicsView.
    QCOMPARE(page->isVisible(), false);
}

void Ut_MSceneManager::testSceneWindowsBehindSheetAreShownWhenSheetAboutToDisappear()
{
    MSceneWindow *page = new MApplicationPage;
    MSceneWindow *sheet = new MSheet;

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(page);
    sm->appearSceneWindowNow(sheet);

    // Sheet covers the entire screen but for the status bar part. Since we consider
    // both status bar and sheet to be fully opaque nothing behind them is visible
    // to a QGraphicsView.
    QCOMPARE(page->isVisible(), false);

    sm->disappearSceneWindow(sheet);

    // Sheet is starting to disappear. Scene windows behind it must be visible again
    QCOMPARE(page->isVisible(), true);

    sm->fastForwardSceneWindowTransitionAnimation(sheet);

    // And keep being visible
    QCOMPARE(page->isVisible(), true);
}

void Ut_MSceneManager::testSceneWindowsBehindFirstSheetAreKeptHiddenWhenSecondSheetDisappears()
{
    MSceneWindow *page = new MApplicationPage;
    MSceneWindow *firstSheet = new MSheet;
    MSceneWindow *secondSheet = new MSheet;

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(page);
    sm->appearSceneWindowNow(firstSheet);
    sm->appearSceneWindowNow(secondSheet);

    // Sheet covers the entire screen but for the status bar part. Since we consider
    // both status bar and sheet to be fully opaque nothing behind them is visible
    // to a QGraphicsView.
    QCOMPARE(page->isVisible(), false);
    QCOMPARE(firstSheet->isVisible(), false);

    sm->disappearSceneWindow(secondSheet);

    // second sheet is starting to disappear. First sheet can now be seen.
    QCOMPARE(firstSheet->isVisible(), true);

    // Scene windows behind first it are still occluded though.
    QCOMPARE(page->isVisible(), false);

    sm->fastForwardSceneWindowTransitionAnimation(secondSheet);

    QCOMPARE(firstSheet->isVisible(), true);
    // And keep hidden after second sheet has gone away
    QCOMPARE(page->isVisible(), false);
}

void Ut_MSceneManager::testDisappearingFirstSheetDoesNotAffectOthersVisibility()
{
    MSceneWindow *page = new MApplicationPage;
    MSceneWindow *firstSheet = new MSheet;
    MSceneWindow *secondSheet = new MSheet;

    gMWindowIsOnDisplay = true;
    mWindow->show();

    sm->appearSceneWindowNow(page);
    sm->appearSceneWindowNow(firstSheet);
    sm->appearSceneWindowNow(secondSheet);

    QCOMPARE(page->isVisible(), false);
    QCOMPARE(secondSheet->isVisible(), true);

    sm->disappearSceneWindow(firstSheet);

    // Nothing changes
    QCOMPARE(page->isVisible(), false);
    QCOMPARE(secondSheet->isVisible(), true);

    sm->fastForwardSceneWindowTransitionAnimation(secondSheet);

    // Nothing changes
    QCOMPARE(page->isVisible(), false);
    QCOMPARE(secondSheet->isVisible(), true);
}

QTEST_MAIN(Ut_MSceneManager);
