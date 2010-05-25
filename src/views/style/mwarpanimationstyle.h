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

#ifndef MWARPANIMATIONSTYLE_H
#define MWARPANIMATIONSTYLE_H

#include <manimationstyle.h>
#include <QEasingCurve>

//! \internal
class MWarpAnimationStyle : public MAnimationStyle
{
    Q_OBJECT
    M_STYLE(MWarpAnimationStyle)

    M_STYLE_ATTRIBUTE(int, warpDuration, WarpDuration)
    M_STYLE_ATTRIBUTE(int, warpDistance, WarpDistance)

    M_STYLE_ATTRIBUTE(int, warpInDelay, WarpInDelay)

    M_STYLE_ATTRIBUTE(QEasingCurve, warpInCurve, WarpInCurve)
    M_STYLE_ATTRIBUTE(QEasingCurve, warpOutCurve, WarpOutCurve)
};

class MWarpAnimationStyleContainer : public MAnimationStyleContainer
{
    M_STYLE_CONTAINER(MWarpAnimationStyle)
};
//! \internal_end

#endif
