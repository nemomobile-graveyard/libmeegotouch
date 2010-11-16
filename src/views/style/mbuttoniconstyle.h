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

#ifndef MBUTTONICONSTYLE_H
#define MBUTTONICONSTYLE_H

#include <mbuttonstyle.h>

/*!
    \class MButtonIconStyle
    \brief Style class for m icon buttons.

    \ingroup styles
    \sa MButtonIconStyleContainer MWidgetStyle MButtonStyle \ref styling MButton MButtonIconView
*/
class M_VIEWS_EXPORT MButtonIconStyle : public MButtonStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MButtonIconStyle)

    /*!
        \property MButtonStyle::contentOpacity
        \brief Content opacity.
    */
    M_STYLE_ATTRIBUTE(qreal, contentOpacity, contentOpacity)
};

/*!
    \class MButtonIconStyleContainer
    \brief Style mode container class for MButtonIconStyle.

    \ingroup styles
    \sa MButtonIconStyle
*/
class M_VIEWS_EXPORT MButtonIconStyleContainer : public MButtonStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MButtonIconStyle)
};

#endif

