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

#include "mondisplaychangeevent.h"

#include <MDebug>
#include <mscenewindowevent_p.h>
#include "mapplicationwindow_p.h"
#include "mapplicationwindow.h"
#include "mapplicationwindowstyle.h"
#include "mapplicationpage.h"
#include "mapplicationpage_p.h"
#include "mnavigationbar.h"
#include "mapplicationmenu.h"
#ifndef Q_OS_WIN
#include "mcomponentcache.h"
#endif
#include <mdynamicpropertywatcher.h>
#include "mtoolbar.h"
#include "mtoolbar_p.h"
#include "mdockwidget.h"
#include "mcomponentdata.h"
#include "maction.h"
#include "mhomebuttonpanel.h"
#include "mapplication.h"
#include "mcomponentdata.h"
#include "mscenemanager.h"
#include "mscenemanager_p.h"
#include "mscene.h"
#include "mstatusbar.h"
#include "mdeviceprofile.h"
#include "mcomponentdata.h"

#include <QList>
#include <QEvent>
#include <QRectF>
#include <QActionEvent>

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
#endif

#ifdef M_OS_MAEMO5
#include <QDBusConnection>
#include <QDBusMessage>
#endif //M_OS_MAEMO5

namespace {
    // TODO: this may be moved to some header if needed elsewhere
    // Returns the longest length variant of a translated string.
    // If a translated string has many length variants, they are
    // separated with U+009C (STRING TERMINATOR) character, and
    // according to DTD for .ts file format, they should be ordered
    // by decreasing length.
    // This helper function returns the longest length variant
    // by returning the string up to the first U+009C character.
    // If there's only one length variant, the function returns
    // the whole unprocessed string.
    QString longestLengthVariant(const QString &text)
    {
        return text.left(text.indexOf(QChar(0x9c)));
    }
}

MApplicationWindowPrivate::MApplicationWindowPrivate()
    : MWindowPrivate()
    , page(NULL)
    , navigationBar(NULL)
    , toolBar(NULL)
    , dockWidget(new MDockWidget)
    , homeButtonPanel(NULL)
    , menu(new MApplicationMenu)
    , statusBar(NULL)
    , pageAreaMaximized(false)
#ifdef Q_WS_X11
    , isChained(false)
    , chainParentWinId(0)
    , chainTaskTitle()
#endif
#ifdef HAVE_CONTEXTSUBSCRIBER
    , callStatusProperty("Phone.Call")
#endif
    , showingNavigationBar(false)
    , showingDockWidget(false)
    , animateComponentsTransitions(false)
    , navigationBarPressed(false)
    , styleContainer(0)
{

}

MApplicationWindowPrivate::~MApplicationWindowPrivate()
{
    if (dockWidget) {
        delete dockWidget;
        dockWidget = 0;
    }
    if (toolBar) {
        delete toolBar;
        toolBar = 0;
    }

    if (menu) {
        delete menu;
        menu = 0;
    }

    if (navigationBar) {
        // MNavigationBarView is not destroyed until MNavigationBar destructor chain reaches ~MWidgetController
        // so we have to notify the view earlier that the toolbar is already destroyed.
        navigationBar->undockToolBar();
        delete navigationBar;
        navigationBar = 0;
    }
    if (homeButtonPanel) {
        delete homeButtonPanel;
        homeButtonPanel = 0;
    }
    if (statusBar) {
        delete statusBar;
        statusBar = 0;
    }
    if (styleContainer){
        delete styleContainer;
        styleContainer = 0;
    }
}

void MApplicationWindowPrivate::init()
{
    Q_Q(MApplicationWindow);

    navigationBar = new MNavigationBar;
    toolBar = new MToolBar(navigationBar);
    homeButtonPanel = new MHomeButtonPanel;

    if(MDeviceProfile::instance()->showStatusbar())    {
        statusBar = new MStatusBar;
    }
    else{
        statusBar = NULL;
    }

    q->connect(q, SIGNAL(orientationChanged(M::Orientation)), q, SLOT(_q_updateStyle()));
    q->connect(MTheme::instance(), SIGNAL(themeChangeCompleted()), q, SLOT(_q_updateStyle()));

    q->setOptimizationFlag(QGraphicsView::DontSavePainterState);

#ifdef Q_WS_X11
    if ( !MComponentCache::populating() &&
         !MComponentData::chainDataStackIsEmpty() )
    {
        // init for chainTask here
        updateChainTaskData();
    }

    appendMApplicationWindowTypeProperty();
#endif

#ifdef M_OS_MAEMO5
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(_q_exitAppView()));
#else
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(closeMenu()));
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(showMinimized()));
#endif //M_OS_MAEMO5

    q->connect(navigationBar, SIGNAL(viewmenuTriggered()),
               q, SLOT(openMenu()));

    q->connect(navigationBar, SIGNAL(sceneWindowStateChanged(MSceneWindow::SceneWindowState, MSceneWindow::SceneWindowState)),
               q, SLOT(_q_navigationBarStateChanged(MSceneWindow::SceneWindowState, MSceneWindow::SceneWindowState)));

    q->connect(menu, SIGNAL(appeared()),
               q, SLOT(_q_menuAppeared()));
    q->connect(menu, SIGNAL(disappeared()),
               q, SLOT(_q_menuDisappeared()));
    q->connect(q, SIGNAL(switcherEntered()),
               SLOT(_q_handleInSwitcherVisibilityChange()));

    // We wanna do it later since this navigation bar emptiness might be a momentary state
    // during the setup of a page switch.
    // E.g. The navigation bar gets empty by the detachment of the current page but when the
    // new page is finally connected the nav bar might get populated by a
    // toolbar, menu button, etc and therefore it won't be empty anymore.
    //
    // So doing it later ensures that the navigation bar visibility is updated only once
    // per page switch.
    MDynamicPropertyWatcher *navigationBarIsEmptyPropertyWatcher = new MDynamicPropertyWatcher(q);
    navigationBarIsEmptyPropertyWatcher->watch(navigationBar);
    navigationBarIsEmptyPropertyWatcher->setPropertyName("isEmpty");
    q->connect(navigationBarIsEmptyPropertyWatcher, SIGNAL(propertyChanged()),
               SLOT(_q_scheduleNavigationBarVisibilityUpdate()));

    updateStyleNames();

    // Create the view to get the "isEmpty" property updates.
    navigationBar->view();
    navigationBar->setVisible(false);
    sceneManager->addSceneWindow(navigationBar);

    sceneManager->appearSceneWindowNow(homeButtonPanel);

    // Initialize escape button to close mode.
    navigationBar->setEscapeButtonMode(MNavigationBarModel::EscapeButtonClose);
    QObject::connect(navigationBar, SIGNAL(closeButtonClicked()), q, SLOT(close()));

    if (needsDockWidget()) {
        sceneManager->appearSceneWindowNow(dockWidget);
    }

    if (!MApplication::fullScreen() && statusBar) {
        sceneManager->appearSceneWindowNow(statusBar);
    }

    _q_placeToolBar();

    // Unlike plain MWindows, the full-fledged application window have rounded corners
    // enabled by default.
    q->setRoundedCornersEnabled(true);

    applyWindowBackground();

    initAutoHideComponentsTimer();

