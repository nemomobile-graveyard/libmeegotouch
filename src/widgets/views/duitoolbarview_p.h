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

#ifndef DUITOOLBARVIEW_P_H
#define DUITOOLBARVIEW_P_H

#include <QObject>
#include <QHash>

#include "private/duiwidgetview_p.h"
#include "duiaction.h"
#include "duinamespace.h"

class DuiToolBar;
class DuiWidget;
class DuiLinearLayoutPolicy;
class QEvent;
class DuiToolBarView;
class QAction;
class DuiButton;
class DuiWidgetAction;
class DuiLayout;
class QGraphicsLayoutItem;
class QGraphicsWidget;

class DuiToolBarViewPrivate : public DuiWidgetViewPrivate, public QObject
{
    Q_DECLARE_PUBLIC(DuiToolBarView)

public:
    DuiToolBarViewPrivate(DuiToolBar *controller);
    virtual ~DuiToolBarViewPrivate();

    virtual void init();
    virtual void createPolicy(Dui::Orientation o);

    virtual void add(QAction *action, QAction *before, bool refreshSpacer);
    virtual void remove(QAction *action, bool refreshPolicies);
    virtual void change(QAction *action);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void addActions();
    DuiWidget *createWidget(QAction *action);
    DuiButton *createButton(QAction *action);
    bool isLocationValid(QAction *action, DuiAction::Location loc);
    bool isVisible(QAction *action);
    void clearWidgets(QHash<QAction *, DuiWidget *>& widgets);
    void deleteWidget(QAction *action, DuiWidget *widget);
    bool releaseWidget(QAction *action, DuiWidget *widget);
    DuiWidget *requestWidget(DuiAction *action);
    bool isWidgetInUseByView(DuiWidgetAction *widgetAction);
    bool hasWidget(QAction *action);
    bool hasTextEditWidget(QAction *action);
    bool isWidgetUsable(QAction *action);
    bool isWidgetUsable(DuiWidgetAction *widgetAction);
    bool hasAction(QAction *action);
    int getItemIndex(DuiLinearLayoutPolicy *policy, QAction *before);
    DuiWidget *getWidget(QAction *action);
    bool changeLocation(QAction *action);
    void changeData(QAction *action);
    bool changeVisibility(QAction *action);
    void refreshSpacers();

protected:
    DuiWidget *widgetsContainer;
    DuiLayout *layout;
    DuiLinearLayoutPolicy *landscapePolicy;
    DuiLinearLayoutPolicy *portraitPolicy;
    QHash<QAction *, DuiWidget *> leasedWidgets;
    QHash<QAction *, DuiWidget *> buttons;
    QList<QGraphicsWidget *> freeSpacers;

    static const int maxWidgets;

private:
    enum PlacementMode {
        Managed = 0,
        Unmanaged
    };

    class ActionPlacementData
    {
    public:
        void reset() {
            mode = Managed;
            hasTextEditor = false;
            placedActions = 0;
        }
        PlacementMode mode;
        bool hasTextEditor;
        int placedActions;
    };

    ActionPlacementData landscapeData;
    ActionPlacementData portraitData;

    void removeAction(DuiLinearLayoutPolicy *policy,
                      ActionPlacementData &policyData,
                      QAction *action);
    void removeAction(DuiLinearLayoutPolicy *policy,
                      ActionPlacementData &policyData,
                      QAction *action,
                      DuiWidget *widget);
    void clearPolicy(DuiLinearLayoutPolicy *policy, ActionPlacementData &policyData);
    void retrieveSpacers(DuiLinearLayoutPolicy *policy, ActionPlacementData &policyData);
    void insertSpacers(DuiLinearLayoutPolicy *policy, const ActionPlacementData &policyData);
    void insertSpacer(DuiLinearLayoutPolicy *policy, int insertIndex);
    QGraphicsWidget *createSpacer();
    bool isItemSpacer(QGraphicsLayoutItem *item);
    bool refreshPolicyData(QAction *action,
                           DuiAction::Location location,
                           ActionPlacementData &policyData);
};

#endif
