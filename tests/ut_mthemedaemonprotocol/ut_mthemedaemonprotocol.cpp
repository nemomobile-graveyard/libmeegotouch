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

#include "ut_mthemedaemonprotocol.h"

#include "mthemedaemonprotocol.h"

#include <QBuffer>

using namespace M::MThemeDaemonProtocol;

bool operator==(const MPixmapHandle& h1, const MPixmapHandle& h2)
{
    return h1.eglHandle == h2.eglHandle && h1.format == h2.format &&
            h1.numBytes == h2.numBytes && h1.shmHandle == h2.shmHandle &&
            h1.size == h2.size && h1.xHandle == h2.xHandle;
}

bool operator==(const PixmapHandle& h1, const PixmapHandle& h2)
{
    return h1.identifier == h2.identifier && h1.pixmapHandle == h2.pixmapHandle;
}

bool operator==(const ClientInfo& c1, const ClientInfo& c2)
{
    return c1.name == c2.name && c1.pixmaps == c2.pixmaps
            && c1.releasedPixmaps == c2.releasedPixmaps
            && c1.requestedPixmaps == c2.requestedPixmaps;
}

void Ut_MThemedaemonProtocol::init()
{
    buffer.open(QBuffer::ReadWrite);
}

void Ut_MThemedaemonProtocol::cleanup()
{
    buffer.close();
    buffer.setData(QByteArray());
}

void Ut_MThemedaemonProtocol::initTestCase()
{
    stream.setDevice(&buffer);
}

void Ut_MThemedaemonProtocol::cleanupTestCase()
{
}

void Ut_MThemedaemonProtocol::streamPixmapId_data()
{
    QTest::addColumn<QString>("imageId");
    QTest::addColumn<QSize>("size");

    QTest::newRow("normal") << "testId" << QSize(100, 17);
    QTest::newRow("empty id") << QString() << QSize(100, 17);
    QTest::newRow("default size") << "testId" << QSize();
    QTest::newRow("all default") << QString() << QSize();
}

void Ut_MThemedaemonProtocol::streamPixmapId()
{
    QFETCH(QString, imageId);
    QFETCH(QSize, size);

    PixmapIdentifier id(imageId, size);

    PixmapIdentifier id2 = streamAndReturn(id);
    verifyStreamIsEmpty();

    QCOMPARE(id2.imageId, id.imageId);
    QCOMPARE(id2.size, id.size);
}

void Ut_MThemedaemonProtocol::streamPixmapHandle_data()
{
    QTest::addColumn<QString>("imageId");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<MPixmapHandle>("pixmapHandle");

    MPixmapHandle pixmapHandle;
    QTest::newRow("normal") << "testId" << QSize(100, 17) << pixmapHandle;
    pixmapHandle.eglHandle = 99999;
    pixmapHandle.format = QImage::Format_ARGB32_Premultiplied;
    pixmapHandle.numBytes = 195;
    pixmapHandle.shmHandle = "abcd";
    pixmapHandle.size = QSize(7,128);
    pixmapHandle.xHandle = 2;
    QTest::newRow("empty id") << QString() << QSize(100, 17) << pixmapHandle;
    pixmapHandle.format = QImage::Format_ARGB32;
    QTest::newRow("default size") << "testId" << QSize() << pixmapHandle;
    pixmapHandle.format = QImage::Format_RGB32;
    QTest::newRow("all default") << QString() << QSize() << pixmapHandle;
}

void Ut_MThemedaemonProtocol::streamPixmapHandle()
{
    QFETCH(QString, imageId);
    QFETCH(QSize, size);
    QFETCH(MPixmapHandle, pixmapHandle);

    PixmapHandle handle(PixmapIdentifier(imageId, size), pixmapHandle);
    PixmapHandle handle2 = streamAndReturn(handle);
    verifyStreamIsEmpty();

    QCOMPARE(handle2, handle);
}

void Ut_MThemedaemonProtocol::streamRequestRegistrationPacket_data()
{
    QTest::addColumn<int>("type");
    QTest::newRow("registration") << int(Packet::RequestRegistrationPacket);
    QTest::newRow("error") << int(Packet::ErrorPacket);
}


void Ut_MThemedaemonProtocol::streamRequestRegistrationPacket()
{
    QFETCH(int, type);
    Packet::PacketType packetType = Packet::PacketType(type);

    quint64 sequenceNumber = 123;
    QString string = "abc";
    Packet packet(packetType, sequenceNumber, new String(string));

    Packet packet2 = streamAndReturn(packet);
    verifyStreamIsEmpty();

    QCOMPARE(packet2.type(), packetType);
    QCOMPARE(packet2.sequenceNumber(), sequenceNumber);
    QString string2 = static_cast<const String *>(packet.data())->string;
    QCOMPARE(string2, string);
}

