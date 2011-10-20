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

#include "ut_mdebug.h"

static QtMsgType expectedMsgType;
static QString expectedMsg;

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
    QCOMPARE(type, expectedMsgType);
    QVERIFY(strlen(msg) == (unsigned int)expectedMsg.length());
    QCOMPARE(QString(msg), expectedMsg);
}

void Ut_MDebug::init()
{
    oldMsgHandler = qInstallMsgHandler(messageHandler);
}

void Ut_MDebug::cleanup()
{
    qInstallMsgHandler(oldMsgHandler);
}

void Ut_MDebug::testInlineFunctions_data()
{
    QTest::addColumn<QtMsgType>("type");
    QTest::addColumn<QString>("module");
    QTest::addColumn<QString>("message");

    QTest::newRow("debug") << QtDebugMsg << "Ut_MDebug debug" << "Test mDebugStream()";
    QTest::newRow("warning") << QtWarningMsg << "Ut_MDebug warning" << "Test mWarningStream()";
    QTest::newRow("critical") << QtCriticalMsg << "Ut_MDebug critical" << "Test mCriticalStream()";
}

void Ut_MDebug::testInlineFunctions()
{
    QFETCH(QtMsgType, type);
    QFETCH(QString, module);
    QFETCH(QString, message);

    expectedMsgType = type;
    expectedMsg = QString("%1: \"%2\" ").arg(module).arg(message);

    switch (type) {
    case QtDebugMsg:
        mDebug(module) << message;
        break;
    case QtWarningMsg:
        mWarning(module) << message;
        break;
    case QtCriticalMsg:
        mCritical(module) << message;
        break;
    default:
        break;
    }
}

void Ut_MDebug::testDebugTypes_data()
{
    QTest::addColumn<MDebug::Type>("type");
    QTest::addColumn<QtMsgType>("qtMsgType");
    QTest::addColumn<QString>("message");

    QTest::newRow("debug") << MDebug::Debug << QtDebugMsg << "Ut_MDebug test debug type";
    QTest::newRow("warning") << MDebug::Warning << QtWarningMsg << "Ut_MDebug test warning type";
    QTest::newRow("critical") << MDebug::Critical << QtCriticalMsg << "Ut_MDebug test critical type";
    QTest::newRow("system") << MDebug::System << QtSystemMsg << "Ut_MDebug test system type";
    QTest::newRow("performance") << MDebug::Performance << QtDebugMsg << "Ut_MDebug test performance type";
    QTest::newRow("user") << MDebug::User << QtDebugMsg << "Ut_MDebug test user type";
}

void Ut_MDebug::testDebugTypes()
{
    QFETCH(MDebug::Type, type);
    QFETCH(QtMsgType, qtMsgType);
    QFETCH(QString, message);

    expectedMsgType = qtMsgType;
    expectedMsg = QString("%1 ").arg(message);

    MDebug(type) << message;
}

QTEST_APPLESS_MAIN(Ut_MDebug)
