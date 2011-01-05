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

#include <sys/types.h>
#include <sys/socket.h>

#include "mthemedaemonserver.h"
#include <theme/mthemedaemonclient.h>
#include <MDebug>
#include <MNamespace>

#include <QLocalSocket>
#include <QDir>
#include <QTimer>
#include <QSettings>
#include <QApplication>

using namespace M::MThemeDaemonProtocol;

const int THEME_CHANGE_TIMEOUT = 3000;

int MThemeDaemonServer::sighupFd[2];
int MThemeDaemonServer::sigtermFd[2];
int MThemeDaemonServer::sigintFd[2];

MThemeDaemonServer::MThemeDaemonServer(const QString &serverAddress) :
    daemon(MThemeDaemon::RemoteDaemon),
    currentTheme("/meegotouch/theme/name"),
    currentLocale("/meegotouch/i18n/language"),
    defaultTheme(M_THEME_DEFAULT),
    delayedThemeChange(false),
    slowDown(false),
    sequenceCounter(0)
{
    // Create sockets and notifiers to handle unix signals
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sighupFd))
        qFatal("Couldn't create HUP socketpair");
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
        qFatal("Couldn't create TERM socketpair");
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigintFd))
        qFatal("Couldn't create INT socketpair");

    snHup = new QSocketNotifier(sighupFd[1], QSocketNotifier::Read, this);
    connect(snHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
    snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
    connect(snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));
    snInt = new QSocketNotifier(sigintFd[1], QSocketNotifier::Read, this);
    connect(snInt, SIGNAL(activated(int)), this, SLOT(handleSigInt()));

    QString filename = M_INSTALL_SYSCONFDIR "/meegotouch/themedaemonpriorities.conf";
    loadPriorities(filename);

    // 1) make sure that gconf has some value for the current theme
    if (currentTheme.value().isNull() || currentTheme.value().toString().isEmpty() )
        currentTheme.set(defaultTheme);

    // 2) activate the current theme
    if (!daemon.activateTheme(currentTheme.value().toString(), currentLocale.value().toString(), QList<MThemeDaemonClient *>(), pixmapsToDeleteWhenThemeChangeHasCompleted)) {
        // could not activate current theme, change to default theme
        if (!daemon.activateTheme(defaultTheme, currentLocale.value().toString(), QList<MThemeDaemonClient *>(), pixmapsToDeleteWhenThemeChangeHasCompleted)) {
            qFatal("MThemeDaemonServer - Could not find themes, aborting!");
        }
    }
    Q_ASSERT(pixmapsToDeleteWhenThemeChangeHasCompleted.isEmpty());

    // 3) make sure we're notified if locale or theme changes
    connect(&currentTheme, SIGNAL(valueChanged()), SLOT(themeChanged()));
    connect(&currentLocale, SIGNAL(valueChanged()), SLOT(localeChanged()));

    // 4) make sure we have a themedaemon directory in /var/cache/meegotouch/
    if( !createCacheDir(MThemeDaemon::systemThemeCacheDirectory()) ) {
        qCritical("Will continue without cache.");
    }

    // 5) make sure we have a cache directory for the current theme
    if (!createCacheDir(MThemeDaemon::systemThemeCacheDirectory() + QDir::separator() + daemon.currentTheme())) {
        qCritical("Will continue without cache for the theme '%s'", qPrintable(daemon.currentTheme()));
    }

    // start socket server for client registeration
    // first remove the old one, if there's such
    const QString address = serverAddress.isEmpty() ? M::MThemeDaemonProtocol::ServerAddress : serverAddress;

    QLocalServer::removeServer(address);
    connect(&server, SIGNAL(newConnection()), SLOT(clientConnected()));
    if (!server.listen(address)) {
        mWarning("MThemeDaemonServer") << "Failed to start socket server.";
    }

    processQueueTimer.setInterval(0);
    processQueueTimer.setSingleShot(false);
    connect(&processQueueTimer, SIGNAL(timeout()), SLOT(processOneQueueItem()));
    connect(&daemon.mostUsedPixmaps, SIGNAL(mostUsedPixmapsChanged(M::MThemeDaemonProtocol::MostUsedPixmaps)), this, SLOT(handleUpdatedMostUsedPixmaps(M::MThemeDaemonProtocol::MostUsedPixmaps)));
}

