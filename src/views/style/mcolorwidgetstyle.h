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

#ifndef MCOLORWIDGETSTYLE_H
#define MCOLORWIDGETSTYLE_H

#include <MWidgetStyle>
#include <QColor>

class MColorWidgetStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MColorWidgetStyle)
    M_STYLE_ATTRIBUTE(qreal, opacity, Opacity)
    M_STYLE_ATTRIBUTE(qreal, xRadius, XRadius)
    M_STYLE_ATTRIBUTE(qreal, yRadius, YRadius)
};

class MColorWidgetStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MColorWidgetStyle)
};

#endif
