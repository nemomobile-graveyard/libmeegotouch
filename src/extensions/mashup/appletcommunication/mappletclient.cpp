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

#include "mappletclient.h"
#include <MDebug>


static const int MAX_CONNECTION_ATTEMPTS = 100;

MAppletClient::MAppletClient()
{
}

MAppletClient::~MAppletClient()
{
    closeConnection();
}

bool MAppletClient::connectToServer(const QString &serverName)
{
    closeConnection();

    // Connect to the specified server
    socket = new QLocalSocket;
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(socketError(QLocalSocket::LocalSocketError)));

    socket->connectToServer(serverName);

    int attempt = 1;
    while (socket->state() != QLocalSocket::ConnectedState) {
        if (attempt >= MAX_CONNECTION_ATTEMPTS) break;
#ifndef Q_OS_WIN
        usleep(100000);
#else
#include <windows.h>
        Sleep(100);
#endif
        ++attempt;
        socket->connectToServer(serverName);
    }

    if (attempt < MAX_CONNECTION_ATTEMPTS) {
        mDebug("MAppletClient") << "Connection took" << attempt << "attempts";
        return true;
    } else {
        mDebug("MAppletClient") << "Couldn't establish connection to server";
        return false;
    }
}

void MAppletClient::closeConnection()
{
    if (socket) {
        socket->disconnectFromServer();
        delete socket;
        socket = NULL;
        delete stream;
        stream = NULL;
    }
}

void MAppletClient::connected()
{
    // Don't accept any additional connections
    disconnect(socket, SIGNAL(connected()), this, SLOT(connected()));

    // Create a stream for the socket data
    stream = new QDataStream(socket);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    emit connectionEstablished();
}

void MAppletClient::socketError(QLocalSocket::LocalSocketError error)
{
    mDebug("MAppletClient") << Q_FUNC_INFO << error << socket->errorString();
}
