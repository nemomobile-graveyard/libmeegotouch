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

#ifndef DUIPAGESWITCHANIMATION_H
#define DUIPAGESWITCHANIMATION_H

#include <QtGlobal>

#include <duiparallelanimationgroup.h>
#include <duipageswitchanimationstyle.h>

class DuiPageSwitchAnimationPrivate;
class DuiSceneWindow;

//! \internal

class DuiPageSwitchAnimation : public DuiParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiPageSwitchAnimation)
    DUI_ANIMATION_GROUP(DuiPageSwitchAnimationStyle)

public:
    enum PageTransitionDirection {
        LeftToRight,
        RightToLeft
    };

    DuiPageSwitchAnimation(QObject *parent = NULL);
    void setNewPage(DuiSceneWindow *newPage);
    void setOldPage(DuiSceneWindow *oldPage);
    void setPageTransitionDirection(PageTransitionDirection direction);

protected:

    DuiPageSwitchAnimation(DuiPageSwitchAnimationPrivate *dd, QObject *parent = NULL);

    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_animationFinished())

};

//! \internal_end

#endif
