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

#ifndef DUIPROGRESSINDICATORSTYLE_H
#define DUIPROGRESSINDICATORSTYLE_H

#include <duiwidgetstyle.h>

/*!
    \class DuiProgressIndicatorStyle
    \brief Style for progress indicator.

    \ingroup styles
    \sa DuiProgressIndicatorStyleContainer
*/
class DUI_EXPORT DuiProgressIndicatorStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(DuiProgressIndicatorStyle)

    /*!
        \property DuiProgressIndicatorStyle::activeImage
        \brief Image of the active bar element.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, activeImage, ActiveImage)

    /*!
        \property DuiProgressIndicatorStyle::inactiveImage
        \brief  Image of the inactive bar element.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, inactiveImage, InactiveImage)

    /*!
        \property DuiProgressIndicatorStyle::speed
        \brief Speed of unknown duration progress animation (distance/sec).
    */
    DUI_STYLE_ATTRIBUTE(int, speed, Speed)

    /*!
        \property DuiProgressIndicatorStyle::activeElementCount
        \brief The number of active elements in circular progress indicator (unknown duration).
    */
    DUI_STYLE_ATTRIBUTE(int, activeElementCount, ActiveElementCount)

    /*!
        \property DuiProgressIndicatorStyle::elementSize
        \brief Size of an element in the circle.
    */
    DUI_STYLE_ATTRIBUTE(qreal, elementSize, ElementSize)

    /*!
        \property DuiProgressIndicatorStyle::elementDistance
        \brief Distance between circle elements as a multiplier to element size.
    */
    DUI_STYLE_ATTRIBUTE(qreal, elementDistance, ElementDistance)
};

/*!
    \class DuiProgressIndicatorStyleContainer
    \brief This class groups all the styling modes for progress indicator.

    \ingroup styles
    \sa DuiProgressIndicatorStyle
*/
class DUI_EXPORT DuiProgressIndicatorStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(DuiProgressIndicatorStyle)
};

#endif

