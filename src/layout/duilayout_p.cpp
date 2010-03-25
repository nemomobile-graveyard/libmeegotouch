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

#include <DuiScene>

DuiLayoutPrivate::DuiLayoutPrivate(DuiLayout *l) :
    q_ptr(l),
    portraitPolicy(NULL),
    landscapePolicy(NULL),
    current_policy(NULL),
    animation(NULL),
    m_orientation(Dui::Portrait)
{
    Q_ASSERT(0 != q_ptr);
    if (DuiApplication::activeWindow())
        m_orientation = DuiApplication::activeWindow()->orientation();

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

    LayoutItem &item = items[index];
    item.geometry = geometry;

    if (animation && item.item->graphicsItem() && item.item->graphicsItem()->isWidget()) {
        animation->setItemGeometry(index, geometry);
    } else {
        // If no animator or the item is not a widget then do not animate
        Q_ASSERT(!item.toBeDeleted); //It should have been deleted immediately in there's no animator or it is a layout
        //Show item
        if(item.item->graphicsItem())
            showItemNow(item.item->graphicsItem());
        item.item->setGeometry(geometry);
    }
}

QRectF DuiLayoutPrivate::itemGeometry(int index) const
{
    Q_Q(const DuiLayout);
    if (0 > index || q->count() <= index) {
        duiWarning("DuiLayout") << Q_FUNC_INFO << "Attempting to set the geometry of the item at index" << index << "when there are" << q->count() << "items";
        return QRectF();
    }

    return items.at(index).geometry;
}
void DuiLayoutPrivate::itemAnimationFinished(int index)
{
    Q_Q(DuiLayout);
    if (items.at(index).toBeDeleted)
        q->removeAt(index);  //This will delete the item as well
}

void DuiLayoutPrivate::animationFinished()
{
    Q_Q(DuiLayout);
    // check whether any item is now really deleted
    for (int i = items.size() - 1; i >= 0; --i) {
        if (items.at(i).toBeDeleted)
            q->removeAt(i);  //This will delete the item as well
    }
}

void DuiLayoutPrivate::hideItem(int index)
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

void DuiLayoutPrivate::setOrientation(Dui::Orientation orientation)
{
    Q_Q(DuiLayout);
    if(orientation == m_orientation)
        return;
    m_orientation = orientation;
    if (orientation == Dui::Portrait) {
        if (portraitPolicy)
            q->setPolicy(portraitPolicy);
    } else if (landscapePolicy) {
        q->setPolicy(landscapePolicy);
    }
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
Dui::Orientation DuiLayoutPrivate::orientation() const {
    return m_orientation;
}
void DuiLayoutPrivate::recheckOrientation() {
    //We need to check if the orientation has changed.
    QGraphicsItem *parent = parentItem();
    if(parent) {
        QGraphicsWidget *w;
        if(parent->isWidget()) {
            w = static_cast<QGraphicsWidget*>(parent); //We can't assume it is a DuiWidget
        } else {
            w = parent->parentWidget();
            if(!w)
                return;
        }
        if(w->scene() && !w->scene()->views().isEmpty()) {
            DuiWindow *window = qobject_cast<DuiWindow *>(w->scene()->views().at(0));
            if(window) {
                setOrientation(window->orientation());
            }
        }
    }
}
void DuiLayoutPrivate::showItemNow(QGraphicsItem *graphicsItem)
{
    DuiWidget *widget = dynamic_cast<DuiWidget *>(graphicsItem);
    if (widget) {
        widget->d_ptr->layoutHidden = false;
        if (!widget->d_ptr->explicitlyHidden)
            graphicsItem->show(); //Show only if item was not set to invisible by the user
    } else
        graphicsItem->show(); //Show always for non-duiwidgets
}
void DuiLayoutPrivate::removeHiddenFlag(QGraphicsLayoutItem *layoutItem)
{
    DuiWidget *widget = dynamic_cast<DuiWidget *>(layoutItem->graphicsItem());
    if (widget)
        widget->d_ptr->layoutHidden = false;
}
void DuiLayoutPrivate::hideItemNow(QGraphicsItem *graphicsItem)
{
    graphicsItem->hide();
    DuiWidget *widget = dynamic_cast<DuiWidget *>(graphicsItem);
    if (widget)
        widget->d_ptr->layoutHidden = true;
}

