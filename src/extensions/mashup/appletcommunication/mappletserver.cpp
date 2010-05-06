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

#include <QLocalServer>
#include <QDataStream>
#include <MDebug>
#include "mappletserver.h"

MAppletServer::MAppletServer() :
    server(NULL)
{
}

MAppletServer::~MAppletServer()
{
    closeConnection();
}

bool MAppletServer::startServer(const QString &serverName)
{
    closeConnection();

    // Remove the socket file if it exists
    QLocalServer::removeServer(serverName);

    // Create a server and start listening for connections
    server = new QLocalServer;
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if (!server->listen(serverName)) {
        mWarning("MAppletServer") << "Could not init server." << server->errorString();
        return false;
    }

    return true;
}


void MAppletServer::closeConnection()
{
    if (server) {
        server->close();
        delete server;
        server = NULL;
        delete stream;
        stream = NULL;
        // socket is deleted by qlocalserver's destructor
        socket = NULL;
    }
}

void MAppletServer::newConnection()
{
    // Disconnect the new connection signal since only one connection is supported
    disconnect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    // Get the socket for the connection
    socket = server->nextPendingConnection();

    if (socket != NULL) {
        // Connect the readyRead() slot for the receiving data from the server
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

        // Create a stream for the socket data
        stream = new QDataStream(socket);

        emit connectionEstablished();
    }
}
