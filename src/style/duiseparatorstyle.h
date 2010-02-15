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

#ifndef DUISEPARATORSTYLE_H
#define DUISEPARATORSTYLE_H

#include <duiwidgetstyle.h>

/*!
    \class DuiSeparatorStyle
    \brief Style class for DuiSeparator.

    \code
        DuiSeparatorStyle {
            span: 0.6mm;
        }
    \endcode

    \ingroup styles
    \sa DuiSeparatorStyleContainer DuiWidgetStyle \ref styling DuiSeparator
*/

class DUI_EXPORT DuiSeparatorStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiSeparatorStyle)

    /*!
        \property DuiSeparatorStyle::span
        \brief separator span.
    */
    DUI_STYLE_ATTRIBUTE(int, span, Span)
};

/*!
    \class DuiSeparatorStyleContainer
    \brief Style mode container class for DuiSeparatorStyle.

    \ingroup styles
    \sa DuiSeparatorStyle
*/
class DUI_EXPORT DuiSeparatorStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiSeparatorStyle)
};

#endif

