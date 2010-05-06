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

#include "ut_mappletpixmapmodifiedmessage.h"

#include <QtTest/QtTest>
#include "mappletpixmapmodifiedmessage.h"

void Ut_MAppletPixmapModifiedMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new MAppletPixmapModifiedMessage();
}

void Ut_MAppletPixmapModifiedMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_MAppletPixmapModifiedMessage::testType()
{
    QCOMPARE(message->type(), MAppletMessage::PixmapModifiedMessage);
}

void Ut_MAppletPixmapModifiedMessage::testGeometryProperty()
{
    QCOMPARE(message->geometry(), QRectF());
    QRectF tmp(1.0, 2.0, 3.0, 4.0);
    message->setGeometry(tmp);
    QCOMPARE(message->geometry(), tmp);
}

void Ut_MAppletPixmapModifiedMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    QRectF geometry(1.0, 2.0, 3.0, 4.0);
    *stream >> geometry;

    QCOMPARE(geometry, QRectF());
}

void Ut_MAppletPixmapModifiedMessage::testUnserialization()
{
    QRectF geometry(1.0, 2.0, 3.0, 4.0);
    *stream << geometry;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->geometry(), geometry);
}

QTEST_APPLESS_MAIN(Ut_MAppletPixmapModifiedMessage)
