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

#include "ut_mappletorientationmessage.h"

#include <QtTest/QtTest>
#include "mappletorientationmessage.h"

void Ut_MAppletOrientationMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new MAppletOrientationMessage();
}

void Ut_MAppletOrientationMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_MAppletOrientationMessage::testType()
{
    QCOMPARE(message->type(), MAppletMessage::OrientationMessage);
}

void Ut_MAppletOrientationMessage::testOrientationProperty()
{
    QCOMPARE(message->orientation(), M::Landscape);
    message->setOrientation(M::Portrait);
    QCOMPARE(message->orientation(), M::Portrait);
}

void Ut_MAppletOrientationMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    uchar tmp = (uchar)M::Portrait;
    *stream >> tmp;
    M::Orientation result = (M::Orientation)tmp;

    QCOMPARE(result, M::Landscape);
}

void Ut_MAppletOrientationMessage::testUnserialization()
{
    *stream << (uchar)M::Portrait;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->orientation(), M::Portrait);
}

QTEST_APPLESS_MAIN(Ut_MAppletOrientationMessage)
