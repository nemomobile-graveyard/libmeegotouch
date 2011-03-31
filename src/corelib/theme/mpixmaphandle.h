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

#include <qnamespace.h>
#include <QString>
#include <QSize>
#include <QImage>

#ifndef MPIXMAPHANDLE_H
#define MPIXMAPHANDLE_H

struct MPixmapHandle
{
    MPixmapHandle();

    bool isValid() const;

    Qt::HANDLE xHandle;
    Qt::HANDLE eglHandle;
    QByteArray shmHandle;
    QSize size;
    QImage::Format format;
    int numBytes;

    bool directMap;
};

#endif //MPIXMAPHANDLE_H
