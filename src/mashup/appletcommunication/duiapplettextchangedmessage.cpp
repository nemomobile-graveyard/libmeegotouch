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

#include "duiapplettextchangedmessage.h"

DuiAppletTextChangedMessage::DuiAppletTextChangedMessage() :
    DuiAppletMessage(APPLET_TEXT_MESSAGE)
{
}

DuiAppletTextChangedMessage::~DuiAppletTextChangedMessage()
{
}

void DuiAppletTextChangedMessage::serialize(QDataStream &stream) const
{
    stream << _text;
}

void DuiAppletTextChangedMessage::unserialize(QDataStream &stream)
{
    stream >> _text;
}

QString DuiAppletTextChangedMessage::text() const
{
    return _text;
}

void DuiAppletTextChangedMessage::setText(const QString &newText)
{
    _text = newText;
}
