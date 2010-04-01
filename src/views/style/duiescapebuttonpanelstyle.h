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

#ifndef DUIESCAPEBUTTONPANELSTYLE_H
#define DUIESCAPEBUTTONPANELSTYLE_H

#include <duiscenewindowstyle.h>

class DUI_EXPORT DuiEscapeButtonPanelStyle : public DuiSceneWindowStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiEscapeButtonPanelStyle)

    DUI_STYLE_ATTRIBUTE(int, buttonAnimationLength, ButtonAnimationLength)
    DUI_STYLE_ATTRIBUTE(QString, backButtonObjectName, BackButtonObjectName)
    DUI_STYLE_ATTRIBUTE(QString, closeButtonObjectName, CloseButtonObjectName)
    DUI_STYLE_ATTRIBUTE(QString, backButtonIconId, BackButtonIconId)
    DUI_STYLE_ATTRIBUTE(QString, closeButtonIconId, CloseButtonIconId)
};

class DUI_EXPORT DuiEscapeButtonPanelStyleContainer : public DuiSceneWindowStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiEscapeButtonPanelStyle)
    DUI_STYLE_MODE(Fullscreen)
};

#endif // DUIESCAPEBUTTONPANELSTYLE_H

