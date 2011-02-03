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

#ifndef MTAPSTATEMACHINE_H
#define MTAPSTATEMACHINE_H

#include <QObject>

#include "mexport.h"

//! \internal

class MTapStateMachinePrivate;

/*!
    \class MTapStateMachine
    \brief MTapStateMachine is state machine class which can be used to
    control the higlighting and unhilighting of a widget in response to mouse events.

    It is a private class used inside MeegoTouch library and should not be used by
    client applications.

    The class will introduce an initial delay before the highlight should be shown,
    which is useful if the target widget is going to be used in a list and should
    not be highlighted when the user executes panning gesture.

    It will also introduce a delay before sending a release signal if the user
    fast-taps the widget, which improves the feedback visualisation.
*/
class M_CORE_EXPORT MTapStateMachine : public QObject
{
    Q_OBJECT

public:
    /*!
      \brief Constructs a MTapStateMachine with a \a target object.
    */
    MTapStateMachine(QObject* target);
    virtual ~MTapStateMachine();

Q_SIGNALS:
    /*!
      \brief Emitted when the widget should be highlighted.
     */
    void delayedPress();
    /*!
      \brief Emitted when the widget should be unhighlighted.
     */
    void release();

protected:
    MTapStateMachinePrivate *const d_ptr;

#ifdef UNIT_TEST
    // Test unit is defined as a friend of production code to access private members
    friend class Ut_MTapStateMachine;
#endif

private:
    Q_DECLARE_PRIVATE(MTapStateMachine)
    Q_DISABLE_COPY(MTapStateMachine)
};

//! \internal_end

#endif // MTAPSTATEMACHINE_H
