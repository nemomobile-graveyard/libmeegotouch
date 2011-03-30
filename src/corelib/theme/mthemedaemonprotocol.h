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

#ifndef MTHEMEDAEMONPROTOCOL_H
#define MTHEMEDAEMONPROTOCOL_H

#include "mpixmaphandle.h"

#include <QDataStream>
#include <QSharedPointer>
#include <QSize>
#include <QColor>
#include <QList>

namespace M
{
    namespace MThemeDaemonProtocol
    {

        const qint32 protocolVersion = 1;

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
                // after sending the protocol version and before
                // doing any other communication
                // The type of the data is String and should contain
                // the name of the client.
                RequestRegistrationPacket           = 1,

                // Package sent to the themedaemon before doing any other
                // communication. It contains a simple integer representing
                // the ideally used protocol version.
                // Themedaemon answers with the protocol version it will
                // use if any further communication happens. It may be a different
                // protocol version. If the client wants to continue it must use
                // the protocol version sent by the themedaemon from now on.
                ProtocolVersionPacket               = 2,

                // Client tells the daemon that it will use a specific
                // package. It does not expect an answer as it already
                // knows the handle.
                PixmapUsedPacket                     = 7,

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

                // client will send this packet as a reply to ThemeChangedPacket
                // when the client has applied the theme change.
                ThemeChangeAppliedPacket            = 34,

                // daemon will send this packet to all clients when the theme change
                // has completed.
                ThemeChangeCompletedPacket          = 35,

                // themedaemon sends the handles of the most used pixmaps to the
                // application.
                MostUsedPixmapsPacket               = 36,

                // client acks the MostUsedPixmapsPacket from the server
                AckMostUsedPixmapsPacket            = 37,

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
            PixmapIdentifier() {}
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

        struct Number : PacketData {
            Number(qint32 value) :
                value(value)
            {}
            virtual ~Number();

            qint32 value;
        };

        struct String : PacketData {
            String(const QString &string) :
                string(string)
            {}
            virtual ~String();

            QString string;
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
            PixmapHandle() : pixmapHandle()
            {}

            PixmapHandle(const PixmapIdentifier &identifier, MPixmapHandle pixmapHandle) :
                identifier(identifier),
                pixmapHandle(pixmapHandle)
            {}

            PixmapHandle(const PixmapHandle &handle) :
                PacketData(),
                identifier(handle.identifier),
                pixmapHandle(handle.pixmapHandle)
            {}

            virtual ~PixmapHandle();

            PixmapIdentifier identifier;
            MPixmapHandle pixmapHandle;
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

        struct ThemeChangeInfo : PacketData {
            ThemeChangeInfo(const QStringList &themeInheritance, const QStringList &themeLibraryNames) :
                themeInheritance(themeInheritance), 
                themeLibraryNames(themeLibraryNames) 
            {}
            virtual ~ThemeChangeInfo();

            QStringList themeInheritance;
            QStringList themeLibraryNames;
        };

        struct MostUsedPixmaps : PacketData {
            MostUsedPixmaps() {}
            MostUsedPixmaps(const QList<PixmapHandle>& addedHandles, const QList<PixmapIdentifier>& removedIdentifiers) :
                addedHandles(addedHandles),
                removedIdentifiers(removedIdentifiers)
            {}

            virtual ~MostUsedPixmaps();

            QList<PixmapHandle> addedHandles;
            QList<PixmapIdentifier> removedIdentifiers;
        };

        struct RequestedPixmap : PacketData {
            RequestedPixmap(const PixmapIdentifier& id, qint32 priority) :
                id(id),
                priority(priority)
            {}

            virtual ~RequestedPixmap();

            PixmapIdentifier id;
            qint32 priority;
        };
    }
}

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::Packet &packet);
void writePacketData(QDataStream &stream, const M::MThemeDaemonProtocol::Packet &packet);
QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::Packet &packet);
void readPacketData(QDataStream &stream, M::MThemeDaemonProtocol::Packet &packet);

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::PixmapHandle &handle);
QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::PixmapHandle &handle);

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::PixmapIdentifier &id);
QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::PixmapIdentifier &id);

#endif
