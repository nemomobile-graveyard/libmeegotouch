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

#include "ut_mappletobjectmenuactionselectedmessage.h"

#include <QtTest/QtTest>
#include <QAction>
#include "mappletobjectmenuactionselectedmessage.h"

void Ut_MAppletObjectMenuActionSelectedMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new MAppletObjectMenuActionSelectedMessage(100);
}

void Ut_MAppletObjectMenuActionSelectedMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_MAppletObjectMenuActionSelectedMessage::testType()
{
    QCOMPARE(message->type(), MAppletMessage::ObjectMenuActionSelectedMessage);
}

void Ut_MAppletObjectMenuActionSelectedMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    uint actionIndex = 0;
    *stream >> actionIndex;

    QCOMPARE(actionIndex, (uint)100);
}

void Ut_MAppletObjectMenuActionSelectedMessage::testUnserialization()
{
    uint actionIndex = 20;
    *stream << actionIndex;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->index(), actionIndex);
}

void Ut_MAppletObjectMenuActionSelectedMessage::testActionIndex()
{
    QCOMPARE(message->index(), (uint)100);
}

QTEST_APPLESS_MAIN(Ut_MAppletObjectMenuActionSelectedMessage)
