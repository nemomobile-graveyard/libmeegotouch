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

#include "ut_mappletobjectmenumessage.h"

#include <QtTest/QtTest>
#include <QAction>
#include "mappletobjectmenumessage.h"

void Ut_MAppletObjectMenuMessage::init()
{
    // Allocate a buffer and a stream for (un)serialization
    buffer = new QBuffer;
    buffer->open(QIODevice::ReadWrite);
    stream = new QDataStream(buffer);

    QList<QAction *> actions;
    QObject mockobject;
    QAction action1("action1", &mockobject);
    QAction action2("action2", &mockobject);
    QAction action3("action3", &mockobject);
    actions.append(&action1);
    actions.append(&action2);
    actions.append(&action3);

    message = new MAppletObjectMenuMessage(actions);
}

void Ut_MAppletObjectMenuMessage::cleanup()
{
    delete message;
    delete stream;
    delete buffer;
}

void Ut_MAppletObjectMenuMessage::testType()
{
    QCOMPARE(message->type(), MAppletMessage::ObjectMenuMessage);
}

void Ut_MAppletObjectMenuMessage::testSerialization()
{
    message->serialize(*stream);

    // Seek to the beginning of the buffer
    buffer->seek(0);

    QList<QString> actionNames;
    *stream >> actionNames;
    QCOMPARE(actionNames.at(0), QString("action1"));
    QCOMPARE(actionNames.at(1), QString("action2"));
    QCOMPARE(actionNames.at(2), QString("action3"));
}

void Ut_MAppletObjectMenuMessage::testUnserialization()
{
    QList<QString> actionNames;
    actionNames.append("action1");
    actionNames.append("action2");
    *stream << actionNames;

    buffer->seek(0);
    message->unserialize(*stream);

    QCOMPARE(message->actionList().size(), 2);
    QCOMPARE(message->actionList().at(0), actionNames.at(0));
    QCOMPARE(message->actionList().at(1), actionNames.at(1));
}

void Ut_MAppletObjectMenuMessage::testActionList()
{
    QCOMPARE(message->actionList().at(0), QString("action1"));
}

QTEST_APPLESS_MAIN(Ut_MAppletObjectMenuMessage)
