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

#include <QGraphicsLinearLayout>
#include <QActionGroup>

#include "duitoolbar.h"
#include "duitheme.h"
#include "duibutton.h"
#include "duiviewcreator.h"
#include "duiwidgetaction.h"
#include "private/duiwidgetview_p.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duibuttongroup.h"

#include "duilayout.h"
#include "duilinearlayoutpolicy.h"
#include "duitextedit.h"
#include "duitoolbartabview.h"
#include "duitoolbartabview_p.h"


const int DuiToolbarTabViewPrivate::maxWidgets = 4;

DuiToolbarTabViewPrivate::DuiToolbarTabViewPrivate(DuiToolBar *controller)
    : DuiWidgetViewPrivate(),
      QObject(),
      widgetsContainer(0),
      layout(0),
      landscapePolicy(0),
      portraitPolicy(0),
      tabButtons()
{
    this->controller = controller;
    controller->installEventFilter(this);
}


DuiToolbarTabViewPrivate::~DuiToolbarTabViewPrivate()
{
    clearWidgets(tabButtons);
    controller->removeEventFilter(this);
    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->removeItem(widgetsContainer);
    delete widgetsContainer;
}

void DuiToolbarTabViewPrivate::init()
{
    widgetsContainer = new DuiWidget();
    widgetsContainer->setObjectName("toolbarTabContainer");

    layout = new DuiLayout(widgetsContainer);
    layout->setContentsMargins(0, 0, 0, 0);

    createPolicy(Dui::Landscape);
    createPolicy(Dui::Portrait);

    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->addItem(widgetsContainer);

    buttonGroup = new DuiButtonGroup(controller);
    buttonGroup->setExclusive(true);

    addActions();
}

void DuiToolbarTabViewPrivate::createPolicy(Dui::Orientation orientation)
{
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    policy->setSpacing(0);
    policy->setContentsMargins(0, 0, 0, 0);
    if (orientation == Dui::Landscape) {
        landscapePolicy = policy;
        layout->setLandscapePolicy(landscapePolicy);
        landscapeData.reset();
    } else {
        portraitPolicy = policy;
        layout->setPortraitPolicy(portraitPolicy);
        portraitData.reset();
    }
}

void DuiToolbarTabViewPrivate::add(QAction *action, QAction *before, bool refreshSpacer)
{
    bool validLocation = (isLocationValid(action, DuiAction::ToolBarLandscapeLocation) ||
                          isLocationValid(action, DuiAction::ToolBarPortraitLocation));
    if (!action || !validLocation || !action->isCheckable()) {
        return;
    }

    bool added = false;
    DuiWidget *w = createWidget(action);
    // add to policies only if the action is visible
    if (action->isVisible()) {
        bool addToLandscape = refreshPolicyData(action,
                                        DuiAction::ToolBarLandscapeLocation,
                                        landscapeData);
        bool addToPortrait = refreshPolicyData(action,
                                      DuiAction::ToolBarPortraitLocation,
                                      portraitData);
        if (addToLandscape || addToPortrait) {
            if (addToLandscape) {
                landscapePolicy->insertItem(getItemIndex(landscapePolicy, before), w);
            }
            if (addToPortrait) {
                portraitPolicy->insertItem(getItemIndex(portraitPolicy, before), w);
            }
            if (refreshSpacer) {
                refreshSpacers();
            }
            added = true;
        }
    }

    if (!added && w) {
        w->setVisible(false);
    }
}

void DuiToolbarTabViewPrivate::remove(QAction *action, bool refreshPolicies)
{
    Q_Q(DuiToolbarTabView);
    DuiButton *widget = (DuiButton*)tabButtons.value(action);

    if (widget) {
        removeAction(landscapePolicy, landscapeData, action, widget);
        removeAction(portraitPolicy, portraitData, action, widget);

        layout->removeItem(widget);
        tabButtons.remove(action);
        buttonGroup->removeButton(widget);
        delete widget;
        widget = 0;
        action->disconnect(q);
    }

    if (refreshPolicies) {
        clearPolicy(landscapePolicy, landscapeData);
        clearPolicy(portraitPolicy, portraitData);
        addActions();
    }
}

void DuiToolbarTabViewPrivate::change(QAction *action)
{
    if (changedLocation(action) || changedVisibility(action)) {
        clearPolicy(landscapePolicy, landscapeData);
        clearPolicy(portraitPolicy, portraitData);
        addActions();
    }
    changedData(action);
}

void DuiToolbarTabViewPrivate::_q_groupButtonClicked(bool)
{
    DuiButton *button = qobject_cast<DuiButton *>(sender());
    if (button) {
        button->setChecked(true);
    }
    updateActionChecked();
}

void DuiToolbarTabViewPrivate::_q_groupActionToggled(bool checked)
{
    QAction* action = qobject_cast<QAction *>(sender());
    DuiButton *button = qobject_cast<DuiButton *>(tabButtons.value(action));
    if (button) {
        button->setChecked(checked);
    }
    updateActionChecked();
}

