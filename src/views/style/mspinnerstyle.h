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

#ifndef MSPINNERSTYLE_H
#define MSPINNERSTYLE_H

#include <mwidgetstyle.h>

/*!
    \class MSpinnerStyle
    \brief Style for progress indicator.

    \ingroup styles
    \sa MSpinnerStyleContainer
*/
class M_EXPORT MSpinnerStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MSpinnerStyle)

    /*!
        \property MSpinnerStyle::activeImage
        \brief Image of the active bar element.
    */
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, activeImage, ActiveImage)

    /*!
        \property MSpinnerStyle::inactiveImage
        \brief  Image of the inactive bar element.
    */
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, inactiveImage, InactiveImage)

    /*!
        \property MSpinnerStyle::speed
        \brief Speed of unknown duration progress animation (distance/sec).
    */
    M_STYLE_ATTRIBUTE(int, speed, Speed)

    /*!
        \property MSpinnerStyle::activeElementCount
        \brief The number of active elements in circular progress indicator (unknown duration).
    */
    M_STYLE_ATTRIBUTE(int, activeElementCount, ActiveElementCount)

    /*!
        \property MSpinnerStyle::elementSize
        \brief Size of an element in the circle.
    */
    M_STYLE_ATTRIBUTE(qreal, elementSize, ElementSize)


    /*!
        \property MSpinnerStyle::elementCount
        \brief Count of elements in spinner.
    */
    M_STYLE_ATTRIBUTE(int, elementCount, ElementCount)
};

/*!
    \class MSpinnerStyleContainer
    \brief This class groups all the styling modes for progress indicator.

    \ingroup styles
    \sa MSpinnerStyle
*/
class M_EXPORT MSpinnerStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MSpinnerStyle)
};

#endif

