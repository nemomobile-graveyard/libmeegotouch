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

#ifndef UT_MSYSLOGCLIENT_H
#define UT_MSYSLOGCLIENT_H

#include <QtTest/QtTest>
#include <QObject>

#include <QLocalServer>
#include <QLocalSocket>
#include <QUdpSocket>
#include <QFile>

class MSyslogClientSocket;
class MApplication;

class MyLocalServer : public QLocalServer
{
    Q_OBJECT

public:
    MyLocalServer (QObject *parent = 0) :
        QLocalServer(parent),
        localSocket(0)
    {
        connect (this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
        QLocalServer::removeServer("myserver");
        listen("myserver");
    }

    ~MyLocalServer()
    {
        delete localSocket;
    }

    QString readLine()
    {
        return QString(localSocket->readLine());
    }

public slots:
    void onNewConnection() {
        localSocket = nextPendingConnection();
    }

private:
    QLocalSocket *localSocket;
};


class MyUdpServer : public QObject
{
    Q_OBJECT

public:
    MyUdpServer (QObject *parent = 0) :
        QObject(parent),
        udpSocket(0)
    {
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(QHostAddress::LocalHost, 7755);
    }

    ~MyUdpServer()
    {
        delete udpSocket;
    }

    QString readLine ()
    {
        QByteArray datagram;
         datagram.resize(udpSocket->pendingDatagramSize());
         udpSocket->readDatagram(datagram.data(), datagram.size());
         return QString(datagram);
    }

private:
    QUdpSocket *udpSocket;
};

class Ut_MSyslogClient : public QObject
{
    Q_OBJECT

public:

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testInvalidServer();
    void testSendLocalMessages();
    void testSendUdpMessages();

private:
    MApplication *app;
    MSyslogClientSocket *socket;
};

#endif
