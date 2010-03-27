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

#ifndef DUISPINNERSTYLE_H
#define DUISPINNERSTYLE_H

#include <duiwidgetstyle.h>

/*!
    \class DuiSpinnerStyle
    \brief Style for progress indicator.

    \ingroup styles
    \sa DuiSpinnerStyleContainer
*/
class DUI_EXPORT DuiSpinnerStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiSpinnerStyle)

    /*!
        \property DuiSpinnerStyle::activeImage
        \brief Image of the active bar element.
    */
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, activeImage, ActiveImage)

    /*!
        \property DuiSpinnerStyle::inactiveImage
        \brief  Image of the inactive bar element.
    */
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, inactiveImage, InactiveImage)

    /*!
        \property DuiSpinnerStyle::speed
        \brief Speed of unknown duration progress animation (distance/sec).
    */
    DUI_STYLE_ATTRIBUTE(int, speed, Speed)

    /*!
        \property DuiSpinnerStyle::activeElementCount
        \brief The number of active elements in circular progress indicator (unknown duration).
    */
    DUI_STYLE_ATTRIBUTE(int, activeElementCount, ActiveElementCount)

    /*!
        \property DuiSpinnerStyle::elementSize
        \brief Size of an element in the circle.
    */
    DUI_STYLE_ATTRIBUTE(qreal, elementSize, ElementSize)


    /*!
        \property DuiSpinnerStyle::elementCount
        \brief Count of elements in spinner.
    */
    DUI_STYLE_ATTRIBUTE(int, elementCount, ElementCount)
};

/*!
    \class DuiSpinnerStyleContainer
    \brief This class groups all the styling modes for progress indicator.

    \ingroup styles
    \sa DuiSpinnerStyle
*/
class DUI_EXPORT DuiSpinnerStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiSpinnerStyle)
};

#endif

