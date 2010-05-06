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

#include "mapplettextchangedmessage.h"

MAppletTextChangedMessage::MAppletTextChangedMessage() :
    MAppletMessage(AppletTextMessage)
{
}

MAppletTextChangedMessage::~MAppletTextChangedMessage()
{
}

void MAppletTextChangedMessage::serialize(QDataStream &stream) const
{
    stream << _text;
}

void MAppletTextChangedMessage::unserialize(QDataStream &stream)
{
    stream >> _text;
}

QString MAppletTextChangedMessage::text() const
{
    return _text;
}

void MAppletTextChangedMessage::setText(const QString &newText)
{
    _text = newText;
}
