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

#include "ut_mappletserver.h"
#include "mappletserver.h"
#include "mappletmessage.h"
#include "mappletmessagefactory.h"

#include <QtTest/QtTest>
#include <QLocalSocket>

void Ut_MAppletServer::init()
{
    connected = false;

    m_subject = new MAppletServer;
    connect(m_subject, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));
}

void Ut_MAppletServer::cleanup()
{
    delete socket;
    socket = NULL;
    delete m_subject;
    m_subject = NULL;
}

void Ut_MAppletServer::setupServer()
{
    // Initiate connection to a process
    QCOMPARE(m_subject->startServer("moopwidget_1"), true);

    // Emulate being the process to connect to
    socket = new QLocalSocket;
    socket->connectToServer("moopwidget_1");

    // Process events max. 100 times to enable Qt signal handling
    for (int retries = 0; retries < 100 && !connected; retries++) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000000);
    }
}

void Ut_MAppletServer::testConnection()
{
    setupServer();

    // Verify whether connection has been established or not
    QCOMPARE(connected, true);
}

void Ut_MAppletServer::testReconnect()
{
    setupServer();
    QCOMPARE(socket->state(), QLocalSocket::ConnectedState);
    QCOMPARE(m_subject->isConnected(), true);

    m_subject->closeConnection();
    QCOMPARE(m_subject->isConnected(), false);

    // Test that new connections can't be made
    delete socket;
    socket = new QLocalSocket;
    socket->connectToServer("moopwidget_1");
    QCOMPARE(socket->waitForConnected(), false);
    QCOMPARE(socket->error(), QLocalSocket::ServerNotFoundError);
    QCOMPARE(m_subject->isConnected(), false);

    // Test that restarting the server works
    delete socket;
    connected = false;
    setupServer();
    QCOMPARE(connected, true);
    QCOMPARE(socket->state(), QLocalSocket::ConnectedState);
    QCOMPARE(m_subject->isConnected(), true);
}

void Ut_MAppletServer::connectionEstablished()
{
    connected = true;
}

QTEST_MAIN(Ut_MAppletServer)
