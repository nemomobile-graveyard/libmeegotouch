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

#include "duithemedaemonserver.h"
#include <theme/duithemedaemonclient.h>
#include <DuiDebug>
#include <DuiNamespace>

#include <QLocalSocket>
#include <QDir>


using namespace Dui::DuiThemeDaemonProtocol;

DuiThemeDaemonServer::DuiThemeDaemonServer() :
    currentTheme("/Dui/theme/name"),
    currentLocale("/Dui/i18n/Language")
{
    const QString defaultTheme("devel");

    // 1) make sure that gconf has some value for the current theme
    if (currentTheme.value().isNull())
        currentTheme.set(defaultTheme);

    // 2) activate the current theme
    QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> > pixmapsToReload;
    if (!daemon.activateTheme(currentTheme.value().toString(), currentLocale.value().toString(), QList<DuiThemeDaemonClient *>(), pixmapsToReload)) {
        // could not activate current theme, change to devel
        if (!daemon.activateTheme(defaultTheme, currentLocale.value().toString(), QList<DuiThemeDaemonClient *>(), pixmapsToReload)) {
            qFatal("DuiThemeDaemonServer - Could not find themes, aborting!");
        }
        currentTheme.set(defaultTheme);
    }
    Q_ASSERT(pixmapsToReload.isEmpty());

    // 3) make sure we're notified if locale or theme changes
    connect(&currentTheme, SIGNAL(valueChanged()), SLOT(themeChanged()));
    connect(&currentLocale, SIGNAL(valueChanged()), SLOT(localeChanged()));

    // 4) make sure we have a themedaemon directory in /var/cache/dui/
    QDir cacheDir(DuiThemeDaemon::systemThemeCacheDirectory());
    if(!cacheDir.exists()) {
        if(!cacheDir.mkpath(DuiThemeDaemon::systemThemeCacheDirectory())) {
            qFatal("DuiThemeDaemonServer - Could not create cache directory for themedaemon. No write permissions to %s",
                qPrintable(cacheDir.absolutePath()));
        }
    }

    // 5) make sure we have a cache directory for the current theme
    if(!cacheDir.exists(currentTheme.value().toString())) {
        if(!cacheDir.mkdir(currentTheme.value().toString())) {
            qFatal("DuiThemeDaemonServer - Could not create cache directory for current theme. No write permissions to %s",
                qPrintable(cacheDir.absolutePath()));
        }
    }


    // start socket server for client registeration
    // first remove the old one, if there's such
    QLocalServer::removeServer(Dui::DuiThemeDaemonProtocol::ServerAddress);
    connect(&server, SIGNAL(newConnection()), SLOT(clientConnected()));
    if (!server.listen(Dui::DuiThemeDaemonProtocol::ServerAddress)) {
        duiWarning("DuiThemeDaemonServer") << "Failed to start socket server.";
    }

    processQueueTimer.setInterval(0);
    processQueueTimer.setSingleShot(false);
    connect(&processQueueTimer, SIGNAL(timeout()), SLOT(processOneQueueItem()));
}

DuiThemeDaemonServer::~DuiThemeDaemonServer()
{
    processQueueTimer.stop();

    // close socket server
    server.close();

    // remove all registered clients
    while (registeredClients.count() > 0) {
        DuiThemeDaemonClient *client = registeredClients.begin().value();
        daemon.removeClient(client);
        delete client;
        registeredClients.erase(registeredClients.begin());
    }

    loadPixmapsQueue.clear();
    releasePixmapsQueue.clear();
}

// gets called when a new client(socket) connects to the daemon
void DuiThemeDaemonServer::clientConnected()
{
    while (server.hasPendingConnections()) {
        QLocalSocket *socket = server.nextPendingConnection();
        QObject::connect(socket, SIGNAL(disconnected()), SLOT(clientDisconnected()));
        QObject::connect(socket, SIGNAL(readyRead()), SLOT(clientDataAvailable()));
    }
}

// gets called when any client has disconnected
void DuiThemeDaemonServer::clientDisconnected()
{
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    if (socket) {
        DuiThemeDaemonClient *client = registeredClients.value(socket, NULL);
        if (client) {
            daemon.removeClient(client);
            registeredClients.remove(socket);

            // remove all queued pixmap requests
            QMutableListIterator<QueueItem> pi = loadPixmapsQueue;
            while (pi.hasNext()) {
                if (pi.next().client == client)
                    pi.remove();
            }
            // remove all queued pixmap releases
            pi = releasePixmapsQueue;
            while (pi.hasNext()) {
                if (pi.next().client == client)
                    pi.remove();
            }

            delete client;
        }
        socket->deleteLater();
    }
}


