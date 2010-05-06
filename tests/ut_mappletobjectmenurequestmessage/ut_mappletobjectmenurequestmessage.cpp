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

#include "ut_mappletobjectmenurequestmessage.h"

#include <QtTest/QtTest>
#include "mappletobjectmenurequestmessage.h"

void Ut_MAppletObjectMenuRequestMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new MAppletObjectMenuRequestMessage(QPoint(1, 2));
}

void Ut_MAppletObjectMenuRequestMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_MAppletObjectMenuRequestMessage::testType()
{
    QCOMPARE(message->type(), MAppletMessage::ObjectMenuRequestMessage);
}

void Ut_MAppletObjectMenuRequestMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    QPointF point;
    *stream >> point;

    QCOMPARE(point, QPointF(1, 2));
}

void Ut_MAppletObjectMenuRequestMessage::testUnserialization()
{
    QPointF point(5, 6);
    *stream << point;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->pos(), point);
}

void Ut_MAppletObjectMenuRequestMessage::testPos()
{
    QCOMPARE(message->pos(), QPointF(1, 2));
}

QTEST_APPLESS_MAIN(Ut_MAppletObjectMenuRequestMessage)
