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

#ifndef UT_MWIDGETCONTROLLER_H
#define UT_MWIDGETCONTROLLER_H

#include <QtTest/QtTest>
#include <QObject>

class TestWidgetController;
class MWidgetController;
class MApplication;
class MWidgetView;

class Ut_MWidgetController : public QObject
{
    Q_OBJECT

public:
    static bool viewCreatesChildWidgets;
    static bool viewSetsItselfActive;

private:
    MApplication *app;
    TestWidgetController *controller;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testBoundingRect();
    void testSetGeometry_data();
    void testSetGeometry();
    void testSizeHint();
    void testActiveStateWhenViewIsSet();
    void testActiveStateWhenViewIsUnset();
    void testUpdateData();
};

#endif // UT_MWIDGETCONTROLLER_H

