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

QTEST_APPLESS_MAIN(Ut_MApplicationMenuView)
