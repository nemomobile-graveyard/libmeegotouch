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

#include "ut_mappletupdategeometrymessage.h"
#include "mappletupdategeometrymessage.h"

#include <QtTest/QtTest>

void Ut_MAppletUpdateGeometryMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new MAppletUpdateGeometryMessage();
}

void Ut_MAppletUpdateGeometryMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_MAppletUpdateGeometryMessage::testType()
{
    QCOMPARE(message->type(), MAppletMessage::UpdateGeometryMessage);
}

void Ut_MAppletUpdateGeometryMessage::testSizeHintsProperty()
{
    QCOMPARE(message->sizeHints(), QVector<QSizeF>(Qt::NSizeHints));
    QVector<QSizeF> sizeHints;
    sizeHints.append(QSizeF(1, 2));
    sizeHints.append(QSizeF(3, 4));
    sizeHints.append(QSizeF(5, 6));
    sizeHints.append(QSizeF(7, 8));
    message->setSizeHints(sizeHints);
    QCOMPARE(message->sizeHints(), sizeHints);
}

void Ut_MAppletUpdateGeometryMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    QVector<QSizeF> sizeHints;
    *stream >> sizeHints;

    QCOMPARE(sizeHints, QVector<QSizeF>(Qt::NSizeHints));
}

void Ut_MAppletUpdateGeometryMessage::testUnserialization()
{
    QVector<QSizeF> sizeHints;
    sizeHints.append(QSizeF(1, 2));
    sizeHints.append(QSizeF(3, 4));
    sizeHints.append(QSizeF(5, 6));
    sizeHints.append(QSizeF(7, 8));
    *stream << sizeHints;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->sizeHints(), sizeHints);
}

QTEST_APPLESS_MAIN(Ut_MAppletUpdateGeometryMessage)
