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
#include <mapplicationmenu.h>

#include "mapplicationmenuview_p.h"
#include <MAction>
#include <MWidgetAction>
#include <MList>
#include <MButton>

#include "ut_mapplicationmenuview.h"

MApplication *app;

Q_DECLARE_METATYPE(QAction*)

void Ut_MApplicationMenuView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MApplicationMenuView" };
    app = new MApplication(argc, app_name);
}

void Ut_MApplicationMenuView::cleanupTestCase()
{
    delete app;
}

void Ut_MApplicationMenuView::init()
{
    m_menu = new MApplicationMenu();
    m_subject = new MApplicationMenuView(m_menu);
}

void Ut_MApplicationMenuView::cleanup()
{
    delete m_menu;
    m_menu = 0;
    // menu destruction also destroys the view
}

void Ut_MApplicationMenuView::testSetView()
{
    QVERIFY(m_menu != 0);
    QVERIFY(m_subject != 0);

    m_menu->setView(m_subject);
}

void Ut_MApplicationMenuView::testActionsModel()
{
    m_menu->setView(m_subject);

    MAction* menuAction = new MAction("menuAction",m_menu);
    menuAction->setLocation(MAction::ApplicationMenuLocation);

    MAction* toolbarAction = new MAction("toolbarAction",m_menu);
    toolbarAction->setLocation(MAction::ToolBarLocation);

    MAction* everyLocationAction = new MAction("everyLocationAction",m_menu);
    everyLocationAction->setLocation(MAction::EveryLocation);
    everyLocationAction->setIconID("ICON");

    QAction* qaction = new QAction("qaction",m_menu);

    MWidgetAction* widgetAction = new MWidgetAction(m_menu);
    MButton* button = new MButton("widgetAction",m_menu);
    widgetAction->setWidget(button);

    m_menu->addAction(menuAction);
    m_menu->addAction(toolbarAction); // this action should be ignored by menu
    m_menu->addAction(everyLocationAction);
    m_menu->addAction(qaction);
    m_menu->addAction(widgetAction);

    QAbstractItemModel* itemModel = m_subject->d_func()->list->itemModel();
    QCOMPARE(itemModel->rowCount(), 4);

    QCOMPARE(itemModel->index(0,0).data(), QVariant("menuAction"));

    QCOMPARE(itemModel->index(1,0).data(), QVariant("everyLocationAction"));
    QCOMPARE(itemModel->index(1,0).data(Qt::DecorationRole), QVariant("ICON"));

    QCOMPARE(itemModel->index(2,0).data(), QVariant("qaction"));

    MWidgetAction* widgetActionInModel = dynamic_cast<MWidgetAction*>(itemModel->index(3,0).data(MActionsItemModel::ActionRole).value<QAction*>());
    QCOMPARE(widgetActionInModel, widgetAction);
    QCOMPARE(widgetActionInModel->widget(), button);
}

QTEST_APPLESS_MAIN(Ut_MApplicationMenuView)

