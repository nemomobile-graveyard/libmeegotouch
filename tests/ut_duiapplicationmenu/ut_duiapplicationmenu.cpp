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

#include "ut_duiapplicationmenu.h"

#include <QStringListModel>
#include <QGraphicsSceneMouseEvent>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiApplicationMenu>
#include <DuiApplicationMenuView>
#include <DuiScene>
#include <DuiWidgetAction>
#include <DuiButton>
#include <DuiComboBox>
#include "duiapplicationmenu_p.h"

DuiApplication *app;
DuiApplicationWindow *appWin;

void Ut_DuiApplicationMenu::init()
{
    m_subject = new DuiApplicationMenu();
}

void Ut_DuiApplicationMenu::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiApplicationMenu::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duiapplicationmenu" };
    app = new DuiApplication(argc, app_name);
    appWin = new DuiApplicationWindow;
}

void Ut_DuiApplicationMenu::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_DuiApplicationMenu::testConstructor()
{
    QVERIFY(m_subject);
}

void Ut_DuiApplicationMenu::testDestructor()
{
    DuiApplicationMenu *menu = new DuiApplicationMenu();
    delete menu;
}

void Ut_DuiApplicationMenu::testAddAction()
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

void Ut_DuiApplicationMenu::testInsertAction()
{

    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    DuiAction *action0 = new DuiAction("action0", m_subject);
    m_subject->addAction(action0);

    DuiAction *action1 = new DuiAction("action1", m_subject);
    m_subject->addAction(action1);

    DuiAction *action2 = new DuiAction("action2", m_subject);
    m_subject->insertAction(action0, action2);

    QVERIFY(m_subject->actions().count() == 3);

    DuiAction *action_at0 = qobject_cast<DuiAction *>(m_subject->actions().at(0));
    DuiAction *action_at1 = qobject_cast<DuiAction *>(m_subject->actions().at(1));
    DuiAction *action_at2 = qobject_cast<DuiAction *>(m_subject->actions().at(2));

    QVERIFY(action_at0);
    QVERIFY(action_at0 == action2);
    QVERIFY(action_at1);
    QVERIFY(action_at1 == action0);
    QVERIFY(action_at2);
    QVERIFY(action_at2 == action1);

}

void Ut_DuiApplicationMenu::testActionVisiblity()
{

    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    DuiAction *action0 = new DuiAction("action0", m_subject);
    action0->setLocation(DuiAction::ApplicationMenuLocation);
    m_subject->addAction(action0);

    DuiAction *action1 = new DuiAction("action1", m_subject);
    action1->setLocation(DuiAction::ApplicationMenuLocation);
    m_subject->addAction(action1);

    DuiAction *action2 = new DuiAction("action2", m_subject);
    action2->setLocation(DuiAction::ApplicationMenuLocation);
    m_subject->addAction(action2);

    DuiAction *action3 = new DuiAction("action3", m_subject);
    action3->setLocation(DuiAction::ApplicationMenuLocation);
    m_subject->addAction(action3);

    QVERIFY(m_subject->actions().count() == 4);

    action0->setVisible(false);
    action1->setVisible(false);

    QVERIFY(m_subject->actions().count() == 4);

    DuiAction *action = qobject_cast<DuiAction *>(m_subject->actions().at(0));
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

    DuiWidgetAction *comboAction = new DuiWidgetAction(m_subject);
    comboAction->setLocation(DuiAction::ApplicationMenuLocation);
    QStringList list;
    for (int i = 0; i < 5; ++i) {
        list << QString::number(100 + i);
    }
    DuiComboBox *comboBox = new DuiComboBox();
    comboBox->setIconVisible(false);
    comboBox->addItems(list);
    comboAction->setWidget(comboBox);
    m_subject->addAction(comboAction);

    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(m_subject->actions().at(4));
    QVERIFY(widgetAction);
    QVERIFY(widgetAction == comboAction);
    QVERIFY(widgetAction->isVisible() == true);

    widgetAction->setVisible(false);
    QVERIFY(widgetAction->widget()->isVisible() == false);
}

void Ut_DuiApplicationMenu::actionSlot(bool checked)
{
    Q_UNUSED(checked);
    senderAction = qobject_cast<DuiAction *>(sender());
    testValue = true;
}


void Ut_DuiApplicationMenu::testClear()
{
    for (int i = 0; i < 5; ++i) {
        DuiAction *action = new DuiAction("", "Action" + QString::number(i), m_subject);
        m_subject->addAction(action);
    }

    QVERIFY(m_subject->actions().count() == 5);

    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);
}

void Ut_DuiApplicationMenu::testEventsPassingThrough()
{
    m_subject->appear();
    // The event should be swallowed if it is not triggering any action.
    QGraphicsSceneMouseEvent mousePressEvent(QEvent::GraphicsSceneMousePress);
    QVERIFY(mousePressEvent.isAccepted());

    QGraphicsSceneMouseEvent mouseReleaseEvent(QEvent::GraphicsSceneMouseRelease);
    m_subject->mouseReleaseEvent(&mouseReleaseEvent);
}


void Ut_DuiApplicationMenu::testPaint()
{
    QPixmap pixmap(864, 480);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    m_subject->addAction(new DuiAction("text action", m_subject));
    m_subject->paint(&painter, NULL, NULL);
    delete m_subject;

    m_subject = new DuiApplicationMenu("object");
    m_subject->addAction(new DuiAction("text action", m_subject));
    m_subject->paint(&painter, NULL, NULL);
}

QTEST_APPLESS_MAIN(Ut_DuiApplicationMenu)
