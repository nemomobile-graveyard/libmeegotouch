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
#include <QtDBus/QtDBus>

#include <MDebug>
#include <mscenewindowevent_p.h>
#include "mapplicationwindow_p.h"
#include "mapplicationwindow.h"
#include "mapplicationpage.h"
#include "mapplicationpage_p.h"
#include "morientationtracker.h"
#include "mnavigationbar.h"
#include "mapplicationmenu.h"
#include "mtoolbar.h"
#include "mdockwidget.h"
#include "mcomponentdata.h"
#include "maction.h"
#include "mhomebuttonpanel.h"
#include "mescapebuttonpanel.h"
#include "mapplication.h"
#include "mcomponentdata.h"
#include "mscenemanager.h"
#include "mapplication_p.h"
#include "mscene.h"
#include "mstatusbar.h"

#include <QList>
#include <QEvent>
#include <QRectF>
#include <QActionEvent>

#ifdef HAVE_DBUS
# include <QDBusConnection>
#endif

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
#endif

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
    , navigationBar(new MNavigationBar)
    , toolBar(new MToolBar)
    , dockWidget(new MDockWidget)
    , homeButtonPanel(new MHomeButtonPanel)
    , escapeButtonPanel(new MEscapeButtonPanel)
    , menu(new MApplicationMenu)
    , statusBar(new MStatusBar)
    , isMenuOpen(false)
    , callOngoing(false)
    , showingStatusBar(false)
    , showingDockWidget(false)
{
}

MApplicationWindowPrivate::~MApplicationWindowPrivate()
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

void MApplicationWindowPrivate::init()
{
    Q_Q(MApplicationWindow);

    q->setOptimizationFlag(QGraphicsView::DontSavePainterState);

    q->connect(q, SIGNAL(orientationChanged(M::Orientation)),
               q, SLOT(_q_placeToolBar(M::Orientation)));

#ifdef Q_WS_X11
    addMStatusBarOverlayProperty();
    appendMApplicationWindowTypeProperty();
#endif

#ifdef HAVE_N900
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(_q_exitAppView()));
#else
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(closeMenu()));
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(showMinimized()));
#endif

    q->connect(navigationBar, SIGNAL(viewmenuTriggered()),
               q, SLOT(openMenu()));

    q->connect(menu, SIGNAL(appeared()),
               q, SLOT(_q_menuAppeared()));
    q->connect(menu, SIGNAL(disappeared()),
               q, SLOT(_q_menuDisappeared()));

    if (!MApplication::fullScreen()) {
        sceneManager->appearSceneWindowNow(statusBar);
    }

    sceneManager->appearSceneWindowNow(navigationBar);
    sceneManager->appearSceneWindowNow(homeButtonPanel);
    sceneManager->appearSceneWindowNow(escapeButtonPanel);

    // Initialize escape button to close mode.
    escapeButtonPanel->setEscapeMode(MEscapeButtonPanelModel::CloseMode);
    QObject::connect(escapeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(close()));

    if (q->orientation() == M::Portrait) {
        sceneManager->appearSceneWindowNow(dockWidget);
    }

    _q_placeToolBar(q->orientation());

    q->setBackgroundBrush(Qt::black);

    initAutoHideComponentsTimer();

#ifdef HAVE_DBUS
    // TODO: Take that info from Context Framework instead,
    //       once it becomes available.
    //       Initialization of callOngoing variable is missing at the moment.
    QDBusConnection systemBus(QDBusConnection::systemBus());
    systemBus.connect("", "/com/nokia/csd/csnet",
                      "com.nokia.csd.CSNet", "ActivityChanged", q,
                      SLOT(_q_updateCallOngoingState(QString)));
#endif
}

