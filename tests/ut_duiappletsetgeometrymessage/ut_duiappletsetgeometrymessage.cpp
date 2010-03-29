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

#include "ut_duiappletsetgeometrymessage.h"

#include <QtTest/QtTest>
#include "duiappletsetgeometrymessage.h"

void Ut_DuiAppletSetGeometryMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new DuiAppletSetGeometryMessage();
}

void Ut_DuiAppletSetGeometryMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_DuiAppletSetGeometryMessage::testType()
{
    QCOMPARE(message->type(), DuiAppletMessage::SetGeometryMessage);
}

void Ut_DuiAppletSetGeometryMessage::testGeometryProperty()
{
    QCOMPARE(message->geometry(), QRectF());
    QRectF tmp(1.0, 2.0, 3.0, 4.0);
    message->setGeometry(tmp);
    QCOMPARE(message->geometry(), tmp);
}

void Ut_DuiAppletSetGeometryMessage::testHandleProperty()
{
#ifdef Q_WS_X11
    QCOMPARE(message->handle(), (Qt::HANDLE)0);
    Qt::HANDLE tmp = 52;
    message->setHandle(tmp);
    QCOMPARE(message->handle(), tmp);
#endif
}

void Ut_DuiAppletSetGeometryMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    QRectF geometry(1.0, 2.0, 3.0, 4.0);
    quint64 handle = 52;
    *stream >> geometry;
    *stream >> handle;

    QCOMPARE(geometry, QRectF());
    QCOMPARE(handle, (quint64)0);
}

void Ut_DuiAppletSetGeometryMessage::testUnserialization()
{
#ifdef Q_WS_X11
    QRectF geometry(1.0, 2.0, 3.0, 4.0);
    Qt::HANDLE handle = 52;
    *stream << geometry;
    *stream << (quint64)handle;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->geometry(), geometry);
    QCOMPARE(message->handle(), handle);
#endif
}

QTEST_APPLESS_MAIN(Ut_DuiAppletSetGeometryMessage)
