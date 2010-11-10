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

#include "ut_mapplicationwindow.h"

#include <mapplication.h>
#include <mapplication_p.h>
#include <mapplicationservice.h>
#include <mapplicationwindow.h>
#include <mapplicationwindow_p.h>
#include <mapplicationpage.h>
#include <minputmethodstate.h>
#include <MComponentData>
#include <MScene>
#include <MSceneManager>
#include <MToolBar>
#include <MNavigationBar>
#include <MApplicationMenu>
#include <MAction>

#include <MSceneWindow>
#include <MHomeButtonPanel>
#include <mscenemanagertestbridge.h>

#include <QSignalSpy>
#include <QEvent>

// Test class implementation

void Ut_MApplicationWindow::initTestCase()
{
    if(MComponentData::instance() == 0) {
        static int argc = 2;
        static char *argv[ 2 ];
        argv[ 0 ] = (char*)"./ut_mscenewindow";
        argv[ 1 ] = (char*)"-local-theme";
        m_componentData = new MComponentData(argc, argv);
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
    QString iconId("icon-id");

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
    MSceneManagerTestBridge testBridge;
    testBridge.setParent(m_subject->sceneManager());

    m_subject->d_func()->navigationBar->setArrowIconVisible(true);

    QCOMPARE(m_subject->isMenuOpen(), false);
    m_subject->openMenu();
    testBridge.fastForwardSceneWindowTransitionAnimation(m_subject->d_func()->menu);
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

    MSceneManagerTestBridge testBridge;
    testBridge.setParent(m_subject->sceneManager());

    MSceneWindow *homeButtonPanel = m_subject->d_func()->homeButtonPanel;
    MSceneWindow *navigationBar = m_subject->d_func()->navigationBar;

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Appeared);
    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Appeared);

    page->setComponentsDisplayMode(MApplicationPage::AllComponents, MApplicationPageModel::Hide);
    testBridge.fastForwardSceneWindowTransitionAnimation(homeButtonPanel);
    testBridge.fastForwardSceneWindowTransitionAnimation(navigationBar);

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappeared);

    page->setComponentsDisplayMode(MApplicationPage::HomeButton, MApplicationPageModel::Show);
    testBridge.fastForwardSceneWindowTransitionAnimation(homeButtonPanel);
    testBridge.fastForwardSceneWindowTransitionAnimation(navigationBar);

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappeared);

    page->setComponentsDisplayMode(MApplicationPage::HomeButton, MApplicationPageModel::AutoHide);
    testBridge.fastForwardSceneWindowTransitionAnimation(homeButtonPanel);
    testBridge.fastForwardSceneWindowTransitionAnimation(navigationBar);

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Disappeared);

    page->setComponentsDisplayMode(MApplicationPage::AllComponents, MApplicationPageModel::Show);
    testBridge.fastForwardSceneWindowTransitionAnimation(homeButtonPanel);
    testBridge.fastForwardSceneWindowTransitionAnimation(navigationBar);

    QCOMPARE(homeButtonPanel->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(navigationBar->sceneWindowState(), MSceneWindow::Appeared);
}

void Ut_MApplicationWindow::testStatusBarVisibility_data()
{
    QTest::addColumn< QList<StatusBarTestOperation> >("operations");
    QTest::addColumn<bool>("expectedVisibility");  // true = visible, false = invisible

    typedef QList<StatusBarTestOperation> OpList;

    QTest::newRow("Fullscreen OFF") << (OpList() << MakeNormal) << true;
    QTest::newRow("Fullscreen ON") << (OpList() << MakeFullScreen) << false;
    QTest::newRow("Maximize page area") << (OpList() << MakeNormal << MaximizePageArea) << false;
    QTest::newRow("Restore page area") << (OpList() << MakeNormal << RestorePageArea) << true;
    QTest::newRow("Restore page area while fullscreen1") << (OpList() << MakeFullScreen << RestorePageArea) << false;
    QTest::newRow("Restore page area while fullscreen2") << (OpList() << MakeFullScreen << RestorePageArea << MakeNormal) << true;
    QTest::newRow("Maximize page area while fullscreen1") << (OpList() << MakeFullScreen << MaximizePageArea) << false;
    QTest::newRow("Maximize page area while fullscreen2") << (OpList() << MakeFullScreen << MaximizePageArea << MakeNormal) << false;
}

