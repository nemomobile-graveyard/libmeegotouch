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

#include "ut_duitoolbar.h"

#include <DuiToolBar>
#include <DuiToolBarView>
#include <DuiApplication>
#include <DuiApplicationWindow>

#include <DuiButton>
#include <DuiTextEdit>
#include <DuiSlider>
#include <DuiButtonGroup>
#include <DuiWidgetAction>

DuiApplication *app;
// TODO: remove this when unneeded in DuiTextEdit's constructor
DuiApplicationWindow *appWin;

void Ut_DuiToolBar::init()
{
    m_subject = new DuiToolBar();
}

void Ut_DuiToolBar::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiToolBar::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duitoolbar" };
    app = new DuiApplication(argc, app_name);
    appWin = new DuiApplicationWindow;
}

void Ut_DuiToolBar::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_DuiToolBar::testConstructor()
{
    QVERIFY(m_subject);
}

void Ut_DuiToolBar::testDestructor()
{
    DuiToolBar *toolbar = new DuiToolBar();
    delete toolbar;
}

void Ut_DuiToolBar::testAddAction()
{
    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    // return value action
    DuiAction *action;

    // test addAction(const QString &text)
    DuiAction *action0 = new DuiAction("TEXTONLY", m_subject);
    m_subject->addAction(action0);
    action = qobject_cast<DuiAction *>(m_subject->actions().at(0));
    // must be one
    QVERIFY(m_subject->actions().count() == 1);
    QVERIFY(action == action0);
    QVERIFY(action->text() == "TEXTONLY");

    // test addAction(const QString &icon, const QString &text)
    DuiAction *action1 = new DuiAction("Icon-list-view-on", "TEXT", m_subject);
    m_subject->addAction(action1);
    QVERIFY(m_subject->actions().count() == 2);
    action = qobject_cast<DuiAction *>(m_subject->actions().at(1));
    QVERIFY(action == action1);
    QVERIFY(action->iconID() == "Icon-list-view-on");
    QVERIFY(action->text() == "TEXT");

    testValue = false;
    senderAction = NULL;
    connect(action1, SIGNAL(triggered(bool)), this, SLOT(actionSlot(bool)));
    action1->trigger();
    QVERIFY(testValue);
    QVERIFY(senderAction == action1);

    // test addAction(DuiAction* action)
    DuiAction *action2 = new DuiAction("Icon-time-line-on", "TEXT4", NULL);
    m_subject->addAction(action2);
    QVERIFY(m_subject->actions().count() == 3);

    action = qobject_cast<DuiAction *>(m_subject->actions().at(2));
    QVERIFY(action == action2);
    delete action2;

}

void Ut_DuiToolBar::testAddTextEditAction()
{
    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    // test addAction(Text Edit)
    DuiTextEdit *textEntry = new DuiTextEdit();
    DuiWidgetAction *actionTextEdit = new DuiWidgetAction(m_subject);
    actionTextEdit->setWidget(textEntry);

    m_subject->addAction(actionTextEdit);
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(m_subject->actions().at(0));

    // must be one
    QVERIFY(m_subject->actions().count() == 1);
    QVERIFY(widgetAction);
    QVERIFY(widgetAction == actionTextEdit);
    QVERIFY(widgetAction->widget() == textEntry);
}

void Ut_DuiToolBar::testInsertAction()
{
    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    //adding text edit
    DuiTextEdit *textEntry = new DuiTextEdit();
    DuiWidgetAction *actionTextEdit = new DuiWidgetAction(m_subject);
    actionTextEdit->setWidget(textEntry);

    m_subject->addAction(actionTextEdit); //first action, indx 0

    //adding actions
    DuiAction *action0 = new DuiAction("action0", m_subject);
    m_subject->addAction(action0); //second action, indx 1

    DuiAction *action1 = new DuiAction("action1", m_subject);
    m_subject->insertAction(action0, action1);   //third action, indx1 -> action0 goes to indx 2

    QVERIFY(m_subject->actions().count() == 3);

    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(m_subject->actions().at(0)); // getting indx 0
    DuiAction *action = qobject_cast<DuiAction *>(m_subject->actions().at(1)); //getting indx 1

    //veryfing:
    QVERIFY(widgetAction);
    QVERIFY(widgetAction  == actionTextEdit);
    QVERIFY(action);
    QVERIFY(action  == action1);

    action = qobject_cast<DuiAction *>(m_subject->actions().at(2)); //getting indx 2
    QVERIFY(action);
    QVERIFY(action  == action0);

}

void Ut_DuiToolBar::testActionVisiblity()
{
    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    //adding actions:
    DuiAction *action0 = new DuiAction("action0", m_subject);
    m_subject->addAction(action0); //first action, indx 0

    DuiAction *action1 = new DuiAction("action1", m_subject);
    m_subject->addAction(action1); //second action, indx 1

    DuiAction *action2 = new DuiAction("action2", m_subject);
    m_subject->addAction(action2); //third action, indx 2

    DuiAction *action3 = new DuiAction("action3", m_subject);
    m_subject->addAction(action3); //fourth action, indx 3

    QVERIFY(m_subject->actions().count() == 4);

    DuiAction *action; //needed for veryfing

    action0->setVisible(false);
    action1->setVisible(false);

    QVERIFY(m_subject->actions().count() == 4);

    action = qobject_cast<DuiAction *>(m_subject->actions().at(0));
    QVERIFY(action);
    QVERIFY(action == action0);
    QVERIFY(action->isVisible() == false);

    action = qobject_cast<DuiAction *>(m_subject->actions().at(1));
    QVERIFY(action);
    QVERIFY(action == action1);
    QVERIFY(action->isVisible() == false);

    action = qobject_cast<DuiAction *>(m_subject->actions().at(2));
    QVERIFY(action);
    QVERIFY(action == action2);
    QVERIFY(action->isVisible() == true);

    action = qobject_cast<DuiAction *>(m_subject->actions().at(3));
    QVERIFY(action);
    QVERIFY(action == action3);
    QVERIFY(action->isVisible() == true);


    //text entry:
    DuiTextEdit *textEntry = new DuiTextEdit();
    DuiWidgetAction *actionTextEdit = new DuiWidgetAction(m_subject);
    actionTextEdit->setWidget(textEntry);

    m_subject->addAction(actionTextEdit);
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(m_subject->actions().at(4));
    QVERIFY(widgetAction);
    QVERIFY(widgetAction == actionTextEdit);
    QVERIFY(widgetAction->isVisible() == true);

    action0->setVisible(true);
    action = qobject_cast<DuiAction *>(m_subject->actions().at(0));
    QVERIFY(action);
    QVERIFY(action == action0);
    QVERIFY(action->isVisible() == true);

    action1->setVisible(true);
    action = qobject_cast<DuiAction *>(m_subject->actions().at(1));
    QVERIFY(action);
    QVERIFY(action == action1);
    QVERIFY(action->isVisible() == true);

    QVERIFY(widgetAction);
    QVERIFY(widgetAction->widget()->isVisible() == false);   //visiblity should be changed, because there is no room for this anymore
}

void Ut_DuiToolBar::actionSlot(bool)
{
    senderAction = qobject_cast<DuiAction *>(sender());
    testValue = true;
}

QTEST_APPLESS_MAIN(Ut_DuiToolBar)
