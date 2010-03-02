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

#include "duilayout.h"
#include "duilayout_p.h"

#include "duiapplication.h"
#include "duiabstractlayoutpolicy.h"
#include "duiabstractlayoutpolicy_p.h"
#include "duiitemstate.h"
#include "duibasiclayoutanimation.h"
#include "duiorientationchangeevent.h"

#include <QGraphicsItem>
#include <QApplication>
#include <DuiDebug>

DuiLayout::DuiLayout(QGraphicsLayoutItem *parent) :
    QGraphicsLayout(parent),
    d_ptr(new DuiLayoutPrivate(this))
{
    Q_ASSERT(0 != d_ptr);
    // some policies have height for width, so set it for the whole layout
    QSizePolicy newSizePolicy(sizePolicy());
    newSizePolicy.setHeightForWidth(true);
    setSizePolicy(newSizePolicy);
    // Disabling layout animations by default. Unfortunately they are not working
    // nicely.
    //new DuiBasicLayoutAnimation(this);
}

DuiLayout::~DuiLayout()
{
    Q_D(DuiLayout);
    d->current_policy = NULL;  //Make sure we have no policy before

    qDeleteAll(d->policies);
    //Remove all the items from the layout and delete any layouts
    //(which are ownedByLayout) inside this layout
    //Because we don't have any policies, this should be pretty fast
    for (int i = count() - 1; i >= 0; --i) {
        bool deleteItem = (d->states.at(i).isSet(DuiItemState::STATE_FLAG_TO_BE_DELETED));
        QGraphicsLayoutItem *item = itemAt(i);
        if (item) {
            item->setParentLayoutItem(0);
            if (item->ownedByLayout() || deleteItem)
                delete item;
        }
    }
    delete d->animation;
    delete d_ptr;
}

// Reimplemented from QGraphicsLayout
void DuiLayout::invalidate()
{
    Q_D(DuiLayout);
    //We need to check if the orientation has changed.  This can cause the current policy to change
    //Unfortunately, this is the only function called when widget->setLayout(layout)  is called.
    d->recheckOrientation();

    if (d->current_policy)
        d->current_policy->invalidate();
    QGraphicsLayout::invalidate();
    //This following warning is useful, but too many false negatives :-/
//    if(!parentLayoutItem())
//        duiWarning("DuiLayout") << "DuiLayout does not have a parent - layouting will not be correct.  Add the layout to a QGraphicsWidget";
}

void DuiLayout::invalidateLayoutOnly()
{
    QGraphicsLayout::invalidate();
}

int DuiLayout::count() const
{
    Q_D(const DuiLayout);
    return d->states.count();
}
bool DuiLayout::isEmpty() const
{
    Q_D(const DuiLayout);
    return d->states.isEmpty();
}
QGraphicsLayoutItem *DuiLayout::itemAt(int index) const
{
    Q_D(const DuiLayout);
    if (0 > index || count() <= index) {
        duiWarning("DuiLayout") << Q_FUNC_INFO << "Attempting to access item at index" << index << "when there are" << count() << "items";
        return NULL;
    }
    return d->states.at(index).item();
}

void DuiLayout::removeAt(int index)
{
    (void)takeAt(index);
}

QGraphicsLayoutItem *DuiLayout::takeAt(int index)
{
    Q_D(DuiLayout);
    if (0 > index || count() <= index) {
        if (index != 0) //It's common to just try deleting the first item until there are none left, so suppress this warning for index=0
            duiWarning("DuiLayout") << Q_FUNC_INFO << "Attempting to remove item at index" << index << "when there are" << count() << "items";
        return NULL;
    }
    bool deleteItem = d->states.at(index).isSet(DuiItemState::STATE_FLAG_TO_BE_DELETED);
    QGraphicsLayoutItem *item = d->states.at(index).item();
    Q_ASSERT(item);
    foreach(DuiAbstractLayoutPolicy * policy, d->policies) {
        Q_ASSERT(policy);
        policy->d_ptr->aboutToBeRemovedFromLayout(item);
    }

    d->states.removeAt(index);
    item->setParentLayoutItem(NULL);
    invalidate();
    if (deleteItem) {
        delete item;
        return NULL;
    }
    d->removeHiddenFlag(item);
    return item;
}

