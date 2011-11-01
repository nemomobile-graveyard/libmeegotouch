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

#include "ut_mcomponentdata.h"
#include "mcomponentdata.h"
#include "mcomponentdata_p.h"
#include <MGConfItem>
#include <MApplicationService>

// Test class implementation

void Ut_MComponentData::initTestCase()
{    
}

void Ut_MComponentData::cleanupTestCase()
{
}

void Ut_MComponentData::init()
{
}

void Ut_MComponentData::cleanup()
{
    delete m_componentData;
    m_componentData = 0;
}

void Ut_MComponentData::testEnvironment_softwareRendering()
{
    setenv("M_USE_SOFTWARE_RENDERING", "1", 1);

    static int argc = 2;
    static char *argv[2];
    argv[0] = (char*)"./ut_mcomponentdata";
    argv[1] = (char*)"";

    setenv("M_USE_SOFTWARE_RENDERING", "1", 1);
    m_componentData = MComponentData::createInstance(argc, argv);
    QVERIFY(m_componentData->softwareRendering() == true);
    delete m_componentData;
    m_componentData = 0;

    unsetenv("M_USE_SOFTWARE_RENDERING");
    m_componentData = MComponentData::createInstance(argc, argv);
    QVERIFY(m_componentData->softwareRendering() == false);
}

void Ut_MComponentData::testArguments_data()
{
    QTest::addColumn<int>("argumentId");
    QTest::addColumn<QString>("argument");
    QTest::addColumn<bool>("defaultState");
    QTest::addColumn<bool>("setState");

    int id=1;
    QTest::newRow("softwareRendering") << id++ << "-software" << false << true;
    QTest::newRow("fullScreen") << id++ << "-fullscreen" << false << true;
    QTest::newRow("boundingRect") << id++ << "-show-br" << false << true;
    QTest::newRow("show fps") << id++ << "-show-fps" << false << true;
    QTest::newRow("log fps") << id++ << "-log-fps" << false << true;
    QTest::newRow("show size") << id++ << "-show-size" << false << true;
    QTest::newRow("show position") << id++ << "-show-position" << false << true;
#ifdef __arm__
    bool showCursor(false);
#else
    bool showCursor(true);
#endif
    QTest::newRow("show cursor") << id++ << "-show-cursor" << showCursor << true;
    QTest::newRow("show object names") << id++ << "-show-object-names" << false << true;
    QTest::newRow("show style names") << id++ << "-show-style-names" << false << true;
    QTest::newRow("dev") << id++ << "-dev" << false << true;
}

void Ut_MComponentData::testArguments()
{
    int argc = 2;
    char *argv[2];
    argv[0] = (char*)"./ut_mcomponentdata";
    argv[1] = (char*)"";

    QFETCH(int, argumentId);
    QFETCH(QString, argument);
    QFETCH(bool, defaultState);
    QFETCH(bool, setState);

    m_componentData = MComponentData::createInstance(argc, argv);

    switch (argumentId) {
    case 1:
        QVERIFY(m_componentData->softwareRendering() == defaultState);
        break;
    case 2:
        QVERIFY(m_componentData->fullScreen() == defaultState);
        break;
    case 3:
        QVERIFY(m_componentData->showBoundingRect() == defaultState);
        break;
    case 4:
        QVERIFY(m_componentData->showFps() == defaultState);
        break;
    case 5:
        QVERIFY(m_componentData->logFps() == defaultState);
        break;
    case 6:
        QVERIFY(m_componentData->showSize() == defaultState);
        break;
    case 7:
        QVERIFY(m_componentData->showPosition() == defaultState);
        break;
    case 8:
        QVERIFY(m_componentData->showCursor() == defaultState);
        break;
    case 9:
        QVERIFY(m_componentData->showObjectNames() == defaultState);
        break;
    case 10:
        QVERIFY(m_componentData->showStyleNames() == defaultState);
        break;
    case 11:
        QVERIFY(m_componentData->showSize() == defaultState);
        QVERIFY(m_componentData->showPosition() == defaultState);
        break;
    default:
        break;
    }

    delete m_componentData;
    m_componentData = 0;

    argv[1] = new char[argument.length()+1];
    strcpy(argv[1], argument.toLatin1().data());

    m_componentData = MComponentData::createInstance(argc, argv);

    switch (argumentId) {
    case 1:
        QVERIFY(m_componentData->softwareRendering() == setState);
        break;
    case 2:
        QVERIFY(m_componentData->fullScreen() == setState);
        break;
    case 3:
        QVERIFY(m_componentData->showBoundingRect() == setState);
        break;
    case 4:
        QVERIFY(m_componentData->showFps() == setState);
        break;
    case 5:
        QVERIFY(m_componentData->logFps() == setState);
        break;
    case 6:
        QVERIFY(m_componentData->showSize() == setState);
        break;
    case 7:
        QVERIFY(m_componentData->showPosition() == setState);
        break;
    case 8:
        QVERIFY(m_componentData->showCursor() == setState);
        break;
    case 9:
        QVERIFY(m_componentData->showObjectNames() == setState);
        break;
    case 10:
        QVERIFY(m_componentData->showStyleNames() == setState);
        break;
    case 11:
        QVERIFY(m_componentData->showSize() == setState);
        QVERIFY(m_componentData->showPosition() == setState);
        break;
    default:
        break;
    }

    delete argv[1];
}