// gets called when any client has sent data over socket
void DuiThemeDaemonServer::clientDataAvailable()
{
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    if (!socket) {
        return;
    }

    // has the client registered?
    DuiThemeDaemonClient *client = registeredClients.value(socket, NULL);
    if (!client) {

        // create a temporary data stream to read from the socket
        QDataStream stream(socket);

        // loop as long as the socket has some data left
        while (socket->bytesAvailable()) {

            // read one packet from the socket
            Packet packet;
            stream >> packet;
            // and check if it's a registration request
            if (packet.type() != Packet::RequestRegistrationPacket) {
                // reply error
                stream << Packet(Packet::ErrorPacket, packet.sequenceNumber(),
                                 new String("You must send registration packet before requesting anything else!"));
            } else {
                // we got the registration packet so register the client, and continue normally
                client = new DuiThemeDaemonClient(socket, static_cast<const String *>(packet.data())->string, daemon.themeInheritanceChain());
                registeredClients.insert(socket, client);
                daemon.addClient(client);
                client->stream() << Packet(Packet::ThemeChangedPacket, packet.sequenceNumber(),
                                           new ThemeChangeInfo(daemon.themeInheritanceChain(), daemon.themeLibraryNames()));
                break;
            }
        }
    }

    // client has registered, so we'll read all packets that are currently available from the socket
    while (socket->bytesAvailable()) {

        // read packet from socket
        Packet packet;
        client->stream() >> packet;

        // process it according to packet type
        switch (packet.type()) {

        case Packet::RequestRegistrationPacket: {
            // client tried to register a second time
            client->stream() << Packet(Packet::ErrorPacket, packet.sequenceNumber(),
                                       new String("You have already registered!"));
            duiWarning("DuiThemeDaemonServer") << "Client with name" << client->name()
                                               << "tried to register a second time!";
        } break;

        case Packet::RequestPixmapPacket: {
            // client requested a pixmap
            const PixmapIdentifier *id = static_cast<const PixmapIdentifier *>(packet.data());
            pixmapRequested(client, *id, packet.sequenceNumber());
        } break;

        case Packet::ReleasePixmapPacket: {
            // client requested a pixmap release
            const PixmapIdentifier *id = static_cast<const PixmapIdentifier *>(packet.data());
            pixmapReleaseRequested(client, *id, packet.sequenceNumber());
        } break;

        case Packet::RequestClearPixmapDirectoriesPacket: {
            client->removeAddedImageDirectories();
        } break;

        case Packet::RequestNewPixmapDirectoryPacket: {
            const StringBool *sb = static_cast<const StringBool *>(packet.data());
            client->addCustomImageDirectory(sb->string, sb->b ? Dui::Recursive : Dui::NonRecursive);
        } break;


        case Packet::QueryThemeDaemonStatusPacket: {
            themeDaemonStatus(client, packet.sequenceNumber());
        } break;


        default: {
            duiWarning("DuiThemeDaemonServer") << "unknown packet received:" << packet.type();
        } break;
        }
    }
}

void DuiThemeDaemonServer::themeChanged()
{
    if (daemon.currentTheme() == currentTheme.value().toString())
        return;

    QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> > pixmapsToReload;
    if (daemon.activateTheme(currentTheme.value().toString(), currentLocale.value().toString(), registeredClients.values(), pixmapsToReload)) {
        // theme change succeeded, let's inform all clients + add the pixmaps to load-list
        Packet themeChangedPacket(Packet::ThemeChangedPacket, 0, new ThemeChangeInfo(daemon.themeInheritanceChain(), daemon.themeLibraryNames()));

        QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> >::iterator i = pixmapsToReload.begin();
        QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> >::iterator end = pixmapsToReload.end();
        for (; i != end; ++i) {
            DuiThemeDaemonClient *client = i.key();
            const QList<PixmapIdentifier> &ids = i.value();

            client->stream() << themeChangedPacket;

            const QList<PixmapIdentifier>::const_iterator idsEnd = ids.end();
            for (QList<PixmapIdentifier>::const_iterator iId = ids.begin(); iId != idsEnd; ++iId) {

                const QueueItem item (client, *iId);
                if (!releasePixmapsQueue.removeOne(item)) {
                    loadPixmapsQueue.enqueue(item);
                }
            }
        }

        // make sure we have a cache directory for the current theme
        QDir cacheDir(DuiThemeDaemon::systemThemeCacheDirectory());
        if(!cacheDir.exists(currentTheme.value().toString())) {
            if(!cacheDir.mkdir(currentTheme.value().toString())) {
                qFatal("DuiThemeDaemonServer - Could not create cache directory for current theme. No write permissions to %s",
                    qPrintable(cacheDir.absolutePath()));
            }
        }

        if (!loadPixmapsQueue.isEmpty() && !processQueueTimer.isActive())
            processQueueTimer.start();

    } else {
        // theme change failed, so change the theme back also in gconf.
        duiWarning("DuiThemeDaemonServer") << "Could not change theme to" << currentTheme.value().toString();
        currentTheme.set(daemon.currentTheme());
    }
}

