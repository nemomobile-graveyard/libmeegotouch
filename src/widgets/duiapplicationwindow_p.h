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

#ifndef DUIAPPLICATIONWINDOW_P_H
#define DUIAPPLICATIONWINDOW_P_H

#include "duiwindow_p.h"
#include "duiapplicationwindow.h"
#include "duiapplicationpagemodel.h"
#include "duiescapebuttonpanelmodel.h"
#include <QPointer>
#include <QTimer>

class QGraphicsItemAnimation;
class QTimeLine;
class DuiScene;
class DuiNavigationBar;
class DuiApplicationMenu;
class DuiToolBar;
class DuiDockWidget;
class DuiHomeButtonPanel;
class DuiEscapeButtonPanel;
class DuiApplicationPage;
class DuiApplicationPageInfo;
class DuiApplicationWindow;
class DuiSceneWindowEvent;
class QActionEvent;

class DuiApplicationWindowPrivate : public DuiWindowPrivate
{
    Q_DECLARE_PUBLIC(DuiApplicationWindow)

public:
    DuiApplicationWindowPrivate();
    virtual ~DuiApplicationWindowPrivate();

    QPointer<DuiApplicationPage> page;
    DuiNavigationBar *navigationBar;
    DuiToolBar *toolBar;
    DuiDockWidget *dockWidget;
    DuiHomeButtonPanel *homeButtonPanel;
    DuiEscapeButtonPanel *escapeButtonPanel;
    DuiApplicationMenu *menu;
    QPointer<DuiSceneWindow> statusBar;
    bool showingStatusBar;
    QTimer autoHideComponentsTimer;
    QList<DuiSceneWindow *> componentsOnAutoHide;

    void manageActions();
    void distributeAction(QAction *action, QAction *before);
    void refreshArrowIconVisibility();

    void setComponentDisplayMode(DuiSceneWindow *component,
                                 DuiApplicationPageModel::ComponentDisplayMode displayMode);
    void removeComponentFromAutoHide(DuiSceneWindow *component);

    /*!
     Connects the \a page's state such as the active navigation bar configuration and registered actions
     to the application window's common components.
     */
    void connectPage(DuiApplicationPage *page);

    /*!
     Disconnects the \a page's state from the application window.
     */
    void disconnectPage(DuiApplicationPage *page);

    /*!
        \brief Makes the dock widget either visible (when invisible) if a
        toolbar has been docked with some actions OR invisible (when visible)
        if a docked toolbar has 0 actions
     */
    void updateDockWidgetVisibility();

    void sceneWindowAppearEvent(DuiSceneWindowEvent *ev);
    void sceneWindowDisappearEvent(DuiSceneWindowEvent *ev);
    void sceneWindowDismissEvent(DuiSceneWindowEvent *ev);

    void updatePageAutoMarginsForComponents(const Dui::Orientation &orientation);

    void applicationPageAppearEvent(DuiSceneWindowEvent *ev);
    void applicationPageDisappearEvent(DuiSceneWindowEvent *ev);

    void windowStateChangeEvent(QWindowStateChangeEvent *event);

#ifdef Q_WS_X11
    void addDuiStatusBarOverlayProperty();
#endif

private:
    void init();
    void initAutoHideComponentsTimer();
    void removePageActions();
    QAction* findPageCheckedAction() const;
public:
    void _q_connectEscapeButton(DuiEscapeButtonPanelModel::EscapeMode);
    void _q_pageTitleChanged(DuiApplicationPage *, const QString &);
    void _q_actionUpdated(QActionEvent *e);
    void _q_placeToolBar(const Dui::Orientation &);
    void _q_handlePageModelModifications(const QList<const char *>&);
    void _q_menuAppeared();
    void _q_menuDisappeared();
#ifdef HAVE_N900
    void _q_exitAppView();
#endif
};

#endif

