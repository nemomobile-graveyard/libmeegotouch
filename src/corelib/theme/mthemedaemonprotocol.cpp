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

#include "mthemedaemonprotocol.h"
#include <QHash>
#include <QtEndian>

using namespace M::MThemeDaemonProtocol;

const QString M::MThemeDaemonProtocol::ServerAddress = "m.mthemedaemon";
static const int SOCKET_DELAY_MS = 15000;

PacketData::~PacketData()
{}

PixmapIdentifier::~PixmapIdentifier()
{}

String::~String()
{}

StringBool::~StringBool()
{}

PixmapHandle::~PixmapHandle()
{}

ClientList::~ClientList()
{}

ThemeChangeInfo::~ThemeChangeInfo()
{}

Packet::Packet(PacketType type, quint64 seq, PacketData *data)
:
    m_seq  (seq),
    m_data (data),
    m_type (type)
{}

Packet::~Packet()
{}

void Packet::setData(PacketData *data)
{
    m_data = QSharedPointer<PacketData>(data);
}

QDataStream &operator<<(QDataStream &stream, const Packet &packet)
{
    Q_ASSERT(packet.type() != Packet::Unknown);

    stream << quint32(packet.type());
    stream << packet.sequenceNumber();

    switch (packet.type()) {

    // NULL as data
    case Packet::RequestClearPixmapDirectoriesPacket:
    case Packet::QueryThemeDaemonStatusPacket:
        break;

    // string as data
    case Packet::ErrorPacket:
    case Packet::RequestRegistrationPacket: {
        stream << static_cast<const String *>(packet.data())->string;
    } break;

    // two string lists as data
    case Packet::ThemeChangedPacket: {
        const ThemeChangeInfo* info = static_cast<const ThemeChangeInfo*>(packet.data());
        stream << info->themeInheritance << info->themeLibraryNames;
    } break;

    // stringbool as data
    case Packet::RequestNewPixmapDirectoryPacket: {
        const StringBool *sb = static_cast<const StringBool *>(packet.data());
        stream << sb->string << sb->b;
    } break;

    // pixmap identifier as data
    case Packet::RequestPixmapPacket:
    case Packet::ReleasePixmapPacket: {
        const PixmapIdentifier *id = static_cast<const PixmapIdentifier *>(packet.data());
        stream << id->imageId << id->size;
    } break;

    // pixmap handle as data
    case Packet::PixmapUpdatedPacket: {
        const PixmapHandle *h = static_cast<const PixmapHandle *>(packet.data());
        stream << h->identifier.imageId << h->identifier.size
               << quint64((quintptr) h->pixmapHandle);
    } break;

    // client list as data
    case Packet::ThemeDaemonStatusPacket: {
        const ClientList *cl = static_cast<const ClientList *>(packet.data());
        quint32 clientCount = cl->clients.count();
        stream << clientCount;
        while (clientCount) {
            const ClientInfo &info = cl->clients.at(clientCount - 1);
            stream << info.name;
            quint32 pixmapCount = info.pixmaps.count();
            stream << pixmapCount;
            while (pixmapCount) {
                const PixmapIdentifier &id = info.pixmaps.at(pixmapCount - 1);
                stream << id.imageId << id.size;
                --pixmapCount;
            }
            quint32 requestedPixmapCount = info.requestedPixmaps.count();
            stream << requestedPixmapCount;
            while (requestedPixmapCount) {
                const PixmapIdentifier &id = info.requestedPixmaps.at(requestedPixmapCount - 1);
                stream << id.imageId << id.size;
                --requestedPixmapCount;
            }
            quint32 releasedPixmapCount = info.releasedPixmaps.count();
            stream << releasedPixmapCount;
            while (releasedPixmapCount) {
                const PixmapIdentifier &id = info.releasedPixmaps.at(releasedPixmapCount - 1);
                stream << id.imageId << id.size;
                --releasedPixmapCount;
            }

            --clientCount;
        }
    } break;

    default:
        // print out warning
        break;
    }

    return stream;
}

static bool waitForAvailableBytes(QDataStream &stream, quint32 count)
{
    while (stream.device()->bytesAvailable() < count) {
        if (!stream.device()->waitForReadyRead(SOCKET_DELAY_MS)) {
            return false;
        }
    }
    return true;
}

static QString readQString(QDataStream &stream)
{
    if (!waitForAvailableBytes(stream, sizeof(quint32))) {
        return QString();
    }
    char b[sizeof(quint32)];
    stream.device()->peek(b, sizeof(quint32));
    quint32 stringSize = qFromBigEndian<quint32>(reinterpret_cast<uchar*>(b));
    if (stringSize == 0xFFFFFFFF) {
        return QString();
    }
    if (!waitForAvailableBytes(stream, stringSize + sizeof(quint32))) {
        return QString();
    }
    QString string;
    stream >> string;
    return string;
}

