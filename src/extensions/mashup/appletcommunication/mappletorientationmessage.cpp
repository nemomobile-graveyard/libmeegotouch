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

#include "mappletorientationmessage.h"
#include "mappletmessagefactory.h"

MAppletOrientationMessage::MAppletOrientationMessage(M::Orientation orientation) :
    MAppletMessage(OrientationMessage),
    _orientation(orientation)
{
}

MAppletOrientationMessage::~MAppletOrientationMessage()
{

}

void MAppletOrientationMessage::serialize(QDataStream &stream) const
{
    stream << (uchar)_orientation;
}

void MAppletOrientationMessage::unserialize(QDataStream &stream)
{
    uchar tmp;
    stream >> tmp;
    _orientation = (M::Orientation)tmp;
}

M::Orientation MAppletOrientationMessage::orientation() const
{
    return _orientation;
}

void MAppletOrientationMessage::setOrientation(M::Orientation orientation)
{
    _orientation = orientation;
}
