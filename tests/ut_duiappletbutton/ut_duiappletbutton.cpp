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

#include "ut_duiappletbutton.h"
#include "duiappletbutton.h"
#include <duidesktopentry_stub.h>
#include <duiappletmetadata_stub.h>

void Ut_DuiAppletButton::init()
{
    subject = new DuiAppletButton();
}

void Ut_DuiAppletButton::cleanup()
{
    delete subject;
}

void Ut_DuiAppletButton::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duibutton" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiAppletButton::cleanupTestCase()
{
    delete app;
}

void Ut_DuiAppletButton::testInitialization()
{
    gDuiAppletMetaDataStub->stubSetReturnValue("isValid", true);
    const DuiAppletMetaData metaData("/dev/null");
    QCOMPARE(subject->initialize(metaData), true);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletButton)