#ifdef HAVE_CONTEXTSUBSCRIBER
    q->connect(&callStatusProperty, SIGNAL(valueChanged()), SLOT(_q_updateStatusBarVisibility()));
#endif
    q->connect(q, SIGNAL(orientationAngleChanged(M::OrientationAngle)),
            SLOT(_q_updatePageExposedContentRect()));
    q->connect(MTheme::instance(), SIGNAL(themeChangeCompleted()),
               SLOT(_q_updatePageExposedContentRect()));

    if (statusBar) {
        q->connect(statusBar, SIGNAL(appeared()), SLOT(_q_updatePageExposedContentRect()));
        q->connect(statusBar, SIGNAL(disappeared()), SLOT(_q_updatePageExposedContentRect()));
    }

    navigationBarVisibilityUpdateTimer.setSingleShot(true);
    navigationBarVisibilityUpdateTimer.setInterval(0);
    q->connect(&navigationBarVisibilityUpdateTimer, SIGNAL(timeout()),
               SLOT(_q_updateNavigationBarVisibility()));
}

void MApplicationWindowPrivate::_q_handleInSwitcherVisibilityChange()
{
}

#ifdef Q_WS_X11
void MApplicationWindowPrivate::setWindowChainedProperty( const Window &parentWinId, const Window &childWinId )
{
    Atom atomMInvokedBy = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_WM_INVOKED_BY", False);
    Display *display = QX11Info::display();

    // for compositor page transition
    XChangeProperty(display, childWinId,
            atomMInvokedBy, XA_WINDOW,
            32, PropModeReplace,
            (unsigned char *)&parentWinId, 1);

    // for task switcher view stacking
    XSetTransientForHint(display, childWinId, parentWinId);
}

void MApplicationWindowPrivate::appendMApplicationWindowTypeProperty()
{
    Q_Q(MApplicationWindow);

    Atom atomWindowType = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_NET_WM_WINDOW_TYPE_MAPPLICATION", False);
    XChangeProperty(QX11Info::display(), q->effectiveWinId(),
                    XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False),
                    XA_ATOM, 32, PropModeAppend, (unsigned char*) &atomWindowType, 1);
}
#endif

void  MApplicationWindowPrivate::initAutoHideComponentsTimer()
{
    autoHideComponentsTimer.setInterval(style()->autoHideTimeout());
    autoHideComponentsTimer.setSingleShot(true);
}

void MApplicationWindowPrivate::windowStateChangeEvent(QWindowStateChangeEvent *event)
{
    Q_Q(MApplicationWindow);
    if (!statusBar)
        return;

    if (q->isFullScreen() != event->oldState().testFlag(Qt::WindowFullScreen)) {
        _q_updateStatusBarVisibility();
    }
}

void MApplicationWindowPrivate::maximizePageArea()
{
    pageAreaMaximized = true;

    // When maximized, the window is in control of these components.
    setComponentDisplayMode(homeButtonPanel, MApplicationPageModel::Hide);

    if (navigationBar->focusItem()) {
        // Always show focused navigation bar.
        setComponentDisplayMode(navigationBar, MApplicationPageModel::Show);
    } else {
        setComponentDisplayMode(navigationBar, MApplicationPageModel::Hide);
    }

    if (needsDockWidget()) {
        if (dockWidget->focusItem()) {
            // Always show focused dock widget.
            setComponentDisplayMode(dockWidget, MApplicationPageModel::Show);
        } else {
            setComponentDisplayMode(dockWidget, MApplicationPageModel::Hide);
        }
    }


    // Now that we've set pageAreaMaximized, this update tries to hide status bar.
    _q_updateStatusBarVisibility();
}

void MApplicationWindowPrivate::restorePageArea()
{
    if (!pageAreaMaximized) {
        return;
    }

    pageAreaMaximized = false;

    if (page) {
        setComponentDisplayMode(homeButtonPanel, page->model()->homeButtonDisplayMode());
        setComponentDisplayMode(navigationBar, page->model()->navigationBarDisplayMode());

        if (needsDockWidget()) {
            // Dock widget follows navigation bar display mode.
            setComponentDisplayMode(dockWidget, page->model()->navigationBarDisplayMode());
        }
    }

    // Show status bar
    _q_updateStatusBarVisibility();
}

void MApplicationWindowPrivate::_q_updateStatusBarVisibility()
{
    Q_Q(MApplicationWindow);
    if (!statusBar)
        return;

#ifdef HAVE_CONTEXTSUBSCRIBER
    // Status bar should always be appeared while a phone call is ongoing.
    if (callStatusProperty.value().toString() == "active") {
        statusBar->appear(q);
        return;
    }
#endif

    if (q->isFullScreen() || pageAreaMaximized) {
        statusBar->disappear();
    } else {
        statusBar->appear(q);
    }
}

