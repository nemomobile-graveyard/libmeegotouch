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

#ifndef MSTATUSBARSTYLE_H
#define MSTATUSBARSTYLE_H

#include <mscenewindowstyle.h>

class M_VIEWS_EXPORT MStatusBarStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE(MStatusBarStyle)

    M_STYLE_ATTRIBUTE(bool, useSwipeGesture, UseSwipeGesture)
    M_STYLE_ATTRIBUTE(int, swipeThreshold, SwipeThreshold)
    M_STYLE_ATTRIBUTE(qreal, pressDimFactor, PressDimFactor)
    M_STYLE_ATTRIBUTE(bool, enableStatusIndicatorMenu, EnableStatusIndicatorMenu)
};

class M_VIEWS_EXPORT MStatusBarStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER(MStatusBarStyle)
};


#endif // MSTATUSBARSTYLE_H
