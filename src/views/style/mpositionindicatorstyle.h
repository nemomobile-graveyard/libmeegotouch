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

#ifndef MPOSITIONINDICATORSTYLE_H
#define MPOSITIONINDICATORSTYLE_H

#include <QFont>
#include <mwidgetstyle.h>

class QPixmap;

class M_VIEWS_EXPORT MPositionIndicatorStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MPositionIndicatorStyle)

    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, indicatorImage, IndicatorImage)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, indicatorImageHorizontal, IndicatorImageHorizontal)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, backgroundImageHorizontal, BackgroundImageHorizontal)
    M_STYLE_ATTRIBUTE(int, minIndicatorSize, MinIndicatorSize)
    M_STYLE_ATTRIBUTE(int, hideTimeout, HideTimeout)
};

class M_VIEWS_EXPORT MPositionIndicatorStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MPositionIndicatorStyle)
};

#endif
