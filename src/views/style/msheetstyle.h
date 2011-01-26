/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSHEETSTYLE_H
#define MSHEETSTYLE_H

#include <mscenewindowstyle.h>

class M_VIEWS_EXPORT MSheetStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MSheetStyle)

    M_STYLE_ATTRIBUTE(QString, headerSlotStyleName, HeaderSlotStyleName)
    M_STYLE_ATTRIBUTE(QString, centralSlotStyleName, CentralSlotStyleName)

    M_STYLE_ATTRIBUTE(QString, headerAnimation, HeaderAnimation)
};

class M_VIEWS_EXPORT MSheetStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MSheetStyle)
};

#endif // MSHEETSTYLE_H
