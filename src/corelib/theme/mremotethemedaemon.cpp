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

#include "mremotethemedaemon.h"
#include "mremotethemedaemon_p.h"
#include "mthemedaemon.h"
#include "mdebug.h"
#include "mthemedaemonprotocol.h"
#include "mapplication.h"
#include "mwindow.h"
#include <QDir>
#include <QElapsedTimer>
#include <QTimer>
#include <QSettings>

#ifndef Q_OS_WIN
# include <unistd.h>
#else
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

using namespace M::MThemeDaemonProtocol;

MRemoteThemeDaemon::MRemoteThemeDaemon(const QString &applicationName, int timeout, const QString &serverAddress) :
    d_ptr(new MRemoteThemeDaemonPrivate)
{
    Q_D(MRemoteThemeDaemon);
    d->q_ptr = this;
    d->sequenceCounter = 0;

    d->stream.setVersion(QDataStream::Qt_4_6);
    QObject::connect(&d->socket, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));

    // this blocking behavior could be removed
    const QString address = serverAddress.isEmpty() ? M::MThemeDaemonProtocol::ServerAddress : serverAddress;
    if (d->waitForServer(address, timeout)) {
        d->stream.setDevice(&d->socket);
        d->negotiateProtocolVersion();
        registerApplicationName(applicationName);
    } else {
        mWarning("MRemoteThemeDaemon") << "Failed to connect to theme daemon (IPC)";
    }

    QString filename = M_INSTALL_SYSCONFDIR "/meegotouch/themedaemonpriorities.conf";
    d->loadThemeDaemonPriorities(filename);
}

void MRemoteThemeDaemon::registerApplicationName(const QString &applicationName)
{
    Q_D(MRemoteThemeDaemon);
    d->applicationName = applicationName;
    const quint64 seq = ++d->sequenceCounter;
    d->stream << Packet(Packet::RequestRegistrationPacket, seq, new String(applicationName));
    Packet reply = d->waitForPacket(seq);
    if (reply.type() == Packet::ThemeChangedPacket) {
        const ThemeChangeInfo* info = static_cast<const ThemeChangeInfo*>(reply.data());
        d->themeInheritanceChain = info->themeInheritance;
        d->themeLibraryNames = info->themeLibraryNames;
    } else {
        d->handleUnexpectedPacket(reply);
    }
}

MRemoteThemeDaemon::~MRemoteThemeDaemon()
{
    Q_D(MRemoteThemeDaemon);

    if (connected()) {
        d->socket.close();
    }
    delete d_ptr;
}

bool MRemoteThemeDaemon::connected() const
{
    Q_D(const MRemoteThemeDaemon);
    return d->socket.state() == QLocalSocket::ConnectedState;
}

void MRemoteThemeDaemon::addDirectoryToPixmapSearchList(const QString &directoryName,
                                                          M::RecursionMode recursive)
{
    Q_D(MRemoteThemeDaemon);
    d->stream << Packet(Packet::RequestNewPixmapDirectoryPacket, ++d->sequenceCounter,
                        new StringBool(directoryName, recursive == M::Recursive));
}

void MRemoteThemeDaemon::clearPixmapSearchList()
{
    Q_D(MRemoteThemeDaemon);
    d->stream << Packet(Packet::RequestClearPixmapDirectoriesPacket, ++d->sequenceCounter);
}

bool MRemoteThemeDaemonPrivate::waitForServer(const QString &serverAddress, int timeout)
{
    QElapsedTimer timer;
    timer.start();

    while (1) {
        // try to connect
        socket.connectToServer(serverAddress);
        if (socket.state() == QLocalSocket::ConnectedState)
            return true;

        // check for timeout
        if (timeout != -1 && timeout <= timer.elapsed()) {
            return false;
        }

        qCritical() << qApp->applicationFilePath() << "Failed to connect to remote themedaemon. Retrying in one second.";

        // wait for a while
#ifndef Q_OS_WIN
        sleep(1);
#else
        Sleep(1000);
#endif
    }
}

