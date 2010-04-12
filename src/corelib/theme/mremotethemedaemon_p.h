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

#ifndef MREMOTETHEMEDAEMON_P_H
#define MREMOTETHEMEDAEMON_P_H

#include <QMultiMap>
#include <QColor>
#include <QLocalSocket>

#include "mthemedaemonprotocol.h"


class MRemoteThemeDaemonPrivate
{
public:
    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, quint64> pixmapRequests;

    M::MThemeDaemonProtocol::Packet readOnePacket();
    void processOnePacket(const M::MThemeDaemonProtocol::Packet &packet);
    void connectionDataAvailable();
    quint64 requestPixmap(const QString &imageId, const QSize &size);
    void pixmapUpdated(const M::MThemeDaemonProtocol::PixmapHandle &handle);
    void themeChanged(const QStringList &themeInheritanceChain, const QStringList &themeLibraryNames);
    bool waitForServer(const QString &serverAddress, int timeout);
    M::MThemeDaemonProtocol::Packet waitForPacket(quint64 sequenceNumber);

    Q_DECLARE_PUBLIC(MRemoteThemeDaemon)
    MRemoteThemeDaemon *q_ptr;
    QLocalSocket socket;
    QDataStream stream;

    QStringList themeInheritanceChain;
    QStringList themeLibraryNames;

    quint64 sequenceCounter;
};

#endif // MTHEMEDAEMONSKELETON_P_H
