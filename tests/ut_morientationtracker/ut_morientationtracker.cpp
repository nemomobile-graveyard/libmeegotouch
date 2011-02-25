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

#include "ut_morientationtracker.h"

#include <mdeviceprofile.h>

#include <MComponentData>
#include <MWindow>
#include <MApplicationWindow>
#include <MOnDisplayChangeEvent>

namespace {
    enum { KeyboardOpen = 0, KeyboardClosed };
    QList<M::OrientationAngle> supportedAnglesStubLists[2];
}

bool MDeviceProfile::orientationAngleIsSupported(M::OrientationAngle angle, bool isKeyboardOpen) const
{
    if (isKeyboardOpen) {
        return supportedAnglesStubLists[KeyboardOpen].contains(angle);
    } else {
        return supportedAnglesStubLists[KeyboardClosed].contains(angle);
    }
}

void Ut_MOrientationTracker::init()
{
    window1 = new MWindow;
    window2 = new MWindow;
}

void Ut_MOrientationTracker::cleanup()
{
    delete window1;
    delete window2;
    supportedAnglesStubLists[KeyboardOpen].clear();
    supportedAnglesStubLists[KeyboardOpen].clear();
}

void Ut_MOrientationTracker::initTestCase()
{
    supportedAnglesStubLists[KeyboardOpen] << M::Angle0;
    supportedAnglesStubLists[KeyboardClosed] << M::Angle0;
    static int argc = 1;
    static char *argv[ 1 ];
    argv[ 0 ] = (char*)"./ut_mwindow";
    m_componentData = new MComponentData(argc, argv);

    mTracker = MOrientationTracker::instance();
}

void Ut_MOrientationTracker::cleanupTestCase()
{
    delete m_componentData;
}

void Ut_MOrientationTracker::testOrientationPolicyWithoutConstraints_data()
{
    QTest::addColumn<M::OrientationAngle>("newAngle");
    QTest::addColumn<bool>("newKeyboardState");
    QTest::newRow("Angle0, keyboard open") << M::Angle0 << true;
    QTest::newRow("Angle90, keyboard open") << M::Angle90 << true;
    QTest::newRow("Angle180, keyboard open") << M::Angle180 << true;
    QTest::newRow("Angle270, keyboard open") << M::Angle270 << true;
    QTest::newRow("Angle0, keyboard closed") << M::Angle0 << false;
    QTest::newRow("Angle90, keyboard closed") << M::Angle90 << false;
    QTest::newRow("Angle180, keyboard closed") << M::Angle180 << false;
    QTest::newRow("Angle270, keyboard closed") << M::Angle270 << false;
}

void Ut_MOrientationTracker::testOrientationPolicyWithoutConstraints()
{
    QFETCH(M::OrientationAngle, newAngle);
    QFETCH(bool, newKeyboardState);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    mTracker->doUpdateOrientationAngle(newAngle, newKeyboardState, false, false);

    QCOMPARE(window1->orientationAngle(), newAngle);
    QCOMPARE(window2->orientationAngle(), newAngle);
}

void Ut_MOrientationTracker::testOrientationPolicyWithTVConnectedConstraint_data()
{
    QTest::addColumn<M::OrientationAngle>("newAngle");
    QTest::addColumn<bool>("newKeyboardState");
    QTest::newRow("Angle0, keyboard open") << M::Angle0 << true;
    QTest::newRow("Angle90, keyboard open") << M::Angle90 << true;
    QTest::newRow("Angle180, keyboard open") << M::Angle180 << true;
    QTest::newRow("Angle270, keyboard open") << M::Angle270 << true;
    QTest::newRow("Angle0, keyboard closed") << M::Angle0 << false;
    QTest::newRow("Angle90, keyboard closed") << M::Angle90 << false;
    QTest::newRow("Angle180, keyboard closed") << M::Angle180 << false;
    QTest::newRow("Angle270, keyboard closed") << M::Angle270 << false;
}

void Ut_MOrientationTracker::testOrientationPolicyWithTVConnectedConstraint()
{
    QFETCH(M::OrientationAngle, newAngle);
    QFETCH(bool, newKeyboardState);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    mTracker->doUpdateOrientationAngle(newAngle, newKeyboardState, false, true);

    QCOMPARE(window1->orientationAngle(), M::Angle0);
    QCOMPARE(window2->orientationAngle(), M::Angle0);
}

void Ut_MOrientationTracker::testOrientationPolicyDeviceFlatAndKeyboardJustClosed_data()
{
    QTest::addColumn<M::OrientationAngle>("firstAngle");
    QTest::addColumn<M::OrientationAngle>("secondAngle");

    QTest::newRow("Angle0 -> 90") << M::Angle0 << M::Angle90;
    QTest::newRow("Angle90 -> 180") << M::Angle90 << M::Angle180;
    QTest::newRow("Angle180 -> 270") << M::Angle180 << M::Angle270;
    QTest::newRow("Angle270 -> 0") << M::Angle270 << M::Angle0;
}

