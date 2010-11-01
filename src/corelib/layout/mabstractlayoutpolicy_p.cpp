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

#include "mabstractlayoutpolicy_p.h"
#include <MLayout>

MAbstractLayoutPolicyPrivate::MAbstractLayoutPolicyPrivate(MLayout *l) :
    layout(l),
    marginLeft(-1), //Negative number means to not override MLayout
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
    hasHeightForWidth(false),
    q_ptr(NULL)
{
    Q_ASSERT(0 != layout);
}

MAbstractLayoutPolicyPrivate::~MAbstractLayoutPolicyPrivate()
{ }

void MAbstractLayoutPolicyPrivate::aboutToBeRemovedFromLayout(int layoutIndex)
{
    //The item is about to be removed from the layout.  It has already been removed from the policy
    //so this gets called so that we can update the items mappings.
    Q_ASSERT(layoutIndex >= 0);
    //Now update all of the indexes

    const int count = items.count();
    for (int i = 0; i < count; ++i) {
        int currentLayoutIndex = items.at(i);
        Q_ASSERT(currentLayoutIndex != layoutIndex);
        if (currentLayoutIndex > layoutIndex)
            items[i] = currentLayoutIndex - 1;
    }
}

