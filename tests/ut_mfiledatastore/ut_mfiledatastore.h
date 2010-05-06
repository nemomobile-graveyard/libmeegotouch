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

#ifndef UT_MFILEDATASTORE_H
#define UT_MFILEDATASTORE_H

#include <QObject>
#include <QtTest/QtTest>

class MFileDataStore;

class SignalReceptor : public QObject
{
    Q_OBJECT

public slots:
    void valueChanged(const QString &key, QVariant value);

public:
    QList<QString> keys;
    QList<QVariant> values;
};

// Test case must inherit QObject
class Ut_MFileDataStore : public QObject
{
    Q_OBJECT

    MFileDataStore *m_subject;

signals:
    void fileChanged(const QString &fileName);

private slots:

    void init();
    void cleanup();

    void testFileOpening();
    void testValueSetting();
    void testValueReading();
    void testGettingAllKeys();
    void testDataRemoval();
    void testClear();
    void testContains();

    void testFileNotReadable();
    void testFileNotWritable();
    void testFileContentsInvalid();
    void testSettingsFileModifiedExternally();
    void testSyncFailure();
};

#endif // UT_MFILEDATASTORE_H
