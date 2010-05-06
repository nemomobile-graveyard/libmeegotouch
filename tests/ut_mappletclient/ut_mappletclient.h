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

#ifndef UT_MAPPLETCLIENT_H
#define UT_MAPPLETCLIENT_H

#include <QObject>
#include <QThread>

class QProcess;
class QLocalSocket;
class QLocalServer;
class MAppletClient;

class Ut_MAppletClient : public QObject
{
    Q_OBJECT

private:
    QProcess *process;
    MAppletClient *client;
    bool connected;

private slots:
    void init();
    void cleanup();

    void testConnection();
    void testReconnect();

public slots:
    void connectionEstablished();
};

class RemoteProcessThread : public QThread
{
    Q_OBJECT

private:
    QLocalSocket *socket;
    QLocalServer *server;

    void waitHere();

private slots:
    void readyRead();
    void newConnection();

public:
    RemoteProcessThread();
    virtual ~RemoteProcessThread();
    virtual void run();

    bool doWait;
};

#endif // UT_MAPPLETCLIENT_H