void Ut_MThemedaemonProtocol::streamThemeChangedPacket_data()
{
    QTest::addColumn<QStringList>("themeInheritance");
    QTest::addColumn<QStringList>("themeLibraryNames");

    QStringList list;
    list << QString("abc") << QString("def") << QString();
    QTest::newRow("non-empty") << list << list;
    QTest::newRow("first empty") << QStringList() << list;
    QTest::newRow("second empty") << list << QStringList();
}

void Ut_MThemedaemonProtocol::streamThemeChangedPacket()
{
    QFETCH(QStringList, themeInheritance);
    QFETCH(QStringList, themeLibraryNames);
    quint64 sequenceNumber = 123;

    Packet packet(Packet::ThemeChangedPacket, sequenceNumber, new ThemeChangeInfo(themeInheritance, themeLibraryNames));

    Packet packet2 = streamAndReturn(packet);
    verifyStreamIsEmpty();
    QCOMPARE(packet2.type(), Packet::ThemeChangedPacket);
    QCOMPARE(packet2.sequenceNumber(), sequenceNumber);
    const ThemeChangeInfo* info = static_cast<const ThemeChangeInfo*>(packet2.data());
    QCOMPARE(info->themeInheritance, themeInheritance);
    QCOMPARE(info->themeLibraryNames, themeLibraryNames);
}

void Ut_MThemedaemonProtocol::streamThemeChangeApplied_data()
{
    QTest::addColumn<qint32>("priority");
    QTest::newRow("negative") << -10;
    QTest::newRow("zero") << 0;
    QTest::newRow("positive") << 100;
}

void Ut_MThemedaemonProtocol::streamThemeChangeApplied()
{
    QFETCH(qint32, priority);
    quint64 sequenceNumber = 123;
    Packet packet(Packet::ThemeChangeAppliedPacket, sequenceNumber, new Number(priority));

    Packet packet2 = streamAndReturn(packet);
    verifyStreamIsEmpty();
    QCOMPARE(packet2.type(), Packet::ThemeChangeAppliedPacket);
    QCOMPARE(packet2.sequenceNumber(), sequenceNumber);
    const Number* number = static_cast<const Number*>(packet2.data());

    QCOMPARE(number->value, priority);
}

void Ut_MThemedaemonProtocol::streamRequestNewPixmapDirectoryPacket_data()
{
    QTest::addColumn<QString>("directory");
    QTest::addColumn<bool>("recursive");

    QTest::newRow("non-empty true") << "abc" << true;
    QTest::newRow("non-empty false") << "abc" << false;
    QTest::newRow("empty true") << QString() << true;
    QTest::newRow("empty false") << QString() << false;
}

void Ut_MThemedaemonProtocol::streamRequestNewPixmapDirectoryPacket()
{
    QFETCH(QString, directory);
    QFETCH(bool, recursive);
    quint64 sequenceNumber = 123;

    Packet packet(Packet::RequestNewPixmapDirectoryPacket, sequenceNumber, new StringBool(directory, recursive));

    Packet packet2 = streamAndReturn(packet);
    verifyStreamIsEmpty();
    QCOMPARE(packet2.type(), Packet::RequestNewPixmapDirectoryPacket);
    QCOMPARE(packet2.sequenceNumber(), sequenceNumber);
    const StringBool* sb = static_cast<const StringBool*>(packet2.data());
    QCOMPARE(sb->string, directory);
    QCOMPARE(sb->b, recursive);
}

void Ut_MThemedaemonProtocol::streamRequestPixmapPacket_data()
{
    QTest::addColumn<qint32>("priority");
    QTest::newRow("negative") << -10;
    QTest::newRow("zero") << 0;
    QTest::newRow("positive") << 100;
}

void Ut_MThemedaemonProtocol::streamRequestPixmapPacket()
{
    QFETCH(qint32, priority);
    Packet::PacketType packetType = Packet::RequestPixmapPacket;

    quint64 sequenceNumber = 123;
    PixmapIdentifier id("imageId", QSize(123, 456));
    Packet packet(packetType, sequenceNumber, new RequestedPixmap(id, priority));

    Packet packet2 = streamAndReturn(packet);
    verifyStreamIsEmpty();
    QCOMPARE(packet2.type(), packetType);
    QCOMPARE(packet2.sequenceNumber(), sequenceNumber);
    const RequestedPixmap* rp = static_cast<const RequestedPixmap *>(packet.data());
    QCOMPARE(rp->id, id);
    QCOMPARE(rp->priority, priority);
}

void Ut_MThemedaemonProtocol::streamPixmapUsedPacket_data()
{
    QTest::addColumn<int>("type");
    QTest::newRow("pixmap used") << int(Packet::PixmapUsedPacket);
    QTest::newRow("release pixmap") << int(Packet::ReleasePixmapPacket);
}

void Ut_MThemedaemonProtocol::streamPixmapUsedPacket()
{
    QFETCH(int, type);
    Packet::PacketType packetType = Packet::PacketType(type);

    quint64 sequenceNumber = 123;
    PixmapIdentifier id("imageId", QSize(123, 456));
    Packet packet(packetType, sequenceNumber, new PixmapIdentifier(id));

    Packet packet2 = streamAndReturn(packet);
    verifyStreamIsEmpty();
    QCOMPARE(packet2.type(), packetType);
    QCOMPARE(packet2.sequenceNumber(), sequenceNumber);
    const PixmapIdentifier* id2 = static_cast<const PixmapIdentifier *>(packet.data());
    QCOMPARE(*id2, id);
}

