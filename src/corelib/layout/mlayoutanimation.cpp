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

#include "mlayoutanimation.h"
#include "mlayoutanimation_p.h"
#include "mlayout_p.h"
#include "manimation.h"

MLayoutAnimation::MLayoutAnimation(MLayout *layout)
    : MAnimation(NULL), d_ptr(new MLayoutAnimationPrivate)
{
    Q_ASSERT(layout);
    Q_D(MLayoutAnimation);
    d->layout = layout;
    layout->setAnimation(this);
}

MLayoutAnimation::MLayoutAnimation(MLayoutAnimationPrivate &p, MLayout *layout)
    : MAnimation(NULL), d_ptr(&p)
{
    Q_ASSERT(layout);
    Q_ASSERT(0 != d_ptr);
    Q_D(MLayoutAnimation);
    d->layout = layout;
    layout->setAnimation(this);
}

MLayout *MLayoutAnimation::layout() const
{
    Q_D(const MLayoutAnimation);
    return d->layout;
}

MLayoutAnimation::~MLayoutAnimation()
{
    delete d_ptr;
}

bool MLayoutAnimation::isAnimating() const
{
    return state() == QAbstractAnimation::Running;
}

void MLayoutAnimation::showItemNow(QGraphicsWidget *item)
{
    Q_D(MLayoutAnimation);
    Q_ASSERT(item);
    d->layout->d_ptr->showItemNow(item);
}
void MLayoutAnimation::hideItemNow(QGraphicsWidget *item)
{
    Q_D(MLayoutAnimation);
    Q_ASSERT(item);
    d->layout->d_ptr->hideItemNow(item);
}
MLayoutAnimationPrivate::MLayoutAnimationPrivate()
{
    layout = NULL;
    q_ptr = NULL;
}

