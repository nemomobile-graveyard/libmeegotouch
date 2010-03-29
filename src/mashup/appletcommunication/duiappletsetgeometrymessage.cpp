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

#include "duiappletsetgeometrymessage.h"
#include "duiappletmessagefactory.h"

DuiAppletSetGeometryMessage::DuiAppletSetGeometryMessage(const QRectF &geometry, Qt::HANDLE handle) :
    DuiAppletMessage(SetGeometryMessage),
    _geometry(geometry),
    _handle(handle)
{
}

DuiAppletSetGeometryMessage::~DuiAppletSetGeometryMessage()
{
}

void DuiAppletSetGeometryMessage::serialize(QDataStream &stream) const
{
    stream << _geometry;
    stream << (quint64)_handle;
}

void DuiAppletSetGeometryMessage::unserialize(QDataStream &stream)
{
    quint64 tmp;
    stream >> _geometry;
    stream >> tmp;
    _handle = (Qt::HANDLE)tmp;
}

QRectF DuiAppletSetGeometryMessage::geometry() const
{
    return _geometry;
}

void DuiAppletSetGeometryMessage::setGeometry(const QRectF &newGeometry)
{
    _geometry = newGeometry;
}

Qt::HANDLE DuiAppletSetGeometryMessage::handle() const
{
    return _handle;
}

void DuiAppletSetGeometryMessage::setHandle(Qt::HANDLE newHandle)
{
    _handle = newHandle;
}
