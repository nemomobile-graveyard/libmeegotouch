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

#ifndef MPANRECOGNIZERTOUCH_P_H
#define MPANRECOGNIZERTOUCH_P_H

#include "mpanrecognizertouch.h"

#include <QTouchEvent>
class MPanGestureTouch;
class MPanRecognizerStyle;

/*!
  Private class used by MPanRecognizerTouch objects to
  store variables during gesture recognition.
 */
class MPanRecognizerTouchPrivate
{
public:

    Q_DECLARE_PUBLIC(MPanRecognizerTouch)
    MPanRecognizerTouchPrivate();
    virtual ~MPanRecognizerTouchPrivate();

    void clearNotAlignedMovement(MPanGestureTouch *panGesture);

    static const QTouchEvent::TouchPoint *fetchPressedPoint(
        const QList<QTouchEvent::TouchPoint> &points);

    static const QTouchEvent::TouchPoint *fetchTouchPointById(
        const QList<QTouchEvent::TouchPoint> &points, int touchPointId);

    QGestureRecognizer::Result finishGesture(MPanGestureTouch *panGesture);

    QGestureRecognizer::Result updateGesture(
        MPanGestureTouch *panGesture, const QTouchEvent::TouchPoint *touchPoint);

    QGestureRecognizer::Result recognizePossibleNewGesture(
        MPanGestureTouch *panGesture, const QTouchEvent::TouchPoint *pressedPoint);

    QGestureRecognizer::Result recognizeExistingGesture(
        MPanGestureTouch *panGesture, QTouchEvent *event);

    QGestureRecognizer::Result recognizeTouchEvent(MPanGestureTouch *panGesture, QTouchEvent *event);

    const MPanRecognizerStyle* style;
    MPanRecognizerTouch* q_ptr;
};

#endif
