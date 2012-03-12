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

#include "ut_mapplicationwindow.h"

#include <mapplication.h>
#include <mapplication_p.h>
#include <mapplicationservice.h>
#include <mapplicationwindow.h>
#include <mapplicationwindow_p.h>
#include <mapplicationpage.h>
#include <MComponentData>
#include <MScene>
#include <MSceneManager>
#include <MToolBar>
#include <MInputMethodState>
#include <MNavigationBar>
#include <MApplicationMenu>
#include <MAction>

#include <MSceneWindow>
#include <MHomeButtonPanel>

#include <QSignalSpy>
#include <QEvent>

#include "testpage.h"

// a workaround for scratchbox portrait mode...
// sbox_portrait_support.h is not used because a slightly different
// implementation is needed here
#ifndef __arm__
#include <MDeviceProfile>

M::Orientation MDeviceProfile::orientationFromAngle(M::OrientationAngle angle) const
{
    if (angle == M::Angle90 || angle == M::Angle270)
        return M::Portrait;
    return M::Landscape;
}

QSize MSceneManager::visibleSceneSize(M::Orientation orientation) const
{
    QSize s = MDeviceProfile::instance()->resolution();
    if (s.height() < s.width() && orientation == M::Portrait)
        return QSize(s.height(), s.width());
    return s;
}
#endif // !__arm__

// QCOMPARE doesn't know MSceneWindow::SceneWindowSate enum. Thus it won't
// print "Expected" and "Actual" values in case of failure unless they're cast
// to a known type
#define STATE_COMPARE(s1, s2) QCOMPARE(static_cast<int>(s1), static_cast<int>(s2));

namespace {
    const char *ExistingIconId = "icon-m-common-dialog-close";
};

// Test class implementation

void Ut_MApplicationWindow::initTestCase()
{
    if(MComponentData::instance() == 0) {
        static int argc = 2;
        static char *argv[ 2 ];
        argv[ 0 ] = (char*)"./ut_mscenewindow";
        argv[ 1 ] = (char*)"-local-theme";
        m_componentData = MComponentData::createInstance(argc, argv);
    }
    qRegisterMetaType<MApplicationPage *>("MApplicationPage*");
    qRegisterMetaType< QList<StatusBarTestOperation> >("QList<StatusBarTestOperation>");

    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_mapplicationwindow.css");
}

void Ut_MApplicationWindow::cleanupTestCase()
{
}

void Ut_MApplicationWindow::init()
{
    m_subject = new MApplicationWindow;
}

void Ut_MApplicationWindow::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MApplicationWindow::testConstructorWithoutScene()
{
    QVERIFY(m_subject->scene());
}

void Ut_MApplicationWindow::testConstructorWithScene()
{
    MScene *scene = new MScene;
    MApplicationWindow *window = new MApplicationWindow(scene);
    QCOMPARE((quintptr) window->scene(), (quintptr) scene);
    delete window;
}

void Ut_MApplicationWindow::testIsOnDisplay()
{
    m_subject = MApplication::activeApplicationWindow();

    QPointer<MApplicationPage> page1 = new MApplicationPage;
    QPointer<MApplicationPage> page2 = new MApplicationPage;

    QVERIFY(page1->isOnDisplay() == false);
    QVERIFY(page2->isOnDisplay() == false);

    // Window is not visible => page should not become visible
    m_subject->sceneManager()->appearSceneWindowNow(page2);
    QVERIFY(page1->isOnDisplay() == false);
    QVERIFY(page2->isOnDisplay() == false);

    // To be revisited: how these could be unit-tested
    // (event->viewRect().intersects(sceneBoundingRect())
    // does not return meaningful values in this.

    page1->disappear();
    page1->disappear();
    QVERIFY(page1->isOnDisplay() == false);
    QVERIFY(page2->isOnDisplay() == false);
}

void Ut_MApplicationWindow::testPrestartNoPrestart()
{
    MApplication::setPrestartMode(M::NoPrestart);
    m_subject->show();
    QCOMPARE(m_subject->isVisible(), true);
    m_subject->close();
    QCOMPARE(m_subject->isVisible(), false);
}

void Ut_MApplicationWindow::testPrestartTerminateOnClose()
{
    MApplication::setPrestartMode(M::TerminateOnClose);
    m_subject->show();
    QCOMPARE(m_subject->isVisible(), true);
    m_subject->close();
    QCOMPARE(m_subject->isVisible(), false);
}

void Ut_MApplicationWindow::testPrestartLazyShutdown()
{
    MApplication::setPrestartMode(M::LazyShutdown);
    m_subject->show();
    QCOMPARE(m_subject->isVisible(), true);
    m_subject->close();
    QCOMPARE(m_subject->isVisible(), false);
    QCOMPARE(m_subject->isHidden(), true);
}

void Ut_MApplicationWindow::testWindowActivate()
{
    QVERIFY(MApplication::activeApplicationWindow() == m_subject);

    MApplicationWindow *appWin = new MApplicationWindow;
    QVERIFY(MApplication::activeApplicationWindow() == m_subject);

    QEvent activate(QEvent::WindowActivate);

    qApp->sendEvent(appWin, &activate);
    QVERIFY(MApplication::activeApplicationWindow() == appWin);

    delete appWin;
    QVERIFY(MApplication::activeApplicationWindow() == m_subject);
}

void Ut_MApplicationWindow::testPageChanged()
{
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(MApplicationPage *)));
    MApplicationPage *page = new MApplicationPage;
    MApplicationPage *page2 = new MApplicationPage;
    m_subject->sceneManager()->appearSceneWindowNow(page);
    m_subject->sceneManager()->appearSceneWindowNow(page2);

    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).value<MApplicationPage *>(), page);
    QCOMPARE(spy.at(1).at(0).value<MApplicationPage *>(), page2);

    delete page;
    delete page2;
}

