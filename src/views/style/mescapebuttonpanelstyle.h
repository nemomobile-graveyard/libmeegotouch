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

#ifndef MESCAPEBUTTONPANELSTYLE_H
#define MESCAPEBUTTONPANELSTYLE_H

#include <mscenewindowstyle.h>

class M_VIEWS_EXPORT MEscapeButtonPanelStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MEscapeButtonPanelStyle)

    M_STYLE_ATTRIBUTE(bool, hasCloseButton, HasCloseButton)
    M_STYLE_ATTRIBUTE(int, buttonAnimationLength, ButtonAnimationLength)
    M_STYLE_ATTRIBUTE(QString, backButtonObjectName, BackButtonObjectName)
    M_STYLE_ATTRIBUTE(QString, closeButtonObjectName, CloseButtonObjectName)
    M_STYLE_ATTRIBUTE(QString, backButtonIconId, BackButtonIconId)
    M_STYLE_ATTRIBUTE(QString, closeButtonIconId, CloseButtonIconId)
};

class M_VIEWS_EXPORT MEscapeButtonPanelStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MEscapeButtonPanelStyle)
    M_STYLE_MODE(Fullscreen)
};

#endif // MESCAPEBUTTONPANELSTYLE_H

