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

#include "duiappletpixmapmodifiedmessage.h"

DuiAppletPixmapModifiedMessage::DuiAppletPixmapModifiedMessage(const QRectF &geometry) :
    DuiAppletMessage(PIXMAP_MODIFIED_MESSAGE),
    _geometry(geometry)
{
}

DuiAppletPixmapModifiedMessage::~DuiAppletPixmapModifiedMessage()
{
}

void DuiAppletPixmapModifiedMessage::serialize(QDataStream &stream) const
{
    stream << _geometry;
}

void DuiAppletPixmapModifiedMessage::unserialize(QDataStream &stream)
{
    stream >> _geometry;
}

QRectF DuiAppletPixmapModifiedMessage::geometry() const
{
    return _geometry;
}

void DuiAppletPixmapModifiedMessage::setGeometry(const QRectF &newGeometry)
{
    _geometry = newGeometry;
}
