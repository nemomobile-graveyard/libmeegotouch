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

#ifndef DUITHEMEDAEMONPROTOCOL_H
#define DUITHEMEDAEMONPROTOCOL_H

#include <QDataStream>
#include <QSharedPointer>
#include <QSize>
#include <QColor>

namespace Dui
{
    namespace DuiThemeDaemonProtocol
    {

        extern const QString ServerAddress;

        struct PacketData {
            virtual ~PacketData() = 0;
        };

        class Packet
        {
        public:

            enum PacketType {
                // initial packet type
                Unknown                             = 0,

                // Packet which must be sent to daemon
                // before asking anything else.
                // The type of the data is String and should contain
                // the name of the client.
                RequestRegistrationPacket           = 1,

                // With this packet the client can request a pixmap
                // data type of the packet is PixmapIdentifier.
                // Client can request one pixmap only once. If the same pixmap
                // is requested twice the daemon will reply with ErrorPacket.
                RequestPixmapPacket                 = 8,

                // With this packet the client can request a pixmap release
                // for a pixmap that the client doesn't need anymore.
                // The data type of the packet is PixmapIdentifier.
                // If client tries to release a pixmap that is not loaded
                // by the daemon, the daemon will reply with ErrorPacket.
                ReleasePixmapPacket                 = 9,

                // Daemon will send this packet when a requested pixmap
                // has been loaded, or when the theme has changed
                // and the pixmap data must be replaced.
                // The data type of this packet is PixmapHandle.
                PixmapUpdatedPacket                 = 10,

                // With this packet the client can request the daemon to
                // add a new directory from which to search pixmaps for the client.
                // The data type of this packet is String, which contains an
                // absolute path to the directory.
                RequestNewPixmapDirectoryPacket     = 16,

                // If client wishes to remove all manually added search directories
                // from the daemon, it can be done with this packet.
                // The data type of the packet is undetermined, so NULL will do.
                RequestClearPixmapDirectoriesPacket = 17,

                // daemon will send this packet when the theme has changed
                // data type of the packet is StringList, and contains
                // the new inheritance chain of the theme
                // e.g:
                // /usr/share/themes/devel/
                // /usr/share/themes/base/
                ThemeChangedPacket                  = 33,

                // With this packet any client can request the state
                // of the daemon. The data type is undetermined, so NULL will do.
                // Daemon will reply with ThemeDaemonStatusPacket and the data type
                // is ClientList.
                QueryThemeDaemonStatusPacket        = 129,
                ThemeDaemonStatusPacket             = 130,

                // daemon will send this packet when client has
                // made something illegal
                ErrorPacket                         = 255
            };

            Packet() : m_seq(0), m_data(), m_type(Unknown) {}
            Packet(PacketType type, quint64 seq, PacketData *data = 0);
            ~Packet();

            PacketType type() const { return m_type; }
            void setType(PacketType type) { m_type = type; }

            quint64 sequenceNumber() const { return m_seq; }
            void setSequenceNumber(quint64 seq) { m_seq = seq; }

            const PacketData *data() const { return m_data.data(); }
            void setData(PacketData *data);

        private:
            quint64 m_seq;
            QSharedPointer<PacketData> m_data;
            PacketType m_type;
        };



        // identifier used for identifying an pixmap request, reply and updates.
        struct PixmapIdentifier : PacketData {
            PixmapIdentifier(const QString &imageId, const QSize &size) :
                imageId(imageId), size(size) {}
            virtual ~PixmapIdentifier();

            QString imageId;
            QSize size;
            bool operator==(const PixmapIdentifier &other) const
                { return (imageId == other.imageId && size == other.size); }
            bool operator!=(const PixmapIdentifier &other) const
                { return (imageId != other.imageId || size != other.size); }
        };
        uint qHash(const PixmapIdentifier &id);


        struct String : PacketData {
            String(const QString &string) :
                string(string)
            {}
            virtual ~String();

            QString string;
        };

        struct StringList : PacketData {
            StringList(const QStringList &stringList) :
                stringList(stringList)
            {}
            virtual ~StringList();

            QStringList stringList;
        };

        struct StringBool : PacketData {
            StringBool(const QString &string, bool b) :
                string(string),
                b(b)
            {}
            virtual ~StringBool();

            QString string;
            bool b;
        };

        struct PixmapHandle : PacketData {
            PixmapHandle(const PixmapIdentifier &identifier, Qt::HANDLE pixmapHandle) :
                identifier(identifier),
                pixmapHandle(pixmapHandle)
            {}
            virtual ~PixmapHandle();

            PixmapIdentifier identifier;
            Qt::HANDLE pixmapHandle;
        };


        // struct containing information of pixmaps loaded by one client
        struct ClientInfo {
            QString name;
            QList<PixmapIdentifier> pixmaps;
            QList<PixmapIdentifier> requestedPixmaps;
            QList<PixmapIdentifier> releasedPixmaps;
        };

        struct ClientList : PacketData {
            ClientList(const QList<ClientInfo>& clients) :
                clients(clients)
            {}
            virtual ~ClientList();

            QList<ClientInfo> clients;
        };

    }
}

QDataStream &operator<<(QDataStream &stream, const Dui::DuiThemeDaemonProtocol::Packet &packet);
QDataStream &operator>>(QDataStream &stream, Dui::DuiThemeDaemonProtocol::Packet &packet);

#endif
