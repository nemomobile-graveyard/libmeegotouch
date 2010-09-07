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

#ifndef UT_MTOOLBAR_H
#define UT_MTOOLBAR_H

// include unit test headers
#include <QtTest/QtTest>
#include <QObject>
#include <MToolBar>

class MAction;

class Ut_MToolBar : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    // test system init and cleanup
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    // class testing
    void testConstructor();
    void testConstructor2();
    void testDestructor();
    void testAddAction();
    void testAddTextEditAction();
    void testInsertAction();
    void testActionVisiblity();
    void testAddTabAction();
    void testRemoveTabAction();
    void testNoLocation();
    void testPropertyChange();

    void actionSlot(bool);

private:
    MToolBar *m_subject;

    MAction *senderAction;
    bool       testValue;
};

#endif
