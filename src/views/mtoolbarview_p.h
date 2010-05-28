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
class ToolBarLayoutPolicy;

class MToolBarViewPrivate : public QObject
{
    Q_DECLARE_PUBLIC(MToolBarView)

public:
    MToolBarViewPrivate(MToolBar *controller);
    virtual ~MToolBarViewPrivate();

    void init();

    void add(QAction *action);
    void remove(QAction *action, bool hideOnly);
    void change(QAction *action);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    MWidget *createWidget(QAction *action);

    int policyIndexForAddingAction(QAction *action, ToolBarLayoutPolicy *policy) const;
    bool isLocationValid(QAction *action, MAction::Location loc) const;
    bool releaseWidget(QAction *action, MWidget *widget) const;
    bool hasTextEditWidget(QAction *action) const;
    bool hasUnusableWidget(QAction *action) const;
    void updateWidgetFromAction(MWidget *widget, QAction *action) const;
    MWidget *getWidget(QAction *action) const;
    void _q_groupButtonClicked(bool);
    void _q_groupActionToggled(bool);
protected:
    MToolBarView *q_ptr;
    MToolBar *controller;
    MWidget *widgetsContainer;
    MLayout *layout;
    QHash<QAction *, MWidget *> leasedWidgets;
    QHash<QAction *, MButton *> buttons;
    /* If this is non-null, created buttons will be placed in this group */
    MButtonGroup * buttonGroup;

    ToolBarLayoutPolicy *landscapePolicy;
    ToolBarLayoutPolicy *portraitPolicy;
};

#endif
