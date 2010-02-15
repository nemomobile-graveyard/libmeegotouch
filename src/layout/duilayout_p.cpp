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

#include "duilayout_p.h"

#include "duiabstractlayoutpolicy.h"
#include "duibasiclayoutanimation.h"
#include "duilayouthelper_p.h"
#include "duiapplication.h"

#include "duiwidget_p.h"

#include "duidebug.h"

DuiLayoutPrivate::DuiLayoutPrivate(DuiLayout *l) :
    q_ptr(l),
    portraitPolicy(NULL),
    landscapePolicy(NULL),
    current_policy(NULL),
    animation(NULL)
{
    Q_ASSERT(0 != q_ptr);
    DuiWindow *window = DuiApplication::activeWindow();
    if (window) {
        connect(window,
                SIGNAL(orientationChanged(Dui::Orientation)),
                SLOT(orientationChanged(Dui::Orientation)));
    }
}

void DuiLayoutPrivate::registerPolicy(DuiAbstractLayoutPolicy *policy)
{
    Q_Q(DuiLayout);
    Q_ASSERT(policy);
    if (policies.contains(policy))
        return;
    policies.append(policy);

    if (!current_policy)
        q->setPolicy(policy);
}

void DuiLayoutPrivate::unregisterPolicy(DuiAbstractLayoutPolicy *policy)
{
    Q_Q(DuiLayout);
    Q_ASSERT(policy);
    if (!policies.contains(policy))
        return;

    if (landscapePolicy == policy)
        q->setLandscapePolicy(NULL);
    if (portraitPolicy == policy)
        q->setPortraitPolicy(NULL);

    policies.removeAll(policy);
    if (current_policy == policy) {
        if (policies.isEmpty())
            q->setPolicy(0);
        else
            q->setPolicy(policies.last());
    }
}

void DuiLayoutPrivate::setItemGeometry(int index, const QRectF &geometry)
{
    Q_Q(DuiLayout);
    if (0 > index || q->count() <= index) {
        duiWarning("DuiLayout") << Q_FUNC_INFO << "Attempting to set the geometry of the item at index" << index << "when there are" << q->count() << "items";
        return;
    }

    DuiItemState &state = states[index];
    state.setTargetGeometry(geometry);

    // If no animator or the item is a layout then do not animate
    if (!animation || state.item()->isLayout()) {
        //Show item
        showItemNow(state.item());
        state.item()->setGeometry(state.targetGeometry());
        state.animationDone();
        itemAnimationFinished(index);
    } else {
        if (state.isSet(DuiItemState::STATE_FLAG_TO_BE_SHOWN)) {
            animation->doItemShownAnimation(&state);
            //we only want to call the item shown animation once, so clear the shown flags
            state.removeFlags(DuiItemState::STATE_FLAG_TO_BE_SHOWN);
            state.addFlags(DuiItemState::STATE_FLAG_SHOWING);
        }
        if (state.isAnimationDone()) //Set the geometry anyway, so that it can refresh
            state.item()->setGeometry(state.targetGeometry());
        else
            animation->startAnimation(&state);
    }
}

QRectF DuiLayoutPrivate::itemGeometry(int index) const
{
    Q_Q(const DuiLayout);
    if (0 > index || q->count() <= index) {
        duiWarning("DuiLayout") << Q_FUNC_INFO << "Attempting to set the geometry of the item at index" << index << "when there are" << q->count() << "items";
        return QRectF();
    }

    return states.at(index).targetGeometry();
}
void DuiLayoutPrivate::itemAnimationFinished(int index)
{
    Q_Q(DuiLayout);
    Q_ASSERT(states.at(index).isAnimationDone());
    if (states.at(index).isSet(DuiItemState::STATE_FLAG_TO_BE_DELETED))
        q->removeAt(index);  //This will delete the item as well
}