MThemeDaemonServer::~MThemeDaemonServer()
{
    processQueueTimer.stop();

    // close socket server
    server.close();

    // remove all registered clients
    while (registeredClients.count() > 0) {
        MThemeDaemonClient *client = registeredClients.begin().value();
        QLocalSocket *socket = registeredClients.begin().key();
        socket->disconnect(this);
        daemon.removeClient(client);
        delete client;
        registeredClients.erase(registeredClients.begin());
    }
}

void MThemeDaemonServer::loadPriorities(const QString& filename)
{
    priorityForegroundApplication = 100;
    QSettings settings(filename, QSettings::IniFormat);
    if(settings.status() != QSettings::NoError) {
        return;
    }

    priorityForegroundApplication = settings.value("ForegroundApplication/priority", priorityForegroundApplication).toInt();
}

// gets called when a new client(socket) connects to the daemon
void MThemeDaemonServer::clientConnected()
{
    while (server.hasPendingConnections()) {
        QLocalSocket *socket = server.nextPendingConnection();
        QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(clientDataAvailable()));
    }
}

// gets called when any client has disconnected
void MThemeDaemonServer::clientDisconnected()
{
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    if (socket) {
        MThemeDaemonClient *client = registeredClients.value(socket, NULL);
        if (client) {
            daemon.removeClient(client);
            registeredClients.remove(socket);

            // remove all queued pixmap requests
            QMap<qint32, QQueue<QueueItem> >::iterator prioIter = loadPixmapsQueue.begin();
            while (prioIter != loadPixmapsQueue.end()) {
                QQueue<QueueItem>::iterator queueIter = prioIter->begin();
                while (queueIter != prioIter->end()) {
                    if (queueIter->client == client) {
                        queueIter = prioIter->erase(queueIter);
                    } else {
                        ++queueIter;
                    }
                }
                if (prioIter->isEmpty()) {
                    prioIter = loadPixmapsQueue.erase(prioIter);
                } else {
                    ++prioIter;
                }
            }
            // remove all queued pixmap releases
            QMutableListIterator<QueueItem> pi = releasePixmapsQueue;
            while (pi.hasNext()) {
                if (pi.next().client == client)
                    pi.remove();
            }

            // remove the client from themechange list and delete the client data
            bool removed = clientsThatHaveNotYetAppliedThemeChange.removeOne(client);
            delete client;
            
            // finalize the theme change now if the disconnected client was 
            //the last who had not replied to theme change request
            if( removed && clientsThatHaveNotYetAppliedThemeChange.isEmpty() ) {
                mWarning("MThemeDaemonServer") << "Finalize theme change due to disconnection of last pending application.";
                finalizeThemeChange();
            }
        }
        socket->deleteLater();
    }
}


// gets called when any client has sent data over socket
void MThemeDaemonServer::clientDataAvailable()
{
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    if (!socket) {
        return;
    }

    // has the client registered?
    MThemeDaemonClient *client = registeredClients.value(socket, NULL);
    if (!client) {
        // create a temporary data stream to read from the socket
        QDataStream stream(socket);
        stream.setVersion(QDataStream::Qt_4_6);

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
                const QString clientName = static_cast<const String *>(packet.data())->string;
                if (clientName.isEmpty()) {
                    stream << Packet(Packet::ErrorPacket, packet.sequenceNumber(),
                                     new String("The registration packet must provide a name for the client"));
                } else {
                    client = new MThemeDaemonClient(socket, clientName, daemon.themeInheritanceChain());
                    registeredClients.insert(socket, client);
                    daemon.addClient(client);
                    client->stream() << Packet(Packet::ThemeChangedPacket, packet.sequenceNumber(),
                                               new ThemeChangeInfo(daemon.themeInheritanceChain(), daemon.themeLibraryNames()));
                    client->stream() << Packet(Packet::MostUsedPixmapsPacket, ++sequenceCounter,
                                               new MostUsedPixmaps(daemon.mostUsedPixmaps.mostUsedPixmapHandles(), QList<PixmapIdentifier>()));
                    break;
                }
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
            // client re-registered: change the name of the client
            if (client->reinit(static_cast<const String *>(packet.data())->string, daemon.themeInheritanceChain())) {
                client->stream() << Packet(Packet::ThemeChangedPacket, packet.sequenceNumber(),
                    new ThemeChangeInfo(daemon.themeInheritanceChain(), daemon.themeLibraryNames()));
            }
        } break;

        case Packet::PixmapUsedPacket: {
            // client wants to use a pixmap
            const PixmapIdentifier *id = static_cast<const PixmapIdentifier *>(packet.data());
            pixmapUsed(client, *id, packet.sequenceNumber());
        } break;

        case Packet::RequestPixmapPacket: {
            // client requested a pixmap
            const RequestedPixmap *pixmap = static_cast<const RequestedPixmap *>(packet.data());
            pixmapRequested(client, pixmap->id, pixmap->priority, packet.sequenceNumber());
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
            client->addCustomImageDirectory(sb->string, sb->b ? M::Recursive : M::NonRecursive);
        } break;

        case Packet::ThemeChangeAppliedPacket: {
            const Number *number = static_cast<const Number *>(packet.data());
            themeChangeApplied(client, number->value, packet.sequenceNumber());
        } break;

        case Packet::AckMostUsedPixmapsPacket: {
            ackMostUsedPixmaps(client, packet.sequenceNumber());
        } break;

        case Packet::QueryThemeDaemonStatusPacket: {
            themeDaemonStatus(client, packet.sequenceNumber());
        } break;

        default: {
            mWarning("MThemeDaemonServer") << "unknown packet received:" << packet.type();
        } break;
        }
    }
}

