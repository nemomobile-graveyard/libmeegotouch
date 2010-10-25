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

#ifndef MVIDEOWIDGETSTYLE_H
#define MVIDEOWIDGETSTYLE_H

#include <mwidgetstyle.h>
#include <QColor>

/*!
    \class MVideoWidgetStyle
    \brief Style class for video widget.

    \code
        MVideoWidgetStyle {
        }
    \endcode

    \ingroup styles
    \sa MVideoWidgetStyleContainer MWidgetStyle \ref styling MVideoWidget MVideoWidgetView
*/
class M_VIEWS_EXPORT MVideoWidgetStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MVideoWidgetStyle)

    /*!
        \property MVideoWidgetStyle::colorKey
        \brief Color key.
    */
    M_STYLE_ATTRIBUTE(QColor, colorKey, ColorKey)
};

/*!
    \class MVideoWidgetStyleContainer
    \brief Style mode container class for MVideoWidgetStyle.

    \ingroup styles
    \sa MVideoWidgetStyle
*/
class M_VIEWS_EXPORT MVideoWidgetStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MVideoWidgetStyle)

    /*!
        \brief Style mode for a fullscreen video widget.

        Mode is activated when video widget is set to fullscreen mode using 
        MVideoWidget::setFullscreen() method.
     */
    M_STYLE_MODE(Fullscreen)
};

#endif

