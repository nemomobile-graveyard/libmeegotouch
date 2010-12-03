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

#include "ut_mtoolbar.h"

#include <MToolBar>
#include <MToolBarView>
#include <MApplication>
#include <MApplicationWindow>

#include <MButton>
#include <MTextEdit>
#include <MSlider>
#include <MButtonGroup>
#include <MWidgetAction>
#include "mtoolbar_p.h"

MApplication *app(NULL);

void Ut_MToolBar::init()
{
    m_subject = new MToolBar();
}

void Ut_MToolBar::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MToolBar::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mtoolbar" };
    app = new MApplication(argc, app_name);
}

void Ut_MToolBar::cleanupTestCase()
{
    // We do not delete the app on purpose as it seems to cause some random bus errors
    //delete app;
    //app = NULL;
}

void Ut_MToolBar::testConstructor()
{
    QVERIFY(m_subject);
}

void Ut_MToolBar::testConstructor2()
{
    class InheritedMToolBar : public MToolBar
    {
    public:
        explicit InheritedMToolBar()
            : MToolBar(new MToolBarPrivate(),new MWidgetModel(),NULL)
        {};
    };
    //testing protected constructor: MToolBar(MToolBarPrivate *dd, MWidgetModel *model, QGraphicsItem *parent);
    InheritedMToolBar* inheritedMToolBar = new InheritedMToolBar();
    QVERIFY(inheritedMToolBar);
    delete inheritedMToolBar;
}

void Ut_MToolBar::testDestructor()
{
    MToolBar *toolbar = new MToolBar();
    delete toolbar;
}

void Ut_MToolBar::testAddAction()
{
    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    // return value action
    MAction *action;

    // test addAction(const QString &text)
    MAction *action0 = new MAction("TEXTONLY", m_subject);
    m_subject->addAction(action0);
    action = qobject_cast<MAction *>(m_subject->actions().at(0));
    // must be one
    QVERIFY(m_subject->actions().count() == 1);
    QVERIFY(action == action0);
    QVERIFY(action->text() == "TEXTONLY");

    // test addAction(const QString &icon, const QString &text)
    MAction *action1 = new MAction("Icon-list-view-on", "TEXT", m_subject);
    m_subject->addAction(action1);
    QVERIFY(m_subject->actions().count() == 2);
    action = qobject_cast<MAction *>(m_subject->actions().at(1));
    QVERIFY(action == action1);
    QVERIFY(action->iconID() == "Icon-list-view-on");
    QVERIFY(action->text() == "TEXT");

    testValue = false;
    senderAction = NULL;
    connect(action1, SIGNAL(triggered(bool)), this, SLOT(actionSlot(bool)));
    action1->trigger();
    QVERIFY(testValue);
    QVERIFY(senderAction == action1);

    // test addAction(MAction* action)
    MAction *action2 = new MAction("Icon-time-line-on", "TEXT4", NULL);
    m_subject->addAction(action2);
    QVERIFY(m_subject->actions().count() == 3);

    action = qobject_cast<MAction *>(m_subject->actions().at(2));
    QVERIFY(action == action2);
    delete action2;

}

void Ut_MToolBar::testAddTextEditAction()
{
    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    // test addAction(Text Edit)
    MTextEdit *textEntry = new MTextEdit();
    MWidgetAction *actionTextEdit = new MWidgetAction(m_subject);
    actionTextEdit->setWidget(textEntry);

    m_subject->addAction(actionTextEdit);
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(m_subject->actions().at(0));

    // must be one
    QVERIFY(m_subject->actions().count() == 1);
    QVERIFY(widgetAction);
    QVERIFY(widgetAction == actionTextEdit);
    QVERIFY(widgetAction->widget() == textEntry);
}

