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

#ifndef MIMAGEWIDGETSTYLE_H
#define MIMAGEWIDGETSTYLE_H

#include <QColor>
#include <mwidgetstyle.h>

/*!
    \class MImageWidgetStyle
    \brief Style class for standard m images.

    \code
        MImageWidgetStyle {
            border-top: 0;
            border-left: 0;
            border-bottom: 0;
            border-right: 0;
            border-color: #FFFFFF;
            border-opacity: 1.0;
        }
    \endcode

    \ingroup styles
    \sa MImageWidgetStyleContainer MWidgetStyle \ref styling MImageWidget MImageWidgetView
*/

class M_VIEWS_EXPORT MImageWidgetStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MImageWidgetStyle)

    /*!
        \property MImageWidgetStyle::borderTop
        \brief top border of MImageWidget.
    */
    M_STYLE_ATTRIBUTE(int,          borderTop,            BorderTop)

    /*!
        \property MImageWidgetStyle::borderLeft
        \brief left border of MImageWidget.
    */
    M_STYLE_ATTRIBUTE(int,          borderLeft,           BorderLeft)

    /*!
        \property MImageWidgetStyle::borderBottom
        \brief bottom border of MImageWidget.
    */
    M_STYLE_ATTRIBUTE(int,          borderBottom,         BorderBottom)

    /*!
        \property MImageWidgetStyle::borderRight
        \brief right border of MImageWidget.
    */
    M_STYLE_ATTRIBUTE(int,          borderRight,          BorderRight)

    /*!
        \property MImageWidgetStyle::borderColor
        \brief border color of MImageWidget.
    */
    M_STYLE_ATTRIBUTE(QColor,       borderColor,          BorderColor)

    /*!
        \property MImageWidgetStyle::borderOpacity
        \brief border opacity of MImageWidget.
    */
    M_STYLE_ATTRIBUTE(qreal,        borderOpacity,        BorderOpacity)
};

/*!
    \class MImageWidgetStyleContainer
    \brief Style mode container class for MImageWidgetStyle.

    \ingroup styles
    \sa MImageWidgetStyle
*/
class M_VIEWS_EXPORT MImageWidgetStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MImageWidgetStyle)
};

#endif

