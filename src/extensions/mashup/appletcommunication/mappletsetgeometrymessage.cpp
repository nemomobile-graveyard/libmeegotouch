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

#include "mappletsetgeometrymessage.h"
#include "mappletmessagefactory.h"

MAppletSetGeometryMessage::MAppletSetGeometryMessage(const QRectF &geometry, Qt::HANDLE handle) :
    MAppletMessage(SetGeometryMessage),
    _geometry(geometry),
    _handle(handle)
{
}

MAppletSetGeometryMessage::~MAppletSetGeometryMessage()
{
}

void MAppletSetGeometryMessage::serialize(QDataStream &stream) const
{
    stream << _geometry;
    stream << (quint64)_handle;
}

void MAppletSetGeometryMessage::unserialize(QDataStream &stream)
{
    quint64 tmp;
    stream >> _geometry;
    stream >> tmp;
    _handle = (Qt::HANDLE)tmp;
}

QRectF MAppletSetGeometryMessage::geometry() const
{
    return _geometry;
}

void MAppletSetGeometryMessage::setGeometry(const QRectF &newGeometry)
{
    _geometry = newGeometry;
}

Qt::HANDLE MAppletSetGeometryMessage::handle() const
{
    return _handle;
}

void MAppletSetGeometryMessage::setHandle(Qt::HANDLE newHandle)
{
    _handle = newHandle;
}