void Ut_MApplicationWindow::testSetWindowIconID()
{
    QString iconId(ExistingIconId);

    m_subject->setWindowIconID(iconId);
    QCOMPARE(m_subject->d_func()->navigationBar->viewMenuIconID(), iconId);
}

void Ut_MApplicationWindow::testCurrentPage()
{
    MApplicationPage *page = new MApplicationPage;

    QCOMPARE(m_subject->currentPage(), (MApplicationPage*) 0);
    m_subject->sceneManager()->appearSceneWindowNow(page);
    QCOMPARE(m_subject->currentPage(), page);
    delete page;
}

void Ut_MApplicationWindow::testMenu()
{
    m_subject->d_func()->navigationBar->setArrowIconVisible(true);

    QCOMPARE(m_subject->isMenuOpen(), false);
    m_subject->openMenu();
    m_subject->sceneManager()->fastForwardSceneWindowTransitionAnimation(m_subject->d_func()->menu);
    QCOMPARE(m_subject->isMenuOpen(), true);
    m_subject->closeMenu();
    QCOMPARE(m_subject->isMenuOpen(), false);
}

void Ut_MApplicationWindow::testIsMenuOpen()
{
    QCOMPARE(m_subject->isMenuOpen(), false);
    m_subject->sceneManager()->appearSceneWindowNow(m_subject->d_func()->menu);
    QCOMPARE(m_subject->isMenuOpen(), true);
    m_subject->sceneManager()->disappearSceneWindowNow(m_subject->d_func()->menu);
    QCOMPARE(m_subject->isMenuOpen(), false);
}

void Ut_MApplicationWindow::testDisplayExitedOnClose()
{
    m_subject->show();
    QSignalSpy spy(m_subject, SIGNAL(displayExited()));
    m_subject->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_MApplicationWindow::testDisplayExitedOnCloseLazyShutdownApp()
{
    MApplication::setPrestartMode(M::LazyShutdown);
    m_subject->show();
    QSignalSpy spy(m_subject, SIGNAL(displayExited()));
    m_subject->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_MApplicationWindow::testPageEscape()
{
    MApplicationPage *page = new MApplicationPage;
    MNavigationBar *navigationBar = m_subject->d_func()->navigationBar;

    page->setEscapeMode(MApplicationPageModel::EscapeManualBack);
    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonClose);

    page->appear(m_subject);
    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonBack);

    page->setEscapeMode(MApplicationPageModel::EscapeCloseWindow);
    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonClose);

    delete page;
}

void Ut_MApplicationWindow::testPageEscapeAuto()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MNavigationBar *navigationBar = m_subject->d_func()->navigationBar;

    QVERIFY(navigationBar != 0);

    m_subject->sceneManager()->appearSceneWindowNow(firstPage);

    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonClose);

    m_subject->sceneManager()->appearSceneWindowNow(secondPage);

    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonBack);

    m_subject->sceneManager()->dismissSceneWindowNow(secondPage);

    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonClose);
}

void Ut_MApplicationWindow::testPageEscapeAutoWhenAddingPageHistory()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MNavigationBar *navigationBar = m_subject->d_func()->navigationBar;
    QList<MSceneWindow *> pageHistory;

    QVERIFY(navigationBar != 0);

    m_subject->sceneManager()->appearSceneWindowNow(secondPage);

    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonClose);

    pageHistory.append(firstPage);
    m_subject->sceneManager()->setPageHistory(pageHistory);

    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonBack);
}

void Ut_MApplicationWindow::testPageEscapeAutoWhenClearingPageHistory()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MNavigationBar *navigationBar = m_subject->d_func()->navigationBar;
    QList<MSceneWindow *> pageHistory;

    QVERIFY(navigationBar != 0);

    m_subject->sceneManager()->appearSceneWindowNow(firstPage);
    m_subject->sceneManager()->appearSceneWindowNow(secondPage);

    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonBack);

    m_subject->sceneManager()->setPageHistory(pageHistory);

    QCOMPARE(navigationBar->escapeButtonMode(), MNavigationBarModel::EscapeButtonClose);
}

