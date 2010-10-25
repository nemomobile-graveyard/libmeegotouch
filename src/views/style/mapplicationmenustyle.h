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

#ifndef MAPPLICATIONMENUSTYLE_H
#define MAPPLICATIONMENUSTYLE_H

#include <QColor>
#include <mscenewindowstyle.h>

class MScalableImage;

/*!
    \class MApplicationMenuStyle
    \brief Style class for application menu.

    \code
        MApplicationMenuStyle {
            canvas-image: "canvas-background" 3mm 3mm 3mm 3mm;
            canvas-color: #000000;
            canvas-opacity: 1.0;
            canvas-margin: 12;

            divider-image:;
            divider-color: #F9A427;
            divider-opacity: 1.0;
            divider-width: 1;

            item-height: 88;
            gap-height: 1;
            gap-color: #F9A427;
            gap-opacity: 1.0;
        }
    \endcode

    \ingroup styles
    \sa MApplicationMenuStyleContainer MSceneWindowStyle \ref styling MApplicationMenu MApplicationMenuView
*/

class M_VIEWS_EXPORT MApplicationMenuStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MApplicationMenuStyle)

    /*!
        \property MApplicationMenuStyle::canvasImage
        \brief Image for the menu canvas.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  canvasImage,          CanvasImage)

    /*!
        \property MApplicationMenuStyle::canvasColor
        \brief Color of the menu canvas.
    */
    M_STYLE_ATTRIBUTE(QColor,                 canvasColor,          CanvasColor)

    /*!
        \property MApplicationMenuStyle::canvasOpacity
        \brief Opacity of the menu canvas.
    */
    M_STYLE_ATTRIBUTE(qreal,                  canvasOpacity,        CanvasOpacity)

    /*!
        \property MApplicationMenuStyle::canvasMargin
        \brief Margin of the menu canvas.
    */
    M_STYLE_ATTRIBUTE(int,                    canvasMargin,         CanvasMargin)

    /*!
        \property MApplicationMenuStyle::dividerImage
        \brief Image for the menu divider.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  dividerImage,         DividerImage)

    /*!
        \property MApplicationMenuStyle::dividerColor
        \brief Color of the menu divider.
    */
    M_STYLE_ATTRIBUTE(QColor,                 dividerColor,         DividerColor)

    /*!
        \property MApplicationMenuStyle::dividerOpacity
        \brief Opacity of the menu divider.
    */
    M_STYLE_ATTRIBUTE(qreal,                  dividerOpacity,       DividerOpacity)

    /*!
        \property MApplicationMenuStyle::dividerWidth
        \brief Width of the menu divider.
    */
    M_STYLE_ATTRIBUTE(int,                    dividerWidth,         DividerWidth)

    /*!
        \property MApplicationMenuStyle::itemHeight
        \brief Height of the menu item.
    */
    M_STYLE_ATTRIBUTE(int,                    itemHeight,           ItemHeight)

    /*!
        \property MApplicationMenuStyle::gapHeight
        \brief Height of the menu gap.
    */
    M_STYLE_ATTRIBUTE(int,                    gapHeight,            GapHeight)

    /*!
        \property MApplicationMenuStyle::gapColor
        \brief Color of the menu gap.
    */
    M_STYLE_ATTRIBUTE(QColor,                 gapColor,             GapColor)

    /*!
        \property MApplicationMenuStyle::gapOpacity
        \brief Opacity of the menu gap.
    */
    M_STYLE_ATTRIBUTE(qreal,                  gapOpacity,           GapOpacity)
};

/*!
    \class MApplicationMenuStyleContainer
    \brief Style mode container class for MApplicationMenuStyle.

    \ingroup styles
    \sa MApplicationMenuStyle
*/
class M_VIEWS_EXPORT MApplicationMenuStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MApplicationMenuStyle)
};

#endif

