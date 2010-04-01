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

#include "duiabstractlayoutpolicy_p.h"
#include <DuiLayout>

DuiAbstractLayoutPolicyPrivate::DuiAbstractLayoutPolicyPrivate(DuiLayout *l) :
    layout(l),
    marginLeft(-1), //Negative number means to not override DuiLayout
    marginTop(-1),
    marginRight(-1),
    marginBottom(-1),
    horizontalSpacing(0),
    verticalSpacing(0),
    userSetVerticalSpacing(false),
    userSetHorizontalSpacing(false),
    userSetContentsMargins(false),
    style(NULL),
    removingFromLayout(false),
    q_ptr(NULL)
{
    Q_ASSERT(0 != layout);
}

DuiAbstractLayoutPolicyPrivate::~DuiAbstractLayoutPolicyPrivate()
{ }

void DuiAbstractLayoutPolicyPrivate::aboutToBeRemovedFromLayout(const QGraphicsLayoutItem *item)
{
    //The item is about to be removed from the layout, so we need to remove it from this policy too
    //This gets called even if the item is not in this policy, so that we can update the items mapping
    Q_Q(DuiAbstractLayoutPolicy);

    // First remove the item from this policy, if it is in this policy
    int policyIndex = q->indexOf(item);
    if (policyIndex > -1) {
        removingFromLayout = true;
        q->removeItem(item);
        removingFromLayout = false;
    }

    //Now update all of the indexes
    int layoutIndex = layout->indexOf(item); //This should always succeed
    Q_ASSERT(layoutIndex >= 0);

    const int count = items.count();
    for (int i = 0; i < count; ++i) {
        int currentLayoutIndex = items.at(i);
        if (currentLayoutIndex > layoutIndex)
            items[i] = currentLayoutIndex - 1;
    }
}

