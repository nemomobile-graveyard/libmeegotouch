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

#include "ut_mappletpixmaptakenintousemessage.h"

#include <QtTest/QtTest>
#include "mappletpixmaptakenintousemessage.h"

void Ut_MAppletPixmapTakenIntoUseMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new MAppletPixmapTakenIntoUseMessage();
}

void Ut_MAppletPixmapTakenIntoUseMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_MAppletPixmapTakenIntoUseMessage::testType()
{
    QCOMPARE(message->type(), MAppletMessage::PixmapTakenIntoUseMessage);
}

void Ut_MAppletPixmapTakenIntoUseMessage::testHandleProperty()
{
    QCOMPARE(message->handle(), (Qt::HANDLE)0);
    Qt::HANDLE tmp = 52;
    message->setHandle(tmp);
    QCOMPARE(message->handle(), tmp);
}

void Ut_MAppletPixmapTakenIntoUseMessage::testSerialization()
{
    message->serialize(*stream);

    buffer->seek(0);
    quint64 handle = 52;
    *stream >> handle;

    QCOMPARE(handle, (quint64)0);
}

void Ut_MAppletPixmapTakenIntoUseMessage::testUnserialization()
{
    Qt::HANDLE handle = 52;
    *stream << (quint64)handle;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->handle(), handle);
}

QTEST_APPLESS_MAIN(Ut_MAppletPixmapTakenIntoUseMessage)
