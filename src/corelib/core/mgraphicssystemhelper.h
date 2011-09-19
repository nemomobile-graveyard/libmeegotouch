/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef MGRAPHICSSYSTEMHELPER_H
#define MGRAPHICSSYSTEMHELPER_H

#include "mexport.h"

class MWindow;
class QGLContext;
struct PixmapCacheEntry;
class QImage;
class QString;
struct  MPixmapHandle;
class QPixmap;
class QSize;

class M_CORE_EXPORT MGraphicsSystemHelper
{
public:
    static void pixmapFromImage(PixmapCacheEntry *cacheEntry, const QImage& image, const QString &uniqueKey, const QSize &requestedSize);
    static QPixmap pixmapFromHandle(const MPixmapHandle& pixmapHandle, void **addr, int *numBytes);

    static bool isRunningNativeGraphicsSystem();
    static bool isRunningMeeGoCompatibleGraphicsSystem();
    static bool isRunningMeeGoGraphicsSystem();

    static bool canSwitchBetweenSoftwareAndHardwareRendering();

    static void enableSwitchEvents();

    static void forceSoftwareRendering();
};



#endif //MGRAPHICSSYSTEMHELPER_H
