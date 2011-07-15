/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <MApplication>

#include "ut_mdebug.h"
#include "mdebug.h"

#define MODULE_NAME "SomeModuleName"
#define MESSAGE     "The quick brown fox jumps over the lazy dog"

static QtMsgType expectedMsgType;

void Ut_MDebug::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mdebug" };
    app = new QApplication(argc, app_name);
}

void Ut_MDebug::cleanupTestCase()
{
    delete app;
}

void messageHandler(QtMsgType type, const char *msg)
{
    const char *expectedMsg = MODULE_NAME ": " MESSAGE " ";

    QVERIFY(type == expectedMsgType);

    QVERIFY(strlen(expectedMsg) == strlen(msg));
    QVERIFY(QString(msg) == expectedMsg);
}

void Ut_MDebug::testMessage (QtMsgType type)
{
    QtMsgHandler oldMsgHandler;

    expectedMsgType = type;

    oldMsgHandler = qInstallMsgHandler(messageHandler);

    switch (type) {
    case QtDebugMsg:
        mDebug(MODULE_NAME) << MESSAGE;
        break;
    case QtWarningMsg:
        mWarning(MODULE_NAME) << MESSAGE;
        break;
    default:
      break;
    }

    qInstallMsgHandler(oldMsgHandler);
}

void Ut_MDebug::testDebug()
{
    testMessage (QtDebugMsg);
}

void Ut_MDebug::testWarning()
{
    testMessage(QtWarningMsg);
}

QTEST_APPLESS_MAIN(Ut_MDebug)
