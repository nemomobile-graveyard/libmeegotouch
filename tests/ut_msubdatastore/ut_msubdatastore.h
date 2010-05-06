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

#ifndef UT_MSUBDATASTORE_H
#define UT_MSUBDATASTORE_H

#include <QObject>
#include <QtTest/QtTest>

class MDataStore;
class MSubDataStore;

class Ut_MSubDataStore : public QObject
{
    Q_OBJECT

    MDataStore *testBaseStore;
    MSubDataStore *testableDataStore;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testPrefixSet();
    void testCreateValue();
    void testSetValue();
    void testValue();
    void testAllKeys();
    void testRemove();
    void testClear();
    void testContains();
    void testValueChangedSignal();
};

#endif // UT_MSUBDATASTORE_H