void Ut_MComponentData::testArgument_syslogserver_data()
{
    QTest::addColumn<QString>("argumentValue");
    QTest::addColumn<QString>("expectedValue");

    QTest::newRow("local") << "local" << "file:///dev/log";
    QTest::newRow("set") << "file:///dev/testlog" << "file:///dev/testlog";
}

void Ut_MComponentData::testArgument_syslogserver()
{
    int argc = 3;
    char *argv[3];
    argv[0] = (char*)"./ut_mcomponentdata";
    argv[1] = (char*)"-syslog-server";

    QFETCH(QString, argumentValue);
    QFETCH(QString, expectedValue);

    argv[2] = new char[argumentValue.length()+1];
    strcpy(argv[2], argumentValue.toLatin1().data());

    m_componentData = MComponentData::createInstance(argc, argv);
    QCOMPARE(m_componentData->d_func()->syslogServer, QUrl(expectedValue));

    delete argv[2];
}

void Ut_MComponentData::testArgument_target()
{
    int argc = 3;
    char *argv[3];
    argv[0] = (char*)"./ut_mcomponentdata";
    argv[1] = (char*)"";
    argv[2] = (char*)"";

    m_componentData = MComponentData::createInstance(argc, argv);
    QCOMPARE(m_componentData->deviceName(), QString(""));

    delete m_componentData;
    m_componentData = 0;

    argv[1] = (char*)"-target";
    argv[2] = (char*)"targetdevicename";
    QString expected = argv[2];

    m_componentData = MComponentData::createInstance(argc, argv);
    QCOMPARE(m_componentData->deviceName(), expected);
}

void Ut_MComponentData::testArgument_fixedorientation_data()
{
    QTest::addColumn<QString>("argumentValue");
    QTest::addColumn<M::OrientationAngle>("expectedAngle");

    QTest::newRow("0") << "0" << M::Angle0;
    QTest::newRow("90") << "90" << M::Angle90;
    QTest::newRow("180") << "180" << M::Angle180;
    QTest::newRow("270") << "270" << M::Angle270;
}

void Ut_MComponentData::testArgument_fixedorientation()
{
    int argc = 3;
    char *argv[3];
    argv[0] = (char*)"./ut_mcomponentdata";
    argv[1] = (char*)"-fixed-orientation";

    QFETCH(QString, argumentValue);
    QFETCH(M::OrientationAngle, expectedAngle);

    argv[2] = new char[argumentValue.length()+1];
    strcpy(argv[2], argumentValue.toLatin1().data());

    m_componentData = MComponentData::createInstance(argc, argv);

    QCOMPARE(m_componentData->isOrientationForced(), true);
    QCOMPARE( m_componentData->forcedOrientationAngle(), expectedAngle);

    delete argv[2];
}

void Ut_MComponentData::testDebugOptions_data()
{
    QTest::addColumn<int>("bit");

    QTest::newRow("show style names") << 0;
    QTest::newRow("show object names") << 1;
    QTest::newRow("show size") <<  2;
    QTest::newRow("show boundingRect") << 3;
    QTest::newRow("show position") << 4;
    QTest::newRow("show margins") << 5;
    QTest::newRow("show fps") << 6;
}

void Ut_MComponentData::testDebugOptions()
{
    int argc = 1;
    char *argv[1];
    argv[0] = (char*)"./ut_mcomponentdata";
    m_componentData = MComponentData::createInstance(argc, argv);

    QFETCH(int, bit);

    QString gconfBitArray("0000000");
    gconfBitArray[bit] = '1';
    MGConfItem debugOptions("/meegotouch/debug/info_display");
    debugOptions.set(gconfBitArray);

    switch (bit) {
    case 0:
        QVERIFY(m_componentData->showStyleNames() == true);
        break;
    case 1:
        QVERIFY(m_componentData->showObjectNames() == true);
        break;
    case 2:
        QVERIFY(m_componentData->showSize() == true);
        break;
    case 3:
        QVERIFY(m_componentData->showBoundingRect() == true);
        break;
    case 4:
        QVERIFY(m_componentData->showPosition() == true);
        break;
    case 5:
        QVERIFY(m_componentData->showMargins() == true);
        break;
    case 6:
        QVERIFY(m_componentData->showFps() == true);
        break;
    default:
        break;
    }

    debugOptions.unset();
}

