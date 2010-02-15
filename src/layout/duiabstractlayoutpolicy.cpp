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

#include "duiabstractlayoutpolicy.h"
#include "duiabstractlayoutpolicy_p.h"
#include "duiabstractlayoutpolicystyle.h"

#include "duilayout_p.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duitheme.h"
#include <DuiDebug>

DuiAbstractLayoutPolicy::DuiAbstractLayoutPolicy(DuiLayout *layout)
    :
    d_ptr(new DuiAbstractLayoutPolicyPrivate(layout))
{
    Q_ASSERT(layout);
    d_ptr->q_ptr = this;
    Q_D(DuiAbstractLayoutPolicy);
    d->layout->d_ptr->registerPolicy(this);

}
DuiAbstractLayoutPolicy::DuiAbstractLayoutPolicy(
    DuiAbstractLayoutPolicyPrivate &p) :
    d_ptr(&p)
{
    Q_ASSERT(0 != d_ptr);
    d_ptr->q_ptr = this;
    Q_D(DuiAbstractLayoutPolicy);
    d->layout->d_ptr->registerPolicy(this);
}

DuiAbstractLayoutPolicy::~DuiAbstractLayoutPolicy()
{
    Q_D(DuiAbstractLayoutPolicy);
    d->layout->d_ptr->unregisterPolicy(this);
    DuiTheme::releaseStyle(d->style);
    delete d_ptr;
}

DuiLayout *DuiAbstractLayoutPolicy::layout() const
{
    Q_D(const DuiAbstractLayoutPolicy);
    return d->layout;
}

void DuiAbstractLayoutPolicy::activate()
{
    layout()->setPolicy(this);
}

bool DuiAbstractLayoutPolicy::isActive() const
{
    return layout()->policy() == this;
}

void DuiAbstractLayoutPolicy::invalidate()
{
}

void DuiAbstractLayoutPolicy::invalidatePolicyAndLayout()
{
    Q_D(DuiAbstractLayoutPolicy);
    if (d->layout && d->layout->policy() == this)
        d->layout->QGraphicsLayout::invalidate();  //Invalidate the layout only, and not the policy again
    invalidate();
}

void DuiAbstractLayoutPolicy::updateGeometry()
{
    Q_D(DuiAbstractLayoutPolicy);
    if (d->layout && d->layout->policy() == this)
        d->layout->updateGeometry();
}

QRectF DuiAbstractLayoutPolicy::contentsArea() const
{
    Q_D(const DuiAbstractLayoutPolicy);
    qreal left, top, right, bottom;
    d->layout->getContentsMargins(&left, &top, &right, &bottom);
    return d->layout->geometry().adjusted(left, top, -right, -bottom);
}

void DuiAbstractLayoutPolicy::setContentsMargins(qreal left, qreal top,
        qreal right, qreal bottom)
{
    Q_D(DuiAbstractLayoutPolicy);

    d->marginLeft = left;
    d->marginTop = top;
    d->marginRight = right;
    d->marginBottom = bottom;

    d->userSetContentsMargins = true;

    invalidatePolicyAndLayout();
}

void DuiAbstractLayoutPolicy::getContentsMargins(qreal *left, qreal *top,
        qreal *right, qreal *bottom) const
{
    Q_D(const DuiAbstractLayoutPolicy);

    if (!d->userSetContentsMargins)
        (void) style(); //Make sure the style is loaded

    if (0 != left)
        *left = d->marginLeft;
    if (0 != top)
        *top = d->marginTop;
    if (0 != right)
        *right = d->marginRight;
    if (0 != bottom)
        *bottom = d->marginBottom;
}

