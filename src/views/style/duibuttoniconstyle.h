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

#ifndef DUIBUTTONICONSTYLE_H
#define DUIBUTTONICONSTYLE_H

#include <QColor>
#include <duibuttonstyle.h>

/*!
    \class DuiButtonIconStyle
    \brief Style class for dui icon buttons.

    \code
        DuiButtonStyle {
            glow-color: #FFFF00;
            glow-duration: 700;
            glow-radius: 8;

            shrink-duration: 100;
            shrink-factor: 0.2;
        }
    \endcode

    \ingroup styles
    \sa DuiButtonIconStyleContainer DuiWidgetStyle DuiButtonStyle \ref styling DuiButton DuiButtonIconView
*/
class DUI_EXPORT DuiButtonIconStyle : public DuiButtonStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiButtonIconStyle)

    /*!
        \property DuiButtonIconStyle::glowColor
        \brief Color of the glow effect.
    */
    DUI_STYLE_ATTRIBUTE(QColor, glowColor,          GlowColor)

    /*!
        \property DuiButtonIconStyle::glowDuration
        \brief Length of glow effect in ms.
    */
    DUI_STYLE_ATTRIBUTE(int,    glowDuration,       GlowDuration)

    /*!
        \property DuiButtonIconStyle::glowRadius
        \brief Radius of the glow effect.
    */
    DUI_STYLE_ATTRIBUTE(int,    glowRadius,         GlowRadius)

    /*!
        \property DuiButtonIconStyle::shrinkDuration
        \brief Length of the shrink animation in ms.
    */
    DUI_STYLE_ATTRIBUTE(int,    shrinkDuration,     ShrinkDuration)

    /*!
        \property DuiButtonIconStyle::shrinkFactor
        \brief Scaling factor for the shrink animation.

        Defines the minimum size for the button.
    */
    DUI_STYLE_ATTRIBUTE(qreal,  shrinkFactor,       ShrinkFactor)
};

/*!
    \class DuiButtonIconStyleContainer
    \brief Style mode container class for DuiButtonIconStyle.

    \ingroup styles
    \sa DuiButtonIconStyle
*/
class DUI_EXPORT DuiButtonIconStyleContainer : public DuiButtonStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiButtonIconStyle)
};

#endif

