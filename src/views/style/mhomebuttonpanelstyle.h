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

#ifndef MHOMEBUTTONPANELSTYLE_H
#define MHOMEBUTTONPANELSTYLE_H

#include <mscenewindowstyle.h>

class M_VIEWS_EXPORT MHomeButtonPanelStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MHomeButtonPanelStyle)

    M_STYLE_ATTRIBUTE(QString, homeButtonObjectName, HomeButtonObjectName)
    M_STYLE_ATTRIBUTE(QString, homeButtonIconId, HomeButtonIconId)
};

class M_VIEWS_EXPORT MHomeButtonPanelStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MHomeButtonPanelStyle)
    M_STYLE_MODE(Fullscreen)
};

#endif // MHOMEBUTTONPANELSTYLE_H