void DuiAbstractLayoutPolicy::updateStyle()
{
    Q_D(DuiAbstractLayoutPolicy);

    Dui::Orientation orientation = Dui::Landscape;
    if (DuiApplication::activeWindow()) {
        orientation = DuiApplication::activeWindow()->orientation();
    }

    const DuiAbstractLayoutPolicyStyle *style =
        static_cast<const DuiAbstractLayoutPolicyStyle *>(DuiTheme::style(
                    "DuiAbstractLayoutPolicyStyle", d->objectName, "", "",
                    orientation));

    if (d->style != style) {
        DuiTheme::releaseStyle(d->style);
        d->style = style;
        applyStyle();
    } else {
        DuiTheme::releaseStyle(style);
    }
}

void DuiAbstractLayoutPolicy::unsetContentsMargins()
{
    Q_D(DuiAbstractLayoutPolicy);
    if (!d->userSetContentsMargins)
        return;  //already unset
    d->userSetContentsMargins = false;
    (void)style(); //Force the style to be loaded
    applyStyle();
}
void DuiAbstractLayoutPolicy::applyStyle()
{
    Q_D(DuiAbstractLayoutPolicy);
    //Do not change the margins if the user manually set them or the margins
    //have not changed
    if (!d->userSetContentsMargins &&
            (d->marginLeft != (int)d->style->marginLeft() ||
             d->marginTop != (int)d->style->marginTop() ||
             d->marginRight != (int)d->style->marginRight() ||
             d->marginBottom != (int)d->style->marginBottom())) {
        d->marginLeft = (int)d->style->marginLeft();
        d->marginTop = (int)d->style->marginTop();
        d->marginRight = (int)d->style->marginRight();
        d->marginBottom = (int)d->style->marginBottom();
        invalidatePolicyAndLayout();
    }

    if (!d->userSetHorizontalSpacing) {
        setHorizontalSpacing(d->style->horizontalSpacing()); //This is a virtual function so we have to call it so that subclasses receive this
        d->userSetHorizontalSpacing = false;
    }

    if (!d->userSetVerticalSpacing) {
        setVerticalSpacing(d->style->verticalSpacing()); //This is a virtual function so we have to call it so that subclasses receive this
        d->userSetVerticalSpacing = false;
    }
}

void DuiAbstractLayoutPolicy::setObjectName(const QString &name)
{
    Q_D(DuiAbstractLayoutPolicy);
    if (name == d->objectName)
        return;
    d->objectName = name;
    updateStyle();
}
QString DuiAbstractLayoutPolicy::objectName() const
{
    Q_D(const DuiAbstractLayoutPolicy);
    return d->objectName;
}

const DuiAbstractLayoutPolicyStyle *DuiAbstractLayoutPolicy::style() const
{
    Q_D(const DuiAbstractLayoutPolicy);
    if (!d->style)
        const_cast<DuiAbstractLayoutPolicy *>(this)->updateStyle();
    return d->style;
}

void DuiAbstractLayoutPolicy::setHorizontalSpacing(qreal spacing)
{
    Q_D(DuiAbstractLayoutPolicy);
    d->userSetHorizontalSpacing = true;
    if (spacing == d->horizontalSpacing)
        return;
    d->horizontalSpacing = spacing;
    invalidatePolicyAndLayout();
}

qreal DuiAbstractLayoutPolicy::horizontalSpacing() const
{
    Q_D(const DuiAbstractLayoutPolicy);
    if (!d->userSetHorizontalSpacing)
        (void) style(); //Make sure the style is loaded
    return d->horizontalSpacing;
}

void DuiAbstractLayoutPolicy::setVerticalSpacing(qreal spacing)
{
    Q_D(DuiAbstractLayoutPolicy);
    d->userSetVerticalSpacing = true;
    if (spacing == d->verticalSpacing)
        return;
    d->verticalSpacing = spacing;
    invalidatePolicyAndLayout();
}

qreal DuiAbstractLayoutPolicy::verticalSpacing() const
{
    Q_D(const DuiAbstractLayoutPolicy);
    if (!d->userSetVerticalSpacing)
        (void) style(); //Make sure the style is loaded
    return d->verticalSpacing;
}

