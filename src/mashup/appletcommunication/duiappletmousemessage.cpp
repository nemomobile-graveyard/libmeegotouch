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

#include "duiappletmousemessage.h"

DuiAppletMouseMessage::DuiAppletMouseMessage(DuiAppletMessageType type, const QPointF &pos, Qt::MouseButton button, Qt::MouseButtons buttons) :
    DuiAppletMessage(type),
    _position(pos),
    _button(button),
    _buttons(buttons)
{
}

DuiAppletMouseMessage::~DuiAppletMouseMessage()
{
}

void DuiAppletMouseMessage::serialize(QDataStream &stream) const
{
    stream << _position;
    stream << _button;
    stream << _buttons;
}

void DuiAppletMouseMessage::unserialize(QDataStream &stream)
{
    quint32 tmp;
    stream >> _position;
    stream >> tmp;
    _button = (Qt::MouseButton)tmp;
    stream >> tmp;
    _buttons = (Qt::MouseButtons)tmp;
}

QPointF DuiAppletMouseMessage::position() const
{
    return _position;
}

void DuiAppletMouseMessage::setPosition(const QPointF &pos)
{
    _position = pos;
}

Qt::MouseButton DuiAppletMouseMessage::button() const
{
    return _button;
}

void DuiAppletMouseMessage::setButton(const Qt::MouseButton &button)
{
    _button = button;
}

Qt::MouseButtons DuiAppletMouseMessage::buttons() const
{
    return _buttons;
}

void DuiAppletMouseMessage::setButtons(const Qt::MouseButtons &buttons)
{
    _buttons = buttons;
}
