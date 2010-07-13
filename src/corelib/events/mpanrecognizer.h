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

#ifndef MPANRECOGNIZER_H
#define MPANRECOGNIZER_H

#include <QGestureRecognizer>

class QGesture;
class MPanRecognizerPrivate;

//! \internal

/*!
  This class provides Pan gesture recognition functionality.
  It provides event based handling of the gestures.
 */
class MPanRecognizer : public QGestureRecognizer
{
public:

    /*!
     Default constructor.
     */
    MPanRecognizer();

    /*!
     Default destructor.
     */
    virtual ~MPanRecognizer();

    /*!
     Method used internally by Qt recognition manager to create a gesture state.
     \sa MPanGesture
     */
    QGesture* create(QObject* target);

    /*!
     Event filtering routine, responsible for changing state of the state machine and
     triggering recognition events.
     */
    QGestureRecognizer::Result recognize(QGesture* gesture, QObject* watched, QEvent* event);

    /*!
     Method for resetting the gesture recognition state machine.
     */
    void reset(QGesture* gesture);

private:
    /*!
     Private object attribute.
     */
    MPanRecognizerPrivate * const d_ptr;

    Q_DECLARE_PRIVATE(MPanRecognizer)
    Q_DISABLE_COPY(MPanRecognizer)

};

//! \internal_end

#endif // MPANRECOGNIZER_H
