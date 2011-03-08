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

#include <mbuttonstyle.h>
#include <mnamespace.h>

/*!
    \class MButtonIconStyle
    \brief Style class for m icon buttons.

    \ingroup styles
    \sa MButtonIconStyleContainer MWidgetStyle MButtonStyle \ref styling MButton MButtonIconView
*/
class M_VIEWS_EXPORT MButtonIconStyle : public MButtonStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MButtonIconStyle)

    /*!
      \deprecated Property was added to MButtonStyle.
    */
    M_STYLE_ATTRIBUTE(qreal, contentOpacity, contentOpacity)

    /*!
      \deprecated Not in use.
    */
    M_STYLE_ATTRIBUTE(QColor, glowColor,          GlowColor)
    /*!
      \deprecated Not in use.
    */
    M_STYLE_ATTRIBUTE(int,    glowDuration,       GlowDuration)
    /*!
      \deprecated Not in use.
    */
    M_STYLE_ATTRIBUTE(int,    glowRadius,         GlowRadius)
    /*!
      \deprecated Not in use.
    */
    M_STYLE_ATTRIBUTE(int,    shrinkDuration,     ShrinkDuration)
    /*!
      \deprecated Not in use.
    */
    M_STYLE_ATTRIBUTE(qreal,  shrinkFactor,       ShrinkFactor)

    /*!
        \property MButtonIconStyle::labelWrapMode
        \brief Wrap mode for the button label.
    */
    M_STYLE_ATTRIBUTE(QTextOption::WrapMode, textWrapMode, textWrapMode)

    /*!
        \property MButtonIconStyle::LabelEliding
        \brief Boolean flag for defining whether label should elide or not.
    */
    M_STYLE_ATTRIBUTE(bool, textEliding, TextEliding)
};

/*!
    \class MButtonIconStyleContainer
    \brief Style mode container class for MButtonIconStyle.

    \ingroup styles
    \sa MButtonIconStyle
*/
class M_VIEWS_EXPORT MButtonIconStyleContainer : public MButtonStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MButtonIconStyle)
};

#endif

