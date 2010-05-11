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

#include <QGraphicsLinearLayout>
#include <QActionGroup>

#include "mtoolbar.h"
#include "mtheme.h"
#include "mbutton.h"
#include "mviewcreator.h"
#include "mwidgetaction.h"
#include "private/mwidgetview_p.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mbuttongroup.h"

#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mtextedit.h"
#include "mtoolbartabview.h"
#include "mtoolbartabview_p.h"


const int MToolbarTabViewPrivate::maxWidgets = 4;

MToolbarTabViewPrivate::MToolbarTabViewPrivate(MToolBar *controller)
    : QObject(),
      q_ptr(0),
      widgetsContainer(0),
      layout(0),
      landscapePolicy(0),
      portraitPolicy(0),
      tabButtons()
{
    this->controller = controller;
    controller->installEventFilter(this);
}


MToolbarTabViewPrivate::~MToolbarTabViewPrivate()
{
    clearWidgets(tabButtons);
    controller->removeEventFilter(this);
    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->removeItem(widgetsContainer);
    delete widgetsContainer;
}

void MToolbarTabViewPrivate::init()
{
    widgetsContainer = new MWidget();
    widgetsContainer->setObjectName("toolbarTabContainer");

    layout = new MLayout(widgetsContainer);
    layout->setContentsMargins(0, 0, 0, 0);

    createPolicy(M::Landscape);
    createPolicy(M::Portrait);

    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->addItem(widgetsContainer);

    buttonGroup = new MButtonGroup(controller);
    buttonGroup->setExclusive(true);

    addActions();
}

void MToolbarTabViewPrivate::createPolicy(M::Orientation orientation)
{
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    policy->setSpacing(0);
    policy->setContentsMargins(0, 0, 0, 0);
    if (orientation == M::Landscape) {
        landscapePolicy = policy;
        layout->setLandscapePolicy(landscapePolicy);
        landscapeData.reset();
    } else {
        portraitPolicy = policy;
        layout->setPortraitPolicy(portraitPolicy);
        portraitData.reset();
    }
}

