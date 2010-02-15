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

#include "duiremotethemedaemon.h"
#include "duiremotethemedaemon_p.h"
#include "duithemedaemon.h"
#include "duidebug.h"
#include "duithemedaemonprotocol.h"
#include <QDir>
#include <QTime>

using namespace Dui::DuiThemeDaemonProtocol;

DuiRemoteThemeDaemon::DuiRemoteThemeDaemon(const QString &applicationName, int timeout) :
    d_ptr(new DuiRemoteThemeDaemonPrivate)
{
    Q_D(DuiRemoteThemeDaemon);
    d->q_ptr = this;

    QObject::connect(&d->socket, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));

    // this blocking behavior could be removed
    if (d->waitForServer(Dui::DuiThemeDaemonProtocol::ServerAddress, timeout)) {
        d->stream.setDevice(&d->socket);

        d->stream << Packet(Packet::RequestRegistrationPacket, new String(applicationName));
        Packet reply = d->waitForPacket(Packet::ThemeChangedPacket);
        if (reply.type() == Packet::ThemeChangedPacket) {
            d->themeInheritanceChain = static_cast<const StringList *>(reply.data())->stringList;
        } else {
            // TODO: print out warning, etc.
        }
    } else {
        duiWarning("DuiRemoteThemeDaemon") << "Failed to connect to theme daemon (IPC)";
    }
}

DuiRemoteThemeDaemon::~DuiRemoteThemeDaemon()
{
    Q_D(DuiRemoteThemeDaemon);

    if (connected()) {
        d->socket.close();
    }
    delete d_ptr;
}

bool DuiRemoteThemeDaemon::connected() const
{
    Q_D(const DuiRemoteThemeDaemon);
    return d->socket.state() == QLocalSocket::ConnectedState;
}

void DuiRemoteThemeDaemon::addDirectoryToPixmapSearchList(const QString &directoryName, Dui::RecursionMode recursive)
{
    Q_D(DuiRemoteThemeDaemon);
    d->stream << Packet(Packet::RequestNewPixmapDirectoryPacket, new StringBool(directoryName, recursive == Dui::Recursive));
}

void DuiRemoteThemeDaemon::clearPixmapSearchList()
{
    Q_D(DuiRemoteThemeDaemon);
    d->stream << Packet(Packet::RequestClearPixmapDirectoriesPacket);
}

bool DuiRemoteThemeDaemonPrivate::waitForServer(const QString &serverAddress, int timeout)
{
    QTime time;
    time.start();

    while (1) {
        // try to connect
        socket.connectToServer(serverAddress);
        if (socket.state() == QLocalSocket::ConnectedState)
            return true;

        // check for timeout
        if (timeout != -1) {
            if (time.elapsed() >= timeout) {
                return false;
            }
        }

        // wait for a while
#ifndef Q_OS_WIN
        sleep(1);
#else
//TODO - this windows.h include somehow does not work.
//#include <windows.h>
//        Sleep(1000);
#endif
    }
}

Packet DuiRemoteThemeDaemonPrivate::waitForPacket(Packet::PacketType type)
{
    Q_Q(DuiRemoteThemeDaemon);

    // send it immediately
    socket.flush();

    QObject::disconnect(&socket, SIGNAL(readyRead()), q, SLOT(connectionDataAvailable()));

    // wait until we get a packet with type
    while (socket.waitForReadyRead(-1)) {
        while (socket.bytesAvailable()) {
            // read one packet
            Packet packet = readOnePacket();
            // check if it was the one we are waiting for
            if (packet.type() == type) {
                // read rest
                QObject::connect(&socket, SIGNAL(readyRead()), q, SLOT(connectionDataAvailable()));
                connectionDataAvailable();
                return packet;
            }
            // if it was not the packet we're waiting for, lets process it
            processOnePacket(packet);
        }
    }

    duiWarning("DuiRemoteThemeDaemon") << "waitForRequest - request timed out.";
    QObject::connect(&socket, SIGNAL(readyRead()), q, SLOT(connectionDataAvailable()));
    return Packet();
}



void DuiRemoteThemeDaemon::pixmapHandleSync(const QString &imageId, const QSize &size)
{
    Q_D(DuiRemoteThemeDaemon);

    PixmapIdentifier id(imageId, size);
    // check if we haven't yet asked for this pixmap
    // if there's no ongoing request, we'll make one
    if (!d->pixmapRequests.contains(id)) {
        Packet packet(Packet::RequestPixmapPacket, new PixmapIdentifier(id));
        d->stream << packet;
    }

    // also mark the request active
    d->pixmapRequests[id] = true;

    while (true) {
        Packet reply = d->waitForPacket(Packet::PixmapUpdatedPacket);
        if (reply.type() == Packet::PixmapUpdatedPacket) {
            // wait succeeded

            // let's process the packet
            d->processOnePacket(reply);

            // is this packet a reply to our request?
            const PixmapHandle *h = static_cast<const PixmapHandle *>(reply.data());
            if (h->identifier.imageId == imageId && h->identifier.size == size) {
                // we've found the correct one
                break;
            }
        } else {
            // TODO: print out warning, etc.
        }
    }
}

