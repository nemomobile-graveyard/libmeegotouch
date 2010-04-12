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

#ifndef UT_MSERVICEFWBASEIF_H
#define UT_MSERVICEFWBASEIF_H

#include <QtTest/QtTest>
#include <QObject>

#include "mservicefwbaseif.h"


Q_DECLARE_METATYPE(MServiceFwBaseIf *);

class Ut_MServiceFwBaseIf : public QObject
{
    Q_OBJECT

public:
    static QString serviceFwService;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    /*
     * no current service; new service for other if
     * should not get signal:serviceAvailable
     * should not get signal:serviceChanged
     */
    void testHandleServiceAvailable0();

    /*
     * no current service; new service for this if
     * should get signal:serviceAvailable
     * should get signal:serviceChanged
     */
    void testHandleServiceAvailable1();

    /*
     * current service; new preferred service for other if
     * should not get signal:serviceAvilable
     * should get signal:serviceChanged
     */
    void testHandleServiceAvailable2();

    /*
     * current service; new preferred service for this if
     * should not get signal:serviceAvilable
     * should get signal:serviceChanged
     */
    void testHandleServiceAvailable3();

    /*
     * no current service; dead service is for other if
     * should not get serviceUnavailable
     * should not get serviceChanged
     */
    void testHandleServiceUnavailable0();

    /*
     * current service; dead service is for other if
     * should not get serviceUnavailable
     * should not get serviceChanged
     */
    void testHandleServiceUnavailable1();

    /*
     * current service; dead service is not last for this if
     * should not get serviceUnavailable
     * should not get serviceChanged
     */
    void testHandleServiceUnavailable2();

    /*
     * current service; dead service is last for this if
     * should not get serviceUnavailable
     * should not get serviceChanged
     */
    void testHandleServiceUnavailable3();

    void testResolveServiceName();

private:
    MServiceFwBaseIf *m_subject;

};
#endif
