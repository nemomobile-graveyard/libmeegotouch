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

#ifndef MCONTAINERSTYLE_H
#define MCONTAINERSTYLE_H

#include <mwidgetstyle.h>

#include <QSize>

class M_EXPORT MContainerStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MContainerStyle)

    M_STYLE_ATTRIBUTE(QSize,   iconSize,           IconSize)
    M_STYLE_ATTRIBUTE(int,     internalMargins,    InternalMargins)
    M_STYLE_ATTRIBUTE(int,     internalItemSpacing, InternalItemSpacing)
};

class M_EXPORT MContainerStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MContainerStyle)
};

#endif
