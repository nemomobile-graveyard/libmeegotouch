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

#include "mlayout.h"
#include "mlayout_p.h"

#include "mapplication.h"
#include "mabstractlayoutpolicy.h"
#include "mabstractlayoutpolicy_p.h"
#include "mbasiclayoutanimation.h"
#include "morientationchangeevent.h"

#include <QGraphicsItem>
#include <QApplication>
#include <MDebug>

MLayout::MLayout(QGraphicsLayoutItem *parent) :
    QGraphicsLayout(parent),
    d_ptr(new MLayoutPrivate(this))
{
    Q_ASSERT(0 != d_ptr);
    d_ptr->recheckOrientation(false);
    // Disabling layout animations by default
    // new MBasicLayoutAnimation(this);
}

MLayout::~MLayout()
{
    Q_D(MLayout);
    d->current_policy = NULL;  //Make sure we have no policy before

    qDeleteAll(d->policies);
    delete d->animation;
    //Remove all the items from the layout and delete any layouts
    //(which are ownedByLayout) inside this layout
    //Because we don't have any policies, this should be pretty fast
    for (int i = count() - 1; i >= 0; --i) {
        const MLayoutPrivate::LayoutItem &item = d->items.at(i);
        if (item.item) {
            item.item->setParentLayoutItem(0);
            if (item.item->ownedByLayout() || item.toBeDeleted)
                delete item.item;
        }
    }
    delete d_ptr;
}

// Reimplemented from QGraphicsLayout
void MLayout::invalidate()
{
    Q_D(MLayout);
    //We need to check if the orientation has changed.  This can cause the current policy to change
    //Unfortunately, this is the only function called when widget->setLayout(layout)  is called.
    d->recheckOrientation();

    if (d->current_policy)
        d->current_policy->invalidate();
    QGraphicsLayout::invalidate();
    //This following warning is useful, but too many false negatives :-/
//    if(!parentLayoutItem())
//        mWarning("MLayout") << "MLayout does not have a parent - layouting will not be correct.  Add the layout to a QGraphicsWidget";
}

void MLayout::invalidateLayoutOnly()
{
    QGraphicsLayout::invalidate();
}

int MLayout::count() const
{
    Q_D(const MLayout);
    return d->items.count();
}
bool MLayout::isEmpty() const
{
    Q_D(const MLayout);
    return d->items.isEmpty();
}
QGraphicsLayoutItem *MLayout::itemAt(int index) const
{
    Q_D(const MLayout);
    if (0 > index || count() <= index) {
        mWarning("MLayout") << Q_FUNC_INFO << "Attempting to access item at index" << index << "when there are" << count() << "items";
        return NULL;
    }
    return d->items.at(index).item;
}

void MLayout::removeAt(int index)
{
    (void)takeAt(index);
}

QGraphicsLayoutItem *MLayout::takeAt(int index)
{
    Q_D(MLayout);
    if (0 > index || count() <= index) {
        if (index != 0) //It's common to just try deleting the first item until there are none left, so suppress this warning for index=0
            mWarning("MLayout") << Q_FUNC_INFO << "Attempting to remove item at index" << index << "when there are" << count() << "items";
        return NULL;
    }
    bool toBeDeleted = d->items.at(index).toBeDeleted;
    QGraphicsLayoutItem *item = d->items.at(index).item;
    Q_ASSERT(item);
    //First remove the item from all the policies, then update the indexes using aboutToBeRemovedFromLayout
    //We cannot do this in once pass since removeAt() is virtual and can do pretty much anything
    foreach(MAbstractLayoutPolicy * policy, d->policies) {
        Q_ASSERT(policy);
        int policyIndex = policy->indexOf(item);
        if (policyIndex >= 0 ) {
            policy->d_ptr->removingFromLayout = true;  //Indicate to the policy to remove the item but not to hide it
            policy->removeAt(policyIndex);
            policy->d_ptr->removingFromLayout = false;
        }
    }
    foreach(MAbstractLayoutPolicy * policy, d->policies) {
        policy->d_ptr->aboutToBeRemovedFromLayout(index);
    }
    if (d->animation)
        d->animation->itemRemovedFromLayout(index);

    d->items.removeAt(index);
    item->setParentLayoutItem(NULL);
    invalidate();
    if (toBeDeleted) {
        delete item;
        return NULL;
    }
    /* We cannot actually show the item as it might be in the process of being deleted */
    d->removeHiddenFlag(item->graphicsItem());
    return item;
}

void MLayout::animatedDeleteItem(const QGraphicsLayoutItem * item)
{
    animatedDeleteAt(indexOf(item));
}

void MLayout::animatedDeleteAt(int index)
{
    Q_D(MLayout);
    if (0 > index || count() <= index) {
        if (index != 0)
            mWarning("MLayout") << Q_FUNC_INFO << "Attempting to remove item at index" << index << "when there are" << count() << "items";
        return;
    }

    const MLayoutPrivate::LayoutItem &item = d->items.at(index);
    // Abort if already scheduled for deletion
    // (this can happen because a graphics layout item removes itself from its
    //  layout upon destruction and would lead to an infinite recursion here)
    if (item.toBeDeleted)
        return;

    if (d->animation && item.item->graphicsItem() && item.item->graphicsItem()->isWidget()) {
        if (item.toBeDeleted)
            return; //Already set
        d->items[index].toBeDeleted = true;
        d->animation->animatedDeleteItem(index);
    } else {
        delete takeAt(index); //Delete immediately if we can't animate it
    }
}

