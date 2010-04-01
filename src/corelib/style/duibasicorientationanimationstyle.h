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

#ifndef DUIBASICORIENTATIONANIMATIONSTYLE_H
#define DUIBASICORIENTATIONANIMATIONSTYLE_H

#include <duianimationstyle.h>

//! \internal
class DuiBasicOrientationAnimationStyle: public DuiAnimationStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiBasicOrientationAnimationStyle)

    DUI_STYLE_ATTRIBUTE(int, phaseZeroDuration, PhaseZeroDuration)
    DUI_STYLE_ATTRIBUTE(int, phaseOneDuration, PhaseOneDuration)
    DUI_STYLE_ATTRIBUTE(int, phaseTwoDuration, PhaseTwoDuration)
};

class DuiBasicOrientationAnimationStyleContainer : public DuiAnimationStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiBasicOrientationAnimationStyle)
};
//! \internal_end

#endif

