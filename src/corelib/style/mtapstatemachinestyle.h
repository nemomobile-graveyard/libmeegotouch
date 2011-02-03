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

#ifndef MTAPSTATEMACHINESTYLE_H
#define MTAPSTATEMACHINESTYLE_H

#include <mstyle.h>

//! \internal

/** \brief Defines a style for a MTapStateMachineStyle class.
 *  This class defines the default threshold values that are
 *  used by MTapStateMachineStyle.
 */
class MTapStateMachineStyle : public MStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MTapStateMachineStyle)

    /*!
    \brief Initial wait time before highlight should be shown
    on the widget after mouse press is received
     */
    M_STYLE_ATTRIBUTE(int, initialWaitTime, InitialWaitTime)

    /*!
    \brief The duration of the highlight if the user fast-taps
    on the widget.
     */
    M_STYLE_ATTRIBUTE(int, blinkDuration, BlinkDuration)
};

class MTapStateMachineStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MTapStateMachineStyle)
};

//! \internal_end

#endif

