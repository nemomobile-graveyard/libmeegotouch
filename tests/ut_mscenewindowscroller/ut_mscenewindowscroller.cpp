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
#include <MSceneWindow>
#include <MApplicationWindow>
#include <MSceneManager>

#include "mscenewindowscroller.h"
#include "ut_mscenewindowscroller.h"

void Ut_MSceneWindowScroller::initTestCase()
{
    qRegisterMetaType<MSceneWindow*>("MSceneWindow*");

    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mscenewindowscroller" };
    app = new MApplication(argc, app_name);
    app->setLoadMInputContext(false);

    appWindow = new MApplicationWindow();

    sceneWindow = new MSceneWindow();
    sceneWindow->setMinimumHeight(2000);

    sceneWindow->appear(appWindow);
    appWindow->show();

    app->processEvents();
    scroller = new MSceneWindowScroller();
}

void Ut_MSceneWindowScroller::cleanupTestCase()
{
    delete scroller;
    delete appWindow;
    delete app;
}

void Ut_MSceneWindowScroller::testQueryScrollingAmount()
{
    // get the size of the visible area
    // we use landscape size because the scroller does so
    QRect sceneRect(QRect(QPoint(), sceneWindow->sceneManager()->visibleSceneSize(M::Landscape)));

    // Ask how much the window can scroll with its 2000 height using current scene rect and
    // with no initial offset
    QPoint amount = scroller->queryScrollingAmount(sceneWindow, sceneRect, QPoint(), QPoint());
    // in this concrete case the amount to scroll should be
    QPoint calculatedAmount(0, sceneRect.height() - sceneWindow->rect().height());

    QCOMPARE(amount, calculatedAmount);

    // but if we try with an initial offset of -100 for example
    amount = scroller->queryScrollingAmount(sceneWindow, sceneRect, QPoint(), QPoint(0, -100));
    // then the amount should reduce as we have already scrolled 100 pixels
    calculatedAmount = QPoint(0, sceneRect.height() - sceneWindow->rect().height() + 100);
    QCOMPARE(amount, calculatedAmount);
}

void Ut_MSceneWindowScroller::testApplyScrolling()
{
    // the scrolling is actually performed by the scene manager in response
    // to the scroller signals. We can't test the scrolling funcionality here
    // but just that the signal is emitted
    QSignalSpy spy(scroller, SIGNAL(sceneWindowDislocationRequest(MSceneWindow*, const QPointF&)));

    // when asking for an empty offset, the signal is not emitted
    scroller->applyScrolling(sceneWindow, QPoint(), 0, QEasingCurve());
    QVERIFY (spy.count() == 0);

    // otherwise the signal is emitted
    scroller->applyScrolling(sceneWindow, QPoint(0, -1000), 0, QEasingCurve());
    QVERIFY (spy.count() == 1);
}

void Ut_MSceneWindowScroller::testRestoreScrolling()
{
    // the scrolling is actually performed by the scene manager in response
    // to the scroller signals. We can't test the scrolling funcionality here
    // but just that the signal is emitted
    QSignalSpy spy(scroller, SIGNAL(sceneWindowUndoDislocationRequest(MSceneWindow*)));

    scroller->restoreScrolling(sceneWindow);
    QVERIFY (spy.count() == 1);
}

QTEST_APPLESS_MAIN(Ut_MSceneWindowScroller)
