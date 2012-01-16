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

#ifndef UT_MLISTITEM_H
#define UT_MLISTITEM_H

#include <QtTest/QtTest>

class MApplication;
class MListItem;

class Ut_MListItem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    // class testing
    void testClick();
    void testLongTap();

    void testReleasedOnHide();

    void testMouseClick();
    void testMouseMoveOutside();
    void testMouseMoveWithinMaxDistance();
    void testMouseMoveBeyondMaxDistance();
    void testCancelEvent();
    void testTapAndHold();

 private:
    MApplication *app;
    MListItem *listItem;

};

#endif // UT_MLISTITEM_H
