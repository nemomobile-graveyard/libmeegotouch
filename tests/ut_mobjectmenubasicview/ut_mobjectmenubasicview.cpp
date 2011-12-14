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

#include <MApplication>
#include <MApplicationWindow>
#include <mobjectmenu.h>

#include <MAction>
#include <MButton>
#include <MLabel>
#include <QSignalSpy>

#include "ut_mobjectmenubasicview.h"

namespace {
    const char *IconId = "icon-l-browser";
};

MApplication *app;

void Ut_MObjectMenuBasicView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MObjectMenuBasicView" };
    app = new MApplication(argc, app_name);
}

void Ut_MObjectMenuBasicView::cleanupTestCase()
{
    delete app;
}

void Ut_MObjectMenuBasicView::init()
{
    m_menu = new MObjectMenu(0);
    m_subject = new MObjectMenuBasicView(m_menu);
}

void Ut_MObjectMenuBasicView::cleanup()
{
    delete m_menu;
    m_menu = 0;
}

void Ut_MObjectMenuBasicView::testSetView()
{
    QVERIFY(m_menu != 0);
    QVERIFY(m_subject != 0);

    m_menu->setView(m_subject);
}

void Ut_MObjectMenuBasicView::testActions()
{
    m_menu->setView(m_subject);

    MAction* menuAction = new MAction("menuAction",m_menu);
    menuAction->setLocation(MAction::ObjectMenuLocation);

    MAction* toolbarAction = new MAction("toolbarAction",m_menu);
    toolbarAction->setLocation(MAction::ToolBarLocation);

    MAction* everyLocationAction = new MAction("everyLocationAction",m_menu);
    everyLocationAction->setLocation(MAction::EveryLocation);

    m_menu->addAction(menuAction);
    QCOMPARE(m_subject->buttons.size(), 1);
    QVERIFY(m_subject->buttons.contains(menuAction));
    QCOMPARE(m_subject->buttons.value(menuAction)->text(), menuAction->text());


    m_menu->addAction(toolbarAction); // this action should be ignored by menu
    QCOMPARE(m_subject->buttons.size(), 1);
    QVERIFY(!m_subject->buttons.contains(toolbarAction));


    m_menu->addAction(everyLocationAction);
    QCOMPARE(m_subject->buttons.size(), 2);
    QVERIFY(m_subject->buttons.contains(everyLocationAction));
    QCOMPARE(m_subject->buttons.value(everyLocationAction)->text(), everyLocationAction->text());

    m_menu->removeAction(menuAction);
    QCOMPARE(m_subject->buttons.size(), 1);
    QVERIFY(!m_subject->buttons.contains(menuAction));

    m_menu->removeAction(toolbarAction);
    QCOMPARE(m_subject->buttons.size(), 1);
    QVERIFY(!m_subject->buttons.contains(menuAction));

    m_menu->removeAction(everyLocationAction);
    QCOMPARE(m_subject->buttons.size(), 0);
    QVERIFY(!m_subject->buttons.contains(everyLocationAction));
}

void Ut_MObjectMenuBasicView::testButtons()
{
    m_menu->setView(m_subject);

    MAction* menuAction = new MAction("menuAction",m_menu);
    menuAction->setLocation(MAction::ObjectMenuLocation);
    menuAction->setText("menuAction");
    menuAction->setIconID(IconId);
    m_menu->addAction(menuAction);

    QCOMPARE(m_subject->buttons.size(), 1);
    QCOMPARE(m_subject->buttons.value(menuAction)->text(), menuAction->text());
    QCOMPARE(m_subject->buttons.value(menuAction)->iconID(), menuAction->iconID());

    QSignalSpy spy(menuAction, SIGNAL(triggered()));
    m_subject->buttons.value(menuAction)->click();
    QCOMPARE(spy.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_MObjectMenuBasicView)

