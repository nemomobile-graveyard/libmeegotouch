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

#include <QList>
#include <QEvent>
#include <QRectF>
#include <QActionEvent>

DuiApplicationWindowPrivate::DuiApplicationWindowPrivate()
    : DuiWindowPrivate()
    , page(NULL)
    , navigationBar(new DuiNavigationBar)
    , toolBar(new DuiToolBar)
    , dockWidget(new DuiDockWidget)
    , homeButtonPanel(new DuiHomeButtonPanel)
    , escapeButtonPanel(new DuiEscapeButtonPanel)
    , menu(new DuiApplicationMenu)
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
}

void DuiApplicationWindowPrivate::init()
{
    Q_Q(DuiApplicationWindow);

    q->setOptimizationFlag(QGraphicsView::DontSavePainterState);

    q->resize(q->visibleSceneSize(Dui::Landscape));
    q->setFrameStyle(0);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    q->setSceneRect(QRectF(QPointF(), q->visibleSceneSize(Dui::Landscape)));
    q->centerOn(q->sceneRect().center());

    q->connect(q, SIGNAL(orientationChanged(Dui::Orientation)),
               q, SLOT(_q_placeToolBar(Dui::Orientation)));

#ifdef HAVE_N900
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(_q_exitAppView()));
#else
    q->connect(homeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(showMinimized()));
#endif

    q->connect(escapeButtonPanel, SIGNAL(escapeModeChanged(DuiEscapeButtonPanelModel::EscapeMode)),
               q, SLOT(_q_connectEscapeButton(DuiEscapeButtonPanelModel::EscapeMode)));

    q->connect(navigationBar, SIGNAL(viewmenuTriggered()),
               q, SLOT(openMenu()));

    q->connect(menu, SIGNAL(appeared()),
               q, SLOT(_q_menuAppeared()));
    q->connect(menu, SIGNAL(disappeared()),
               q, SLOT(_q_menuDisappeared()));

    navigationBar->appearNow(q);
    homeButtonPanel->appearNow(q);
    escapeButtonPanel->appearNow(q);
    _q_connectEscapeButton(escapeButtonPanel->escapeMode());

    if (q->orientation() == Dui::Portrait) {
        dockWidget->appearNow(q);
    }

    // make toolbar
    _q_placeToolBar(q->orientation());

    q->setBackgroundBrush(Qt::black);

    initAutoHideComponentsTimer();
}

void  DuiApplicationWindowPrivate::initAutoHideComponentsTimer()
{
    // TODO: Get the interval from CSS or some system wide
    // configuration.
    autoHideComponentsTimer.setInterval(2000);

    autoHideComponentsTimer.setSingleShot(true);
}

void DuiApplicationWindowPrivate::_q_connectEscapeButton(DuiEscapeButtonPanelModel::EscapeMode mode)
{
    Q_Q(DuiApplicationWindow);

    QObject::disconnect(escapeButtonPanel, SIGNAL(buttonClicked()), 0, 0);

    switch (mode) {
    case DuiEscapeButtonPanelModel::BackMode:
        if (page)
            QObject::connect(escapeButtonPanel, SIGNAL(buttonClicked()), page, SIGNAL(backButtonClicked()));

        QObject::connect(escapeButtonPanel, SIGNAL(buttonClicked()), menu, SLOT(disappear()));
        break;
    case DuiEscapeButtonPanelModel::CloseMode:
        if (page)
            QObject::connect(escapeButtonPanel, SIGNAL(buttonClicked()), page, SIGNAL(closeButtonClicked()));

        QObject::connect(escapeButtonPanel, SIGNAL(buttonClicked()), q, SLOT(close()));
        break;
    default:;
    }
}

void DuiApplicationWindowPrivate::_q_pageTitleChanged(DuiApplicationPage *page, const QString &title)
{
    if (page->isVisible())
        navigationBar->setViewMenuDescription(title);
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

        _q_updatePageAutoMarginsForComponents(q->orientation());
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

            _q_updatePageAutoMarginsForComponents(q->orientation());

        } else if (member == DuiApplicationPageModel::ProgressIndicatorVisible) {
            navigationBar->setProgressIndicatorVisible(page->model()->progressIndicatorVisible());

        } else if (member == DuiApplicationPageModel::AutoMarginsForComponentsEnabled) {
            _q_updatePageAutoMarginsForComponents(q->orientation());
        }
    }
}

void DuiApplicationWindowPrivate::_q_placeToolBar(const Dui::Orientation &orientation)
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

void DuiApplicationWindowPrivate::_q_updatePageAutoMarginsForComponents(const Dui::Orientation &orientation)
{
    if (page) {
        page->d_func()->updateAutoMarginsForComponents(orientation,
                navigationBarHeight(),
                dockWidget->size().height(), dockWidget->isVisible());
    }
}

void DuiApplicationWindowPrivate::_q_menuAppeared()
{
    Q_Q(DuiApplicationWindow);
    QObject::disconnect(navigationBar, SIGNAL(viewmenuTriggered()),
                        q, SLOT(openMenu()));
    QObject::connect(navigationBar, SIGNAL(viewmenuTriggered()),
                     q, SLOT(closeMenu()));
}

