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

#ifndef UT_DUINOTIFICATIONMANAGER_H
#define UT_DUINOTIFICATIONMANAGER_H

#include <QtTest/QtTest>
#include <QObject>
#include <QList>
#include <QVariant>
#include <QHash>

class Ut_DuiNotificationManager : public QObject
{
    Q_OBJECT

public:
    static bool captureCalls;
    static QList<QString> asyncCallMethods;
    static QList< QList<QVariant> > asyncCallArguments;
    static QHash<QString, QVariant> mockStore;

private:
    // The notification user ID returned by the manager
    uint notificationUserId;

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test notification adding
    void testAddNotification();
    // Test notification updating
    void testUpdateNotification();
    // Test notification removal
    void testRemoveNotification();
    // Test notification group adding
    void testAddGroup();
    // Test notification group updating
    void testUpdateGroup();
    // Test notification group removal
    void testRemoveGroup();
    // Test adding a notification into a notification group
    void testAddToGroup();
    // Test quering of notifications
    void testNotificationIdList();
};

#endif
