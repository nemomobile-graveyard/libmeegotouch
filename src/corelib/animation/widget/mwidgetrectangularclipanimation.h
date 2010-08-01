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


#ifndef MWIDGETRECTANGULARCLIPANIMATION_H
#define MWIDGETRECTANGULARCLIPANIMATION_H

#include "mabstractwidgetanimation.h"
#include "mwidgetrectangularclipanimationstyle.h"

class MWidgetRectangularClipAnimationPrivate;

//! \internal
class MWidgetRectangularClipAnimation : public MAbstractWidgetAnimation
{
    Q_OBJECT
    M_ANIMATION_GROUP(MWidgetRectangularClipAnimationStyle)

public:
    MWidgetRectangularClipAnimation(QObject *parent = NULL);
    virtual ~MWidgetRectangularClipAnimation();

    virtual void setTargetWidget(MWidgetController *widget);
    virtual void restoreTargetWidgetState();

    void setTransitionDirection(TransitionDirection direction);

protected:
    MWidgetRectangularClipAnimation(MWidgetRectangularClipAnimationPrivate *dd, QObject *parent = NULL);

    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    Q_DECLARE_PRIVATE(MWidgetRectangularClipAnimation)
};
//! \internal_end

#endif // MWIDGETRECTANGULARCLIPANIMATION_H
