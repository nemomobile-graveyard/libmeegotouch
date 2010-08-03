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

#ifndef MSWIPERECOGNIZER_H
#define MSWIPERECOGNIZER_H

#include <QGestureRecognizer>

class MSwipeRecognizerPrivate;

//! \internal

/*!
  This is a private class that provides Swipe gesture recognition functionality.
  It provides event based handling of the gestures.

 */
class MSwipeRecognizer : public QGestureRecognizer
{
public:

    /*!
     Default constructor.
     */
    MSwipeRecognizer();

    /*!
     Default destructor.
     */
    virtual ~MSwipeRecognizer();

    /*!
     Method used internally by Qt recognition manager to create a gesture state.
     \sa MSwipeGesture
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

protected:
    /*!
     Private object attribute.
     */
    MSwipeRecognizerPrivate * const d_ptr;

private:

    Q_DECLARE_PRIVATE(MSwipeRecognizer)
    Q_DISABLE_COPY(MSwipeRecognizer)

#ifdef UNIT_TEST
    friend class Ut_MSwipeRecognizer;
#endif
};

//! \internal_end

#endif
