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

#include "ut_duiappletorientationmessage.h"

#include <QtTest/QtTest>
#include "duiappletorientationmessage.h"

void Ut_DuiAppletOrientationMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new DuiAppletOrientationMessage();
}

void Ut_DuiAppletOrientationMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_DuiAppletOrientationMessage::testType()
{
    QCOMPARE(message->type(), DuiAppletMessage::OrientationMessage);
}

void Ut_DuiAppletOrientationMessage::testOrientationProperty()
{
    QCOMPARE(message->orientation(), Dui::Landscape);
    message->setOrientation(Dui::Portrait);
    QCOMPARE(message->orientation(), Dui::Portrait);
}

void Ut_DuiAppletOrientationMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    uchar tmp = (uchar)Dui::Portrait;
    *stream >> tmp;
    Dui::Orientation result = (Dui::Orientation)tmp;

    QCOMPARE(result, Dui::Landscape);
}

void Ut_DuiAppletOrientationMessage::testUnserialization()
{
    *stream << (uchar)Dui::Portrait;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->orientation(), Dui::Portrait);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletOrientationMessage)
