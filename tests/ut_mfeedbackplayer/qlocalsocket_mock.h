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

#ifndef QLOCALSOCKET_H
#define QLOCALSOCKET_H

#include <QtCore/qiodevice.h>
#include <QtNetwork/qabstractsocket.h>
#include <QBuffer>

class QLocalSocket : public QIODevice
{
    Q_OBJECT

public:
    enum LocalSocketError {
        ConnectionRefusedError = QAbstractSocket::ConnectionRefusedError,
        PeerClosedError = QAbstractSocket::RemoteHostClosedError,
        ServerNotFoundError = QAbstractSocket::HostNotFoundError,
        SocketAccessError = QAbstractSocket::SocketAccessError,
        SocketResourceError = QAbstractSocket::SocketResourceError,
        SocketTimeoutError = QAbstractSocket::SocketTimeoutError,
        DatagramTooLargeError = QAbstractSocket::DatagramTooLargeError,
        ConnectionError = QAbstractSocket::NetworkError,
        UnsupportedSocketOperationError = QAbstractSocket::UnsupportedSocketOperationError,
        UnknownSocketError = QAbstractSocket::UnknownSocketError
    };

    enum LocalSocketState {
        UnconnectedState = QAbstractSocket::UnconnectedState,
        ConnectingState = QAbstractSocket::ConnectingState,
        ConnectedState = QAbstractSocket::ConnectedState,
        ClosingState = QAbstractSocket::ClosingState
    };

    QLocalSocket(QObject *parent = 0);
    ~QLocalSocket();

    void connectToServer(const QString &name, OpenMode openMode = ReadWrite);
    void disconnectFromServer();

    bool flush();
    LocalSocketState state() const;
    qint64 bytesAvailable() const;

    static void setFailConnectCount(int count);
    QByteArray *getWrittenData();
    int getAttemptedConnectCount();
    void suddenDisconnect();
    void clearReceivedData();

    // Interface to get instance of created QLocalSocket
    static QLocalSocket *instance();

Q_SIGNALS:
    void connected();
    void disconnected();
    void error(QLocalSocket::LocalSocketError socketError);
    void stateChanged(QLocalSocket::LocalSocketState socketState);

protected:
    virtual qint64 readData(char *data, qint64 maxSize);
    virtual qint64 writeData(const char *data, qint64 maxSize);

    LocalSocketState socketState;

    // How many times should connecting fail before succeeding
    static int failConnectCount;
    int attemptedConnectCount;

    // Buffer for received data from client
    QByteArray bytesWritten;

    // Created instances of created QLocalSockets
    static QLocalSocket *self;

private:
    Q_DISABLE_COPY(QLocalSocket)
};

#endif // QLOCALSOCKET_H