#ifdef Q_WS_X11
void MApplicationWindowPrivate::addMStatusBarOverlayProperty()
{
    Q_Q(MWindow);

    Atom atomMStatusBarOverlay = XInternAtom(QX11Info::display(), "_DUI_STATUSBAR_OVERLAY", False);
    long propertyData = 1;

    XChangeProperty(QX11Info::display(), q->winId(),
            atomMStatusBarOverlay, XA_CARDINAL /* type */,
            32 /* format, in bits */, PropModeReplace,
            (unsigned char *) &propertyData, 1 /* number of elements */);
}

void MApplicationWindowPrivate::appendMApplicationWindowTypeProperty()
{
    Q_Q(MApplicationWindow);

    Atom atomWindowType = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_NET_WM_WINDOW_TYPE_MAPPLICATION", False);
    XChangeProperty(QX11Info::display(), q->winId(),
                    XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False),
                    XA_ATOM, 32, PropModeAppend, (unsigned char*) &atomWindowType, 1);
}
#endif

void  MApplicationWindowPrivate::initAutoHideComponentsTimer()
{
    // TODO: Get the interval from CSS or some system wide
    // configuration.
    autoHideComponentsTimer.setInterval(2000);

    autoHideComponentsTimer.setSingleShot(true);
}

void MApplicationWindowPrivate::windowStateChangeEvent(QWindowStateChangeEvent *event)
{
    Q_Q(MApplicationWindow);
    Q_ASSERT(statusBar != 0);

    // Status bar should always be visible while a phone call is ongoing.
    if (!callOngoing) {
        if (q->isFullScreen() && !event->oldState().testFlag(Qt::WindowFullScreen)) {
            q->sceneManager()->disappearSceneWindowNow(statusBar);
        } else if (!q->isFullScreen() && event->oldState().testFlag(Qt::WindowFullScreen)) {
            q->sceneManager()->appearSceneWindowNow(statusBar);
        }
    }
}

