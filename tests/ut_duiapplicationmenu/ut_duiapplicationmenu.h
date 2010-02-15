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

#ifndef UT_DUIAPPLICATIONMENU_H
#define UT_DUIAPPLICATIONMENU_H

#include <QtTest/QtTest>
#include <QObject>

class DuiAction;
class DuiApplicationMenu;

class Ut_DuiApplicationMenu : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    // class testing
    void testConstructor();
    void testDestructor();
    void testAddAction();
    void testInsertAction();
    void testActionVisiblity();
    void testClear();
    void testPaint();
    void testEventsPassingThrough();

    void actionSlot(bool checked);

private:
    DuiApplicationMenu    *m_subject;

    DuiAction  *senderAction;
    bool        testValue;
};

#endif