void MThemeDaemonServer::setSlowDown(int slowDown)
{
    this->slowDown = slowDown;
}

void MThemeDaemonServer::themeChanged(bool forceReload)
{
    if (!forceReload && daemon.currentTheme() == currentTheme.value().toString())
        return;

    //if previous theme change is not finished yet we need wait it to complete before changing to new one
    if(!clientsThatHaveNotYetAppliedThemeChange.isEmpty()) {
        mWarning("MThemeDaemonServer") << "Will not change theme while another theme change is still ongoing. Change requests still pending:" << clientsThatHaveNotYetAppliedThemeChange.size();
        QStringList list;
        for (int i = 0; i < clientsThatHaveNotYetAppliedThemeChange.size(); ++i) {
            list << clientsThatHaveNotYetAppliedThemeChange.at(i)->name();
        }        
        mWarning("MThemeDaemonServer") << list;
        delayedThemeChange = true;
        QTimer::singleShot(THEME_CHANGE_TIMEOUT, this, SLOT(themeChangeTimeout()));
        return;
    }

    bool changeOk = daemon.activateTheme(currentTheme.value().toString(), currentLocale.value().toString(), registeredClients.values(), pixmapsToDeleteWhenThemeChangeHasCompleted, forceReload);
    if(!changeOk) {
        mWarning("MThemeDaemonServer") << "Could not change theme to" << currentTheme.value().toString() << ". Falling back to default theme " << defaultTheme;
        if( daemon.currentTheme() == defaultTheme )
            return;
        changeOk = daemon.activateTheme(defaultTheme, currentLocale.value().toString(), registeredClients.values(), pixmapsToDeleteWhenThemeChangeHasCompleted);
    }

    if (changeOk) {
        // theme change succeeded, let's inform all clients + add the pixmaps to load-list
        Packet themeChangedPacket(Packet::ThemeChangedPacket, 0, new ThemeChangeInfo(daemon.themeInheritanceChain(), daemon.themeLibraryNames()));

        foreach(MThemeDaemonClient *client, registeredClients) {
            // Do not send a theme change packet to the booster, as it will not reply
            if (client->name() != QLatin1String("componentcache_pre_initialized_mapplication")) {
                clientsThatHaveNotYetAppliedThemeChange.append(client);
                client->stream() << themeChangedPacket;
                client->pixmaps.clear();
            }
        }

        // make sure we have a cache directory for the current theme
        if( !createCacheDir(MThemeDaemon::systemThemeCacheDirectory() + QDir::separator() + daemon.currentTheme()) )
            qFatal("MThemeDaemonServer - Failed to create theme specific cache directory.");        

        if (!loadPixmapsQueue.isEmpty() && !processQueueTimer.isActive())
            processQueueTimer.start();

    } else {
        // could not change to default theme, something seriously wrong now nothing we can do
        qFatal("MThemeDaemonServer - Could not fall back to default theme.");
    }
}

