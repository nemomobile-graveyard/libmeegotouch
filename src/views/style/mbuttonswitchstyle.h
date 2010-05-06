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

#ifndef MBUTTONSWITCHSTYLE_H
#define MBUTTONSWITCHSTYLE_H

#include <QColor>
#include <mbuttonstyle.h>

/*!
    \class MButtonSwitchStyle
    \brief Style class for switch buttons.

    \code
        MButtonSwitchStyle {
            // slider is behind thumb, but above background
            slider-image: "mbutton-switch-colorstripe";

            // slider-mask defines the shape of the slider-image
            slider-mask: "mbutton-switch-mask" 15px 15px 15px 15px;

            // thumb is the sliding part, should slide across the whole area
            thumb-image: "mbutton-switch-thumb" 15px 15px 15px 15px;
        }
    \endcode

    \ingroup styles
    \sa MButtonSwitchStyleContainer MWidgetStyle MButtonStyle \ref styling MButton MButtonSwitchView
*/
class M_EXPORT MButtonSwitchStyle : public MButtonStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MButtonSwitchStyle)

    /*!
        \property MButtonSwitchStyle::sliderImage
        \brief Image for the sliding background of the switch.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, sliderImage, SliderImage)

    /*!
        \property MButtonSwitchStyle::sliderMask
        \brief Image for masking the sliding background.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, sliderMask, SliderMask)

    /*!
        \property MButtonSwitchStyle::thumbImage
        \brief Image for the thumb of the switch.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  thumbImage, ThumbImage)
};

/*!
    \class MButtonSwitchStyleContainer
    \brief Style mode container class for MButtonSwitchStyle.

    \ingroup styles
    \sa MButtonSwitchStyle
*/
class M_EXPORT MButtonSwitchStyleContainer : public MButtonStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MButtonSwitchStyle)
};

#endif

