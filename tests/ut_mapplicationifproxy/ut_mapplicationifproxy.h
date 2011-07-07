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

#ifndef UT_MAPPLICATIONIFPROXY_H
#define UT_MAPPLICATIONIFPROXY_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class MApplicationIfProxy;

class Ut_MApplicationIfProxy : public QObject
{
    Q_OBJECT

public:
    static bool launchCalled;
    static bool launchCalledWithParams;
    static bool exitCalled;
    static bool closeCalled;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testLaunch();
    void testLaunchWithParams();
    void testClose();
    void testExit();

private:
    MApplication *app;
    MApplicationIfProxy *subject;
};

#endif
