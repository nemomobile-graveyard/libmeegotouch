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

#include "ut_mapplicationmenu.h"

#include <QStringListModel>
#include <QGraphicsSceneMouseEvent>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationMenu>
#include <MApplicationMenuView>
#include <MScene>
#include <MWidgetAction>
#include <MButton>
#include <MComboBox>
#include "mapplicationmenu_p.h"
#include "mapplicationmenuview_p.h"
#include "mlist.h"

MApplication *app;
MApplicationWindow *appWin;

void Ut_MApplicationMenu::init()
{
    m_subject = new MApplicationMenu();
    MApplication::activeWindow()->setOrientationAngle(M::Angle0);
}

void Ut_MApplicationMenu::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MApplicationMenu::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplicationmenu" };
    app = new MApplication(argc, app_name);
    appWin = new MApplicationWindow;
}

void Ut_MApplicationMenu::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_MApplicationMenu::testConstructor()
{
    QVERIFY(m_subject);
}

void Ut_MApplicationMenu::testDestructor()
{
    MApplicationMenu *menu = new MApplicationMenu();
    delete menu;
}

void Ut_MApplicationMenu::testAddAction()
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

void Ut_MApplicationMenu::testInsertAction()
{

    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    MAction *action0 = new MAction("action0", m_subject);
    m_subject->addAction(action0);

    MAction *action1 = new MAction("action1", m_subject);
    m_subject->addAction(action1);

    MAction *action2 = new MAction("action2", m_subject);
    m_subject->insertAction(action0, action2);

    QVERIFY(m_subject->actions().count() == 3);

    MAction *action_at0 = qobject_cast<MAction *>(m_subject->actions().at(0));
    MAction *action_at1 = qobject_cast<MAction *>(m_subject->actions().at(1));
    MAction *action_at2 = qobject_cast<MAction *>(m_subject->actions().at(2));

    QVERIFY(action_at0);
    QVERIFY(action_at0 == action2);
    QVERIFY(action_at1);
    QVERIFY(action_at1 == action0);
    QVERIFY(action_at2);
    QVERIFY(action_at2 == action1);

}

void Ut_MApplicationMenu::testSetWidgetAfterWidgetActionIsAdded()
{
    MWidgetAction *action = new MWidgetAction(m_subject);
    action->setLocation(MAction::ApplicationMenuLocation);
    m_subject->addAction(action);
    QVERIFY(m_subject->actions().count() == 1);

    MComboBox *widgetInAction = new MComboBox;
    action->setWidget(widgetInAction);

    const MApplicationMenuViewPrivate *viewPrivate = menuViewPrivate();
    QVERIFY(viewPrivate);
    MMenuListItem* menuItem = getMenuItem(viewPrivate, 0);
    QVERIFY(menuItem);
    QVERIFY(menuItem->customWidget() == widgetInAction);
}

void Ut_MApplicationMenu::testChangeWidgetInWidgetAction()
{
    MWidgetAction *action = new MWidgetAction(m_subject);
    action->setLocation(MAction::ApplicationMenuLocation);
    MComboBox *widgetInAction = new MComboBox;
    action->setWidget(widgetInAction);
    m_subject->addAction(action);

    action->releaseWidget(widgetInAction);

    MButton *newWidgetInAction = new MButton;
    action->setWidget(newWidgetInAction);

    const MApplicationMenuViewPrivate *viewPrivate = menuViewPrivate();
    QVERIFY(viewPrivate);
    MMenuListItem* menuItem = getMenuItem(viewPrivate, 0);
    QVERIFY(menuItem);
    QVERIFY(menuItem->customWidget() == newWidgetInAction);
}

void Ut_MApplicationMenu::testRemoveWidgetFromWidgetAction()
{
    MWidgetAction *action = new MWidgetAction(m_subject);
    action->setLocation(MAction::ApplicationMenuLocation);
    MComboBox *widgetInAction = new MComboBox;
    action->setWidget(widgetInAction);
    m_subject->addAction(action);

    action->releaseWidget(widgetInAction);

    action->setWidget(0);

    const MApplicationMenuViewPrivate *viewPrivate = menuViewPrivate();
    QVERIFY(viewPrivate);
    MMenuListItem* menuItem = getMenuItem(viewPrivate, 0);
    QVERIFY(menuItem);
    QVERIFY(!menuItem->customWidget());
}

void Ut_MApplicationMenu::testActionVisiblity()
{

    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    MAction *action0 = new MAction("action0", m_subject);
    action0->setLocation(MAction::ApplicationMenuLocation);
    m_subject->addAction(action0);

    MAction *action1 = new MAction("action1", m_subject);
    action1->setLocation(MAction::ApplicationMenuLocation);
    m_subject->addAction(action1);

    MAction *action2 = new MAction("action2", m_subject);
    action2->setLocation(MAction::ApplicationMenuLocation);
    m_subject->addAction(action2);

    MAction *action3 = new MAction("action3", m_subject);
    action3->setLocation(MAction::ApplicationMenuLocation);
    m_subject->addAction(action3);

    QVERIFY(m_subject->actions().count() == 4);

    action0->setVisible(false);
    action1->setVisible(false);

    QVERIFY(m_subject->actions().count() == 4);

    MAction *action = qobject_cast<MAction *>(m_subject->actions().at(0));
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

    MWidgetAction *comboAction = new MWidgetAction(m_subject);
    comboAction->setLocation(MAction::ApplicationMenuLocation);
    QStringList list;
    for (int i = 0; i < 5; ++i) {
        list << QString::number(100 + i);
    }
    MComboBox *comboBox = new MComboBox();
    comboBox->setIconVisible(false);
    comboBox->addItems(list);
    comboAction->setWidget(comboBox);
    m_subject->addAction(comboAction);

    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(m_subject->actions().at(4));
    QVERIFY(widgetAction);
    QVERIFY(widgetAction == comboAction);
    QVERIFY(widgetAction->isVisible() == true);

    widgetAction->setVisible(false);
    QVERIFY(widgetAction->widget()->isVisible() == false);
}

