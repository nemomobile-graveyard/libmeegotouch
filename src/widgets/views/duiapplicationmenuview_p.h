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

#ifndef DUIAPPLICATIONMENUVIEW_P_H
#define DUIAPPLICATIONMENUVIEW_P_H

#include <QObject>
#include <QSize>
#include <QHash>
#include "duiaction.h"
#include "duiscenewindowview_p.h"
#include "duinamespace.h"
#include "duiapplicationmenuview.h"

class DuiApplicationMenu;
class DuiPannableViewport;
class QGraphicsLayoutItem;
class DuiAbstractLayoutPolicy;
class DuiGridLayoutPolicy;
class DuiLinearLayoutPolicy;
class DuiFlowLayoutPolicy;
class DuiButton;
class DuiWidgetAction;
class DuiLayout;
class QGraphicsLinearLayout;
class DuiButtonGroup;
class DuiWidget;
class DuiContentItem;
class DuiComboBox;

class DuiApplicationMenuViewPrivate : public DuiSceneWindowViewPrivate, public QObject
{
    Q_DECLARE_PUBLIC(DuiApplicationMenuView)

public:
    DuiApplicationMenuViewPrivate(DuiApplicationMenu *menu);
    virtual ~DuiApplicationMenuViewPrivate();
    virtual void init();
    virtual void createPolicy(Dui::Orientation o);

    virtual void add(QAction *action, QAction *before);
    virtual void remove(QAction *action);
    virtual void change(QAction *action);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void addActions();
    DuiWidget *createWidget(QAction *action);
    DuiWidget *createButton(QAction *action);
    bool isStyleAction(QAction *action);
    bool isLocationValid(QAction *action, DuiAction::Location loc);
    bool isVisible(QAction *action);
    void clearWidgets(QHash<QAction *, DuiWidget *>& widgets);
    void deleteWidget(QAction *action, DuiWidget *widget);
    bool releaseWidget(QAction *action, DuiWidget *widget);
    DuiWidget *requestWidget(DuiAction *action);
    bool isWidgetInUseByView(DuiWidgetAction *widgetAction);
    bool hasWidget(QAction *action);
    bool isWidgetUsable(QAction *action);
    bool isWidgetUsable(DuiWidgetAction *widgetAction);
    DuiWidget *getWidget(QAction *action);
    bool canAddMoreActions(QAction *action);
    void visibleActionsCount(int &commandActionsCount, int &styleActionsCount);
    bool changeLocation(QAction *action);
    void changeData(QAction *action);
    void changeVisibility(QAction *action);
    void changeStyleAttribute(QAction *action);
    void refreshPolicies(bool refresh);
    void clearPolicy(DuiAbstractLayoutPolicy *policy);
    void refreshLandscapePolicy();
    void updateItemMode();
    void updateContentItemMode(DuiContentItem *contentItem, int columnsCount, int itemCount, int index);
    void updateComboboxMode(DuiComboBox *comboBox, int columnsCount, int itemCount, int index);
    void addStyleWidget(DuiWidget *widget, DuiWidget *before);
    void addActionCommandWidget(DuiWidget *widget, DuiWidget *before);
    void removeStyleWidget(DuiWidget *widget);
    void addStyleSpacer();
    void makeLandscapePolicyColumnsEqual();

protected:
    QGraphicsLinearLayout *controllerLayout;
    DuiLayout *styleCommandLayout;
    DuiLayout *actionCommandLayout;
    DuiGridLayoutPolicy *landscapePolicy;
    DuiLinearLayoutPolicy *portraitPolicy;
    DuiLinearLayoutPolicy *stylePolicy;
    DuiButtonGroup *styleButtonGroup;
    QHash<QAction *, DuiWidget *> leasedWidgets;
    QHash<QAction *, DuiWidget *> buttons;
};

#endif