void Ut_MApplicationWindow::testComponentsDisplayMode()
{
    MApplicationPage *page = new MApplicationPage;
    // add dummy action to ensure that navigationbar isn't empty
    page->addAction(new QAction("Dummy action", page));
    page->appear(m_subject);

    MSceneManager *sceneManager = m_subject->sceneManager();

    MSceneWindow *homeButtonPanel = m_subject->d_func()->homeButtonPanel;
    MSceneWindow *navigationBar = m_subject->d_func()->navigationBar;

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Appeared);

    page->setComponentsDisplayMode(MApplicationPage::AllComponents, MApplicationPageModel::Hide);
    sceneManager->fastForwardSceneWindowTransitionAnimation(homeButtonPanel);
    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappeared);

    page->setComponentsDisplayMode(MApplicationPage::HomeButton, MApplicationPageModel::Show);
    sceneManager->fastForwardSceneWindowTransitionAnimation(homeButtonPanel);
    sceneManager->fastForwardSceneWindowTransitionAnimation(navigationBar);

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappeared);

    page->setComponentsDisplayMode(MApplicationPage::HomeButton, MApplicationPageModel::AutoHide);
    sceneManager->fastForwardSceneWindowTransitionAnimation(homeButtonPanel);
    sceneManager->fastForwardSceneWindowTransitionAnimation(navigationBar);

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappeared);

    page->setComponentsDisplayMode(MApplicationPage::AllComponents, MApplicationPageModel::Show);
    sceneManager->fastForwardSceneWindowTransitionAnimation(homeButtonPanel);
    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MApplicationWindow::testStatusBarVisibility_data()
{
    QTest::addColumn< QList<StatusBarTestOperation> >("operations");
    QTest::addColumn<MSceneWindow::SceneWindowState>("expectedState");

    typedef QList<StatusBarTestOperation> OpList;

    QTest::newRow("Fullscreen OFF") << (OpList() << MakeNormal) << MSceneWindow::Appeared;
    QTest::newRow("Fullscreen ON") << (OpList() << MakeFullScreen) << MSceneWindow::Disappeared;
    QTest::newRow("Maximize page area") << (OpList() << MakeNormal << MaximizePageArea) << MSceneWindow::Disappeared;
    QTest::newRow("Restore page area") << (OpList() << MakeNormal << RestorePageArea) << MSceneWindow::Appeared;
    QTest::newRow("Restore page area while fullscreen1") << (OpList() << MakeFullScreen << RestorePageArea) << MSceneWindow::Disappeared;
    QTest::newRow("Restore page area while fullscreen2") << (OpList() << MakeFullScreen << RestorePageArea << MakeNormal) << MSceneWindow::Appeared;
    QTest::newRow("Maximize page area while fullscreen1") << (OpList() << MakeFullScreen << MaximizePageArea) << MSceneWindow::Disappeared;
    QTest::newRow("Maximize page area while fullscreen2") << (OpList() << MakeFullScreen << MaximizePageArea << MakeNormal) << MSceneWindow::Disappeared;
    QTest::newRow("Fullscreen OFF, Hide display mode") << (OpList() << PageAppearWithStatusBarHideDisplayMode  << MakeNormal) << MSceneWindow::Disappeared;
    QTest::newRow("Fullscreen ON, Show display mode") << (OpList() << PageAppearWithStatusBarShowDisplayMode  << MakeFullScreen) << MSceneWindow::Disappeared;
}

void Ut_MApplicationWindow::testStatusBarVisibility()
{
    QFETCH(QList<StatusBarTestOperation>, operations);
    QFETCH(MSceneWindow::SceneWindowState, expectedState);

    MSceneWindow *statusBar = 0;

    statusBar = m_subject->d_func()->statusBar;

    if (!statusBar) {
        QSKIP("No status bar used so skipping test.", SkipSingle);
    }

    m_subject->setOrientationAngleLocked(true);
    m_subject->setLandscapeOrientation();

    QApplication::processEvents();
    m_subject->sceneManager()->fastForwardAllSceneWindowTransitionAnimations();

    foreach (StatusBarTestOperation op, operations) {
        switch (op) {
        case MakeFullScreen:
            m_subject->showFullScreen();
            break;
        case MakeNormal:
            m_subject->showNormal();
            break;
        case MaximizePageArea:
            // Faking the presence of an input method panel (e.g.
            // virtual keyboard) which will make MInputWidgetRelocator
            // call MApplicationWindow::maximizePageArea
            MInputMethodState::instance()->setInputMethodArea(
                        QRect(0, 0, m_subject->width(), m_subject->height()/2));
            break;
        case RestorePageArea:
            // MAke the fake input method panel (e.g.
            // virtual keyboard) go away, which will make MInputWidgetRelocator
            // call MApplicationWindow::restorePageArea
            MInputMethodState::instance()->setInputMethodArea(QRect());
            break;
        case PageAppearWithStatusBarHideDisplayMode: {
            MApplicationPage *page = new MApplicationPage;
            page->setComponentsDisplayMode(MApplicationPage::StatusBar,
                                           MApplicationPageModel::Hide);
            page->appear(m_subject);
            } break;
        case PageAppearWithStatusBarShowDisplayMode: {
            MApplicationPage *page = new MApplicationPage;
            page->setComponentsDisplayMode(MApplicationPage::StatusBar,
                                           MApplicationPageModel::Show);
            page->appear(m_subject);
            } break;
        }
        QApplication::processEvents();
        m_subject->sceneManager()->fastForwardAllSceneWindowTransitionAnimations();
    }

    STATE_COMPARE(statusBar->sceneWindowState(), expectedState);

    // Restore original MInputMethodState state since it's a singleton and
    // will therefore preserve its state beyond this test method.
    MInputMethodState::instance()->setInputMethodArea(QRect());
}


void Ut_MApplicationWindow::initToolbarLocationTC(M::OrientationAngle angle, const QString &viewType)
{
    // set orientation
    m_subject->sceneManager()->setOrientationAngle(angle, MSceneManager::ImmediateTransition);

    // set toolbar type
    m_subject->setToolbarViewType(viewType);
}

bool Ut_MApplicationWindow::isToolBarFloating()
{
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    MNavigationBar *navBar = m_subject->d_func()->navigationBar;
    MNavigationBarModel *model = navBar->model();

    return (!model->toolBar() || model->toolBar() != m_subject->d_func()->toolBar);
}

void Ut_MApplicationWindow::testToolBarInLandscapeIsFloating()
{
    initToolbarLocationTC(M::Angle0, MToolBar::defaultType);
    QVERIFY(isToolBarFloating());
}

void Ut_MApplicationWindow::testToolBarInPortraiIsInNavigationBar()
{
    initToolbarLocationTC(M::Angle90, MToolBar::defaultType);
    QVERIFY(isToolBarFloating() == false);
}

