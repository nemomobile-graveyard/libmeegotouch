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

#include "mappletpixmaptakenintousemessage.h"
#include "mappletmessagefactory.h"

MAppletPixmapTakenIntoUseMessage::MAppletPixmapTakenIntoUseMessage(Qt::HANDLE handle) :
    MAppletMessage(PixmapTakenIntoUseMessage),
    _handle(handle)
{
}

MAppletPixmapTakenIntoUseMessage::~MAppletPixmapTakenIntoUseMessage()
{
}

void MAppletPixmapTakenIntoUseMessage::serialize(QDataStream &stream) const
{
    stream << (quint64)_handle;
}

void MAppletPixmapTakenIntoUseMessage::unserialize(QDataStream &stream)
{
    quint64 tmp;
    stream >> tmp;
    _handle = (Qt::HANDLE)tmp;
}

Qt::HANDLE MAppletPixmapTakenIntoUseMessage::handle() const
{
    return _handle;
}

void MAppletPixmapTakenIntoUseMessage::setHandle(Qt::HANDLE newHandle)
{
    _handle = newHandle;
}
