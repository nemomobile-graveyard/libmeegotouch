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

#include "ut_mapplicationextensionarea.h"
#include "mdatastore.h"
#include "mapplication.h"
#include "mapplicationextensionarea.h"
#include "mapplicationextensionmetadata.h"
#include "mapplicationextensionmanager_stub.h"

#include <QtTest/QtTest>

// The test class
void Ut_MApplicationExtensionArea::init()
{
    gMApplicationExtensionManagerStub->stubReset();
    area = new MApplicationExtensionArea("testcanvas");
}

void Ut_MApplicationExtensionArea::cleanup()
{
    delete area;
}

void Ut_MApplicationExtensionArea::initTestCase()
{
}

void Ut_MApplicationExtensionArea::cleanupTestCase()
{
}

void Ut_MApplicationExtensionArea::testInitialization()
{
    gMApplicationExtensionManagerStub->stubSetReturnValue("init", true);
    QCOMPARE(area->init(), true);

    gMApplicationExtensionManagerStub->stubSetReturnValue("init", false);
    QCOMPARE(area->init(), false);
}

void Ut_MApplicationExtensionArea::testInProcessFilter()
{
    area->setInProcessFilter(QRegExp("test"));
    QCOMPARE(gMApplicationExtensionManagerStub->stubCallCount("setInProcessFilter"), 1);
    QCOMPARE(gMApplicationExtensionManagerStub->stubLastCallTo("setInProcessFilter").parameter<QRegExp>(0), QRegExp("test"));
}

void Ut_MApplicationExtensionArea::testOutOfProcessFilter()
{
    area->setOutOfProcessFilter(QRegExp("test"));
    QCOMPARE(gMApplicationExtensionManagerStub->stubCallCount("setOutOfProcessFilter"), 1);
    QCOMPARE(gMApplicationExtensionManagerStub->stubLastCallTo("setOutOfProcessFilter").parameter<QRegExp>(0), QRegExp("test"));
}

void Ut_MApplicationExtensionArea::testExtensions()
{
    QList<MApplicationExtensionInterface*> extensions;
    gMApplicationExtensionManagerStub->stubSetReturnValue("extensions", extensions);
    QCOMPARE(area->extensions(), extensions);
}

QTEST_APPLESS_MAIN(Ut_MApplicationExtensionArea)
