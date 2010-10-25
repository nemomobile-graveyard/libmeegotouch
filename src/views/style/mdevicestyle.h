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

#ifndef MDEVICESTYLE_H
#define MDEVICESTYLE_H

#include <mstyle.h>

#include <QObject>
#include <QSize>

class M_VIEWS_EXPORT MDeviceStyle : public MStyle
{
    Q_OBJECT
    M_STYLE(MDeviceStyle)

    M_STYLE_ATTRIBUTE(QSize, resolution, Resolution)
    M_STYLE_ATTRIBUTE(QSize, pixelsPerInch, PixelsPerInch)
};

class M_VIEWS_EXPORT MDeviceStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER(MDeviceStyle)
};

#endif
