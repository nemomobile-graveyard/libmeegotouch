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

#ifndef UT_MAPPLETCOMMUNICATOR_H
#define UT_MAPPLETCOMMUNICATOR_H

#include <mappletcommunicator.h>

class TestAppletCommunicator : public MAppletCommunicator
{
    Q_OBJECT

public:
    TestAppletCommunicator();
    virtual ~TestAppletCommunicator();

    virtual void closeConnection();
    void setSocketState(QLocalSocket::LocalSocketState state);
    void setupConnection();
    QDataStream &getStream() const;

    QByteArray *testByteArray;

};

class Ut_MAppletCommunicator : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testIsConnected();
    void testMessageSendingWhenNotConnected();
    void testMessageSending();
    void testReceivingMessages();
    void testInvalidMessageTypeReceiving();
    void testReceivingMessageInMultipleParts();
    void testReceivingMessageWithSocketFailure();

signals:
    void signalReadyRead();

public slots:
    void messageReceived(const MAppletMessage &message);

public:
    static TestAppletCommunicator *m_subject;

    QList<uint> messageDatas;
};

#endif