void Ut_MApplicationMenu::testOpeningAndClosingAppMenuWithDisabledAction()
{
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);

    MAction *action = new MAction("action", m_subject);
    action->setLocation(MAction::ApplicationMenuLocation);
    action->setEnabled(true);
    m_subject->addAction(action);

    const MApplicationMenuViewPrivate *viewPrivate = menuViewPrivate();
    QVERIFY(viewPrivate);
    MMenuListItem *menuItem = getMenuItem(viewPrivate, 0);
    QVERIFY(menuItem);
    QVERIFY(menuItem->isEnabled());

    // Test opening and closing the app menu with the action disabled, it should remain disabled
    action->setEnabled(false);
    menuItem = getMenuItem(viewPrivate, 0);
    QVERIFY(menuItem);
    QVERIFY(!menuItem->isEnabled());
    appWin->sceneManager()->appearSceneWindowNow(m_subject);
    appWin->sceneManager()->dismissSceneWindowNow(m_subject);
    QVERIFY(!menuItem->isEnabled());

}

const MApplicationMenuViewPrivate* Ut_MApplicationMenu::menuViewPrivate()
{
    const MApplicationMenuView *view = dynamic_cast<const MApplicationMenuView *>(m_subject->view());
    if (!view)
        return 0;
    const MApplicationMenuViewPrivate *viewPrivate = view->d_func();
    return viewPrivate;
}

MMenuListItem* Ut_MApplicationMenu::getMenuItem(const MApplicationMenuViewPrivate *viewPrivate, int actionNumber)
{
    QModelIndex index = viewPrivate->list->itemModel()->index(actionNumber, 0);
    MWidget* cell = viewPrivate->list->cellCreator()->createCell(index, *MWidgetRecycler::instance());
    return dynamic_cast<MMenuListItem*>(cell);
}

void Ut_MApplicationMenu::actionSlot(bool checked)
{
    Q_UNUSED(checked);
    senderAction = qobject_cast<MAction *>(sender());
    testValue = true;
}


void Ut_MApplicationMenu::testClear()
{
    for (int i = 0; i < 5; ++i) {
        MAction *action = new MAction("", "Action" + QString::number(i), m_subject);
        m_subject->addAction(action);
    }

    QVERIFY(m_subject->actions().count() == 5);

    // empty action list, check
    m_subject->clearActions();
    QVERIFY(m_subject->actions().count() == 0);
}

void Ut_MApplicationMenu::testEventsPassingThrough()
{
    m_subject->appear(appWin);
    // The event should be swallowed if it is not triggering any action.
    QGraphicsSceneMouseEvent mousePressEvent(QEvent::GraphicsSceneMousePress);
    QVERIFY(mousePressEvent.isAccepted());

    QGraphicsSceneMouseEvent mouseReleaseEvent(QEvent::GraphicsSceneMouseRelease);
    m_subject->mouseReleaseEvent(&mouseReleaseEvent);
}


void Ut_MApplicationMenu::testPaint()
{
    QPixmap pixmap(864, 480);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    m_subject->addAction(new MAction("text action", m_subject));
    m_subject->paint(&painter, NULL, NULL);
    delete m_subject;

    m_subject = new MApplicationMenu("object");
    m_subject->addAction(new MAction("text action", m_subject));
    m_subject->paint(&painter, NULL, NULL);
}

void Ut_MApplicationMenu::testRotation()
{
    MAction *action = new MAction("Italic", NULL);
    action->setLocation(MAction::ApplicationMenuLocation);
    m_subject->addAction(action);

    action = new MAction("Normal", NULL);
    action->setLocation(MAction::ApplicationMenuLocation);
    m_subject->addAction(action);

    action = new MAction("icon-m-grid", "", NULL);
    action->setLocation(MAction::ApplicationMenuLocation);
    action->setStyleAction(true);
    m_subject->addAction(action);

    /* Rotate the scene by 90 degrees and back again. We should find that the
     * preferred size hasn't changed overall.  We call processEvents to check that there
     * are no outstanding layouting etc */
    m_subject->appear(appWin);
    MApplication::activeWindow()->setOrientationAngle(M::Angle0);
    QSizeF preferredSizeLandscape = m_subject->preferredSize();
    app->processEvents();
    QCOMPARE(m_subject->preferredSize(), preferredSizeLandscape);

    MApplication::activeWindow()->setOrientationAngle(M::Angle90);
    QSizeF preferredSizePortrait = m_subject->preferredSize();
    app->processEvents();
    QCOMPARE(m_subject->preferredSize(), preferredSizePortrait);

    MApplication::activeWindow()->setOrientationAngle(M::Angle0);
    QCOMPARE(m_subject->preferredSize(), preferredSizeLandscape);
    app->processEvents();
    QCOMPARE(m_subject->preferredSize(), preferredSizeLandscape);

    MApplication::activeWindow()->setOrientationAngle(M::Angle90);
    QCOMPARE(m_subject->preferredSize(), preferredSizePortrait);
    app->processEvents();
    QCOMPARE(m_subject->preferredSize(), preferredSizePortrait);
}


QTEST_APPLESS_MAIN(Ut_MApplicationMenu)
