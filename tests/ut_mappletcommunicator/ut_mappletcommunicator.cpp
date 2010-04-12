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

#include "ut_mappletcommunicator.h"
#include <mappletcommunicator.h>
#include <mappletmessage.h>
#include <mappletmessagefactory.h>
#include <QDataStream>

#include <QtTest/QtTest>

// static variables
TestAppletCommunicator *Ut_MAppletCommunicator::m_subject;


// QDataStream stubs
QDataStream::Status gDataStreamStatus = QDataStream::Ok;
QDataStream::Status QDataStream::status() const
{
    return gDataStreamStatus;
}

// QLocalSocket stubs
QLocalSocket::LocalSocketState gLocalSocketState;
QLocalSocket::LocalSocketState QLocalSocket::state() const
{
    return gLocalSocketState;
}

bool gLocalSocketFlushed;
bool QLocalSocket::flush()
{
    gLocalSocketFlushed = true;
    return true;
}

class TestCommunicatorSocket : public QLocalSocket
{
public:
    virtual qint64 bytesAvailable() const {
        return Ut_MAppletCommunicator::m_subject->getStream().device()->bytesAvailable();
    }
};

// A simple applet message for testing
class TestAppletMessage : public MAppletMessage
{
public:
    // Some data in the message
    uint foo;

    TestAppletMessage(uint value = 2, MAppletMessageType type = (MAppletMessageType)1) :
        MAppletMessage(type),
        foo(value) {
    }

    void serialize(QDataStream &stream) const {
        stream << foo;
    }

    void unserialize(QDataStream &stream) {
        stream >> foo;
    }
};

void myMessageSerializer(QDataStream &stream, const MAppletMessage &message)
{
    // Serialize the message to a temporary stream
    QByteArray array;
    QDataStream tmpStream(&array, QIODevice::ReadWrite);
    message.serialize(tmpStream);

    // Type
    stream << (uint)message.type();
    // Size
    int size = array.size();
    stream << size;
    // Copy message data
    stream.writeRawData(array.data(), size);
}

// MAppletMessageFactory stubs
MAppletMessage *MAppletMessageFactory::create(MAppletMessage::MAppletMessageType messageType)
{
    switch ((uint)messageType) {
    case 1:
        return new TestAppletMessage;
    default:
        return NULL;
    }
}

// The test subject
TestAppletCommunicator::TestAppletCommunicator() :
    testByteArray(NULL)
{
}

TestAppletCommunicator::~TestAppletCommunicator()
{
    delete testByteArray;
}

void TestAppletCommunicator::closeConnection()
{
}

void TestAppletCommunicator::setSocketState(QLocalSocket::LocalSocketState state)
{
    if (socket == NULL) {
        socket = new TestCommunicatorSocket;
    }
    gLocalSocketState = state;
}

void TestAppletCommunicator::setupConnection()
{
    setSocketState(QLocalSocket::ConnectedState);

    if (stream != NULL) {
        delete stream;
    }
    if (testByteArray != NULL) {
        delete testByteArray;
    }
    testByteArray = new QByteArray;
    stream = new QDataStream(testByteArray, QIODevice::ReadWrite);
}

QDataStream &TestAppletCommunicator::getStream() const
{
    return *stream;
}

// The test class
void Ut_MAppletCommunicator::initTestCase()
{
}

void Ut_MAppletCommunicator::cleanupTestCase()
{
}

void Ut_MAppletCommunicator::init()
{
    gLocalSocketState = QLocalSocket::UnconnectedState;
    gLocalSocketFlushed = false;

    messageDatas.clear();

    m_subject = new TestAppletCommunicator;
    connect(this, SIGNAL(signalReadyRead()), m_subject, SLOT(readyRead()));
    connect(m_subject, SIGNAL(messageReceived(MAppletMessage)), this, SLOT(messageReceived(MAppletMessage)));
}

void Ut_MAppletCommunicator::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

void Ut_MAppletCommunicator::testIsConnected()
{
    // By default shouldn't be connected (there's no socket)
    QCOMPARE(m_subject->isConnected(), false);

    // Set the socket's state to something that tells that it's not connected (this also creates the socket)
    m_subject->setSocketState(QLocalSocket::UnconnectedState);
    QCOMPARE(m_subject->isConnected(), false);

    // Set the socket's state to connected, which ultimately indicates, that there is a connection
    m_subject->setSocketState(QLocalSocket::ConnectedState);
    QCOMPARE(m_subject->isConnected(), true);
}

void Ut_MAppletCommunicator::testMessageSendingWhenNotConnected()
{
    // Try to send a message
    TestAppletMessage message;
    bool result = m_subject->sendMessage(message);
    QCOMPARE(result, false);
}

