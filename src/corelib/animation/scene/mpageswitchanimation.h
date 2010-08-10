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

#include <mparallelanimationgroup.h>
#include <mpageswitchanimationstyle.h>

#include <QPointer>

class MSceneWindow;
class MPageSwitchAnimationPrivate;

class MPageSwitchAnimation : public MParallelAnimationGroup
{
    Q_OBJECT
    M_ANIMATION_GROUP(MPageSwitchAnimationStyle)

public:
    enum TransitionDirection {
        ToChildPage,
        ToParentPage,
        ToSiblingPage
    };

public:
    MPageSwitchAnimation(QObject *parent = NULL);
    virtual ~MPageSwitchAnimation();

    void setNewPage(MSceneWindow *newPage);
    void setOldPage(MSceneWindow *oldPage);
    void setTransitionDirection(TransitionDirection direction);

    MSceneWindow *newPage() const;
    MSceneWindow *oldPage() const;
    TransitionDirection transitionDirection() const;

protected:
    MPageSwitchAnimation(MPageSwitchAnimationPrivate *dd, QObject *parent = NULL);

private:
    Q_DISABLE_COPY(MPageSwitchAnimation)
    Q_DECLARE_PRIVATE(MPageSwitchAnimation)
};

#endif // MPAGESWITCHANIMATION_H
