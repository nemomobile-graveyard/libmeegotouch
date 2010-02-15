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

#include "duitoolbar.h"
#include "duitoolbar_p.h"

#include "duitheme.h"
#include "duibutton.h"
#include "duiviewcreator.h"
#include "duiwidgetaction.h"
#include "private/duiwidgetview_p.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"

#include "duilayout.h"
#include "duilinearlayoutpolicy.h"
#include "duitextedit.h"
#include "duitoolbarview.h"
#include "duitoolbarview_p.h"

const int DuiToolBarViewPrivate::maxWidgets = 4;

DuiToolBarViewPrivate::DuiToolBarViewPrivate(DuiToolBar *controller)
    : DuiWidgetViewPrivate(),
      QObject(),
      widgetsContainer(0),
      layout(0),
      landscapePolicy(0),
      portraitPolicy(0),
      leasedWidgets(),
      buttons()
{
    this->controller = controller;
    controller->installEventFilter(this);
}


DuiToolBarViewPrivate::~DuiToolBarViewPrivate()
{
    clearWidgets(leasedWidgets);
    clearWidgets(buttons);
    removeEventFilter(controller);
}

void DuiToolBarViewPrivate::init()
{
    widgetsContainer = new DuiWidget();
    widgetsContainer->setObjectName("toolbarContainer");

    layout = new DuiLayout(widgetsContainer);
    layout->setAnimation(NULL);
    layout->setContentsMargins(0, 0, 0, 0);

    createPolicy(Dui::Landscape);
    createPolicy(Dui::Portrait);

    QGraphicsLinearLayout *layout = (QGraphicsLinearLayout *)(controller->layout());
    layout->addItem(widgetsContainer);

    addActions();
}

void DuiToolBarViewPrivate::createPolicy(Dui::Orientation orientation)
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

void DuiToolBarViewPrivate::add(QAction *action, QAction *before, bool refreshSpacer)
{
    bool validLocation = (isLocationValid(action, DuiAction::ToolBarLandscapeLocation) ||
                          isLocationValid(action, DuiAction::ToolBarPortraitLocation));
    if (!action || !validLocation ||
            (hasWidget(action) && !isWidgetUsable(action))) {
        return;
    }

    bool added = false;
    DuiWidget *w = createWidget(action);
    // add to policies only if the action is visible
    if (action->isVisible()) {
        bool addToLandscape = refreshPolicyData(action,
                                                DuiAction::ToolBarLandscapeLocation,
                                                landscapeData);
        bool addToPortrait  = refreshPolicyData(action,
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

void DuiToolBarViewPrivate::remove(QAction *action, bool refreshPolicies)
{
    DuiWidget *button = buttons.value(action);
    DuiWidget *leased = leasedWidgets.value(action);
    DuiWidget *widget = (button != 0) ? button : leased;

    if (widget) {
        removeAction(landscapePolicy, landscapeData, action, widget);
        removeAction(portraitPolicy, portraitData, action, widget);
        layout->removeItem(widget);
    }

    if (button) {
        buttons.remove(action);
        delete button;
    } else if (leased) {
        releaseWidget(action, leased);
        leasedWidgets.remove(action);
    }
    if (refreshPolicies) {
        clearPolicy(landscapePolicy, landscapeData);
        clearPolicy(portraitPolicy, portraitData);
        addActions();
    }
}

void DuiToolBarViewPrivate::change(QAction *action)
{
    if (changeLocation(action) || changeVisibility(action)) {
        clearPolicy(landscapePolicy, landscapeData);
        clearPolicy(portraitPolicy, portraitData);
        addActions();
    }
    changeData(action);
}

bool DuiToolBarViewPrivate::eventFilter(QObject *obj, QEvent *e)
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
    }

    return QObject::eventFilter(obj, e);
}

void DuiToolBarViewPrivate::addActions()
{
    QList<QAction *> acts = controller->actions();
    int count = acts.count();
    for (int i = 0; i < count; ++i) {
        add(acts.at(i), 0, false);
    }
    refreshSpacers();
}

DuiWidget *DuiToolBarViewPrivate::createWidget(QAction *action)
{
    // If widget is not already created then create it
    DuiWidget *widget = buttons.value(action);
    if (!widget) {
        widget = leasedWidgets.value(action);
    }
    if (!widget) {
        DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
        if (widgetAction) {
            widget = requestWidget(widgetAction);
            leasedWidgets.insert(action, widget);
        } else {
            widget = createButton(action);
            buttons.insert(action, widget);
        }
    }
    widget->setVisible(true);
    widget->setEnabled(action->isEnabled());
    return widget;
}

DuiButton *DuiToolBarViewPrivate::createButton(QAction *action)
{
    DuiButton *button = new DuiButton(action->text());
    DuiAction *duiAction = qobject_cast<DuiAction *>(action);
    if (duiAction) {
        button->setIconID(duiAction->iconID());
    }
    connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));
    button->setViewType("toolbar");
    return button;
}

