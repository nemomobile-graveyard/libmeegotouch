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

#include "mappletpixmapmodifiedmessage.h"

MAppletPixmapModifiedMessage::MAppletPixmapModifiedMessage(const QRectF &geometry) :
    MAppletMessage(PixmapModifiedMessage),
    _geometry(geometry)
{
}

MAppletPixmapModifiedMessage::~MAppletPixmapModifiedMessage()
{
}

void MAppletPixmapModifiedMessage::serialize(QDataStream &stream) const
{
    stream << _geometry;
}

void MAppletPixmapModifiedMessage::unserialize(QDataStream &stream)
{
    stream >> _geometry;
}

QRectF MAppletPixmapModifiedMessage::geometry() const
{
    return _geometry;
}

void MAppletPixmapModifiedMessage::setGeometry(const QRectF &newGeometry)
{
    _geometry = newGeometry;
}
