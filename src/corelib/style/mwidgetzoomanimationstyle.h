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

#ifndef MWIDGETZOOMANIMATIONSTYLE_H
#define MWIDGETZOOMANIMATIONSTYLE_H

#include <manimationstyle.h>
#include <QEasingCurve>

//! \internal
class MWidgetZoomAnimationStyle: public MAnimationStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MWidgetZoomAnimationStyle)

    M_STYLE_ATTRIBUTE(int, opacityAnimationDelay, opacityAnimationDelay)
    M_STYLE_ATTRIBUTE(int, opacityAnimationDuration, opacityAnimationDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, opacityAnimationEasingCurve, OpacityAnimationEasingCurve)

    M_STYLE_ATTRIBUTE(int, scaleAnimationDuration, scaleAnimationDuration)
    M_STYLE_ATTRIBUTE(QEasingCurve, scaleAnimationEasingCurve, ScaleAnimationEasingCurve)

    M_STYLE_ATTRIBUTE(int, delay, Delay)
    M_STYLE_ATTRIBUTE(qreal, scale, Scale)
    M_STYLE_ATTRIBUTE(qreal, opacity, Opacity)
    M_STYLE_ATTRIBUTE(qreal, positionAnimationDistanceFactor, PositionAnimationDistanceFactor)
};

class MWidgetZoomAnimationStyleContainer : public MAnimationStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MWidgetZoomAnimationStyle)
};
//! \internal_end

#endif

