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

#ifndef UT_DUIWIDGETCONTROLLER_H
#define UT_DUIWIDGETCONTROLLER_H

#include <QtTest/QtTest>
#include <QObject>

class TestWidgetController;
class DuiWidgetController;
class DuiApplication;
class DuiWidgetView;

class Ut_DuiWidgetController : public QObject
{
    Q_OBJECT

public:
    static bool viewCreatesChildWidgets;
    static bool viewSetsItselfActive;

private:
    DuiApplication *app;
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
};

#endif // UT_DUIWIDGETCONTROLLER_H

