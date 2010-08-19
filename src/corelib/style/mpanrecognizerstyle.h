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

#ifndef MPANRECOGNIZERSTYLE_H
#define MPANRECOGNIZERSTYLE_H

#include <mstyle.h>

/** \brief Defines a style for a MPanRecognizerStyle class.
 *  This class defines the default threshold values that are
 *  used by pan gesture recognizers.
 */
class MPanRecognizerStyle : public MStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MPanRecognizerStyle)

    /*!
    \brief Distance threshold which is used to check if
    the pointer was moved far enough to recognize pan gesture.
     */
    M_STYLE_ATTRIBUTE(qreal, distanceThreshold, DistanceThreshold)

};

class MPanRecognizerStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MPanRecognizerStyle)
};

#endif