void MApplicationWindowPrivate::_q_actionUpdated(QActionEvent *event)
{
    QAction *action = event->action();

    if (event->type() == QEvent::ActionAdded) {
        QAction *before = event->before();
        distributeAction(action, before);
    } else if (event->type() == QEvent::ActionRemoved) {
        menu->removeAction(action);
        toolBar->removeAction(action);
    }
    refreshArrowIconVisibility();

    if (page) {
        // Dock widget follows navigation bar display mode.
        setComponentDisplayMode(dockWidget,
                                page->model()->navigationBarDisplayMode());
    }
}

void MApplicationWindowPrivate::_q_handlePageModelModifications(const QList<const char *> &modifications)
{
    Q_Q(MApplicationWindow);
    const char *member;

    foreach(member, modifications) {
        if (!pageAreaMaximized) {
            if (member == MApplicationPageModel::HomeButtonDisplayMode) {
                setComponentDisplayMode(homeButtonPanel,
                                        page->model()->homeButtonDisplayMode());

            } else if (member == MApplicationPageModel::NavigationBarDisplayMode) {
                if (page->model()->navigationBarDisplayMode() == MApplicationPageModel::AutoHide)
                    addComponentToAutoHide(navigationBar);
                else
                    removeComponentFromAutoHide(navigationBar);

                _q_scheduleNavigationBarVisibilityUpdate();

                // Dock widget follows navigation bar display mode.
                setComponentDisplayMode(dockWidget,
                                        page->model()->navigationBarDisplayMode());

                // Display mode can be changed between "auto-hide" and "show" which does not
                // affect appearance state. Need to update exposed rectangle explicitly.
                _q_updatePageExposedContentRect();
            } else if (member == MApplicationPageModel::EscapeButtonDisplayMode) {
                _q_scheduleNavigationBarVisibilityUpdate();
            }
        }

        if (member == MApplicationPageModel::ProgressIndicatorVisible) {
            navigationBar->setProgressIndicatorVisible(page->model()->progressIndicatorVisible());

        } else if (member == MApplicationPageModel::EscapeMode) {
            setupPageEscape();

        } else if (member == MApplicationPageModel::Title) {
            if (page->isVisible()) {
                navigationBar->setViewMenuDescription(page->model()->title());
                q->setWindowTitle(longestLengthVariant(page->model()->title()));
            }
        }
    }
}

void MApplicationWindowPrivate::_q_placeToolBar()
{
    // set position of toolbar
    if (needsDockWidget()) {
        // take toolbar from navigation bar
        navigationBar->undockToolBar();
        // dock it in lower dock widget
        dockWidget->dockToolBar(toolBar);
        // show dockwidget
        if (page) {
            // Dock widget follows navigation bar display mode.
            setComponentDisplayMode(dockWidget,
                                    page->model()->navigationBarDisplayMode());
        }
    } else {
        // hide dockwidget
        dockWidget->disappear();
        // undock toolbar
        dockWidget->undockToolBar();
        // redock to navigation bar
        navigationBar->dockToolBar(toolBar);
    }
}

void MApplicationWindowPrivate::_q_updatePageExposedContentRect()
{
    if (!page) {
        return;
    }

    qreal topCoverage = 0.0f;
    qreal bottomCoverage = 0.0f;

    QRectF pageExposedContentRect;

    // Interpretation of whether the navigation bar is covering the page
    // changes from "auto-hide" to "show". On "auto-hide" the navigation
    // bar doesn't count as covering the page since it does so only momentarily.
    const bool navBarIsCovering = showingNavigationBar &&
            page->componentDisplayMode(MApplicationPage::NavigationBar) != MApplicationPageModel::AutoHide;

    if (navBarIsCovering) {
        if (navigationBar->alignment().testFlag(Qt::AlignTop))
            topCoverage += navigationBar->size().height();
        else if (navigationBar->alignment().testFlag(Qt::AlignBottom))
            bottomCoverage += navigationBar->size().height();
    }


    if (showingDockWidget) {
        if (dockWidget->alignment().testFlag(Qt::AlignTop))
            topCoverage += dockWidget->size().height();
        else if (dockWidget->alignment().testFlag(Qt::AlignBottom))
            bottomCoverage += dockWidget->size().height();
    }

    pageExposedContentRect.setY(topCoverage);
    pageExposedContentRect.setWidth(page->geometry().width());
    pageExposedContentRect.setHeight(page->geometry().height()
                              - pageExposedContentRect.y() - bottomCoverage);

    page->d_func()->setExposedContentRect(pageExposedContentRect);
}

void MApplicationWindowPrivate::openMenu()
{
    Q_Q(MApplicationWindow);
    if (navigationBar->isArrowIconVisible() &&
        (navigationBar->sceneWindowState() != MSceneWindow::Disappearing)) {
        menu->appear(q);
        navigationBar->setEscapeButtonEnabled(false);

        // Simply calling setEnabled(false) would uncheck the currently
        // checked item in the toolbar if tab view is being used.
        toolBar->setProperty(_M_IsEnabledPreservingSelection, QVariant(false));
    }
}

void MApplicationWindowPrivate::closeMenu()
{
    if (menu->sceneWindowState() == MSceneWindow::Appeared || menu->sceneWindowState() == MSceneWindow::Appearing)
        menu->disappear();
}

bool MApplicationWindowPrivate::isMenuOpen() const
{
    return (menu->sceneWindowState() == MSceneWindow::Appeared);
}

void MApplicationWindowPrivate::_q_menuAppeared()
{
    Q_Q(MApplicationWindow);
    QObject::disconnect(navigationBar, SIGNAL(viewmenuTriggered()),
                        q, SLOT(openMenu()));
    QObject::connect(navigationBar, SIGNAL(viewmenuTriggered()),
                     q, SLOT(closeMenu()));
    autoHideComponentsTimer.stop();
}