void Ut_MAppletCommunicator::testMessageSending()
{
    m_subject->setupConnection();

    // Send a message
    TestAppletMessage message;
    bool result = m_subject->sendMessage(message);

    // Verify that sending was successful
    QCOMPARE(result, true);

    //                 msg type       msg size      msg payload
    int expectedSize = sizeof(uint) + sizeof(int) + sizeof(uint);
    QCOMPARE(m_subject->testByteArray->size(), expectedSize);

    uint value = 0;
    m_subject->getStream().device()->seek(0);

    // First there should be the type
    m_subject->getStream() >> value;
    QCOMPARE(value, (uint)1);

    // Then there should be the size
    m_subject->getStream() >> value;
    QCOMPARE(value, (uint)sizeof(uint));

    // Then there should be the data
    m_subject->getStream() >> value;
    QCOMPARE(value, (uint)2);

    // The socket should have been flushed
    QCOMPARE(gLocalSocketFlushed, true);
}

void Ut_MAppletCommunicator::testReceivingMessages()
{
    m_subject->setupConnection();

    // Put test data for two messages to the stream
    myMessageSerializer(m_subject->getStream(), TestAppletMessage(3));
    myMessageSerializer(m_subject->getStream(), TestAppletMessage(4));
    m_subject->getStream().device()->seek(0);

    emit signalReadyRead();

    QCOMPARE(messageDatas.count(), 2);
    QCOMPARE(messageDatas.at(0), (uint)3);
    QCOMPARE(messageDatas.at(1), (uint)4);
}

void Ut_MAppletCommunicator::testReceivingMessageWithSocketFailure()
{
    m_subject->setupConnection();

    // Put test data for two messages to the stream
    myMessageSerializer(m_subject->getStream(), TestAppletMessage(3));
    myMessageSerializer(m_subject->getStream(), TestAppletMessage(4));
    m_subject->getStream().device()->seek(0);
    QSignalSpy connectionSpy(m_subject, SIGNAL(connectionLost()));
    // make socket fail
    gDataStreamStatus = static_cast<QDataStream::Status>(-1);

    // socket fails, no data, connectionLost signal received
    emit signalReadyRead();
    QCOMPARE(connectionSpy.count(), 1);
    QCOMPARE(messageDatas.count(), 0);

    // make socket succeed
    gDataStreamStatus = QDataStream::Ok;

    // Test that we can read valid data after socket error
    m_subject->getStream().device()->seek(0);
    myMessageSerializer(m_subject->getStream(), TestAppletMessage(3));
    myMessageSerializer(m_subject->getStream(), TestAppletMessage(4));
    m_subject->getStream().device()->seek(0);
    emit signalReadyRead();
    QCOMPARE(connectionSpy.count(), 1); // no new connectionLost signals
    QCOMPARE(messageDatas.count(), 2);
    QCOMPARE(messageDatas.at(0), (uint)3);
    QCOMPARE(messageDatas.at(1), (uint)4);
}

void Ut_MAppletCommunicator::testInvalidMessageTypeReceiving()
{
    m_subject->setupConnection();

    // Put test data with unknown message type to the stream
    myMessageSerializer(m_subject->getStream(), TestAppletMessage(2, (MAppletMessage::MAppletMessageType)10));
    m_subject->getStream().device()->seek(0);

    emit signalReadyRead();

    QCOMPARE(messageDatas.count(), 0);
}

void Ut_MAppletCommunicator::testReceivingMessageInMultipleParts()
{
    m_subject->setupConnection();

    // Serialize a message to a temporary stream
    QByteArray byteArray;
    QDataStream tmpStream(&byteArray, QIODevice::ReadWrite);
    myMessageSerializer(tmpStream, TestAppletMessage());
    tmpStream.device()->seek(0);

    QIODevice *device = m_subject->getStream().device();
    // Put the data to the communicator's stream one byte at a time
    for (int i = 0; i < byteArray.size(); ++i) {
        uchar tmpValue;
        tmpStream >> tmpValue;
        qint64 oldPos = device->pos();
        device->seek(device->size());
        m_subject->getStream() << tmpValue;
        device->seek(oldPos);

        emit signalReadyRead();

        if (i < byteArray.size() - 1) {
            // No message signals should be emitted before the whole message is transfered
            QCOMPARE(messageDatas.count(), 0);
        } else {
            // Now the whole message is transfered, so a signal should be emitted
            QCOMPARE(messageDatas.count(), 1);
            QCOMPARE(messageDatas.at(0), (uint)2);
        }
    }
}

void Ut_MAppletCommunicator::messageReceived(const MAppletMessage &message)
{
    if (message.type() == 1) {
        messageDatas.append(dynamic_cast<const TestAppletMessage &>(message).foo);
    }
}

QTEST_APPLESS_MAIN(Ut_MAppletCommunicator)