#ifdef HAVE_DBUS
void MApplicationWindowPrivate::_q_updateCallOngoingState(QString mode)
{
    Q_Q(MApplicationWindow);

    // Status bar should always be visible while a phone call is ongoing.

    if (mode == "Call") {
        callOngoing = true;
        if (q->isFullScreen())
            q->sceneManager()->appearSceneWindowNow(statusBar);
    } else if (callOngoing) {
        callOngoing = false;
        if (q->isFullScreen())
            q->sceneManager()->disappearSceneWindowNow(statusBar);
    }
}
#endif

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
        if (member == MApplicationPageModel::HomeButtonDisplayMode) {
            setComponentDisplayMode(homeButtonPanel,
                                    page->model()->homeButtonDisplayMode());

        } else if (member == MApplicationPageModel::EscapeButtonDisplayMode) {
            setComponentDisplayMode(escapeButtonPanel,
                                    page->model()->escapeButtonDisplayMode());

        } else if (member == MApplicationPageModel::NavigationBarDisplayMode) {
            setComponentDisplayMode(navigationBar,
                                    page->model()->navigationBarDisplayMode());

            // Dock widget follows navigation bar display mode.
            setComponentDisplayMode(dockWidget,
                                    page->model()->navigationBarDisplayMode());

            // Interpretation of whether the navigation bar is covering the page
            // changes from "auto-hide" to "show". On "auto-hide" the navigation
            // bar doesn't count as covering the page since it does so only momentarily.
            updatePageExposedContentRect();

        } else if (member == MApplicationPageModel::ProgressIndicatorVisible) {
            navigationBar->setProgressIndicatorVisible(page->model()->progressIndicatorVisible());

        } else if (member == MApplicationPageModel::AutoMarginsForComponentsEnabled) {

            // FIXME: Fix this on the next ABI break.
            // updateAutoMarginsForComponents() calls due to changes in
            // MApplicationPageModel::autoMarginsForComponentsEnabled() should
            // be handled in MApplicationPage::updateData().
            page->d_func()->updateAutoMarginsForComponents();

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

void MApplicationWindowPrivate::_q_placeToolBar(M::Orientation orientation)
{
    // set position of toolbar according to orientation
    if (orientation == M::Portrait) {
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

void MApplicationWindowPrivate::updatePageExposedContentRect()
{
    Q_Q(MApplicationWindow);
    if (!page) {
        return;
    }

    qreal topCoverage = 0.0f;
    qreal bottomCoverage = 0.0f;

    QRectF pageExposedContentRect;

    if (showingStatusBar) {
        topCoverage += statusBar->effectiveSizeHint(Qt::PreferredSize).height();
    }

    if (page->componentDisplayMode(MApplicationPage::NavigationBar) == MApplicationPageModel::Show) {
        topCoverage += navigationBar->size().height();
    }

    if (showingDockWidget) {
        bottomCoverage += dockWidget->size().height();
    }

    pageExposedContentRect.setY(topCoverage);
    pageExposedContentRect.setWidth(q->visibleSceneSize().width());
    pageExposedContentRect.setHeight(q->visibleSceneSize().height()
                              - pageExposedContentRect.y() - bottomCoverage);

    page->d_func()->setExposedContentRect(pageExposedContentRect);
}

void MApplicationWindowPrivate::openMenu()
{
    Q_Q(MApplicationWindow);
    if (menu->actions().count() > 0) {
        menu->appear(q);
        escapeButtonPanel->setEnabled(false);
        toolBar->setEnabled(false);
    }
}

void MApplicationWindowPrivate::_q_menuAppeared()
{
    Q_Q(MApplicationWindow);
    QObject::disconnect(navigationBar, SIGNAL(viewmenuTriggered()),
                        q, SLOT(openMenu()));
    QObject::connect(navigationBar, SIGNAL(viewmenuTriggered()),
                     q, SLOT(closeMenu()));
    isMenuOpen = true;
}

void MApplicationWindowPrivate::_q_menuDisappeared()
{
    Q_Q(MApplicationWindow);
    QObject::disconnect(navigationBar, SIGNAL(viewmenuTriggered()),
                        q, SLOT(closeMenu()));
    QObject::connect(navigationBar, SIGNAL(viewmenuTriggered()),
                     q, SLOT(openMenu()));
    isMenuOpen = false;

    escapeButtonPanel->setEnabled(true);
    toolBar->setEnabled(true);
}

#ifdef HAVE_N900
void MApplicationWindowPrivate::_q_exitAppView()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage message = QDBusMessage::createSignal("/", "com.nokia.hildon_desktop", "exit_app_view");
    bus.send(message);
}
#endif

void MApplicationWindowPrivate::manageActions()
{
    Q_Q(MApplicationWindow);

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

void MApplicationWindowPrivate::distributeAction(QAction *action, QAction *before)
{
    MAction::Locations location = MAction::EveryLocation;
    MAction *mAction = qobject_cast<MAction *>(action);
    if (mAction)
        location = mAction->location();

    switch (location) {
    case MAction::NoLocation: {
        break;
    }
    case MAction::ToolBarLocation:
    case MAction::ToolBarLandscapeLocation:
    case MAction::ToolBarPortraitLocation: {
        toolBar->insertAction(before, action);
        break;
    }
    case MAction::ApplicationMenuLocation: {
        menu->insertAction(before, action);
        break;
    }
    case MAction::EveryLocation: {
        toolBar->insertAction(before, action);
        menu->insertAction(before, action);
        break;
    }
    default: {
        mDebug("MApplicationWindowPrivate") << "Unknown Locations value.";
        break;
    }
    }
}

void MApplicationWindowPrivate::refreshArrowIconVisibility()
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

void MApplicationWindowPrivate::setComponentDisplayMode(
    MSceneWindow *component, MApplicationPageModel::ComponentDisplayMode displayMode)
{
    Q_Q(MApplicationWindow);

    switch (displayMode) {
    case MApplicationPageModel::Show:
        removeComponentFromAutoHide(component);

        if (component == dockWidget) {
            // Dock widget is a special guy.
            updateDockWidgetVisibility();
        } else {
            component->appear(q);
        }
        break;

    case MApplicationPageModel::AutoHide:
        if (!componentsOnAutoHide.contains(component)) {
            component->connect(&autoHideComponentsTimer, SIGNAL(timeout()),
                               SLOT(disappear()));

            // Visibility in sync with other components in auto-hide mode.
            if (autoHideComponentsTimer.isActive()) {
                if (component == dockWidget) {
                    // Dock widget is a special guy.
                    updateDockWidgetVisibility();
                } else {
                    sceneManager->appearSceneWindowNow(component);
                }
            } else {
                component->disappear();
            }

            componentsOnAutoHide << component;
        }
        break;

    case MApplicationPageModel::Hide:
        removeComponentFromAutoHide(component);
        if (component->isVisible())
            component->disappear();
        break;
    };

    if (componentsOnAutoHide.count() == 0) {
        autoHideComponentsTimer.stop();
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
    Q_Q(MApplicationWindow);

    // Make dock widget visible only if a toolbar is docked and
    // the docked toolbar has some actions

    if (!dockWidget->toolBar() || (q->orientation() != M::Portrait)) {
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
        sceneManager->appearSceneWindowNow(dockWidget);
    } else {
        sceneManager->disappearSceneWindowNow(dockWidget);
    }
}

void MApplicationWindowPrivate::sceneWindowAppearEvent(MSceneWindowEvent *event)
{
    MSceneWindow *sceneWindow = event->sceneWindow();

    switch (sceneWindow->windowType()) {
        case MSceneWindow::ApplicationPage:
            applicationPageAppearEvent(event);
            break;

        case MSceneWindow::StatusBar:
            showingStatusBar = true;
            updatePageExposedContentRect();
            break;

        case MSceneWindow::NavigationBar:
            updatePageExposedContentRect();
            break;

        case MSceneWindow::DockWidget:
            showingDockWidget = true;
            updatePageExposedContentRect();
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

        case MSceneWindow::StatusBar:
            showingStatusBar = false;
            updatePageExposedContentRect();
            break;

        case MSceneWindow::NavigationBar:
            updatePageExposedContentRect();
            break;

        case MSceneWindow::DockWidget:
            showingDockWidget = false;
            updatePageExposedContentRect();
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
        menu->disappear();
        disconnectPage(page);
    }

    connectPage(pageFromEvent);
    updatePageExposedContentRect();
    pageFromEvent->d_func()->prepareForAppearance();
}

void MApplicationWindowPrivate::applicationPageDisappearEvent(MSceneWindowEvent *event)
{
    MApplicationPage *pageFromEvent = static_cast<MApplicationPage *>(event->sceneWindow());

    // Page is going away. Let's disconnect it if it's the current page.
    if (pageFromEvent == page) {
        menu->disappear();
        disconnectPage(pageFromEvent);
    }
}

void MApplicationWindowPrivate::setupPageEscape()
{
    if (!page) {
        // Nothing to be done.
        return;
    }

    // Tear down any previous page escape setup.
    tearDownPageEscape();

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
}

void MApplicationWindowPrivate::setupPageEscapeAuto()
{
    Q_Q(MApplicationWindow);
    QList<MSceneWindow*> pageHistory = q->sceneManager()->pageHistory();

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

void MApplicationWindowPrivate::setupPageEscapeBack()
{
    escapeButtonPanel->setEscapeMode(MEscapeButtonPanelModel::BackMode);
    page->connect(escapeButtonPanel, SIGNAL(buttonClicked()), SIGNAL(backButtonClicked()));
}

void MApplicationWindowPrivate::setupPageEscapeClose()
{
    Q_Q(MApplicationWindow);

    escapeButtonPanel->setEscapeMode(MEscapeButtonPanelModel::CloseMode);
    page->connect(escapeButtonPanel, SIGNAL(buttonClicked()), SIGNAL(closeButtonClicked()));
    QObject::connect(escapeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(close()));
}

void MApplicationWindowPrivate::tearDownPageEscape()
{
    Q_Q(MApplicationWindow);

    QObject::disconnect(escapeButtonPanel, SIGNAL(buttonClicked()), 0, 0);

    QObject::disconnect(q->sceneManager(), SIGNAL(pageHistoryChanged()),
            q, SLOT(_q_updatePageEscapeAuto()));
}

void MApplicationWindowPrivate::_q_updatePageEscapeAuto()
{
    Q_Q(MApplicationWindow);
    QList<MSceneWindow*> pageHistory = q->sceneManager()->pageHistory();

    Q_ASSERT(page != 0);
    Q_ASSERT(page->escapeMode() == MApplicationPageModel::EscapeAuto);

    if (pageHistory.isEmpty() &&
            (escapeButtonPanel->escapeMode() != MEscapeButtonPanelModel::CloseMode)) {

        QObject::disconnect(escapeButtonPanel, SIGNAL(buttonClicked()), 0, 0);
        setupPageEscapeClose();

    } else if (!pageHistory.isEmpty() &&
            (escapeButtonPanel->escapeMode() != MEscapeButtonPanelModel::BackMode)) {

        QObject::disconnect(escapeButtonPanel, SIGNAL(buttonClicked()), 0, 0);
        setupPageEscapeBack();
        page->connect(escapeButtonPanel, SIGNAL(buttonClicked()), SLOT(dismiss()));
    }
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

        case QEvent::ActionChanged:
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
            event->accept();
            return;
        }
    }

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

void MApplicationWindow::openMenu()
{
    Q_D(MApplicationWindow);
    d->openMenu();
}

void MApplicationWindow::closeMenu()
{
    Q_D(MApplicationWindow);
    if (d->isMenuOpen) d->menu->disappear();
}

void MApplicationWindow::setWindowIconID(const QString &windowIconID)
{
    Q_D(MApplicationWindow);
    d->navigationBar->setViewMenuIconID(windowIconID);
}

void MApplicationWindow::setToolbarViewType(const MTheme::ViewType& viewType)
{
    Q_D(MApplicationWindow);
    d->toolBar->setViewType(viewType);
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

    setupPageEscape();

    navigationBar->setViewMenuDescription(page->title());
    q->setWindowTitle(longestLengthVariant(page->title()));

    setComponentDisplayMode(homeButtonPanel, page->model()->homeButtonDisplayMode());
    setComponentDisplayMode(escapeButtonPanel, page->model()->escapeButtonDisplayMode());
    setComponentDisplayMode(navigationBar, page->model()->navigationBarDisplayMode());

    // Dock widget follows navigation bar display mode.
    setComponentDisplayMode(dockWidget, page->model()->navigationBarDisplayMode());

    navigationBar->setProgressIndicatorVisible(page->model()->progressIndicatorVisible());

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

    tearDownPageEscape();


    removePageActions();

    page = 0;
}

void MApplicationWindowPrivate::removePageActions()
{
    QAction* checkedAction = findPageCheckedAction();

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

QAction* MApplicationWindowPrivate::findPageCheckedAction() const
{
    QAction* checkedAction = 0;
    QList<QAction *> actions = page->actions();
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
    return d->isMenuOpen;
}

void MApplicationWindow::mousePressEvent(QMouseEvent *event)
{
    Q_D(MApplicationWindow);

    if (d->autoHideComponentsTimer.isActive()) {
        d->autoHideComponentsTimer.stop();
    }

    MWindow::mousePressEvent(event);
}

void MApplicationWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(MApplicationWindow);
    MSceneWindow *component;

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

    if (d->isMenuOpen) {
        if (d->navigationBar->boundingRect().contains(d->navigationBar->mapFromScene(event->pos().x(), event->pos().y()))
            || d->toolBar->boundingRect().contains(d->toolBar->mapFromScene(event->pos().x(), event->pos().y()))) {
            closeMenu();
        }
    }

    MWindow::mouseReleaseEvent(event);
}

#include "moc_mapplicationwindow.cpp"
