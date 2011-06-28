/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MPANGESTURETOUCH_H
#define MPANGESTURETOUCH_H

#include <QPanGesture>

//! \internal

/*!
   \class MPanGestureTouch
   \brief The kind of QPanGesture that's is created by MPanRecognizerTouch
 */
class MPanGestureTouch : public QPanGesture
{
    Q_OBJECT

public:
    /*!
     Default constructor.
     */
    MPanGestureTouch(QObject *parent = 0);

    /*!
     Default destructor.
     */
    virtual ~MPanGestureTouch();

    /*!
      The start position of the pan gesture in scene coordinates.
     */
    QPointF             startScenePos;
    Qt::Orientations    panDirection;

    // ID of the touch point that is driving this gesture
    int touchPointId;
};

//! \internal_end

#endif