void Ut_MApplicationWindow::testToolBarMovesFromFloatingToNavigationBarWhenRotatedToPortrait()
{
    initToolbarLocationTC(M::Angle0, MToolBar::defaultType);
    QVERIFY(isToolBarFloating());

    m_subject->sceneManager()->setOrientationAngle(M::Angle90, MSceneManager::ImmediateTransition);
    QVERIFY(isToolBarFloating() == false);
}

void Ut_MApplicationWindow::testTabBarInLandscapeIsInNavigationBar()
{
    initToolbarLocationTC(M::Angle0, MToolBar::tabType);
    QVERIFY(isToolBarFloating() == false);
}

void Ut_MApplicationWindow::testTabBarInPortraitIsFloating()
{
    initToolbarLocationTC(M::Angle90, MToolBar::tabType);
    QVERIFY(isToolBarFloating());
}

void Ut_MApplicationWindow::testTabBarMovesFromNavigationBarToFloatingWhenRotatedToPortrait()
{
    initToolbarLocationTC(M::Angle0, MToolBar::tabType);
    QVERIFY(isToolBarFloating() == false);

    m_subject->sceneManager()->setOrientationAngle(M::Angle90, MSceneManager::ImmediateTransition);
    QVERIFY(isToolBarFloating());
}

void Ut_MApplicationWindow::processEventsAndFastForwardDisappearAppearAnimations(MSceneWindow *sceneWindow)
{
    // For the navigation bar case, updates on its scene window state based on its "isEmpty"
    // property are deferred to a separate event.
    QCoreApplication::processEvents();

    m_subject->sceneManager()->fastForwardSceneWindowTransitionAnimation(sceneWindow);
}

void Ut_MApplicationWindow::testNavigationBarVisibility_data()
{
    QTest::addColumn<bool>("floatableTabbar");
    QTest::addColumn<bool>("hasCloseButton");
    QTest::addColumn<bool>("hasTitle");
    QTest::addColumn<int>("pageEscapeMode");
    QTest::addColumn<bool>("hasTabbarAction");
    QTest::addColumn<bool>("hasMenuAction");

    QTest::addColumn<bool>("navbarVisibility");

    const int EscapeAuto = MApplicationPageModel::EscapeAuto;
    const int EscapeManualBack = MApplicationPageModel::EscapeManualBack;

    QTest::newRow("floatable tabbar, menu action")
            << true  << false << false << EscapeAuto       << true  << true
            << true;
    QTest::newRow("floatable tabbar, manual back")
            << true  << false << false << EscapeManualBack << true  << false
            << true;
    QTest::newRow("floatable tabbar, close button")
            << true  << true  << false << EscapeAuto       << true  << false
            << true;
    QTest::newRow("floatable tabbar, has title")
            << true  << false << true  << EscapeAuto       << true  << false
            << true;
    QTest::newRow("floatable tabbar, empty navigationbar")
            << true  << false << false << EscapeAuto       << true  << false
            << false;

    QTest::newRow("docked tabbar, tabbar actions")
            << false << false << false << EscapeAuto       << true  << false
            << true;
    QTest::newRow("docked tabbar, no tabbar actions")
            << false << false << false << EscapeAuto       << false << false
            << false;
    QTest::newRow("docked tabbar, no tabbar actions, menu action")
            << false << false << false << EscapeAuto       << false << true
            << true;
    QTest::newRow("docked tabbar, no tabbar actions, manual back")
            << false << false << false << EscapeManualBack << false << false
            << true;
    QTest::newRow("docked tabbar, no tabbar actions, close button")
            << false  << true  << false << EscapeAuto      << false << false
            << true;
    QTest::newRow("docked tabbar, no tabbar actions, has title")
            << false  << false << true  << EscapeAuto      << false << false
            << true;
}

void Ut_MApplicationWindow::testNavigationBarVisibility()
{
    QFETCH(bool, floatableTabbar);
    QFETCH(bool, hasCloseButton);
    QFETCH(bool, hasTitle);
    QFETCH(int, pageEscapeMode);
    QFETCH(bool, hasTabbarAction);
    QFETCH(bool, hasMenuAction);
    QFETCH(bool, navbarVisibility);

    // in ut_mapplicationwindow.css tabbar is floatable in portrait
    initToolbarLocationTC(floatableTabbar ? M::Angle270 : M::Angle0, MToolBar::tabType);

    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;
    if (!hasCloseButton && !hasTitle)
        navigationBar->setStyleName("");
    else if (hasCloseButton && !hasTitle)
        navigationBar->setStyleName("closebtn");
    else if (hasCloseButton && hasTitle)
        navigationBar->setStyleName("title_closebtn");
    else if (!hasCloseButton && hasTitle)
        navigationBar->setStyleName("title");

    MApplicationPage* page = new MApplicationPage;
    page->setEscapeMode((MApplicationPageModel::PageEscapeMode)pageEscapeMode);
    if (hasMenuAction) {
        MAction* action = new MAction("Menu action", page);
        action->setLocation(MAction::ApplicationMenuLocation);
        page->addAction(action);
    }

    if (hasTabbarAction) {
        MAction* action1 = new MAction("Tabbar action1", page);
        action1->setCheckable(true);
        action1->setLocation(MAction::ToolBarLocation);
        MAction* action2 = new MAction("Tabbar action2", page);
        action2->setCheckable(true);
        action2->setChecked(true);
        action2->setLocation(MAction::ToolBarLocation);

        page->addAction(action1);
        page->addAction(action2);
    }

    m_subject->sceneManager()->appearSceneWindow(page);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), navbarVisibility);

    delete page;
}