// QGraphicsLayoutItem:

QSizeF MLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MLayout);
    if (d->current_policy) {
        return d->current_policy->sizeHint(which, constraint);
    }
    return QSizeF(-1, -1);
}

void MLayout::setGeometry(const QRectF &rect)
{
    Q_D(MLayout);
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

int MLayout::addItem(QGraphicsLayoutItem *item)
{
    Q_D(MLayout);
    // do we know this item already?
    if (!item)
        return -1; //error
    int index = indexOf(item);
    if (index < 0) {
        // If the item is a layout, we need to add all of its items
        addChildLayoutItem(item);
        // Add item
        index = d->items.size();

        MLayoutPrivate::LayoutItem layoutItem;
        layoutItem.item = item;
        layoutItem.toBeDeleted = false;
        layoutItem.geometry = item->geometry();
        d->items.append(layoutItem);
        // Hide item for now since we do not know where to place it
        if(item->graphicsItem())
            d->hideItemNow(item->graphicsItem());
        if(d->animation)
            d->animation->itemAddedToLayout(index);
    }
    return index;
}

void MLayout::removeItem(const QGraphicsLayoutItem *const item)
{
    if (0 == item) {
        return;
    }

    int index = indexOf(item);
    if (index == -1)
        return; //Should we display a warning?
    removeAt(index);
}

int MLayout::indexOf(const QGraphicsItem *item) const
{
    Q_D(const MLayout);
    if (0 == item) {
        return -1;
    }

    const int theCount = count();
    for (int i = 0; i < theCount; ++i) {
        if (d->items.at(i).item->graphicsItem() == item) {
            return i;
        }
    }

    return -1;
}

int MLayout::indexOf(const QGraphicsLayoutItem *item) const
{
    Q_D(const MLayout);
    if (0 == item) {
        return -1;
    }

    const int theCount = count();
    for (int i = 0; i < theCount; ++i) {
        if (d->items.at(i).item == item) {
            return i;
        }
    }

    return -1;
}

void MLayout::setPolicy(MAbstractLayoutPolicy *policy)
{
    Q_D(MLayout);
    if (d->current_policy == policy)
        return;

    d->current_policy = policy;

    QSizePolicy newSizePolicy(sizePolicy());
    newSizePolicy.setHeightForWidth(d->current_policy?d->current_policy->hasHeightForWidth():false);
    setSizePolicy(newSizePolicy);

    if (d->current_policy) {
        Q_ASSERT(d->policies.contains(policy));
        policy->activated();
    }
    invalidate();
    //Note that the policy might not be fully constructed yet, since we might still be in the
    //MAbstractLayoutPolicy constructor.  So we cannot immediately do a re-layout - we have to
    //invalidate, waiting until the next event loop to activate()
}
void MLayout::setLandscapePolicy(MAbstractLayoutPolicy *policy)
{
    Q_D(MLayout);
    if (d->landscapePolicy == policy)
        return;
    d->landscapePolicy = policy;
    Q_ASSERT(!policy || policy->layout() == this);
    d->recheckOrientation();
    if (d->landscapePolicy && d->m_orientation == M::Landscape) {
        policy->activate();
    }
}
MAbstractLayoutPolicy *MLayout::landscapePolicy() const
{
    Q_D(const MLayout);
    return d->landscapePolicy;
}

void MLayout::setPortraitPolicy(MAbstractLayoutPolicy *policy)
{
    Q_D(MLayout);
    if (d->portraitPolicy == policy)
        return;
    d->portraitPolicy = policy;
    Q_ASSERT(!policy || policy->layout() == this);
    d->recheckOrientation();
    if (d->portraitPolicy && d->m_orientation == M::Portrait) {
        policy->activate();
    }
}
MAbstractLayoutPolicy *MLayout::portraitPolicy() const
{
    Q_D(const MLayout);
    return d->portraitPolicy;
}

MAbstractLayoutPolicy *MLayout::policy() const
{
    Q_D(const MLayout);
    return d->current_policy;
}

QList<MAbstractLayoutPolicy *> MLayout::registeredPolicies() const
{
    Q_D(const MLayout);
    return d->policies;
}

void MLayout::setAnimation(MLayoutAnimation *animation)
{
    Q_D(MLayout);
    delete d->animation;
    d->animation = animation;
}

MLayoutAnimation *MLayout::animation() const
{
    Q_D(const MLayout);
    return d->animation;
}

void MLayout::setContentsMargins(qreal left, qreal top, qreal right, qreal bottom)
{
    QGraphicsLayout::setContentsMargins(left, top, right, bottom);
}

void MLayout::getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const
{
    Q_D(const MLayout);
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
Qt::LayoutDirection MLayout::layoutDirection() const
{
    Q_D(const MLayout);
    if (QGraphicsItem *maybeWidget = d->parentItem()) {
        if (maybeWidget->isWidget())
            return static_cast<QGraphicsWidget *>(maybeWidget)->layoutDirection();
    }
    return QApplication::layoutDirection();
}
void MLayout::widgetEvent ( QEvent * event )
{
    Q_D(MLayout);
    if(event->type() == QEvent::ParentChange || event->type() == QEvent::PolishRequest) {
        d->recheckOrientation();
    } else if(event->type() == MOrientationChangeEvent::eventNumber()) {
        MOrientationChangeEvent *oce = static_cast<MOrientationChangeEvent *>(event);
        d->setOrientation(oce->orientation());
        QGraphicsLayout::invalidate();
    }

    QGraphicsLayout::widgetEvent(event);
}
