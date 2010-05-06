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

#ifndef MTOOLBARVIEW_P_H
#define MTOOLBARVIEW_P_H

#include <QObject>
#include <QHash>

#include "maction.h"
#include "mnamespace.h"

class MToolBar;
class MWidget;
class MLinearLayoutPolicy;
class QEvent;
class MToolBarView;
class QAction;
class MButton;
class MWidgetAction;
class MLayout;
class QGraphicsLayoutItem;
class QGraphicsWidget;

class MToolBarViewPrivate : public QObject
{
    Q_DECLARE_PUBLIC(MToolBarView)

public:
    MToolBarViewPrivate(MToolBar *controller);
    virtual ~MToolBarViewPrivate();

    virtual void init();
    virtual void createPolicy(M::Orientation o);

    virtual void add(QAction *action, QAction *before, bool refreshSpacer);
    virtual void remove(QAction *action, bool refreshPolicies);
    virtual void change(QAction *action);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void addActions();
    MWidget *createWidget(QAction *action);
    MButton *createButton(QAction *action);
    bool isLocationValid(QAction *action, MAction::Location loc);
    bool isVisible(QAction *action);
    void clearWidgets(QHash<QAction *, MWidget *>& widgets);
    void deleteWidget(QAction *action, MWidget *widget);
    bool releaseWidget(QAction *action, MWidget *widget);
    MWidget *requestWidget(MAction *action);
    bool isWidgetInUseByView(MWidgetAction *widgetAction);
    bool hasWidget(QAction *action);
    bool hasTextEditWidget(QAction *action);
    bool isWidgetUsable(QAction *action);
    bool isWidgetUsable(MWidgetAction *widgetAction);
    bool hasAction(QAction *action);
    int getItemIndex(MLinearLayoutPolicy *policy, QAction *before);
    MWidget *getWidget(QAction *action);
    bool changeLocation(QAction *action);
    void changeData(QAction *action);
    bool changeVisibility(QAction *action);
    void refreshSpacers();

protected:
    MToolBarView *q_ptr;
    MToolBar *controller;
    MWidget *widgetsContainer;
    MLayout *layout;
    MLinearLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;
    QHash<QAction *, MWidget *> leasedWidgets;
    QHash<QAction *, MWidget *> buttons;
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

    void removeAction(MLinearLayoutPolicy *policy,
                      ActionPlacementData &policyData,
                      QAction *action);
    void removeAction(MLinearLayoutPolicy *policy,
                      ActionPlacementData &policyData,
                      QAction *action,
                      MWidget *widget);
    void clearPolicy(MLinearLayoutPolicy *policy, ActionPlacementData &policyData);
    void retrieveSpacers(MLinearLayoutPolicy *policy, ActionPlacementData &policyData);
    void insertSpacers(MLinearLayoutPolicy *policy, const ActionPlacementData &policyData);
    void insertSpacer(MLinearLayoutPolicy *policy, int insertIndex);
    QGraphicsWidget *createSpacer();
    bool isItemSpacer(QGraphicsLayoutItem *item);
    bool refreshPolicyData(QAction *action,
                           MAction::Location location,
                           ActionPlacementData &policyData);
};

#endif
