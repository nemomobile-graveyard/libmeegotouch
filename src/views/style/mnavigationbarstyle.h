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

#ifndef MNAVIGATIONBARSTYLE_H
#define MNAVIGATIONBARSTYLE_H

#include <mscenewindowstyle.h>

class M_VIEWS_EXPORT MNavigationBarStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MNavigationBarStyle)

    M_STYLE_ATTRIBUTE(int, itemSpacing,           ItemSpacing)
    M_STYLE_ATTRIBUTE(int, buttonAnimationLength, ButtonAnimationLength)
    M_STYLE_ATTRIBUTE(int, height,                Height)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage*, dropShadowImage, DropShadowImage)
    M_STYLE_ATTRIBUTE(bool, hasTitle, HasTitle)
    M_STYLE_ATTRIBUTE(bool, hasCloseButton, HasCloseButton)

    M_STYLE_ATTRIBUTE(QString, menuButtonStyleName, MenuButtonStyleName)
    M_STYLE_ATTRIBUTE(QString, escapeButtonSlotStyleName, EscapeButtonSlotStyleName)
    M_STYLE_ATTRIBUTE(QString, backButtonStyleName, BackButtonStyleName)
    M_STYLE_ATTRIBUTE(QString, closeButtonStyleName, CloseButtonStyleName)
};

class M_VIEWS_EXPORT MNavigationBarStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MNavigationBarStyle)
    M_STYLE_MODE(Fullscreen)
};

#endif

