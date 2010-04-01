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

#ifndef DUIDEVICESTYLE_H
#define DUIDEVICESTYLE_H

#include <duistyle.h>

#include <QObject>
#include <QSize>

class DUI_EXPORT DuiDeviceStyle : public DuiStyle
{
    Q_OBJECT
    DUI_STYLE(DuiDeviceStyle)

    DUI_STYLE_ATTRIBUTE(QSize, resolution, Resolution)
    DUI_STYLE_ATTRIBUTE(QSize, pixelsPerInch, PixelsPerInch)
};

class DUI_EXPORT DuiDeviceStyleContainer : public DuiStyleContainer
{
    DUI_STYLE_CONTAINER(DuiDeviceStyle)
};

#endif