void DuiApplicationWindowPrivate::_q_menuDisappeared()
{
    Q_Q(DuiApplicationWindow);
    QObject::disconnect(navigationBar, SIGNAL(viewmenuTriggered()),
                        q, SLOT(closeMenu()));
    QObject::connect(navigationBar, SIGNAL(viewmenuTriggered()),
                     q, SLOT(openMenu()));
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
    // empty toolbar and menu
    toolBar->clearActions();
    menu->clearActions();

    QAction *before = 0;
    QList<QAction *> actions = page->actions();
    const int actionsSize = actions.size();
    for (int i = 0; i < actionsSize; ++i) {
        distributeAction(actions[i], before);
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

    int count = toolBar->actions().count();
    if (count > 0) {
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

    if (sceneWindow->windowType() != DuiSceneWindow::ApplicationPage) {
        // We are only interested in application page transitions
        return;
    }

    DuiApplicationPage *pageFromEvent = static_cast<DuiApplicationPage *>(sceneWindow);

    // It cannot be the current page
    Q_ASSERT(pageFromEvent != page);

    if (page != 0) {
        // We are going to change pages.

        q->closeMenu();
    }

    connectPage(pageFromEvent);
    _q_updatePageAutoMarginsForComponents(q->orientation());
    pageFromEvent->d_func()->prepareForAppearance();
}

void DuiApplicationWindowPrivate::sceneWindowDisappearEvent(DuiSceneWindowEvent *event)
{
    DuiSceneWindow *sceneWindow = event->sceneWindow();

    if (sceneWindow->windowType() != DuiSceneWindow::ApplicationPage) {
        // We are only interested in application page transitions
        return;
    }

    DuiApplicationPage *pageFromEvent = static_cast<DuiApplicationPage *>(sceneWindow);

    // Page is going away. Let's disconnect it if it's the current page.
    if (pageFromEvent == page)
        disconnectPage(pageFromEvent);
}

void DuiApplicationWindowPrivate::sceneWindowDismissEvent(DuiSceneWindowEvent *event)
{
    DuiSceneWindow *sceneWindow = event->sceneWindow();

    if (sceneWindow->windowType() != DuiSceneWindow::ApplicationPage) {
        // We are only interested in application page transitions
        return;
    }

    DuiApplicationPage *pageFromEvent = static_cast<DuiApplicationPage *>(sceneWindow);

    // Page is going away. Let's disconnect it if it's the current page.
    if (pageFromEvent == page)
        disconnectPage(pageFromEvent);
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

void DuiApplicationWindow::show()
{
    // Act differently if app is in prestarted mode
    if (DuiApplication::isPrestarted()) {
        switch (DuiApplication::prestartMode()) {
        case Dui::LazyShutdown:
        case Dui::TerminateOnClose:
            return;

        default:
            break;
        }
    }

    DuiWindow::show();
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

bool DuiApplicationWindow::close()
{
    if (DuiApplication::prestartMode() == Dui::LazyShutdown) {
        DuiApplicationPrivate::restorePrestart();
        DuiOnDisplayChangeEvent ev(false, sceneRect());
        onDisplayChangeEvent(&ev);
        return true;
    } else {
        return DuiWindow::close();
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

void DuiApplicationWindowPrivate::connectPage(DuiApplicationPage *newPage)
{
    Q_Q(DuiApplicationWindow);

    Q_ASSERT(newPage != 0);
    page = newPage;

    q->connect(page, SIGNAL(pageTitleChanged(DuiApplicationPage *, QString)),
               SLOT(_q_pageTitleChanged(DuiApplicationPage *, QString)));

    q->connect(page, SIGNAL(actionUpdated(QActionEvent *)), SLOT(_q_actionUpdated(QActionEvent *)));

    manageActions();

    QObject::connect(page, SIGNAL(escapeButtonModeChanged(DuiEscapeButtonPanelModel::EscapeMode)),
                     escapeButtonPanel, SLOT(setEscapeMode(DuiEscapeButtonPanelModel::EscapeMode)));

    q->connect(page->model(), SIGNAL(modified(QList<const char *>)),
               SLOT(_q_handlePageModelModifications(QList<const char *>)));

    DuiEscapeButtonPanelModel::EscapeMode pageEscapeMode = page->escapeButtonMode();
    if (pageEscapeMode != escapeButtonPanel->escapeMode()) {
        // Change the mode of the escape button, which will trigger a reconnection
        // between escape button and application window + page.
        escapeButtonPanel->setEscapeMode(pageEscapeMode);
    } else {
        // Escape button is already in the correct mode.
        // Force a reconnection between escape button and application window + page
        _q_connectEscapeButton(page->escapeButtonMode());
    }

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

    QObject::disconnect(page, SIGNAL(pageTitleChanged(DuiApplicationPage *, QString)),
                        q, SLOT(_q_pageTitleChanged(DuiApplicationPage *, QString)));

    QObject::disconnect(page, SIGNAL(actionUpdated(QActionEvent *)),
                        q, SLOT(_q_actionUpdated(QActionEvent *)));

    QObject::disconnect(page, SIGNAL(escapeButtonModeChanged(DuiEscapeButtonPanelModel::EscapeMode)),
                        escapeButtonPanel, SLOT(setEscapeMode(DuiEscapeButtonPanelModel::EscapeMode)));

    QObject::disconnect(page->model(), SIGNAL(modified(QList<const char *>)),
                        q, SLOT(_q_handlePageModelModifications(QList<const char *>)));

    QObject::disconnect(escapeButtonPanel, SIGNAL(buttonClicked()), page, SIGNAL(backButtonClicked()));
    QObject::disconnect(escapeButtonPanel, SIGNAL(buttonClicked()), page, SIGNAL(closeButtonClicked()));

    page = 0;
}

int DuiApplicationWindowPrivate::navigationBarHeight() const
{
    return navigationBar->minimumHeight();
}

QString DuiApplicationWindow::windowIconID() const
{
    Q_D(const DuiApplicationWindow);
    return d->navigationBar->viewMenuIconID();
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
