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

#include "mappletupdategeometrymessage.h"

MAppletUpdateGeometryMessage::MAppletUpdateGeometryMessage(const QVector<QSizeF> &sizeHints) :
    MAppletMessage(UpdateGeometryMessage),
    _sizeHints(sizeHints)
{
}

MAppletUpdateGeometryMessage::~MAppletUpdateGeometryMessage()
{
}

void MAppletUpdateGeometryMessage::serialize(QDataStream &stream) const
{
    stream << _sizeHints;
}

void MAppletUpdateGeometryMessage::unserialize(QDataStream &stream)
{
    stream >> _sizeHints;
}

QVector<QSizeF> MAppletUpdateGeometryMessage::sizeHints() const
{
    return _sizeHints;
}

void MAppletUpdateGeometryMessage::setSizeHints(const QVector<QSizeF> &sizeHints)
{
    _sizeHints = sizeHints;
}