void MThemeDaemonServer::themeChangeTimeout()
{
    //check if some applications have not yet replied to theme 
    //change request, output some info and clear the list
    if(!clientsThatHaveNotYetAppliedThemeChange.isEmpty()) {
        mWarning("MThemeDaemonServer") << "Theme change timeout. Change requests still pending:" << clientsThatHaveNotYetAppliedThemeChange.size();
        QStringList list;
        for (int i = 0; i < clientsThatHaveNotYetAppliedThemeChange.size(); ++i) {
            list << clientsThatHaveNotYetAppliedThemeChange.at(i)->name();
        }        
        mWarning("MThemeDaemonServer") << list;        
        clientsThatHaveNotYetAppliedThemeChange.clear();
    }

    mWarning("MThemeDaemonServer") << "Finalize theme change due to timeout.";
    finalizeThemeChange();
}

void MThemeDaemonServer::localeChanged()
{
    QHash<MThemeDaemonClient *, QList<PixmapIdentifier> > pixmapsToReload;
    daemon.changeLocale(currentLocale.value().toString(), registeredClients.values(), pixmapsToReload);

    QHash<MThemeDaemonClient *, QList<PixmapIdentifier> >::iterator i = pixmapsToReload.begin();
    QHash<MThemeDaemonClient *, QList<PixmapIdentifier> >::iterator end = pixmapsToReload.end();
    for (; i != end; ++i) {
        MThemeDaemonClient *client = i.key();
        const QList<PixmapIdentifier> &ids = i.value();

        const QList<PixmapIdentifier>::const_iterator idsEnd = ids.end();
        for (QList<PixmapIdentifier>::const_iterator iId = ids.begin(); iId != idsEnd; ++iId) {

            const QueueItem item (client, *iId);
            if (!releasePixmapsQueue.removeOne(item)) {
                // we do not know the state of the individual clients -- using same priority
                // for all of them
                loadPixmapsQueue[50].enqueue(item);
            }
        }
    }
    if (!loadPixmapsQueue.isEmpty() && !processQueueTimer.isActive())
        processQueueTimer.start();
}

void MThemeDaemonServer::processOneQueueItem()
{
    if (!loadPixmapsQueue.isEmpty()) {
        // all items requested by a foreground application will be processed immediately,
        // others one my one
        while (true) {
            QMap<qint32, QQueue<QueueItem> >::iterator iter = loadPixmapsQueue.end() - 1;
            const QueueItem item = iter->dequeue();
            if (iter->isEmpty()) {
                loadPixmapsQueue.erase(iter);
            }

            MPixmapHandle handle;
            if (daemon.pixmap(item.client, item.pixmapId, &handle)) {
                if (slowDown > 0)
                    usleep(slowDown);
                item.client->stream() << Packet(Packet::PixmapUpdatedPacket, item.sequenceNumber,
                                                new PixmapHandle(item.pixmapId, handle));
            } else {
                const QString message =
                        QString::fromLatin1("requested pixmap '%1' QSize(%2, %3) already acquired by client").arg(
                                item.pixmapId.imageId,
                                QString::number(item.pixmapId.size.width()),
                                QString::number(item.pixmapId.size.height()));
                item.client->stream() << Packet(Packet::ErrorPacket, item.sequenceNumber,
                                                new String(message));
            }

            if (loadPixmapsQueue.isEmpty()) {
                break;
            } else if ((loadPixmapsQueue.end() - 1).key() < priorityForegroundApplication) {
                break;
            }
        }
    } else if (!releasePixmapsQueue.isEmpty()) {
        const QueueItem item = releasePixmapsQueue.dequeue();
        if (!daemon.releasePixmap(item.client, item.pixmapId)) {
            const QString message =
                QString::fromLatin1("pixmap to release '%1' QSize(%2, %3) not acquired by client").arg(
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

void MThemeDaemonServer::handleUpdatedMostUsedPixmaps(const M::MThemeDaemonProtocol::MostUsedPixmaps& mostUsed)
{
    quint64 sequenceNumber = ++sequenceCounter;

    foreach(MThemeDaemonClient * c, registeredClients) {
        c->stream() << Packet(Packet::MostUsedPixmapsPacket, sequenceNumber, new MostUsedPixmaps(mostUsed));
        if (!mostUsed.removedIdentifiers.empty()) {
            clientsThatHaveNotYetUpdatedMostUsed[sequenceNumber].append(c);
        }
    }
    if (!mostUsed.removedIdentifiers.empty()) {
        pixmapsToDeleteWhenUpdatedMostUsed[sequenceNumber] = mostUsed.removedIdentifiers;
    }
}

void MThemeDaemonServer::pixmapUsed(MThemeDaemonClient *client,
                                         const PixmapIdentifier &id, quint64 sequenceNumber)
{
    Q_UNUSED(sequenceNumber)
    // if the client has requested a release for this pixmap, we'll remove the
    // release request, otherwise we increase the reference count
    const QueueItem item (client, id, sequenceNumber);
    if (!releasePixmapsQueue.removeOne(item)) {
        MPixmapHandle handle;
        daemon.pixmap(item.client, item.pixmapId, &handle);
    }
}

void MThemeDaemonServer::pixmapRequested(MThemeDaemonClient *client,
                                           const PixmapIdentifier &id, qint32 priority,
                                           quint64 sequenceNumber)
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
                                       new PixmapHandle(id, MPixmapHandle()));
        } else {
#ifndef Q_WS_MAC
            client->stream() << Packet(Packet::PixmapUpdatedPacket, sequenceNumber,
                                       new PixmapHandle(id, resource->pixmapHandle(id.size)));
#endif
        }
    } else {
        loadPixmapsQueue[priority].enqueue(item);
        if (!processQueueTimer.isActive())
            processQueueTimer.start();
    }
}

