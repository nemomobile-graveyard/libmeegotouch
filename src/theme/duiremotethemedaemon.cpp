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

#ifndef Q_OS_WIN
# include <unistd.h>
#else
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

using namespace Dui::DuiThemeDaemonProtocol;

DuiRemoteThemeDaemon::DuiRemoteThemeDaemon(const QString &applicationName, int timeout) :
    d_ptr(new DuiRemoteThemeDaemonPrivate)
{
    Q_D(DuiRemoteThemeDaemon);
    d->q_ptr = this;
    d->sequenceCounter = 0;

    QObject::connect(&d->socket, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));

    // this blocking behavior could be removed
    if (d->waitForServer(Dui::DuiThemeDaemonProtocol::ServerAddress, timeout)) {
        d->stream.setDevice(&d->socket);

        const quint64 seq = ++d->sequenceCounter;
        d->stream << Packet(Packet::RequestRegistrationPacket, seq, new String(applicationName));
        Packet reply = d->waitForPacket(seq);
        if (reply.type() == Packet::ThemeChangedPacket) {
            const ThemeChangeInfo* info = static_cast<const ThemeChangeInfo*>(reply.data());
            d->themeInheritanceChain = info->themeInheritance;
            d->themeLibraryNames = info->themeLibraryNames;
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

void DuiRemoteThemeDaemon::addDirectoryToPixmapSearchList(const QString &directoryName,
                                                          Dui::RecursionMode recursive)
{
    Q_D(DuiRemoteThemeDaemon);
    d->stream << Packet(Packet::RequestNewPixmapDirectoryPacket, ++d->sequenceCounter,
                        new StringBool(directoryName, recursive == Dui::Recursive));
}

void DuiRemoteThemeDaemon::clearPixmapSearchList()
{
    Q_D(DuiRemoteThemeDaemon);
    d->stream << Packet(Packet::RequestClearPixmapDirectoriesPacket, ++d->sequenceCounter);
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
        Sleep(1000);
#endif
    }
}

Packet DuiRemoteThemeDaemonPrivate::waitForPacket(quint64 sequenceNumber)
{
    Q_Q(DuiRemoteThemeDaemon);

    // send it immediately
    socket.flush();

    QObject::disconnect(&socket, SIGNAL(readyRead()), q, SLOT(connectionDataAvailable()));

    // wait until we get a packet with type
    while (socket.waitForReadyRead(-1)) {
        while (socket.bytesAvailable()) {
            // read one packet
            const Packet packet = readOnePacket();
            // check if it was the one we are waiting for
            if (packet.sequenceNumber() == sequenceNumber) {
                // read rest
                QObject::connect(&socket, SIGNAL(readyRead()), q, SLOT(connectionDataAvailable()));
                connectionDataAvailable();
                return packet;
            }
            // if it was not the packet we're waiting for, lets process it
            processOnePacket(packet);
        }
    }

    duiWarning("DuiRemoteThemeDaemon") << "waitForPacket: connection broken";
    QObject::connect(&socket, SIGNAL(readyRead()), q, SLOT(connectionDataAvailable()));
    return Packet();
}

quint64 DuiRemoteThemeDaemonPrivate::requestPixmap(const QString &imageId, const QSize &size)
{
    const PixmapIdentifier id (imageId, size);
    // check if we haven't yet asked for this pixmap
    // if there's no ongoing request, we'll make one
    const QHash<PixmapIdentifier, quint64>::const_iterator req = pixmapRequests.constFind(id);
    quint64 sequenceNumber;
    if (req != pixmapRequests.constEnd()) {
        sequenceNumber = req.value();
        duiWarning("DuiRemoteThemeDaemon") << "requested pixmap which already exists in cache";
    }
    else {
        sequenceNumber = ++sequenceCounter;
        stream << Packet(Packet::RequestPixmapPacket, sequenceNumber, new PixmapIdentifier(id));
        // remember sequence number of ongoing request
        pixmapRequests.insert(id, sequenceNumber);
    }
    return sequenceNumber;
}

void DuiRemoteThemeDaemon::pixmapHandleSync(const QString &imageId, const QSize &size)
{
    Q_D(DuiRemoteThemeDaemon);

    const quint64 sequenceNumber = d->requestPixmap(imageId, size);
    const Packet reply = d->waitForPacket(sequenceNumber);

    d->processOnePacket(reply);
}

void DuiRemoteThemeDaemon::pixmapHandle(const QString &imageId, const QSize &size)
{
    Q_D(DuiRemoteThemeDaemon);

    d->requestPixmap(imageId, size);
}

void DuiRemoteThemeDaemon::releasePixmap(const QString &imageId, const QSize &size)
{
    Q_D(DuiRemoteThemeDaemon);

    PixmapIdentifier *const id = new PixmapIdentifier(imageId, size);

    // If a request for this pixmap is still in the queue, forget about it
    // so that subsequent calls to pixmapHandle() will issue a new request.
    d->pixmapRequests.remove(*id);

    d->stream << Packet(Packet::ReleasePixmapPacket, ++d->sequenceCounter,
                        id /*callee assumes ownership*/);
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

QStringList DuiRemoteThemeDaemon::themeLibraryNames()
{
    Q_D(DuiRemoteThemeDaemon);
    return d->themeLibraryNames;
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

    case Packet::ThemeChangedPacket: {
        const ThemeChangeInfo* info = static_cast<const ThemeChangeInfo*>(packet.data());
        themeChanged(info->themeInheritance, info->themeLibraryNames);
        break;
    }
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

    pixmapRequests.remove(handle.identifier);
    // The pixmap may have been updated either in response to a request or
    // due to a theme change.  It may have gone already, if a release
    // request was processed by the server in the meantime.  The recipient
    // of the signal must be able to handle such a situation gracefully.
    emit q->pixmapChanged(handle.identifier.imageId, handle.identifier.size, handle.pixmapHandle);
}

void DuiRemoteThemeDaemonPrivate::themeChanged(const QStringList &themeInheritanceChain, const QStringList &themeLibraryNames)
{
    Q_Q(DuiRemoteThemeDaemon);
    this->themeInheritanceChain = themeInheritanceChain;
    this->themeLibraryNames = themeLibraryNames;
    emit q->themeChanged(themeInheritanceChain, themeLibraryNames);
}

#include "moc_duiremotethemedaemon.cpp"
