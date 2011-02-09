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
#include <MComponentData>
#include <MScene>
#include <MSceneManager>
#include <MToolBar>
#include <MNavigationBar>
#include <MApplicationMenu>
#include <MAction>

#include <MSceneWindow>
#include <MHomeButtonPanel>

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

    m_subject->sceneManager()->fastForwardSceneWindowTransitionAnimation(statusBar);
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

QTEST_MAIN(Ut_MApplicationWindow)
