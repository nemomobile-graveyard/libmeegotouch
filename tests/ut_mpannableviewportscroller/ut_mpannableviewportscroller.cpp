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

#include <QObject>
#include <MDebug>
#include <MApplication>
#include <MPannableViewport>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MApplicationPage>

#include "mpannableviewportscroller.h"
#include "ut_mpannableviewportscroller.h"

void Ut_MPannableViewportScroller::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mpannableviewportscroller" };
    app = new MApplication(argc, app_name);
    app->setLoadMInputContext(false);

    appWindow = new MApplicationWindow();

    page = new MApplicationPage();
    page->setComponentsDisplayMode(MApplicationPage::AllComponents,
                                   MApplicationPageModel::Hide);
    viewport = page->pannableViewport();
    panned = page->centralWidget();
    panned->setMinimumHeight(2000);

    page->appear(appWindow);
    appWindow->show();

    app->processEvents();
    scroller = new MPannableViewportScroller();
}

void Ut_MPannableViewportScroller::cleanupTestCase()
{
    delete scroller;
    delete appWindow;
    delete app;
}

void Ut_MPannableViewportScroller::testQueryScrollingAmount()
{
    // get the size of the visible area
    QRect sceneRect(QRect(QPoint(), page->sceneManager()->visibleSceneSize()));

    // Ask how much the window we need to scroll to get to a position smaller than its range
    QPoint amount = scroller->queryScrollingAmount(viewport, sceneRect, QPoint(0, 1000), QPoint());
    // in this case, the amount should be the position asked
    QCOMPARE(amount, QPoint(0, -1000));

    // Ask how much the window we need to scroll to get to a position bigger than its range
    amount = scroller->queryScrollingAmount(viewport, sceneRect, QPoint(0, 1800), QPoint());
    // in this case, the amount should be the range of the viewport
    QCOMPARE(amount, QPoint(0, -viewport->range().height()));
}

void Ut_MPannableViewportScroller::testApplyScrolling()
{
    scroller->applyScrolling(viewport, QPoint(0, 1000), 0, QEasingCurve());
    QCOMPARE(viewport->position(), QPointF(0, -1000));
}

void Ut_MPannableViewportScroller::testStopScrolling()
{
    scroller->applyScrolling(viewport, QPoint(0, 0), 1000, QEasingCurve());
    QTest::qWait(200);
    scroller->stopScrolling(viewport);

    // interrupting the animation at 200ms when its duration is 1000 ensures
    // that we haven't achieved the final position
    QVERIFY(viewport->position() != QPoint(0,0));
}

void Ut_MPannableViewportScroller::testRestoreScrolling()
{
    // doesn't really do anything
    scroller->restoreScrolling(viewport);
}

QTEST_APPLESS_MAIN(Ut_MPannableViewportScroller)
