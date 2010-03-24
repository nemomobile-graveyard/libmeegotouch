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

#include "ut_duiappletvisibilitymessage.h"

#include <QtTest/QtTest>
#include "duiappletvisibilitymessage.h"

void Ut_DuiAppletVisibilityMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new DuiAppletVisibilityMessage();
}

void Ut_DuiAppletVisibilityMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_DuiAppletVisibilityMessage::testType()
{
    QCOMPARE(message->type(), DuiAppletMessage::VISIBILITY_MESSAGE);
}

void Ut_DuiAppletVisibilityMessage::testVisibilityProperty()
{
    QVERIFY(message->isVisible());
    message->setVisible(false);
    QVERIFY(!message->isVisible());
}

void Ut_DuiAppletVisibilityMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    bool result = false;
    *stream >> result;

    QVERIFY(result);
}

void Ut_DuiAppletVisibilityMessage::testUnserialization()
{
    *stream << false;

    buffer->seek(0);
    message->unserialize(*stream);

    QVERIFY(!message->isVisible());
}

QTEST_APPLESS_MAIN(Ut_DuiAppletVisibilityMessage)
