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

#include "ut_mnavigationbarview.h"

#include "mnavigationbarview_p.h"

#include <QGraphicsLayout>
#include <MLinearLayoutPolicy>

void Ut_MNavigationBarView::init()
{
}

void Ut_MNavigationBarView::cleanup()
{
}

void Ut_MNavigationBarView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mnavigationbarview" };
    app = new MApplication(argc, app_name);

    controller = new MNavigationBar();
    subject = new MNavigationBarView(controller);
    controller->setView(subject);
    model = controller->model();
}

void Ut_MNavigationBarView::cleanupTestCase()
{
    delete controller;
    controller = 0;

    delete app;
    app = 0;
}

void Ut_MNavigationBarView::testCustomContentOwnershipTransfer()
{
    // OBS: The ownership of customContent is transfered by
    // MNavigationBarView code.
    // That's why this test is here and not in Ut_MNavigationBar

    QGraphicsWidget *customContent = new QGraphicsWidget;

    controller->setCustomContent(customContent);

    QVERIFY(controller->isAncestorOf(customContent));

    controller->setCustomContent(0);

    QVERIFY(customContent->scene() == 0);
    QVERIFY(customContent->parent() == 0);

    // Clean up
    delete customContent;
}

void Ut_MNavigationBarView::testCustomContentPolicySelection()
{
    QGraphicsWidget *customContent = new QGraphicsWidget;

    QCOMPARE(subject->d_func()->customContentPolicy->isActive(), false);

    controller->setCustomContent(customContent);

    QCOMPARE(subject->d_func()->customContentPolicy->isActive(), true);

    controller->setCustomContent(0);

    QCOMPARE(subject->d_func()->customContentPolicy->isActive(), false);

    // Clean up
    delete customContent;
}

QTEST_APPLESS_MAIN(Ut_MNavigationBarView)
