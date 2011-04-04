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

#ifndef MAPPLICATIONWINDOW_P_H
#define MAPPLICATIONWINDOW_P_H

#include "mwindow_p.h"
#include "mapplicationwindow.h"
#include "mapplicationpagemodel.h"
#include "mescapebuttonpanelmodel.h"
#include <QPointer>
#include <QTimer>
#include "mapplicationwindowstyle.h"

#ifdef HAVE_CONTEXTSUBSCRIBER
# include "contextproperty.h"
#endif

class QGraphicsItemAnimation;
class QTimeLine;
class MScene;
class MNavigationBar;
class MApplicationMenu;
class MToolBar;
class MDockWidget;
class MHomeButtonPanel;
class MEscapeButtonPanel;
class MApplicationPage;
class MApplicationPageInfo;
class MApplicationWindow;
class MSceneWindowEvent;
class QActionEvent;

class MApplicationWindowPrivate : public MWindowPrivate
{
    Q_DECLARE_PUBLIC(MApplicationWindow)

public:
    MApplicationWindowPrivate();
    virtual ~MApplicationWindowPrivate();

    QPointer<MApplicationPage> page;
    QPointer<MNavigationBar> navigationBar;
    QPointer<MToolBar> toolBar;
    QPointer<MDockWidget> dockWidget;
    QPointer<MHomeButtonPanel> homeButtonPanel;
    QPointer<MApplicationMenu> menu;
    QPointer<MSceneWindow> statusBar;
    QTimer autoHideComponentsTimer;
    QList<MSceneWindow *> componentsOnAutoHide;
    bool pageAreaMaximized;
#ifdef Q_WS_X11
    bool    isChained;
    WId     chainParentWinId;
    QString chainTaskTitle;
#endif

#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty callStatusProperty;
#endif

    // TODO: Use some API like "bool MSceneWindow::isAppearing()" or something,
    //       once it becomes available, instead of manually tracking the scene windows
    //       appear/disappear statuses.
    bool showingNavigationBar;
    bool showingDockWidget;

    bool animateComponentsTransitions;
    QTimer navigationBarVisibilityUpdateTimer;

    bool navigationBarPressed;

    void manageActions();
    void distributeAction(QAction *action, QAction *before);
    void refreshArrowIconVisibility();

    void setComponentDisplayMode(MSceneWindow *component,
                                 MApplicationPageModel::ComponentDisplayMode displayMode);

    void addComponentToAutoHide(MSceneWindow *component);

    void removeComponentFromAutoHide(MSceneWindow *component);

    /*!
     Connects the \a page's state such as the active navigation bar configuration and registered actions
     to the application window's common components.
     */
    void connectPage(MApplicationPage *page);

    /*!
     Disconnects the \a page's state from the application window.
     */
    void disconnectPage(MApplicationPage *page);

    /*!
        \brief Makes the dock widget either visible (when invisible) if a
        toolbar has been docked with some actions OR invisible (when visible)
        if a docked toolbar has 0 actions
     */
    void updateDockWidgetVisibility();

    void _q_updateNavigationBarVisibility();

    // Update the navigation bar visibility at a subsequent event.
    void _q_scheduleNavigationBarVisibilityUpdate();

    void sceneWindowAppearEvent(MSceneWindowEvent *ev);
    void sceneWindowDisappearEvent(MSceneWindowEvent *ev);
    void sceneWindowDismissEvent(MSceneWindowEvent *ev);

    void applicationPageAppearEvent(MSceneWindowEvent *ev);
    void applicationPageDisappearEvent(MSceneWindowEvent *ev);

    void windowStateChangeEvent(QWindowStateChangeEvent *event);

    /*!
     Opens menu and disables a toolbar and an escape button.
     */
    void openMenu();
    void closeMenu();
    bool isMenuOpen() const;
#ifdef Q_WS_X11
    void appendMApplicationWindowTypeProperty();
#endif

    void setupPageEscape();
#ifdef Q_WS_X11
    bool setupPageEscapeChainedApplication();
#endif
    void setupPageEscapeAuto();
    void setupPageEscapeBack();
    void setupPageEscapeClose();
    void tearDownPageEscape();
    void _q_updatePageEscapeAuto();

    /*!
     * Maximizes visible area for page by hiding all other components (e.g. navigation bar).
     * Overrides each page's own preference for displaying components.
     */
    void maximizePageArea();

    /*!
     * Restores page area to current page's preference.
     */
    void restorePageArea();

    /*!
     * Whether the dock widget is needed or not
     */
    bool needsDockWidget();

    /*!
     * Set viewtype of the toolbar (tools or tabs)
     */
    void setToolBarViewType(const MTheme::ViewType& viewType);

    MApplicationWindowStyleContainer& style();
    const MApplicationWindowStyleContainer& style() const;
    virtual MApplicationWindowStyleContainer* createStyleContainer() const;

private:
    MApplicationWindowStyleContainer* styleContainer;

    void init();
    void initAutoHideComponentsTimer();
    void removePageActions();
    QAction* findCheckedAction(const QList<QAction *> &actions) const;
#ifdef Q_WS_X11
    void setWindowChainedProperty( const WId &parentWinId, const WId &childWinId );
#endif
    void updateStyleNames();
public:
    void _q_pageTitleChanged(MApplicationPage *, const QString &);
    void _q_actionUpdated(QActionEvent *e);
    void _q_placeToolBar();
    void _q_handlePageModelModifications(const QList<const char *>&);
    void _q_menuAppeared();
    void _q_menuDisappeared();
    void _q_updateStatusBarVisibility();
    void _q_navigationBarStateChanged(MSceneWindow::SceneWindowState newState, MSceneWindow::SceneWindowState oldState);
#ifdef M_OS_MAEMO5
    void _q_exitAppView();
#endif //M_OS_MAEMO5
    void _q_updatePageExposedContentRect();
    void _q_enablePaintUpdates();
    void _q_handleInSwitcherVisibilityChange();
    void _q_updateStyle();
    void _q_setupNavigationBarCustomContent();

    void updateChainTaskData();
    void applyWindowBackground();
    void setSceneWindowVisibility(MSceneWindow *sceneWindow, bool visible);
};

#endif
