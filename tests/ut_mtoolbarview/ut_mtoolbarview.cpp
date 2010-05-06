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
#include <MApplicationWindow>
#include <MAction>
#include <MToolBar>
#include <MToolBarView>
#include <MToolBarTabView>
#include <MTextEdit>
#include <MWidgetAction>
#include <QGraphicsLinearLayout>

#include "ut_mtoolbarview.h"

MApplication *app;
MApplicationWindow *appWin;

void Ut_MToolBarView::initTestCase()

{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MToolBarView" };
    app = new MApplication(argc, app_name);
    appWin = new MApplicationWindow;
}

void Ut_MToolBarView::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_MToolBarView::init()
{
    m_toolbar = new MToolBar();
    QVERIFY(m_toolbar != 0);

    m_subject = new MToolBarView(m_toolbar);
    QVERIFY(m_subject != 0);

    m_toolbar->setView(m_subject);
    //m_subject->updateStyle();
}

void Ut_MToolBarView::cleanup()
{
    delete m_toolbar;
    m_toolbar = 0;
}

void Ut_MToolBarView::testDeleteAddAction()
{
    m_toolbar->clearActions();

    MWidget *parentWidget = new MWidget();
    m_toolbar->addAction(createTextEditAction(parentWidget));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    delete parentWidget;

    // toolbar should not crash as action has been deleted
    m_toolbar->addAction(new MAction("TEXTONLY", m_toolbar));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QVERIFY(m_toolbar->actions().count() == 1);
}

void Ut_MToolBarView::testTabView()
{
    m_toolbar->clearActions();

    MToolbarTabView* tabView = new MToolbarTabView(m_toolbar);
    QVERIFY(tabView != 0);

    m_toolbar->setView(tabView);
}

void Ut_MToolBarView::testTabActionExclusivity()
{
    m_toolbar->clearActions();
    MToolbarTabView* tabView = new MToolbarTabView(m_toolbar);
    QVERIFY(tabView != 0);
    m_toolbar->setView(tabView);

    MAction *action0 = new MAction("action0", m_toolbar);
    action0->setLocation(MAction::ToolBarLandscapeLocation);
    action0->setCheckable(true);
    action0->setChecked(true);
    m_toolbar->addAction(action0);
    MAction* action = qobject_cast<MAction *>(m_toolbar->actions().at(0));
    QVERIFY(action);
    QVERIFY(action == action0);

    QVERIFY(action0->isChecked() == true);

    MAction *action1 = new MAction("action1", m_toolbar);
    action1->setLocation(MAction::ToolBarPortraitLocation);
    action1->setCheckable(true);
    m_toolbar->addAction(action1);
    action = qobject_cast<MAction *>(m_toolbar->actions().at(1));
    QVERIFY(action);
    QVERIFY(action == action1);

    QVERIFY(action0->isChecked() == true);
    QVERIFY(action1->isChecked() == false);

    MAction *action2 = new MAction("action2", m_toolbar);
    action2->setLocation(MAction::ToolBarLocation);
    action2->setCheckable(true);
    m_toolbar->addAction(action2);
    action = qobject_cast<MAction *>(m_toolbar->actions().at(2));
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

MWidgetAction *Ut_MToolBarView::createTextEditAction(MWidget *parentWidget)
{
    MTextEdit *textEntry = new MTextEdit();
    MWidgetAction *actionTextEdit = new MWidgetAction(parentWidget);
    actionTextEdit->setWidget(textEntry);
    return actionTextEdit;
}

QTEST_APPLESS_MAIN(Ut_MToolBarView)
