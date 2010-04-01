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

#ifndef DUIGRIDITEMSTYLE_H
#define DUIGRIDITEMSTYLE_H

#include <duiwidgetstyle.h>

/*!
    \class DuiGridItemStyle
    \brief Style class for DuiGridItem.

    \code
        DuiGridItemStyle {
            icon-align: left;
            icon-size: 64px 64px;
        }
    \endcode

    \ingroup styles
    \sa DuiGridItemStyleContainer DuiWidgetStyle \ref styling DuiGridItem DuiGridItemView
*/

class DUI_EXPORT DuiGridItemStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(DuiGridItemStyle)

    /*!
        \property DuiGridItemStyle::iconAlign
        \brief Alignmentation of the icon.
    */
    DUI_STYLE_ATTRIBUTE(Qt::Alignment,          iconAlign,                IconAlign)

    /*!
        \property DuiGridItemStyle::iconSize
        \brief Size of the icon.
    */
    DUI_STYLE_ATTRIBUTE(QSize,                  iconSize,                 IconSize)
};

/*!
    \class DuiGridItemStyleContainer
    \brief Style mode container class for DuiGridItemStyle.

    \ingroup styles
    \sa DuiGridItemStyle
*/
class DUI_EXPORT DuiGridItemStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(DuiGridItemStyle)
};

#endif