void MToolbarTabViewPrivate::add(QAction *action, QAction *before, bool refreshSpacer)
{
    bool validLocation = (isLocationValid(action, MAction::ToolBarLandscapeLocation) ||
                          isLocationValid(action, MAction::ToolBarPortraitLocation));
    if (!action || !validLocation || !action->isCheckable()) {
        return;
    }

    bool added = false;
    MWidget *w = createWidget(action);
    // add to policies only if the action is visible
    if (action->isVisible()) {
        bool addToLandscape = refreshPolicyData(action,
                                        MAction::ToolBarLandscapeLocation,
                                        landscapeData);
        bool addToPortrait = refreshPolicyData(action,
                                      MAction::ToolBarPortraitLocation,
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

void MToolbarTabViewPrivate::remove(QAction *action, bool refreshPolicies)
{
    Q_Q(MToolbarTabView);
    MButton *widget = (MButton*)tabButtons.value(action);

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

void MToolbarTabViewPrivate::change(QAction *action)
{
    if (changedLocation(action) || changedVisibility(action)) {
        clearPolicy(landscapePolicy, landscapeData);
        clearPolicy(portraitPolicy, portraitData);
        MButton *widget = (MButton*)tabButtons.value(action);
        buttonGroup->removeButton(widget);
        addActions();
    }
    changedData(action);
}

void MToolbarTabViewPrivate::_q_groupButtonClicked(bool)
{
    Q_Q(MToolbarTabView);
    MButton *button = qobject_cast<MButton *>(q->sender());
    if (button) {
        button->setChecked(true);
    }
    updateActionChecked();
}

void MToolbarTabViewPrivate::_q_groupActionToggled(bool checked)
{
    Q_Q(MToolbarTabView);
    QAction* action = qobject_cast<QAction *>(q->sender());
    MButton *button = qobject_cast<MButton *>(tabButtons.value(action));
    if (button) {
        button->setChecked(checked);
    }
    updateActionChecked();
}

bool MToolbarTabViewPrivate::eventFilter(QObject *obj, QEvent *e)
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

void MToolbarTabViewPrivate::addActions()
{
    QList<QAction *> acts = controller->actions();
    int count = acts.count();
    for (int i = 0; i < count; ++i) {
        add(acts.at(i), 0, false);
    }
    refreshSpacers();
    updateActionChecked();
}

MWidget *MToolbarTabViewPrivate::createWidget(QAction *action)
{
    // If widget is not already created then create it
    MWidget *widget = tabButtons.value(action);
    if (!widget) {
        widget = createTabButton(action);
        tabButtons.insert(action, widget);
    }
    widget->setVisible(true);
    widget->setEnabled(action->isEnabled());
    buttonGroup->addButton((MButton *)widget);

    return widget;
}

MButton *MToolbarTabViewPrivate::createTabButton(QAction *action)
{
    Q_Q(MToolbarTabView);

    MButton *button = new MButton();
    MAction *mAction = qobject_cast<MAction *>(action);
    if (mAction) {
        button->setIconID(mAction->iconID());
    }
    QObject::connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));
    QObject::connect(button, SIGNAL(clicked(bool)), q, SLOT(_q_groupButtonClicked(bool)));
    QObject::connect(action, SIGNAL(toggled(bool)), q, SLOT(_q_groupActionToggled(bool)));

    button->setViewType("toolbartab");
    button->setCheckable(true);
    button->setChecked(action->isChecked());

    return button;
}

bool MToolbarTabViewPrivate::isLocationValid(QAction *action, MAction::Location loc)
{
    bool valid = true; //any QAction is valid to place on toolbar
    MAction *mAction = qobject_cast<MAction *>(action);
    if (mAction) {
        valid = mAction->location().testFlag(loc);
    }
    return valid;
}

bool MToolbarTabViewPrivate::isVisible(QAction *action)
{
    return action &&
           action->isVisible();
}

void MToolbarTabViewPrivate::updateActionChecked()
{
    Q_Q(MToolbarTabView);

    controller->removeEventFilter(this);

    // remove invisible actions from button group as this can update the checked button
    QHashIterator<QAction *, MWidget *> iterator(tabButtons);
    while (iterator.hasNext()) {
        iterator.next();
        MButton *button = qobject_cast<MButton *>(iterator.value());
        QAction* action = iterator.key();
        if (!action->isVisible()) {
            buttonGroup->removeButton(button);
            button->setChecked(false);
        }
    }

    iterator.toFront();
    while (iterator.hasNext()) {
        iterator.next();
        MButton *button = qobject_cast<MButton *>(iterator.value());
        QAction* action = iterator.key();
        if (button && (button->isChecked() != action->isChecked())) {
            QObject::disconnect(action, SIGNAL(toggled(bool)), q, SLOT(_q_groupActionToggled(bool)));
            action->setChecked(button->isChecked());
            QObject::connect(action, SIGNAL(toggled(bool)), q, SLOT(_q_groupActionToggled(bool)));
        }
    }

    controller->installEventFilter(this);
}

void MToolbarTabViewPrivate::clearWidgets(QHash<QAction *, MWidget *>& widgets)
{
    qDeleteAll(widgets);
    widgets.clear();
}

void MToolbarTabViewPrivate::removeAction(MLinearLayoutPolicy *policy,
        ActionPlacementData &policyData,
        QAction *action)
{
    MWidget *widget = tabButtons.value(action);
    removeAction(policy, policyData, action, widget);
}

void MToolbarTabViewPrivate::removeAction(MLinearLayoutPolicy *policy,
        ActionPlacementData &policyData,
        QAction *action,
        MWidget *widget)
{
    Q_UNUSED(action);
    int index = policy->indexOf(widget);
    if (index >= 0) {
        policyData.placedActions--;
        policy->removeAt(index);
    }
}

bool MToolbarTabViewPrivate::hasAction(QAction *action)
{
    return (tabButtons.contains(action));
}

int MToolbarTabViewPrivate::getItemIndex(MLinearLayoutPolicy *policy, QAction *before)
{
    int index = policy->count();
    MWidget *w = getWidget(before);
    if (w) {
        int beforeIndex = -1;
        if ((beforeIndex = policy->indexOf(w)) >= 0) {
            index = beforeIndex;
        }
    }
    return index;
}

MWidget *MToolbarTabViewPrivate::getWidget(QAction *action)
{
    return tabButtons.value(action);
}

bool MToolbarTabViewPrivate::changedLocation(QAction *action)
{
    // If the location of an action gets changed, then remove it from the toolbar
    bool validInLandscape = isLocationValid(action, MAction::ToolBarLandscapeLocation);
    bool validInPortrait = isLocationValid(action, MAction::ToolBarPortraitLocation);
    if (!validInLandscape && !validInPortrait) {
        remove(action, false);
    }

    return true;
}

void MToolbarTabViewPrivate::changedData(QAction *action)
{
    MWidget *widget = tabButtons.value(action);
    MButton *button = qobject_cast<MButton *>(widget);
    if (button) {
        // Update button data accordingly
        button->setEnabled(action->isEnabled());
        button->setCheckable(action->isCheckable());
        button->setChecked(action->isChecked());
        MAction *mAction = qobject_cast<MAction *>(action);
        if (mAction) {
            button->setIconID(mAction->iconID());
        }
    }
}

bool MToolbarTabViewPrivate::changedVisibility(QAction *action)
{
    MWidget *widget = getWidget(action);
    if (widget) {
        bool wasVisible = (landscapePolicy->indexOf(widget) >= 0) ||
                          (portraitPolicy->indexOf(widget) >= 0);
        //Check if visibility has been changed
        return (!action->isVisible() && wasVisible) ||
               (action->isVisible() && !wasVisible);
    }
    return false;
}

void MToolbarTabViewPrivate::clearPolicy(MLinearLayoutPolicy *policy,
                               ActionPlacementData &policyData)
{
    while (policy->count()) {
        policy->removeAt(0);
    }
    policyData.reset();
}

void MToolbarTabViewPrivate::refreshSpacers()
{
    retrieveSpacers(landscapePolicy);
    retrieveSpacers(portraitPolicy);

    insertSpacers(landscapePolicy);
    insertSpacers(portraitPolicy);
}

void MToolbarTabViewPrivate::retrieveSpacers(MLinearLayoutPolicy *policy)
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

void MToolbarTabViewPrivate::insertSpacers(MLinearLayoutPolicy *policy)
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

void MToolbarTabViewPrivate::insertSpacer(MLinearLayoutPolicy *policy,
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

QGraphicsWidget *MToolbarTabViewPrivate::createSpacer()
{
    QGraphicsWidget *spacer = new QGraphicsWidget(widgetsContainer);
    spacer->setMinimumSize(0, 0);
    spacer->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    return spacer;
}

bool MToolbarTabViewPrivate::isItemSpacer(QGraphicsLayoutItem *item)
{
    // Since spacers are QGraphicsWidget object and if casting of widget
    // to MWidget fails, then its a policy specific spacer.
    QGraphicsWidget *widget = (QGraphicsWidget *)item;
    MWidget *mWidget = qobject_cast<MWidget *>(widget);
    return (!mWidget && (widget->parentItem() == widgetsContainer));
}

bool MToolbarTabViewPrivate::refreshPolicyData(QAction *action, MAction::Location location, ActionPlacementData &policyData)
{
    bool add = false;
    if ((policyData.placedActions < maxWidgets) && isLocationValid(action, location)) {
        policyData.placedActions++;
        add = true;
    }
    return add;
}

MToolbarTabView::MToolbarTabView(MToolBar *controller) :
    MWidgetView(controller),
    d_ptr(new MToolbarTabViewPrivate(controller))
{
    Q_D(MToolbarTabView);
    d->q_ptr = this;
    d->init();
}

MToolbarTabView::MToolbarTabView(MToolbarTabViewPrivate &dd, MToolBar *controller) :
    MWidgetView(controller),
    d_ptr(&dd)
{
    Q_D(MToolbarTabView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MToolbarTabView::~MToolbarTabView()
{
    delete d_ptr;
}

// bind view and controller together
M_REGISTER_VIEW_NEW(MToolbarTabView, MToolBar)

#include "moc_mtoolbartabview.cpp"
