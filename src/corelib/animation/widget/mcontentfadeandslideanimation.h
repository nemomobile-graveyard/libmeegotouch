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
#ifndef MCONTENTFADEANDSLIDEANIMATIONMATION_H
#define MCONTENTFADEANDSLIDEANIMATIONMATION_H

#include "mabstractwidgetanimation.h"
#include "mcontentfadeandslideanimationstyle.h"

class MContentFadeAndSlideAnimationPrivate;
class MContentFadeAndSlideAnimationStyle;

//! \internal
class MContentFadeAndSlideAnimation : public MAbstractWidgetAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MContentFadeAndSlideAnimation)
    M_ANIMATION_GROUP(MContentFadeAndSlideAnimationStyle)

public:
    explicit MContentFadeAndSlideAnimation(QObject *parent = 0);
    virtual ~MContentFadeAndSlideAnimation();

    virtual void setTargetWidget(MWidgetController *widget);
    virtual void setTransitionDirection(MAbstractWidgetAnimation::TransitionDirection direction);

public slots:
    void takeContentSnapshot();

protected:
    //! \reimp
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    //! \reimp_end

    virtual void restoreTargetWidgetState();
};
//! \internal_end

#endif // MContentFadeAndSlideAnimationmation_H
