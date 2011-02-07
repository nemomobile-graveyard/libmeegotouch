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

#ifndef MBANNERSTYLE_H
#define MBANNERSTYLE_H

#include <mscenewindowstyle.h>

class M_VIEWS_EXPORT MBannerStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE(MBannerStyle)

    M_STYLE_ATTRIBUTE(QSize,iconSize,IconSize)
    M_STYLE_ATTRIBUTE(qreal,pressDownBannerOpacity,PressDownBannerOpacity)
};

class M_VIEWS_EXPORT MBannerStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER(MBannerStyle)
};

#endif
