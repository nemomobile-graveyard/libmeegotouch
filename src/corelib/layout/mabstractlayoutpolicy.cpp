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

#include "mabstractlayoutpolicy.h"
#include "mabstractlayoutpolicy_p.h"
#include "mabstractlayoutpolicystyle.h"

#include "mlayout_p.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mtheme.h"
#include <MDebug>

MAbstractLayoutPolicy::MAbstractLayoutPolicy(MLayout *layout)
    :
    d_ptr(new MAbstractLayoutPolicyPrivate(layout))
{
    Q_ASSERT(layout);
    d_ptr->q_ptr = this;
    Q_D(MAbstractLayoutPolicy);
    d->layout->d_ptr->registerPolicy(this);

}
MAbstractLayoutPolicy::MAbstractLayoutPolicy(
    MAbstractLayoutPolicyPrivate &p) :
    d_ptr(&p)
{
    Q_ASSERT(0 != d_ptr);
    d_ptr->q_ptr = this;
    Q_D(MAbstractLayoutPolicy);
    d->layout->d_ptr->registerPolicy(this);
}

MAbstractLayoutPolicy::~MAbstractLayoutPolicy()
{
    Q_D(MAbstractLayoutPolicy);
    d->layout->d_ptr->unregisterPolicy(this);
    MTheme::releaseStyle(d->style);
    delete d_ptr;
}

MLayout *MAbstractLayoutPolicy::layout() const
{
    Q_D(const MAbstractLayoutPolicy);
    return d->layout;
}

void MAbstractLayoutPolicy::activate()
{
    layout()->setPolicy(this);
}

bool MAbstractLayoutPolicy::isActive() const
{
    return layout()->policy() == this;
}

void MAbstractLayoutPolicy::invalidate()
{
}

void MAbstractLayoutPolicy::invalidatePolicyAndLayout()
{
    Q_D(MAbstractLayoutPolicy);
    if (d->layout && d->layout->policy() == this)
        d->layout->QGraphicsLayout::invalidate();  //Invalidate the layout only, and not the policy again
    invalidate();
}

void MAbstractLayoutPolicy::updateGeometry()
{
    Q_D(MAbstractLayoutPolicy);
    if (d->layout && d->layout->policy() == this)
        d->layout->updateGeometry();
}

QRectF MAbstractLayoutPolicy::contentsArea() const
{
    Q_D(const MAbstractLayoutPolicy);
    qreal left, top, right, bottom;
    d->layout->getContentsMargins(&left, &top, &right, &bottom);
    return d->layout->geometry().adjusted(left, top, -right, -bottom);
}

void MAbstractLayoutPolicy::setContentsMargins(qreal left, qreal top,
        qreal right, qreal bottom)
{
    Q_D(MAbstractLayoutPolicy);

    d->marginLeft = left;
    d->marginTop = top;
    d->marginRight = right;
    d->marginBottom = bottom;

    d->userSetContentsMargins = true;

    invalidatePolicyAndLayout();
}

