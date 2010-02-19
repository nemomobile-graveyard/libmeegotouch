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

#ifndef DUITAPANDHOLDRECOGNIZER_H
#define DUITAPANDHOLDRECOGNIZER_H

#include <QGestureRecognizer>

class DuiTapAndHoldRecognizerPrivate;

//! \internal

/*!
  This is a private class that provides Tap&Hold gesture recognition functionality.
  It provides event based handling of the gestures.

 */
class DuiTapAndHoldRecognizer : public QGestureRecognizer
{
public:

    /*!
     Default constructor.
     */
    DuiTapAndHoldRecognizer();

    /*!
     Default destructor.
     */
    virtual ~DuiTapAndHoldRecognizer();

    /*!
     Method used internally by Qt recognition manager to create a gesture state.
     \sa DuiTapAndHoldGesture
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
    DuiTapAndHoldRecognizerPrivate * const d_ptr;

private:

    Q_DECLARE_PRIVATE(DuiTapAndHoldRecognizer)
    Q_DISABLE_COPY(DuiTapAndHoldRecognizer)

#ifdef UNIT_TEST
    friend class Ut_DuiTapAndHoldRecognizer;
#endif
};

//! \internal_end

#endif