void DuiLayout::animatedDeleteItem(const QGraphicsLayoutItem *const item)
{
    animatedDeleteAt(indexOf(item));
}

void DuiLayout::animatedDeleteAt(int index)
{
    Q_D(DuiLayout);
    if (0 > index || count() <= index) {
        if (index != 0)
            duiWarning("DuiLayout") << Q_FUNC_INFO << "Attempting to remove item at index" << index << "when there are" << count() << "items";
        return;
    }

    DuiItemState &state = d->states[index];
    // Abort if already scheduled for deletion
    // (this can happen because a graphics layout item removes itself from its
    //  layout upon destruction and would lead to an infinite recursion here)
    if (state.isSet(DuiItemState::STATE_FLAG_TO_BE_DELETED))
        return;

    if (!d->animation || (!state.isSet(DuiItemState::STATE_FLAG_SHOWING))) {
        delete takeAt(index); //Delete immediately if its not visible
    } else {
        if (state.isSet(DuiItemState::STATE_FLAG_TO_BE_DELETED))
            return; //Already set
        state.addFlags(DuiItemState::STATE_FLAG_TO_BE_DELETED);
        d->animation->doItemDeletingAnimation(&state);
        d->animation->startAnimation(&state);
    }
}

// QGraphicsLayoutItem:

QSizeF DuiLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiLayout);
    if (d->current_policy) {
        return d->current_policy->sizeHint(which, constraint);
    }
    return QSizeF(-1, -1);
}

void DuiLayout::setGeometry(const QRectF &rect)
{
    Q_D(DuiLayout);
    QRectF previousGeometry = geometry();
    //setGeometry is also called when the layout is activated after being invalidated
    //so we should always setGeometry even if the rect has not changed

    //Note that setGeometry first adjusts rect to obey the sizehints, so our actual geometry
    //afterwards is not necessarily equal to the parameter 'rect'.
    QGraphicsLayout::setGeometry(rect);

    if (d->current_policy) {
        d->current_policy->relayout();
        //If the width really has changed and our height depends on the width, we need to recalculate
        //the width.
        if (d->current_policy->hasHeightForWidth() && previousGeometry.width() != geometry().width())
            updateGeometry(); //Since the height sizeHint has changed, we need to invalidate the current size hint
    }

}

int DuiLayout::addItem(QGraphicsLayoutItem *item)
{
    Q_D(DuiLayout);
    // do we know this item already?
    if (!item)
        return -1; //error
    int index = indexOf(item);
    if (index < 0) {
        // If the item is a layout, we need to add all of its items
        DuiItemState state(item);
        addChildLayoutItem(item);
        // Add item
        index = d->states.size();
        d->states.append(state);
        // Hide item for now since we do not know where to place it
        d->hideItemNow(item);
    }
    return index;
}

void DuiLayout::removeItem(const QGraphicsLayoutItem *const item)
{
    if (0 == item) {
        return;
    }

    int index = indexOf(item);
    if (index == -1)
        return; //Should we display a warning?
    removeAt(index);
}

int DuiLayout::indexOf(const QGraphicsItem *item) const
{
    Q_D(const DuiLayout);
    if (0 == item) {
        return -1;
    }

    const int theCount = count();
    for (int i = 0; i < theCount; ++i) {
        if (d->states.at(i).item()->graphicsItem() == item) {
            return i;
        }
    }

    return -1;
}

int DuiLayout::indexOf(const QGraphicsLayoutItem *item) const
{
    Q_D(const DuiLayout);
    if (0 == item) {
        return -1;
    }

    const int theCount = count();
    for (int i = 0; i < theCount; ++i) {
        if (d->states.at(i).item() == item) {
            return i;
        }
    }

    return -1;
}