void MAbstractLayoutPolicy::getContentsMargins(qreal *left, qreal *top,
        qreal *right, qreal *bottom) const
{
    Q_D(const MAbstractLayoutPolicy);

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

void MAbstractLayoutPolicy::updateStyle()
{
    Q_D(MAbstractLayoutPolicy);

    M::Orientation orientation = d->layout->d_ptr->orientation();

    const MAbstractLayoutPolicyStyle *style =
        static_cast<const MAbstractLayoutPolicyStyle *>(MTheme::style(
                    "MAbstractLayoutPolicyStyle", d->styleName, "", "",
                    orientation));

    if (d->style != style) {
        MTheme::releaseStyle(d->style);
        d->style = style;
        applyStyle();
    } else {
        MTheme::releaseStyle(style);
    }
}

void MAbstractLayoutPolicy::unsetContentsMargins()
{
    Q_D(MAbstractLayoutPolicy);
    if (!d->userSetContentsMargins)
        return;  //already unset
    d->userSetContentsMargins = false;
    (void)style(); //Force the style to be loaded
    applyStyle();
}
void MAbstractLayoutPolicy::applyStyle()
{
    Q_D(MAbstractLayoutPolicy);
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

void MAbstractLayoutPolicy::setObjectName(const QString &name)
{
    mWarning("MAbstractLayoutPolicy") << Q_FUNC_INFO << "is deprecated. Use setStyleName() instead.";
    setStyleName(name);
}
void MAbstractLayoutPolicy::setStyleName(const QString &name)
{
    Q_D(MAbstractLayoutPolicy);
    if (name == d->styleName)
        return;
    d->styleName = name;
    updateStyle();
}
QString MAbstractLayoutPolicy::objectName() const
{
    mWarning("MAbstractLayoutPolicy") << Q_FUNC_INFO << "is deprecated. Use styleName() instead.";
    return styleName();
}
QString MAbstractLayoutPolicy::styleName() const
{
    Q_D(const MAbstractLayoutPolicy);
    return d->styleName;
}

const MAbstractLayoutPolicyStyle *MAbstractLayoutPolicy::style() const
{
    Q_D(const MAbstractLayoutPolicy);
    if (!d->style)
        const_cast<MAbstractLayoutPolicy *>(this)->updateStyle();
    return d->style;
}

void MAbstractLayoutPolicy::setHorizontalSpacing(qreal spacing)
{
    Q_D(MAbstractLayoutPolicy);
    d->userSetHorizontalSpacing = true;
    if (spacing == d->horizontalSpacing)
        return;
    d->horizontalSpacing = spacing;
    invalidatePolicyAndLayout();
}

qreal MAbstractLayoutPolicy::horizontalSpacing() const
{
    Q_D(const MAbstractLayoutPolicy);
    if (!d->userSetHorizontalSpacing)
        (void) style(); //Make sure the style is loaded
    return d->horizontalSpacing;
}

void MAbstractLayoutPolicy::setVerticalSpacing(qreal spacing)
{
    Q_D(MAbstractLayoutPolicy);
    d->userSetVerticalSpacing = true;
    if (spacing == d->verticalSpacing)
        return;
    d->verticalSpacing = spacing;
    invalidatePolicyAndLayout();
}

qreal MAbstractLayoutPolicy::verticalSpacing() const
{
    Q_D(const MAbstractLayoutPolicy);
    if (!d->userSetVerticalSpacing)
        (void) style(); //Make sure the style is loaded
    return d->verticalSpacing;
}

void MAbstractLayoutPolicy::setSpacing(qreal spacing)
{
    setHorizontalSpacing(spacing);
    setVerticalSpacing(spacing);
}

int MAbstractLayoutPolicy::count() const
{
    Q_D(const MAbstractLayoutPolicy);
    return d->items.count();
}

void MAbstractLayoutPolicy::addItem(QGraphicsLayoutItem *item)
{
    MAbstractLayoutPolicy::insertItem(count(), item);
}
void MAbstractLayoutPolicy::insertItem(int index, QGraphicsLayoutItem *item)
{
    Q_D(MAbstractLayoutPolicy);
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

QGraphicsLayoutItem *MAbstractLayoutPolicy::itemAt(int index) const
{
    Q_D(const MAbstractLayoutPolicy);
    if (index >= d->items.count() || index < 0)
        return NULL;
    return layout()->itemAt(d->items.at(index));
}

void MAbstractLayoutPolicy::setItemGeometry(int index, const QRectF &geometry)
{
    Q_D(MAbstractLayoutPolicy);
    if (isActive())
        d->layout->d_ptr->setItemGeometry(d->items.at(index), geometry);
}

QRectF MAbstractLayoutPolicy::itemGeometry(int index) const
{
    Q_D(const MAbstractLayoutPolicy);
    return d->layout->d_ptr->itemGeometry(d->items.at(index));
}
void MAbstractLayoutPolicy::hideItem(int index)
{
    Q_D(MAbstractLayoutPolicy);
    d->layout->d_ptr->hideItem(d->items.at(index));
}
void MAbstractLayoutPolicy::removeAt(int index)
{
    Q_D(MAbstractLayoutPolicy);

    if (isActive() && !d->removingFromLayout) //Do not hide the item if it's going to be removed from the layout totally
        hideItem(index);
    d->items.removeAt(index);
    //This is an overkill - not all policies require laying out all the items again when one is removed
    //(e.g. freestyle)
    updateGeometry();
    invalidatePolicyAndLayout();
}

void MAbstractLayoutPolicy::removeItem(const QGraphicsLayoutItem *item)
{
    int index = indexOf(item);
    if (index >= 0)
        removeAt(index);
}

int MAbstractLayoutPolicy::indexOf(const QGraphicsLayoutItem *item) const
{
    const int theCount = count();
    for (int i = 0; i < theCount; ++i) {
        if (itemAt(i) == item)
            return i;
    }
    return -1;
}

void MAbstractLayoutPolicy::activated()
{
    Q_D(MAbstractLayoutPolicy);
    const int count = d->layout->count();
    for (int i = 0; i < count; ++i) {
        if (indexOf(d->layout->itemAt(i)) == -1)
            d->layout->d_ptr->hideItem(i); //Item is not the policy so hide it
        //if the item is currently hidden but is in the new layout, it will be shown when relayout() calls setTargetGeometry()
    }
    //Note that this is called from the constructor, so we can't do anything here that
    //would update the style since this class is not fully constructed yet
}

void MAbstractLayoutPolicy::aboutToBeRemovedFromLayout(const QGraphicsLayoutItem *item)
{
    Q_D(MAbstractLayoutPolicy);
    d->aboutToBeRemovedFromLayout(layout()->indexOf(item));
}

void MAbstractLayoutPolicy::setHeightForWidth(bool hasHeightForWidth)
{
    Q_D(MAbstractLayoutPolicy);
    if (d->hasHeightForWidth == hasHeightForWidth)
        return;
    d->hasHeightForWidth = hasHeightForWidth;
    if (d->layout && d->layout->policy() == this) {
        QSizePolicy newSizePolicy(d->layout->sizePolicy());
        newSizePolicy.setHeightForWidth(hasHeightForWidth);
        d->layout->setSizePolicy(newSizePolicy);
    }
    invalidatePolicyAndLayout();
}

bool MAbstractLayoutPolicy::hasHeightForWidth() const
{
    Q_D(const MAbstractLayoutPolicy);
    return d->hasHeightForWidth;
}