void DuiRemoteThemeDaemon::pixmapHandle(const QString &imageId, const QSize &size)
{
    Q_D(DuiRemoteThemeDaemon);

    // make sure we don't have this kind of inactive request ongoing
    PixmapIdentifier id(imageId, size);
    if (!d->pixmapRequests.contains(id)) {
        Packet packet(Packet::RequestPixmapPacket, new PixmapIdentifier(id));
        d->stream << packet;
    } else {
        // we have already requested this pixmap, do not request twice
        Q_ASSERT_X(!d->pixmapRequests[id], "DuiRemoteThemeDaemon", "Requested pixmap, which already exists in DuiTheme cache");
    }

    d->pixmapRequests[id] = true;
}

void DuiRemoteThemeDaemon::releasePixmap(const QString &imageId, const QSize &size)
{
    Q_D(DuiRemoteThemeDaemon);

    PixmapIdentifier id(imageId, size);
    if (d->pixmapRequests.contains(id)) {
        // the pixmap is not yet loaded, mark the request as inactive
        d->pixmapRequests[id] = false;
    } else {
        // the pixmap is already loaded, so send the release request.
        Packet packet(Packet::ReleasePixmapPacket, new PixmapIdentifier(id));
        d->stream << packet;
    }
}


QString DuiRemoteThemeDaemon::currentTheme()
{
    Q_D(DuiRemoteThemeDaemon);
    QDir dir(d->themeInheritanceChain.at(0));
    return dir.dirName();
}

QStringList DuiRemoteThemeDaemon::themeInheritanceChain()
{
    Q_D(DuiRemoteThemeDaemon);
    return d->themeInheritanceChain;
}

bool DuiRemoteThemeDaemon::hasPendingRequests() const
{
    Q_D(const DuiRemoteThemeDaemon);
    return !d->pixmapRequests.isEmpty();
}

Packet DuiRemoteThemeDaemonPrivate::readOnePacket()
{
    Packet packet;
    stream >> packet;
    return packet;
}

void DuiRemoteThemeDaemonPrivate::processOnePacket(const Packet &packet)
{
    // process it according to packet type
    switch (packet.type()) {
    case Packet::PixmapUpdatedPacket:
        pixmapUpdated(*static_cast<const PixmapHandle *>(packet.data()));
        break;

    case Packet::ThemeChangedPacket:
        themeChanged(static_cast<const StringList *>(packet.data())->stringList);
        break;

    default:
        duiDebug("DuiRemoteThemeDaemon") << "Couldn't process packet of type" << packet.type();
        break;
    }
}

void DuiRemoteThemeDaemonPrivate::connectionDataAvailable()
{
    while (socket.bytesAvailable()) {
        processOnePacket(readOnePacket());
    }
}

void DuiRemoteThemeDaemonPrivate::pixmapUpdated(const PixmapHandle &handle)
{
    Q_Q(DuiRemoteThemeDaemon);

    QHash<Dui::DuiThemeDaemonProtocol::PixmapIdentifier, bool>::iterator iter = pixmapRequests.find(handle.identifier);
    if (iter == pixmapRequests.end()) {
        // the pixmap has not been requested, so it's updated by theme daemon (caused by theme change)
        emit q->pixmapChanged(handle.identifier.imageId, handle.identifier.size, handle.pixmapHandle);
        return;
    }

    bool active = *iter;
    pixmapRequests.erase(iter);
    if (active) {
        // request is still active
        emit q->pixmapCreated(handle.identifier.imageId, handle.identifier.size, handle.pixmapHandle);
    } else {
        // request is inactive, release
        // this was requested to be released during the loading phase
        Packet packet(Packet::ReleasePixmapPacket, new PixmapIdentifier(handle.identifier));
        stream << packet;
    }
}

void DuiRemoteThemeDaemonPrivate::themeChanged(const QStringList &themeInheritanceChain)
{
    Q_Q(DuiRemoteThemeDaemon);
    this->themeInheritanceChain = themeInheritanceChain;
    emit q->themeChanged(themeInheritanceChain);
}

#include "moc_duiremotethemedaemon.cpp"
