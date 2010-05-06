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

#include "mappletmessage.h"

MAppletMessage::MAppletMessage(MAppletMessageType type) :
    messageType(type)
{
}

MAppletMessage::~MAppletMessage()
{
}

MAppletMessage::MAppletMessageType MAppletMessage::type() const
{
    return messageType;
}

void MAppletMessage::serialize(QDataStream &stream) const
{
    Q_UNUSED(stream);
}

void MAppletMessage::unserialize(QDataStream &stream)
{
    Q_UNUSED(stream);
}
