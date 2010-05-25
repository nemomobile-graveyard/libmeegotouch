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
#include "mbuttongroup.h"
#include "mwidgetaction.h"
#include "private/mwidgetview_p.h"

#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mtextedit.h"
#include "mtoolbarview.h"
#include "mtoolbarview_p.h"

const int MToolBarViewPrivate::maxWidgets = 4;

/* Make a layout policy for aligning widgets nicely by
 * adding spacers.
 * This is a very crude policy, and doesn't support most
 * of the functions of MLinearLayoutPolicy */
class ToolBarLayoutPolicy : public MLinearLayoutPolicy
{
public:
    explicit ToolBarLayoutPolicy(MLayout *layout) :
        MLinearLayoutPolicy(layout, Qt::Horizontal) {
            insertSpacer(0);
        }
    ~ToolBarLayoutPolicy() {
        for(int i = count()-1; i >= 0; i--)
            removeAt(i);
        QGraphicsLayoutItem *item = MLinearLayoutPolicy::itemAt(0);
        MLinearLayoutPolicy::removeAt(0);
        delete item;
    }
    virtual void insertItem(int index, QGraphicsLayoutItem *item)
    {
        Q_ASSERT(item);
        Q_ASSERT(MLinearLayoutPolicy::count() % 2);
        Q_ASSERT(MLinearLayoutPolicy::count() > index*2);

        //Add an item and a spacer after the item
        MLinearLayoutPolicy::insertItem(1 + index*2, item);
        insertSpacer(2+index*2);
        Q_ASSERT(MLinearLayoutPolicy::count() % 2);
    }
    virtual void removeAt(int index)
    {
        Q_ASSERT(MLinearLayoutPolicy::count() % 2);
        Q_ASSERT(MLinearLayoutPolicy::count() > 2+index*2);

        //Remove the item first
        QGraphicsLayoutItem *item = MLinearLayoutPolicy::itemAt(2+index*2);
        MLinearLayoutPolicy::removeAt(2+index*2);
        MLinearLayoutPolicy::removeAt(1+index*2);
        layout()->removeItem(item);
    }
    virtual int count() const
    {
        return (MLinearLayoutPolicy::count()-1)/2;
    }
    virtual QGraphicsLayoutItem *itemAt(int index) const
    {
        return MLinearLayoutPolicy::itemAt(1+index*2);
    }

private:
    void insertSpacer(int position) {
        QGraphicsWidget *item =  new QGraphicsWidget;
        item->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        MLinearLayoutPolicy::insertItem(position, item);
    }
};

MToolBarViewPrivate::MToolBarViewPrivate(MToolBar *controller)
    : QObject(),
      q_ptr(0),
      layout(0),
      landscapePolicy(0),
      portraitPolicy(0),
      buttonGroup(0)
{
    this->controller = controller;
    controller->installEventFilter(this);
}


MToolBarViewPrivate::~MToolBarViewPrivate()
{
    removeEventFilter(controller);
    if(buttonGroup) {
        foreach(MButton *button, buttonGroup->buttons())
            buttonGroup->removeButton(button);
        delete buttonGroup;
        buttonGroup = NULL;
    }
    QHashIterator<QAction *, MWidget *> iterator(leasedWidgets);
    while (iterator.hasNext()) {
        iterator.next();

        if (!releaseWidget(iterator.key(), iterator.value()))
            delete iterator.value(); //Is this the right thing to do ?
    }
    qDeleteAll(buttons);
    delete layout;
    layout = NULL;
}

void MToolBarViewPrivate::init()
{
    layout = new MLayout;
    layout->setAnimation(NULL);
    layout->setContentsMargins(0, 0, 0, 0);

    landscapePolicy = new ToolBarLayoutPolicy(layout);
    landscapePolicy->setSpacing(0);
    layout->setLandscapePolicy(landscapePolicy);

    portraitPolicy = new ToolBarLayoutPolicy(layout);
    portraitPolicy->setSpacing(0);
    layout->setPortraitPolicy(portraitPolicy);

    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->addItem(layout);

    //Add any existing actions now
    foreach(QAction *action, controller->actions()) {
        add(action);
    }
}

int MToolBarViewPrivate::policyIndexForAddingAction(QAction *action, MLinearLayoutPolicy *policy) const {
    Q_Q(const MToolBarView);
    //We need to add the action's given widget to the policy
    //This is a bit complicated because we ideally want to add it in the right place,
    //preserving the same order as in the controller->actions()
    QList< QAction *> actions = controller->actions();
    int parentIndex = actions.indexOf(action)+1;
    while(parentIndex < actions.size()) {
        MWidget *w = q->getWidget(actions.at(parentIndex));
        if(!w) {
            int policyIndex = policy->indexOf(w);
            if(policyIndex >= 0)
                return policyIndex;
        }
        parentIndex++;
    }
    return policy->count(); //Add it to the end
}

