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

#ifndef MPAGESWITCHSLIDEANIMATION_H
#define MPAGESWITCHSLIDEANIMATION_H

#include <QtGlobal>

#include <mpageswitchanimation.h>

class MPageSwitchSlideAnimationPrivate;
class MSceneWindow;

//! \internal

class MPageSwitchSlideAnimation : public MPageSwitchAnimation
{
    Q_OBJECT

public:
    MPageSwitchSlideAnimation(QObject *parent = NULL);

protected:

    MPageSwitchSlideAnimation(MPageSwitchSlideAnimationPrivate *dd, QObject *parent = NULL);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    Q_DECLARE_PRIVATE(MPageSwitchSlideAnimation)
};

//! \internal_end

#endif
