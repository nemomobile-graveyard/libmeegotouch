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

#include "duiappletvisibilitymessage.h"
#include "duiappletmessagefactory.h"

DuiAppletVisibilityMessage::DuiAppletVisibilityMessage(bool visible) :
    DuiAppletMessage(VisibilityMessage),
    _visible(visible)
{
}

DuiAppletVisibilityMessage::~DuiAppletVisibilityMessage()
{
}

void DuiAppletVisibilityMessage::serialize(QDataStream &stream) const
{
    stream << _visible;
}

void DuiAppletVisibilityMessage::unserialize(QDataStream &stream)
{
    stream >> _visible;
}

bool DuiAppletVisibilityMessage::isVisible() const
{
    return _visible;
}

void DuiAppletVisibilityMessage::setVisible(bool visible)
{
    _visible = visible;
}