void MRemoteThemeDaemonPrivate::negotiateProtocolVersion() {
    const quint64 seq = ++sequenceCounter;
    stream << Packet(Packet::ProtocolVersionPacket, seq, new Number(M::MThemeDaemonProtocol::protocolVersion));
    Packet reply = waitForPacket(seq);
    if (reply.type() == Packet::ProtocolVersionPacket) {
        const Number* protocolVersion = static_cast<const Number*>(reply.data());
        if (protocolVersion->value != M::MThemeDaemonProtocol::protocolVersion) {
            qCritical("Running themedaemon and this client do not support the same protocol version.\n"
                      "Maybe you need to restart the themedaemon server or to upgrade your installation.\n"
                      "Exiting.");
            exit(EXIT_FAILURE);
        }
    } else {
        handleUnexpectedPacket(reply);
    }
}

void MRemoteThemeDaemonPrivate::handleUnexpectedPacket(const Packet& packet) {
    if (packet.type() == Packet::ErrorPacket) {
        const String *errorString = static_cast<const String*>(packet.data());
        qCritical() << "Themedaemon replied with error packet:\n" <<
                       errorString->string << "\nExiting.";
        exit(EXIT_FAILURE);
    } else {
        qCritical() << "Received unexpected packet" << packet.type() << "from themedaemon. Exiting.";
        exit(EXIT_FAILURE);
    }
}

