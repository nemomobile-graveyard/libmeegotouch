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
#include <QSocketNotifier>


#include <mthemedaemon.h>

class QLocalSocket;
class MThemeDaemonClient;

//! \internal
class MThemeDaemonServer : public QObject
{
    Q_OBJECT
public:
    /**
     * @param serverAddress Address that is used for the local connection
     *                      between the clients and the server. If an empty
     *                      string is passed, M::MThemeDaemonProtocol::ServerAddress
     *                      is used per default.
     */
    MThemeDaemonServer(const QString &serverAddress = QString());
    virtual ~MThemeDaemonServer();

    static void hupSignalHandler(int unused);
    static void termSignalHandler(int unused);
    static void intSignalHandler(int unused);
    static void usr1SignalHandler(int unused);

public slots:
    void themeChanged(bool forceReload = false);
    void setSlowDown(int slowDown);

    // Qt signal handlers.
    void handleSigHup();
    void handleSigTerm();
    void handleSigInt();
    void handleSigUsr1();

private slots:
    void clientConnected();
    void clientDisconnected();
    void clientDataAvailable();

    void themeChangeTimeout();

    void localeChanged();

    void processOneQueueItem();

    void handleUpdatedMostUsedPixmaps(const M::MThemeDaemonProtocol::MostUsedPixmaps& mostUsed);

private:
    void loadPriorities(const QString& filename);

    void pixmapUsed(MThemeDaemonClient *client,
                                const M::MThemeDaemonProtocol::PixmapIdentifier &id,
                                quint64 sequenceNumber);
    void pixmapRequested(MThemeDaemonClient *client,
                                const M::MThemeDaemonProtocol::PixmapIdentifier &id,
                                qint32 priority,
                                quint64 sequenceNumber);
    void pixmapReleaseRequested(MThemeDaemonClient *client,
                                const M::MThemeDaemonProtocol::PixmapIdentifier &id,
                                quint64 sequenceNumber);

    void themeChangeApplied(MThemeDaemonClient *client,
                                qint32 priority, quint64 sequenceNumber);

    void ackMostUsedPixmaps(MThemeDaemonClient *client, quint64 sequenceNumber);

    void themeDaemonStatus(MThemeDaemonClient *client, quint64 sequenceNumber) const;

    void finalizeThemeChange();

    bool createCacheDir(const QString& path);

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

    /**
     * Prints the memory-usage required for all shared pixmaps to the default
     * temporary directory (e.g. /var/tmp) as "mthemedaemonserver-stat.txt".
     */
    class MemoryUsagePrinter
    {
    public:
        MemoryUsagePrinter(const QList<MThemeDaemonClient*> &clients);
        void print();
    private:
        QString keyForHandle(const MPixmapHandle &handle) const;
        void addPixmapCacheEntries(const QString &client,
                                   const QString &imageId,
                                   const QHash<QSize, const PixmapCacheEntry*> &pixmapCacheEntries,
                                   QHash<QString, QString>& sharedPixmaps);
        QList<QString> layoutedLines(const QList<QString> &lines) const;
        int countColumns(const QList<QString> &lines) const;

        const QChar columnSeparator;
        QList<QString> lines;
        QHash<QString, int> clientMemoryUsage;
        int sharedMemoryUsage;
    };

    QLocalServer server;
    QHash<QLocalSocket *, MThemeDaemonClient *> registeredClients;
    MThemeDaemon daemon;

    MGConfItem currentTheme;
    MGConfItem currentLocale;
    QString defaultTheme;
    bool delayedThemeChange;
    int slowDown;

    QMap<qint32, QQueue<QueueItem> > loadPixmapsQueue;
    QQueue<QueueItem> releasePixmapsQueue;
    QQueue<MThemeDaemonClient*> clearCustomDirectoriesQueue;
    QTimer processQueueTimer;

    QList<MThemeDaemonClient*> clientsThatHaveNotYetAppliedThemeChange;
    QList<PixmapCacheEntry*> pixmapsToDeleteWhenThemeChangeHasCompleted;

    QHash<quint64, QList<MThemeDaemonClient*> > clientsThatHaveNotYetUpdatedMostUsed;
    QHash<quint64, QList<M::MThemeDaemonProtocol::PixmapIdentifier> > pixmapsToDeleteWhenUpdatedMostUsed;

    quint64 sequenceCounter;
    qint32 priorityForegroundApplication;

    static int sighupFd[2];
    static int sigtermFd[2];
    static int sigintFd[2];
    static int sigusr1Fd[2];
    QSocketNotifier *snHup;
    QSocketNotifier *snTerm;
    QSocketNotifier *snInt;
    QSocketNotifier *snUsr1;
};
//! \internal_end
#endif