void Ut_MOrientationTracker::testOrientationPolicyDeviceFlatAndKeyboardJustClosed()
{
    QFETCH(M::OrientationAngle, firstAngle);
    QFETCH(M::OrientationAngle, secondAngle);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    //set in normal way
    mTracker->doUpdateOrientationAngle(firstAngle, true, false, false);

    //emulate state - keyboard just closed while device flat - no rotation
    mTracker->doUpdateOrientationAngle(secondAngle, false, true, false);

    QCOMPARE(window1->orientationAngle(), firstAngle);
    QCOMPARE(window2->orientationAngle(), firstAngle);
}

void Ut_MOrientationTracker::testOrientationPolicyDeviceProfileConstraints_data()
{
    QTest::addColumn<M::OrientationAngle>("allowedAngle");
    QTest::addColumn<M::OrientationAngle>("sensorAngle");

    QTest::newRow("Angle90, allowed 0") << M::Angle0 << M::Angle90;
    QTest::newRow("Angle180, allowed 90") << M::Angle90 << M::Angle180;
    QTest::newRow("Angle270, allowed 180") << M::Angle180 << M::Angle270;
    QTest::newRow("Angle0, allowed 270") << M::Angle270 << M::Angle0;
}

void Ut_MOrientationTracker::testOrientationPolicyDeviceProfileConstraints()
{
    QFETCH(M::OrientationAngle, allowedAngle);
    QFETCH(M::OrientationAngle, sensorAngle);

    supportedAnglesStubLists[KeyboardOpen] << allowedAngle;
    supportedAnglesStubLists[KeyboardClosed] << allowedAngle;

    mTracker->doUpdateOrientationAngle(sensorAngle, true, false, false);

    QCOMPARE(window1->orientationAngle(), allowedAngle);
    QCOMPARE(window2->orientationAngle(), allowedAngle);
}

void Ut_MOrientationTracker::testWindowOrientationAngleLock_data()
{
    QTest::addColumn<M::OrientationAngle>("lockedAngle1");
    QTest::addColumn<M::OrientationAngle>("lockedAngle2");
    QTest::addColumn<M::OrientationAngle>("sensorAngle");

    QTest::newRow("Angle90, locked 0, 270") << M::Angle0 << M::Angle270 << M::Angle90;
    QTest::newRow("Angle180, locked 90, 0") << M::Angle90 << M::Angle0 << M::Angle180;
    QTest::newRow("Angle270, locked 180, 90") << M::Angle180 << M::Angle90 << M::Angle270;
    QTest::newRow("Angle0, locked 270, 180") << M::Angle270 << M::Angle180 << M::Angle0;
}

void Ut_MOrientationTracker::testWindowOrientationAngleLock()
{
    QFETCH(M::OrientationAngle, lockedAngle1);
    QFETCH(M::OrientationAngle, lockedAngle2);
    QFETCH(M::OrientationAngle, sensorAngle);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    window1->setOrientationAngle(lockedAngle1);
    window1->lockOrientationAngle();

    window2->setOrientationAngle(lockedAngle2);
    window2->lockOrientationAngle();

    mTracker->doUpdateOrientationAngle(sensorAngle, true, false, false);

    QCOMPARE(window1->orientationAngle(), lockedAngle1);
    QCOMPARE(window2->orientationAngle(), lockedAngle2);
}

void Ut_MOrientationTracker::testWindowOrientationLock_data()
{
    QTest::addColumn<M::OrientationAngle>("lockedAngle1");
    QTest::addColumn<M::OrientationAngle>("lockedAngle2");
    QTest::addColumn<M::OrientationAngle>("sensorAngle");

    QTest::newRow("Angle90, locked 0, 270") << M::Angle0 << M::Angle270 << M::Angle90;
    QTest::newRow("Angle180, locked 90, 0") << M::Angle90 << M::Angle0 << M::Angle180;
    QTest::newRow("Angle270, locked 180, 90") << M::Angle180 << M::Angle90 << M::Angle270;
    QTest::newRow("Angle0, locked 270, 180") << M::Angle270 << M::Angle180 << M::Angle0;
}

void Ut_MOrientationTracker::testWindowOrientationLock()
{
    QFETCH(M::OrientationAngle, lockedAngle1);
    QFETCH(M::OrientationAngle, lockedAngle2);
    QFETCH(M::OrientationAngle, sensorAngle);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    window1->setOrientationAngle(lockedAngle1);
    window1->lockOrientation();
    M::Orientation orientation1 = (lockedAngle1 == M::Angle90 || lockedAngle1 == M::Angle270)?
                                  M::Portrait : M::Landscape;

    window2->setOrientationAngle(lockedAngle2);
    window2->lockOrientation();
    M::Orientation orientation2 = (lockedAngle2 == M::Angle90 || lockedAngle2 == M::Angle270)?
                                  M::Portrait : M::Landscape;

    mTracker->doUpdateOrientationAngle(sensorAngle, true, false, false);

    QCOMPARE(window1->orientation(), orientation1);
    QCOMPARE(window2->orientation(), orientation2);
}

///////////////////////////////////////////////////////
//////////////////HELPER FUNCTIONS/////////////////////
///////////////////////////////////////////////////////

void Ut_MOrientationTracker::setAllAngles(QList<M::OrientationAngle> *list)
{
    *list << M::Angle0 << M::Angle90 << M::Angle180 << M::Angle270;
}

QTEST_MAIN(Ut_MOrientationTracker);

