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

#ifndef MGRIDITEMSTYLE_H
#define MGRIDITEMSTYLE_H

#include <mwidgetstyle.h>

/*!
    \class MGridItemStyle
    \brief Style class for MGridItem.

    \code
        MGridItemStyle {
            icon-align: left;
            icon-size: 64px 64px;
        }
    \endcode

    \ingroup styles
    \sa MGridItemStyleContainer MWidgetStyle \ref styling MGridItem MGridItemView
*/

class M_VIEWS_EXPORT MGridItemStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MGridItemStyle)

    /*!
        \property MGridItemStyle::iconAlign
        \brief Alignmentation of the icon.
    */
    M_STYLE_ATTRIBUTE(Qt::Alignment,          iconAlign,                IconAlign)

    /*!
        \property MGridItemStyle::iconSize
        \brief Size of the icon.
    */
    M_STYLE_ATTRIBUTE(QSize,                  iconSize,                 IconSize)
};

/*!
    \class MGridItemStyleContainer
    \brief Style mode container class for MGridItemStyle.

    \ingroup styles
    \sa MGridItemStyle
*/
class M_VIEWS_EXPORT MGridItemStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MGridItemStyle)
};

#endif

