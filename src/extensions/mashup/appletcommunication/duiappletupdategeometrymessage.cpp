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

#include "duiappletupdategeometrymessage.h"

DuiAppletUpdateGeometryMessage::DuiAppletUpdateGeometryMessage(const QVector<QSizeF> &sizeHints) :
    DuiAppletMessage(UpdateGeometryMessage),
    _sizeHints(sizeHints)
{
}

DuiAppletUpdateGeometryMessage::~DuiAppletUpdateGeometryMessage()
{
}

void DuiAppletUpdateGeometryMessage::serialize(QDataStream &stream) const
{
    stream << _sizeHints;
}

void DuiAppletUpdateGeometryMessage::unserialize(QDataStream &stream)
{
    stream >> _sizeHints;
}

QVector<QSizeF> DuiAppletUpdateGeometryMessage::sizeHints() const
{
    return _sizeHints;
}

void DuiAppletUpdateGeometryMessage::setSizeHints(const QVector<QSizeF> &sizeHints)
{
    _sizeHints = sizeHints;
}
