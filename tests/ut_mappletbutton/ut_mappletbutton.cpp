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

#include "ut_mappletbutton.h"
#include "mappletbutton.h"
#include <mdesktopentry_stub.h>
#include <mappletmetadata_stub.h>

void Ut_MAppletButton::init()
{
    subject = new MAppletButton();
}

void Ut_MAppletButton::cleanup()
{
    delete subject;
}

void Ut_MAppletButton::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbutton" };
    app = new MApplication(argc, app_name);
}

void Ut_MAppletButton::cleanupTestCase()
{
    delete app;
}

void Ut_MAppletButton::testInitialization()
{
    gMAppletMetaDataStub->stubSetReturnValue("isValid", true);
    const MAppletMetaData metaData("/dev/null");
    QCOMPARE(subject->initialize(metaData), true);
}

QTEST_APPLESS_MAIN(Ut_MAppletButton)
