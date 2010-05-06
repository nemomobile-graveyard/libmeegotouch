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

#include "ut_mappletinstancedata.h"
#include "mappletinstancedata.h"
#include "mappletid_stub.h"

// The test class
void Ut_MAppletInstanceData::initTestCase()
{
}

void Ut_MAppletInstanceData::cleanupTestCase()
{
}

void Ut_MAppletInstanceData::init()
{
    instanceData = new MAppletInstanceData;
}

void Ut_MAppletInstanceData::cleanup()
{
    // Destroy the manager data
    delete instanceData;
}

QTEST_APPLESS_MAIN(Ut_MAppletInstanceData)
