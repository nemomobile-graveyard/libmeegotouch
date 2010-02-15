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

#ifndef DUICONTENTITEMSTYLE_H
#define DUICONTENTITEMSTYLE_H

#include <duiwidgetstyle.h>

class DUI_EXPORT DuiContentItemStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiContentItemStyle)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageTopLeft,    BackgroundImageTopLeft)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageTop,    BackgroundImageTop)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageTopRight,    BackgroundImageTopRight)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageLeft,    BackgroundImageLeft)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageCenter,    BackgroundImageCenter)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageRight,    BackgroundImageRight)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageBottomLeft,    BackgroundImageBottomLeft)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageBottom,    BackgroundImageBottom)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageBottomRight,    BackgroundImageBottomRight)

    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageSinglerowLeft,    BackgroundImageSinglerowLeft)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageSinglerowCenter,    BackgroundImageSinglerowCenter)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageSinglerowRight,    BackgroundImageSinglerowRight)

    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageSinglecolumnTop,    BackgroundImageSinglecolumnTop)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageSinglecolumnCenter,    BackgroundImageSinglecolumnCenter)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *,  backgroundImageSinglecolumnBottom,    BackgroundImageSinglecolumnBottom)

    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, backgroundImageSingle, BackgroundImageSingle)

    DUI_STYLE_ATTRIBUTE(QString, titleObjectName, TitleObjectName)
    DUI_STYLE_ATTRIBUTE(QString, subtitleObjectName, SubtitleObjectName)
    DUI_STYLE_ATTRIBUTE(QString, imageObjectName, ImageObjectName)
};

class DUI_EXPORT DuiContentItemStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiContentItemStyle)
};

#endif