void MApplicationWindowPrivate::_q_menuDisappeared()
{
    Q_Q(MApplicationWindow);
    QObject::disconnect(navigationBar, SIGNAL(viewmenuTriggered()),
                        q, SLOT(closeMenu()));
    QObject::connect(navigationBar, SIGNAL(viewmenuTriggered()),
                     q, SLOT(openMenu()));

    navigationBar->setEscapeButtonEnabled(true);
    toolBar->setProperty(_M_IsEnabledPreservingSelection, QVariant(true));

    if (!componentsOnAutoHide.isEmpty() && !autoHideComponentsTimer.isActive())
        autoHideComponentsTimer.start();
}

void MApplicationWindowPrivate::_q_navigationBarStateChanged(
        MSceneWindow::SceneWindowState newState, MSceneWindow::SceneWindowState oldState)
{
    // Check for an animated or immediate disappearance.
    if (newState == MSceneWindow::Disappearing ||
            (newState == MSceneWindow::Disappeared && oldState == MSceneWindow::Appeared)) {
        closeMenu();
    }
}

#ifdef M_OS_MAEMO5
void MApplicationWindowPrivate::_q_exitAppView()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createSignal("/", "com.nokia.hildon_desktop", "exit_app_view");
    bus.send(message);
}
#endif //M_OS_MAEMO5

void MApplicationWindowPrivate::manageActions()
{
    Q_Q(MApplicationWindow);

    QAction* checkedAction = findCheckedAction(page->actions());
    if (!checkedAction) {
        checkedAction = findCheckedAction(q->actions());
    }

    bool pageHasToolbarActions = false;

    foreach(QAction* qaction, page->actions()) {
        MAction* maction = qobject_cast<MAction*>(qaction);
        if (!maction || (maction->location() & MAction::ToolBarLocation)) {
            pageHasToolbarActions = true;
            break;
        }
    }

    if (pageHasToolbarActions) {
        toolBar->clearActions();

        // add page actions
        foreach(QAction* action, page->actions()) {
            toolBar->insertAction(NULL, action);
        }

        // add window actions
        foreach(QAction* action, q->actions()) {
            toolBar->insertAction(NULL, action);
        }
    }

    menu->clearActions();

    // add page actions
    foreach(QAction* action, page->actions()) {
        menu->insertAction(NULL, action);
    }

    // add window actions
    foreach(QAction* action, q->actions()) {
        menu->insertAction(NULL, action);
    }

    if (checkedAction) {
        checkedAction->setChecked(true);
    }

    refreshArrowIconVisibility();
}

void MApplicationWindowPrivate::distributeAction(QAction *action, QAction *before)
{
    //The toolbar and menu will check for themselves whether to actually show the action or not
    toolBar->insertAction(before, action);
    menu->insertAction(before, action);
}

void MApplicationWindowPrivate::refreshArrowIconVisibility()
{
    bool haveVisibleMenuAction = false;
    foreach(QAction *action, menu->actions()) {
        if(action->isVisible()) {
            MAction *maction = qobject_cast<MAction*>(action);
            if(!maction || maction->location().testFlag(MAction::ApplicationMenuLocation)) {
                haveVisibleMenuAction = true;
                break;
            }
        }
    }
    navigationBar->setArrowIconVisible(haveVisibleMenuAction);
}

void MApplicationWindowPrivate::setComponentDisplayMode(
    MSceneWindow *component, MApplicationPageModel::ComponentDisplayMode displayMode)
{
    switch (displayMode) {
    case MApplicationPageModel::Show:
        removeComponentFromAutoHide(component);

        if (component == dockWidget) {
            updateDockWidgetVisibility();
        } else if (component == navigationBar) {
            _q_updateNavigationBarVisibility();
        } else {
            setSceneWindowVisibility(component, true);
        }
        break;

    case MApplicationPageModel::AutoHide:
        addComponentToAutoHide(component);

        // Visibility in sync with other components in auto-hide mode.
        if (component == dockWidget && autoHideComponentsTimer.isActive()) {
            updateDockWidgetVisibility();
        } else if (component == navigationBar) {
            _q_updateNavigationBarVisibility();
        } else {
            setSceneWindowVisibility(component, autoHideComponentsTimer.isActive());
        }
        break;

    case MApplicationPageModel::Hide:
        removeComponentFromAutoHide(component);

        setSceneWindowVisibility(component, false);
        break;
    };

    if (componentsOnAutoHide.count() == 0) {
        autoHideComponentsTimer.stop();
    }
}

void MApplicationWindowPrivate::addComponentToAutoHide(MSceneWindow *component)
{
    if (!componentsOnAutoHide.contains(component)) {
        component->connect(&autoHideComponentsTimer, SIGNAL(timeout()),
                           SLOT(disappear()));

        componentsOnAutoHide << component;
    }
}

void MApplicationWindowPrivate::removeComponentFromAutoHide(MSceneWindow *component)
{
    if (componentsOnAutoHide.contains(component)) {
        autoHideComponentsTimer.disconnect(SIGNAL(timeout()),
                                           component, SLOT(disappear()));

        componentsOnAutoHide.removeAll(component);
    }
}

void MApplicationWindowPrivate::updateDockWidgetVisibility()
{
    // Make dock widget visible only if a toolbar is docked and
    // the docked toolbar has some actions

    if (!dockWidget->toolBar() || !needsDockWidget()) {
        return;
    }

    QList<QAction *> actions = toolBar->actions();
    int count = actions.count();
    bool toolbarHasVisibleActions = false;
    for (int i = 0; i < count; ++i) {
        if (actions[i]->isVisible()) {
            MAction *action = qobject_cast<MAction*>(actions[i]);
            if(!action || action->location().testFlag(MAction::ToolBarPortraitLocation)) {
                toolbarHasVisibleActions = true;
                break;
            }
        }
    }

    setSceneWindowVisibility(dockWidget, toolbarHasVisibleActions);
}

