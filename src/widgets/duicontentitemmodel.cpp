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

#include "duicontentitemmodel.h"

void DuiContentItemModel::setItemPixmap(const QPixmap &itemImage)
{
    _itemPixmap() = itemImage;
    memberModified(ItemPixmap);
}

void DuiContentItemModel::setOptionalPixmap(const QPixmap& pixmap)
{
    _optionalPixmap() = pixmap;
    memberModified(OptionalPixmap);
}

const QPixmap &DuiContentItemModel::optionalPixmap() const
{
    return _optionalPixmap();
}

const QPixmap &DuiContentItemModel::itemPixmap() const
{
    return _itemPixmap();
}

void DuiContentItemModel::setItemImage(const QImage &itemImage)
{
    _itemImage() = itemImage;
    memberModified(ItemImage);
}

void DuiContentItemModel::setOptionalImage(const QImage& image)
{
    _optionalImage() = image;
    memberModified(OptionalImage);
}

const QImage &DuiContentItemModel::optionalImage() const
{
    return _optionalImage();
}

const QImage &DuiContentItemModel::itemImage() const
{
    return _itemImage();
}

void DuiContentItemModel::setItemQImage(const QImage &itemImage)
{
    _itemQImage() = itemImage;
    memberModified(ItemQImage);
}

void DuiContentItemModel::setOptionalQImage(const QImage& pixmap)
{
    _optionalQImage() = pixmap;
    memberModified(OptionalQImage);
}

const QImage &DuiContentItemModel::optionalQImage() const
{
    return _optionalQImage();
}

const QImage &DuiContentItemModel::itemQImage() const
{
    return _itemQImage();
}
