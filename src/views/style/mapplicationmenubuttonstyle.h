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

#ifndef MAPPLICATIONMENUBUTTONSTYLE_H
#define MAPPLICATIONMENUBUTTONSTYLE_H

//! \internal
#include "mbuttonstyle.h"

/*!
    \class MApplicationMenuButtonStyle
    \brief Style class for application menu title button

    \code
        MApplicationMenuButtonStyle {
            arrow-icon: "arrow-icon";
        }
    \endcode

    \ingroup styles
    \sa MApplicationMenuButtonStyleContainer MButtonStyle \ref styling MApplicationMenuButton MApplicationMenuButtonView
*/
class M_VIEWS_EXPORT MApplicationMenuButtonStyle : public MButtonStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MApplicationMenuButtonStyle)

    /*!
        \property MApplicationMenuButtonStyle::arrowIcon
        \brief Id of arrow icon
    */
    M_STYLE_ATTRIBUTE(QString,                  arrowIcon,                     ArrowIcon)

    /*!
        \property MApplicationMenuButtonStyle::arrowIcon
        \brief Size of arrow icon
    */
    M_STYLE_ATTRIBUTE(QSize,                    arrowIconSize,                 ArrowIconSize)
};

/*!
    \class MApplicationMenuButtonStyleContainer
    \brief Style mode container class for MApplicationMenuButtonStyle.

    \ingroup styles
    \sa MApplicationMenuButtonStyle
*/
class M_VIEWS_EXPORT MApplicationMenuButtonStyleContainer : public MButtonStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MApplicationMenuButtonStyle)
};
//! \internal_end

#endif

