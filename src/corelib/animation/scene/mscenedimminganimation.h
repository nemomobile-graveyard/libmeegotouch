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

#ifndef MSCENEDIMMINGANIMATION_H
#define MSCENEDIMMINGANIMATION_H

#include <mparallelanimationgroup.h>
#include <mscenedimminganimationstyle.h>

class MSceneDimmingAnimationPrivate;
class MWidgetController;
class MGraphicsEffect;
class QPropertyAnimation;

//! \internal
class MSceneDimmingAnimation : public MParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MSceneDimmingAnimation)
    M_ANIMATION_GROUP(MSceneDimmingAnimationStyle)

public:
    explicit MSceneDimmingAnimation(QObject *parent = NULL);
    virtual ~MSceneDimmingAnimation();

    enum TransitionDirection {
        In,
        Out
    };

    void setTransitionDirection(TransitionDirection direction);

    void setStyleParent(MWidgetController *widget);

    void addGraphicsEffect(MGraphicsEffect* effect);
};
//! \internal_end

#endif // MSCENEDIMMINGANIMATION_H