void Ut_MToolBar::testInsertAction()
{
    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    //adding text edit
    MTextEdit *textEntry = new MTextEdit();
    MWidgetAction *actionTextEdit = new MWidgetAction(m_subject);
    actionTextEdit->setWidget(textEntry);

    m_subject->addAction(actionTextEdit); //first action, indx 0

    //adding actions
    MAction *action0 = new MAction("action0", m_subject);
    m_subject->addAction(action0); //second action, indx 1

    MAction *action1 = new MAction("action1", m_subject);
    m_subject->insertAction(action0, action1);   //third action, indx1 -> action0 goes to indx 2

    QVERIFY(m_subject->actions().count() == 3);

    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(m_subject->actions().at(0)); // getting indx 0
    MAction *action = qobject_cast<MAction *>(m_subject->actions().at(1)); //getting indx 1

    //veryfing:
    QVERIFY(widgetAction);
    QVERIFY(widgetAction  == actionTextEdit);
    QVERIFY(action);
    QVERIFY(action  == action1);

    action = qobject_cast<MAction *>(m_subject->actions().at(2)); //getting indx 2
    QVERIFY(action);
    QVERIFY(action  == action0);

}

void Ut_MToolBar::testActionVisiblity()
{
    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    //adding actions:
    MAction *action0 = new MAction("action0", m_subject);
    m_subject->addAction(action0); //first action, indx 0

    MAction *action1 = new MAction("action1", m_subject);
    m_subject->addAction(action1); //second action, indx 1

    MAction *action2 = new MAction("action2", m_subject);
    m_subject->addAction(action2); //third action, indx 2

    MAction *action3 = new MAction("action3", m_subject);
    m_subject->addAction(action3); //fourth action, indx 3

    QVERIFY(m_subject->actions().count() == 4);

    MAction *action; //needed for veryfing

    action0->setVisible(false);
    action1->setVisible(false);

    QVERIFY(m_subject->actions().count() == 4);

    action = qobject_cast<MAction *>(m_subject->actions().at(0));
    QVERIFY(action);
    QVERIFY(action == action0);
    QVERIFY(action->isVisible() == false);

    action = qobject_cast<MAction *>(m_subject->actions().at(1));
    QVERIFY(action);
    QVERIFY(action == action1);
    QVERIFY(action->isVisible() == false);

    action = qobject_cast<MAction *>(m_subject->actions().at(2));
    QVERIFY(action);
    QVERIFY(action == action2);
    QVERIFY(action->isVisible() == true);

    action = qobject_cast<MAction *>(m_subject->actions().at(3));
    QVERIFY(action);
    QVERIFY(action == action3);
    QVERIFY(action->isVisible() == true);


    //text entry:
    MTextEdit *textEntry = new MTextEdit();
    MWidgetAction *actionTextEdit = new MWidgetAction(m_subject);
    actionTextEdit->setWidget(textEntry);

    m_subject->addAction(actionTextEdit);
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(m_subject->actions().at(4));
    QVERIFY(widgetAction);
    QVERIFY(widgetAction == actionTextEdit);
    QVERIFY(widgetAction->isVisible() == true);

    action0->setVisible(true);
    action = qobject_cast<MAction *>(m_subject->actions().at(0));
    QVERIFY(action);
    QVERIFY(action == action0);
    QVERIFY(action->isVisible() == true);

    action1->setVisible(true);
    action = qobject_cast<MAction *>(m_subject->actions().at(1));
    QVERIFY(action);
    QVERIFY(action == action1);
    QVERIFY(action->isVisible() == true);

    QVERIFY(widgetAction);
    QVERIFY(widgetAction->widget()->isVisible() == false);   //visiblity should be changed, because there is no room for this anymore
}

void Ut_MToolBar::testAddTabAction()
{
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    m_subject->setViewType(MToolBar::tabType);

    MAction *action0 = new MAction("action0", m_subject);
    action0->setLocation(MAction::ToolBarLandscapeLocation);
    action0->setCheckable(true);
    action0->setChecked(true);
    m_subject->addAction(action0);
    QVERIFY(m_subject->actions().count() == 1);

    MAction *action1 = new MAction("action1", m_subject);
    action1->setLocation(MAction::ToolBarPortraitLocation);
    action1->setCheckable(true);
    m_subject->addAction(action1);
    QVERIFY(m_subject->actions().count() == 2);

    MAction *action2 = new MAction("action2", m_subject);
    action2->setLocation(MAction::ToolBarLocation);
    action2->setCheckable(true);
    m_subject->addAction(action2);
    QVERIFY(m_subject->actions().count() == 3);
}