Packet MRemoteThemeDaemonPrivate::waitForPacket(quint64 sequenceNumber)
{
    Q_Q(MRemoteThemeDaemon);

    // send it immediately
    socket.flush();

    QObject::disconnect(&socket, SIGNAL(readyRead()), q, SLOT(connectionDataAvailable()));

    // wait until we get a packet with type
    while (socket.waitForReadyRead(3000)) {
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

    mWarning("MRemoteThemeDaemon") << "waitForPacket: connection timeout when waiting for packet " << sequenceNumber;
    QObject::connect(&socket, SIGNAL(readyRead()), q, SLOT(connectionDataAvailable()));
    return Packet();
}

quint64 MRemoteThemeDaemonPrivate::requestPixmap(const QString &imageId, const QSize &size)
{
    const PixmapIdentifier id (imageId, size);
    // check if we haven't yet asked for this pixmap
    // if there's no ongoing request, we'll make one
    const QHash<PixmapIdentifier, quint64>::const_iterator req = pixmapRequests.constFind(id);
    quint64 sequenceNumber;
    if (req != pixmapRequests.constEnd()) {
        sequenceNumber = req.value();
        mWarning("MRemoteThemeDaemon") << "requested pixmap which already exists in the request queue.";
    }
    else {
        sequenceNumber = ++sequenceCounter;

        stream << Packet(Packet::RequestPixmapPacket, sequenceNumber, new RequestedPixmap(id, priority()));
        // remember sequence number of ongoing request
        pixmapRequests.insert(id, sequenceNumber);
    }
    return sequenceNumber;
}

void MRemoteThemeDaemonPrivate::addMostUsedPixmaps(const QList<PixmapHandle>& handles)
{
    QList<PixmapHandle>::const_iterator it = handles.begin();
    while (it != handles.end()) {
        if (!mostUsedPixmaps.contains(it->identifier)) {
            mostUsedPixmaps[it->identifier] = it->pixmapHandle;
        }
        ++it;
    }
}

void MRemoteThemeDaemonPrivate::removeMostUsedPixmaps(const QList<PixmapIdentifier>& identifiers)
{
    QList<PixmapIdentifier>::const_iterator it2 = identifiers.begin();
    while (it2 != identifiers.end()) {
        mostUsedPixmaps.remove(*it2);
        ++it2;
    }
}

void MRemoteThemeDaemonPrivate::pixmapHandle(const QString &imageId, const QSize &size, bool sync)
{
    Q_Q(MRemoteThemeDaemon);

    MPixmapHandle handle = pixmapHandleFromMostUsed(imageId, size);
    if (handle.isValid()) {
        emit q->pixmapCreatedOrChanged(imageId, size, handle);
        return;
    }

    const quint64 sequenceNumber = requestPixmap(imageId, size);

    if (sync) {
        const Packet reply = waitForPacket(sequenceNumber);
        processOnePacket(reply);
    }
}

void MRemoteThemeDaemon::pixmapHandleSync(const QString &imageId, const QSize &size)
{
    Q_D(MRemoteThemeDaemon);

    d->pixmapHandle(imageId, size, true);
}

void MRemoteThemeDaemon::pixmapHandle(const QString &imageId, const QSize &size)
{
    Q_D(MRemoteThemeDaemon);

    d->pixmapHandle(imageId, size, false);
}

MPixmapHandle MRemoteThemeDaemonPrivate::pixmapHandleFromMostUsed(const QString &imageId, const QSize &size)
{
    PixmapIdentifier identifier(imageId, size);
    QHash<PixmapIdentifier, MPixmapHandle>::iterator it = mostUsedPixmaps.find(identifier);
    if (it != mostUsedPixmaps.end())
    {
        int sequenceNumber = ++sequenceCounter;
        stream << Packet(Packet::PixmapUsedPacket, sequenceNumber, new PixmapIdentifier(imageId, size));

        return it.value();
    }

    return MPixmapHandle();
}

void MRemoteThemeDaemon::releasePixmap(const QString &imageId, const QSize &size)
{
    Q_D(MRemoteThemeDaemon);

    PixmapIdentifier *const id = new PixmapIdentifier(imageId, size);

    // If a request for this pixmap is still in the queue, forget about it
    // so that subsequent calls to pixmapHandle() will issue a new request.
    d->pixmapRequests.remove(*id);

    d->stream << Packet(Packet::ReleasePixmapPacket, ++d->sequenceCounter,
                        id /*callee assumes ownership*/);
}


QString MRemoteThemeDaemon::currentTheme()
{
    Q_D(MRemoteThemeDaemon);

    QString theme;
    if (!d->themeInheritanceChain.isEmpty()) {
        QDir dir(d->themeInheritanceChain.at(0));
        theme = dir.dirName();
    } else {
        qCritical() << "MRemoteThemeDaemon::currentTheme() - Asking for current theme and no theme is selected. Returning an empty string";
    }
    return theme;
}

QStringList MRemoteThemeDaemon::themeInheritanceChain()
{
    Q_D(MRemoteThemeDaemon);
    return d->themeInheritanceChain;
}

QStringList MRemoteThemeDaemon::themeLibraryNames()
{
    Q_D(MRemoteThemeDaemon);
    return d->themeLibraryNames;
}

bool MRemoteThemeDaemon::hasPendingRequests() const
{
    Q_D(const MRemoteThemeDaemon);
    return !d->pixmapRequests.isEmpty();
}

Packet MRemoteThemeDaemonPrivate::readOnePacket()
{
    Packet packet;
    stream >> packet;
    return packet;
}

void MRemoteThemeDaemonPrivate::processOnePacket(const Packet &packet)
{
    Q_Q(MRemoteThemeDaemon);
    // process it according to packet type
    switch (packet.type()) {
    case Packet::PixmapUpdatedPacket:
        pixmapUpdated(*static_cast<const PixmapHandle *>(packet.data()));
        break;

    case Packet::ThemeChangedPacket: {
        const ThemeChangeInfo* info = static_cast<const ThemeChangeInfo*>(packet.data());
        themeChanged(info->themeInheritance, info->themeLibraryNames);
        stream << Packet(Packet::ThemeChangeAppliedPacket, packet.sequenceNumber(), new Number(priority()));
    } break;

    case Packet::ThemeChangeCompletedPacket: {
        QTimer::singleShot(0, q, SLOT(emitThemeChangeCompletedSignal()));
    } break;

    case Packet::MostUsedPixmapsPacket: {
        const MostUsedPixmaps *mostUsedPacket = static_cast<const MostUsedPixmaps*>(packet.data());
        addMostUsedPixmaps(mostUsedPacket->addedHandles);
        if (!mostUsedPacket->removedIdentifiers.empty()) {
            removeMostUsedPixmaps(mostUsedPacket->removedIdentifiers);
            stream << Packet(Packet::AckMostUsedPixmapsPacket, packet.sequenceNumber());
        }
    } break;

    case Packet::ErrorPacket: {
        //something went wrong in server side when handling a request from client
        //client tried to request/release a pixmap multiple times, or it did not 
        //send a registration package before requesting pixmaps
        //output the error message and remove pixmap request that caused the problems
        mWarning("MRemoteThemeDaemon") << "Packet::ErrorPacket:" << static_cast<const String *>(packet.data())->string;
        pixmapRequests.remove(pixmapRequests.key(packet.sequenceNumber()));
    }break;

    default:
        mWarning("MRemoteThemeDaemon") << "Couldn't process packet of type" << packet.type();
        break;
    }
}

void MRemoteThemeDaemonPrivate::connectionDataAvailable()
{
    // when reading a packet block all signals to not start
    // reading a second one
    bool blocked = socket.blockSignals(true);
    while (socket.bytesAvailable()) {
        processOnePacket(readOnePacket());
    }

    socket.blockSignals(blocked);
}

void MRemoteThemeDaemonPrivate::emitThemeChangedSignal()
{
    Q_Q(MRemoteThemeDaemon);
    emit q->themeChanged(themeInheritanceChain, themeLibraryNames);
}

void MRemoteThemeDaemonPrivate::emitThemeChangeCompletedSignal()
{
    Q_Q(MRemoteThemeDaemon);
    emit q->themeChangeCompleted();
}

void MRemoteThemeDaemonPrivate::pixmapUpdated(const PixmapHandle &handle)
{
    Q_Q(MRemoteThemeDaemon);

    pixmapRequests.remove(handle.identifier);
   
    // The pixmap may have been updated either in response to a request or
    // due to a theme change.  It may have gone already, if a release
    // request was processed by the server in the meantime.  The recipient
    // of the signal must be able to handle such a situation gracefully.
    emit q->pixmapCreatedOrChanged(handle.identifier.imageId, handle.identifier.size, handle.pixmapHandle);
}

void MRemoteThemeDaemonPrivate::themeChanged(const QStringList &themeInheritanceChain, const QStringList &themeLibraryNames)
{
    Q_Q(MRemoteThemeDaemon);
    mostUsedPixmaps.clear();
    this->themeInheritanceChain = themeInheritanceChain;
    this->themeLibraryNames = themeLibraryNames;

    // Emitting the themeChanged() signal must be done asynchronously, otherwise
    // nested waitForPacket() calls might get triggered.
    QTimer::singleShot(0, q, SLOT(emitThemeChangedSignal()));
}

qint32 MRemoteThemeDaemonPrivate::priority()
{
    if (MApplication::isPrestarted()) {
        return priorityPrestartedApplication;
    }

    MWindow *window = MApplication::activeWindow();
    if (window && window->isOnDisplay()) {
        return priorityForegroundApplication;
    } else {
        return applicationSpecificPriorities.value(applicationName, priorityBackgroundApplication);
    }
}

void MRemoteThemeDaemonPrivate::loadThemeDaemonPriorities(const QString& filename)
{
    priorityForegroundApplication = 100;
    priorityBackgroundApplication = 0;
    priorityPrestartedApplication = -10;
    applicationSpecificPriorities.clear();

    QSettings settings(filename, QSettings::IniFormat);
    if(settings.status() != QSettings::NoError) {
        return;
    }

    priorityForegroundApplication = settings.value("ForegroundApplication/priority", priorityForegroundApplication).toInt();
    priorityBackgroundApplication = settings.value("BackgroundApplication/priority", priorityBackgroundApplication).toInt();
    priorityPrestartedApplication = settings.value("PrestartedApplication/priority", priorityPrestartedApplication).toInt();

    settings.beginGroup("SpecificApplicationPriorities");
    QStringList apps = settings.childKeys();
    foreach (const QString& app, apps) {
        applicationSpecificPriorities[app] = settings.value(app).toInt();
    }
}

#include "moc_mremotethemedaemon.cpp"
