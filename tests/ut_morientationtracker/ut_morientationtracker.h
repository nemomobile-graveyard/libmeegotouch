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

#ifndef UT_MORIENTATIONTRACKER_H
#define UT_MORIENTATIONTRACKER_H

#include <QtTest/QtTest>
#include <QObject>

#include "morientationtracker.h"

class MWindow;
//class MApplication;
class MComponentData;

#define MAX_PARAMS 10
class Ut_MOrientationTracker: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testOrientationPolicyWithoutConstraints_data();
    void testOrientationPolicyWithoutConstraints();
    void testOrientationPolicyWithTVConnectedConstraint_data();
    void testOrientationPolicyWithTVConnectedConstraint();
    void testOrientationPolicyDeviceFlatAndKeyboardJustClosed_data();
    void testOrientationPolicyDeviceFlatAndKeyboardJustClosed();
    void testOrientationPolicyDeviceProfileConstraints_data();
    void testOrientationPolicyDeviceProfileConstraints();

    void testWindowOrientationAngleLock_data();
    void testWindowOrientationAngleLock();
    void testWindowOrientationLock_data();
    void testWindowOrientationLock();
    void testWindowRemoteOrientationLock_data();
    void testWindowRemoteOrientationLock();

    void testUpdatesPostponedUntilRotationsAreEnabled();

private:
    /*  Helper members   */

    //enables all angles for device profile
    void setAllAngles(QList<M::OrientationAngle>* list);

    MOrientationTracker *mTracker;
    MComponentData* m_componentData;
    MWindow *window1;
    MWindow *window2;
};

#endif