void Ut_MComponentData::testVariousSetters()
{
    int argc = 1;
    char *argv[1];
    argv[0] = (char*)"./ut_mcomponentdata";
    m_componentData = MComponentData::createInstance(argc, argv);

    m_componentData->setShowPosition(false);
    QVERIFY(m_componentData->showPosition() == false);
    m_componentData->setShowPosition(true);
    QVERIFY(m_componentData->showPosition() == true);

    m_componentData->setShowSize(false);
    QVERIFY(m_componentData->showSize() == false);
    m_componentData->setShowSize(true);
    QVERIFY(m_componentData->showSize() == true);

    m_componentData->setShowMargins(false);
    QVERIFY(m_componentData->showMargins() == false);
    m_componentData->setShowMargins(true);
    QVERIFY(m_componentData->showMargins() == true);

    m_componentData->setShowObjectNames(false);
    QVERIFY(m_componentData->showObjectNames() == false);
    m_componentData->setShowObjectNames(true);
    QVERIFY(m_componentData->showObjectNames() == true);

    m_componentData->setShowStyleNames(false);
    QVERIFY(m_componentData->showStyleNames() == false);
    m_componentData->setShowStyleNames(true);
    QVERIFY(m_componentData->showStyleNames() == true);

    m_componentData->setShowBoundingRect(false);
    QVERIFY(m_componentData->showBoundingRect() == false);
    m_componentData->setShowBoundingRect(true);
    QVERIFY(m_componentData->showBoundingRect() == true);

    m_componentData->setShowFps(false);
    QVERIFY(m_componentData->showFps() == false);
    m_componentData->setShowFps(true);
    QVERIFY(m_componentData->showFps() == true);

    m_componentData->setLogFps(false);
    QVERIFY(m_componentData->logFps() == false);
    m_componentData->setLogFps(true);
    QVERIFY(m_componentData->logFps() == true);

    m_componentData->setShowCursor(false);
    QVERIFY(m_componentData->showCursor() == false);
    m_componentData->setShowCursor(true);
    QVERIFY(m_componentData->showCursor() == true);

    m_componentData->setPrestarted(false);
    QVERIFY(m_componentData->prestarted() == false);
    m_componentData->setPrestarted(true);
    QVERIFY(m_componentData->prestarted() == true);

    m_componentData->setPrestartMode(M::NoPrestart);
    QVERIFY(m_componentData->prestartMode() == M::NoPrestart);
    m_componentData->setPrestartMode(M::LazyShutdown);
    QVERIFY(m_componentData->prestartMode() == M::LazyShutdown);
    m_componentData->setPrestartMode(M::TerminateOnClose);
    QVERIFY(m_componentData->prestartMode() == M::TerminateOnClose);
    m_componentData->setPrestartMode(M::LazyShutdownMultiWindow);
    QVERIFY(m_componentData->prestartMode() == M::LazyShutdownMultiWindow);
    m_componentData->setPrestartMode(M::TerminateOnCloseMultiWindow);
    QVERIFY(m_componentData->prestartMode() == M::TerminateOnCloseMultiWindow);

    m_componentData->setEmulateTwoFingerGestures(false);
    QVERIFY(m_componentData->emulateTwoFingerGestures() == false);
    m_componentData->setEmulateTwoFingerGestures(true);
    QVERIFY(m_componentData->emulateTwoFingerGestures() == true);

    m_componentData->setLoadMInputContext(false);
    QVERIFY(m_componentData->isLoadMInputContextEnabled() == false);
    m_componentData->setLoadMInputContext(true);
    QVERIFY(m_componentData->isLoadMInputContextEnabled() == true);
}

void Ut_MComponentData::testNames()
{
    int argc = 1;
    char *argv[1];
    argv[0] = (char*)"./ut_mcomponentdata";
    QString serviceName("com.nokia.appName");
    QString appIdentifier("testapp");

    MApplicationService *service = new MApplicationService(serviceName);
    m_componentData = MComponentData::createInstance(argc, argv, appIdentifier, service);

    QCOMPARE(m_componentData->appName(), appIdentifier);
    QCOMPARE(m_componentData->binaryName(), QString(argv[0]));
    QCOMPARE(m_componentData->serviceName(), serviceName);
}

void Ut_MComponentData::testChainData()
{
    int argc = 1;
    char *argv[1];
    argv[0] = (char*)"./ut_mcomponentdata";
    m_componentData = MComponentData::createInstance(argc, argv);

    MComponentData::ChainData data1(1, QString("testChainData1"));
    m_componentData->pushChainData(data1);
    QVERIFY(m_componentData->chainDataStackIsEmpty() == false);

    MComponentData::ChainData data2(2, QString("testChainData2"));
    MComponentData::pushChainData(data2);
    QVERIFY(m_componentData->chainDataStackIsEmpty() == false);
    QCOMPARE(m_componentData->lastChainParentWinId(), (WId)2);

    MComponentData::ChainData popped = m_componentData->popChainData();
    QVERIFY(m_componentData->chainDataStackIsEmpty() == false);
    QCOMPARE(m_componentData->lastChainParentWinId(), (WId)1);
    QCOMPARE(popped.first, (WId)2);
    QCOMPARE(popped.second, QString("testChainData2"));

    popped = m_componentData->popChainData();
    QVERIFY(m_componentData->chainDataStackIsEmpty() == true);
    QCOMPARE(m_componentData->lastChainParentWinId(), (WId)0);
    QCOMPARE(popped.first, (WId)1);
    QCOMPARE(popped.second, QString("testChainData1"));
}


QTEST_MAIN(Ut_MComponentData);
