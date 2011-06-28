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

#ifndef MPANRECOGNIZERTOUCH_H
#define MPANRECOGNIZERTOUCH_H

#include <QGestureRecognizer>

class QGesture;
class MPanRecognizerTouchPrivate;

//! \internal

/*!
  This class provides Pan gesture recognition functionality.
  It uses only QTouchEvents so that it works well when panning
  is done in a multitouch screen.

  Use case:
      1 - user puts finger on touchscreen and starts panning
      2 - user puts a second finger on touchscreen (or touches the screen
          (with his palm)
      3 - user keeps panning with his finger from step 1
      4 - user raises finger from step 1.
      5 - user moves finger/contact point from step 2
      6 - user puts another finger on touchscreen and starts panning

      Outcome: panning shouldn't be affected at all by the touch point from step 2.
      That touch point should be completely ignored.
      It's hard (or impossible?) to achieve that with a gesture recognizer
      that uses QMouseEvents.

  The drawback of this recognizer is that it cannot be used on a desktop machine
  (i.e. a target device that uses a mouse pointer instead of a touch screen) as
  QTouchEvents are not generated in such environments.
 */
class MPanRecognizerTouch : public QGestureRecognizer
{
public:

    /*!
     Default constructor.
     */
    MPanRecognizerTouch();

    /*!
     Default destructor.
     */
    virtual ~MPanRecognizerTouch();

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
    MPanRecognizerTouchPrivate * const d_ptr;

    Q_DECLARE_PRIVATE(MPanRecognizerTouch)
    Q_DISABLE_COPY(MPanRecognizerTouch)

};

//! \internal_end

#endif // MPANRECOGNIZERTOUCH_H