void DuiLayout::setPolicy(DuiAbstractLayoutPolicy *policy)
{
    Q_D(DuiLayout);
    if (d->current_policy == policy)
        return;

    d->current_policy = policy;
    if (d->current_policy) {
        Q_ASSERT(d->policies.contains(policy));
        policy->activated();
    }
    invalidate();
    //Note that the policy might not be fully constructed yet, since we might still be in the
    //DuiAbstractLayoutPolicy constructor.  So we cannot immediately do a re-layout - we have to
    //invalidate, waiting until the next event loop to activate()
}
void DuiLayout::setLandscapePolicy(DuiAbstractLayoutPolicy *policy)
{
    Q_D(DuiLayout);
    if (d->landscapePolicy == policy)
        return;
    d->landscapePolicy = policy;
    Q_ASSERT(!policy || policy->layout() == this);
    if (d->landscapePolicy && d->m_orientation == Dui::Landscape) {
        policy->activate();
    }
}
DuiAbstractLayoutPolicy *DuiLayout::landscapePolicy() const
{
    Q_D(const DuiLayout);
    return d->landscapePolicy;
}

void DuiLayout::setPortraitPolicy(DuiAbstractLayoutPolicy *policy)
{
    Q_D(DuiLayout);
    if (d->portraitPolicy == policy)
        return;
    d->portraitPolicy = policy;
    Q_ASSERT(!policy || policy->layout() == this);
    if (d->portraitPolicy && d->m_orientation == Dui::Portrait) {
        policy->activate();
    }
}
DuiAbstractLayoutPolicy *DuiLayout::portraitPolicy() const
{
    Q_D(const DuiLayout);
    return d->portraitPolicy;
}

DuiAbstractLayoutPolicy *DuiLayout::policy() const
{
    Q_D(const DuiLayout);
    return d->current_policy;
}

QList<DuiAbstractLayoutPolicy *> DuiLayout::registeredPolicies() const
{
    Q_D(const DuiLayout);
    return d->policies;
}

void DuiLayout::setAnimation(DuiLayoutAnimation *animation)
{
    Q_D(DuiLayout);
    delete d->animation;
    d->animation = animation;
}

DuiLayoutAnimation *DuiLayout::animation() const
{
    Q_D(const DuiLayout);
    return d->animation;
}

void DuiLayout::setContentsMargins(qreal left, qreal top, qreal right, qreal bottom)
{
    QGraphicsLayout::setContentsMargins(left, top, right, bottom);
}

void DuiLayout::getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const
{
    Q_D(const DuiLayout);
    //If there is a current policy, give it a chance to override current margins
    if (d->current_policy)
        d->current_policy->getContentsMargins(left, top, right, bottom);

    //if the policy returned negative values, use the QGraphicsLayout margins, otherwise do not change.
    if (left && *left >= 0)
        left = NULL;
    if (top && *top >= 0)
        top = NULL;
    if (right && *right >= 0)
        right = NULL;
    if (bottom && *bottom >= 0)
        bottom = NULL;

    QGraphicsLayout::getContentsMargins(left, top, right, bottom);
}

/* This function was copied from Qt 4.5's qgraphicslayout_p.cpp's visualDirection*/
Qt::LayoutDirection DuiLayout::layoutDirection() const
{
    Q_D(const DuiLayout);
    if (QGraphicsItem *maybeWidget = d->parentItem()) {
        if (maybeWidget->isWidget())
            return static_cast<QGraphicsWidget *>(maybeWidget)->layoutDirection();
    }
    return QApplication::layoutDirection();
}
void DuiLayout::widgetEvent ( QEvent * event )
{
    Q_D(DuiLayout);
    if(event->type() == QEvent::ParentChange || event->type() == QEvent::PolishRequest) {
        d->recheckOrientation();
    } else if(event->type() == DuiOrientationChangeEvent::eventNumber()) {
        DuiOrientationChangeEvent *oce = static_cast<DuiOrientationChangeEvent *>(event);
        d->setOrientation(oce->orientation());
        activate();
    }

    QGraphicsLayout::widgetEvent(event);
}
