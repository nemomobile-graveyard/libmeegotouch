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

#include "testclient.h"
#include "clientmanager.h"

#include "mtheme.h"
#include "mtheme_p.h"
#include "mthemedaemon.h"
#include "mthemedaemonprotocol.h"
#include "imthemedaemon.h"
#include "mremotethemedaemon.h"
#include "mremotethemedaemon_p.h"
#include "mgconfitem.h"

#include <stdlib.h>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QImage>

using namespace M::MThemeDaemonProtocol;

ClientThread::ClientThread(ClientManager* manager) : manager(manager)
{
}

void ClientThread::run()
{
    TestClient client(identifier);
    connect(&client,
            SIGNAL(pixmapReady(const QString&, TestClient*, quint32, const QString&, const QSize&)),
            manager, 
            SLOT(pixmapReady(const QString&, TestClient*, quint32, const QString&, const QSize&)));

    exec();
}

void ClientThread::setId(const QString &id)
{
    identifier = id;
}

const QString &ClientThread::getId() const
{
    return identifier;
}

TestClient::TestClient(const QString &identifier) : identifier(identifier)
{
    operationCount = rand() % MAX_OPERATION_COUNT;

    int count = 50;
    // list all themes
    QDir themeDirectory(THEME_ROOT_DIRECTORY);
    QStringList list = themeDirectory.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i=0; i<list.size(); i++) {
        QString themeName = list.at(i);

        // create path for images
        QString imagesPath = themeName + QDir::separator() + getImageDirectory();
        if(themeDirectory.mkpath(imagesPath))
        {
            // create imagery
            //int count = rand() % 200;
            // create imagery
            for (int i = 0; i < count; i++) {
                QImage image(64, 64, QImage::Format_ARGB32);
                image.fill(rand());

                QString filename = themeDirectory.absolutePath() + QDir::separator() + imagesPath + QDir::separator() + QString::number(i) + ".png";
                image.save(filename, "PNG");
            }
        }
    }

    daemon = new MRemoteThemeDaemon(identifier, 0);
    if (!daemon->connected()) {
        qCritical("Could not connect to remote themedaemon");
        return;
    }

    currentTheme = daemon->themeInheritanceChain().at(0);

    connect(daemon, SIGNAL(pixmapChanged(QString, QSize, Qt::HANDLE)),
            SLOT(pixmapChangedSlot(QString, QSize, Qt::HANDLE)));
    connect(daemon, SIGNAL(themeChanged(QStringList, QStringList)),
            SLOT(themeChangedSlot(QStringList, QStringList)));

    connect(&daemon->d_ptr->socket, SIGNAL(connected()), SLOT(connected()));
    connect(&daemon->d_ptr->socket, SIGNAL(disconnected()), SLOT(disconnected()));

    QTimer::singleShot(0, this, SLOT(sendPacket()));
}

TestClient::~TestClient()
{

}

const QString &TestClient::getId() const
{
    return identifier;
}

void TestClient::connected()
{
//    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
}

void TestClient::disconnected()
{
//    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
}

void TestClient::pixmapChangedSlot(const QString &imageId, const QSize &size, Qt::HANDLE pixmapHandle)
{
    emit pixmapReady(currentTheme, this, pixmapHandle, imageId, size);
    waitVerify.acquire();
}

void TestClient::themeChangedSlot(const QStringList &themeInheritance, const QStringList& libraryNames)
{
    currentTheme = themeInheritance.at(0);
}

void TestClient::pixmapVerified(const QString& imageId, const QSize& size)
{

    PixmapIdentifier identifier(imageId, size);

    if (requestedPixmaps.contains(identifier)) {
        // pixmap found from requested list -> new one
        requestedPixmaps.remove(identifier);
        readyPixmaps.insert(identifier);
    } else if(readyPixmaps.contains(identifier)) {
        // this pixmap was already ready, so it is just updated (probably due to theme change)
    } else {
        qWarning() << "ERROR:" << imageId << "- pixmap reply to unknown request";
    }

    waitVerify.release();
}

void TestClient::sendPacket()
{
    if (operationCount > 0) {

        // randomize a task to us
        Task task = (Task)(rand() % NumberOfTasks);
        switch (task) {
        case RegisterToServer:
            registerToServer();
            break;

        case RequestPixmap: {
            // this directory contains all icons for this theme
            QDir themeIconDirectory = currentTheme + QDir::separator() + QString("meegotouch") + QDir::separator() + QString("icons");
            QStringList iconList = themeIconDirectory.entryList(QDir::Files);

            // this directory contains all images for this client
            QDir imageDirectory = currentTheme + QDir::separator() + getImageDirectory();
            QStringList imageList = imageDirectory.entryList(QDir::Files);

            // combine both lists as one, we'll request something from this list
            QStringList list;
            list.append(iconList);
            list.append(imageList);

            if (list.count() > 0) {
                // select image
                int index = rand() % list.count();

                // get pixmap identifier
                QFileInfo info(list[index]);
                PixmapIdentifier pixmapIdentifier(info.baseName(), QSize(64, 64));

                // make sure this has not been requested already
                if (!requestedPixmaps.contains(pixmapIdentifier) && !readyPixmaps.contains(pixmapIdentifier)) {
                    requestPixmap(pixmapIdentifier);
                } else {
                    // it was already requested, should we try to request some other pixmap instead?
                    // nah, go away and do something else next time..
                }
            } else {
                qWarning() << "request empty";
            }
        } break;

        case ReleasePixmap: {
            // make sure that we have something to release
            if (readyPixmaps.count() > 0) {
                // release the first pixmap we have, this could have better logic..
                PixmapIdentifier toRemove = *readyPixmaps.begin();
                releasePixmap(toRemove);
            }
        } break;

        case CheckConsistency:
            checkConsistency();
            break;

        default:
            break;
        }
        // do something else next time
        operationCount--;
        QTimer::singleShot(TASK_EXECUTION_INTERVAL, this, SLOT(sendPacket()));
    } else if (readyPixmaps.count() > 0 || requestedPixmaps.count() > 0) {

        while (readyPixmaps.count() > 0) {
            PixmapIdentifier toRemove = *readyPixmaps.begin();
            releasePixmap(toRemove);
        }

        while (requestedPixmaps.count() > 0) {
            PixmapIdentifier toRemove = *requestedPixmaps.begin();
            releasePixmap(toRemove);
        }

        QTimer::singleShot(1000, this, SLOT(sendPacket()));
    } else {
        checkConsistency();
        QThread::currentThread()->quit();
    }
}