void Ut_MApplicationWindow::testStatusBarVisibility()
{
    QFETCH(QList<StatusBarTestOperation>, operations);
    QFETCH(bool, expectedVisibility);

    MSceneManagerTestBridge testBridge;
    testBridge.setParent(m_subject->sceneManager());

    MSceneWindow *statusBar = 0;

    statusBar = m_subject->d_func()->statusBar;

    if (!statusBar) {
        QSKIP("No status bar used so skipping test.", SkipSingle);
    }

    m_subject->sceneManager()->setOrientationAngle(M::Angle0, MSceneManager::ImmediateTransition);
    QVERIFY(m_subject->orientationAngle() == M::Angle0);

    foreach (StatusBarTestOperation op, operations) {
        switch (op) {
        case MakeFullScreen:
            m_subject->showFullScreen();
            break;
        case MakeNormal:
            m_subject->showNormal();
            break;
        case MaximizePageArea:
            // Status bar hides because along with other decorations.
            m_subject->d_func()->pageAreaMaximized = true;
            QMetaObject::invokeMethod(m_subject, "_q_updateStatusBarVisibility", Qt::DirectConnection);
            break;
        case RestorePageArea:
            m_subject->d_func()->pageAreaMaximized = false;
            QMetaObject::invokeMethod(m_subject, "_q_updateStatusBarVisibility", Qt::DirectConnection);
            break;
        }
    }

    testBridge.fastForwardSceneWindowTransitionAnimation(statusBar);
    // This relies on status bar being out of display which means no animation
    // and change in QGraphicsItem visibility is immediate.
    QCOMPARE(statusBar->isVisible(), expectedVisibility);
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

void Ut_MApplicationWindow::fastForwardDisappearAppearAnimations(MSceneWindow *sceneWindow)
{
    MSceneManagerTestBridge testBridge;
    testBridge.setParent(m_subject->sceneManager());

    // we can have at most 1 ongoing and 1 queued disappearing/appearing animation
    // ensure both animations are fast forwarded
    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);
    testBridge.fastForwardSceneWindowTransitionAnimation(sceneWindow);
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

    fastForwardDisappearAppearAnimations(navigationBar);
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

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->addAction(menuAction);

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->removeAction(menuAction);

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->setEscapeMode(MApplicationPageModel::EscapeManualBack);

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->setEscapeMode(MApplicationPageModel::EscapeAuto);

    fastForwardDisappearAppearAnimations(navigationBar);
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

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->removeAction(action1);
    page->removeAction(action2);

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->addAction(menuAction);

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->removeAction(menuAction);

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->setEscapeMode(MApplicationPageModel::EscapeManualBack);

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    page->setEscapeMode(MApplicationPageModel::EscapeAuto);

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);

    page->addAction(action1);

    fastForwardDisappearAppearAnimations(navigationBar);
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

    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), true);

    action->setVisible(false);
    fastForwardDisappearAppearAnimations(navigationBar);
    QCOMPARE(navigationBar->isVisible(), false);
}

void Ut_MApplicationWindow::testFirstPageWithCustomNavigationBarContent()
{
    MSceneManagerTestBridge testBridge;
    testBridge.setParent(m_subject->sceneManager());
    MNavigationBar* navigationBar = m_subject->d_func()->navigationBar;
    MApplicationPage* page = new MApplicationPage;
    QGraphicsWidget *customNavBarContent = new QGraphicsWidget;

    page->setCustomNavigationBarContent(customNavBarContent);

    m_subject->show();

    m_subject->sceneManager()->appearSceneWindow(page);

    QVERIFY(navigationBar->customContent() == customNavBarContent);

    testBridge.fastForwardSceneWindowTransitionAnimation(navigationBar);

    m_subject->sceneManager()->dismissSceneWindow(page);

    QVERIFY(navigationBar->customContent() == 0);
}

QTEST_MAIN(Ut_MApplicationWindow)
