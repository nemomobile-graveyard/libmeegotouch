/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MPAGESWITCHANIMATION_P_H
#define MPAGESWITCHANIMATION_P_H

#include <mpageswitchanimation.h>
#include <mparallelanimationgroup_p.h>

class MPageSwitchAnimation;

class MPageSwitchAnimationPrivate : public MParallelAnimationGroupPrivate
{
public:
    MPageSwitchAnimationPrivate();
    virtual ~MPageSwitchAnimationPrivate();

private:
    Q_DECLARE_PUBLIC(MPageSwitchAnimation)

    QPointer<MSceneWindow> newPage;
    QPointer<MSceneWindow> oldPage;
    MPageSwitchAnimation::TransitionDirection direction;
};

#endif // MPAGESWITCHANIMATION_P_H
