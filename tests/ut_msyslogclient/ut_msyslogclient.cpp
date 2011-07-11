/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QObject>
#include <MDebug>
#include <QLocalSocket>
#include <QSignalSpy>
#include <QLocalServer>
#include <MApplication>

#include "ut_msyslogclient.h"
#include "msyslogclient.h"

void Ut_MSyslogClient::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_msyslogclient" };
    app = new MApplication(argc, app_name);

    socket = new MSyslogClientSocket();
}

void Ut_MSyslogClient::cleanupTestCase()
{
    delete app;
    delete socket;
}

void Ut_MSyslogClient::testInvalidServer()
{
    bool result;

    // empty url
    result = socket->connectToServer(QUrl());
    QVERIFY(result == false);

    // invalid url
    result = socket->connectToServer(QUrl("invalidurl"));
    QVERIFY(result == false);
}

void Ut_MSyslogClient::testSendLocalMessages()
{
    QString socketFileName;
    const char *message = "test message";
    QString received;
    QString expected;
    bool result;

    // create custom log server
    MyLocalServer server;
    QSignalSpy spy(&server, SIGNAL(newConnection()));
    socketFileName = server.fullServerName();

    // connect to server
    result = socket->connectToServer(QUrl::fromLocalFile(socketFileName));
    QVERIFY(result == true);

    // send Fatal message
    socket->sendMsg(QtFatalMsg, message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 2)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // send Critical message
    socket->sendMsg(QtCriticalMsg, message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 3)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // send Warning  message
    socket->sendMsg(QtWarningMsg, message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 4)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // send Debug message
    socket->sendMsg(QtDebugMsg, message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 6)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // send uncategorized debug message
    socket->sendMsg(message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 6)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // close client socket
    socket->close();
}

void Ut_MSyslogClient::testSendUdpMessages()
{
    QString socketFileName;
    const char *message = "test message";
    QString received;
    QString expected;
    bool result;

    // create custom log server
    MyUdpServer server;

    // connect to server
    result = socket->connectToServer(QUrl("udp://127.0.0.1:7755"));
    QVERIFY(result == true);

    // send Fatal message
    socket->sendMsg(QtFatalMsg, message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 2)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // send Critical message
    socket->sendMsg(QtCriticalMsg, message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 3)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // send Warning  message
    socket->sendMsg(QtWarningMsg, message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 4)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // send Debug message
    socket->sendMsg(QtDebugMsg, message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 6)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // send uncategorized debug message
    socket->sendMsg(message);
    QTest::qWait(200);
    expected = QString("<%1>%2: %3")
        .arg((1 << 3) + 6)
        .arg(socket->syslogId())
        .arg(message);
    received = server.readLine();
    QCOMPARE(received, expected);

    // close client socket
    socket->close();
}


QTEST_APPLESS_MAIN(Ut_MSyslogClient)
