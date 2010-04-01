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

#ifndef DUILABELSTYLE_H
#define DUILABELSTYLE_H

#include <QColor>
#include <QFont>
#include <duiwidgetstyle.h>

/*!
    \class DuiLabelStyle
    \brief Style class for DuiLabel.

    \code
        DuiLabelStyle {
            color: blue;
            font: arial 12;
        }
    \endcode

    \ingroup styles
    \sa DuiLabelStyleContainer DuiWidgetStyle \ref styling DuiLabel
*/
class DUI_EXPORT DuiLabelStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(DuiLabelStyle)

    /*!
        \property DuiLabelStyle::color
        \brief Label font color.

        See QColor::setNamedColor for possbile values.
    */
    DUI_STYLE_ATTRIBUTE(QColor, color, Color)

    /*!
        \property DuiLabelStyle::font
        \brief Label font.
    */
    DUI_STYLE_ATTRIBUTE(QFont, font, Font)

    /*!
        \property DuiLabelStyle::highlightColor
        \brief Color for highlighted text fragments.

        See QColor::setNamedColor for possbile values.
    */
    DUI_STYLE_ATTRIBUTE(QColor, highlightColor, HighlightColor)

    /*!
        \property DuiLabelStyle::activeHighlightColor
        \brief Color for active (aka link that is being pressed down) highlighted text fragments.

        See QColor::setNamedColor for possbile values.
    */
    DUI_STYLE_ATTRIBUTE(QColor, activeHighlightColor, ActiveHighlightColor)
};

/*!
    \class DuiLabelStyleContainer
    \brief Style mode container class for DuiLabelStyle.

    \ingroup styles
    \sa DuiLabelStyle
*/
class DUI_EXPORT DuiLabelStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(DuiLabelStyle)
};

#endif

