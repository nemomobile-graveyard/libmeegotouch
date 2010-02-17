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

#include "duilayoutanimation.h"
#include "duilayoutanimation_p.h"
#include "duilayout_p.h"
#include "duianimation.h"

DuiLayoutAnimation::DuiLayoutAnimation(DuiLayout *layout)
    : DuiAnimation(NULL), d_ptr(new DuiLayoutAnimationPrivate)
{
    Q_ASSERT(layout);
    Q_D(DuiLayoutAnimation);
    d->layout = layout;
    layout->setAnimation(this);
}

DuiLayoutAnimation::DuiLayoutAnimation(DuiLayoutAnimationPrivate &p, DuiLayout *layout)
    : DuiAnimation(NULL), d_ptr(&p)
{
    Q_ASSERT(layout);
    Q_ASSERT(0 != d_ptr);
    Q_D(DuiLayoutAnimation);
    d->layout = layout;
    layout->setAnimation(this);
}

DuiLayout *DuiLayoutAnimation::layout() const
{
    Q_D(const DuiLayoutAnimation);
    return d->layout;
}

DuiLayoutAnimation::~DuiLayoutAnimation()
{
    delete d_ptr;
}

void DuiLayoutAnimation::startAnimation(DuiItemState *itemstate)
{
    layoutAnimationStarted(itemstate);
}
bool DuiLayoutAnimation::isAnimating() const
{
    return state() == QAbstractAnimation::Running;
}
void DuiLayoutAnimation::stopAnimation()
{
    Q_D(DuiLayoutAnimation);
    if (!isAnimating())
        return;
    d->layout->d_ptr->animationFinished();
    layoutAnimationFinished();
}

void DuiLayoutAnimation::showItemNow(QGraphicsLayoutItem *item)
{
    Q_D(DuiLayoutAnimation);
    Q_ASSERT(item);
    d->layout->d_ptr->showItemNow(item);
}
void DuiLayoutAnimation::hideItemNow(QGraphicsLayoutItem *item)
{
    Q_D(DuiLayoutAnimation);
    Q_ASSERT(item);
    d->layout->d_ptr->hideItemNow(item);
}
void DuiLayoutAnimationPrivate()
{
    layout = NULL;
    q_ptr = NULL;
}
