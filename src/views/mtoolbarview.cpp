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

#include "mtoolbar.h"

#include "mtheme.h"
#include "mbutton.h"
#include "mviewcreator.h"
#include "mwidgetaction.h"
#include "private/mwidgetview_p.h"
#include "mapplication.h"
#include "mapplicationwindow.h"

#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mtextedit.h"
#include "mtoolbarview.h"
#include "mtoolbarview_p.h"

const int MToolBarViewPrivate::maxWidgets = 4;

MToolBarViewPrivate::MToolBarViewPrivate(MToolBar *controller)
    : QObject(),
      q_ptr(0),
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


MToolBarViewPrivate::~MToolBarViewPrivate()
{
    clearWidgets(leasedWidgets);
    clearWidgets(buttons);
    removeEventFilter(controller);
    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->removeItem(widgetsContainer);
    delete widgetsContainer;
}

void MToolBarViewPrivate::init()
{
    widgetsContainer = new MWidget();
    widgetsContainer->setObjectName("toolbarContainer");

    layout = new MLayout(widgetsContainer);
    layout->setAnimation(NULL);
    layout->setContentsMargins(0, 0, 0, 0);

    createPolicy(M::Landscape);
    createPolicy(M::Portrait);

    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->addItem(widgetsContainer);

    addActions();
}

void MToolBarViewPrivate::createPolicy(M::Orientation orientation)
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

void MToolBarViewPrivate::add(QAction *action, QAction *before, bool refreshSpacer)
{
    bool validLocation = (isLocationValid(action, MAction::ToolBarLandscapeLocation) ||
                          isLocationValid(action, MAction::ToolBarPortraitLocation));
    if (!action || !validLocation ||
            (hasWidget(action) && !isWidgetUsable(action))) {
        return;
    }

    bool added = false;
    MWidget *w = createWidget(action);
    // add to policies only if the action is visible
    if (action->isVisible()) {
        bool addToLandscape = refreshPolicyData(action,
                                                MAction::ToolBarLandscapeLocation,
                                                landscapeData);
        bool addToPortrait  = refreshPolicyData(action,
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

void MToolBarViewPrivate::remove(QAction *action, bool refreshPolicies)
{
    MWidget *button = buttons.value(action);
    MWidget *leased = leasedWidgets.value(action);
    MWidget *widget = (button != 0) ? button : leased;

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

void MToolBarViewPrivate::change(QAction *action)
{
    if (changeLocation(action) || changeVisibility(action)) {
        clearPolicy(landscapePolicy, landscapeData);
        clearPolicy(portraitPolicy, portraitData);
        addActions();
    }
    changeData(action);
}

bool MToolBarViewPrivate::eventFilter(QObject *obj, QEvent *e)
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

void MToolBarViewPrivate::addActions()
{
    QList<QAction *> acts = controller->actions();
    int count = acts.count();
    for (int i = 0; i < count; ++i) {
        add(acts.at(i), 0, false);
    }
    refreshSpacers();
}

MWidget *MToolBarViewPrivate::createWidget(QAction *action)
{
    // If widget is not already created then create it
    MWidget *widget = buttons.value(action);
    if (!widget) {
        widget = leasedWidgets.value(action);
    }
    if (!widget) {
        MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
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

MButton *MToolBarViewPrivate::createButton(QAction *action)
{
    MButton *button = new MButton(action->text());
    MAction *mAction = qobject_cast<MAction *>(action);
    if (mAction) {
        button->setIconID(mAction->iconID());
    }
    connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));
    button->setViewType("toolbar");
    return button;
}

bool MToolBarViewPrivate::isLocationValid(QAction *action, MAction::Location loc)
{
    bool valid = true; //any QAction is valid to place on toolbar
    MAction *mAction = qobject_cast<MAction *>(action);
    if (mAction) {
        valid = mAction->location().testFlag(loc);
    }
    return valid;
}

bool MToolBarViewPrivate::isVisible(QAction *action)
{
    return action &&
           action->isVisible();
}

void MToolBarViewPrivate::clearWidgets(QHash<QAction *, MWidget *>& widgets)
{
    QHashIterator<QAction *, MWidget *> iterator(widgets);
    while (iterator.hasNext()) {
        iterator.next();
        deleteWidget(iterator.key(), iterator.value());
    }
    widgets.clear();
}

void MToolBarViewPrivate::deleteWidget(QAction *action, MWidget *widget)
{
    if (!releaseWidget(action, widget)) {
        delete widget;
    }
}

bool MToolBarViewPrivate::releaseWidget(QAction *action, MWidget *widget)
{
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction) {
        widgetAction->releaseWidget(widget);
    }
    return (widgetAction != 0);
}

MWidget *MToolBarViewPrivate::requestWidget(MAction *action)
{
    MWidget *widget = 0;
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction) {
        widget = widgetAction->requestWidget(widgetsContainer);
    }
    return widget;
}

bool MToolBarViewPrivate::isWidgetInUseByView(MWidgetAction *widgetAction)
{
    return (buttons.contains(widgetAction) || leasedWidgets.contains(widgetAction));
}

bool MToolBarViewPrivate::isWidgetUsable(QAction *action)
{
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    return(widgetAction && isWidgetUsable(widgetAction));
}

bool MToolBarViewPrivate::hasWidget(QAction *action)
{
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    return(widgetAction && widgetAction->widget());
}

bool MToolBarViewPrivate::hasTextEditWidget(QAction *action)
{
    MTextEdit *textEditWidget = 0;
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction) {
        textEditWidget = qobject_cast<MTextEdit *>(widgetAction->widget());
    }
    return (textEditWidget != 0);
}

void MToolBarViewPrivate::removeAction(MLinearLayoutPolicy *policy,
        ActionPlacementData &policyData,
        QAction *action)
{
    MWidget *button = buttons.value(action);
    MWidget *leased = leasedWidgets.value(action);
    MWidget *widget = (button != 0) ? button : leased;

    removeAction(policy, policyData, action, widget);
}

void MToolBarViewPrivate::removeAction(MLinearLayoutPolicy *policy,
        ActionPlacementData &policyData,
        QAction *action,
        MWidget *widget)
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

bool MToolBarViewPrivate::isWidgetUsable(MWidgetAction *widgetAction)
{
    return (!widgetAction->isWidgetInUse() || isWidgetInUseByView(widgetAction));
}

bool MToolBarViewPrivate::hasAction(QAction *action)
{
    return (buttons.contains(action) || leasedWidgets.contains(action));
}

int MToolBarViewPrivate::getItemIndex(MLinearLayoutPolicy *policy, QAction *before)
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

MWidget *MToolBarViewPrivate::getWidget(QAction *action)
{
    MWidget *button = buttons.value(action);
    MWidget *leased = leasedWidgets.value(action);
    return (button != 0) ? button : leased;
}

bool MToolBarViewPrivate::changeLocation(QAction *action)
{
    // If the location of an action gets changed, then remove it from the toolbar
    bool validInLandscape = isLocationValid(action, MAction::ToolBarLandscapeLocation);
    bool validInPortrait = isLocationValid(action, MAction::ToolBarPortraitLocation);
    if (!validInLandscape && !validInPortrait) {
        remove(action, false);
    }

    return true;
}

void MToolBarViewPrivate::changeData(QAction *action)
{
    MWidget *widget = buttons.value(action);
    MButton *button = qobject_cast<MButton *>(widget);
    if (button) {
        // Update button data accordingly
        button->setText(action->text());
        button->setEnabled(action->isEnabled());
        button->setCheckable(action->isCheckable());
        button->setChecked(action->isChecked());
        MAction *mAction = qobject_cast<MAction *>(action);
        if (mAction) {
            button->setIconID(mAction->iconID());
        }
    }
}

bool MToolBarViewPrivate::changeVisibility(QAction *action)
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

void MToolBarViewPrivate::clearPolicy(MLinearLayoutPolicy *policy,
                                        ActionPlacementData &policyData)
{
    while (policy->count()) {
        policy->removeAt(0);
    }
    policyData.reset();
}

void MToolBarViewPrivate::refreshSpacers()
{
    retrieveSpacers(landscapePolicy, landscapeData);
    retrieveSpacers(portraitPolicy, portraitData);

    insertSpacers(landscapePolicy, landscapeData);
    insertSpacers(portraitPolicy, portraitData);
}

void MToolBarViewPrivate::retrieveSpacers(MLinearLayoutPolicy *policy,
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
            MWidget *widget = (MWidget *)item;
            QAction *action = leasedWidgets.key(widget);
            if (action && !hasTextEditWidget(action)) {
                policyData.mode = Unmanaged;
            }
        }
    }
}

