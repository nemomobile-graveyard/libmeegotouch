/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MTEXTSELECTIONHANDLESTYLE_H
#define MTEXTSELECTIONHANDLESTYLE_H

#include "mwidgetstyle.h"
#include <QEasingCurve>

//! \internal
class MTextSelectionHandleStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MTextSelectionHandleStyle)

    //! Defines vertical offset from edge of selected text to actual drawn handle.
    M_STYLE_ATTRIBUTE(qreal, visualOffset, VisualOffset)

    //! Easing curve for show/hide animation
    M_STYLE_ATTRIBUTE(QEasingCurve, showHideEasingCurve, ShowHideEasingCurve)

    //! Duration for show/hide animation
    M_STYLE_ATTRIBUTE(int, showHideDuration, ShowHideDuration)

    //! The smallest visible movement in pixels
    M_STYLE_ATTRIBUTE(int, movementSensitivity, MovementSensitivity)

    //! Duration of transition between normal and disabled states
    M_STYLE_ATTRIBUTE(int, disableDuration, DisableDuration)

    //! Opacity in disable state
    M_STYLE_ATTRIBUTE(qreal, disableOpacity, DisableOpacity)
};

class MTextSelectionHandleStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MTextSelectionHandleStyle)
};
//! \internal_end

#endif

