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
#include "mapplication.h"
#include "mapplicationextensionarea.h"
#include "mapplicationextensionmanager.h"

#include <QtTest/QtTest>

// The test class
void Ut_MApplicationExtensionArea::init()
{
    area = new MApplicationExtensionArea("testcanvas");
}

void Ut_MApplicationExtensionArea::cleanup()
{
    delete area;
}

void Ut_MApplicationExtensionArea::initTestCase()
{
    // MApplications must be created manually due to theme system changes
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplicationextensionarea" };
    app = new MApplication(argc, app_name);
}

void Ut_MApplicationExtensionArea::cleanupTestCase()
{
    delete app;
}

QTEST_APPLESS_MAIN(Ut_MApplicationExtensionArea)