bool DuiToolbarTabViewPrivate::eventFilter(QObject *obj, QEvent *e)
{
    QActionEvent *actionEvent = dynamic_cast<QActionEvent *>(e);

    if (actionEvent) {
        switch (e->type()) {
        case QEvent::ActionRemoved: {
            remove(actionEvent->action(), true);
            break;
        }
        case QEvent::ActionAdded: {
            add(actionEvent->action(), actionEvent->before(), true);
            break;
        }
        case QEvent::ActionChanged: {
            change(actionEvent->action());
            break;
        }
        default: {
            break;
        }
        }
        updateActionChecked();
    }


    return QObject::eventFilter(obj, e);
}

void DuiToolbarTabViewPrivate::addActions()
{
    QList<QAction *> acts = controller->actions();
    int count = acts.count();
    for (int i = 0; i < count; ++i) {
        add(acts.at(i), 0, false);
    }
    refreshSpacers();
}

DuiWidget *DuiToolbarTabViewPrivate::createWidget(QAction *action)
{
    // If widget is not already created then create it
    DuiWidget *widget = tabButtons.value(action);
    if (!widget) {
        widget = createTabButton(action);
        tabButtons.insert(action, widget);
    }
    widget->setVisible(true);
    widget->setEnabled(action->isEnabled());
    return widget;
}

DuiButton *DuiToolbarTabViewPrivate::createTabButton(QAction *action)
{
    Q_Q(DuiToolbarTabView);

    DuiButton *button = new DuiButton();
    DuiAction *duiAction = qobject_cast<DuiAction *>(action);
    if (duiAction) {
        button->setIconID(duiAction->iconID());
    }
    QObject::connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));
    QObject::connect(button, SIGNAL(clicked(bool)), q, SLOT(_q_groupButtonClicked(bool)));
    QObject::connect(action, SIGNAL(toggled(bool)), q, SLOT(_q_groupActionToggled(bool)));

    button->setViewType("toolbartab");
    button->setCheckable(true);
    button->setChecked(action->isChecked());
    buttonGroup->addButton(button);

    return button;
}

bool DuiToolbarTabViewPrivate::isLocationValid(QAction *action, DuiAction::Location loc)
{
    bool valid = true; //any QAction is valid to place on toolbar
    DuiAction *duiAction = qobject_cast<DuiAction *>(action);
    if (duiAction) {
        valid = duiAction->location().testFlag(loc);
    }
    return valid;
}

bool DuiToolbarTabViewPrivate::isVisible(QAction *action)
{
    return action &&
           action->isVisible();
}

void DuiToolbarTabViewPrivate::updateActionChecked()
{
    Q_Q(DuiToolbarTabView);

    controller->removeEventFilter(this);

    QHashIterator<QAction *, DuiWidget *> iterator(tabButtons);
    while (iterator.hasNext()) {
        iterator.next();
        DuiButton *button = qobject_cast<DuiButton *>(iterator.value());
        QAction* action = iterator.key();
        bool butChecked = button->isChecked();
        if (button && butChecked != action->isChecked()) {
            QObject::disconnect(action, SIGNAL(toggled(bool)), q, SLOT(_q_groupActionToggled(bool)));
            action->setChecked(butChecked);
            QObject::connect(action, SIGNAL(toggled(bool)), q, SLOT(_q_groupActionToggled(bool)));
        }
    }

    controller->installEventFilter(this);
}

void DuiToolbarTabViewPrivate::clearWidgets(QHash<QAction *, DuiWidget *>& widgets)
{
    qDeleteAll(widgets);
    widgets.clear();
}

void DuiToolbarTabViewPrivate::removeAction(DuiLinearLayoutPolicy *policy,
        ActionPlacementData &policyData,
        QAction *action)
{
    DuiWidget *widget = tabButtons.value(action);
    removeAction(policy, policyData, action, widget);
}

void DuiToolbarTabViewPrivate::removeAction(DuiLinearLayoutPolicy *policy,
        ActionPlacementData &policyData,
        QAction *action,
        DuiWidget *widget)
{
    Q_UNUSED(action);
    int index = policy->indexOf(widget);
    if (index >= 0) {
        policyData.placedActions--;
        policy->removeAt(index);
    }
}

bool DuiToolbarTabViewPrivate::hasAction(QAction *action)
{
    return (tabButtons.contains(action));
}

int DuiToolbarTabViewPrivate::getItemIndex(DuiLinearLayoutPolicy *policy, QAction *before)
{
    int index = policy->count();
    DuiWidget *w = getWidget(before);
    if (w) {
        int beforeIndex = -1;
        if ((beforeIndex = policy->indexOf(w)) >= 0) {
            index = beforeIndex;
        }
    }
    return index;
}

DuiWidget *DuiToolbarTabViewPrivate::getWidget(QAction *action)
{
    return tabButtons.value(action);
}

bool DuiToolbarTabViewPrivate::changedLocation(QAction *action)
{
    // If the location of an action gets changed, then remove it from the toolbar
    bool validInLandscape = isLocationValid(action, DuiAction::ToolBarLandscapeLocation);
    bool validInPortrait = isLocationValid(action, DuiAction::ToolBarPortraitLocation);
    if (!validInLandscape && !validInPortrait) {
        remove(action, false);
    }

    return true;
}

