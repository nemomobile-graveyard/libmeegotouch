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

#include <QtTest/QtTest>
#include <mappletsharedmutex.h>
#include "ut_mappletsharedmutex.h"

void Ut_MAppletSharedMutex::init()
{
    mutex1 = new MAppletSharedMutex();
    mutex2 = NULL;
    mutex3 = NULL;
}

void Ut_MAppletSharedMutex::cleanup()
{
    delete mutex1;
    delete mutex2;
    delete mutex3;
}

void Ut_MAppletSharedMutex::initTestCase()
{
}

void Ut_MAppletSharedMutex::cleanupTestCase()
{
}

void Ut_MAppletSharedMutex::testInitSuccess()
{
    // Initialize the mutex
    QVERIFY(mutex1->init("mutex"));

    // Attach to the same mutex
    mutex2 = new MAppletSharedMutex;
    QVERIFY(mutex2->init("mutex"));
    delete mutex2;
    mutex2 = NULL;

    // Create a new mutex (the first one should still exist)
    mutex3 = new MAppletSharedMutex;
    QVERIFY(mutex3->init("mutex3"));
    delete mutex3;
    mutex3 = NULL;
}

void Ut_MAppletSharedMutex::testInitFailureEmptyKey()
{
    // Initializing with an empty key should fail
    QVERIFY(!mutex1->init(""));
}

void Ut_MAppletSharedMutex::testLockInitialized()
{
    // Locking an initialized mutex should succeed
    QVERIFY(mutex1->init("mutex"));
    QVERIFY(mutex1->lock());
}

void Ut_MAppletSharedMutex::testLockNotInitialized()
{
    // Locking an uninitialized mutex should not succeed
    QVERIFY(!mutex1->lock());
}

void Ut_MAppletSharedMutex::testUnlockInitialized()
{
    // Unlocking an initialized mutex should succeed
    QVERIFY(mutex1->init("mutex"));
    QVERIFY(mutex1->unlock());
}

void Ut_MAppletSharedMutex::testUnlockNotInitialized()
{
    // Unlocking an uninitialized mutex should not succeed
    QVERIFY(!mutex1->unlock());
}

void Ut_MAppletSharedMutex::testTryLockInitialized()
{
    // Locking an initialized mutex should succeed
    QVERIFY(mutex1->init("mutex"));
    QVERIFY(mutex1->tryLock());

    // Trying to lock the mutex again should not succeed
    QVERIFY(!mutex1->tryLock());

    // Attach to the same mutex
    mutex2 = new MAppletSharedMutex;
    QVERIFY(mutex2->init("mutex"));

    // Locking the same mutex should not succeed
    QVERIFY(!mutex2->tryLock());
    delete mutex2;
    mutex2 = NULL;
}

void Ut_MAppletSharedMutex::testTryLockNotInitialized()
{
    // Locking an uninitialized mutex should not succeed
    QVERIFY(!mutex1->tryLock());
}

QTEST_MAIN(Ut_MAppletSharedMutex)