void Ut_MApplicationWindow::testNavigationBarVisibilityFloatableTabbar()
{
    // floatable tabbar
    initToolbarLocationTC(M::Angle270, MToolBar::tabType);

    // no close button, no title
    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;
    navigationBar->setStyleName("");

    MApplicationPage* page = new MApplicationPage;
    MAction* action1 = new MAction("Tabbar action1", page);
    action1->setCheckable(true);
    action1->setLocation(MAction::ToolBarLocation);
    MAction* action2 = new MAction("Tabbar action2", page);
    action2->setCheckable(true);
    action2->setChecked(true);
    action2->setLocation(MAction::ToolBarLocation);
    MAction* menuAction = new MAction("Menu action", page);
    menuAction->setLocation(MAction::ApplicationMenuLocation);

    page->addAction(action1);
    page->addAction(action2);
    page->setEscapeMode(MApplicationPageModel::EscapeAuto);

    m_subject->sceneManager()->appearSceneWindowNow(page);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->addAction(menuAction);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->removeAction(menuAction);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->setEscapeMode(MApplicationPageModel::EscapeManualBack);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->setEscapeMode(MApplicationPageModel::EscapeAuto);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    delete page;
}

void Ut_MApplicationWindow::testNavigationBarVisibilityDockedTabbar()
{
    // docked tabbar
    initToolbarLocationTC(M::Angle0, MToolBar::tabType);

    // no close button, no title
    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;
    navigationBar->setStyleName("");

    MApplicationPage* page = new MApplicationPage;
    MAction* action1 = new MAction("Tabbar action1", page);
    action1->setCheckable(true);
    action1->setLocation(MAction::ToolBarLocation);
    MAction* action2 = new MAction("Tabbar action2", page);
    action2->setCheckable(true);
    action2->setChecked(true);
    action2->setLocation(MAction::ToolBarLocation);
    MAction* menuAction = new MAction("Menu action", page);
    menuAction->setLocation(MAction::ApplicationMenuLocation);

    page->addAction(action1);
    page->addAction(action2);
    page->setEscapeMode(MApplicationPageModel::EscapeAuto);

    m_subject->sceneManager()->appearSceneWindowNow(page);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->removeAction(action1);
    page->removeAction(action2);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->addAction(menuAction);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->removeAction(menuAction);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->setEscapeMode(MApplicationPageModel::EscapeManualBack);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->setEscapeMode(MApplicationPageModel::EscapeAuto);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->addAction(action1);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    delete page;
}

void Ut_MApplicationWindow::testNavigationBarVisibilityHideToolbarAction()
{
    // docked tabbar
    initToolbarLocationTC(M::Angle0, MToolBar::tabType);

    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;

    MAction* action = new MAction("Action", m_subject);
    action->setLocation(MAction::ToolBarLocation);
    m_subject->addAction(action);

    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    action->setVisible(false);
    processEventsAndFastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);
}

void Ut_MApplicationWindow::testFirstPageWithCustomNavigationBarContent()
{
    MSceneManager *sceneManager = m_subject->sceneManager();
    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;
    MApplicationPage* page = new MApplicationPage;
    QGraphicsWidget *customNavBarContent = new QGraphicsWidget;

    page->setCustomNavigationBarContent(customNavBarContent);

    m_subject->show();

    sceneManager->appearSceneWindow(page);

    QVERIFY(navigationBar->customContent() == customNavBarContent);

    sceneManager->fastForwardSceneWindowTransitionAnimation(navigationBar);

    sceneManager->dismissSceneWindow(page);

    QVERIFY(navigationBar->customContent() == 0);
}

void Ut_MApplicationWindow::testNavigationBarOpacity()
{
    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;

    QCOMPARE(m_subject->navigationBarOpacity(), (qreal)1.0);
    QCOMPARE(navigationBar->opacity(), (qreal)1.0);

    m_subject->setNavigationBarOpacity((qreal)0.5);

    QCOMPARE(m_subject->navigationBarOpacity(), (qreal)0.5);
    QCOMPARE(navigationBar->opacity(), (qreal)0.5);
}

