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

#include "duiondisplaychangeevent.h"
#include <QtDBus/QtDBus>

#include <DuiDebug>
#include <duiscenewindowevent_p.h>
#include "duiapplicationwindow_p.h"
#include "duiapplicationwindow.h"
#include "duiapplicationpage.h"
#include "duiapplicationpage_p.h"
#include "duiorientationtracker.h"
#include "duinavigationbar.h"
#include "duiapplicationmenu.h"
#include "duitoolbar.h"
#include "duidockwidget.h"
#include "duicomponentdata.h"
#include "duiaction.h"
#include "duihomebuttonpanel.h"
#include "duiescapebuttonpanel.h"
#include "duiapplication.h"
#include "duicomponentdata.h"
#include "duiscenemanager.h"
#include "duiapplication_p.h"
#include "duiscene.h"
#include "duistatusbar.h"

#include <QList>
#include <QEvent>
#include <QRectF>
#include <QActionEvent>

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
#endif

DuiApplicationWindowPrivate::DuiApplicationWindowPrivate()
    : DuiWindowPrivate()
    , page(NULL)
    , navigationBar(new DuiNavigationBar)
    , toolBar(new DuiToolBar)
    , dockWidget(new DuiDockWidget)
    , homeButtonPanel(new DuiHomeButtonPanel)
    , escapeButtonPanel(new DuiEscapeButtonPanel)
    , menu(new DuiApplicationMenu)
    , statusBar(new DuiStatusBar)
    , showingStatusBar(false)
    , isMenuOpen(false)
{
}

DuiApplicationWindowPrivate::~DuiApplicationWindowPrivate()
{
    delete dockWidget;
    dockWidget = 0;
    delete toolBar;
    toolBar = 0;
    delete menu;
    menu = 0;
    delete navigationBar;
    navigationBar = 0;
    delete homeButtonPanel;
    homeButtonPanel = 0;
    delete escapeButtonPanel;
    escapeButtonPanel = 0;

    if (statusBar) {
        delete statusBar;
        statusBar = 0;
    }
}

void DuiApplicationWindowPrivate::init()
{
    Q_Q(DuiApplicationWindow);

    q->setOptimizationFlag(QGraphicsView::DontSavePainterState);

    q->setSceneRect(QRectF(QPointF(), q->visibleSceneSize(Dui::Landscape)));
    q->centerOn(q->sceneRect().center());

    q->connect(q, SIGNAL(orientationChanged(Dui::Orientation)),
               q, SLOT(_q_placeToolBar(Dui::Orientation)));

#ifdef Q_WS_X11
    addDuiStatusBarOverlayProperty();
#endif

#ifdef HAVE_N900
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(_q_exitAppView()));
#else
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(showMinimized()));
#endif

    q->connect(navigationBar, SIGNAL(viewmenuTriggered()),
               q, SLOT(openMenu()));

    q->connect(menu, SIGNAL(appeared()),
               q, SLOT(_q_menuAppeared()));
    q->connect(menu, SIGNAL(disappeared()),
               q, SLOT(_q_menuDisappeared()));

    if (!DuiApplication::fullScreen()) {
        statusBar->appearNow(q);
        showingStatusBar = true;
    }

    navigationBar->appearNow(q);
    homeButtonPanel->appearNow(q);
    escapeButtonPanel->appearNow(q);

    // Initialize escape button to close mode.
    escapeButtonPanel->setEscapeMode(DuiEscapeButtonPanelModel::CloseMode);
    QObject::connect(escapeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(close()));

    if (q->orientation() == Dui::Portrait) {
        dockWidget->appearNow(q);
    }

    _q_placeToolBar(q->orientation());

    q->setBackgroundBrush(Qt::black);

    initAutoHideComponentsTimer();
}

