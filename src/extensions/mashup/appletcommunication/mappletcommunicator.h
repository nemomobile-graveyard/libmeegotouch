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

#ifndef MAPPLETCOMMUNICATOR_H_
#define MAPPLETCOMMUNICATOR_H_

#include "mexport.h"
#include "mappletmessage.h"

#include <QLocalSocket>
#include <QBuffer>
class QDataStream;

/*!
 * The MAppletCommunicator is a base class for implementing interprocess
 * communication between two processes (a host application and an applet).
 */
class M_EXTENSIONS_EXPORT MAppletCommunicator : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs an MAppletCommunicator.
     */
    MAppletCommunicator();

    /*!
     * Destroys the MAppletCommunicator.
     */
    virtual ~MAppletCommunicator();

    /*!
     * Closes the connection.
     */
    virtual void closeConnection() = 0;

    /*!
     * Sends a MAppletMessage.
     *
     * \param message the MAppletMessage to send
     * \return the result of the operation: \c true if sending was successful and \c false otherwise.
     */
    bool sendMessage(const MAppletMessage &message);

    /*!
     * Queries if this communicator is currently connected.
     * \return \c true if connected, \c false if not.
     */
    bool isConnected() const;

protected:
    //! The socket for IPC communication, created by subclasses
    QLocalSocket *socket;

    //! Stream for reading and writing from the socket, created by subclasses
    QDataStream *stream;

private slots:
    /*!
     * \brief A slot that gets called if the client side socket encounters an error.
     * \param error the error type.
     */
    void socketError(QLocalSocket::LocalSocketError error);

    /*!
     * \brief A slot that is called when there is new data available in the connection socket.
     */
    void readyRead();

signals:
    /*!
     * \brief A signal that is emitted when this communicator has successfully established a connection.
     */
    void connectionEstablished();

    /*!
     * A signal that is emitted when there's an error in communication.
     * That usually means the reading of messages fails because the
     * other end has crashed or otherwise closed the communication channel.
     */
    void connectionLost();

    /*!
     * \brief A signal that is emitted when this communicator has received a new message.
     * \param message the message that was received
     */
    void messageReceived(const MAppletMessage &message);

private:
    /*!
     * States for reading the data from the socket.
     */
    enum ReadState {
        NEEDS_TYPE,    //!< The message type is expected to come next
        NEEDS_SIZE,    //!< The message size is expected to come next
        NEEDS_DATA,    //!< The message data is expected to come next or hasn't yet arrived completely
        MESSAGE_RECEIVED //!< The message data has been received completely. Next the message should be constructed from the data and emitted
    };

    //! The reading state
    ReadState readState;

    //! The type of the received message
    MAppletMessage::MAppletMessageType receivedMessageType;

    //! The size of the payload that is still expected to be received for a message
    int pendingMessagePayloadSize;

    //! The buffer used for the stream that receives the incoming message
    // data
    QBuffer receivingBuffer;

    //! A data stream for collecting the incoming message data
    QDataStream messagePayloadReceivingStream;

    /*!
     * Tries to read the message type from the socket. This method should be
     * called when the state is \c NEEDS_TYPE. If there is enough data available
     * in the socket and the type can be read, the state is set to \c NEEDS_SIZE
     * and \c true is returned.
     * \return \c true if the type could be read, \c false if not.
     */
    bool readMessageType();

    /*!
     * Tries to read the message payload size from the socket. This method should be
     * called when the state is \c NEEDS_SIZE. If there is enough data available
     * in the socket and the payload size can be read, the state is set to \c NEEDS_DATA
     * and \c true is returned.
     * \return \c true if the payload size could be read, \c false if not.
     */
    bool readMessageSize();

    /*!
     * Reads any data that is available in the socket, but at most \c pendingMessagePayloadSize
     * bytes. This method should be called when the state is \c NEEDS_DATA. If \c pendingMessagePayloadSize
     * bytes could be read this method sets the state to \c MESSAGE_RECEIVED and returns \c true.
     * Otherwise this method returns \c false.
     * \return \c true if no more message data is pendind, \c false otherwise.
     */
    bool readMessageData();

    /*!
     * Checks for a socket error. Resets the state of the communicator
     * if an error has occurred.
     * \param bytesRead Optional count of read bytes.
     * \return \c true if the socket or the stream are in an error state.
     */
    bool checkSocketError(qint64 bytesRead = 0);
};

#endif /* MAPPLETCOMMUNICATOR_H_ */
