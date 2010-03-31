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

#ifndef DUISCENEWINDOWSTYLE_H
#define DUISCENEWINDOWSTYLE_H

#include <QPointF>
#include <duiwidgetstyle.h>

class DUI_EXPORT DuiSceneWindowStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(DuiSceneWindowStyle)

    DUI_STYLE_ATTRIBUTE(Qt::Alignment,  horizontalAlign,      HorizontalAlign)
    DUI_STYLE_ATTRIBUTE(Qt::Alignment,  verticalAlign,        VerticalAlign)
    DUI_STYLE_ATTRIBUTE(QPointF,        offset,               Offset)
};

class DUI_EXPORT DuiSceneWindowStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(DuiSceneWindowStyle)
};

#endif