void MApplicationWindowPrivate::_q_updateNavigationBarVisibility()
{
    // remove any pending update since we're doing it right now.
    if (navigationBarVisibilityUpdateTimer.isActive()) {
        navigationBarVisibilityUpdateTimer.stop();
    }

    bool navigationBarIsEmpty = navigationBar->property("isEmpty").toBool();

    bool navigationBarHasJustEscapeButton = navigationBar->property("justEscapeButton").toBool();

    bool navigationBarVisible = !page || (page->model()->navigationBarDisplayMode() == MApplicationPageModel::Show
                                          || (page->model()->navigationBarDisplayMode() == MApplicationPageModel::AutoHide
                                              && autoHideComponentsTimer.isActive()));

    bool escapeButtonVisible = !page || page->model()->escapeButtonDisplayMode() != MApplicationPageModel::Hide;

    if (navigationBarHasJustEscapeButton)
        setSceneWindowVisibility(navigationBar, navigationBarVisible && escapeButtonVisible);
    else
        setSceneWindowVisibility(navigationBar, navigationBarVisible && !navigationBarIsEmpty);

    if (escapeButtonVisible || navigationBar->sceneWindowState() != MSceneWindow::Disappearing) {
        navigationBar->setEscapeButtonVisible(escapeButtonVisible);
    }
}

void MApplicationWindowPrivate::_q_scheduleNavigationBarVisibilityUpdate()
{
    if (!navigationBarVisibilityUpdateTimer.isActive()) {
        navigationBarVisibilityUpdateTimer.start();
    }
}

void MApplicationWindowPrivate::sceneWindowAppearEvent(MSceneWindowEvent *event)
{
    // Note that, when listening scene window state changed events, the actual state
    // of the scene window is not yet changed, and is needed to store separately
    // before call to _q_updatePageExposedContentRect().

    MSceneWindow *sceneWindow = event->sceneWindow();

    switch (sceneWindow->windowType()) {
        case MSceneWindow::ApplicationPage:
            {
                applicationPageAppearEvent(event);
            }
            break;

        case MSceneWindow::NavigationBar:
            showingNavigationBar = true;
            _q_updatePageExposedContentRect();
            break;

        case MSceneWindow::DockWidget:
            showingDockWidget = true;
            _q_updatePageExposedContentRect();
            break;

        default:
            break;
    }

}

void MApplicationWindowPrivate::sceneWindowDisappearEvent(MSceneWindowEvent *event)
{
    MSceneWindow *sceneWindow = event->sceneWindow();

    switch (sceneWindow->windowType()) {
        case MSceneWindow::ApplicationPage:
            applicationPageDisappearEvent(event);
            break;

        case MSceneWindow::NavigationBar:
            showingNavigationBar = false;
            _q_updatePageExposedContentRect();
            break;

        case MSceneWindow::DockWidget:
            showingDockWidget = false;
            _q_updatePageExposedContentRect();
            break;

        default:
            break;
    }
}

void MApplicationWindowPrivate::sceneWindowDismissEvent(MSceneWindowEvent *event)
{
    // Currently we handle it in the same way as for a regular disappearance.
    sceneWindowDisappearEvent(event);
}

void MApplicationWindowPrivate::applicationPageAppearEvent(MSceneWindowEvent *event)
{
    MApplicationPage *pageFromEvent = static_cast<MApplicationPage *>(event->sceneWindow());

    // It cannot be the current page
    Q_ASSERT(pageFromEvent != page);

    if (page != 0) {
        if (menu->isVisible())
            menu->disappear();
        disconnectPage(page);
    }

    animateComponentsTransitions = event->animatedTransition();
    connectPage(pageFromEvent);
    animateComponentsTransitions = true;
    _q_updatePageExposedContentRect();

#ifdef Q_WS_X11
    if (pageFromEvent && isChained && sceneManager) {
        bool isFirstPage = sceneManager->pageHistory().isEmpty();
        if (isFirstPage) {
            // for title in task switcher
            page->setTitle(chainTaskTitle);
        }
    }
#endif
}

void MApplicationWindowPrivate::applicationPageDisappearEvent(MSceneWindowEvent *event)
{
    MApplicationPage *pageFromEvent = static_cast<MApplicationPage *>(event->sceneWindow());

    // Page is going away. Let's disconnect it if it's the current page.
    if (pageFromEvent == page) {
        if (menu->isVisible())
            menu->disappear();
        disconnectPage(pageFromEvent);
    }
}

void MApplicationWindowPrivate::setupPageEscape()
{
    bool setupDone = false;

    if (!page) {
        // Nothing to be done.
        return;
    }

    // Tear down any previous page escape setup.
    tearDownPageEscape();

#ifdef Q_WS_X11
    setupDone = setupPageEscapeChainedApplication();
#endif

    if (!setupDone) {
        switch (page->escapeMode()) {
            case MApplicationPageModel::EscapeAuto:
                setupPageEscapeAuto();
                break;

            case MApplicationPageModel::EscapeManualBack:
                setupPageEscapeBack();
                break;

            case MApplicationPageModel::EscapeCloseWindow:
                setupPageEscapeClose();
                break;

            default:
                qFatal("MApplicationWindow: Invalid page escape mode");
        };

        setupDone = true;
    }

    Q_ASSERT(setupDone);
}

#ifdef Q_WS_X11
bool MApplicationWindowPrivate::setupPageEscapeChainedApplication()
{
    Q_Q(MApplicationWindow);
    Q_ASSERT(page);

    bool setupDone = false;

    // The root page of a chained application has a special escape setup

    if (isChained && sceneManager) {
        bool isFirstPage = sceneManager->pageHistory().isEmpty();
        if (isFirstPage) {
            setupPageEscapeBack();
            q->connect(page, SIGNAL(backButtonClicked()), SLOT(close()));
            setupDone = true;
        }
    }

    return setupDone;
}
#endif

