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

#include "mpageswitchanimation.h"
#include "mpageswitchanimation_p.h"

#include <MSceneWindow>

MPageSwitchAnimationPrivate::MPageSwitchAnimationPrivate()
    : MParallelAnimationGroupPrivate()
{
    newPage = NULL;
    oldPage = NULL;
    direction = MPageSwitchAnimation::ToChildPage;
}

MPageSwitchAnimationPrivate::~MPageSwitchAnimationPrivate()
{
}

MPageSwitchAnimation::MPageSwitchAnimation(QObject *parent)
    : MParallelAnimationGroup(new MPageSwitchAnimationPrivate, parent)
{
}

MPageSwitchAnimation::MPageSwitchAnimation(MPageSwitchAnimationPrivate *dd, QObject *parent)
    : MParallelAnimationGroup(dd, parent)
{
}

MPageSwitchAnimation::~MPageSwitchAnimation()
{
}


void MPageSwitchAnimation::setNewPage(MSceneWindow *newPage)
{
    Q_D(MPageSwitchAnimation);

    d->newPage = newPage;
}

void MPageSwitchAnimation::setOldPage(MSceneWindow *oldPage)
{
    Q_D(MPageSwitchAnimation);

    d->oldPage = oldPage;
}

void MPageSwitchAnimation::setTransitionDirection(TransitionDirection direction)
{
    Q_D(MPageSwitchAnimation);

    d->direction = direction;
}

MPageSwitchAnimation::TransitionDirection MPageSwitchAnimation::transitionDirection() const
{
    Q_D(const MPageSwitchAnimation);
    return d->direction;
}

MSceneWindow *MPageSwitchAnimation::oldPage() const
{
    Q_D(const MPageSwitchAnimation);
    return d->oldPage;
}

MSceneWindow *MPageSwitchAnimation::newPage() const
{
    Q_D(const MPageSwitchAnimation);
    return d->newPage;
}

#include "moc_mpageswitchanimation.cpp"

