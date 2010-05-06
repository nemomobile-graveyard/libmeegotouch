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

#ifndef MBUTTONSTYLE_H
#define MBUTTONSTYLE_H

#include <QFont>
#include <QColor>
#include <QSize>
#include <mwidgetstyle.h>

/*!
    \class MButtonStyle
    \brief Style class for standard m buttons.

    \code
        MButtonStyle {
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
    \sa MButtonStyleContainer MWidgetStyle \ref styling MButton MButtonView
*/
class M_EXPORT MButtonStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MButtonStyle)

    /*!
        \property MButtonStyle::font
        \brief Font for the button text.
    */
    M_STYLE_ATTRIBUTE(QFont,                  font,                     Font)

    /*!
        \property MButtonStyle::iconSize
        \brief Size of the button icon.
    */
    M_STYLE_ATTRIBUTE(QSize,                  iconSize,                 IconSize)

    /*!
        \property MButtonStyle::iconAlign
        \brief Alignmentation of the icon.
    */
    M_STYLE_ATTRIBUTE(Qt::Alignment,          iconAlign,                IconAlign)

    /*!
        \property MButtonStyle::textColor
        \brief Text color.
    */
    M_STYLE_ATTRIBUTE(QColor,                 textColor,                TextColor)

    /*!
        \property MButtonStyle::horizontalTextAlign
        \brief Horizontal text alignment.
    */
    M_STYLE_ATTRIBUTE(Qt::Alignment,          horizontalTextAlign,      HorizontalTextAlign)

    /*!
        \property MButtonStyle::verticalTextAlign
        \brief Vertical text alignment.
    */
    M_STYLE_ATTRIBUTE(Qt::Alignment,          verticalTextAlign,        VerticalTextAlign)

    /*!
        \property MButtonStyle::textMarginLeft
        \brief Left text margin.

        Empty space after left text boundary.
    */
    M_STYLE_ATTRIBUTE(int,                    textMarginLeft,           TextMarginLeft)

    /*!
        \property MButtonStyle::textMarginTop
        \brief Top text margin.

        Empty space after top text boundary.
    */
    M_STYLE_ATTRIBUTE(int,                    textMarginTop,            TextMarginTop)

    /*!
        \property MButtonStyle::textMarginRight
        \brief Right text margin.

        Empty space after right text boundary.
    */
    M_STYLE_ATTRIBUTE(int,                    textMarginRight,          TextMarginRight)

    /*!
        \property MButtonStyle::textMarginBottom
        \brief Bottom text margin.

        Empty space after bottom text boundary.
    */
    M_STYLE_ATTRIBUTE(int,                    textMarginBottom,         TextMarginBottom)
};

/*!
    \class MButtonStyleContainer
    \brief Style mode container class for MButtonStyle.

    \ingroup styles
    \sa MButtonStyle
*/
class M_EXPORT MButtonStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MButtonStyle)

    /*!
        \brief Style mode for a pressed button.

        Mode is activated when a button is pressed down and deactivated when
        button is released.
     */
    M_STYLE_MODE(Pressed)
};

#endif

