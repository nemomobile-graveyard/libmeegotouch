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

#ifndef MSEPARATORSTYLE_H
#define MSEPARATORSTYLE_H

#include <mwidgetstyle.h>

/*!
    \class MSeparatorStyle
    \brief Style class for MSeparator.

    \code
        MSeparatorStyle {
            span: 0.6mm;
        }
    \endcode

    \ingroup styles
    \sa MSeparatorStyleContainer MWidgetStyle \ref styling MSeparator
*/

class M_VIEWS_EXPORT MSeparatorStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MSeparatorStyle)

    /*!
        \property MSeparatorStyle::span
        \brief separator span.
    */
    M_STYLE_ATTRIBUTE(int, span, Span)
};

/*!
    \class MSeparatorStyleContainer
    \brief Style mode container class for MSeparatorStyle.

    \ingroup styles
    \sa MSeparatorStyle
*/
class M_VIEWS_EXPORT MSeparatorStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MSeparatorStyle)
    M_STYLE_MODE(Horizontal)
    M_STYLE_MODE(Vertical)
};

#endif

