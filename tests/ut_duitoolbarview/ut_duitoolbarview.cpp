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
#include <DuiApplicationWindow>
#include <DuiAction>
#include <DuiToolBar>
#include <DuiToolBarView>
#include <DuiToolBarTabView>
#include <DuiTextEdit>
#include <DuiWidgetAction>
#include <QGraphicsLinearLayout>

#include "ut_duitoolbarview.h"

DuiApplication *app;
DuiApplicationWindow *appWin;

void Ut_DuiToolBarView::initTestCase()

{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_DuiToolBarView" };
    app = new DuiApplication(argc, app_name);
    appWin = new DuiApplicationWindow;
}

void Ut_DuiToolBarView::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_DuiToolBarView::init()
{
    m_toolbar = new DuiToolBar();
    QVERIFY(m_toolbar != 0);

    m_subject = new DuiToolBarView(m_toolbar);
    QVERIFY(m_subject != 0);

    m_toolbar->setView(m_subject);
    //m_subject->updateStyle();
}

void Ut_DuiToolBarView::cleanup()
{
    delete m_toolbar;
    m_toolbar = 0;
}

void Ut_DuiToolBarView::testDeleteAddAction()
{
    m_toolbar->clearActions();

    DuiWidget *parentWidget = new DuiWidget();
    m_toolbar->addAction(createTextEditAction(parentWidget));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    delete parentWidget;

    // toolbar should not crash as action has been deleted
    m_toolbar->addAction(new DuiAction("TEXTONLY", m_toolbar));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QVERIFY(m_toolbar->actions().count() == 1);
}

void Ut_DuiToolBarView::testTabView()
{
    m_toolbar->clearActions();

    DuiToolbarTabView* tabView = new DuiToolbarTabView(m_toolbar);
    QVERIFY(tabView != 0);

    m_toolbar->setView(tabView);
}

void Ut_DuiToolBarView::testTabActionExclusivity()
{
    m_toolbar->clearActions();
    DuiToolbarTabView* tabView = new DuiToolbarTabView(m_toolbar);
    QVERIFY(tabView != 0);
    m_toolbar->setView(tabView);

    DuiAction *action0 = new DuiAction("action0", m_toolbar);
    action0->setLocation(DuiAction::ToolBarLandscapeLocation);
    action0->setCheckable(true);
    action0->setChecked(true);
    m_toolbar->addAction(action0);
    DuiAction* action = qobject_cast<DuiAction *>(m_toolbar->actions().at(0));
    QVERIFY(action);
    QVERIFY(action == action0);

    QVERIFY(action0->isChecked() == true);

    DuiAction *action1 = new DuiAction("action1", m_toolbar);
    action1->setLocation(DuiAction::ToolBarPortraitLocation);
    action1->setCheckable(true);
    m_toolbar->addAction(action1);
    action = qobject_cast<DuiAction *>(m_toolbar->actions().at(1));
    QVERIFY(action);
    QVERIFY(action == action1);

    QVERIFY(action0->isChecked() == true);
    QVERIFY(action1->isChecked() == false);

    DuiAction *action2 = new DuiAction("action2", m_toolbar);
    action2->setLocation(DuiAction::ToolBarLocation);
    action2->setCheckable(true);
    m_toolbar->addAction(action2);
    action = qobject_cast<DuiAction *>(m_toolbar->actions().at(2));
    QVERIFY(action);
    QVERIFY(action == action2);

    QVERIFY(action0->isChecked() == true);
    QVERIFY(action1->isChecked() == false);
    QVERIFY(action2->isChecked() == false);

    action2->setChecked(true);
    QVERIFY(action0->isChecked() == false);
    QVERIFY(action1->isChecked() == false);
    QVERIFY(action2->isChecked() == true);
}

DuiWidgetAction *Ut_DuiToolBarView::createTextEditAction(DuiWidget *parentWidget)
{
    DuiTextEdit *textEntry = new DuiTextEdit();
    DuiWidgetAction *actionTextEdit = new DuiWidgetAction(parentWidget);
    actionTextEdit->setWidget(textEntry);
    return actionTextEdit;
}

QTEST_APPLESS_MAIN(Ut_DuiToolBarView)
