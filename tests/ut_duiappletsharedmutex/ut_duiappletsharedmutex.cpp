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

#include <QtTest/QtTest>
#include <duiappletsharedmutex.h>
#include "ut_duiappletsharedmutex.h"

void Ut_DuiAppletSharedMutex::init()
{
    mutex1 = new DuiAppletSharedMutex();
    mutex2 = NULL;
    mutex3 = NULL;
}

void Ut_DuiAppletSharedMutex::cleanup()
{
    delete mutex1;
    delete mutex2;
    delete mutex3;
}

void Ut_DuiAppletSharedMutex::initTestCase()
{
}

void Ut_DuiAppletSharedMutex::cleanupTestCase()
{
}

void Ut_DuiAppletSharedMutex::testInitSuccess()
{
    // Initialize the mutex
    QVERIFY(mutex1->init("mutex"));

    // Attach to the same mutex
    mutex2 = new DuiAppletSharedMutex;
    QVERIFY(mutex2->init("mutex"));
    delete mutex2;
    mutex2 = NULL;

    // Create a new mutex (the first one should still exist)
    mutex3 = new DuiAppletSharedMutex;
    QVERIFY(mutex3->init("mutex3"));
    delete mutex3;
    mutex3 = NULL;
}

void Ut_DuiAppletSharedMutex::testInitFailureEmptyKey()
{
    // Initializing with an empty key should fail
    QVERIFY(!mutex1->init(""));
}

void Ut_DuiAppletSharedMutex::testLockInitialized()
{
    // Locking an initialized mutex should succeed
    QVERIFY(mutex1->init("mutex"));
    QVERIFY(mutex1->lock());
}

void Ut_DuiAppletSharedMutex::testLockNotInitialized()
{
    // Locking an uninitialized mutex should not succeed
    QVERIFY(!mutex1->lock());
}

void Ut_DuiAppletSharedMutex::testUnlockInitialized()
{
    // Unlocking an initialized mutex should succeed
    QVERIFY(mutex1->init("mutex"));
    QVERIFY(mutex1->unlock());
}

void Ut_DuiAppletSharedMutex::testUnlockNotInitialized()
{
    // Unlocking an uninitialized mutex should not succeed
    QVERIFY(!mutex1->unlock());
}

void Ut_DuiAppletSharedMutex::testTryLockInitialized()
{
    // Locking an initialized mutex should succeed
    QVERIFY(mutex1->init("mutex"));
    QVERIFY(mutex1->tryLock());

    // Trying to lock the mutex again should not succeed
    QVERIFY(!mutex1->tryLock());

    // Attach to the same mutex
    mutex2 = new DuiAppletSharedMutex;
    QVERIFY(mutex2->init("mutex"));

    // Locking the same mutex should not succeed
    QVERIFY(!mutex2->tryLock());
    delete mutex2;
    mutex2 = NULL;
}

void Ut_DuiAppletSharedMutex::testTryLockNotInitialized()
{
    // Locking an uninitialized mutex should not succeed
    QVERIFY(!mutex1->tryLock());
}

QTEST_MAIN(Ut_DuiAppletSharedMutex)