void MApplicationWindowPrivate::setupPageEscapeAuto()
{
    Q_Q(MApplicationWindow);
    QList<MSceneWindow*> pageHistory = q->sceneManager()->pageHistory();

    if (pageHistory.isEmpty()) {
        setupPageEscapeClose();
    } else {
        setupPageEscapeBack();
        page->connect(page, SIGNAL(backButtonClicked()), SLOT(dismiss()));
    }

    // We must update the wiring of our escape button if the application manually
    // changes the page history while a page in EscapeAuto mode is being displayed.
    q->connect(q->sceneManager(), SIGNAL(pageHistoryChanged()), SLOT(_q_updatePageEscapeAuto()));
}

void MApplicationWindowPrivate::setupPageEscapeBack()
{
    navigationBar->setEscapeButtonMode(MNavigationBarModel::EscapeButtonBack);
}

void MApplicationWindowPrivate::setupPageEscapeClose()
{
    navigationBar->setEscapeButtonMode(MNavigationBarModel::EscapeButtonClose);
}

void MApplicationWindowPrivate::tearDownPageEscape()
{
    Q_Q(MApplicationWindow);

    QObject::disconnect(q->sceneManager(), SIGNAL(pageHistoryChanged()),
            q, SLOT(_q_updatePageEscapeAuto()));

    QObject::disconnect(page, SIGNAL(backButtonClicked()), page, SLOT(dismiss()));
}

void MApplicationWindowPrivate::_q_updatePageEscapeAuto()
{
    Q_Q(MApplicationWindow);
    QList<MSceneWindow*> pageHistory = q->sceneManager()->pageHistory();

    Q_ASSERT(page != 0);
    Q_ASSERT(page->escapeMode() == MApplicationPageModel::EscapeAuto);

    if (pageHistory.isEmpty() &&
            (navigationBar->escapeButtonMode() != MNavigationBarModel::EscapeButtonClose)) {

        QObject::disconnect(page, SIGNAL(backButtonClicked()), page, SLOT(dismiss()));
        // we don't want MEscapeButtonPanel::escapeModeChanged() to be intercepted
        // by the scene manager which would update close button geometry.
        // The geometry will be updated later on in connectPage().
        navigationBar->blockSignals(true);
        setupPageEscapeClose();
        navigationBar->blockSignals(false);

    } else if (!pageHistory.isEmpty() &&
            (navigationBar->escapeButtonMode() != MNavigationBarModel::EscapeButtonBack)) {

        setupPageEscapeBack();
        page->connect(page, SIGNAL(backButtonClicked()), SLOT(dismiss()));
    }
}

bool MApplicationWindowPrivate::needsDockWidget()
{
    return ((toolBar->viewType() == MToolBar::defaultType && style()->floatableToolBar())
            || (toolBar->viewType() == MToolBar::tabType && style()->floatableTabBar()));
}

void MApplicationWindowPrivate::setToolBarViewType(const MTheme::ViewType& viewType)
{
    toolBar->setViewType(viewType);
    updateStyleNames();
    _q_placeToolBar();
}


MApplicationWindowStyleContainer& MApplicationWindowPrivate::style()
{
    Q_Q(MApplicationWindow);
    if (!styleContainer) {
        styleContainer = createStyleContainer();
        styleContainer->initialize(q->objectName(), "", NULL);
    }
    return *styleContainer;
}

const MApplicationWindowStyleContainer& MApplicationWindowPrivate::style() const
{
    return const_cast<MApplicationWindowPrivate*>(this)->style();
}

MApplicationWindowStyleContainer* MApplicationWindowPrivate::createStyleContainer() const
{
    return new MApplicationWindowStyleContainer();
}

void MApplicationWindowPrivate::_q_updateStyle()
{
    style().updateCurrentStyle();
    updateStyleNames();
    _q_placeToolBar();
    initAutoHideComponentsTimer();
    applyWindowBackground();
}

void MApplicationWindow::reinit()
{
    updateChainTaskData();
}

void MApplicationWindow::updateChainTaskData()
{
    Q_D(MApplicationWindow);
    d->updateChainTaskData();
}

void MApplicationWindowPrivate::updateChainTaskData()
{
#ifdef Q_WS_X11
    // here we try to inject the chainTaskData into this MApplicationWindow
    Q_Q(MApplicationWindow);

    if (!MComponentData::chainDataStackIsEmpty()) {
        MComponentData::ChainData thisData = MComponentData::popChainData();

        WId     thisDataWId       = thisData.first;
        QString thisDataTaskTitle = thisData.second;

        chainParentWinId = thisDataWId;
        bool taskTitleSet = !thisDataTaskTitle.isEmpty();
        if (taskTitleSet) {
            chainTaskTitle = thisDataTaskTitle;
        } else {
            chainTaskTitle = MComponentData::appName();
        }

        isChained = true;

        // for compositor page animation
        setWindowChainedProperty( chainParentWinId, q->effectiveWinId() );
    }
#endif
}

void MApplicationWindowPrivate::applyWindowBackground()
{
    Q_Q(MApplicationWindow);

    if (style()->backgroundColor().isValid())
        q->setBackgroundBrush(style()->backgroundColor());
    else
        q->setBackgroundBrush(Qt::black);
}

void MApplicationWindowPrivate::updateStyleNames()
{
    navigationBar->setStyleName(style()->navigationBarStyleName());
    if (toolBar->viewType() == MToolBar::tabType)
        toolBar->setStyleName(style()->tabBarStyleName());
    else
        toolBar->setStyleName(style()->toolBarStyleName());
    dockWidget->setStyleName(toolBar->styleName()); //dock widget will have the same style name as tool bar
    if (statusBar)
        statusBar->setStyleName(style()->statusBarStyleName());
}


