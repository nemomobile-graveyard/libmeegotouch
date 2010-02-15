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

#ifndef DUIMODALSCENEWINDOWSTYLE_H
#define DUIMODALSCENEWINDOWSTYLE_H

#include <duiscenewindowstyle.h>

class DUI_EXPORT DuiModalSceneWindowStyle : public DuiSceneWindowStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiModalSceneWindowStyle)
};

class DUI_EXPORT DuiModalSceneWindowStyleContainer : public DuiSceneWindowStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiModalSceneWindowStyle)
};

#endif

