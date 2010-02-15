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

#ifndef DUIBUTTONSTYLE_H
#define DUIBUTTONSTYLE_H

#include <QFont>
#include <QColor>
#include <QSize>
#include <duiwidgetstyle.h>

/*!
    \class DuiButtonStyle
    \brief Style class for standard dui buttons.

    \code
        DuiButtonStyle {
            font: arial 12;

            icon-size: 32 32;
            icon-align: left;

            text-color: white;

            vertical-text-align: vcenter;
            horizontal-text-align: hcenter;

            text-margin-left: 5;
            text-margin-top: 0;
            text-margin-right: 5;
            text-margin-bottom: 0;
        }
    \endcode

    \ingroup styles
    \sa DuiButtonStyleContainer DuiWidgetStyle \ref styling DuiButton DuiButtonView
*/
class DUI_EXPORT DuiButtonStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiButtonStyle)

    /*!
        \property DuiButtonStyle::font
        \brief Font for the button text.
    */
    DUI_STYLE_ATTRIBUTE(QFont,                  font,                     Font)

    /*!
        \property DuiButtonStyle::iconSize
        \brief Size of the button icon.
    */
    DUI_STYLE_ATTRIBUTE(QSize,                  iconSize,                 IconSize)

    /*!
        \property DuiButtonStyle::iconAlign
        \brief Alignmentation of the icon.
    */
    DUI_STYLE_ATTRIBUTE(Qt::Alignment,          iconAlign,                IconAlign)

    /*!
        \property DuiButtonStyle::textColor
        \brief Text color.
    */
    DUI_STYLE_ATTRIBUTE(QColor,                 textColor,                TextColor)

    /*!
        \property DuiButtonStyle::horizontalTextAlign
        \brief Horizontal text alignment.
    */
    DUI_STYLE_ATTRIBUTE(Qt::Alignment,          horizontalTextAlign,      HorizontalTextAlign)

    /*!
        \property DuiButtonStyle::verticalTextAlign
        \brief Vertical text alignment.
    */
    DUI_STYLE_ATTRIBUTE(Qt::Alignment,          verticalTextAlign,        VerticalTextAlign)

    /*!
        \property DuiButtonStyle::textMarginLeft
        \brief Left text margin.

        Empty space after left text boundary.
    */
    DUI_STYLE_ATTRIBUTE(int,                    textMarginLeft,           TextMarginLeft)

    /*!
        \property DuiButtonStyle::textMarginTop
        \brief Top text margin.

        Empty space after top text boundary.
    */
    DUI_STYLE_ATTRIBUTE(int,                    textMarginTop,            TextMarginTop)

    /*!
        \property DuiButtonStyle::textMarginRight
        \brief Right text margin.

        Empty space after right text boundary.
    */
    DUI_STYLE_ATTRIBUTE(int,                    textMarginRight,          TextMarginRight)

    /*!
        \property DuiButtonStyle::textMarginBottom
        \brief Bottom text margin.

        Empty space after bottom text boundary.
    */
    DUI_STYLE_ATTRIBUTE(int,                    textMarginBottom,         TextMarginBottom)
};

/*!
    \class DuiButtonStyleContainer
    \brief Style mode container class for DuiButtonStyle.

    \ingroup styles
    \sa DuiButtonStyle
*/
class DUI_EXPORT DuiButtonStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiButtonStyle)

    /*!
        \brief Style mode for a pressed button.

        Mode is activated when a button is pressed down and deactivated when
        button is released.
     */
    DUI_STYLE_MODE(Pressed)
};

#endif

