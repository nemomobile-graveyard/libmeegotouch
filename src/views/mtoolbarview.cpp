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
#include "mtoolbar_p.h"

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

const int maxSlots = 4;

/* Make a layout policy for aligning widgets nicely by
 * adding spacers.
 * This is a very crude policy, and doesn't support most
 * of the functions of MLinearLayoutPolicy */
class ToolBarLayoutPolicy : public MLinearLayoutPolicy
{
public:
    explicit ToolBarLayoutPolicy(MLayout *layout) :
        MLinearLayoutPolicy(layout, Qt::Horizontal) {
            textEditIndex = -1;
            insertSpacer(0);
            setSpacing(0);
        }
    ~ToolBarLayoutPolicy() {
        for(int i = widgetCount()-1; i >= 0; i--)
            removeWidgetAt(i);
        QGraphicsLayoutItem *item = MLinearLayoutPolicy::itemAt(0);
        MLinearLayoutPolicy::removeAt(0);
        delete item;
    }
    void insertWidgetAndRemoveOverflow(int index, QGraphicsLayoutItem *item)
    {
        index = qMin((uint)index, (uint)widgetCount());
        //A ugly hack to try to cope with QTBUG-11134
        item->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        Q_ASSERT(item);
        Q_ASSERT(MLinearLayoutPolicy::count() % 2);
        Q_ASSERT(MLinearLayoutPolicy::count() > index*2);

        if(dynamic_cast<MTextEdit *>(item->graphicsItem())) {
            if(textEditIndex != -1)  //Remove any previous text edit
                removeWidgetAt(textEditIndex);
            textEditIndex = index;
        } else if(index <= textEditIndex)
            textEditIndex++;

        //Add an item and a spacer after the item
        MLinearLayoutPolicy::insertItem(1 + index*2, item);

        insertSpacer(2+index*2);
        Q_ASSERT(MLinearLayoutPolicy::count() % 2);

        //Prevent overflow - remove any that we've pushed off the end
        while(takenSlots() > maxSlots) {
            removeWidgetAt(widgetCount()-1);
        }
    }
    bool roomForWidget(int index, bool textEdit) const {
        index = qMin((uint)index, (uint)widgetCount());

        if(textEdit) {
            if(textEditIndex != -1 && index > textEditIndex) {
                //If we come first, we push the old textedit out of the way
                return false; //No room for us
            } else if(index + 1 >= maxSlots) {
                return false; //No room to add
            }
        }
        else if(index >= maxSlots)
            return false; //No room to add
        else if(textEditIndex != -1 && index >= maxSlots -1)
            return false; //No room to add because textedit takes up two spaces
        return true;
    }
    void removeWidgetAt(int index)
    {
        if(index < 0 || index >= widgetCount())
            return;
        Q_ASSERT(MLinearLayoutPolicy::count() % 2);
        Q_ASSERT(MLinearLayoutPolicy::count() > 2+index*2);

        //Remove the item first
        if(index == textEditIndex)
            textEditIndex = -1;
        else if(textEditIndex != -1 && index < textEditIndex)
            textEditIndex--;
        MLinearLayoutPolicy::removeAt(2+index*2);
        MLinearLayoutPolicy::removeAt(1+index*2);
    }

    int widgetCount() const
    {
        return (MLinearLayoutPolicy::count()-1)/2;
    }
    int takenSlots() const
    {
        if(textEditIndex != -1)
            return widgetCount() + 1;  //textedit counts for two slots
        else
            return widgetCount();
    }
    int widgetIndexOf(const QGraphicsLayoutItem *item) const {
        for(int i = widgetCount() -1; i >= 0; i--) {
            if(MLinearLayoutPolicy::itemAt(1+i*2) == item)
                return i;
        }
        return -1;
    }
    void removeWidget(const QGraphicsLayoutItem *item)
    {
        removeWidgetAt(widgetIndexOf(item));
    }

protected:
    /** Hide the normal functions since these include the spacers
     *  and we want to encourage the user to only use our functions */
    virtual int count() const { return MLinearLayoutPolicy::count(); }
    virtual QGraphicsLayoutItem *itemAt(int index) const { return MLinearLayoutPolicy::itemAt(index); }
    virtual void insertItem(int index, QGraphicsLayoutItem *item) { return MLinearLayoutPolicy::insertItem(index, item); }
    void indexOf(const QGraphicsLayoutItem *item) const;
    virtual void removeAt(int index) {
        //This can be called by the layout, and we need to make sure that we delete the spacer as well
        removeWidgetAt((index-1)/2);
    }

private:
    int textEditIndex;

    void insertSpacer(int position) {
        //Create an item that will expand if necessary
        QGraphicsWidget *item =  new QGraphicsWidget;
        item->setPreferredSize(1,1); //Ugly hack around QTBUG-11134
        item->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        MLinearLayoutPolicy::insertItem(position, item);
    }
};

