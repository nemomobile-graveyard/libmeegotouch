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

#ifndef MSWIPERECOGNIZER_P_H
#define MSWIPERECOGNIZER_P_H

#include "mswiperecognizer.h"
#include "mswiperecognizerstyle.h"

class MSwipeGesture;

class QPointF;
class QMouseEvent;

/*!
  Private class used by MSwipeRecognizer objects to
  store variables during gesture recognition.
 */
class MSwipeRecognizerPrivate
{
public:

    Q_DECLARE_PUBLIC(MSwipeRecognizer)
    MSwipeRecognizerPrivate();
    virtual ~MSwipeRecognizerPrivate();

protected:

    const MSwipeRecognizerStyle *style;

    qreal vectorLength(const QPointF& vector);
    bool isAngleDeltaBelowThreshold(qreal angle, qreal threshold);
    void snapToRightAngle(MSwipeGesture *swipeGesture);

    QGestureRecognizer::Result startRecognition(MSwipeGesture *swipeGesture, const QMouseEvent *mouseEvent);
    QGestureRecognizer::Result updateRecognition(MSwipeGesture *swipeGesture, const QMouseEvent *mouseEvent);
    QGestureRecognizer::Result finishRecognition(MSwipeGesture *swipeGesture);

    MSwipeRecognizer* q_ptr;

#ifdef UNIT_TEST
    friend class Ut_MSwipeRecognizer;
#endif
};

#endif
