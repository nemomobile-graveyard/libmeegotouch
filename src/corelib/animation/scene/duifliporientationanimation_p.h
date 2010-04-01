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

#ifndef DUIFLIPORIENTATIONANIMATION_P_H
#define DUIFLIPORIENTATIONANIMATION_P_H

#include "duiorientationanimation_p.h"
#include "duinamespace.h"

class QParallelAnimationGroup;
class QPropertyAnimation;
class DuiBasicOrientationAnimation;
class DuiSceneWindowAnimation;

class DuiFlipOrientationAnimationPrivate : public DuiOrientationAnimationPrivate
{
    Q_DECLARE_PUBLIC(DuiFlipOrientationAnimation)
public:
    DuiFlipOrientationAnimationPrivate();
    virtual ~DuiFlipOrientationAnimationPrivate();
private:
    void finishedPhase0();

    QPropertyAnimation   *phase0;
    QPropertyAnimation   *phase1;

    Dui::OrientationAngle angle;
};

#endif
