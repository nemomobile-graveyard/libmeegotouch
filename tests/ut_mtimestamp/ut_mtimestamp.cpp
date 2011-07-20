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

#include <QRegExp>

#include "ut_mtimestamp.h"
#include "mtimestamp.h"

#define MODULE_NAME "SomeModuleName"
#define MESSAGE     "The quick brown fox jumps over the lazy dog"

void Ut_MTimestamp::initTestCase()
{
    static int argc = 2;
    static char *app_name[2] = { (char *) "./ut_mtimestamp", (char *) "-debug" };
    app = new QApplication(argc, app_name);
}

void messageHandler(QtMsgType type, const char *msg)
{
    QVERIFY(type == QtWarningMsg);

    const char *regexSt = "^" MODULE_NAME ": \"([0-9]*).[0-9]* MARK\\|" __FILE__ "\\|void Ut_MTimestamp::testMTimestampStream\\(\\)\\|" MESSAGE "\" $";
    QRegExp regex(regexSt);

    QVERIFY(regex.indexIn(msg) == 0);
}

void Ut_MTimestamp::testMTimestamp()
{
    QtMsgHandler oldMsgHandler;

    oldMsgHandler = qInstallMsgHandler(messageHandler);

    mTimestamp(MODULE_NAME, MESSAGE);

    qInstallMsgHandler(oldMsgHandler);
}

void Ut_MTimestamp::testMTimestampStream()
{
    QtMsgHandler oldMsgHandler;

    oldMsgHandler = qInstallMsgHandler(messageHandler);

    mTimestampStream(MODULE_NAME, __FILE__, __PRETTY_FUNCTION__, MESSAGE);

    qInstallMsgHandler(oldMsgHandler);
}

void Ut_MTimestamp::cleanupTestCase()
{
    delete app;
}

QTEST_APPLESS_MAIN(Ut_MTimestamp)
