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

#ifndef DUITOOLBARTABVIEW_P_H
#define DUITOOLBARTABVIEW_P_H

#include <QObject>
#include <QHash>

#include "private/duiwidgetview_p.h"
#include "duiaction.h"
#include "duinamespace.h"

class DuiToolBar;
class DuiWidget;
class DuiLinearLayoutPolicy;
class QEvent;
class DuiToolbarTabView;
class QAction;
class DuiButton;
class DuiWidgetAction;
class DuiLayout;
class QGraphicsLayoutItem;
class QGraphicsWidget;
class DuiButtonGroup;

class DuiToolbarTabViewPrivate : public DuiWidgetViewPrivate, public QObject
{
    Q_DECLARE_PUBLIC(DuiToolbarTabView)

public:
    DuiToolbarTabViewPrivate(DuiToolBar *controller);
    virtual ~DuiToolbarTabViewPrivate();

    virtual void init();
    virtual void createPolicy(Dui::Orientation o);

    virtual void add(QAction *action, QAction *before, bool refreshSpacer);
    virtual void remove(QAction *action, bool refreshPolicies);
    virtual void change(QAction *action);

    void _q_groupButtonClicked(bool);
    void _q_groupActionToggled(bool);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void addActions();
    DuiWidget *createWidget(QAction *action);
    DuiButton *createTabButton(QAction *action);
    bool isLocationValid(QAction *action, DuiAction::Location loc);
    bool isVisible(QAction *action);
    void updateActionChecked();
    void clearWidgets(QHash<QAction *, DuiWidget *>& widgets);
    bool hasAction(QAction *action);
    int getItemIndex(DuiLinearLayoutPolicy *policy, QAction *before);
    DuiWidget *getWidget(QAction *action);
    bool changedLocation(QAction *action);
    void changedData(QAction *action);
    bool changedVisibility(QAction *action);
    void refreshSpacers();

protected:
    DuiWidget * widgetsContainer;
    DuiLayout *layout;
    DuiLinearLayoutPolicy *landscapePolicy;
    DuiLinearLayoutPolicy *portraitPolicy;
    QHash<QAction *, DuiWidget *> tabButtons;
    QList<QGraphicsWidget *> freeSpacers;
    DuiButtonGroup * buttonGroup;

    static const int maxWidgets;

private:
    class ActionPlacementData
    {
    public:
        void reset() {
            placedActions = 0;
        }
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
    void retrieveSpacers(DuiLinearLayoutPolicy *policy);
    void insertSpacers(DuiLinearLayoutPolicy *policy);
    void insertSpacer(DuiLinearLayoutPolicy *policy, int insertIndex);
    QGraphicsWidget *createSpacer();
    bool isItemSpacer(QGraphicsLayoutItem *item);
    bool refreshPolicyData(QAction *action,
                     DuiAction::Location location,
                     ActionPlacementData &policyData);
};

#endif