void DuiThemeDaemonServer::localeChanged()
{
    QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> > pixmapsToReload;
    daemon.changeLocale(currentLocale.value().toString(), registeredClients.values(), pixmapsToReload);

    QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> >::iterator i = pixmapsToReload.begin();
    QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> >::iterator end = pixmapsToReload.end();
    for (; i != end; ++i) {
        DuiThemeDaemonClient *client = i.key();
        const QList<PixmapIdentifier> &ids = i.value();

        const QList<PixmapIdentifier>::const_iterator idsEnd = ids.end();
        for (QList<PixmapIdentifier>::const_iterator iId = ids.begin(); iId != idsEnd; ++iId) {

            const QueueItem item (client, *iId);
            if (!releasePixmapsQueue.removeOne(item)) {
                loadPixmapsQueue.enqueue(item);
            }
        }
    }
    if (!loadPixmapsQueue.isEmpty() && !processQueueTimer.isActive())
        processQueueTimer.start();
}

void DuiThemeDaemonServer::processOneQueueItem()
{
    if (!loadPixmapsQueue.isEmpty()) {
        const QueueItem item = loadPixmapsQueue.dequeue();
        Qt::HANDLE handle = 0;
        if (daemon.pixmap(item.client, item.pixmapId, handle)) {
            item.client->stream() << Packet(Packet::PixmapUpdatedPacket, item.sequenceNumber,
                                            new PixmapHandle(item.pixmapId, handle));
        } else {
            const QString message =
                QString::fromLatin1("requested pixmap '%1' %2x%3 already acquired by client").arg(
                                    item.pixmapId.imageId,
                                    QString::number(item.pixmapId.size.width()),
                                    QString::number(item.pixmapId.size.height()));
            item.client->stream() << Packet(Packet::ErrorPacket, item.sequenceNumber,
                                            new String(message));
        }
    } else if (!releasePixmapsQueue.isEmpty()) {
        const QueueItem item = releasePixmapsQueue.dequeue();
        if (!daemon.releasePixmap(item.client, item.pixmapId)) {
            const QString message =
                QString::fromLatin1("pixmap to release '%1' %2x%3 not acquired by client").arg(
                                    item.pixmapId.imageId,
                                    QString::number(item.pixmapId.size.width()),
                                    QString::number(item.pixmapId.size.height()));
            item.client->stream() << Packet(Packet::ErrorPacket, item.sequenceNumber,
                                            new String(message));
        }
    }

    if (loadPixmapsQueue.isEmpty() && releasePixmapsQueue.isEmpty()) {
        processQueueTimer.stop();
    }
}

void DuiThemeDaemonServer::pixmapRequested(DuiThemeDaemonClient *client,
                                           const PixmapIdentifier &id, quint64 sequenceNumber)
{
    // if the client has requested a release for this pixmap, we'll remove the
    // release request, and reply with the existing pixmap handle
    const QueueItem item (client, id, sequenceNumber);
    if (releasePixmapsQueue.removeOne(item)) {
        // removeOne succeeds if there was a release request still on queue
        // find the resource for the requested pixmap
        ImageResource *resource = client->pixmaps.value(id);
        // in case the resource is NULL, the pixmap is not loaded (it's not found from the current theme)
        if (!resource) {
            client->stream() << Packet(Packet::PixmapUpdatedPacket, sequenceNumber,
                                       new PixmapHandle(id, 0));
        } else {
#ifndef Q_WS_MAC
            client->stream() << Packet(Packet::PixmapUpdatedPacket, sequenceNumber,
                                       new PixmapHandle(id, resource->pixmapHandle(id.size)));
#endif
        }
    } else {
        // the requested pixmap was not in release queue, so we'll queue the load
        loadPixmapsQueue.enqueue(item);
        if (!processQueueTimer.isActive())
            processQueueTimer.start();
    }
}

void DuiThemeDaemonServer::pixmapReleaseRequested(DuiThemeDaemonClient *client,
                                                  const PixmapIdentifier &id,
                                                  quint64 sequenceNumber)
{
    // if the pixmap request is in queue, we can just remove it from there
    const QueueItem item (client, id, sequenceNumber);
    if (!loadPixmapsQueue.removeOne(item)) {
        // in case the removeOne fails, we need to queue the release request.
        releasePixmapsQueue.enqueue(item);
        if (!processQueueTimer.isActive())
            processQueueTimer.start();
    }
}

void DuiThemeDaemonServer::themeDaemonStatus(DuiThemeDaemonClient *client,
                                             quint64 sequenceNumber) const
{
    QList<ClientInfo> clientList;

    foreach(const DuiThemeDaemonClient * c, registeredClients.values()) {
        ClientInfo info;
        info.name = c->name();
        info.pixmaps = c->pixmaps.keys();

        foreach(const QueueItem &item, loadPixmapsQueue) {
            if (item.client == c)
                info.requestedPixmaps.append(item.pixmapId);
        }
        foreach(const QueueItem &item, releasePixmapsQueue) {
            if (item.client == c)
                info.releasedPixmaps.append(item.pixmapId);
        }

        clientList.append(info);
    }

    client->stream() << Packet(Packet::ThemeDaemonStatusPacket, sequenceNumber,
                               new ClientList(clientList));
}

