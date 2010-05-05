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
    MNavigationBar *navigationBar;
    MToolBar *toolBar;
    MDockWidget *dockWidget;
    MHomeButtonPanel *homeButtonPanel;
    MEscapeButtonPanel *escapeButtonPanel;
    MApplicationMenu *menu;
    QPointer<MSceneWindow> statusBar;
    QTimer autoHideComponentsTimer;
    QList<MSceneWindow *> componentsOnAutoHide;
    bool isMenuOpen;
    bool callOngoing;

    // TODO: Use some API like "bool MSceneWindow::isAppearing()" or something,
    //       once it becomes available, instead of manually tracking the scene windows
    //       appear/disappear statuses.
    bool showingStatusBar;
    bool showingDockWidget;

    void manageActions();
    void distributeAction(QAction *action, QAction *before);
    void refreshArrowIconVisibility();

    void setComponentDisplayMode(MSceneWindow *component,
                                 MApplicationPageModel::ComponentDisplayMode displayMode);
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
#ifdef Q_WS_X11
    void addMStatusBarOverlayProperty();
    void appendMApplicationWindowTypeProperty();
#endif

    void setupPageEscape();
    void setupPageEscapeAuto();
    void setupPageEscapeBack();
    void setupPageEscapeClose();
    void tearDownPageEscape();
    void _q_updatePageEscapeAuto();

private:
    void init();
    void initAutoHideComponentsTimer();
    void removePageActions();
    QAction* findPageCheckedAction() const;
public:
    void _q_pageTitleChanged(MApplicationPage *, const QString &);
    void _q_actionUpdated(QActionEvent *e);
    void _q_placeToolBar(M::Orientation);
    void _q_handlePageModelModifications(const QList<const char *>&);
    void _q_menuAppeared();
    void _q_menuDisappeared();
#ifdef HAVE_DBUS
    void _q_updateCallOngoingState(QString mode);
#endif
#ifdef HAVE_N900
    void _q_exitAppView();
#endif
    void _q_updatePageExposedContentRect();
};

#endif

