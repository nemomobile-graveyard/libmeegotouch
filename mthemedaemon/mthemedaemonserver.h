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

#ifndef MTHEMEDAEMONSERVER_H
#define MTHEMEDAEMONSERVER_H

#include <QObject>
#include <QQueue>
#include <QLocalServer>
#include <MGConfItem>
#include <QTimer>


#include <mthemedaemon.h>

class QLocalSocket;
class MThemeDaemonClient;

//! \internal
class MThemeDaemonServer : public QObject
{
    Q_OBJECT
public:
    MThemeDaemonServer();
    virtual ~MThemeDaemonServer();

private slots:
    void clientConnected();
    void clientDisconnected();
    void clientDataAvailable();

    void themeChanged();
    void localeChanged();

    void processOneQueueItem();

private:
    void pixmapRequested(MThemeDaemonClient *client,
                         const M::MThemeDaemonProtocol::PixmapIdentifier &id,
                         quint64 sequenceNumber);
    void pixmapReleaseRequested(MThemeDaemonClient *client,
                                const M::MThemeDaemonProtocol::PixmapIdentifier &id,
                                quint64 sequenceNumber);

    void themeChangeApplied(MThemeDaemonClient *client, quint64 sequenceNumber);

    void themeDaemonStatus(MThemeDaemonClient *client, quint64 sequenceNumber) const;

private:
    struct QueueItem
    {
        quint64                                       sequenceNumber;
        MThemeDaemonClient *                        client;
        M::MThemeDaemonProtocol::PixmapIdentifier pixmapId;

        QueueItem(MThemeDaemonClient *c, M::MThemeDaemonProtocol::PixmapIdentifier p,
                  quint64 s = 0) : sequenceNumber(s), client(c), pixmapId(p) {}

        // Ignore the sequence number when testing for equality.  A custom
        // predicate would be more appropriate, if Qt would support that.
        bool operator==(const QueueItem &other) const
            { return (client == other.client && pixmapId == other.pixmapId); }
        bool operator!=(const QueueItem &other) const
            { return (client != other.client || pixmapId != other.pixmapId); }
    };

    QLocalServer server;
    QHash<QLocalSocket *, MThemeDaemonClient *> registeredClients;
    MThemeDaemon daemon;

    MGConfItem currentTheme;
    MGConfItem currentLocale;

    QQueue<QueueItem> loadPixmapsQueue;
    QQueue<QueueItem> releasePixmapsQueue;
    QTimer processQueueTimer;

    QList<MThemeDaemonClient*> clientsThatHaveNotYetAppliedThemeChange;
    QList<QPixmap*> pixmapsToDeleteWhenThemeChangeHasCompleted;
};
//! \internal_end
#endif

