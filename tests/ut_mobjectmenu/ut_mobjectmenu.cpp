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

#include "ut_mobjectmenu.h"

#include <QObject>
#include <mapplication.h>
#include <mobjectmenumodel.h>

void Ut_MObjectMenu::init()
{
}

void Ut_MObjectMenu::cleanup()
{
}

MApplication *app;

void Ut_MObjectMenu::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *) "./ut_mobjectmenu";
    app = new MApplication(argc, &app_name);
}

void Ut_MObjectMenu::cleanupTestCase()
{
    delete app;
}

void Ut_MObjectMenu::testConstructionAndDestruction()
{
    MWidget *widget = new MWidget();
    MObjectMenu *menu = new MObjectMenu(widget);
    MObjectMenuModel *model = dynamic_cast<MObjectMenuModel *>(menu->model());

    // check that the model has been created.
    QVERIFY(model != NULL);

    // check that the action count is zero
    QVERIFY(model->actions().count() == 0);

    delete menu;
    delete widget;
}

void Ut_MObjectMenu::testActionsAddingAndRemoving()
{
    MWidget *widget = new MWidget();
    MAction *action1 = new MAction("Test1", widget);
    action1->setLocation(MAction::ObjectMenuLocation);
    widget->addAction(action1);

    MObjectMenu *menu = new MObjectMenu(widget);
    MObjectMenuModel *model = dynamic_cast<MObjectMenuModel *>(menu->model());

    // check that the model has the action
    QCOMPARE(model->actions().count(), 1);
    QCOMPARE(model->actions().at(0), action1);

    QSignalSpy addedSpy(model, SIGNAL(actionAdded(MAction *)));
    QSignalSpy removedSpy(model, SIGNAL(actionRemoved(MAction *)));
    QSignalSpy modifiedSpy(model, SIGNAL(actionModified(MAction *)));

    MAction *action2 = new MAction("Test2", widget);

    // check that we don't get the signal yet.
    QCOMPARE(addedSpy.count(), 0);

    action2->setLocation(MAction::ObjectMenuLocation);

    widget->addAction(action2);
    QCOMPARE(addedSpy.count(), 1);
    QCOMPARE(removedSpy.count(), 0);
    QCOMPARE(modifiedSpy.count(), 0);
    QCOMPARE(model->actions().count(), 2);
    QCOMPARE(model->actions().at(0), action1);
    QCOMPARE(model->actions().at(1), action2);

    widget->removeAction(action1);
    QCOMPARE(addedSpy.count(), 1);
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(modifiedSpy.count(), 0);
    QCOMPARE(model->actions().count(), 1);
    QCOMPARE(model->actions().at(0), action2);

    widget->addAction(action1);
    QCOMPARE(addedSpy.count(), 2);
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(modifiedSpy.count(), 0);
    QCOMPARE(model->actions().count(), 2);
    QCOMPARE(model->actions().at(0), action2);
    QCOMPARE(model->actions().at(1), action1);

    action2->setText("Test123");
    QCOMPARE(addedSpy.count(), 2);
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(modifiedSpy.count(), 1);
    QCOMPARE(model->actions().count(), 2);
    QCOMPARE(model->actions().at(0), action2);
    QCOMPARE(model->actions().at(1), action1);


    action1->setText("Test234");
    QCOMPARE(addedSpy.count(), 2);
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(modifiedSpy.count(), 2);
    QCOMPARE(model->actions().count(), 2);
    QCOMPARE(model->actions().at(0), action2);
    QCOMPARE(model->actions().at(1), action1);
}

void Ut_MObjectMenu::testCursorPosition()
{
    MWidget *widget = new MWidget();
    MAction *action1 = new MAction("Test1", widget);
    action1->setLocation(MAction::ObjectMenuLocation);
    widget->addAction(action1);

    MObjectMenu *menu = new MObjectMenu(widget);

    QPointF point ( 1.0, 1.0 );
    menu->setCursorPosition( point );

    QCOMPARE( menu->cursorPosition(), point );

    delete menu;
    delete widget;
}

void Ut_MObjectMenu::testTitle()
{
    MWidget *widget = new MWidget();
    MObjectMenu *menu = new MObjectMenu(widget);

    QCOMPARE(menu->title(), QString(""));
    
    menu->setTitle("title");
    QCOMPARE(menu->title(), QString("title"));
    
    widget->setProperty("objectMenuTitle", QString("property title"));
    QCOMPARE(menu->title(), QString("title"));
    
    delete menu;

    menu = new MObjectMenu(widget);
    QCOMPARE(menu->title(), QString("property title"));

    menu->setTitle("");
    QCOMPARE(menu->title(), QString(""));

    delete menu;
    delete widget;
}


QTEST_APPLESS_MAIN(Ut_MObjectMenu)