bool DuiToolBarViewPrivate::isLocationValid(QAction *action, DuiAction::Location loc)
{
    bool valid = true; //any QAction is valid to place on toolbar
    DuiAction *duiAction = qobject_cast<DuiAction *>(action);
    if (duiAction) {
        valid = duiAction->location().testFlag(loc);
    }
    return valid;
}

bool DuiToolBarViewPrivate::isVisible(QAction *action)
{
    return action &&
           action->isVisible();
}

void DuiToolBarViewPrivate::clearWidgets(QHash<QAction *, DuiWidget *>& widgets)
{
    QHashIterator<QAction *, DuiWidget *> iterator(widgets);
    while (iterator.hasNext()) {
        iterator.next();
        deleteWidget(iterator.key(), iterator.value());
    }
    widgets.clear();
}

void DuiToolBarViewPrivate::deleteWidget(QAction *action, DuiWidget *widget)
{
    if (!releaseWidget(action, widget)) {
        delete widget;
    }
}

bool DuiToolBarViewPrivate::releaseWidget(QAction *action, DuiWidget *widget)
{
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
    if (widgetAction) {
        widgetAction->releaseWidget(widget);
    }
    return (widgetAction != 0);
}

DuiWidget *DuiToolBarViewPrivate::requestWidget(DuiAction *action)
{
    DuiWidget *widget = 0;
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
    if (widgetAction) {
        widget = widgetAction->requestWidget(widgetsContainer);
    }
    return widget;
}

bool DuiToolBarViewPrivate::isWidgetInUseByView(DuiWidgetAction *widgetAction)
{
    return (buttons.contains(widgetAction) || leasedWidgets.contains(widgetAction));
}

bool DuiToolBarViewPrivate::isWidgetUsable(QAction *action)
{
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
    return(widgetAction && isWidgetUsable(widgetAction));
}

bool DuiToolBarViewPrivate::hasWidget(QAction *action)
{
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
    return(widgetAction && widgetAction->widget());
}

bool DuiToolBarViewPrivate::hasTextEditWidget(QAction *action)
{
    DuiTextEdit *textEditWidget = 0;
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
    if (widgetAction) {
        textEditWidget = qobject_cast<DuiTextEdit *>(widgetAction->widget());
    }
    return (textEditWidget != 0);
}

void DuiToolBarViewPrivate::removeAction(DuiLinearLayoutPolicy *policy,
        ActionPlacementData &policyData,
        QAction *action)
{
    DuiWidget *button = buttons.value(action);
    DuiWidget *leased = leasedWidgets.value(action);
    DuiWidget *widget = (button != 0) ? button : leased;

    removeAction(policy, policyData, action, widget);
}

void DuiToolBarViewPrivate::removeAction(DuiLinearLayoutPolicy *policy,
        ActionPlacementData &policyData,
        QAction *action,
        DuiWidget *widget)
{
    bool hasTextEdit = hasTextEditWidget(action);
    int index = policy->indexOf(widget);
    if (index >= 0) {
        policyData.placedActions--;
        if (hasTextEdit) {
            policyData.hasTextEditor = false;
            policyData.placedActions--;
        }
        policy->removeAt(index);
    }
}

bool DuiToolBarViewPrivate::isWidgetUsable(DuiWidgetAction *widgetAction)
{
    return (!widgetAction->isWidgetInUse() || isWidgetInUseByView(widgetAction));
}

bool DuiToolBarViewPrivate::hasAction(QAction *action)
{
    return (buttons.contains(action) || leasedWidgets.contains(action));
}

int DuiToolBarViewPrivate::getItemIndex(DuiLinearLayoutPolicy *policy, QAction *before)
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

DuiWidget *DuiToolBarViewPrivate::getWidget(QAction *action)
{
    DuiWidget *button = buttons.value(action);
    DuiWidget *leased = leasedWidgets.value(action);
    return (button != 0) ? button : leased;
}

bool DuiToolBarViewPrivate::changeLocation(QAction *action)
{
    // If the location of an action gets changed, then remove it from the toolbar
    bool validInLandscape = isLocationValid(action, DuiAction::ToolBarLandscapeLocation);
    bool validInPortrait = isLocationValid(action, DuiAction::ToolBarPortraitLocation);
    if (!validInLandscape && !validInPortrait) {
        remove(action, false);
    }

    return true;
}

