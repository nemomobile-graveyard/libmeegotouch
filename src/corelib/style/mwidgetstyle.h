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

#ifndef MWIDGETSTYLE_H
#define MWIDGETSTYLE_H

#include <mstyle.h>

#include <QObject>
#include <QColor>
#include <QSize>
#include <mfeedback.h>
#include <mbackgroundtiles.h>

class QPixmap;
class MScalableImage;

/*!
    \class MWidgetStyle
    \brief MWidgetStyle implements a base for style objects for MVC views

 */
class M_EXPORT MWidgetStyle : public MStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MWidgetStyle)

    M_STYLE_ATTRIBUTE(int, marginLeft, MarginLeft)
    M_STYLE_ATTRIBUTE(int, marginTop, MarginTop)
    M_STYLE_ATTRIBUTE(int, marginRight, MarginRight)
    M_STYLE_ATTRIBUTE(int, marginBottom, MarginBottom)
    M_STYLE_ATTRIBUTE(int, paddingLeft, PaddingLeft)
    M_STYLE_ATTRIBUTE(int, paddingTop, PaddingTop)
    M_STYLE_ATTRIBUTE(int, paddingRight, PaddingRight)
    M_STYLE_ATTRIBUTE(int, paddingBottom, PaddingBottom)
    M_STYLE_ATTRIBUTE(int, reactiveMarginLeft, ReactiveMarginLeft)
    M_STYLE_ATTRIBUTE(int, reactiveMarginTop, ReactiveMarginTop)
    M_STYLE_ATTRIBUTE(int, reactiveMarginRight, ReactiveMarginRight)
    M_STYLE_ATTRIBUTE(int, reactiveMarginBottom, ReactiveMarginBottom)

    M_STYLE_ATTRIBUTE(int, backgroundBoxedPercent, BackgroundBoxedPercent)

    M_STYLE_ATTRIBUTE(MBackgroundTiles, backgroundTiles, BackgroundTiles)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImage,    BackgroundImage)
    M_STYLE_ATTRIBUTE(QColor,                 backgroundColor,    BackgroundColor)
    M_STYLE_ATTRIBUTE(qreal,                  backgroundOpacity,  BackgroundOpacity)

    M_STYLE_ATTRIBUTE(QSize, preferredSize, PreferredSize)
    M_STYLE_ATTRIBUTE(QSize, minimumSize,   MinimumSize)
    M_STYLE_ATTRIBUTE(QSize, maximumSize,   MaximumSize)

    M_STYLE_ATTRIBUTE(MFeedback, pressFeedback, PressFeedback)
    M_STYLE_ATTRIBUTE(MFeedback, releaseFeedback, ReleaseFeedback)
    M_STYLE_ATTRIBUTE(MFeedback, cancelFeedback, CancelFeedback)
};

class M_EXPORT MWidgetStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MWidgetStyle)
    M_STYLE_MODE(Disabled)
    M_STYLE_MODE(Selected)
    M_STYLE_MODE(Active)
    M_STYLE_MODE(Pressed)
};

#endif
