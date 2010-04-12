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

#ifndef MPAGESWITCHANIMATIONSTYLE_H
#define MPAGESWITCHANIMATIONSTYLE_H

#include <manimationstyle.h>
#include <QEasingCurve>

//! \internal
class MPageSwitchAnimationStyle : public MAnimationStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MPageSwitchAnimationStyle)

    M_STYLE_ATTRIBUTE(int, duration, Duration)
    M_STYLE_ATTRIBUTE(QEasingCurve, easingCurve, EasingCurve)
};

class MPageSwitchAnimationStyleContainer : public MAnimationStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MPageSwitchAnimationStyle)
};
//! \internal_end

#endif
