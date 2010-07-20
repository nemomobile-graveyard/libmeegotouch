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

class TestClient;
class ClientManager;
class MRemoteThemeDaemon;

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

class TestClient : public QObject
{
    Q_OBJECT

    static const int MAX_OPERATION_COUNT = 250;
    static const int TASK_EXECUTION_INTERVAL = 50;

    enum Task {
        RegisterToServer,
        RequestPixmap,
        ReleasePixmap,
        CheckConsistency,

        NumberOfTasks
    };

public:
    TestClient(const QString &identifier);
    ~TestClient();

    const QString& getId() const;

    QString getImageDirectory() const;
    void pixmapVerified(const QString& imageId, const QSize& size);

signals:
    void pixmapReady(const QString& theme, TestClient* client, quint32 handle, const QString&, const QSize&);


protected:

    void registerToServer();
    void requestPixmap(M::MThemeDaemonProtocol::PixmapIdentifier &);
    void releasePixmap(M::MThemeDaemonProtocol::PixmapIdentifier &);
    void checkConsistency();

private slots:
    void pixmapChangedSlot(const QString &imageId, const QSize &size, Qt::HANDLE pixmapHandle);
    void themeChangedSlot(const QStringList &themeInheritance, const QStringList& libraryNames);
    void connected();
    void disconnected();
    void sendPacket();

private:

    bool isDataConsistent(const M::MThemeDaemonProtocol::ClientList *reply);
    void quit();

    MRemoteThemeDaemon *daemon;

    QSet<M::MThemeDaemonProtocol::PixmapIdentifier> requestedPixmaps;
    QSet<M::MThemeDaemonProtocol::PixmapIdentifier> readyPixmaps;
    QString identifier;
    int operationCount;
    QString currentTheme;
    QSemaphore waitVerify;
};

#endif
