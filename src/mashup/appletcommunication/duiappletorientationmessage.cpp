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

#include "duiappletorientationmessage.h"
#include "duiappletmessagefactory.h"

DuiAppletOrientationMessage::DuiAppletOrientationMessage(Dui::Orientation orientation) :
    DuiAppletMessage(ORIENTATION_MESSAGE),
    _orientation(orientation)
{
}

DuiAppletOrientationMessage::~DuiAppletOrientationMessage()
{

}

void DuiAppletOrientationMessage::serialize(QDataStream &stream) const
{
    stream << (uchar)_orientation;
}

void DuiAppletOrientationMessage::unserialize(QDataStream &stream)
{
    uchar tmp;
    stream >> tmp;
    _orientation = (Dui::Orientation)tmp;
}

Dui::Orientation DuiAppletOrientationMessage::orientation() const
{
    return _orientation;
}

void DuiAppletOrientationMessage::setOrientation(Dui::Orientation orientation)
{
    _orientation = orientation;
}