void MToolBarViewPrivate::insertSpacers(MLinearLayoutPolicy *policy,
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

void MToolBarViewPrivate::insertSpacer(MLinearLayoutPolicy *policy,
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

QGraphicsWidget *MToolBarViewPrivate::createSpacer()
{
    QGraphicsWidget *spacer = new QGraphicsWidget(widgetsContainer);
    spacer->setMinimumSize(0, 0);
    spacer->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    return spacer;
}

bool MToolBarViewPrivate::isItemSpacer(QGraphicsLayoutItem *item)
{
    // Since spacers are QGraphicsWidget object and if casting of widget
    // to MWidget fails, then its a policy specific spacer.
    QGraphicsWidget *widget = (QGraphicsWidget *)item;
    MWidget *mWidget = qobject_cast<MWidget *>(widget);
    return (!mWidget && (widget->parentItem() == widgetsContainer));
}

bool MToolBarViewPrivate::refreshPolicyData(QAction *action,
        MAction::Location location,
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

MToolBarView::MToolBarView(MToolBar *controller) :
    MWidgetView(controller),
    d_ptr(new MToolBarViewPrivate(controller))
{
    Q_D(MToolBarView);
    d->q_ptr = this;
    d->init();
}

MToolBarView::MToolBarView(MToolBarViewPrivate &dd, MToolBar *controller) :
    MWidgetView(controller),
    d_ptr(&dd)
{
    Q_D(MToolBarView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MToolBarView::~MToolBarView()
{
    delete d_ptr;
}

// bind view and controller together
M_REGISTER_VIEW_NEW(MToolBarView, MToolBar)

#include "moc_mtoolbarview.cpp"
