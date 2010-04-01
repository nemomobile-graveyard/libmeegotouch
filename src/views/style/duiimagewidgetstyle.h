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

#ifndef DUIIMAGEWIDGETSTYLE_H
#define DUIIMAGEWIDGETSTYLE_H

#include <QColor>
#include <duiwidgetstyle.h>

/*!
    \class DuiImageWidgetStyle
    \brief Style class for standard dui images.

    \code
        DuiImageWidgetStyle {
            border-top: 0;
            border-left: 0;
            border-bottom: 0;
            border-right: 0;
            border-color: #FFFFFF;
            border-opacity: 1.0;
        }
    \endcode

    \ingroup styles
    \sa DuiImageWidgetStyleContainer DuiWidgetStyle \ref styling DuiImageWidget DuiImageWidgetView
*/

class DUI_EXPORT DuiImageWidgetStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(DuiImageWidgetStyle)

    /*!
        \property DuiImageWidgetStyle::borderTop
        \brief top border of DuiImageWidget.
    */
    DUI_STYLE_ATTRIBUTE(int,          borderTop,            BorderTop)

    /*!
        \property DuiImageWidgetStyle::borderLeft
        \brief left border of DuiImageWidget.
    */
    DUI_STYLE_ATTRIBUTE(int,          borderLeft,           BorderLeft)

    /*!
        \property DuiImageWidgetStyle::borderBottom
        \brief bottom border of DuiImageWidget.
    */
    DUI_STYLE_ATTRIBUTE(int,          borderBottom,         BorderBottom)

    /*!
        \property DuiImageWidgetStyle::borderRight
        \brief right border of DuiImageWidget.
    */
    DUI_STYLE_ATTRIBUTE(int,          borderRight,          BorderRight)

    /*!
        \property DuiImageWidgetStyle::borderColor
        \brief border color of DuiImageWidget.
    */
    DUI_STYLE_ATTRIBUTE(QColor,       borderColor,          BorderColor)

    /*!
        \property DuiImageWidgetStyle::borderOpacity
        \brief border opacity of DuiImageWidget.
    */
    DUI_STYLE_ATTRIBUTE(qreal,        borderOpacity,        BorderOpacity)
};

/*!
    \class DuiImageWidgetStyleContainer
    \brief Style mode container class for DuiImageWidgetStyle.

    \ingroup styles
    \sa DuiImageWidgetStyle
*/
class DUI_EXPORT DuiImageWidgetStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(DuiImageWidgetStyle)
};

#endif

