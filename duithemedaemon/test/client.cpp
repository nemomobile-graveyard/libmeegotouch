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

#include <stdlib.h>
#include <QTimer>
#include <QDebug>
#include <QImage>
#include "client.h"
#include "duithemedaemon.h"
#include "duithemedaemonprotocol.h"

void ClientThread::run()
{
    Client client(identifier);
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

using namespace Dui::DuiThemeDaemonProtocol;
Client::Client(const QString &identifier) : identifier(identifier), registered(false)
{
    operationCount = rand() % MAX_OPERATION_COUNT;
    imageDirectory.setPath(QString(IMAGESDIR) + QDir::separator() + identifier);

    int count = rand() % 200;
    // create imagery
    for (int i = 0; i < count; i++) {
        QImage image(64, 64, QImage::Format_ARGB32);
        image.fill(rand());

        QString filename = imageDirectory.absolutePath() + QDir::separator() + QString::number(i) + ".png";
        image.save(filename, "PNG");
    }

    stream.setDevice(&socket);

    connect(&socket, SIGNAL(connected()), SLOT(connected()));
    connect(&socket, SIGNAL(disconnected()), SLOT(disconnected()));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    // connect to server
    socket.connectToServer(Dui::DuiThemeDaemonProtocol::ServerAddress);

    if (!socket.waitForConnected(10000)) {
        qDebug() << "ERROR:" << identifier << "- failed to connect server:" << socket.error();
    } else {
        // perform some task once we get into event loop
        QTimer::singleShot(0, this, SLOT(sendPacket()));
    }
}

Client::~Client()
{
    if (socket.state() == QLocalSocket::ConnectedState) {
        socket.disconnectFromServer();
    }
}

void Client::connected()
{
//    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
}

void Client::disconnected()
{
//    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
}

Dui::DuiThemeDaemonProtocol::Packet Client::processOnePacket()
{
    Packet packet;
    stream >> packet;

    switch (packet.type()) {
    case Packet::PixmapUpdatedPacket: {
        const PixmapHandle *handle = static_cast<const PixmapHandle *>(packet.data());
        if (requestedPixmaps.contains(handle->identifier)) {
            requestedPixmaps.remove(handle->identifier);
            readyPixmaps.insert(handle->identifier);
        } else {
            qDebug() << "ERROR:" << identifier << "- pixmap reply to unknown request";
        }
    } break;

    case Packet::ThemeChangedPacket:
    case Packet::ThemeListPacket:
        break;

    case Packet::ThemeDaemonStatusPacket:
        break;

    default:
        break;
    }

    return packet;
}

void Client::readyRead()
{
    while (socket.bytesAvailable() > 0) {
        processOnePacket();
    }
}

void Client::sendPacket()
{
    if (operationCount > 0) {
        // randomize a task to us
        Task task = (Task)(rand() % NumberOfTasks);

        switch (task) {
        case RegisterToServer:
            registerToServer();
            break;

        case RequestPixmap: {
            QStringList list = imageDirectory.entryList(QDir::Files);
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

void Client::registerToServer()
{
#ifdef PRINT_INFO_MESSAGES
    qDebug() << "INFO:" << identifier << "- registering to server";
#endif
    Packet registration(Packet::RequestRegistrationPacket, new String(identifier));
    stream << registration;

    Packet directory(Packet::RequestNewPixmapDirectoryPacket, new StringBool(imageDirectory.absolutePath(), false));
    stream << directory;

    registered = true;
}

void Client::requestPixmap(Dui::DuiThemeDaemonProtocol::PixmapIdentifier &pixmapIdentifier)
{
#ifdef PRINT_INFO_MESSAGES
    qDebug() << "INFO:" << identifier << "- requesting pixmap" << pixmapIdentifier.imageId << pixmapIdentifier.size;
#endif
    Packet packet(Packet::RequestPixmapPacket, new PixmapIdentifier(pixmapIdentifier));
    stream << packet;

    if (registered) {
        requestedPixmaps.insert(pixmapIdentifier);
    }
}

void Client::releasePixmap(Dui::DuiThemeDaemonProtocol::PixmapIdentifier &pixmapIdentifier)
{
#ifdef PRINT_INFO_MESSAGES
    qDebug() << "INFO:" << identifier << "- releasing pixmap" << pixmapIdentifier.imageId << pixmapIdentifier.size;
#endif
    Packet packet(Packet::ReleasePixmapPacket, new PixmapIdentifier(pixmapIdentifier));
    stream << packet;
    readyPixmaps.remove(pixmapIdentifier);
}

void Client::checkConsistency()
{
    if (!registered)
        return;

    bool consistency = false;

    // disconnect signal
    disconnect(&socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    // send status query packet
    Packet packet(Packet::QueryThemeDaemonStatusPacket);
    stream << packet;
    if (!socket.flush()) {
        qDebug() << "ERROR:" << identifier << "- failed to write to socket" << socket.error();
    }

    bool done = false;
    while (!done) {
        // wait for response
        if (socket.waitForReadyRead(10000)) {
            while (socket.bytesAvailable() > 0) {
                // handle packet that was received
                Packet packet = processOnePacket();

                // check whether it's correct type
                if (packet.type() == Packet::ThemeDaemonStatusPacket) {

                    // extract reply and perform consistency check
                    const ClientList *list = static_cast<const ClientList *>(packet.data());
                    consistency = isDataConsistent(list);
                    done = true;
                    break;
                }
            }
        } else {
            // something bad happened, we did not get reply
            qDebug() << "ERROR:" << identifier << "- failed to receive data from socket" << socket.error();
            break;
        }
    }

    // reconnect signal back
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    // process rest of the packets
    readyRead();

    if (!consistency) {
        qDebug() << "ERROR:" << identifier << "- Consistency check FAILED!";
    }
}

bool Client::isDataConsistent(const Dui::DuiThemeDaemonProtocol::ClientList *list)
{
    foreach(ClientInfo info, list->clients) {
        // find correct info
        if (info.name == identifier) {

            // check that we're really registered
            if (!registered) {
                // we're not registered but our name is still in the client list
                return false;
            }

            // check that the daemon has correct amount of pixmaps in load queue
            if (info.requestedPixmaps.count() != requestedPixmaps.count()) {
                qDebug() << "ERROR:" << identifier << "- incorrect requested pixmap count, Themedaemon says:" << info.requestedPixmaps.count() << "and client says:" << requestedPixmaps.count();
                break;
            }

            // check that we can find all of these from our list
            foreach(const PixmapIdentifier & pixmapIdentifier, requestedPixmaps) {
                if (!info.requestedPixmaps.contains(pixmapIdentifier)) {
                    // pixmap not found from us, but themedaemon reported it -> inconsistent state
                    qDebug() << "ERROR:" << identifier << "- client-side requested pixmap not found:" << pixmapIdentifier.imageId << '(' << pixmapIdentifier.size << ')';
                    break;
                }
            }

            // check that there is correct amount of pixmaps
            if (info.pixmaps.count() - info.releasedPixmaps.count() != readyPixmaps.count()) {
                qDebug() << "ERROR:" << identifier << "- incorrect pixmap count, Themedaemon says:" << info.pixmaps.count() + info.releasedPixmaps.count() << "and client says:" << readyPixmaps.count();
                break;
            }

            // check that we can find all of these from our list
            foreach(const PixmapIdentifier & pixmapIdentifier, readyPixmaps) {
                if (!info.pixmaps.contains(pixmapIdentifier) && !info.releasedPixmaps.contains(pixmapIdentifier)) {
                    // pixmap not found from us, but themedaemon reported it -> inconsistent state
                    qDebug() << "ERROR:" << identifier << "- client-side pixmap not found:" << pixmapIdentifier.imageId << '(' << pixmapIdentifier.size << ')';
                    break;
                }
            }

            return true;
        }
    }

    // if we're registered, we shouldn't end up being here and vice versa
    return registered ? false : true;
}


