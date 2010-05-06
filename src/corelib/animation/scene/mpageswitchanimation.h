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

#ifndef MPAGESWITCHANIMATION_H
#define MPAGESWITCHANIMATION_H

#include <QtGlobal>

#include <mparallelanimationgroup.h>
#include <mpageswitchanimationstyle.h>

class MPageSwitchAnimationPrivate;
class MSceneWindow;

//! \internal

class MPageSwitchAnimation : public MParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MPageSwitchAnimation)
    M_ANIMATION_GROUP(MPageSwitchAnimationStyle)

public:
    enum PageTransitionDirection {
        LeftToRight,
        RightToLeft
    };

    MPageSwitchAnimation(QObject *parent = NULL);
    void setNewPage(MSceneWindow *newPage);
    void setOldPage(MSceneWindow *oldPage);
    void setPageTransitionDirection(PageTransitionDirection direction);

protected:

    MPageSwitchAnimation(MPageSwitchAnimationPrivate *dd, QObject *parent = NULL);

    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_animationFinished())

};

//! \internal_end

#endif