void MToolBarViewPrivate::add(QAction *action)
{
    if (!action || !action->isVisible() || hasUnusableWidget(action))
        return; //Cancel adding action

    // add to policies only if the action is visible
    bool addToLandscape =  isLocationValid(action, MAction::ToolBarLandscapeLocation) &&
        reserveSpaceForAction(action, landscapeData);
    bool addToPortrait  = isLocationValid(action, MAction::ToolBarPortraitLocation) &&
        reserveSpaceForAction(action, portraitData);
    if (!addToLandscape && !addToPortrait)
        return; //Cancel adding action
    MWidget *widget = createWidget(action);
    Q_ASSERT(widget);
    if (addToLandscape)
        landscapePolicy->insertItem( policyIndexForAddingAction(action, landscapePolicy), widget );
    if (addToPortrait)
        portraitPolicy->insertItem( policyIndexForAddingAction(action, portraitPolicy), widget );
    updateWidgetFromAction(widget, action);
}

void MToolBarViewPrivate::remove(QAction *action, bool hideOnly)
{
    MButton *button = buttons.value(action);
    MWidget *leased = leasedWidgets.value(action);
    MWidget *widget = (button != 0) ? button : leased;

    if(!widget)
        return; //Action already removed
    bool isTextEditWidget = hasTextEditWidget(action);
    int landscapeIndex = landscapePolicy->indexOf(widget);
    int portraitIndex = portraitPolicy->indexOf(widget);
    if(hideOnly && landscapeIndex == -1 && portraitIndex == -1)
        return; //Action already hidden

    if(landscapeIndex >= 0) {
        if(isTextEditWidget) {
            landscapeData.hasTextEditor = false;
            //one text-edit widget takes space of two buttons
            landscapeData.placedActions -= 2;
        } else
            landscapeData.placedActions--;
        if(hideOnly)
            landscapePolicy->removeAt(landscapeIndex);
    }
    if(portraitIndex >= 0) {
        if(isTextEditWidget) {
            portraitData.hasTextEditor = false;
            //one text-edit widget takes space of two buttons
            portraitData.placedActions -= 2;
        } else
            portraitData.placedActions--;
        if(hideOnly)
            portraitPolicy->removeAt(portraitIndex);
    }

    if(!hideOnly) {
        //Need to fully remove the action
        layout->removeItem(widget);

        if (button) {
            buttons.remove(action);
            if(buttonGroup)
                buttonGroup->removeButton(button);
            delete button;
        } else {
            releaseWidget(action, leased);
            leasedWidgets.remove(action);
        }
    }

    //There might be space now any actions not already added.  Signal a change action which
    //will check if an item now has room to be shown
    foreach(QAction *action, controller->actions()) {
        if(action->isVisible())
            change(action);
    }
}

void MToolBarViewPrivate::change(QAction *action)
{
    Q_Q(MToolBarView);
    if(hasUnusableWidget(action))
        return;
    if(!action->isVisible()) {
        remove(action, true); //Remove action, but only hiding the widget, not deleting/releasing it
        return;
    }
    bool validInLandscape = isLocationValid(action, MAction::ToolBarLandscapeLocation);
    bool validInPortrait = isLocationValid(action, MAction::ToolBarPortraitLocation);
    if (!validInLandscape && !validInPortrait) {
        remove(action, true);
        return;
    }

    //Check that the widget is in the controller actions
    QList< QAction *> actions = controller->actions();
    int indexOfAction = actions.indexOf(action);
    if(indexOfAction == -1) {
        remove(action, false); // I don't think this is possible
        return;
    }
        
    MWidget *widget = q->getWidget(action);
    if (!widget) {
        //We need to add the action
        add(action);
        return;
    }

    //We have now an action and a widget for it
    int landscapeIndex = landscapePolicy->indexOf(widget);
    int portraitIndex = portraitPolicy->indexOf(widget);
    if(!validInLandscape && landscapeIndex >= 0) {
        //We are showing it in landscape view but should not be
        landscapePolicy->removeAt(landscapeIndex);
    }
    if(!validInPortrait && portraitIndex >= 0) {
        //We are showing it in portrait view but should not be
        portraitPolicy->removeAt(portraitIndex);
    }
    if(validInLandscape && landscapeIndex == -1) {
        if( reserveSpaceForAction(action, landscapeData) )
            landscapePolicy->insertItem( policyIndexForAddingAction(action, landscapePolicy), widget );
    }
    if(validInPortrait && portraitIndex == -1) {
        if( reserveSpaceForAction(action, portraitData) )
            portraitPolicy->insertItem( policyIndexForAddingAction(action, portraitPolicy), widget );
    }

    updateWidgetFromAction(widget, action);
}

