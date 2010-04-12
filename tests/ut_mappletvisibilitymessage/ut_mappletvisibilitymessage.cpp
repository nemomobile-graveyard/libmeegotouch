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

#include "ut_mappletvisibilitymessage.h"

#include <QtTest/QtTest>
#include "mappletvisibilitymessage.h"

void Ut_MAppletVisibilityMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new MAppletVisibilityMessage();
}

void Ut_MAppletVisibilityMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_MAppletVisibilityMessage::testType()
{
    QCOMPARE(message->type(), MAppletMessage::VisibilityMessage);
}

void Ut_MAppletVisibilityMessage::testVisibilityProperty()
{
    QVERIFY(message->isVisible());
    message->setVisible(false);
    QVERIFY(!message->isVisible());
}

void Ut_MAppletVisibilityMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    bool result = false;
    *stream >> result;

    QVERIFY(result);
}

void Ut_MAppletVisibilityMessage::testUnserialization()
{
    *stream << false;

    buffer->seek(0);
    message->unserialize(*stream);

    QVERIFY(!message->isVisible());
}

QTEST_APPLESS_MAIN(Ut_MAppletVisibilityMessage)
