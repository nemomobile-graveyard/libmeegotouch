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

#include "qlocalsocket_mock.h"

int QLocalSocket::failConnectCount = 0;
QLocalSocket *QLocalSocket::self = 0;

QLocalSocket::QLocalSocket(QObject *parent) : QIODevice(parent)
{
    socketState = UnconnectedState;
    attemptedConnectCount = 0;
    self = this;
}

QLocalSocket::~QLocalSocket()
{
}

qint64 QLocalSocket::bytesAvailable() const
{
    return QIODevice::bytesAvailable();
}

void QLocalSocket::connectToServer(const QString &name, OpenMode openMode)
{
    Q_UNUSED(name);
    Q_UNUSED(openMode);

    attemptedConnectCount++;

    if (failConnectCount > 0) {
        failConnectCount--;
        emit error(ServerNotFoundError);
    } else {
        this->open(QIODevice::ReadWrite);
        socketState = ConnectedState;
        emit connected();
    }
}

void QLocalSocket::suddenDisconnect()
{
    socketState = UnconnectedState;
    bytesWritten.clear();
    this->close();
    attemptedConnectCount = 0;
    emit error(UnknownSocketError);
}

void QLocalSocket::clearReceivedData()
{
    bytesWritten.clear();
}

void QLocalSocket::disconnectFromServer()
{
}

bool QLocalSocket::flush()
{
    return 0;
}

QLocalSocket::LocalSocketState QLocalSocket::state() const
{
    return socketState;
}

qint64 QLocalSocket::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);

    return 0;
}

qint64 QLocalSocket::writeData(const char *data, qint64 maxSize)
{
    bytesWritten.append(data, maxSize);

    return 0;
}

QByteArray *QLocalSocket::getWrittenData()
{
    return &bytesWritten;
}

int QLocalSocket::getAttemptedConnectCount()
{
    return attemptedConnectCount;
}

void QLocalSocket::setFailConnectCount(int count)
{
    failConnectCount = count;
}


QLocalSocket *QLocalSocket::instance()
{
    return self;
}