void Ut_MThemedaemonProtocol::streamPixmapUpdatedPacket()
{
    quint64 sequenceNumber = 123;

    MPixmapHandle ph;
    ph.xHandle = 789;
    PixmapHandle handle(PixmapIdentifier("abc", QSize(123,456)), ph);
    Packet packet(Packet::PixmapUpdatedPacket, sequenceNumber, new PixmapHandle(handle));

    Packet packet2 = streamAndReturn(packet);
    verifyStreamIsEmpty();
    QCOMPARE(packet2.type(), Packet::PixmapUpdatedPacket);
    QCOMPARE(packet2.sequenceNumber(), sequenceNumber);
    const PixmapHandle* handle2 = static_cast<const PixmapHandle*>(packet2.data());
    QCOMPARE(*handle2, handle);
}

void Ut_MThemedaemonProtocol::streamMostUsedPixmapsPacket_data()
{
    QTest::addColumn<QList<PixmapHandle> >("addedHandles");
    QTest::addColumn<QList<PixmapIdentifier> >("removedIdentifiers");

    QList<PixmapHandle> handles;
    MPixmapHandle ph;
    ph.xHandle = 23;
    handles << PixmapHandle(PixmapIdentifier("abc", QSize()), ph);
    ph.xHandle = 97;
    handles << PixmapHandle(PixmapIdentifier("def", QSize(17,23)), ph);
    QList<PixmapIdentifier> identifiers;
    identifiers << PixmapIdentifier("abc", QSize())
            << PixmapIdentifier("def", QSize(17,23));

    QTest::newRow("added and removed") << handles << identifiers;
    QTest::newRow("just added") << handles << QList<PixmapIdentifier>();
    QTest::newRow("just removed") << QList<PixmapHandle>() << identifiers;
}

void Ut_MThemedaemonProtocol::streamMostUsedPixmapsPacket()
{
    QFETCH(QList<PixmapHandle>, addedHandles);
    QFETCH(QList<PixmapIdentifier>, removedIdentifiers);

    quint64 sequenceNumber = 123;
    Packet packet(Packet::MostUsedPixmapsPacket, sequenceNumber, new MostUsedPixmaps(addedHandles, removedIdentifiers));
    Packet packet2 = streamAndReturn(packet);
    verifyStreamIsEmpty();
    QCOMPARE(packet2.type(), Packet::MostUsedPixmapsPacket);
    QCOMPARE(packet2.sequenceNumber(), sequenceNumber);
    const MostUsedPixmaps* mostUsed = static_cast<const MostUsedPixmaps*>(packet2.data());

    QCOMPARE(mostUsed->addedHandles, addedHandles);
    QCOMPARE(mostUsed->removedIdentifiers, removedIdentifiers);
}

// this packet is just used for testing. so test data is not as
// deep as it could be
void Ut_MThemedaemonProtocol::streamThemeDaemonStatusPacket()
{
    QList<ClientInfo> clients;
    ClientInfo client;
    client.name = "client1";
    client.pixmaps << PixmapIdentifier("abc", QSize())
        << PixmapIdentifier("def", QSize(17,23));
    client.releasedPixmaps = client.pixmaps;
    client.requestedPixmaps = client.pixmaps;
    clients << client;
    client.name = "client2";
    client.releasedPixmaps.clear();
    clients << client;
    client.name = "client3";
    client.pixmaps.clear();
    clients << client;

    quint64 sequenceNumber = 123;
    Packet packet(Packet::ThemeDaemonStatusPacket, sequenceNumber, new ClientList(clients));
    Packet packet2 = streamAndReturn(packet);
    verifyStreamIsEmpty();
    QCOMPARE(packet2.type(), Packet::ThemeDaemonStatusPacket);
    QCOMPARE(packet2.sequenceNumber(), sequenceNumber);
    const ClientList* clientList = static_cast<const ClientList*>(packet2.data());

    QCOMPARE(clientList->clients.count(), clients.count());
    QCOMPARE(clientList->clients, clients);
}

Q_DECLARE_METATYPE(QList<PixmapHandle>)
Q_DECLARE_METATYPE(QList<PixmapIdentifier>)
Q_DECLARE_METATYPE(MPixmapHandle)

void Ut_MThemedaemonProtocol::verifyStreamIsEmpty()
{
    QCOMPARE(buffer.bytesAvailable(), (qint64)0);
}

template <typename T>
T Ut_MThemedaemonProtocol::streamAndReturn(const T& element)
{
    stream << element;
    buffer.seek(0);

    T element2;
    stream >> element2;

    return element2;
}


QTEST_APPLESS_MAIN(Ut_MThemedaemonProtocol)