MToolBarViewPrivate::MToolBarViewPrivate(MToolBar *controller)
    : QObject(),
      q_ptr(0),
      layout(0),
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
    layout->setLandscapePolicy(landscapePolicy);

    portraitPolicy = new ToolBarLayoutPolicy(layout);
    layout->setPortraitPolicy(portraitPolicy);

    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->addItem(layout);

    //Add any existing actions now
    foreach(QAction *action, controller->actions()) {
        add(action);
    }
}

int MToolBarViewPrivate::policyIndexForAddingAction(QAction *action, ToolBarLayoutPolicy *policy) const {
    Q_Q(const MToolBarView);
    //We need to add the action's given widget to the policy
    //This is a bit complicated because we ideally want to add it in the right place,
    //preserving the same order as in the controller->actions()
    QList< QAction *> actions = controller->actions();
    int parentIndex = actions.indexOf(action)+1;
    int index = policy->widgetCount();
    while(parentIndex < actions.size()) {
        MWidget *w = q->getWidget(actions.at(parentIndex));
        if(w) {
            int policyIndex = policy->widgetIndexOf(w);
            if(policyIndex >= 0) {
                index = policyIndex;
                break;
            }
        }
        parentIndex++;
    }
    if(policy->roomForWidget(index, hasTextEditWidget(action)))
        return index;
    return -1;
}

void MToolBarViewPrivate::add(QAction *action)
{
    if (!action || !action->isVisible() || hasUnusableWidget(action))
        return; //Cancel adding action

    // add to policies only if the action is visible
    int landscapeIndex = -1;
    int portraitIndex = -1;
    if (isLocationValid(action, MAction::ToolBarLandscapeLocation))
        landscapeIndex = policyIndexForAddingAction(action, landscapePolicy);
    if (isLocationValid(action, MAction::ToolBarPortraitLocation))
        portraitIndex = policyIndexForAddingAction(action, portraitPolicy);

    if (landscapeIndex == -1 && portraitIndex == -1)
        return; //Cancel adding action
    MWidget *widget = createWidget(action);
    Q_ASSERT(widget);
    if (landscapeIndex != -1)
        landscapePolicy->insertWidgetAndRemoveOverflow( landscapeIndex, widget );
    if (portraitIndex != -1)
        portraitPolicy->insertWidgetAndRemoveOverflow( portraitIndex, widget );
    updateWidgetFromAction(widget, action);
}

void MToolBarViewPrivate::remove(QAction *action, bool hideOnly)
{
    MButton *button = buttons.value(action);
    MWidget *leased = leasedWidgets.value(action);
    MWidget *widget = (button != 0) ? button : leased;
    if(!widget)
        return;

    if(hideOnly) {
        landscapePolicy->removeWidget(widget);
        portraitPolicy->removeWidget(widget);
    } else {
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
    int landscapeIndex = landscapePolicy->widgetIndexOf(widget);
    int portraitIndex = portraitPolicy->widgetIndexOf(widget);
    if(!validInLandscape && landscapeIndex >= 0) {
        //We are showing it in landscape view but should not be
        landscapePolicy->removeWidgetAt(landscapeIndex);
    }
    if(!validInPortrait && portraitIndex >= 0) {
        //We are showing it in portrait view but should not be
        portraitPolicy->removeWidgetAt(portraitIndex);
    }
    if(validInLandscape && landscapeIndex == -1) {
        int index = policyIndexForAddingAction(action, landscapePolicy);
        if(index != -1)
            landscapePolicy->insertWidgetAndRemoveOverflow( index, widget );
    }
    if(validInPortrait && portraitIndex == -1) {
        int index = policyIndexForAddingAction(action, portraitPolicy);
        if(index != -1)
            portraitPolicy->insertWidgetAndRemoveOverflow( index, widget );
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

        case QEvent::DynamicPropertyChange:
            {
                QDynamicPropertyChangeEvent *propertyEvent = static_cast<QDynamicPropertyChangeEvent*>(e);
                if (propertyEvent->propertyName() == _M_IsEnabledPreservingSelection) {
                    bool enabledPreservingSelection = obj->property(_M_IsEnabledPreservingSelection).toBool();
                    setEnabledPreservingSelection(enabledPreservingSelection);
                }
            }
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

void MToolBarViewPrivate::setEnabledPreservingSelection(bool enabled)
{
    foreach(MButton *button, buttons) {
        if (!button->isChecked())
            button->setEnabled(enabled);
    }

    foreach(MWidget *leasedWidget, leasedWidgets) {
        MButton *button = qobject_cast<MButton *>(leasedWidget);
        if (!button || !button->isChecked())
            leasedWidget->setEnabled(enabled);
    }
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
