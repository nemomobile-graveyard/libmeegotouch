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

#include "mcontentitemmodel.h"

void MContentItemModel::setItemQImage(QImage const& itemQImage)
{
    _itemQImage() = itemQImage;
    memberModified(ItemQImage);
}

const QImage &MContentItemModel::itemQImage() const
{
    return _itemQImage();
}

void MContentItemModel::setItemPixmap(const QPixmap &itemImage)
{
    _itemImageID() = "";
    _itemPixmap() = itemImage;
    memberModified(ItemPixmap);
}

void MContentItemModel::setOptionalPixmap(const QPixmap& pixmap)
{
    _optionalPixmap() = pixmap;
    memberModified(OptionalPixmap);
}

const QPixmap &MContentItemModel::optionalPixmap() const
{
    return _optionalPixmap();
}

const QPixmap &MContentItemModel::itemPixmap() const
{
    return _itemPixmap();
}

void MContentItemModel::setItemImage(const QImage &itemImage)
{
    _itemImageID() = "";
    _itemImage() = itemImage;
    memberModified(ItemImage);
}

void MContentItemModel::setOptionalImage(const QImage& image)
{
    _optionalImage() = image;
    memberModified(OptionalImage);
}

const QImage &MContentItemModel::optionalImage() const
{
    return _optionalImage();
}

const QImage &MContentItemModel::itemImage() const
{
    return _itemImage();
}