void DuiLayoutPrivate::animationFinished()
{
    // check whether any item is now really deleted
    for (int i = states.size() - 1; i >= 0; --i) {
        if (states.at(i).isAnimationDone())
            itemAnimationFinished(i);
    }
}

void DuiLayoutPrivate::hideItem(int index)
{
    DuiItemState &state = states[index];
    //Check if its being hidden or if it is hiding (i.e. not shown and not to-be-shown
    if (state.isSet(DuiItemState::STATE_FLAG_TO_BE_HIDDEN) ||
            (!state.isSet(DuiItemState::STATE_FLAG_TO_BE_SHOWN) &&
             !state.isSet(DuiItemState::STATE_FLAG_SHOWING))) {
        return; //It's already hiding/hidden.  Nothing to do
    }
    state.hide();

    QGraphicsItem *graphics_item = state.item()->graphicsItem();
    if (!animation || (graphics_item && !graphics_item->isVisibleTo(NULL))) { //If it isn't visible, do not bother to animate it
        //Hide item directly
        hideItemNow(state.item());
        //we can't do anything to hide something which isn't a QGraphicsItem
        state.removeFlags(
            DuiItemState::STATE_FLAG_TO_BE_HIDDEN |
            DuiItemState::STATE_FLAG_TO_BE_SHOWN |
            DuiItemState::STATE_FLAG_SHOWING);
    } else {
        animation->doItemHiddenAnimation(&state);
        state.hide(); //setTargetGeometry clears TO_BE_HIDDEN, so re-add
        animation->startAnimation(&state);
    }

}

void DuiLayoutPrivate::orientationChanged(const Dui::Orientation &orientation)
{
    if (orientation == Dui::Portrait) {
        if (portraitPolicy)
            portraitPolicy->activate();
    } else if (landscapePolicy)
        landscapePolicy->activate();
    //since the rotation has changed, reread the values from the CSS config
    foreach(DuiAbstractLayoutPolicy * policy, policies) {
        Q_ASSERT(policy);
        policy->updateStyle();
    }
}
QGraphicsItem *DuiLayoutPrivate::parentItem() const
{
    Q_Q(const DuiLayout);

    const QGraphicsLayoutItem *parent = q;
    while (parent && parent->isLayout()) {
        parent = parent->parentLayoutItem();
    }
    return parent ? parent->graphicsItem() : 0;
}

void DuiLayoutPrivate::showItemNow(QGraphicsLayoutItem *layoutItem)
{
    QGraphicsItem *graphicsItem = layoutItem->graphicsItem();
    if (graphicsItem) {
        DuiWidget *widget = dynamic_cast<DuiWidget *>(graphicsItem);
        if (widget) {
            widget->d_ptr->layoutHidden = false;
            if (!widget->d_ptr->explicitlyHidden)
                graphicsItem->show(); //Show only if item was not set to invisible by the user
        } else
            graphicsItem->show(); //Show always for non-duiwidgets
    }
}
void DuiLayoutPrivate::removeHiddenFlag(QGraphicsLayoutItem *layoutItem)
{
    DuiWidget *widget = dynamic_cast<DuiWidget *>(layoutItem->graphicsItem());
    if (widget)
        widget->d_ptr->layoutHidden = false;
}
void DuiLayoutPrivate::hideItemNow(QGraphicsLayoutItem *layoutItem)
{
    QGraphicsItem *graphicsItem = layoutItem->graphicsItem();
    if (graphicsItem) {
        graphicsItem->hide();
        DuiWidget *widget = dynamic_cast<DuiWidget *>(graphicsItem);
        if (widget)
            widget->d_ptr->layoutHidden = true;
    }
    //There's not really anything we can do to hide an item that isn't a QGraphicsItem
    //DuiLayout, for example, is not a QGraphicsItem, but just a QGraphicsLayoutItem
    //The following code moves the item out of the way, but in practice this doesn't
    //really work since coordinates all relative
}


#include "moc_duilayout_p.cpp"