void DuiToolBarViewPrivate::changeData(QAction *action)
{
    DuiWidget *widget = buttons.value(action);
    DuiButton *button = qobject_cast<DuiButton *>(widget);
    if (button) {
        // Update button data accordingly
        button->setText(action->text());
        button->setEnabled(action->isEnabled());
        button->setCheckable(action->isCheckable());
        button->setChecked(action->isChecked());
        DuiAction *duiAction = qobject_cast<DuiAction *>(action);
        if (duiAction) {
            button->setIconID(duiAction->iconID());
        }
    }
}

bool DuiToolBarViewPrivate::changeVisibility(QAction *action)
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

void DuiToolBarViewPrivate::clearPolicy(DuiLinearLayoutPolicy *policy,
                                        ActionPlacementData &policyData)
{
    while (policy->count()) {
        policy->removeAt(0);
    }
    policyData.reset();
}

void DuiToolBarViewPrivate::refreshSpacers()
{
    retrieveSpacers(landscapePolicy, landscapeData);
    retrieveSpacers(portraitPolicy, portraitData);

    insertSpacers(landscapePolicy, landscapeData);
    insertSpacers(portraitPolicy, portraitData);
}

void DuiToolBarViewPrivate::retrieveSpacers(DuiLinearLayoutPolicy *policy,
        ActionPlacementData &policyData)
{
    policyData.mode = Managed;
    QGraphicsLayoutItem *item = 0;
    int count = policy->count();
    for (int i = count - 1; i >= 0; --i) {
        item = policy->itemAt(i);
        if (isItemSpacer(item)) {
            policy->removeAt(i);
            freeSpacers.append((QGraphicsWidget *)item);
        } else {
            DuiWidget *widget = (DuiWidget *)item;
            QAction *action = leasedWidgets.key(widget);
            if (action && !hasTextEditWidget(action)) {
                policyData.mode = Unmanaged;
            }
        }
    }
}

void DuiToolBarViewPrivate::insertSpacers(DuiLinearLayoutPolicy *policy,
        const ActionPlacementData &policyData)
{
    // Add spacer(s) only if there is no text-editor action placed or
    // the widgets are not placed in unmanaged way or there are some
    // actions in the policy
    int count = policy->count();
    if (policyData.mode == Unmanaged || policyData.hasTextEditor || count <= 0)
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

void DuiToolBarViewPrivate::insertSpacer(DuiLinearLayoutPolicy *policy,
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

QGraphicsWidget *DuiToolBarViewPrivate::createSpacer()
{
    QGraphicsWidget *spacer = new QGraphicsWidget(widgetsContainer);
    spacer->setMinimumSize(0, 0);
    spacer->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    return spacer;
}

bool DuiToolBarViewPrivate::isItemSpacer(QGraphicsLayoutItem *item)
{
    // Since spacers are QGraphicsWidget object and if casting of widget
    // to DuiWidget fails, then its a policy specific spacer.
    QGraphicsWidget *widget = (QGraphicsWidget *)item;
    DuiWidget *duiWidget = qobject_cast<DuiWidget *>(widget);
    return (!duiWidget && (widget->parentItem() == widgetsContainer));
}

bool DuiToolBarViewPrivate::refreshPolicyData(QAction *action,
        DuiAction::Location location,
        ActionPlacementData &policyData)
{
    bool add = false;
    if ((policyData.placedActions < maxWidgets) && isLocationValid(action, location)) {
        bool hasTextEdit = hasTextEditWidget(action);
        // The action is not added if it has a text-edit widget and the toolbar location
        // has already a text-edit widget placed or there is no space for new text-edit widget
        // as text-edit widget takes space of two buttons
        if (!hasTextEdit || (!policyData.hasTextEditor && (policyData.placedActions < maxWidgets - 1))) {
            add = true;
            if (hasTextEdit) {
                //one text-edit widget takes space of two buttons
                policyData.placedActions++;
                policyData.hasTextEditor = true;
            }
            // If there is a widget that is usable and is not text-entry widget, then the widgets
            // are placed as it is i.e. in unmanaged way.
            if (hasWidget(action) && !hasTextEdit) {
                policyData.mode = Unmanaged;
            }
        }
    }
    policyData.placedActions += add;
    return add;
}

DuiToolBarView::DuiToolBarView(DuiToolBar *controller) :
    DuiWidgetView(* new DuiToolBarViewPrivate(controller), controller)
{
    Q_D(DuiToolBarView);
    d->init();
}

DuiToolBarView::DuiToolBarView(DuiToolBarViewPrivate &dd, DuiToolBar *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiToolBarView);
    d->init();
}

DuiToolBarView::~DuiToolBarView()
{
}

// bind view and controller together
DUI_REGISTER_VIEW_NEW(DuiToolBarView, DuiToolBar)

#include "moc_duitoolbarview.cpp"