MApplicationWindow::MApplicationWindow(MApplicationWindowPrivate &dd, QWidget *parent)
    : MWindow(dd, new MSceneManager, parent)
{
    Q_D(MApplicationWindow);

    d->init();

    MComponentData::registerWindow(this);
}

MApplicationWindow::MApplicationWindow(MApplicationWindowPrivate &dd, MScene *scene, QWidget *parent)
    : MWindow(dd, scene, parent)
{
    Q_D(MApplicationWindow);

    d->init();

    MComponentData::registerWindow(this);
}

MApplicationWindow::MApplicationWindow(QWidget *parent)
    : MWindow(*new MApplicationWindowPrivate, new MSceneManager, parent)
{
    Q_D(MApplicationWindow);

    d->init();

    MComponentData::registerWindow(this);
}

MApplicationWindow::MApplicationWindow(MScene *scene, QWidget *parent)
    : MWindow(*new MApplicationWindowPrivate, scene, parent)
{
    Q_D(MApplicationWindow);

    d->init();

    MComponentData::registerWindow(this);
}

MApplicationWindow::~MApplicationWindow()
{
    MComponentData::unregisterWindow(this);
}

MApplicationPage *MApplicationWindow::currentPage() const
{
    Q_D(const MApplicationWindow);
    return d->page;
}

bool MApplicationWindow::event(QEvent *event)
{
    Q_D(MApplicationWindow);
    switch (event->type()) {
        case QEvent::ActionRemoved: {
            QActionEvent *actionEvent = static_cast<QActionEvent *>(event);
            QAction *action = actionEvent->action();
            if (action) {
                action->disconnect(this);
            }
            //fall through is intentional
        }

        case QEvent::ActionChanged:
        case QEvent::ActionAdded: {
            QActionEvent *actionEvent = static_cast<QActionEvent *>(event);
            d->_q_actionUpdated(actionEvent);
            return true;
        }

        case QEvent::WindowStateChange:
            d->windowStateChangeEvent(static_cast<QWindowStateChangeEvent *>(event));
            break;

        default:
            // Do nothing
            break;
    }

    return MWindow::event(event);
}

// We have to send this root message because the window itself is managing
// its close events. In directUi, there's no WM with a close button
void MApplicationWindow::closeEvent(QCloseEvent *event)
{
    // Don't really close if lazy shutdown used.
    if (!closeOnLazyShutdown()) {
        if (MApplication::prestartMode() == M::LazyShutdownMultiWindow ||
            MApplication::prestartMode() == M::LazyShutdown) {
            closeMenu();
            event->accept();
            return;
        }
    }

    event->accept();
}

void MApplicationWindow::openMenu()
{
    Q_D(MApplicationWindow);
    d->openMenu();
}

void MApplicationWindow::closeMenu()
{
    Q_D(MApplicationWindow);
    d->closeMenu();
}

void MApplicationWindow::setWindowIconID(const QString &windowIconID)
{
    Q_D(MApplicationWindow);
    d->navigationBar->setViewMenuIconID(windowIconID);
}

void MApplicationWindow::setToolbarViewType(const MTheme::ViewType& viewType)
{
    Q_D(MApplicationWindow);
    d->setToolBarViewType(viewType);
}

void MApplicationWindowPrivate::connectPage(MApplicationPage *newPage)
{
    Q_Q(MApplicationWindow);

    Q_ASSERT(newPage != 0);
    page = newPage;

    q->connect(page, SIGNAL(actionUpdated(QActionEvent *)), SLOT(_q_actionUpdated(QActionEvent *)));

    manageActions();

    q->connect(page->model(), SIGNAL(modified(QList<const char *>)),
               SLOT(_q_handlePageModelModifications(QList<const char *>)));

    navigationBar->setViewMenuDescription(page->title());
    q->setWindowTitle(longestLengthVariant(page->title()));

    setupPageEscape();

    page->connect(navigationBar, SIGNAL(backButtonClicked()), SIGNAL(backButtonClicked()));
    page->connect(navigationBar, SIGNAL(closeButtonClicked()), SIGNAL(closeButtonClicked()));

    navigationBar->setProgressIndicatorVisible(page->model()->progressIndicatorVisible());

    _q_setupNavigationBarCustomContent();
    q->connect(page, SIGNAL(customNavigationBarContentChanged()),
               SLOT(_q_setupNavigationBarCustomContent()));

    setComponentDisplayMode(homeButtonPanel, page->model()->homeButtonDisplayMode());
    setComponentDisplayMode(navigationBar, page->model()->navigationBarDisplayMode());
    // escapeButtonDisplayMode is handled along with navigationBarDisplayMode

    // Dock widget follows navigation bar display mode.
    setComponentDisplayMode(dockWidget, page->model()->navigationBarDisplayMode());

    emit q->pageChanged(page);
}

void MApplicationWindowPrivate::disconnectPage(MApplicationPage *pageToDisconnect)
{
    Q_UNUSED(pageToDisconnect)
    Q_Q(MApplicationWindow);

    Q_ASSERT(pageToDisconnect == page);

    QObject::disconnect(page, SIGNAL(actionUpdated(QActionEvent *)),
                        q, SLOT(_q_actionUpdated(QActionEvent *)));

    QObject::disconnect(page->model(), SIGNAL(modified(QList<const char *>)),
                        q, SLOT(_q_handlePageModelModifications(QList<const char *>)));

    QObject::disconnect(page, SIGNAL(customNavigationBarContentChanged()),
                        q, SLOT(_q_setupNavigationBarCustomContent()));

    tearDownPageEscape();
    QObject::disconnect(navigationBar, SIGNAL(backButtonClicked()), page, 0);
    QObject::disconnect(navigationBar, SIGNAL(closeButtonClicked()), page, 0);

    removePageActions();

    navigationBar->setCustomContent(0);

    page = 0;
}