static QStringList readQStringList(QDataStream &stream)
{
    quint32 nrOfStrings;
    if (!waitForAvailableBytes(stream, sizeof(quint32))) {
        return QStringList();
    }
    stream >> nrOfStrings;

    QStringList stringList;
    for (quint32 i = 0; i < nrOfStrings; ++i) {
        QString entry = readQString(stream);
        if (entry.isNull()) {
            return QStringList();
        }
        stringList.append(entry);
    }
    return stringList;
}

QDataStream &operator>>(QDataStream &stream, Packet &packet)
{
    Q_ASSERT(!packet.data());

    quint32 type = 0;
    quint64 seq  = 0;
    if (!waitForAvailableBytes(stream, sizeof(quint32) + sizeof(quint64))) {
        return stream;
    }
    stream >> type >> seq;
    packet.setType(Packet::PacketType(type));
    packet.setSequenceNumber(seq);

    switch (packet.type()) {

    // NULL as data
    case Packet::RequestClearPixmapDirectoriesPacket:
    case Packet::QueryThemeDaemonStatusPacket:
        break;

    // string as data
    case Packet::ErrorPacket:
    case Packet::RequestRegistrationPacket: {
        QString string = readQString(stream);
        packet.setData(new String(string));
    } break;

    // two string lists as data
    case Packet::ThemeChangedPacket: {
        QStringList themeInheritance, themeLibraryNames;
        themeInheritance = readQStringList(stream);
        themeLibraryNames = readQStringList(stream);
        packet.setData(new ThemeChangeInfo(themeInheritance, themeLibraryNames));
    } break;

    // stringbool as data
    case Packet::RequestNewPixmapDirectoryPacket: {
        QString string = readQString(stream);
        bool b = false;
        waitForAvailableBytes(stream, sizeof(bool));
        stream >> b;
        packet.setData(new StringBool(string, b));
    } break;

    // pixmap identifier as data
    case Packet::RequestPixmapPacket:
    case Packet::ReleasePixmapPacket: {
        QString imageId = readQString(stream);
        QSize size;
        waitForAvailableBytes(stream, 2*sizeof(qint32));
        stream >> size;
        packet.setData(new PixmapIdentifier(imageId, size));
    } break;

    // pixmap handle as data
    case Packet::PixmapUpdatedPacket: {
        QString imageId = readQString(stream);;
        QSize size;
        quint64 pixmapHandle = 0;
        waitForAvailableBytes(stream, 2*sizeof(qint32)+sizeof(qint64));
        stream >> size >> pixmapHandle;
        packet.setData(new PixmapHandle(PixmapIdentifier(imageId, size),
                       (Qt::HANDLE) quintptr(pixmapHandle)));
    } break;


    // client list as data
    case Packet::ThemeDaemonStatusPacket: {
        QList<ClientInfo> clients;
        quint32 clientCount = 0;
        stream >> clientCount;
        while (clientCount) {
            ClientInfo info;
            info.name = readQString(stream);
            quint32 pixmapCount = 0;
            waitForAvailableBytes(stream, 2*sizeof(quint32));
            stream >> pixmapCount;
            while (pixmapCount) {
                QString imageId = readQString(stream);
                QSize size;
                waitForAvailableBytes(stream, 2*sizeof(qint32));
                stream >> size;
                info.pixmaps.append(PixmapIdentifier(imageId, size));
                --pixmapCount;
            }
            quint32 requestedPixmapCount = 0;
            stream >> requestedPixmapCount;
            while (requestedPixmapCount) {
                QString imageId = readQString(stream);
                QSize size;
                waitForAvailableBytes(stream, 2*sizeof(qint32));
                stream >> size;
                info.requestedPixmaps.append(PixmapIdentifier(imageId, size));
                --requestedPixmapCount;
            }
            quint32 releasedPixmapCount = 0;
            stream >> releasedPixmapCount;
            while (releasedPixmapCount) {
                QString imageId = readQString(stream);
                QSize size;
                waitForAvailableBytes(stream, 2*sizeof(qint32));
                stream >> size;
                info.releasedPixmaps.append(PixmapIdentifier(imageId, size));
                --releasedPixmapCount;
            }

            clients.append(info);
            --clientCount;
        }
        packet.setData(new ClientList(clients));
    } break;

    default:
        // print out warning
        break;
    }

    return stream;
}


uint M::MThemeDaemonProtocol::qHash(const PixmapIdentifier &id)
{
    using ::qHash;

    const uint idHash     = qHash(id.imageId);
    const uint widthHash  = qHash(id.size.width());
    const uint heightHash = qHash(id.size.height());

    // Twiddle the bits a little, taking a cue from Qt's own qHash() overloads
    return idHash ^ (widthHash << 8) ^ (widthHash >> 24) ^ (heightHash << 24) ^ (heightHash >> 8);
}
