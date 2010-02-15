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

#include "duiappleticonchangedmessage.h"

DuiAppletIconChangedMessage::DuiAppletIconChangedMessage() :
    DuiAppletMessage(APPLET_ICON_MESSAGE)
{
}

DuiAppletIconChangedMessage::~DuiAppletIconChangedMessage()
{
}

void DuiAppletIconChangedMessage::serialize(QDataStream &stream) const
{
    stream << _icon;
}

void DuiAppletIconChangedMessage::unserialize(QDataStream &stream)
{
    stream >> _icon;
}

QString DuiAppletIconChangedMessage::icon() const
{
    return _icon;
}

void DuiAppletIconChangedMessage::setIcon(const QString &newIcon)
{
    _icon = newIcon;
}
