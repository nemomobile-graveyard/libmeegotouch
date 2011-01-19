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

#ifndef MSWIPEGESTURE_P_H
#define MSWIPEGESTURE_P_H

#include <QSwipeGesture>
#include <QElapsedTimer>

//! \internal

/*!
  This is a private class that provides Swipe gesture state.
  It's objects are delivered to registered handlers when a gesture is initiated.
 */
class MSwipeGesture : public QSwipeGesture
{
    Q_OBJECT

public:

    /*!
     Default constructor.
     */
    explicit MSwipeGesture(QObject *parent = 0);

    /*!
     Default destructor.
     */
    virtual ~MSwipeGesture();

private:

    /*!
     Recognized direction of the swipe gesture. The change of them beyond angle threshold
     indicates that the user "zigzagged" on the screen and it is not a swipe gesture.
     */
    qreal recognizedAngle;

    /*!
     The previously calculated distance traveled by the pointer. Used to check
     if the user changed the direction in which he was swiping.
     */
    qreal prevDistance;

    /*!
     Initial mouse press position in scene space coordinates.
     */
    QPointF startPosition;

    /*!
     Time of the initial mousepress used to check if the swipe gesture is fast enough.
     */
    QElapsedTimer timer;

    /*!
     Filter for stray mouse events, we will only continue recognizing gesture if there
     was a mouse press before mouse move event.
     */
    bool pressed;

    friend class MSwipeRecognizer;
    friend class MSwipeRecognizerPrivate;

#ifdef UNIT_TEST
    friend class Ut_MSwipeRecognizer;
#endif

    Q_DISABLE_COPY(MSwipeGesture)
};

//! \internal_end

#endif