void MThemeDaemonServer::pixmapReleaseRequested(MThemeDaemonClient *client,
                                                  const PixmapIdentifier &id,
                                                  quint64 sequenceNumber)
{
    // if the pixmap request is in queue, we can just remove it from there
    const QueueItem item (client, id, sequenceNumber);

    if (client->pixmapsToReload.contains(id)) {
        // during a theme change clients will free all but the ones they
        // continue to use. we need to keep these and update them manually
        client->pixmapsToReload.removeOne(id);
    }

    bool removedFromLoadList = false;
    QMap<qint32, QQueue<QueueItem> >::iterator iter = loadPixmapsQueue.begin();
    while (iter != loadPixmapsQueue.end()) {
        if (iter->removeOne(item)) {
            removedFromLoadList = true;
            if (iter->isEmpty()) {
                loadPixmapsQueue.erase(iter);
            }
            break;
        }
        ++iter;
    }

    if (!removedFromLoadList) {
        if (!client->pixmaps.contains(id)) {
            // ignore request if client has no handle of this pixmap
            // happens for example during theme change
            return;
        }
        // we need to queue the release request.
        releasePixmapsQueue.enqueue(item);
        if (!processQueueTimer.isActive())
            processQueueTimer.start();
    }
}

void MThemeDaemonServer::themeChangeApplied(MThemeDaemonClient *client, qint32 priority,
                                             quint64 sequenceNumber)
{
    Q_UNUSED(sequenceNumber);

    if(!clientsThatHaveNotYetAppliedThemeChange.removeOne(client)) {
        mWarning("MThemeDaemonServer") << "Client" << client->name() << "has already sent theme change applied packet!";
        return;
    }

    // reload all pixmaps which have not been freed by the client during the theme change
    // the client will continue to use the pixmap and we need to manually reload it
    while (!client->pixmapsToReload.isEmpty()) {
        PixmapIdentifier id = client->pixmapsToReload.takeLast();
        const QueueItem item (client, id);
        loadPixmapsQueue[priority].enqueue(item);
    }

    if(clientsThatHaveNotYetAppliedThemeChange.isEmpty()) {
        finalizeThemeChange();
    }
}

void MThemeDaemonServer::ackMostUsedPixmaps(MThemeDaemonClient *client,
                                            quint64 sequenceNumber)
{
    if(!clientsThatHaveNotYetUpdatedMostUsed[sequenceNumber].removeOne(client)) {
        mWarning("MThemeDaemonServer") << "Client" << client->name() << "has already acked most used pixmaps packet!";
        return;
    }

    if (clientsThatHaveNotYetUpdatedMostUsed.empty()) {
        foreach(const PixmapIdentifier& id, pixmapsToDeleteWhenUpdatedMostUsed[sequenceNumber]) {
            pixmapReleaseRequested(client, id, sequenceNumber);
        }
    }
}

