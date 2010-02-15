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

#include <DuiApplication>
#include <duiapplicationmenu.h>

#include "ut_duiapplicationmenuview.h"

DuiApplication *app;

void Ut_DuiApplicationMenuView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_DuiApplicationMenuView" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiApplicationMenuView::cleanupTestCase()
{
    delete app;
}

void Ut_DuiApplicationMenuView::init()
{
    m_menu = new DuiApplicationMenu();
    m_subject = new DuiApplicationMenuView(m_menu);
}

void Ut_DuiApplicationMenuView::cleanup()
{
    delete m_menu;
    m_menu = 0;
    // menu destruction also destroys the view
}

void Ut_DuiApplicationMenuView::testSetView()
{
    QVERIFY(m_menu != 0);
    QVERIFY(m_subject != 0);

    m_menu->setView(m_subject);
}

QTEST_APPLESS_MAIN(Ut_DuiApplicationMenuView)
