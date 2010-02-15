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

#ifndef DUIBUTTONSWITCHSTYLE_H
#define DUIBUTTONSWITCHSTYLE_H

#include <QColor>
#include <duibuttonstyle.h>

/*!
    \class DuiButtonSwitchStyle
    \brief Style class for switch buttons.

    \code
        DuiButtonSwitchStyle {
            // slider is behind thumb, but above background
            slider-image: "duibutton-switch-colorstripe";

            // slider-mask defines the shape of the slider-image
            slider-mask: "duibutton-switch-mask" 15px 15px 15px 15px;

            // thumb is the sliding part, should slide across the whole area
            thumb-image: "duibutton-switch-thumb" 15px 15px 15px 15px;
        }
    \endcode

    \ingroup styles
    \sa DuiButtonSwitchStyleContainer DuiWidgetStyle DuiButtonStyle \ref styling DuiButton DuiButtonSwitchView
*/
class DUI_EXPORT DuiButtonSwitchStyle : public DuiButtonStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiButtonSwitchStyle)

    /*!
        \property DuiButtonSwitchStyle::sliderImage
        \brief Image for the sliding background of the switch.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, sliderImage, SliderImage)

    /*!
        \property DuiButtonSwitchStyle::sliderMask
        \brief Image for masking the sliding background.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, sliderMask, SliderMask)

    /*!
        \property DuiButtonSwitchStyle::thumbImage
        \brief Image for the thumb of the switch.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  thumbImage, ThumbImage)
};

/*!
    \class DuiButtonSwitchStyleContainer
    \brief Style mode container class for DuiButtonSwitchStyle.

    \ingroup styles
    \sa DuiButtonSwitchStyle
*/
class DUI_EXPORT DuiButtonSwitchStyleContainer : public DuiButtonStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiButtonSwitchStyle)
};

#endif

