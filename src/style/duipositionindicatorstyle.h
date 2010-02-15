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

#ifndef DUIPOSITIONINDICATORSTYLE_H
#define DUIPOSITIONINDICATORSTYLE_H

#include <QFont>
#include <duiwidgetstyle.h>

class QPixmap;

class DUI_EXPORT DuiPositionIndicatorStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiPositionIndicatorStyle)

    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *,       onPixmap,         OnPixmap)
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *,       offPixmap,        OffPixmap)
    DUI_STYLE_ATTRIBUTE(int,                pixmapDistance,   PixmapDistance)
    DUI_STYLE_ATTRIBUTE(int,                minIndicatorDots, MinIndicatorDots)
    DUI_STYLE_ATTRIBUTE(int,                hideTimeout,      HideTimeout)
};

class DUI_EXPORT DuiPositionIndicatorStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiPositionIndicatorStyle)
};

#endif

