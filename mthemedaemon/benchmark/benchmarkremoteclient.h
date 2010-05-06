/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libm.
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
#include <QMutex>
#include <QWaitCondition>
#include <QSemaphore>
#include <QMap>
#include <QTime>

#include "mthemedaemonclient.h"

#include "common.h"


class BenchmarkRemoteClient : public QObject
{
    Q_OBJECT

public:
    BenchmarkRemoteClient();
    ~BenchmarkRemoteClient();

signals:
    void pixmapReady(quint32 handle, const M::MThemeDaemonProtocol::PixmapIdentifier& identifier);
    void finished();

public slots:
    void run();

protected:
    void requestPixmap(M::MThemeDaemonProtocol::PixmapIdentifier &);
    void releasePixmap(const M::MThemeDaemonProtocol::PixmapIdentifier &);

    M::MThemeDaemonProtocol::Packet processOnePacket();

private slots:
    void connected();
    void disconnected();
    void registerToServer();
    void sendPacket();
    void updatePixmapStats(quint32 handle, const M::MThemeDaemonProtocol::PixmapIdentifier& identifier);
    void readyRead();
    void printResults();

private:

    bool isDataConsistent(const M::MThemeDaemonProtocol::ClientList *reply);
    void quit();

    QLocalSocket socket;
    QDataStream stream;
    QList<QPair<QString,QSize> > pixmapsToRequest;
    QMap<QString, RequestInfo> requestedPixmaps;
    QMap<QString, RequestInfo> readyPixmaps;
    quint64 packetsSent;
};

#endif
