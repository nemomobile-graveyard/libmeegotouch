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

#ifndef MTAPANDHOLDRECOGNIZERSTYLE_H
#define MTAPANDHOLDRECOGNIZERSTYLE_H

#include <mstyle.h>

/** \brief Defines a style for a MTapAndHoldRecognizerStyle class.
 *  This class defines the default threshold values that are
 *  used by tap&hold gesture recognizers.
 */
class MTapAndHoldRecognizerStyle : public MStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MTapAndHoldRecognizerStyle)

    /*!
     \brief A time threshold which is used to check if
     the pointer was hold in place long enough to recognize gesture
     */
    M_STYLE_ATTRIBUTE(int, timeout, Timeout)

    /*!
     \brief A distance threshold which is used to check if
     the pointer was hold within specified range when gesture was
     recognized. If the pointer was moved further than this threshold,
     the gesture will be cancelled.
     */
    M_STYLE_ATTRIBUTE(qreal, movementThreshold, MovementThreshold)
};

class MTapAndHoldRecognizerStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MTapAndHoldRecognizerStyle)
};

#endif

