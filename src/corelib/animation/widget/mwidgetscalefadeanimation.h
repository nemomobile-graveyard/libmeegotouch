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

#ifndef MWIDGETSCALEFADEANIMATION_H
#define MWIDGETSCALEFADEANIMATION_H

#include "mabstractwidgetanimation.h"
#include <mwidgetscalefadeanimationstyle.h>

//! \internal

class MWidgetScaleFadeAnimationPrivate;

class MWidgetScaleFadeAnimation : public MAbstractWidgetAnimation
{
    Q_OBJECT
    M_ANIMATION_GROUP(MWidgetScaleFadeAnimationStyle)

public:
    MWidgetScaleFadeAnimation(QObject *parent = NULL);

    virtual void setTargetWidget(MWidgetController *widget);
    virtual void restoreTargetWidgetState();

    void setTransitionDirection(TransitionDirection direction);

protected:
    MWidgetScaleFadeAnimation(MWidgetScaleFadeAnimationPrivate *dd, QObject *parent = NULL);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    Q_DECLARE_PRIVATE(MWidgetScaleFadeAnimation)
};

//! \internal_end

#endif // MWIDGETSCALEFADEANIMATION_H
