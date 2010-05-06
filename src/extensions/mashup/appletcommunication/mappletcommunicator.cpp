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

#include <MDebug>
#include <QLocalServer>
#include <QLocalSocket>
#include <QProcess>
#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QTime>

#include "mappletcommunicator.h"
#include "mappletmessage.h"
#include "mappletmessagefactory.h"

MAppletCommunicator::MAppletCommunicator() :
    socket(NULL),
    stream(NULL),
    readState(NEEDS_TYPE),
    receivedMessageType(MAppletMessage::InvalidMessage),
    pendingMessagePayloadSize(-1),
    messagePayloadReceivingStream(&receivingBuffer)
{
    messagePayloadReceivingStream.device()->open(QIODevice::ReadWrite);
}

MAppletCommunicator::~MAppletCommunicator()
{
}

bool MAppletCommunicator::sendMessage(const MAppletMessage &message)
{
    if (!isConnected()) {
        return false;
    }

    // Serialize the message to a temporary stream so that the size of the payload can be calculated
    QByteArray tmpArray;
    QDataStream tmpStream(&tmpArray, QIODevice::WriteOnly);
    message.serialize(tmpStream);

    // Write the message type to the stream
    *stream << (uint)message.type();

    // Write the message payload size to the stream
    *stream << tmpArray.size();

    // Write the message payload to the stream
    stream->writeRawData(tmpArray.data(), tmpArray.size());

    // Flush the socket and calculate return value
    bool result = (stream->status() == QDataStream::Ok) && socket->flush();
    if (!result) {
        emit connectionLost();
    }
    return result;
}

bool MAppletCommunicator::isConnected() const
{
    // Check that the socket exists (the communicator is connected)
    if (socket == NULL || socket->state() != QLocalSocket::ConnectedState) {
        return false;
    }
    return true;
}

void MAppletCommunicator::socketError(QLocalSocket::LocalSocketError error)
{
    mDebug("MAppletCommunicator") << Q_FUNC_INFO << error
                                      << socket->errorString();
    readState = NEEDS_TYPE;
    emit connectionLost();
}

void MAppletCommunicator::readyRead()
{
    bool canProceed = true;
    while (canProceed) {
        if (readState == NEEDS_TYPE) {
            canProceed = readMessageType();
        }

        if (readState == NEEDS_SIZE) {
            canProceed = readMessageSize();
        }

        if (readState == NEEDS_DATA) {
            canProceed = readMessageData();
        }

        if (readState == MESSAGE_RECEIVED) {
            // Create a message from the type
            QScopedPointer<MAppletMessage> message(MAppletMessageFactory::create(receivedMessageType));

            // Make sure the message exists (it won't if type is unknown)
            if (!message.isNull()) {
                // If the message had payload, unserialize it
                if (messagePayloadReceivingStream.device()->size() > 0) {
                    messagePayloadReceivingStream.device()->seek(0);
                    message->unserialize(messagePayloadReceivingStream);

                    // Clear the previous data buffer
                    messagePayloadReceivingStream.device()->seek(0);
                    receivingBuffer.buffer().clear();
                }

                emit messageReceived(*message);
            }

            readState = NEEDS_TYPE;
        }
    }
}

bool MAppletCommunicator::readMessageType()
{
    if (checkSocketError()) {
        return false;
    }
    if (socket->bytesAvailable() >= (int)sizeof(uint)) {
        uint tmp;
        *stream >> tmp;
        if (checkSocketError()) {
            return false;
        }
        receivedMessageType = static_cast<MAppletMessage::MAppletMessageType>(tmp);
        readState = NEEDS_SIZE;
        return true;
    }
    return false;
}

bool MAppletCommunicator::readMessageSize()
{
    pendingMessagePayloadSize = -1;
    if (checkSocketError()) {
        return false;
    }
    if (socket->bytesAvailable() >= (int)sizeof(int)) {
        *stream >> pendingMessagePayloadSize;
        if (checkSocketError()) {
            return false;
        }
        if (pendingMessagePayloadSize > 0) {
            readState = NEEDS_DATA;

            static_cast<QBuffer *>(messagePayloadReceivingStream.device())->buffer().reserve(pendingMessagePayloadSize);
        } else {
            readState = MESSAGE_RECEIVED;
        }
        return true;
    }
    return false;
}

bool MAppletCommunicator::readMessageData()
{
    if (pendingMessagePayloadSize <= 0 && socket->bytesAvailable() <= 0) {
        return false;
    }
    QByteArray byteArray;
    byteArray.reserve(pendingMessagePayloadSize);
    while (pendingMessagePayloadSize > 0
            && socket->bytesAvailable() > 0) {
        int bytesRead = stream->readRawData(byteArray.data(), pendingMessagePayloadSize);
        if (checkSocketError(bytesRead)) {
            return false;
        }
        messagePayloadReceivingStream.writeRawData(byteArray.data(), bytesRead);
        pendingMessagePayloadSize -= bytesRead;
    }
    if (checkSocketError()) {
        return false;
    }
    if (pendingMessagePayloadSize == 0) {
        readState = MESSAGE_RECEIVED;
        return true;
    }
    return false;
}

bool MAppletCommunicator::checkSocketError(qint64 bytesRead)
{
    if ((bytesRead < 0)
            || (socket->bytesAvailable() < 0)
            || (stream->status() != QDataStream::Ok)) {
        readState = NEEDS_TYPE;
        emit connectionLost();
        return true;
    }
    return false;
}
