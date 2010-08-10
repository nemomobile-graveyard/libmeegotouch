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

#ifndef MPAGESWITCHSLIDEANIMATION_P_H
#define MPAGESWITCHSLIDEANIMATION_P_H

#include "mpageswitchanimation_p.h"
#include "mpageswitchslideanimation.h"
#include <QPointer>

class MSceneWindow;
class QPropertyAnimation;

class MPageSwitchSlideAnimationPrivate : public MPageSwitchAnimationPrivate
{
public:
    MPageSwitchSlideAnimationPrivate();
    virtual ~MPageSwitchSlideAnimationPrivate();
    void _q_animationFinished();

private:
    Q_DECLARE_PUBLIC(MPageSwitchSlideAnimation)

    MSceneWindow *sceneWindow;

    QPropertyAnimation *positionNewPageAnimation;
    QPropertyAnimation *positionOldPageAnimation;
};

#endif // MPAGESWITCHSLIDEANIMATION_P_H