void DuiToolbarTabViewPrivate::changedData(QAction *action)
{
    DuiWidget *widget = tabButtons.value(action);
    DuiButton *button = qobject_cast<DuiButton *>(widget);
    if (button) {
        // Update button data accordingly
        button->setEnabled(action->isEnabled());
        button->setCheckable(action->isCheckable());
        button->setChecked(action->isChecked());
        DuiAction *duiAction = qobject_cast<DuiAction *>(action);
        if (duiAction) {
            button->setIconID(duiAction->iconID());
        }
    }
}

bool DuiToolbarTabViewPrivate::changedVisibility(QAction *action)
{
    DuiWidget *widget = getWidget(action);
    if (widget) {
        bool wasVisible = (landscapePolicy->indexOf(widget) >= 0) ||
                          (portraitPolicy->indexOf(widget) >= 0);
        //Check if visibility has been changed
        return (!action->isVisible() && wasVisible) ||
               (action->isVisible() && !wasVisible);
    }
    return false;
}

void DuiToolbarTabViewPrivate::clearPolicy(DuiLinearLayoutPolicy *policy,
                               ActionPlacementData &policyData)
{
    while (policy->count()) {
        policy->removeAt(0);
    }
    policyData.reset();
}

void DuiToolbarTabViewPrivate::refreshSpacers()
{
    retrieveSpacers(landscapePolicy);
    retrieveSpacers(portraitPolicy);

    insertSpacers(landscapePolicy);
    insertSpacers(portraitPolicy);
}

void DuiToolbarTabViewPrivate::retrieveSpacers(DuiLinearLayoutPolicy *policy)
{
    QGraphicsLayoutItem *item = 0;
    int count = policy->count();
    for (int i = count-1; i >= 0; --i) {
        item = policy->itemAt(i);
        if (isItemSpacer(item)) {
            policy->removeAt(i);
            freeSpacers.append((QGraphicsWidget *)item);
        }
    }
}

void DuiToolbarTabViewPrivate::insertSpacers(DuiLinearLayoutPolicy *policy)
{
    // Add spacer(s) only if there are some/ actions in the policy
    int count = policy->count();
    if (count <= 0)
        return;

    // In landscape, spacer need to be added for right alignment
    if (policy == landscapePolicy) {
        if (count < maxWidgets)
            insertSpacer(policy, 0);
        return;
    }

    // In portrait, spacer(s) need to be added if portrait actions
    // are less then max actions OR in case of maximum actions,
    // no spacers in the start and in the end
    int spacersCount = count + 1;
    for (int i = 0; i < spacersCount; ++i) {
        if (count < maxWidgets) {
            insertSpacer(policy, i << 1); //indices are multiples of 2 i.e 0,2,4,6(if max=4)
        } else if ((i >= 0) && (i < (count - 1))) {
            insertSpacer(policy, (i << 1) + 1); //indices are 1,3,5 (if max=4)
        }
    }

}

void DuiToolbarTabViewPrivate::insertSpacer(DuiLinearLayoutPolicy *policy,
        int insertIndex)
{
    QGraphicsWidget *item = 0;
    if (freeSpacers.count() > 0) { //use existing spacer, if any
        item = freeSpacers.at(0);
        freeSpacers.removeAt(0);
    } else {
        item = createSpacer();
    }
    policy->insertItem(insertIndex, item);
}

QGraphicsWidget *DuiToolbarTabViewPrivate::createSpacer()
{
    QGraphicsWidget *spacer = new QGraphicsWidget(widgetsContainer);
    spacer->setMinimumSize(0, 0);
    spacer->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    return spacer;
}

bool DuiToolbarTabViewPrivate::isItemSpacer(QGraphicsLayoutItem *item)
{
    // Since spacers are QGraphicsWidget object and if casting of widget
    // to DuiWidget fails, then its a policy specific spacer.
    QGraphicsWidget *widget = (QGraphicsWidget *)item;
    DuiWidget *duiWidget = qobject_cast<DuiWidget *>(widget);
    return (!duiWidget && (widget->parentItem() == widgetsContainer));
}

bool DuiToolbarTabViewPrivate::refreshPolicyData(QAction *action, DuiAction::Location location, ActionPlacementData &policyData)
{
    bool add = false;
    if ((policyData.placedActions < maxWidgets) && isLocationValid(action, location)) {
        policyData.placedActions++;
        add = true;
    }
    return add;
}

DuiToolbarTabView::DuiToolbarTabView(DuiToolBar *controller) :
    DuiWidgetView(* new DuiToolbarTabViewPrivate(controller), controller)
{
    Q_D(DuiToolbarTabView);
    d->init();
}

DuiToolbarTabView::DuiToolbarTabView(DuiToolbarTabViewPrivate &dd, DuiToolBar *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiToolbarTabView);
    d->init();
}

DuiToolbarTabView::~DuiToolbarTabView()
{
}

// bind view and controller together
DUI_REGISTER_VIEW_NEW(DuiToolbarTabView, DuiToolBar)

#include "moc_duitoolbartabview.cpp"