// Regression test for NB#205177
// https://projects.maemo.org/bugzilla/show_bug.cgi?id=205177
void Ut_MApplicationWindow::testGoingBackDoesntMakeNavigationBarDisappearAndReappear()
{
    MApplicationPage *rootPage = new MApplicationPage;
    MApplicationPage *subPage = new MApplicationPage;
    MAction *action;
    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;
    MSceneManager *sceneManager = m_subject->sceneManager();

    action = new MAction(rootPage);
    action->setText("foo bar");
    action->setLocation(MAction::ApplicationMenuLocation);
    rootPage->addAction(action);

    sceneManager->appearSceneWindowNow(rootPage);
    sceneManager->appearSceneWindowNow(subPage);

    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Appeared);

    sceneManager->dismissSceneWindow(subPage);

    // Navigation bar shouldn't go away since the root page needs the navigation
    // bar to at least display a application menu button.
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MApplicationWindow::testNavigationBarInOutAnimations_data()
{
    //init data
    QTest::addColumn<bool>("hasRootPage");
    QTest::addColumn<int>("rootPageNavBarDisplayMode");
    QTest::addColumn<bool>("hasSubPage");
    QTest::addColumn<int>("subPageNavBarDisplayMode");
    QTest::addColumn<bool>("windowHasActions");
    //results
    QTest::addColumn<MSceneWindow::SceneWindowState>("initialNavBarState");
    QTest::addColumn<MSceneWindow::SceneWindowState>("subPageNavBarState");
    QTest::addColumn<MSceneWindow::SceneWindowState>("finalNavBarState");

    // first page no actions, second page back button > no navbar, navbar slides in
    QTest::newRow("1st page no actions, 2nd back button")
            << true  << (int)MApplicationPageModel::Show << true << (int)MApplicationPageModel::Show << false
            << MSceneWindow::Disappeared << MSceneWindow::Appearing << MSceneWindow::Appeared;

    // first page no actions, second page back button and navbar hidden > no navbar, no navbar
    QTest::newRow("1st page no actions, 2nd back button and navbar hidden")
            << true  << (int)MApplicationPageModel::Show << true << (int)MApplicationPageModel::Hide << false
            << MSceneWindow::Disappeared << MSceneWindow::Disappeared << MSceneWindow::Disappeared;

    // first page one action (hidden before appear),
    // second page back button and navbar hidden in create contents > no navbar, no navbar
    QTest::newRow("1st page one action (hidden), 2nd + back button and navbar hidden")
            << true  << (int)MApplicationPageModel::Hide << true << (int)MApplicationPageModel::Hide << true
            << MSceneWindow::Disappeared << MSceneWindow::Disappeared << MSceneWindow::Disappeared;

    // first page one action (hidden before appear),
    // second page + back button > no navbar, navbar slides in
    QTest::newRow("1st page one action, 2nd page + back button")
            << true  << (int)MApplicationPageModel::Hide << true << (int)MApplicationPageModel::Show << true
            << MSceneWindow::Disappeared << MSceneWindow::Appearing << MSceneWindow::Appeared;

    // first page one action (hiiden before appear) > no navbar
    QTest::newRow("1st page one action (hidden before appear)")
            << true  << (int)MApplicationPageModel::Hide << false << (int)MApplicationPageModel::Show << true
            << MSceneWindow::Disappeared << MSceneWindow::Disappeared << MSceneWindow::Disappeared;

    // app window no page, no actions > no navbar
    QTest::newRow("app window no page, no actions")
            << false << (int)MApplicationPageModel::Show << false << (int)MApplicationPageModel::Show << false
            << MSceneWindow::Disappeared << MSceneWindow::Disappeared << MSceneWindow::Disappeared;

    // app window no page, one action > initial navbar
    QTest::newRow("app window no page, one action")
            << false << (int)MApplicationPageModel::Show << false << (int)MApplicationPageModel::Show << true
            << MSceneWindow::Appeared << MSceneWindow::Disappeared << MSceneWindow::Appeared;

    // 1st page with actions > initial navbar
    QTest::newRow("1st page with actions")
            << true << (int)MApplicationPageModel::Show << false << (int)MApplicationPageModel::Show << true
            << MSceneWindow::Appeared << MSceneWindow::Disappeared << MSceneWindow::Appeared;

    // 1st page with actions, navbar hidden  > no navbar
    QTest::newRow("1st page with actions, navbar hidden")
            << true << (int)MApplicationPageModel::Hide << false << (int)MApplicationPageModel::Show << true
            << MSceneWindow::Disappeared << MSceneWindow::Disappeared << MSceneWindow::Disappeared;

    // 1st page one action, 2nd page + back button (hidden before appear)
    //          > initial navbar, navbar slides out
    QTest::newRow("1st page with actions, 2nd page navbar hidden")
            << true << (int)MApplicationPageModel::Show << true << (int)MApplicationPageModel::Hide << true
            << MSceneWindow::Appeared << MSceneWindow::Disappearing << MSceneWindow::Disappeared;


}

void Ut_MApplicationWindow::testNavigationBarInOutAnimations()
{
    QFETCH(bool, hasRootPage);
    QFETCH(int,rootPageNavBarDisplayMode);
    QFETCH(bool, hasSubPage);
    QFETCH(int,subPageNavBarDisplayMode);
    QFETCH(bool, windowHasActions);
    QFETCH(MSceneWindow::SceneWindowState,initialNavBarState);
    QFETCH(MSceneWindow::SceneWindowState,subPageNavBarState);
    QFETCH(MSceneWindow::SceneWindowState,finalNavBarState);

    MApplicationPage *rootPage = new MApplicationPage;
    MApplicationPage *subPage = new MApplicationPage;
    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;
    MSceneManager *sceneManager = m_subject->sceneManager();

    if(windowHasActions) {
        MAction* action = new MAction("window action", m_subject);
        action->setLocation(MAction::ApplicationMenuLocation);
        m_subject->addAction(action);
    }

    if(hasRootPage) {
        rootPage->setComponentsDisplayMode(MApplicationPage::AllComponents, (MApplicationPageModel::ComponentDisplayMode) rootPageNavBarDisplayMode);
        rootPage->appear();
    }

    m_subject->show();
    QCoreApplication::processEvents();
    QCOMPARE(navigationBar->sceneWindowState(), initialNavBarState);

    if(hasSubPage) {
        subPage->setComponentsDisplayMode(MApplicationPage::AllComponents, (MApplicationPageModel::ComponentDisplayMode) subPageNavBarDisplayMode);
        subPage->appear();
        QCOMPARE(navigationBar->sceneWindowState(), subPageNavBarState);
    }

    sceneManager->fastForwardSceneWindowTransitionAnimation(navigationBar);
    QCOMPARE(navigationBar->sceneWindowState(), finalNavBarState);
}

