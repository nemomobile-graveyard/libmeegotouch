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

#ifndef DUITHEMEDAEMONSERVER_H
#define DUITHEMEDAEMONSERVER_H

#include <QObject>
#include <QQueue>
#include <QLocalServer>
#include <DuiGConfItem>
#include <QTimer>


#include <duithemedaemon.h>

class QLocalSocket;
class DuiThemeDaemonClient;

//! \internal
class DuiThemeDaemonServer : public QObject
{
    Q_OBJECT
public:
    DuiThemeDaemonServer();
    virtual ~DuiThemeDaemonServer();

private slots:
    void clientConnected();
    void clientDisconnected();
    void clientDataAvailable();

    void themeChanged();
    void localeChanged();

    void processOneQueueItem();

private:
    void pixmapRequested(DuiThemeDaemonClient *client,
                         const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id,
                         quint32 sequenceNumber);
    void pixmapReleaseRequested(DuiThemeDaemonClient *client,
                                const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id,
                                quint32 sequenceNumber);
    void themeDaemonStatus(DuiThemeDaemonClient *client, quint32 sequenceNumber) const;

private:
    struct QueueItem
    {
        DuiThemeDaemonClient *                        client;
        Dui::DuiThemeDaemonProtocol::PixmapIdentifier pixmapId;
        quint32                                       sequenceNumber;

        QueueItem(DuiThemeDaemonClient *c, Dui::DuiThemeDaemonProtocol::PixmapIdentifier p,
                  quint32 s = 0) : client(c), pixmapId(p), sequenceNumber(s) {}

        // Ignore the sequence number when testing for equality.  A custom
        // predicate would be more appropriate, if Qt would support that.
        bool operator==(const QueueItem &other) const
            { return (client == other.client && pixmapId == other.pixmapId); }
        bool operator!=(const QueueItem &other) const
            { return (client != other.client || pixmapId != other.pixmapId); }
    };

    QLocalServer server;
    QHash<QLocalSocket *, DuiThemeDaemonClient *> registeredClients;
    DuiThemeDaemon daemon;

    DuiGConfItem currentTheme;
    DuiGConfItem currentLocale;

    QQueue<QueueItem> loadPixmapsQueue;
    QQueue<QueueItem> releasePixmapsQueue;
    QTimer processQueueTimer;
};
//! \internal_end
#endif

