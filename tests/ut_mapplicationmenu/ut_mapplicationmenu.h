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

#ifndef UT_MAPPLICATIONMENU_H
#define UT_MAPPLICATIONMENU_H

#include <QtTest/QtTest>
#include <QObject>

class MAction;
class MApplicationMenu;
class MApplicationMenuViewPrivate;
class MMenuListItem;

class Ut_MApplicationMenu : public QObject
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
    void testSetWidgetAfterWidgetActionIsAdded();
    void testChangeWidgetInWidgetAction();
    void testRemoveWidgetFromWidgetAction();
    void testActionVisiblity();
    void testOpeningAndClosingAppMenuWithDisabledAction();
    void testClear();
    void testPaint();
    void testEventsPassingThrough();
    void testRotation();
    void actionSlot(bool checked);

private:
    MApplicationMenu    *m_subject;

    MAction  *senderAction;
    bool        testValue;

    const MApplicationMenuViewPrivate* menuViewPrivate();
    MMenuListItem* getMenuItem(const MApplicationMenuViewPrivate *viewPrivate, int actionNumber);
};

#endif
