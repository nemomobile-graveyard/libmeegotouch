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

#include "ut_duiappletpixmaptakenintousemessage.h"

#include <QtTest/QtTest>
#include "duiappletpixmaptakenintousemessage.h"

void Ut_DuiAppletPixmapTakenIntoUseMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new DuiAppletPixmapTakenIntoUseMessage();
}

void Ut_DuiAppletPixmapTakenIntoUseMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_DuiAppletPixmapTakenIntoUseMessage::testType()
{
    QCOMPARE(message->type(), DuiAppletMessage::PIXMAP_TAKEN_INTO_USE_MESSAGE);
}

void Ut_DuiAppletPixmapTakenIntoUseMessage::testHandleProperty()
{
    QCOMPARE(message->handle(), (Qt::HANDLE)0);
    Qt::HANDLE tmp = 52;
    message->setHandle(tmp);
    QCOMPARE(message->handle(), tmp);
}

void Ut_DuiAppletPixmapTakenIntoUseMessage::testSerialization()
{
    message->serialize(*stream);

    buffer->seek(0);
    quint64 handle = 52;
    *stream >> handle;

    QCOMPARE(handle, (quint64)0);
}

void Ut_DuiAppletPixmapTakenIntoUseMessage::testUnserialization()
{
    Qt::HANDLE handle = 52;
    *stream << (quint64)handle;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->handle(), handle);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletPixmapTakenIntoUseMessage)
