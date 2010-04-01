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

#include <QPoint>
#include <QContextMenuEvent>
#include "duiappletobjectmenurequestmessage.h"
#include "duiappletmessagefactory.h"

DuiAppletObjectMenuRequestMessage::DuiAppletObjectMenuRequestMessage(QPointF pos) :
    DuiAppletMessage(ObjectMenuRequestMessage),
    _pos(pos)
{
}

DuiAppletObjectMenuRequestMessage::~DuiAppletObjectMenuRequestMessage()
{
}

void DuiAppletObjectMenuRequestMessage::serialize(QDataStream &stream) const
{
    stream << _pos;
}

void DuiAppletObjectMenuRequestMessage::unserialize(QDataStream &stream)
{
    stream >> _pos;
}

QPointF DuiAppletObjectMenuRequestMessage::pos() const
{
    return _pos;
}
