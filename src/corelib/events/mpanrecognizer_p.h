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

#ifndef MPANRECOGNIZER_P_H
#define MPANRECOGNIZER_P_H

#include "mpanrecognizer.h"
#include "mpanrecognizerstyle.h"

#include "mpangesture_p.h"

#include <QMouseEvent>

/*!
  Private class used by MPanRecognizer objects to
  store variables during gesture recognition.
 */
class MPanRecognizerPrivate
{
public:

    Q_DECLARE_PUBLIC(MPanRecognizer)
    MPanRecognizerPrivate();
    virtual ~MPanRecognizerPrivate();

private:
    const MPanRecognizerStyle* style;

    void clearNotAlignedMovement(MPanGesture *panGesture);

    QGestureRecognizer::Result recognitionStart(MPanGesture *panGesture, const QMouseEvent *ev);
    QGestureRecognizer::Result recognitionUpdate(MPanGesture *panGesture, const QMouseEvent *ev);
    QGestureRecognizer::Result recognitionFinish(MPanGesture *panGesture);

    MPanRecognizer* q_ptr;
};

#endif
