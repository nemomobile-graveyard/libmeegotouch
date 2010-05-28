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

#ifndef MSCENEDIMMINGANIMATION_P_H
#define MSCENEDIMMINGANIMATION_P_H

#include "mparallelanimationgroup_p.h"
#include "mscenedimminganimation.h"

class MSceneDimmingAnimationPrivate : public MParallelAnimationGroupPrivate
{
    Q_DECLARE_PUBLIC(MSceneDimmingAnimation)
public:
    void init();

private:
    MSceneDimmingAnimation::TransitionDirection direction;
};

#endif // MSCENEDIMMINGANIMATION_P_H
