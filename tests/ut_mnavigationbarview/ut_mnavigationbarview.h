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

#ifndef UT_MNAVIGATIONBARVIEW_H
#define UT_MNAVIGATIONBARVIEW_H

#include <QtTest/QtTest>
#include <QObject>

#include <views/mnavigationbarview.h>
#include <MNavigationBar>
#include <MApplication>

class Ut_MNavigationBarView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testCustomContentOwnershipTransfer();
    void testCustomContentPolicySelection();

    // test "isEmpty" property
    void testIsEmptyProperty();
    void testIsEmptyProperty_data();
    void testDockedToolBarChangingItsIsEmptyProperty();
    void testToolBarViewTypeChange();

private:
    MNavigationBarView *subject;
    MNavigationBar *controller;
    MNavigationBarModel *model;
    MApplication *app;
};

#endif