#ifdef Q_WS_X11
void DuiApplicationWindowPrivate::addDuiStatusBarOverlayProperty()
{
    Q_Q(DuiWindow);

    Atom atomDuiStatusBarOverlay = XInternAtom(QX11Info::display(), "_DUI_STATUSBAR_OVERLAY", False);
    long propertyData = 1;

    XChangeProperty(QX11Info::display(), q->winId(),
            atomDuiStatusBarOverlay, XA_CARDINAL /* type */,
            32 /* format, in bits */, PropModeReplace,
            (unsigned char *) &propertyData, 1 /* number of elements */);
}
#endif

void  DuiApplicationWindowPrivate::initAutoHideComponentsTimer()
{
    // TODO: Get the interval from CSS or some system wide
    // configuration.
    autoHideComponentsTimer.setInterval(2000);

    autoHideComponentsTimer.setSingleShot(true);
}

void DuiApplicationWindowPrivate::windowStateChangeEvent(QWindowStateChangeEvent *event)
{
    Q_Q(DuiApplicationWindow);
    Q_ASSERT(statusBar != 0);

    if (q->isFullScreen() && !event->oldState().testFlag(Qt::WindowFullScreen)) {
        q->sceneManager()->disappearSceneWindowNow(statusBar);

    } else if (!q->isFullScreen() && event->oldState().testFlag(Qt::WindowFullScreen)) {
        q->sceneManager()->appearSceneWindowNow(statusBar);
    }
}

void DuiApplicationWindowPrivate::_q_actionUpdated(QActionEvent *event)
{
    Q_Q(DuiApplicationWindow);
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

        updatePageAutoMarginsForComponents(q->orientation());
    }
}

void DuiApplicationWindowPrivate::_q_handlePageModelModifications(const QList<const char *> &modifications)
{
    Q_Q(DuiApplicationWindow);
    const char *member;

    foreach(member, modifications) {
        if (member == DuiApplicationPageModel::HomeButtonDisplayMode) {
            setComponentDisplayMode(homeButtonPanel,
                                    page->model()->homeButtonDisplayMode());

        } else if (member == DuiApplicationPageModel::EscapeButtonDisplayMode) {
            setComponentDisplayMode(escapeButtonPanel,
                                    page->model()->escapeButtonDisplayMode());

        } else if (member == DuiApplicationPageModel::NavigationBarDisplayMode) {
            setComponentDisplayMode(navigationBar,
                                    page->model()->navigationBarDisplayMode());

            // Dock widget follows navigation bar display mode.
            setComponentDisplayMode(dockWidget,
                                    page->model()->navigationBarDisplayMode());

            updatePageAutoMarginsForComponents(q->orientation());

        } else if (member == DuiApplicationPageModel::ProgressIndicatorVisible) {
            navigationBar->setProgressIndicatorVisible(page->model()->progressIndicatorVisible());

        } else if (member == DuiApplicationPageModel::AutoMarginsForComponentsEnabled) {
            updatePageAutoMarginsForComponents(q->orientation());

        } else if (member == DuiApplicationPageModel::EscapeMode) {
            setupPageEscape();

        } else if (member == DuiApplicationPageModel::Title) {
            if (page->isVisible())
                navigationBar->setViewMenuDescription(page->model()->title());

        }
    }
}

