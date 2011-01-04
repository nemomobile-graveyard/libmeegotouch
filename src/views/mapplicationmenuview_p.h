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

#ifndef MAPPLICATIONMENUVIEW_P_H
#define MAPPLICATIONMENUVIEW_P_H

#include <QObject>
#include <QSize>
#include <QHash>
#include "maction.h"
#include "mnamespace.h"

class MApplicationMenu;
class MApplicationMenuView;
class MPannableViewport;
class QGraphicsLayoutItem;
class MAbstractLayoutPolicy;
class MGridLayoutPolicy;
class MLinearLayoutPolicy;
class MFlowLayoutPolicy;
class MButton;
class MWidgetAction;
class MLayout;
class QGraphicsLinearLayout;
class MButtonGroup;
class MWidget;
class MBasicListItem;
class MComboBox;
class MWidgetController;
class MStylableWidget;

class MApplicationMenuViewPrivate : public QObject
{
    Q_DECLARE_PUBLIC(MApplicationMenuView)

protected:
    MApplicationMenuView *q_ptr;

public:
    MApplicationMenuViewPrivate(MApplicationMenu *menu);
    virtual ~MApplicationMenuViewPrivate();
    virtual void init();
    virtual void createPolicy(M::Orientation o);

    virtual void add(QAction *action, QAction *before);
    virtual void remove(QAction *action);
    virtual void change(QAction *action);

    void _q_displayEntered();

    static M::Position calculateLayoutPosition(int columnsCount, int itemCount, int index);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void addActions();
    MWidget *createWidget(QAction *action);
    MWidget *createButton(QAction *action);
    bool isStyleAction(QAction *action) const;
    bool isLocationValid(QAction *action, MAction::Location loc);
    bool isVisible(QAction *action);
    void clearWidgets(QHash<QAction *, MWidget *>& widgets);
    void deleteWidget(QAction *action, MWidget *widget);
    bool releaseWidget(QAction *action, MWidget *widget);
    MWidget *requestWidget(MAction *action);
    bool isWidgetInUseByView(MWidgetAction *widgetAction);
    bool hasWidget(QAction *action);
    bool isWidgetUsable(QAction *action);
    bool isWidgetUsable(MWidgetAction *widgetAction);
    MWidget *getWidget(QAction *action) const;
    bool canAddMoreActions(QAction *action) const;
    bool actionCountAndExists(QAction *action, bool isStyle, int &count) const;
    bool actionCountAndExists(QAction *action, QHash<QAction *, MWidget *> const &widgets,
                              bool isStyle, int& count ) const;
    bool changeLocation(QAction *action);
    void changeData(QAction *action);
    void changeVisibility(QAction *action);
    void changeStyleAttribute(QAction *action);
    void refreshPolicies(bool refresh);
    void clearPolicy(MAbstractLayoutPolicy *policy);
    void refreshLandscapePolicy();
    void updateItemMode();
    void updateItemLayout(MWidgetController *item, int columnsCount, int itemCount, int index);
    void addStyleWidget(MWidget *widget, MWidget *before);
    void addActionCommandWidget(MWidget *widget, MWidget *before);
    void removeStyleWidget(MWidget *widget);
    void addStyleSpacer();
    void makeLandscapePolicyColumnsEqual();

protected:
    QGraphicsLinearLayout *controllerLayout;
    MLayout *styleCommandLayout;
    MLayout *actionCommandLayout;
    MGridLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;
    MLinearLayoutPolicy *stylePolicy;
    MButtonGroup *styleButtonGroup;

    QHash<QAction *, MWidget *> leasedWidgets;
    QHash<QAction *, MWidget *> buttons;

    MApplicationMenu *controller;

    MPannableViewport* actionCommandViewport;
    MStylableWidget *topArea;

#ifdef UNIT_TEST
    friend class Ut_MApplicationMenu;
#endif
};

#endif
