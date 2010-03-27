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

void DuiContentItemModel::setItemImage(const QPixmap &itemImage)
{
    _itemImage() = itemImage;
    memberModified(ItemImage);
}

void DuiContentItemModel::setOptionalImage(const QPixmap& pixmap)
{
    _optionalImage() = pixmap;
    memberModified(OptionalImage);
}

const QPixmap &DuiContentItemModel::optionalImage() const
{
    return _optionalImage();
}

const QPixmap &DuiContentItemModel::itemImage() const
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
