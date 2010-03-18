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

#ifndef CLIENT_H
#define CLIENT_H

#include <QThread>
#include <QDataStream>
#include <QSize>
#include <QDir>
#include <QLocalSocket>

#include "duithemedaemonclient.h"

class Client;

class ClientThread : public QThread
{
public:
    void setId(const QString &id);
    const QString &getId() const;

protected:
    virtual void run();
private:
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

    void setId(const QString &id);
    const QString &getId() const;

protected:

    void registerToServer();
    void requestPixmap(Dui::DuiThemeDaemonProtocol::PixmapIdentifier &);
    void releasePixmap(Dui::DuiThemeDaemonProtocol::PixmapIdentifier &);
    void checkConsistency();
    Dui::DuiThemeDaemonProtocol::Packet processOnePacket();

private slots:
    void connected();
    void disconnected();
    void sendPacket();
    void readyRead();

private:

    bool isDataConsistent(const Dui::DuiThemeDaemonProtocol::ClientList *reply);
    void quit();

    QLocalSocket socket;
    QDataStream stream;
    QSet<Dui::DuiThemeDaemonProtocol::PixmapIdentifier> requestedPixmaps;
    QSet<Dui::DuiThemeDaemonProtocol::PixmapIdentifier> readyPixmaps;
    QString identifier;
    bool registered;
    int operationCount;
    quint64 packetsSent;
    QString currentTheme;
};

#endif
