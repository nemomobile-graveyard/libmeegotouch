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

#include <MApplication>
#include <mapplicationmenu.h>

#include "mapplicationmenuview_p.h"

#include "ut_mapplicationmenuview.h"

MApplication *app;

void Ut_MApplicationMenuView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MApplicationMenuView" };
    app = new MApplication(argc, app_name);
}

void Ut_MApplicationMenuView::cleanupTestCase()
{
    delete app;
}

void Ut_MApplicationMenuView::init()
{
    m_menu = new MApplicationMenu();
    m_subject = new MApplicationMenuView(m_menu);
}

void Ut_MApplicationMenuView::cleanup()
{
    delete m_menu;
    m_menu = 0;
    // menu destruction also destroys the view
}

void Ut_MApplicationMenuView::testSetView()
{
    QVERIFY(m_menu != 0);
    QVERIFY(m_subject != 0);

    m_menu->setView(m_subject);
}

void Ut_MApplicationMenuView::testCalculateLayoutPosition()
{
    // Empty menu
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(0, 0, 0), M::DefaultPosition);

    // Single item in menu
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(1, 1, 0), M::DefaultPosition);

    // Single column
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(1, 3, 0), M::VerticalTopPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(1, 3, 1), M::VerticalCenterPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(1, 3, 2), M::VerticalBottomPosition);

    // Single row
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 3, 0), M::HorizontalLeftPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 3, 1), M::HorizontalCenterPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 3, 2), M::HorizontalRightPosition);

    // 2D 3x3 layout
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 9, 0), M::TopLeftPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 9, 1), M::TopCenterPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 9, 2), M::TopRightPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 9, 3), M::CenterLeftPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 9, 4), M::CenterPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 9, 5), M::CenterRightPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 9, 6), M::BottomLeftPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 9, 7), M::BottomCenterPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 9, 8), M::BottomRightPosition);

    // Incomplete last row
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(2, 3, 1), M::TopRightPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(2, 5, 3), M::CenterRightPosition);

    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 4, 3), M::BottomLeftPosition);
    QCOMPARE(MApplicationMenuViewPrivate::calculateLayoutPosition(3, 5, 4), M::BottomCenterPosition);
}

QTEST_APPLESS_MAIN(Ut_MApplicationMenuView)
