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
#include <QGraphicsSceneMouseEvent>
#include <QStateMachine>
#include <QState>
#include "ut_mlistitem.h"
#include "mlistitem.h"
#include "mlistitemview.h"
#include "mlistitemview_p.h"
#include "mtapstatemachine.h"
#include "mtapstatemachine_p.h"

void Ut_MListItem::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mlistitem" };
    app = new MApplication(argc, app_name);
}

void Ut_MListItem::cleanupTestCase()
{
    delete app;
}

void Ut_MListItem::init()
{
    listItem = new MListItem();
}

void Ut_MListItem::cleanup()
{
    delete listItem;
}

void Ut_MListItem::testClick()
{
    listItem->click();
}

void Ut_MListItem::testLongTap()
{
    QPointF point (0, 0);
    listItem->longTap(point);
}

void Ut_MListItem::testClicked()
{
    listItem->clicked();
}

void Ut_MListItem::testLongTapped()
{
    QPointF point (0, 0);
    listItem->longTapped(point);
}

void Ut_MListItem::testReleasedOnHide()
{
    MListItemView *view = new MListItemView(listItem);
    listItem->setView(view);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addItem(listItem);

    // needed for MTapStateMachine to start working
    QCoreApplication::processEvents();

    QStateMachine *machine = view->d_func()->tapStateMachine->d_func()->highlightMachine;;
    QState *unhighlighted = view->d_func()->tapStateMachine->d_func()->unhighlightedState;
    QState *highlighted = view->d_func()->tapStateMachine->d_func()->pressHighlightedState;

    // initial state:
    // view must not be down
    QCOMPARE(view->d_func()->down, false);
    // state machine must be in unhighlighted state
    QVERIFY(machine->configuration().contains(unhighlighted));

    // send press event and wait the for the initial press time
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    scene->sendEvent(listItem, &pressEvent);
    QTest::qWait(1000);

    // pressed state
    // view must be down
    QCOMPARE(view->d_func()->down, true);
    // state machine must be in highlighted state
    QVERIFY(machine->configuration().contains(highlighted));

    // now hide the item
    listItem->hide();

    // hidden item state
    // view must not be down
    QCOMPARE(view->d_func()->down, false);
    // state machine must be in unhighlighted state
    QVERIFY(machine->configuration().contains(unhighlighted));

    scene->removeItem(listItem);
    delete scene;
}

QTEST_APPLESS_MAIN(Ut_MListItem)
