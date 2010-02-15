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

#ifndef DUIWIDGETSTYLE_H
#define DUIWIDGETSTYLE_H

#include <duistyle.h>

#include <QObject>
#include <QColor>
#include <QSize>
#include <duifeedback.h>

class QPixmap;
class DuiScalableImage;

/*!
    \class DuiWidgetStyle
    \brief DuiWidgetStyle implements a base for style objects for MVC views

 */
class DUI_EXPORT DuiWidgetStyle : public DuiStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiWidgetStyle)

    DUI_STYLE_ATTRIBUTE(QString, showAnimation, ShowAnimation)
    DUI_STYLE_ATTRIBUTE(QString, hideAnimation, HideAnimation)

    DUI_STYLE_ATTRIBUTE(int, marginLeft, MarginLeft)
    DUI_STYLE_ATTRIBUTE(int, marginTop, MarginTop)
    DUI_STYLE_ATTRIBUTE(int, marginRight, MarginRight)
    DUI_STYLE_ATTRIBUTE(int, marginBottom, MarginBottom)
    DUI_STYLE_ATTRIBUTE(int, paddingLeft, PaddingLeft)
    DUI_STYLE_ATTRIBUTE(int, paddingTop, PaddingTop)
    DUI_STYLE_ATTRIBUTE(int, paddingRight, PaddingRight)
    DUI_STYLE_ATTRIBUTE(int, paddingBottom, PaddingBottom)
    DUI_STYLE_ATTRIBUTE(int, reactiveMarginLeft, ReactiveMarginLeft)
    DUI_STYLE_ATTRIBUTE(int, reactiveMarginTop, ReactiveMarginTop)
    DUI_STYLE_ATTRIBUTE(int, reactiveMarginRight, ReactiveMarginRight)
    DUI_STYLE_ATTRIBUTE(int, reactiveMarginBottom, ReactiveMarginBottom)

    DUI_STYLE_ATTRIBUTE(int, backgroundBoxedPercent, BackgroundBoxedPercent)

    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImage,    BackgroundImage)
    DUI_STYLE_ATTRIBUTE(QColor,                 backgroundColor,    BackgroundColor)
    DUI_STYLE_ATTRIBUTE(qreal,                  backgroundOpacity,  BackgroundOpacity)

    DUI_STYLE_ATTRIBUTE(QSize, preferredSize, PreferredSize)
    DUI_STYLE_ATTRIBUTE(QSize, minimumSize,   MinimumSize)
    DUI_STYLE_ATTRIBUTE(QSize, maximumSize,   MaximumSize)

    DUI_STYLE_ATTRIBUTE(DuiFeedback, pressFeedback, PressFeedback)
    DUI_STYLE_ATTRIBUTE(DuiFeedback, releaseFeedback, ReleaseFeedback)
};

class DUI_EXPORT DuiWidgetStyleContainer : public DuiStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiWidgetStyle)
    DUI_STYLE_MODE(Disabled)
    DUI_STYLE_MODE(Selected)
    DUI_STYLE_MODE(Active)
    DUI_STYLE_MODE(Pressed)
};

#endif