void Ut_MToolBar::testRemoveTabAction()
{
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    m_subject->setViewType(MToolBar::tabType);

    MAction *action0 = new MAction("action0", m_subject);
    action0->setLocation(MAction::ToolBarLandscapeLocation);
    action0->setCheckable(true);
    action0->setChecked(true);
    m_subject->addAction(action0);
    QVERIFY(m_subject->actions().count() == 1);

    MAction *action1 = new MAction("action1", m_subject);
    action1->setLocation(MAction::ToolBarPortraitLocation);
    action1->setCheckable(true);
    m_subject->addAction(action1);
    QVERIFY(m_subject->actions().count() == 2);
    m_subject->removeAction(action1);
    QVERIFY(m_subject->actions().count() == 1);

    MAction *action2 = new MAction("action2", m_subject);
    action2->setLocation(MAction::ToolBarLocation);
    action2->setCheckable(true);
    m_subject->addAction(action2);
    QVERIFY(m_subject->actions().count() == 2);

    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);
}

void Ut_MToolBar::actionSlot(bool)
{
    senderAction = qobject_cast<MAction *>(sender());
    testValue = true;
}

void Ut_MToolBar::testNoLocation()
{
    //Add an action to everywhere and then change it to nowhere, to test that this does not crash etc
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    //Add a normal action
    MAction *action0 = new MAction("action0", m_subject);
    action0->setCheckable(true);
    action0->setChecked(true);
    m_subject->addAction( action0 );
  
    //Add a widget action
    MTextEdit *textEntry = new MTextEdit();
    MWidgetAction *actionTextEdit = new MWidgetAction(m_subject);
    actionTextEdit->setWidget(textEntry);
    m_subject->addAction(actionTextEdit );

    QCOMPARE(action0->location(), MAction::EveryLocation);
    QCOMPARE(actionTextEdit->location(), MAction::EveryLocation);
    //Now remove both actions
    action0->setLocation(MAction::NoLocation);
    actionTextEdit->setLocation(MAction::NoLocation);

}

void Ut_MToolBar::testPropertyChange()
{
    // Add an action then set the Property which is triggered by the AppMenu when it is opened and closed
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    // Add a normal action
    MAction *action = new MAction("action", m_subject);
    action->setLocation(MAction::ToolBarLocation);
    action->setEnabled(true);
    action->setVisible(true);
    m_subject->addAction(action);

    // Retrieve the widget associated with the action
    const MToolBarView *view = dynamic_cast<const MToolBarView *>(m_subject->view());
    Q_ASSERT(view);
    MWidget *widget = view->getWidget(action);
    QVERIFY(widget);
    QVERIFY(widget->isEnabled());

    // Test that enabled state is restored, this is called i.e. when appmenu is opened
    // widget/action starting as enabled
    m_subject->setProperty(_M_IsEnabledPreservingSelection, QVariant(false));
    QVERIFY(!widget->isEnabled());
    m_subject->setProperty(_M_IsEnabledPreservingSelection, QVariant(true));
    QVERIFY(widget->isEnabled());

    // widget/action starting as disabled
    action->setEnabled(false);
    QVERIFY(!widget->isEnabled());
    m_subject->setProperty(_M_IsEnabledPreservingSelection, QVariant(false));
    QVERIFY(!widget->isEnabled());
    m_subject->setProperty(_M_IsEnabledPreservingSelection, QVariant(true));
    QVERIFY(!widget->isEnabled());
}

QTEST_APPLESS_MAIN(Ut_MToolBar)
