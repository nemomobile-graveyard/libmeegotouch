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

#include "minfobannermodel.h"

const QPixmap &MInfoBannerModel::pixmap() const
{
    return _pixmap();
}

void MInfoBannerModel::setPixmap(const QPixmap &pixmap)
{
    _pixmap() = pixmap;
    _imageID().clear();

    memberModified(Pixmap);
}

const QString &MInfoBannerModel::imageID() const
{
    return _imageID();
}

void MInfoBannerModel::setImageID(const QString &image)
{
    _pixmap() = QPixmap();
    _imageID() = image;

    memberModified(ImageID);
}
