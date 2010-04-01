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

#ifndef DUIGESTURETAPANDHOLDGESTURE_P_H
#define DUIGESTURETAPANDHOLDGESTURE_P_H

#include <QTapAndHoldGesture>

//! \internal

/*!
  This is a private class that provides Tap&Hold gesture state.
  It's objects are delivered to registered handlers when a gesture is initiated.
 */
class DuiTapAndHoldGesture : public QTapAndHoldGesture
{
    Q_OBJECT

public:

    /*!
     Default constructor.
     */
    DuiTapAndHoldGesture(QObject *parent = 0);

    /*!
     Default destructor.
     */
    virtual ~DuiTapAndHoldGesture();

private:

    /*!
     Timer identificator used in long tap recognizer.
     */
    int timerId;

    friend class DuiTapAndHoldRecognizer;

#ifdef UNIT_TEST
    friend class Ut_DuiTapAndHoldRecognizer;
#endif

    Q_DISABLE_COPY(DuiTapAndHoldGesture)
};

//! \internal_end

#endif
