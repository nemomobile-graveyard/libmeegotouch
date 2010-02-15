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

#include "ut_duiappletinstancedata.h"
#include "duiappletinstancedata.h"
#include "duiappletid_stub.h"

// The test class
void Ut_DuiAppletInstanceData::initTestCase()
{
}

void Ut_DuiAppletInstanceData::cleanupTestCase()
{
}

void Ut_DuiAppletInstanceData::init()
{
    instanceData = new DuiAppletInstanceData;
}

void Ut_DuiAppletInstanceData::cleanup()
{
    // Destroy the manager data
    delete instanceData;
}

QTEST_APPLESS_MAIN(Ut_DuiAppletInstanceData)
