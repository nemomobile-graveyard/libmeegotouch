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

#include "duibasiclayoutanimation_p.h"
#include "duilayout_p.h"

DuiBasicLayoutAnimationPrivate::DuiBasicLayoutAnimationPrivate()
{ }

DuiBasicLayoutAnimationPrivate::~DuiBasicLayoutAnimationPrivate()
{ }

void DuiBasicLayoutAnimationPrivate::tick()
{
    Q_Q(DuiBasicLayoutAnimation);
    recordedTimeSinceLastUpdate = timeSinceLastUpdate.elapsed();
    timeSinceLastUpdate.restart();

    bool layout_is_done(true);
    const int size = layout->d_ptr->states.size();
    for (int i = 0; i < size; ++i) {
        if (layout->d_ptr->states.at(i).isAnimationDone())
            continue;

        DuiItemState state(layout->d_ptr->states.at(i));
        q->animate(state);
        layout->d_ptr->states[i] = state;

        if (!layout->d_ptr->states.at(i).isAnimationDone())
            layout_is_done = false;
    }
    if (layout_is_done) {
        // notify layout and derived animators
        q->stopAnimation();
    }
}
