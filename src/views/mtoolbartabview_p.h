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

#ifndef MTOOLBARTABVIEW_P_H
#define MTOOLBARTABVIEW_P_H

#include <QObject>
#include <QHash>

#include "maction.h"
#include "mnamespace.h"

class MToolBar;
class MWidget;
class MLinearLayoutPolicy;
class QEvent;
class MToolbarTabView;
class QAction;
class MButton;
class MWidgetAction;
class MLayout;
class QGraphicsLayoutItem;
class QGraphicsWidget;
class MButtonGroup;

class MToolbarTabViewPrivate : public QObject
{
    Q_DECLARE_PUBLIC(MToolbarTabView)

protected:
    MToolbarTabView *q_ptr;
    MToolBar *controller;

public:
    MToolbarTabViewPrivate(MToolBar *controller);
    virtual ~MToolbarTabViewPrivate();

    virtual void init();
    virtual void createPolicy(M::Orientation o);

    virtual void add(QAction *action, QAction *before, bool refreshSpacer);
    virtual void remove(QAction *action, bool refreshPolicies);
    virtual void change(QAction *action);

    void _q_groupButtonClicked(bool);
    void _q_groupActionToggled(bool);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void addActions();
    MWidget *createWidget(QAction *action);
    MButton *createTabButton(QAction *action);
    bool isLocationValid(QAction *action, MAction::Location loc);
    bool isVisible(QAction *action);
    void updateActionChecked();
    void clearWidgets(QHash<QAction *, MWidget *>& widgets);
    bool hasAction(QAction *action);
    int getItemIndex(MLinearLayoutPolicy *policy, QAction *before);
    MWidget *getWidget(QAction *action);
    bool changedLocation(QAction *action);
    void changedData(QAction *action);
    bool changedVisibility(QAction *action);
    void refreshSpacers();

protected:
    MWidget * widgetsContainer;
    MLayout *layout;
    MLinearLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;
    QHash<QAction *, MWidget *> tabButtons;
    QList<QGraphicsWidget *> freeSpacers;
    MButtonGroup * buttonGroup;

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

    void removeAction(MLinearLayoutPolicy *policy,
                      ActionPlacementData &policyData,
                      QAction *action);
    void removeAction(MLinearLayoutPolicy *policy,
                      ActionPlacementData &policyData,
                      QAction *action,
                      MWidget *widget);
    void clearPolicy(MLinearLayoutPolicy *policy, ActionPlacementData &policyData);
    void retrieveSpacers(MLinearLayoutPolicy *policy);
    void insertSpacers(MLinearLayoutPolicy *policy);
    void insertSpacer(MLinearLayoutPolicy *policy, int insertIndex);
    QGraphicsWidget *createSpacer();
    bool isItemSpacer(QGraphicsLayoutItem *item);
    bool refreshPolicyData(QAction *action,
                     MAction::Location location,
                     ActionPlacementData &policyData);
};

#endif
