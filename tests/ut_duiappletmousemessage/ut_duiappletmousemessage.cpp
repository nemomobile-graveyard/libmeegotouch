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

#include "ut_duiappletmousemessage.h"

#include <QtTest/QtTest>
#include "duiappletmousemessage.h"

void Ut_DuiAppletMouseMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    message = new DuiAppletMouseMessage(DuiAppletMessage::MOUSE_PRESS_MESSAGE);
}

void Ut_DuiAppletMouseMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_DuiAppletMouseMessage::testTypes()
{
    QCOMPARE(message->type(), DuiAppletMessage::MOUSE_PRESS_MESSAGE);
    delete message;

    message = new DuiAppletMouseMessage(DuiAppletMessage::MOUSE_RELEASE_MESSAGE);
    QCOMPARE(message->type(), DuiAppletMessage::MOUSE_RELEASE_MESSAGE);
    delete message;

    message = new DuiAppletMouseMessage(DuiAppletMessage::MOUSE_MOVE_MESSAGE);
    QCOMPARE(message->type(), DuiAppletMessage::MOUSE_MOVE_MESSAGE);
}

void Ut_DuiAppletMouseMessage::testPositionProperty()
{
    QCOMPARE(message->position(), QPointF(0.0, 0.0));
    message->setPosition(QPointF(1.0, 2.0));
    QCOMPARE(message->position(), QPointF(1.0, 2.0));

    delete message;
    message = new DuiAppletMouseMessage(DuiAppletMessage::MOUSE_PRESS_MESSAGE, QPointF(1.0, 2.0));
    QCOMPARE(message->position(), QPointF(1.0, 2.0));
}

void Ut_DuiAppletMouseMessage::testButtonProperty()
{
    QCOMPARE(message->button(), Qt::NoButton);
    message->setButton(Qt::RightButton);
    QCOMPARE(message->button(), Qt::RightButton);

    delete message;
    message = new DuiAppletMouseMessage(DuiAppletMessage::MOUSE_PRESS_MESSAGE, QPointF(1.0, 2.0), Qt::MidButton);
    QCOMPARE(message->button(), Qt::MidButton);
}

void Ut_DuiAppletMouseMessage::testButtonsProperty()
{
    QCOMPARE(message->buttons(), Qt::NoButton);
    message->setButtons(Qt::LeftButton | Qt::RightButton);
    QCOMPARE(message->buttons(), Qt::LeftButton | Qt::RightButton);

    delete message;
    message = new DuiAppletMouseMessage(DuiAppletMessage::MOUSE_PRESS_MESSAGE, QPointF(1.0, 2.0), Qt::MidButton, Qt::LeftButton | Qt::MidButton);
    QCOMPARE(message->buttons(), Qt::LeftButton | Qt::MidButton);
}

void Ut_DuiAppletMouseMessage::testSerialization()
{
    QPointF pos(-1.0, -2.0);
    Qt::MouseButton button(Qt::MidButton);
    Qt::MouseButtons buttons(Qt::LeftButton | Qt::MidButton);
    message->setPosition(pos);
    message->setButton(button);
    message->setButtons(buttons);
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);
    QPointF resPos;
    quint32 tmp;
    *stream >> resPos;
    *stream >> tmp;
    Qt::MouseButton resButton = (Qt::MouseButton)tmp;
    *stream >> tmp;
    Qt::MouseButtons resButtons = (Qt::MouseButtons)tmp;

    QCOMPARE(resPos, pos);
    QCOMPARE(resButton, button);
    QCOMPARE(resButtons, buttons);
}

void Ut_DuiAppletMouseMessage::testUnserialization()
{
    QPointF pos(-1.0, -2.0);
    Qt::MouseButton button(Qt::RightButton);
    Qt::MouseButtons buttons(Qt::LeftButton | Qt::RightButton);
    *stream << pos;
    *stream << button;
    *stream << buttons;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->position(), pos);
    QCOMPARE(message->button(), button);
    QCOMPARE(message->buttons(), buttons);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletMouseMessage)
