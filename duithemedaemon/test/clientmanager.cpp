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

#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QDir>
#include "clientmanager.h"
#include "client.h"

#define CLIENT_ID(client) "Client (0x" + QString::number((quint32) client, 16) + ')'

void removeDirectoryRecursive(const QString &path)
{
    QDir root(path);
    if (root.exists()) {
        QFileInfoList entries = root.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
        for (int i = 0; i < entries.count(); i++) {
            if (entries[i].isDir()) {
                removeDirectoryRecursive(entries[i].filePath());
                root.rmdir(entries[i].absoluteFilePath());
            } else {
                root.remove(entries[i].absoluteFilePath());
            }
        }
    }
}

ClientManager::ClientManager(QProcess &process) : shutdown(false), themedaemon(process)
{
    removeDirectoryRecursive(QDir(IMAGESDIR).absolutePath());
    QDir dir;
    dir.mkdir(IMAGESDIR);

    // start the test after 1 sec (allow themedaemon to get online)
    QTimer::singleShot(1000, this, SLOT(start()));
}

ClientManager::~ClientManager()
{
    // perform cleanup
    removeDirectoryRecursive(IMAGESDIR);
}

void ClientManager::spawnClient()
{
    static unsigned int clientId = 0;
    ClientThread *client = new ClientThread();
    client->setId("client" + QString::number(clientId++));

    // generate imagery for testing purposes
    QDir themeDirectory(IMAGESDIR);
    if (themeDirectory.mkdir(client->getId())) {
        connect(client, SIGNAL(started()), this, SLOT(clientStarted()));
        connect(client, SIGNAL(finished()), this, SLOT(clientFinished()));
        clients.insert(client);

        client->start();
#ifdef PRINT_INFO_MESSAGES
        qDebug() << "INFO: ClientManager - Client started, number of active clients:" << clients.count();
#endif
    } else {
        qDebug() << "ERROR: ClientManager - Failed to spawn new client" << client->getId();
        delete client;
        stop();
    }
}

void ClientManager::start()
{
#ifdef PRINT_INFO_MESSAGES
    qDebug() << "INFO: ClientManager - Starting up...";
#endif
    // make sure themedaemon is online

    // start performing consistency checks & spawn new clients
    QTimer::singleShot(0, this, SLOT(checkConsistency()));
}

void ClientManager::stop()
{
    shutdown = true;
    qDebug() << "INFO: ClientManager - Shutting down...";

    if (clients.count() == 0) {
        qApp->quit();
    }
}

void ClientManager::clientStarted()
{
    Client *client = static_cast<Client *>(sender());
    Q_UNUSED(client);
}


void ClientManager::clientFinished()
{
    ClientThread *client = static_cast<ClientThread *>(sender());

    removeDirectoryRecursive(QString(IMAGESDIR) + QDir::separator() + client->getId());
    clients.remove(client);
    client->exit();
    client->wait();
    delete client;
#ifdef PRINT_INFO_MESSAGES
    qDebug() << "INFO: ClientManager - Client finished, number of active clients:" << clients.count();
#endif
    if (shutdown) {
        if (clients.count() == 0) {
            qApp->quit();
        }
    }
}

void ClientManager::checkConsistency()
{
    // check that themedaemon has not crashed
    if (themedaemon.state() != QProcess::Running) {
        qDebug() << "ERROR: ClientManager - Themedaemon is not running";

        shutdown = true;
        foreach(ClientThread * thread, clients) {
            thread->exit();
            thread->wait();
            delete thread;
        }
        qApp->quit();
    }

    if (shutdown == false) {
        // spawn new clients if there is room
        while (clients.count() < ClientManager::MAX_CLIENT_COUNT) {
            spawnClient();
        }
        // get ready to perform another consistency check
        QTimer::singleShot(5000, this, SLOT(checkConsistency()));
    }
}

