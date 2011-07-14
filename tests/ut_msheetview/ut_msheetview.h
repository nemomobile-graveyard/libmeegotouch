/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_MSHEETVIEW_H
#define UT_MSHEETVIEW_H

#include <QObject>
#include <QtTest/QtTest>

class MSheetView;
class MApplication;
class MApplicationPage;
class MSheet;
class MWindow;

class Ut_MSheetView : public QObject
{
    Q_OBJECT
public:
    Ut_MSheetView();

private slots:
    void initTestCase();     // called once before all tests
    void cleanupTestCase(); // called once after all tests
    void init(); // called before every single test
    void cleanup(); // called after every single test

    void testHeaderHiding();

private:
    MApplication* app;
    MWindow* window;
    MSheetView *subject;
    MSheet* sheet;
};

#endif

