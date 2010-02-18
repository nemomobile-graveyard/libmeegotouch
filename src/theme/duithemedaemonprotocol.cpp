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

#include "duithemedaemonprotocol.h"
#include <QHash>

using namespace Dui::DuiThemeDaemonProtocol;

const QString Dui::DuiThemeDaemonProtocol::ServerAddress = "dui.duithemedaemon";

PacketData::~PacketData()
{}

PixmapIdentifier::~PixmapIdentifier()
{}

String::~String()
{}

StringList::~StringList()
{}

StringBool::~StringBool()
{}

PixmapHandle::~PixmapHandle()
{}

ClientList::~ClientList()
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

    // string list as data
    case Packet::ThemeChangedPacket: {
        stream << static_cast<const StringList *>(packet.data())->stringList;
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

QDataStream &operator>>(QDataStream &stream, Packet &packet)
{
    Q_ASSERT(!packet.data());

    quint32 type = 0;
    quint64 seq  = 0;
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
        QString string;
        stream >> string;
        packet.setData(new String(string));
    } break;

    // string list as data
    case Packet::ThemeChangedPacket: {
        QStringList stringList;
        stream >> stringList;
        packet.setData(new StringList(stringList));
    } break;

    // stringbool as data
    case Packet::RequestNewPixmapDirectoryPacket: {
        QString string;
        bool b = false;
        stream >> string >> b;
        packet.setData(new StringBool(string, b));
    } break;

    // pixmap identifier as data
    case Packet::RequestPixmapPacket:
    case Packet::ReleasePixmapPacket: {
        QString imageId;
        QSize size;
        stream >> imageId >> size;
        packet.setData(new PixmapIdentifier(imageId, size));
    } break;

    // pixmap handle as data
    case Packet::PixmapUpdatedPacket: {
        QString imageId;
        QSize size;
        quint64 pixmapHandle = 0;
        stream >> imageId >> size >> pixmapHandle;
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
            stream >> info.name;
            quint32 pixmapCount = 0;
            stream >> pixmapCount;
            while (pixmapCount) {
                QString imageId;
                QSize size;
                stream >> imageId >> size;
                info.pixmaps.append(PixmapIdentifier(imageId, size));
                --pixmapCount;
            }
            quint32 requestedPixmapCount = 0;
            stream >> requestedPixmapCount;
            while (requestedPixmapCount) {
                QString imageId;
                QSize size;
                stream >> imageId >> size;
                info.requestedPixmaps.append(PixmapIdentifier(imageId, size));
                --requestedPixmapCount;
            }
            quint32 releasedPixmapCount = 0;
            stream >> releasedPixmapCount;
            while (releasedPixmapCount) {
                QString imageId;
                QSize size;
                stream >> imageId >> size;
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


uint Dui::DuiThemeDaemonProtocol::qHash(const PixmapIdentifier &id)
{
    using ::qHash;

    const uint idHash     = qHash(id.imageId);
    const uint widthHash  = qHash(id.size.width());
    const uint heightHash = qHash(id.size.height());

    // Twiddle the bits a little, taking a cue from Qt's own qHash() overloads
    return idHash ^ (widthHash << 8) ^ (widthHash >> 24) ^ (heightHash << 24) ^ (heightHash >> 8);
}
