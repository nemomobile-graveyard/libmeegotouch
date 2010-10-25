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

#ifndef MCONTENTITEMSTYLE_H
#define MCONTENTITEMSTYLE_H

#include <mwidgetstyle.h>

class M_VIEWS_EXPORT MContentItemStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MContentItemStyle)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageTopLeft,    BackgroundImageTopLeft)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageTop,    BackgroundImageTop)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageTopRight,    BackgroundImageTopRight)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageLeft,    BackgroundImageLeft)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageCenter,    BackgroundImageCenter)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageRight,    BackgroundImageRight)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageBottomLeft,    BackgroundImageBottomLeft)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageBottom,    BackgroundImageBottom)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageBottomRight,    BackgroundImageBottomRight)

    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageSinglerowLeft,    BackgroundImageSinglerowLeft)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageSinglerowCenter,    BackgroundImageSinglerowCenter)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageSinglerowRight,    BackgroundImageSinglerowRight)

    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageSinglecolumnTop,    BackgroundImageSinglecolumnTop)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageSinglecolumnCenter,    BackgroundImageSinglecolumnCenter)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  backgroundImageSinglecolumnBottom,    BackgroundImageSinglecolumnBottom)

    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, backgroundImageSingle, BackgroundImageSingle)

    M_STYLE_ATTRIBUTE(QString, titleObjectName, TitleObjectName)
    M_STYLE_ATTRIBUTE(QString, subtitleObjectName, SubtitleObjectName)
    M_STYLE_ATTRIBUTE(QString, imageObjectName, ImageObjectName)
    M_STYLE_ATTRIBUTE(QString, optionalImageObjectName, OptionalImageObjectName)
};

class M_VIEWS_EXPORT MContentItemStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MContentItemStyle)
};

#endif