void Ut_MApplicationWindow::testNavigationBarShowAfterNonAnimatedPageAppearance()
{
    MApplicationPage *page = new MApplicationPage;
    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;

    page->setEscapeMode(MApplicationPageModel::EscapeManualBack);

    page->setComponentsDisplayMode(MApplicationPage::NavigationBar, MApplicationPageModel::Hide);

    page->appear(m_subject);
    m_subject->show();

    QCOMPARE(page->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE((int)navigationBar->sceneWindowState(), (int)MSceneWindow::Disappeared);

    page->setComponentsDisplayMode(MApplicationPage::NavigationBar, MApplicationPageModel::Show);

    QCoreApplication::processEvents();

    QCOMPARE((int)navigationBar->sceneWindowState(), (int)MSceneWindow::Appearing);

    delete page;
}

/* during a page swtich animation the navigation bar should
   appear or disappear using a fade animation
*/
void Ut_MApplicationWindow::testNavigationBarFadesInPageSwitch()
{
    MApplicationPage *rootPage = new MApplicationPage;
    MApplicationPage *childPage = new MApplicationPage;
    MSceneWindow *navigationBar = m_subject->d_func()->navigationBar;
    MSceneManager *sceneManager = m_subject->sceneManager();

    rootPage->appear(m_subject);

    m_subject->show();

    QApplication::processEvents();

    STATE_COMPARE(rootPage->sceneWindowState(), MSceneWindow::Appeared);
    STATE_COMPARE(childPage->sceneWindowState(), MSceneWindow::Disappeared);
    STATE_COMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappeared);

    childPage->appear(m_subject);

    STATE_COMPARE(rootPage->sceneWindowState(), MSceneWindow::Disappearing);
    STATE_COMPARE(childPage->sceneWindowState(), MSceneWindow::Appearing);
    STATE_COMPARE(navigationBar->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(navigationBar->property("_m_appearanceAnimation").toString(),
             QString("MWidgetFadeAnimation"));

    sceneManager->fastForwardAllSceneWindowTransitionAnimations();

    childPage->dismiss();

    STATE_COMPARE(rootPage->sceneWindowState(), MSceneWindow::Appearing);
    STATE_COMPARE(childPage->sceneWindowState(), MSceneWindow::Disappearing);
    STATE_COMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(navigationBar->property("_m_disappearanceAnimation").toString(),
             QString("MWidgetFadeAnimation"));
}

/* if the navigation bar is shown or hidden while the current page remains
   the same (e.g. tapping on the page to get a fullscreen view), it should do
   so using a slide animation
*/
void Ut_MApplicationWindow::testNavigationBarSlidesIfPageIsStatic()
{
    MApplicationPage *page = new MApplicationPage;
    MNavigationBar *navigationBar = m_subject->d_func()->navigationBar;
    MSceneManager *sceneManager = m_subject->sceneManager();

    // Put something on the nav bar so that it's not completely empty
    // MApplicationWindow will refuse to show an empty navigation bar.
    page->setCustomNavigationBarContent(new QGraphicsWidget);

    page->appear(m_subject);

    m_subject->show();

    QApplication::processEvents();

    STATE_COMPARE(page->sceneWindowState(), MSceneWindow::Appeared);
    STATE_COMPARE(navigationBar->sceneWindowState(), MSceneWindow::Appeared);

    page->setComponentsDisplayMode(MApplicationPage::NavigationBar,
                                   MApplicationPageModel::Hide);
    // processing of new nav bar display mode is done only in the next event loop
    QApplication::processEvents();

    STATE_COMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappearing);
    // That's what we are really interested in checking in this test
    QCOMPARE(navigationBar->property("_m_disappearanceAnimation").toString(),
             QString("MWidgetSlideAnimation"));

    sceneManager->fastForwardAllSceneWindowTransitionAnimations();

    STATE_COMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappeared);

    page->setComponentsDisplayMode(MApplicationPage::NavigationBar,
                                   MApplicationPageModel::Show);
    // processing of new nav bar display mode is done only in the next event loop
    QApplication::processEvents();

    STATE_COMPARE(navigationBar->sceneWindowState(), MSceneWindow::Appearing);
    // That's what we are really interested in checking in this test
    QCOMPARE(navigationBar->property("_m_appearanceAnimation").toString(),
             QString("MWidgetSlideAnimation"));
}

/*
  Check if page::exposedContentRect() gets properly updated if status bar
  appears immediately

  Regression test for https://projects.maemo.org/bugzilla/show_bug.cgi?id=274325
 */
void Ut_MApplicationWindow::testPageExposedContentRectWhenStatusBarAppearsImmediately()
{
    MApplicationPage *page = new MApplicationPage;
    MSceneWindow *statusBar = m_subject->d_func()->statusBar;

    // We don't want MOrientationTracker to mess up with
    // the orientation angle of our subject.
    m_subject->setOrientationAngleLocked(true);
    m_subject->setLandscapeOrientation();

    page->appear(m_subject);

    // Unlike in real usage, showing the window here will cause
    // a paint event to come before the zeroed timer that calls
    //_q_updateStatusBarVisibility. That will cause the status bar
    // to be shown in an animated fashion instead of immediately
    //m_subject->show();

    QApplication::processEvents();

    // make sure it came immediately
    STATE_COMPARE(statusBar->sceneWindowState(), MSceneWindow::Appeared);

    QRectF expectedExposedContentRect;
    expectedExposedContentRect.setWidth(m_subject->width());
    expectedExposedContentRect.setHeight(
                m_subject->height() - statusBar->geometry().height());

    QCOMPARE(page->exposedContentRect(), expectedExposedContentRect);
}

/*
  Check if page::exposedContentRect() gets properly updated after a change in
  orientation. When the orientation changes (from landscape to portrait or
  vice-versa), the page geometry is also changed (unless the screen is
  perfectly square) which, on its turn, causes page::exposedContentRect()
  to be different.
 */
