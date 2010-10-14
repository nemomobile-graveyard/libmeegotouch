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

#ifndef MSWIPERECOGNIZERSTYLE_H
#define MSWIPERECOGNIZERSTYLE_H

#include <mstyle.h>

/** \brief Defines a style for a MSwipeRecognizerStyle class.
 *  This class defines the default threshold values that are
 *  used by swipe gesture recognizers.
 */
class MSwipeRecognizerStyle : public MStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MSwipeRecognizerStyle)

    /*!
     \brief A time threshold which is used to check if
     the pointer was moved fast enough to recognize tap&hold gesture.
     */
    M_STYLE_ATTRIBUTE(int, timeout, Timeout)

    /*!
     \brief A distance threshold which is used to check if
     the pointer was moved far enough to recognize tap&hold gesture.
     */
    M_STYLE_ATTRIBUTE(qreal, distanceThreshold, DistanceThreshold)

    /*!
     \brief An angle threshold which is used to check if the pointer
     was moved consistently after the gesture was recognized.
     If the pointer skewed its movement by more than angleThreshold from
     the angle that was initially recognized the gesture is canceled.
     */
    M_STYLE_ATTRIBUTE(qreal, angleThreshold, angleThreshold)

    /*!
     \brief An angle threshold which is used to check if the pointer
     was moved horizontally or vertically. If the angle of swipe is
     equal to 0,90,180 or 270 degrees with epsilon equal to
     angleSnappingThreshold, then the final swipe angle is snapped
     to the closest of 0,90,180 or 270 degree angles.
     */
    M_STYLE_ATTRIBUTE(qreal, angleSnappingThreshold, angleSnappingThreshold)

    /*!
     \brief The jitterThreshold is a movement value used to avoid
     canceling gesture because of jitter effect when user removes
     his finger from the screen. When that happens the screen
     driver can sometimes report rapidly changing positions of
     the pointer and that could be interpreted as a change of direction.
     */
    M_STYLE_ATTRIBUTE(qreal, jitterThreshold, jitterThreshold)
};

class MSwipeRecognizerStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MSwipeRecognizerStyle)
};

#endif