void MApplicationWindowPrivate::removePageActions()
{
    QAction* checkedAction = findCheckedAction(page->actions());

    // remove all the page actions
    QList<QAction *> actions = page->actions();
    int actionsSize = actions.size();
    for (int i = 0; i < actionsSize; ++i) {
        QAction* action = actions[i];
        QAction* toolbarAction = 0;
        if (toolBar->viewType() == MToolBar::tabType &&
            toolBar->actions().contains(action)) {
            toolbarAction = action;
        }
        toolBar->removeAction(action);
        menu->removeAction(action);
        if (toolbarAction)
            toolbarAction->setChecked(false);
    }

    // restore the checked action
    if (checkedAction) {
        checkedAction->setChecked(true);
    }
}

QAction* MApplicationWindowPrivate::findCheckedAction(const QList<QAction *> &actions) const
{
    QAction* checkedAction = 0;
    int actionsSize = actions.size();
    if (toolBar->viewType() == MToolBar::tabType) {
        for (int i = 0; i < actionsSize; ++i) {
            QAction* action = actions[i];
            MAction* mAction = qobject_cast<MAction*>(action);
            bool isToolbarAction = (!mAction || mAction->location().testFlag(MAction::ToolBarPortraitLocation) ||
                             mAction->location().testFlag(MAction::ToolBarLandscapeLocation));
            if (action->isChecked() && isToolbarAction) {
                checkedAction = action;
                break;
            }
        }
    }
    return checkedAction;
}

void MApplicationWindowPrivate::_q_setupNavigationBarCustomContent()
{
    Q_ASSERT(page != 0);

    // Make sure the page has already created its content.
    // It can perfectly be creating its navigation bar custom content from
    // within MApplicationPage::createContent()
    if (!page->d_func()->contentCreated) {
        page->createContent();
        page->d_func()->contentCreated = true;
    }

    navigationBar->setCustomContent(page->customNavigationBarContent());
}

void MApplicationWindowPrivate::setSceneWindowVisibility(MSceneWindow* sceneWindow, bool visible)
{
    if (visible && animateComponentsTransitions)
        sceneManager->appearSceneWindow(sceneWindow);
    else if (visible && !animateComponentsTransitions)
        sceneManager->appearSceneWindowNow(sceneWindow);
    else if (!visible && animateComponentsTransitions)
        sceneManager->disappearSceneWindow(sceneWindow);
    else if (!visible && !animateComponentsTransitions)
        sceneManager->disappearSceneWindowNow(sceneWindow);
}

QString MApplicationWindow::windowIconID() const
{
    Q_D(const MApplicationWindow);
    return d->navigationBar->viewMenuIconID();
}

MTheme::ViewType MApplicationWindow::toolbarViewType() const
{
    Q_D(const MApplicationWindow);
    return d->toolBar->viewType();
}

bool MApplicationWindow::isMenuOpen() const
{
    Q_D(const MApplicationWindow);
    return d->isMenuOpen();
}

#ifdef Q_WS_X11
WId MApplicationWindow::chainParentWinId() const
{
    Q_D(const MApplicationWindow);
    return d->chainParentWinId;
}

bool MApplicationWindow::isChained() const
{
    Q_D(const MApplicationWindow);
    return d->isChained;
}
#endif

void MApplicationWindow::mousePressEvent(QMouseEvent *event)
{
    Q_D(MApplicationWindow);

    //check if the navigation or toolbar is pressed
    if (d->navigationBar->boundingRect().contains(d->navigationBar->mapFromScene(event->pos().x(), event->pos().y()))
        || d->toolBar->boundingRect().contains(d->toolBar->mapFromScene(event->pos().x(), event->pos().y()))) {
        d->navigationBarPressed = true;
    }   else
        d->navigationBarPressed = false;

    if (d->autoHideComponentsTimer.isActive()) {
        d->autoHideComponentsTimer.stop();
    }

    MWindow::mousePressEvent(event);
}

void MApplicationWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(MApplicationWindow);
    MSceneWindow *component;

    //close menu if navigation or toolbar was clicked
    if (isMenuOpen() && d->navigationBarPressed) {
        if (d->navigationBar->boundingRect().contains(d->navigationBar->mapFromScene(event->pos().x(), event->pos().y()))
            || d->toolBar->boundingRect().contains(d->toolBar->mapFromScene(event->pos().x(), event->pos().y()))) {
            closeMenu();
        }
    }
    d->navigationBarPressed = false;

    MWindow::mouseReleaseEvent(event);

    // This is for skipping appearing of auto hide elements on page switch
    if (d->page && d->page->sceneWindowState() == MSceneWindow::Appearing) return;

    if (d->componentsOnAutoHide.count() > 0) {
        // restart timer
        d->autoHideComponentsTimer.start();

        const int count = d->componentsOnAutoHide.count();
        for (int i = 0; i < count; ++i) {
            component = d->componentsOnAutoHide.at(i);

            if (component == d->dockWidget) {
                // Dock widget is a special guy.
                d->updateDockWidgetVisibility();
            } else if (component == d->navigationBar) {
                d->_q_updateNavigationBarVisibility();
            } else if (component->sceneWindowState() != MSceneWindow::Disappearing) {
                component->appear(this);
            }
        }

    }
}

QString MApplicationWindow::styleName() const
{
    Q_D(const MApplicationWindow);
    return d->style().objectName();
}

void MApplicationWindow::setStyleName(const QString &name)
{
    Q_D(MApplicationWindow);
    d->style().setObjectName(name);
    d->_q_updateStyle();
}

qreal MApplicationWindow::navigationBarOpacity() const
{
    Q_D(const MApplicationWindow);
    return d->navigationBar->opacity();
}

void MApplicationWindow::setNavigationBarOpacity(qreal opacity)
{
    Q_D(MApplicationWindow);
    d->navigationBar->setOpacity(opacity);
}

#include "moc_mapplicationwindow.cpp"
