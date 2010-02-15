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

#include <QtTest/QtTest>
#include <QLocalServer>
#include <QLocalSocket>

#include "duiappletserver.h"
#include "duiappletclient.h"
#include "duiappletmessage.h"
#include "duiappletmessagefactory.h"
#include "ut_duiappletclient.h"


RemoteProcessThread::RemoteProcessThread() :
    socket(NULL),
    server(NULL),
    doWait(false)
{
}

RemoteProcessThread::~RemoteProcessThread()
{
    doWait = false;
    wait();
    delete server;
}

void RemoteProcessThread::waitHere()
{
    while (doWait) {
        QThread::msleep(100);
    }
}

void RemoteProcessThread::run()
{
    QLocalServer::removeServer("foobar");
    server = new QLocalServer();
    server->listen("foobar");

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    doWait = true;
    waitHere();
}

void RemoteProcessThread::readyRead()
{
}

void RemoteProcessThread::newConnection()
{
    socket = server->nextPendingConnection();
}

void Ut_DuiAppletClient::init()
{
    connected = false;

    process = new QProcess;
    client = new DuiAppletClient;
    connect(client, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));
}

void Ut_DuiAppletClient::cleanup()
{
    delete client;
    client = NULL;
    delete process;
    process = NULL;
}

void Ut_DuiAppletClient::testConnection()
{
    RemoteProcessThread thread;
    thread.start();
    QCOMPARE(client->connectToServer("foobar"), true);
    thread.doWait = false;

    // Verify whether connection has been established or not
    QCOMPARE(connected, true);
}

void Ut_DuiAppletClient::testReconnect()
{
    RemoteProcessThread thread;
    thread.start();
    client->connectToServer("foobar");

    QCOMPARE(client->isConnected(), true);
    QCOMPARE(connected, true);

    // Close the connection
    client->closeConnection();
    QCOMPARE(client->isConnected(), false);

    connected = false;
    // Try to connect again
    client->connectToServer("foobar");
    QCOMPARE(client->isConnected(), true);
    QCOMPARE(connected, true);

    thread.doWait = false;
}

void Ut_DuiAppletClient::connectionEstablished()
{
    connected = true;
}

QTEST_MAIN(Ut_DuiAppletClient)
