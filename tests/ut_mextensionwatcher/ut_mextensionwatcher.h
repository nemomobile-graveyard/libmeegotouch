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

#ifndef UT_MEXTENSIONWATCHER_H
#define UT_MEXTENSIONWATCHER_H

#include <QtTest/QtTest>
#include <QObject>

#include "mextensionwatcher.h"

class Ut_MExtensionWatcher : public QObject
{
    Q_OBJECT

signals:
    void fileChanged(const QString &path);
private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testConstruction();

    void testExtensionAddition();
    void testExtensionRemoval();

    void testFileWatcherNotifications();



private:
    MExtensionWatcher *m_subject;
};

#endif
