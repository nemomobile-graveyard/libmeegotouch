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
}

void MToolBarViewPrivate::init()
{
    layout = new MLayout;
    layout->setAnimation(NULL);
    layout->setContentsMargins(0, 0, 0, 0);

    landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    landscapePolicy->setSpacing(0);
    layout->setLandscapePolicy(landscapePolicy);
    landscapeData.reset();

    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    portraitPolicy->setSpacing(0);
    layout->setPortraitPolicy(portraitPolicy);
    portraitData.reset();

    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->addStretch();
    controllerlayout->addItem(layout);

    //Add any existing actions now
    foreach(QAction *action, controller->actions()) {
        add(action, 0);
    }

}

void MToolBarViewPrivate::add(QAction *action, QAction *before)
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
            added = true;
        }
    }

    if (!added && w) {
        w->setVisible(false);
    }
}

void MToolBarViewPrivate::remove(QAction *action)
{
    MWidget *button = buttons.value(action);
    MWidget *leased = leasedWidgets.value(action);
    MWidget *widget = (button != 0) ? button : leased;

    if (widget) {
        bool isTextEditWidget = hasTextEditWidget(action);
        if(landscapePolicy->indexOf(widget) >= 0) {
            if(isTextEditWidget) {
                landscapeData.hasTextEditor = false;
                //one text-edit widget takes space of two buttons
                landscapeData.placedActions -= 2;
            } else
                landscapeData.placedActions--;
        }
        if(portraitPolicy->indexOf(widget) >= 0) {
            if(isTextEditWidget) {
                portraitData.hasTextEditor = false;
                //one text-edit widget takes space of two buttons
                portraitData.placedActions -= 2;
            } else
                portraitData.placedActions--;
        }

        layout->removeItem(widget);
    }

    if (button) {
        buttons.remove(action);
        delete button;
    } else if (leased) {
        releaseWidget(action, leased);
        leasedWidgets.remove(action);
    }
}

void MToolBarViewPrivate::change(QAction *action)
{
    changeData(action);
}

bool MToolBarViewPrivate::eventFilter(QObject *obj, QEvent *e)
{
    QActionEvent *actionEvent = dynamic_cast<QActionEvent *>(e);
    if (!controller->childItems().isEmpty()) {
        QList<QGraphicsItem *>children = controller->childItems();
    }

    if (actionEvent) {
        switch (e->type()) {
        case QEvent::ActionRemoved: {
            remove(actionEvent->action());
            break;
        }
        case QEvent::ActionAdded: {
            add(actionEvent->action(), actionEvent->before());
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

MWidget *MToolBarViewPrivate::createWidget(QAction *action)
{
    // If widget is not already created then create it
    MWidget *widget = buttons.value(action);
    if (!widget)
        widget = leasedWidgets.value(action);
    if (!widget) {
        MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
        if (widgetAction) {
            widget = requestWidget(widgetAction);
            leasedWidgets.insert(action, widget);
        } else {
            widget = createButton(action);
            buttons.insert(action, widget);
            Q_ASSERT(widget);
            Q_ASSERT(buttons.value(action) == widget);
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
        widget = widgetAction->requestWidget(NULL);
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
        remove(action);
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

bool MToolBarViewPrivate::refreshPolicyData(QAction *action,
        MAction::Location location,
        ActionPlacementData &policyData)
{
    if (!isLocationValid(action, location))
        return false;
    if(hasTextEditWidget(action)) {
        if (policyData.hasTextEditor || policyData.placedActions > maxWidgets)
            return false; //We can't add two text edits, and there must be two spaces available for a text edit
        policyData.hasTextEditor = true;
        //one text-edit widget takes space of two buttons
        policyData.placedActions += 2;
    } else {
        if (policyData.placedActions >= maxWidgets)
            return false; //no more room
        policyData.placedActions += 1;
    }
    return true;
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