void TestClient::registerToServer()
{
    Packet registration(Packet::RequestRegistrationPacket, ++daemon->d_ptr->sequenceCounter);
    registration.setData(new String(identifier));
    daemon->d_ptr->stream << registration;
}

void TestClient::requestPixmap(M::MThemeDaemonProtocol::PixmapIdentifier &pixmapIdentifier)
{
#ifdef PRINT_INFO_MESSAGES
    qDebug() << "INFO:" << identifier << "- requesting pixmap" << pixmapIdentifier.imageId << pixmapIdentifier.size;
#endif

    daemon->pixmapHandle(pixmapIdentifier.imageId, pixmapIdentifier.size);
    requestedPixmaps.insert(pixmapIdentifier);
}

void TestClient::releasePixmap(M::MThemeDaemonProtocol::PixmapIdentifier &pixmapIdentifier)
{
#ifdef PRINT_INFO_MESSAGES
    qDebug() << "INFO:" << identifier << "- releasing pixmap" << pixmapIdentifier.imageId << pixmapIdentifier.size;
#endif
    daemon->releasePixmap(pixmapIdentifier.imageId, pixmapIdentifier.size);
    readyPixmaps.remove(pixmapIdentifier);
    requestedPixmaps.remove(pixmapIdentifier);
}

void TestClient::checkConsistency()
{
    bool consistency = false;

    // send status query packet
    int sequenceNr = ++daemon->d_ptr->sequenceCounter;
    Packet packet(Packet::QueryThemeDaemonStatusPacket, sequenceNr);
    daemon->d_ptr->stream << packet;
    if (!daemon->d_ptr->socket.flush()) {
        qWarning() << "ERROR:" << identifier << "- failed to write to socket" << daemon->d_ptr->socket.error();
    }

    Packet packet2 = daemon->d_ptr->waitForPacket(sequenceNr);
    const ClientList *list = static_cast<const ClientList *>(packet2.data());
    consistency = isDataConsistent(list);

    if (!consistency) {
        qWarning() << "ERROR:" << identifier << "- Consistency check FAILED!";
    }
}

bool TestClient::isDataConsistent(const M::MThemeDaemonProtocol::ClientList *list)
{
    foreach(ClientInfo info, list->clients) {
        // find correct info
        if (info.name == identifier) {
            // theme change may cause pixmaps to move from loaded list to request list in daemon side,
            // so we'll compare both lists as one

            // create list of daemon-side pixmaps (requests & loaded)
            QList<PixmapIdentifier> daemon;
            daemon.append(info.requestedPixmaps);
            daemon.append(info.pixmaps);

            // create list of client-side pixmaps (requests & loaded)
            QList<PixmapIdentifier> client;
            client.append(requestedPixmaps.toList());
            client.append(readyPixmaps.toList());

            // check that the daemon has correct amount of pixmaps in load queue
            if (daemon.count() != client.count()) {
                qWarning() << "ERROR:" << identifier << "- incorrect pixmap count, Themedaemon says:" << daemon.count() << "and client says:" << client.count();
                break;
            }

            // check that we can find all daemon-side pixmaps from the client-side list
            foreach(const PixmapIdentifier & pixmapIdentifier, daemon) {
                if (!client.contains(pixmapIdentifier)) {
                    // pixmap not found from client, but themedaemon reported it -> inconsistent state
                    qWarning() << "ERROR:" << identifier << "- pixmap not found from client-side list:" << pixmapIdentifier.imageId << '(' << pixmapIdentifier.size << ')';
                    break;
                } else {
                    // found, we can remove this one from client list
                    client.removeOne(pixmapIdentifier);
                }
            }

            // check that we can find all client-side pixmaps from the daemon-side list
            foreach(const PixmapIdentifier & pixmapIdentifier, client) {
                if (!daemon.contains(pixmapIdentifier)) {
                    // pixmap not found from daemon-side list, but exists in client-side list -> inconsistent state
                    qWarning() << "ERROR:" << identifier << "- pixmap not found from daemon-side list:" << pixmapIdentifier.imageId << '(' << pixmapIdentifier.size << ')';
                    break;
                }
            }

            return true;
        }
    }

    // if we are registered, we shouldn't end up being here
    return false;
}

QString TestClient::getImageDirectory() const
{
    return QString("meegotouch") + QDir::separator() +
           QString(identifier) + QDir::separator() + 
           QString("images");
}

