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

#ifndef MLABELSTYLE_H
#define MLABELSTYLE_H

#include <QColor>
#include <QFont>
#include <mwidgetstyle.h>

/*!
    \class MLabelStyle
    \brief Style class for MLabel.

    \code
        MLabelStyle {
            color: blue;
            font: arial 12;
        }
    \endcode

    \ingroup styles
    \sa MLabelStyleContainer MWidgetStyle \ref styling MLabel
*/
class M_VIEWS_EXPORT MLabelStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MLabelStyle)

    /*!
        \property MLabelStyle::color
        \brief Label font color.

        See QColor::setNamedColor for possbile values.
    */
    M_STYLE_ATTRIBUTE(QColor, color, Color)

    /*!
        \property MLabelStyle::font
        \brief Label font.
    */
    M_STYLE_ATTRIBUTE(QFont, font, Font)

    /*!
        \property MLabelStyle::highlightColor
        \brief Color for highlighted text fragments.

        See QColor::setNamedColor for possbile values.
    */
    M_STYLE_ATTRIBUTE(QColor, highlightColor, HighlightColor)

    /*!
        \property MLabelStyle::activeHighlightColor
        \brief Color for active (aka link that is being pressed down) highlighted text fragments.

        See QColor::setNamedColor for possbile values.
    */
    M_STYLE_ATTRIBUTE(QColor, activeHighlightColor, ActiveHighlightColor)

    /*!
        \property MLabelStyle::labelOpacity
        \brief Opacity for the label
    */

    M_STYLE_ATTRIBUTE(qreal, textOpacity, textOpacity)

    /*!
      \property MLabelStyle::horizontalAlignment
      \brief Horizontal alignment of the label.
    */
    M_STYLE_ATTRIBUTE(Qt::Alignment, horizontalAlignment, HorizontalAlignment)

    /*!
      \property MLabelStyle::verticalAlignment
      \brief Vertical alignment of the label.
    */
    M_STYLE_ATTRIBUTE(Qt::Alignment, verticalAlignment, VerticalAlignment)

};

/*!
    \class MLabelStyleContainer
    \brief Style mode container class for MLabelStyle.

    \ingroup styles
    \sa MLabelStyle
*/
class M_VIEWS_EXPORT MLabelStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MLabelStyle)
};

#endif

