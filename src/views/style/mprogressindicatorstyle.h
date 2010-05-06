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

#ifndef MPROGRESSINDICATORSTYLE_H
#define MPROGRESSINDICATORSTYLE_H

#include <mwidgetstyle.h>

/*!
    \class MProgressIndicatorStyle
    \brief Style for progress indicator.

    \ingroup styles
    \sa MProgressIndicatorStyleContainer
*/
class M_EXPORT MProgressIndicatorStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MProgressIndicatorStyle)

    /*!
        \property MProgressIndicatorStyle::activeImage
        \brief Image of the active bar element.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, activeImage, ActiveImage)

    /*!
        \property MProgressIndicatorStyle::inactiveImage
        \brief  Image of the inactive bar element.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, inactiveImage, InactiveImage)

    /*!
        \property MProgressIndicatorStyle::speed
        \brief Speed of unknown duration progress animation (distance/sec).
    */
    M_STYLE_ATTRIBUTE(int, speed, Speed)

    /*!
        \property MProgressIndicatorStyle::activeElementCount
        \brief The number of active elements in circular progress indicator (unknown duration).
    */
    M_STYLE_ATTRIBUTE(int, activeElementCount, ActiveElementCount)

    /*!
        \property MProgressIndicatorStyle::elementSize
        \brief Size of an element in the circle.
    */
    M_STYLE_ATTRIBUTE(qreal, elementSize, ElementSize)

    /*!
        \property MProgressIndicatorStyle::elementDistance
        \brief Distance between circle elements as a multiplier to element size.
    */
    M_STYLE_ATTRIBUTE(qreal, elementDistance, ElementDistance)
};

/*!
    \class MProgressIndicatorStyleContainer
    \brief This class groups all the styling modes for progress indicator.

    \ingroup styles
    \sa MProgressIndicatorStyle
*/
class M_EXPORT MProgressIndicatorStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MProgressIndicatorStyle)
};

#endif

