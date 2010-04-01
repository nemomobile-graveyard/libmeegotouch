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

#ifndef DUINAVIGATIONBARSTYLE_H
#define DUINAVIGATIONBARSTYLE_H

#include <duiscenewindowstyle.h>

class DUI_EXPORT DuiNavigationBarStyle : public DuiSceneWindowStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiNavigationBarStyle)

    DUI_STYLE_ATTRIBUTE(int, itemSpacing,           ItemSpacing)
    DUI_STYLE_ATTRIBUTE(int, buttonAnimationLength, ButtonAnimationLength)
    DUI_STYLE_ATTRIBUTE(int, height,                Height)
};

class DUI_EXPORT DuiNavigationBarStyleContainer : public DuiSceneWindowStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiNavigationBarStyle)
    DUI_STYLE_MODE(Fullscreen)
};

#endif

