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

#ifndef DUIAPPLICATIONMENUBUTTONSTYLE_H
#define DUIAPPLICATIONMENUBUTTONSTYLE_H

//! \internal
#include "duibuttonstyle.h"

/*!
    \class DuiApplicationMenuButtonStyle
    \brief Style class for application menu title button

    \code
        DuiApplicationMenuButtonStyle {
            arrow-icon: "arrow-icon";
        }
    \endcode

    \ingroup styles
    \sa DuiApplicationMenuButtonStyleContainer DuiButtonStyle \ref styling DuiApplicationMenuButton DuiApplicationMenuButtonView
*/
class DUI_EXPORT DuiApplicationMenuButtonStyle : public DuiButtonStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiApplicationMenuButtonStyle)

    /*!
        \property DuiApplicationMenuButtonStyle::arrowIcon
        \brief Id of arrow icon
    */
    DUI_STYLE_ATTRIBUTE(QString,                  arrowIcon,                     ArrowIcon)

    /*!
        \property DuiApplicationMenuButtonStyle::arrowIcon
        \brief Size of arrow icon
    */
    DUI_STYLE_ATTRIBUTE(QSize,                    arrowIconSize,                 ArrowIconSize)
};

/*!
    \class DuiApplicationMenuButtonStyleContainer
    \brief Style mode container class for DuiApplicationMenuButtonStyle.

    \ingroup styles
    \sa DuiApplicationMenuButtonStyle
*/
class DuiApplicationMenuButtonStyleContainer : public DuiButtonStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiApplicationMenuButtonStyle)
};
//! \internal_end

#endif