void DuiAbstractLayoutPolicy::setSpacing(qreal spacing)
{
    setHorizontalSpacing(spacing);
    setVerticalSpacing(spacing);
}

int DuiAbstractLayoutPolicy::count() const
{
    Q_D(const DuiAbstractLayoutPolicy);
    return d->items.count();
}

void DuiAbstractLayoutPolicy::addItem(QGraphicsLayoutItem *item)
{
    DuiAbstractLayoutPolicy::insertItem(count(), item);
}
void DuiAbstractLayoutPolicy::insertItem(int index, QGraphicsLayoutItem *item)
{
    Q_D(DuiAbstractLayoutPolicy);
    if (!item)
        return;

    //Note that we do not show the item, since for most policies we do not know where to place it
    //until we have called relayout().  So instead we invalidate the policy, which we trigger a relayout, which will set
    //the item's positions

    int layoutIndex = d->layout->addItem(item);
    d->items.insert(index, layoutIndex);
    //Invalidate the policy to trigger a relayout to place the new item.
    //FIXME: For some policies, this is not optimal, since we might know the position of the new item without having to layout everything
    invalidatePolicyAndLayout();
}

QGraphicsLayoutItem *DuiAbstractLayoutPolicy::itemAt(int index) const
{
    Q_D(const DuiAbstractLayoutPolicy);
    if (index >= d->items.count() || index < 0)
        return NULL;
    return layout()->itemAt(d->items.at(index));
}

void DuiAbstractLayoutPolicy::setItemGeometry(int index, const QRectF &geometry)
{
    Q_D(DuiAbstractLayoutPolicy);
    if (isActive())
        d->layout->d_ptr->setItemGeometry(d->items.at(index), geometry);
}

QRectF DuiAbstractLayoutPolicy::itemGeometry(int index) const
{
    Q_D(const DuiAbstractLayoutPolicy);
    return d->layout->d_ptr->itemGeometry(d->items.at(index));
}
void DuiAbstractLayoutPolicy::hideItem(int index)
{
    Q_D(DuiAbstractLayoutPolicy);
    d->layout->d_ptr->hideItem(d->items.at(index));
}
void DuiAbstractLayoutPolicy::removeAt(int index)
{
    Q_D(DuiAbstractLayoutPolicy);

    if (isActive() && !d->removingFromLayout) //Do not hide the item if it's going to be removed from the layout totally
        hideItem(index);
    d->items.removeAt(index);
    //This is an overkill - not all policies require laying out all the items again when one is removed
    //(e.g. freestyle)
    updateGeometry();
    invalidatePolicyAndLayout();
}

void DuiAbstractLayoutPolicy::removeItem(const QGraphicsLayoutItem *item)
{
    int index = indexOf(item);
    if (index >= 0)
        removeAt(index);
}

int DuiAbstractLayoutPolicy::indexOf(const QGraphicsLayoutItem *item) const
{
    const int theCount = count();
    for (int i = 0; i < theCount; ++i) {
        if (itemAt(i) == item)
            return i;
    }
    return -1;
}

void DuiAbstractLayoutPolicy::activated()
{
    Q_D(DuiAbstractLayoutPolicy);
    const int count = d->layout->count();
    for (int i = 0; i < count; ++i) {
        if (indexOf(d->layout->itemAt(i)) == -1)
            d->layout->d_ptr->hideItem(i); //Item is not the policy so hide it
        //if the item is currently hidden but is in the new layout, it will be shown when relayout() calls setTargetGeometry()
    }
    //Note that this is called from the constructor, so we can't do anything here that
    //would update the style since this class is not fully constructed yet
}

void DuiAbstractLayoutPolicy::aboutToBeRemovedFromLayout(const QGraphicsLayoutItem *item)
{
    Q_D(DuiAbstractLayoutPolicy);
    d->aboutToBeRemovedFromLayout(item);
}

