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

#ifndef MBUTTONICONSTYLE_H
#define MBUTTONICONSTYLE_H

#include <QColor>
#include <mbuttonstyle.h>

/*!
    \class MButtonIconStyle
    \brief Style class for m icon buttons.

    \code
        MButtonStyle {
            glow-color: #FFFF00;
            glow-duration: 700;
            glow-radius: 8;

            shrink-duration: 100;
            shrink-factor: 0.2;
        }
    \endcode

    \ingroup styles
    \sa MButtonIconStyleContainer MWidgetStyle MButtonStyle \ref styling MButton MButtonIconView
*/
class M_EXPORT MButtonIconStyle : public MButtonStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MButtonIconStyle)

    /*!
        \property MButtonIconStyle::glowColor
        \brief Color of the glow effect.
    */
    M_STYLE_ATTRIBUTE(QColor, glowColor,          GlowColor)

    /*!
        \property MButtonIconStyle::glowDuration
        \brief Length of glow effect in ms.
    */
    M_STYLE_ATTRIBUTE(int,    glowDuration,       GlowDuration)

    /*!
        \property MButtonIconStyle::glowRadius
        \brief Radius of the glow effect.
    */
    M_STYLE_ATTRIBUTE(int,    glowRadius,         GlowRadius)

    /*!
        \property MButtonIconStyle::shrinkDuration
        \brief Length of the shrink animation in ms.
    */
    M_STYLE_ATTRIBUTE(int,    shrinkDuration,     ShrinkDuration)

    /*!
        \property MButtonIconStyle::shrinkFactor
        \brief Scaling factor for the shrink animation.

        Defines the minimum size for the button.
    */
    M_STYLE_ATTRIBUTE(qreal,  shrinkFactor,       ShrinkFactor)
};

/*!
    \class MButtonIconStyleContainer
    \brief Style mode container class for MButtonIconStyle.

    \ingroup styles
    \sa MButtonIconStyle
*/
class M_EXPORT MButtonIconStyleContainer : public MButtonStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MButtonIconStyle)
};

#endif