void DuiApplicationWindowPrivate::_q_placeToolBar(Dui::Orientation orientation)
{
    // set position of toolbar according to orientation
    if (orientation == Dui::Portrait) {
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

void DuiApplicationWindowPrivate::updatePageAutoMarginsForComponents(Dui::Orientation orientation)
{
    if (!page) {
        return;
    }

    qreal statusBarHeight;

    if (showingStatusBar) {
        statusBarHeight = statusBar->effectiveSizeHint(Qt::PreferredSize).height();
    } else {
        statusBarHeight = 0;
    }

    page->d_func()->updateAutoMarginsForComponents(orientation,
            statusBarHeight,
            navigationBar->size().height(),
            dockWidget->size().height(), dockWidget->isVisible());
}

void DuiApplicationWindowPrivate::_q_menuAppeared()
{
    Q_Q(DuiApplicationWindow);
    QObject::disconnect(navigationBar, SIGNAL(viewmenuTriggered()),
                        q, SLOT(openMenu()));
    QObject::connect(navigationBar, SIGNAL(viewmenuTriggered()),
                     q, SLOT(closeMenu()));
    isMenuOpen = true;
}

void DuiApplicationWindowPrivate::_q_menuDisappeared()
{
    Q_Q(DuiApplicationWindow);
    QObject::disconnect(navigationBar, SIGNAL(viewmenuTriggered()),
                        q, SLOT(closeMenu()));
    QObject::connect(navigationBar, SIGNAL(viewmenuTriggered()),
                     q, SLOT(openMenu()));
    isMenuOpen = false;
}

#ifdef HAVE_N900
void DuiApplicationWindowPrivate::_q_exitAppView()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createSignal("/", "com.nokia.hildon_desktop", "exit_app_view");
    bus.send(message);
}
#endif

void DuiApplicationWindowPrivate::manageActions()
{
    Q_Q(DuiApplicationWindow);

    QAction* pageCheckedAction = findPageCheckedAction();

    toolBar->clearActions();
    menu->clearActions();

    // add page actions
    QAction *before = 0;
    QList<QAction *> actions = page->actions();
    int actionsSize = actions.size();
    for (int i = 0; i < actionsSize; ++i) {
        distributeAction(actions[i], before);
    }

    // add window actions
    actions = q->actions();
    actionsSize = actions.size();
    for (int i = 0; i < actionsSize; ++i) {
        distributeAction(actions[i], before);
    }

    if (pageCheckedAction) {
        pageCheckedAction->setChecked(true);
    }

    refreshArrowIconVisibility();
}

void DuiApplicationWindowPrivate::distributeAction(QAction *action, QAction *before)
{
    DuiAction::Locations location = DuiAction::EveryLocation;
    DuiAction *duiAction = qobject_cast<DuiAction *>(action);
    if (duiAction)
        location = duiAction->location();

    switch (location) {
    case DuiAction::NoLocation: {
        break;
    }
    case DuiAction::ToolBarLocation:
    case DuiAction::ToolBarLandscapeLocation:
    case DuiAction::ToolBarPortraitLocation: {
        toolBar->insertAction(before, action);
        break;
    }
    case DuiAction::ApplicationMenuLocation: {
        menu->insertAction(before, action);
        break;
    }
    case DuiAction::EveryLocation: {
        toolBar->insertAction(before, action);
        menu->insertAction(before, action);
        break;
    }
    default: {
        duiDebug("DuiApplicationWindowPrivate") << "Unknown Locations value.";
        break;
    }
    }
}

void DuiApplicationWindowPrivate::refreshArrowIconVisibility()
{
    bool visible = navigationBar->isArrowIconVisible();
    if (menu->actions().count() == 0) {
        if (visible)
            navigationBar->setArrowIconVisible(false);
    } else {
        if (!visible)
            navigationBar->setArrowIconVisible(true);
    }
}

void DuiApplicationWindowPrivate::setComponentDisplayMode(
    DuiSceneWindow *component, DuiApplicationPageModel::ComponentDisplayMode displayMode)
{
    Q_Q(DuiApplicationWindow);

    switch (displayMode) {
    case DuiApplicationPageModel::Show:
        removeComponentFromAutoHide(component);

        if (component == dockWidget) {
            // Dock widget is a special guy.
            updateDockWidgetVisibility();
        } else {
            component->appear(q);
        }
        break;

    case DuiApplicationPageModel::AutoHide:
        if (!componentsOnAutoHide.contains(component)) {
            component->connect(&autoHideComponentsTimer, SIGNAL(timeout()),
                               SLOT(disappear()));

            // Visibility in sync with other components in auto-hide mode.
            if (autoHideComponentsTimer.isActive()) {
                if (component == dockWidget) {
                    // Dock widget is a special guy.
                    updateDockWidgetVisibility();
                } else {
                    component->appearNow(q);
                }
            } else {
                component->disappear();
            }

            componentsOnAutoHide << component;
        }
        break;

    case DuiApplicationPageModel::Hide:
        removeComponentFromAutoHide(component);
        if (component->isVisible())
            component->disappear();
        break;
    };

    if (componentsOnAutoHide.count() == 0) {
        autoHideComponentsTimer.stop();
    }
}

void DuiApplicationWindowPrivate::removeComponentFromAutoHide(DuiSceneWindow *component)
{
    if (componentsOnAutoHide.contains(component)) {
        autoHideComponentsTimer.disconnect(SIGNAL(timeout()),
                                           component, SLOT(disappear()));

        componentsOnAutoHide.removeAll(component);
    }
}

void DuiApplicationWindowPrivate::updateDockWidgetVisibility()
{
    Q_Q(DuiApplicationWindow);

    // Make dock widget visible only if a toolbar is docked and
    // the docked toolbar has some actions

    if (!dockWidget->toolBar() || (q->orientation() != Dui::Portrait)) {
        return;
    }

    QList<QAction *> actions = toolBar->actions();
    int count = actions.count();
    bool toolbarHasVisibleActions = false;
    for (int i = 0; i < count; ++i) {
        if (actions[i]->isVisible()) {
            toolbarHasVisibleActions = true;
            break;
        }
    }

    if (toolbarHasVisibleActions) {
        //TODO: no animation until appear/disappear starts working properly
        dockWidget->appearNow(q);
    } else {
        dockWidget->disappearNow();
    }
}

void DuiApplicationWindowPrivate::sceneWindowAppearEvent(DuiSceneWindowEvent *event)
{
    Q_Q(DuiApplicationWindow);
    DuiSceneWindow *sceneWindow = event->sceneWindow();

    if (sceneWindow->windowType() == DuiSceneWindow::ApplicationPage) {
        applicationPageAppearEvent(event);
    } else if (sceneWindow->windowType() == DuiSceneWindow::StatusBar) {
        showingStatusBar = true;
        updatePageAutoMarginsForComponents(q->orientation());
    }
}

void DuiApplicationWindowPrivate::sceneWindowDisappearEvent(DuiSceneWindowEvent *event)
{
    Q_Q(DuiApplicationWindow);
    DuiSceneWindow *sceneWindow = event->sceneWindow();

    if (sceneWindow->windowType() == DuiSceneWindow::ApplicationPage) {
        applicationPageDisappearEvent(event);
    } else if (sceneWindow->windowType() == DuiSceneWindow::StatusBar) {
        showingStatusBar = false;
        updatePageAutoMarginsForComponents(q->orientation());
    }
}

void DuiApplicationWindowPrivate::sceneWindowDismissEvent(DuiSceneWindowEvent *event)
{
    // Currently we handle it in the same way as for a regular disappearance.
    sceneWindowDisappearEvent(event);
}

void DuiApplicationWindowPrivate::applicationPageAppearEvent(DuiSceneWindowEvent *event)
{
    Q_Q(DuiApplicationWindow);
    DuiApplicationPage *pageFromEvent = static_cast<DuiApplicationPage *>(event->sceneWindow());

    // It cannot be the current page
    Q_ASSERT(pageFromEvent != page);

    if (page != 0) {
        menu->disappear();
        disconnectPage(page);
    }

    connectPage(pageFromEvent);
    updatePageAutoMarginsForComponents(q->orientation());
    pageFromEvent->d_func()->prepareForAppearance();
}

void DuiApplicationWindowPrivate::applicationPageDisappearEvent(DuiSceneWindowEvent *event)
{
    DuiApplicationPage *pageFromEvent = static_cast<DuiApplicationPage *>(event->sceneWindow());

    // Page is going away. Let's disconnect it if it's the current page.
    if (pageFromEvent == page) {
        menu->disappear();
        disconnectPage(pageFromEvent);
    }
}

void DuiApplicationWindowPrivate::setupPageEscape()
{
    if (!page) {
        // Nothing to be done.
        return;
    }

    // Tear down any previous page escape setup.
    tearDownPageEscape();

    switch (page->escapeMode()) {
        case DuiApplicationPageModel::EscapeAuto:
            setupPageEscapeAuto();
            break;

        case DuiApplicationPageModel::EscapeManualBack:
            setupPageEscapeBack();
            break;

        case DuiApplicationPageModel::EscapeCloseWindow:
            setupPageEscapeClose();
            break;

        default:
            qFatal("DuiApplicationWindow: Invalid page escape mode");
    };
}

void DuiApplicationWindowPrivate::setupPageEscapeAuto()
{
    Q_Q(DuiApplicationWindow);
    QList<DuiSceneWindow*> pageHistory = q->sceneManager()->pageHistory();

    if (pageHistory.isEmpty()) {
        setupPageEscapeClose();
    } else {
        setupPageEscapeBack();
        page->connect(escapeButtonPanel, SIGNAL(buttonClicked()), SLOT(dismiss()));
    }

    // We must update the wiring of our escape button if the application manually
    // changes the page history while a page in EscapeAuto mode is being displayed.
    q->connect(q->sceneManager(), SIGNAL(pageHistoryChanged()), SLOT(_q_updatePageEscapeAuto()));
}

void DuiApplicationWindowPrivate::setupPageEscapeBack()
{
    escapeButtonPanel->setEscapeMode(DuiEscapeButtonPanelModel::BackMode);
    page->connect(escapeButtonPanel, SIGNAL(buttonClicked()), SIGNAL(backButtonClicked()));
}

void DuiApplicationWindowPrivate::setupPageEscapeClose()
{
    Q_Q(DuiApplicationWindow);

    escapeButtonPanel->setEscapeMode(DuiEscapeButtonPanelModel::CloseMode);
    page->connect(escapeButtonPanel, SIGNAL(buttonClicked()), SIGNAL(closeButtonClicked()));
    QObject::connect(escapeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(close()));
}

void DuiApplicationWindowPrivate::tearDownPageEscape()
{
    Q_Q(DuiApplicationWindow);

    QObject::disconnect(escapeButtonPanel, SIGNAL(buttonClicked()), 0, 0);

    QObject::disconnect(q->sceneManager(), SIGNAL(pageHistoryChanged()),
            q, SLOT(_q_updatePageEscapeAuto()));
}

void DuiApplicationWindowPrivate::_q_updatePageEscapeAuto()
{
    Q_Q(DuiApplicationWindow);
    QList<DuiSceneWindow*> pageHistory = q->sceneManager()->pageHistory();

    Q_ASSERT(page != 0);
    Q_ASSERT(page->escapeMode() == DuiApplicationPageModel::EscapeAuto);

    if (pageHistory.isEmpty() &&
            (escapeButtonPanel->escapeMode() != DuiEscapeButtonPanelModel::CloseMode)) {

        QObject::disconnect(escapeButtonPanel, SIGNAL(buttonClicked()), 0, 0);
        setupPageEscapeClose();

    } else if (!pageHistory.isEmpty() &&
            (escapeButtonPanel->escapeMode() != DuiEscapeButtonPanelModel::BackMode)) {

        QObject::disconnect(escapeButtonPanel, SIGNAL(buttonClicked()), 0, 0);
        setupPageEscapeBack();
        page->connect(escapeButtonPanel, SIGNAL(buttonClicked()), SLOT(dismiss()));
    }
}

// TODO: Remove that now useless method override after API freeze period
void DuiApplicationWindow::onDisplayChangeEvent(DuiOnDisplayChangeEvent *event)
{
    DuiWindow::onDisplayChangeEvent(event);
}

DuiApplicationWindow::DuiApplicationWindow(DuiApplicationWindowPrivate &dd, QWidget *parent)
    : DuiWindow(dd, new DuiSceneManager, parent)
{
    Q_D(DuiApplicationWindow);

    d->init();

    DuiComponentData::registerWindow(this);
}

DuiApplicationWindow::DuiApplicationWindow(DuiApplicationWindowPrivate &dd, DuiScene *scene, QWidget *parent)
    : DuiWindow(dd, scene, parent)
{
    Q_D(DuiApplicationWindow);

    d->init();

    DuiComponentData::registerWindow(this);
}

DuiApplicationWindow::DuiApplicationWindow(QWidget *parent)
    : DuiWindow(*new DuiApplicationWindowPrivate, new DuiSceneManager, parent)
{
    Q_D(DuiApplicationWindow);

    d->init();

    DuiComponentData::registerWindow(this);
}

DuiApplicationWindow::DuiApplicationWindow(DuiScene *scene, QWidget *parent)
    : DuiWindow(*new DuiApplicationWindowPrivate, scene, parent)
{
    Q_D(DuiApplicationWindow);

    d->init();

    DuiComponentData::registerWindow(this);
}

DuiApplicationWindow::~DuiApplicationWindow()
{
    DuiComponentData::unregisterWindow(this);
}

DuiApplicationPage *DuiApplicationWindow::currentPage() const
{
    Q_D(const DuiApplicationWindow);
    return d->page;
}

bool DuiApplicationWindow::event(QEvent *event)
{
    Q_D(DuiApplicationWindow);

    QActionEvent * actionEvent = (QActionEvent *)event;
    QEvent::Type type = event->type();
    switch (type) {
        case QEvent::ActionRemoved: {
            QAction *action = actionEvent->action();
            if (action) {
                action->disconnect(this);
            }
            //fall through is intentional
        }
        case QEvent::ActionAdded: {
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

    return DuiWindow::event(event);
}

// We have to send this root message because the window itself is managing
// its close events. In directUi, there's no WM with a close button
void DuiApplicationWindow::closeEvent(QCloseEvent *event)
{
#ifdef Q_WS_X11

    if (testAttribute(Qt::WA_QuitOnClose) && (windowState() & Qt::WindowNoState)) {
        XEvent ev;
        Display *dpy = QX11Info::display();
        memset(&ev, 0, sizeof(ev));

        ev.xclient.type         = ClientMessage;
        ev.xclient.display      = dpy;
        ev.xclient.window       = winId();
        ev.xclient.message_type = XInternAtom(dpy, "_NET_CLOSE_WINDOW", False);
        ev.xclient.format       = 32;
        ev.xclient.data.l[0]    = CurrentTime;
        ev.xclient.data.l[1]    = QX11Info::appRootWindow();

        XSendEvent(dpy, QX11Info::appRootWindow(), False, SubstructureRedirectMask | SubstructureNotifyMask,
                   &ev);

        XSync(dpy, False);
    }
#endif
    event->accept();
}

void DuiApplicationWindow::openMenu()
{
    Q_D(DuiApplicationWindow);

    if (d->menu->actions().count() > 0) {
        d->menu->appear(this);
    }
}

void DuiApplicationWindow::closeMenu()
{
    Q_D(DuiApplicationWindow);
    d->menu->disappear();

}

void DuiApplicationWindow::setWindowIconID(const QString &windowIconID)
{
    Q_D(DuiApplicationWindow);
    d->navigationBar->setViewMenuIconID(windowIconID);
}

void DuiApplicationWindow::setToolbarViewType(const DuiTheme::ViewType& viewType)
{
    Q_D(DuiApplicationWindow);
    d->toolBar->setViewType(viewType);
}

void DuiApplicationWindowPrivate::connectPage(DuiApplicationPage *newPage)
{
    Q_Q(DuiApplicationWindow);

    Q_ASSERT(newPage != 0);
    page = newPage;

    q->connect(page, SIGNAL(actionUpdated(QActionEvent *)), SLOT(_q_actionUpdated(QActionEvent *)));

    manageActions();

    q->connect(page->model(), SIGNAL(modified(QList<const char *>)),
               SLOT(_q_handlePageModelModifications(QList<const char *>)));

    setupPageEscape();

    navigationBar->setViewMenuDescription(page->title());

    setComponentDisplayMode(homeButtonPanel, page->model()->homeButtonDisplayMode());
    setComponentDisplayMode(escapeButtonPanel, page->model()->escapeButtonDisplayMode());
    setComponentDisplayMode(navigationBar, page->model()->navigationBarDisplayMode());

    // Dock widget follows navigation bar display mode.
    setComponentDisplayMode(dockWidget, page->model()->navigationBarDisplayMode());

    navigationBar->setProgressIndicatorVisible(page->model()->progressIndicatorVisible());

    emit q->pageChanged(page);
}

void DuiApplicationWindowPrivate::disconnectPage(DuiApplicationPage *pageToDisconnect)
{
    Q_UNUSED(pageToDisconnect)
    Q_Q(DuiApplicationWindow);

    Q_ASSERT(pageToDisconnect == page);

    QObject::disconnect(page, SIGNAL(actionUpdated(QActionEvent *)),
                        q, SLOT(_q_actionUpdated(QActionEvent *)));

    QObject::disconnect(page->model(), SIGNAL(modified(QList<const char *>)),
                        q, SLOT(_q_handlePageModelModifications(QList<const char *>)));

    tearDownPageEscape();


    removePageActions();

    page = 0;
}

void DuiApplicationWindowPrivate::removePageActions()
{
    QAction* checkedAction = findPageCheckedAction();

    // remove all the page actions
    QList<QAction *> actions = page->actions();
    int actionsSize = actions.size();
    for (int i = 0; i < actionsSize; ++i) {
        QAction* action = actions[i];
        QAction* toolbarAction = 0;
        if (toolBar->viewType() == DuiToolBar::tabType &&
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

QAction* DuiApplicationWindowPrivate::findPageCheckedAction() const
{
    QAction* checkedAction = 0;
    QList<QAction *> actions = page->actions();
    int actionsSize = actions.size();
    if (toolBar->viewType() == DuiToolBar::tabType) {
        for (int i = 0; i < actionsSize; ++i) {
            QAction* action = actions[i];
            DuiAction* duiAction = qobject_cast<DuiAction*>(action);
            bool isToolbarAction = (!duiAction || duiAction->location().testFlag(DuiAction::ToolBarPortraitLocation) ||
                             duiAction->location().testFlag(DuiAction::ToolBarLandscapeLocation));
            if (action->isChecked() && isToolbarAction) {
                checkedAction = action;
                break;
            }
        }
    }
    return checkedAction;
}

QString DuiApplicationWindow::windowIconID() const
{
    Q_D(const DuiApplicationWindow);
    return d->navigationBar->viewMenuIconID();
}

DuiTheme::ViewType DuiApplicationWindow::toolbarViewType() const
{
    Q_D(const DuiApplicationWindow);
    return d->toolBar->viewType();
}

bool DuiApplicationWindow::isMenuOpen() const
{
    Q_D(const DuiApplicationWindow);
    return d->isMenuOpen;
}

void DuiApplicationWindow::mousePressEvent(QMouseEvent *event)
{
    Q_D(DuiApplicationWindow);

    if (d->autoHideComponentsTimer.isActive()) {
        d->autoHideComponentsTimer.stop();
    }

    DuiWindow::mousePressEvent(event);
}

void DuiApplicationWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(DuiApplicationWindow);
    DuiSceneWindow *component;

    if (d->componentsOnAutoHide.count() > 0) {
        const int count = d->componentsOnAutoHide.count();
        for (int i = 0; i < count; ++i) {
            component = d->componentsOnAutoHide.at(i);

            if (component == d->dockWidget) {
                // Dock widget is a special guy.
                d->updateDockWidgetVisibility();
            } else {
                component->appear(this);
            }
        }

        // restart timer
        d->autoHideComponentsTimer.start();
    }

    DuiWindow::mouseReleaseEvent(event);
}

#include "moc_duiapplicationwindow.cpp"