void MThemeDaemonServer::themeDaemonStatus(MThemeDaemonClient *client,
                                             quint64 sequenceNumber) const
{
    QList<ClientInfo> clientList;

    foreach(const MThemeDaemonClient * c, registeredClients.values()) {
        ClientInfo info;
        info.name = c->name();
        info.pixmaps = c->pixmaps.keys();
        foreach (const QQueue<QueueItem> &queue, loadPixmapsQueue) {
            foreach(const QueueItem &item, queue) {
                if (item.client == c)
                    info.requestedPixmaps.append(item.pixmapId);
            }
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

void MThemeDaemonServer::finalizeThemeChange()
{
    //make sure all the clients have applied to the theme change request
    if( clientsThatHaveNotYetAppliedThemeChange.isEmpty() ) {

        //release the old pixmaps
        qDeleteAll(pixmapsToDeleteWhenThemeChangeHasCompleted);
        pixmapsToDeleteWhenThemeChangeHasCompleted.clear();

        //inform everyone that theme change has been completed
        Packet themeChangeFinishedPacket(Packet::ThemeChangeCompletedPacket, 0);
        foreach(MThemeDaemonClient * c, registeredClients.values()) {
            c->themeChangeApplied();
            c->stream() << themeChangeFinishedPacket;
        }

        //if another theme change was already requested, start the change now
        if( delayedThemeChange ) {
            mWarning("MThemeDaemonServer") << "Starting delayed theme change.";
            delayedThemeChange = false;
            themeChanged();
        }
    }
}

bool MThemeDaemonServer::createCacheDir(const QString& path)
{
    //make sure that directory exists and we have write access to it
    QFileInfo fileInfo(path);
    if( fileInfo.exists() ) {
        if( fileInfo.isDir() ) {
            if( !fileInfo.isWritable() || !fileInfo.isReadable() ) {
                qCritical("Cannot access the cache directory %s. Permission denied.", qPrintable(fileInfo.absoluteFilePath()));
                return false;
            }
        } else {
            //TODO We could destroy the file and create dir
            qCritical("Path %s is not a directory.", qPrintable(fileInfo.absoluteFilePath()));
            return false;
        }
    }
    //cache dir did not exist, try to create it
    else {
        QDir cacheDir(path);
        if(!cacheDir.mkpath(path)) {
            qCritical("Could not create cache directory %s", qPrintable(cacheDir.absolutePath()));
            return false;
        }
    }
    return true;
}

void MThemeDaemonServer::hupSignalHandler(int)
{
    char a = 1;
    ssize_t writtenBytes = ::write(sighupFd[0], &a, sizeof(a));
    Q_UNUSED(writtenBytes);
}

void MThemeDaemonServer::termSignalHandler(int)
{
    char a = 1;
    ssize_t writtenBytes = ::write(sigtermFd[0], &a, sizeof(a));
    Q_UNUSED(writtenBytes);
}

void MThemeDaemonServer::intSignalHandler(int)
{
    char a = 1;
    ssize_t writtenBytes = ::write(sigintFd[0], &a, sizeof(a));
    Q_UNUSED(writtenBytes);
}

void MThemeDaemonServer::handleSigTerm()
{
    snTerm->setEnabled(false);
    char tmp;
    ssize_t readBytes = ::read(sigtermFd[1], &tmp, sizeof(tmp));
    Q_UNUSED(readBytes);

    // kill the daemon so that it can save it's current state (caches, refcounts, etc)
    qApp->quit();

    snTerm->setEnabled(true);
}

void MThemeDaemonServer::handleSigHup()
{
    snHup->setEnabled(false);
    char tmp;
    ssize_t readBytes = ::read(sighupFd[1], &tmp, sizeof(tmp));
    Q_UNUSED(readBytes);

    themeChanged(true);

    snHup->setEnabled(true);
}

void MThemeDaemonServer::handleSigInt()
{
    snInt->setEnabled(false);
    char tmp;
    ssize_t readBytes = ::read(sigintFd[1], &tmp, sizeof(tmp));
    Q_UNUSED(readBytes);

    // kill the daemon so that it can save it's current state (caches, refcounts, etc)
    qApp->quit();

    snInt->setEnabled(true);
}
