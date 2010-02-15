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

#ifndef DUIAPPLICATIONMENUSTYLE_H
#define DUIAPPLICATIONMENUSTYLE_H

#include <QColor>
#include <duiscenewindowstyle.h>

class DuiScalableImage;

/*!
    \class DuiApplicationMenuStyle
    \brief Style class for application menu.

    \code
        DuiApplicationMenuStyle {
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
    \sa DuiApplicationMenuStyleContainer DuiSceneWindowStyle \ref styling DuiApplicationMenu DuiApplicationMenuView
*/

class DUI_EXPORT DuiApplicationMenuStyle : public DuiSceneWindowStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiApplicationMenuStyle)

    /*!
        \property DuiApplicationMenuStyle::canvasImage
        \brief Image for the menu canvas.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  canvasImage,          CanvasImage)

    /*!
        \property DuiApplicationMenuStyle::canvasColor
        \brief Color of the menu canvas.
    */
    DUI_STYLE_ATTRIBUTE(QColor,                 canvasColor,          CanvasColor)

    /*!
        \property DuiApplicationMenuStyle::canvasOpacity
        \brief Opacity of the menu canvas.
    */
    DUI_STYLE_ATTRIBUTE(qreal,                  canvasOpacity,        CanvasOpacity)

    /*!
        \property DuiApplicationMenuStyle::canvasMargin
        \brief Margin of the menu canvas.
    */
    DUI_STYLE_ATTRIBUTE(int,                    canvasMargin,         CanvasMargin)

    /*!
        \property DuiApplicationMenuStyle::dividerImage
        \brief Image for the menu divider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  dividerImage,         DividerImage)

    /*!
        \property DuiApplicationMenuStyle::dividerColor
        \brief Color of the menu divider.
    */
    DUI_STYLE_ATTRIBUTE(QColor,                 dividerColor,         DividerColor)

    /*!
        \property DuiApplicationMenuStyle::dividerOpacity
        \brief Opacity of the menu divider.
    */
    DUI_STYLE_ATTRIBUTE(qreal,                  dividerOpacity,       DividerOpacity)

    /*!
        \property DuiApplicationMenuStyle::dividerWidth
        \brief Width of the menu divider.
    */
    DUI_STYLE_ATTRIBUTE(int,                    dividerWidth,         DividerWidth)

    /*!
        \property DuiApplicationMenuStyle::itemHeight
        \brief Height of the menu item.
    */
    DUI_STYLE_ATTRIBUTE(int,                    itemHeight,           ItemHeight)

    /*!
        \property DuiApplicationMenuStyle::gapHeight
        \brief Height of the menu gap.
    */
    DUI_STYLE_ATTRIBUTE(int,                    gapHeight,            GapHeight)

    /*!
        \property DuiApplicationMenuStyle::gapColor
        \brief Color of the menu gap.
    */
    DUI_STYLE_ATTRIBUTE(QColor,                 gapColor,             GapColor)

    /*!
        \property DuiApplicationMenuStyle::gapOpacity
        \brief Opacity of the menu gap.
    */
    DUI_STYLE_ATTRIBUTE(qreal,                  gapOpacity,           GapOpacity)
};

/*!
    \class DuiApplicationMenuStyleContainer
    \brief Style mode container class for DuiApplicationMenuStyle.

    \ingroup styles
    \sa DuiApplicationMenuStyle
*/
class DUI_EXPORT DuiApplicationMenuStyleContainer : public DuiSceneWindowStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiApplicationMenuStyle)
};

#endif

