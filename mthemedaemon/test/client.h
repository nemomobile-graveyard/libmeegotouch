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

#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>
#include <QDataStream>
#include <QSize>
#include <QDir>
#include <QLocalSocket>
#include <QSemaphore>

#include "mthemedaemonclient.h"

class Client;
class ClientManager;

class ClientThread : public QThread
{
public:
    ClientThread(ClientManager* manager);
    void setId(const QString &id);
    const QString &getId() const;

protected:
    virtual void run();
private:
    ClientManager* manager;
    QString identifier;
};

class Client : public QObject
{
    Q_OBJECT

    static const int MAX_OPERATION_COUNT = 100;
    static const int TASK_EXECUTION_INTERVAL = 50;

    enum Task {
        RegisterToServer,
        RequestPixmap,
        ReleasePixmap,
        CheckConsistency,

        NumberOfTasks
    };

public:
    Client(const QString &identifier);
    ~Client();

    const QString& getId() const;

    QString getImageDirectory() const;
    void pixmapVerified(const QString& imageId, const QSize& size);

signals:
    void pixmapReady(const QString& theme, Client* client, quint32 handle, const QString&, const QSize&);


protected:

    void registerToServer();
    void requestPixmap(M::MThemeDaemonProtocol::PixmapIdentifier &);
    void releasePixmap(M::MThemeDaemonProtocol::PixmapIdentifier &);
    void checkConsistency();
    M::MThemeDaemonProtocol::Packet processOnePacket();

private slots:
    void connected();
    void disconnected();
    void sendPacket();
    void readyRead();

private:

    bool isDataConsistent(const M::MThemeDaemonProtocol::ClientList *reply);
    void quit();

    QLocalSocket socket;
    QDataStream stream;
    QSet<M::MThemeDaemonProtocol::PixmapIdentifier> requestedPixmaps;
    QSet<M::MThemeDaemonProtocol::PixmapIdentifier> readyPixmaps;
    QString identifier;
    bool registered;
    int operationCount;
    quint64 packetsSent;
    QString currentTheme;
    QSemaphore waitVerify;
};

#endif
