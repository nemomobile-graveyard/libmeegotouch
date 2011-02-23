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

#include "mlayout_p.h"

#include "mabstractlayoutpolicy.h"
#include "mbasiclayoutanimation.h"
#include "mlayouthelper_p.h"
#include "mapplication.h"

#include "mwidget_p.h"

#include "mdebug.h"

#include <MScene>

MLayoutPrivate::MLayoutPrivate(MLayout *l) :
    q_ptr(l),
    portraitPolicy(NULL),
    landscapePolicy(NULL),
    current_policy(NULL),
    animation(NULL),
    m_orientation(M::Portrait)
{
    Q_ASSERT(0 != q_ptr);
}

void MLayoutPrivate::registerPolicy(MAbstractLayoutPolicy *policy)
{
    Q_Q(MLayout);
    Q_ASSERT(policy);
    if (policies.contains(policy))
        return;
    policies.append(policy);

    if (!current_policy)
        q->setPolicy(policy);
}

void MLayoutPrivate::unregisterPolicy(MAbstractLayoutPolicy *policy)
{
    Q_Q(MLayout);
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

void MLayoutPrivate::setItemGeometry(int index, const QRectF &geometry)
{
    Q_Q(MLayout);
    if (0 > index || q->count() <= index) {
        mWarning("MLayout") << Q_FUNC_INFO << "Attempting to set the geometry of the item at index" << index << "when there are" << q->count() << "items";
        return;
    }

    LayoutItem &item = items[index];
    item.geometry = geometry;

    if (animation && item.item->graphicsItem() && item.item->graphicsItem()->isWidget()) {
        animation->setItemGeometry(index, geometry);
    } else {
        // If no animator or the item is not a widget then do not animate
        Q_ASSERT(!item.toBeDeleted); //It should have been deleted immediately in there's no animator or it is a layout
        //Show item
        if(item.item->graphicsItem()) {
            MWidget *widget = dynamic_cast<MWidget *>(item.item->graphicsItem());
            if (widget)
                widget->d_ptr->layoutHidden = false;
            if (!widget || !widget->d_ptr->explicitlyHidden)
                item.item->graphicsItem()->show();
        }
        item.item->setGeometry(geometry);
    }
}

QRectF MLayoutPrivate::itemGeometry(int index) const
{
    Q_Q(const MLayout);
    if (0 > index || q->count() <= index) {
        mWarning("MLayout") << Q_FUNC_INFO << "Attempting to set the geometry of the item at index" << index << "when there are" << q->count() << "items";
        return QRectF();
    }

    return items.at(index).geometry;
}
void MLayoutPrivate::itemAnimationFinished(int index)
{
    Q_Q(MLayout);
    if (items.at(index).toBeDeleted)
        q->removeAt(index);  //This will delete the item as well
}

void MLayoutPrivate::animationFinished()
{
    Q_Q(MLayout);
    // check whether any item is now really deleted
    for (int i = items.size() - 1; i >= 0; --i) {
        if (items.at(i).toBeDeleted)
            q->removeAt(i);  //This will delete the item as well
    }
}

void MLayoutPrivate::hideItem(int index)
{
    const LayoutItem &item = items.at(index);
    QGraphicsItem *graphicsItem = item.item->graphicsItem();
    if(!graphicsItem)
        return;

    if (animation && graphicsItem->isWidget())
        animation->hideItem(index);
    else // If no animator, or the item is not a widget, or the widget is not visible, then do not animate
        hideItemNow(graphicsItem);
}

void MLayoutPrivate::setOrientation(M::Orientation orientation)
{
    Q_Q(MLayout);
    if(orientation == m_orientation)
        return;
    m_orientation = orientation;
    if (orientation == M::Portrait) {
        if (portraitPolicy)
            q->setPolicy(portraitPolicy);
    } else if (landscapePolicy) {
        q->setPolicy(landscapePolicy);
    }
    //since the rotation has changed, reread the values from the CSS config
    foreach(MAbstractLayoutPolicy * policy, policies) {
        Q_ASSERT(policy);
        policy->updateStyle();
    }
}
QGraphicsItem *MLayoutPrivate::parentItem() const
{
    Q_Q(const MLayout);

    const QGraphicsLayoutItem *parent = q;
    while (parent && parent->isLayout()) {
        parent = parent->parentLayoutItem();
    }
    return parent ? parent->graphicsItem() : 0;
}
M::Orientation MLayoutPrivate::orientation() const {
    return m_orientation;
}
void MLayoutPrivate::recheckOrientation(bool fallbackToActiveWindow) {
    //We need to check if the orientation has changed.
    QGraphicsItem *parent = parentItem();
    MWindow *window = NULL;
    if(parent) {
        QGraphicsWidget *w;
        if(parent->isWidget()) {
            w = static_cast<QGraphicsWidget*>(parent); //We can't assume it is a MWidget
        } else {
            w = parent->parentWidget();
            if(!w)
                return;
        }
        if(w->scene() && !w->scene()->views().isEmpty()) {
            window = qobject_cast<MWindow *>(w->scene()->views().at(0));
        }
    }

    if(!window && fallbackToActiveWindow)
        window = MApplication::activeWindow();
    if(window)
        setOrientation(window->orientation());
}
void MLayoutPrivate::showItemNow(QGraphicsItem *graphicsItem) const
{
    if(!graphicsItem)
        return;
    MWidget *widget = dynamic_cast<MWidget *>(graphicsItem);
    if (widget) {
        widget->d_ptr->layoutHidden = false;
        if (!widget->d_ptr->explicitlyHidden) {
            widget->d_ptr->setVisible(true); //Show only if item was not set to invisible by the user
        }
    } else
        graphicsItem->show(); //Show always for non-mwidgets
}
void MLayoutPrivate::removeHiddenFlag(QGraphicsItem *graphicsItem) const
{
    MWidget *widget = dynamic_cast<MWidget *>(graphicsItem);
    if (widget)
        widget->d_ptr->layoutHidden = false;
}
void MLayoutPrivate::hideItemNow(QGraphicsItem *graphicsItem) const
{
    MWidget *widget = dynamic_cast<MWidget *>(graphicsItem);
    if (widget) {
        widget->d_ptr->setVisible(false);
        widget->d_ptr->layoutHidden = true;
    } else {
        graphicsItem->hide();
    }
}

