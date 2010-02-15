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

#include "ut_duiobjectmenu.h"

#include <QObject>
#include <duiapplication.h>
#include <duiobjectmenumodel.h>

void Ut_DuiObjectMenu::init()
{
}

void Ut_DuiObjectMenu::cleanup()
{
}

DuiApplication *app;

void Ut_DuiObjectMenu::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./ut_duiobjectmenu";
    app = new DuiApplication(argc, &app_name);
}

void Ut_DuiObjectMenu::cleanupTestCase()
{
    delete app;
}

void Ut_DuiObjectMenu::testConstructionAndDestruction()
{
    DuiWidget *widget = new DuiWidget();
    DuiObjectMenu *menu = new DuiObjectMenu(widget);
    DuiObjectMenuModel *model = dynamic_cast<DuiObjectMenuModel *>(menu->model());

    // check that the model has been created.
    QVERIFY(model != NULL);

    // check that the action count is zero
    QVERIFY(model->actions().count() == 0);

    delete menu;
    delete widget;
}

void Ut_DuiObjectMenu::testActionsAddingAndRemoving()
{
    DuiWidget *widget = new DuiWidget();
    DuiAction *action1 = new DuiAction("Test1", widget);
    action1->setLocation(DuiAction::ObjectMenuLocation);
    widget->addAction(action1);

    DuiObjectMenu *menu = new DuiObjectMenu(widget);
    DuiObjectMenuModel *model = dynamic_cast<DuiObjectMenuModel *>(menu->model());

    // check that the model has the action
    QCOMPARE(model->actions().count(), 1);
    QCOMPARE(model->actions().at(0), action1);

    QSignalSpy addedSpy(model, SIGNAL(actionAdded(DuiAction *)));
    QSignalSpy removedSpy(model, SIGNAL(actionRemoved(DuiAction *)));
    QSignalSpy modifiedSpy(model, SIGNAL(actionModified(DuiAction *)));

    DuiAction *action2 = new DuiAction("Test2", widget);

    // check that we don't get the signal yet.
    QCOMPARE(addedSpy.count(), 0);

    action2->setLocation(DuiAction::ObjectMenuLocation);

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

QTEST_APPLESS_MAIN(Ut_DuiObjectMenu)