void MToolBarViewPrivate::updateWidgetFromAction(MWidget *widget, QAction *action) const
{
    widget->setEnabled(action->isEnabled());
    MButton *button = qobject_cast<MButton *>(widget);
    if (button) {
        // Update button data accordingly
        button->setText(action->text());
        button->setCheckable(action->isCheckable());
        button->setChecked(action->isChecked());
        MAction *mAction = qobject_cast<MAction *>(action);
        if (mAction)
            button->setIconID(mAction->iconID());
    }
}

bool MToolBarViewPrivate::eventFilter(QObject *obj, QEvent *e)
{
    switch (e->type()) {
        case QEvent::ActionRemoved:
            remove(static_cast<QActionEvent *>(e)->action(), false);
            break;
        case QEvent::ActionAdded:
            add(static_cast<QActionEvent *>(e)->action());
            break;
        case QEvent::ActionChanged:
            change(static_cast<QActionEvent *>(e)->action());
            break;
        default:
            break;
    }
    return QObject::eventFilter(obj, e);
}

MWidget *MToolBarViewPrivate::createWidget(QAction *action)
{
    Q_Q(MToolBarView);
    // If widget is not already created then create it
    MWidget *widget = buttons.value(action);
    if (!widget)
        widget = leasedWidgets.value(action);
    if (!widget) {
        MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
        if (widgetAction) {
            widget = widgetAction->requestWidget(NULL);
            leasedWidgets.insert(action, widget);
            widget->show();
        } else {
            MButton *button = new MButton;

            button->setObjectName("toolbaractioncommand");
            if (widgetAction && !widgetAction->objectName().isEmpty())
                button->setObjectName(button->objectName() + "_" + widgetAction->objectName());

            if(buttonGroup) {
                button->setViewType("toolbartab");
                button->setCheckable(action->isCheckable());
                //We can't set button->checked until we've added it to the scene
                buttonGroup->addButton(button);
                connect(button, SIGNAL(toggled(bool)), q, SLOT(_q_groupButtonClicked(bool)));
                connect(action, SIGNAL(toggled(bool)), q, SLOT(_q_groupActionToggled(bool)));
            } else {
                button->setViewType("toolbar");
            }
            connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));

            buttons.insert(action, button);
            widget = button;
        }
    }
    Q_ASSERT(widget);
    return widget;
}
bool MToolBarViewPrivate::isLocationValid(QAction *action, MAction::Location loc) const
{
    MAction *mAction = qobject_cast<MAction *>(action);
    if(!mAction)
        return true; //A normal QAction is valid to place on toolbar
    return mAction->location().testFlag(loc);
}

bool MToolBarViewPrivate::releaseWidget(QAction *action, MWidget *widget) const
{
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction) {
        widgetAction->releaseWidget(widget);
    }
    return (widgetAction != 0);
}

bool MToolBarViewPrivate::hasTextEditWidget(QAction *action) const
{
    MTextEdit *textEditWidget = 0;
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction) {
        textEditWidget = qobject_cast<MTextEdit *>(widgetAction->widget());
    }
    return (textEditWidget != 0);
}

bool MToolBarViewPrivate::hasUnusableWidget(QAction *action) const
{
    if( buttons.contains(action) || leasedWidgets.contains(action) )
        return false; //If we are already using it, it must be usable
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if(!widgetAction || !widgetAction->widget())
        return false; //We can create a button for it
    return widgetAction->isWidgetInUse();
}

MWidget *MToolBarView::getWidget(QAction *action) const
{
    Q_D(const MToolBarView);
    MWidget *button = d->buttons.value(action);
    if( button )
        return button;
    return d->leasedWidgets.value(action);
}

bool MToolBarViewPrivate::reserveSpaceForAction(QAction *action, ActionPlacementData &policyData)
{
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

void MToolBarViewPrivate::_q_groupButtonClicked(bool checked)
{
    Q_Q(MToolBarView);
    MButton *button = qobject_cast<MButton *>(q->sender());
    if(!button)
        return; //impossible?

    button->setChecked(checked);

    QAction *action = buttons.key(button);
    if(action->isChecked() != checked)
        action->setChecked(checked);
}

void MToolBarViewPrivate::_q_groupActionToggled(bool checked)
{
    Q_Q(MToolBarView);
    QAction* action = qobject_cast<QAction *>(q->sender());
    Q_ASSERT(action);
    MButton *button = buttons.value(action);
    if (button) {
        button->setChecked(checked);
    }
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