void Ut_MApplicationWindow::testPageExposedContentRectAfterOrientationChange()
{
    MApplicationPage *page = new MApplicationPage;
    MSceneManager *sceneManager = m_subject->sceneManager();
    QRectF expectedExposedContentRect;

    // We don't want MOrientationTracker to mess up with
    // the orientation angle of our subject.
    m_subject->setOrientationAngleLocked(true);

    m_subject->setPortraitOrientation();
    sceneManager->fastForwardOrientationChangeAnimation();

    // let's take status bar out of the equation to simplify
    // the calculation of the exposed content rect.
    page->setComponentsDisplayMode(MApplicationPage::StatusBar,
                                   MApplicationPageModel::Hide);
    page->appear(m_subject);

    m_subject->show();

    QApplication::processEvents();

    sceneManager->fastForwardAllSceneWindowTransitionAnimations();

    expectedExposedContentRect.setWidth(m_subject->height());
    expectedExposedContentRect.setHeight(m_subject->width());
    QCOMPARE(page->exposedContentRect(), expectedExposedContentRect);

    m_subject->setLandscapeOrientation();
    sceneManager->fastForwardOrientationChangeAnimation();

    expectedExposedContentRect.setWidth(m_subject->width());
    expectedExposedContentRect.setHeight(m_subject->height());
    QCOMPARE(page->exposedContentRect(), expectedExposedContentRect);
}

/*
  Check the exposedContentRect of page after two orientation changes.

  Regression test for: https://projects.maemo.org/bugzilla/show_bug.cgi?id=276436
 */
void Ut_MApplicationWindow::testPageExposedContentRectAfterTwoOrientationChanges()
{
    MApplicationPage *page = new MApplicationPage;
    MSceneManager *sceneManager = m_subject->sceneManager();

    // We don't want MOrientationTracker to mess up with
    // the orientation angle of our subject.
    m_subject->setOrientationAngleLocked(true);
    m_subject->setLandscapeOrientation();
    sceneManager->fastForwardOrientationChangeAnimation();
    sceneManager->fastForwardAllSceneWindowTransitionAnimations();

    // force navigation bar to be displayed
    page->setEscapeMode(MApplicationPageModel::EscapeManualBack);

    page->appear(m_subject);
    m_subject->show();

    QApplication::processEvents();
    sceneManager->fastForwardAllSceneWindowTransitionAnimations();

    QRectF initialContentRect = page->exposedContentRect();

    // to to protrait
    m_subject->setPortraitOrientation();
    sceneManager->fastForwardOrientationChangeAnimation();
    sceneManager->fastForwardAllSceneWindowTransitionAnimations();

    // and then return back to landscape
    m_subject->setLandscapeOrientation();
    sceneManager->fastForwardOrientationChangeAnimation();
    sceneManager->fastForwardAllSceneWindowTransitionAnimations();

    // we should have the same exposedContentRect() as we had initially,
    // before going to portrait
    QCOMPARE(initialContentRect, page->exposedContentRect());
}

/*
  Regression test for the following bug:
  https://projects.maemo.org/bugzilla/show_bug.cgi?id=277533
 */
void Ut_MApplicationWindow::testRotatingPageAndShowingStatusBarDoesNotCrash()
{
    MSceneManager *sceneManager = m_subject->sceneManager();
    MApplicationPage *rootPage = new TestPage;
    MApplicationPage *childPage = new TestPage;
    MSceneWindow *statusBar = 0;

    statusBar = m_subject->d_func()->statusBar;
    if (!statusBar) {
        QSKIP("No status bar used so skipping test.", SkipSingle);
    }

    // We don't want MOrientationTracker to mess up with
    // the orientation angle of our subject.
    m_subject->setOrientationAngleLocked(true);

    m_subject->setLandscapeOrientation();
    sceneManager->fastForwardOrientationChangeAnimation();

    sceneManager->appearSceneWindowNow(rootPage);

    m_subject->show();

    // Set MWindowPrivate::beforeFirstPaintEvent to false, causing status bar [dis]appearances
    // to be animated as defined in MApplicationWindowPrivate::_q_updateStatusBarVisibility()
    MWindow *w = dynamic_cast<MWindow *>(m_subject);
    w->d_func()->beforeFirstPaintEvent = false;

    STATE_COMPARE(statusBar->sceneWindowState(), MSceneWindow::Disappeared)

    sceneManager->appearSceneWindowNow(childPage);

    // Causes TestPage::orientationChangeEvent() to be called, which will ask
    // MApplicationWindow to show its status bar
    m_subject->setPortraitOrientation();
    sceneManager->fastForwardOrientationChangeAnimation();

    STATE_COMPARE(statusBar->sceneWindowState(), MSceneWindow::Appearing)

    // Simulate a user tapping on the back button right after the rotation
    // animation is complete
    childPage->dismiss();

    STATE_COMPARE(childPage->sceneWindowState(), MSceneWindow::Disappearing)
    STATE_COMPARE(rootPage->sceneWindowState(), MSceneWindow::Appearing)

    // In the original bug, this call would cause a crash as status bar
    // reaches its Appeared state.
    sceneManager->fastForwardAllSceneWindowTransitionAnimations();

    STATE_COMPARE(statusBar->sceneWindowState(), MSceneWindow::Appeared)
    STATE_COMPARE(childPage->sceneWindowState(), MSceneWindow::Disappeared)
    STATE_COMPARE(rootPage->sceneWindowState(), MSceneWindow::Appeared)
}

QTEST_MAIN(Ut_MApplicationWindow)
