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

#ifndef UT_MThemedaemonProtocol_H
#define UT_MThemedaemonProtocol_H

#include <QtTest/QtTest>
#include <QObject>

class Ut_MThemedaemonProtocol : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void streamPixmapId_data();
    void streamPixmapId();

    void streamPixmapHandle_data();
    void streamPixmapHandle();

    // same as error packet
    void streamRequestRegistrationPacket_data();
    void streamRequestRegistrationPacket();

    void streamThemeChangedPacket_data();
    void streamThemeChangedPacket();

    void streamThemeChangeApplied_data();
    void streamThemeChangeApplied();

    void streamRequestNewPixmapDirectoryPacket_data();
    void streamRequestNewPixmapDirectoryPacket();

    void    streamRequestPixmapPacket_data();
    void streamRequestPixmapPacket();

    // same as ReleasePixmapPacket packet
    void streamPixmapUsedPacket_data();
    void streamPixmapUsedPacket();

    void streamPixmapUpdatedPacket();

    void streamMostUsedPixmapsPacket_data();
    void streamMostUsedPixmapsPacket();

    void streamThemeDaemonStatusPacket();

private:
    void verifyStreamIsEmpty();

    template<typename T>
    T streamAndReturn(const T& element);

    QBuffer buffer;
    QDataStream stream;
};

#endif
