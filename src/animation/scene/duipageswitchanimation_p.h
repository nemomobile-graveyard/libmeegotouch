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

#ifndef DUIPAGESWITCHANIMATION_P_H
#define DUIPAGESWITCHANIMATION_P_H

#include "duipageswitchanimation.h"
#include "duiparallelanimationgroup_p.h"
#include <QPointer>

class DuiSceneWindow;
class QPropertyAnimation;

class DuiPageSwitchAnimationPrivate : public DuiParallelAnimationGroupPrivate
{
    Q_DECLARE_PUBLIC(DuiPageSwitchAnimation)
public:
    DuiSceneWindow *sceneWindow;

    void _q_animationFinished();

protected:
    QPointer<DuiSceneWindow> newPage;
    QPointer<DuiSceneWindow> oldPage;

    QPropertyAnimation *positionNewPageAnimation;
    QPropertyAnimation *positionOldPageAnimation;

    DuiPageSwitchAnimation::PageTransitionDirection direction;
};

#endif
