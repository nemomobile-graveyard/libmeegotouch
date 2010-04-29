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

#include <QtTest/QtTest>
#include <QBuffer>
#include <QDataStream>

#include "mappletmessage.h"
#include "ut_mappletmessage.h"

bool testAppletMessageSerialized = false;
bool testAppletMessageUnserialized = false;

class TestAppletMessage : public MAppletMessage
{
public:
    TestAppletMessage() : MAppletMessage((MAppletMessageType)1),
            foo(0)
    {
    }

    int foo;

    void serialize(QDataStream &stream) const {
        stream << foo;
        testAppletMessageSerialized = true;
    }

    void unserialize(QDataStream &stream) {
        stream >> foo;
        testAppletMessageUnserialized = true;
    }
};

void Ut_MAppletMessage::init()
{
    // Allocate a buffer and a stream for serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);

    stream = new QDataStream(buffer);

    // Allocate test messages
    message = new TestAppletMessage;
}

void Ut_MAppletMessage::cleanup()
{
    delete buffer;
    delete stream;
    delete message;
}

void Ut_MAppletMessage::testType()
{
    QCOMPARE(message->type(), (MAppletMessage::MAppletMessageType)1);
}

void Ut_MAppletMessage::testSerialization()
{
    testAppletMessageSerialized = false;

    message->serialize(*stream);

    QVERIFY(testAppletMessageSerialized);
}

void Ut_MAppletMessage::testUnserialization()
{
    testAppletMessageUnserialized = false;

    *stream << 1;
    // Seek to the beginning of the buffer
    buffer->seek(0);

    // Unserialize the message and compare whether it is similar than it used to be
    message = new TestAppletMessage;
    message->unserialize(*stream);

    QCOMPARE(((TestAppletMessage *)message)->foo, 1);
    QVERIFY(testAppletMessageUnserialized);
}

QTEST_APPLESS_MAIN(Ut_MAppletMessage)
