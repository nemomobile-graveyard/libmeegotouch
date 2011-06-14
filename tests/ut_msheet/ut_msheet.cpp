/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ut_msheet.h"

#include <QObject>
#include <MApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <MDismissEvent>

#include <mscenemanager.h>
#include <mwindow.h>

#include <msheet.h>

// QCOMPARE doesn't know MSceneWindow::SceneWindowSate enum. Thus it won't
// print "Expected" and "Actual" values in case of failure unless they're cast
// to a known type
#define STATE_COMPARE(s1, s2) QCOMPARE(static_cast<int>(s1), static_cast<int>(s2));

Ut_MSheet::Ut_MSheet():
        subject(0),
        app(0)
{
}

void Ut_MSheet::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_msheet" };
    app = new MApplication(argc, app_name);
}

void Ut_MSheet::cleanupTestCase()
{
    delete app;
}

void Ut_MSheet::init()
{
    subject = new MSheet;
}

void Ut_MSheet::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_MSheet::testClosingStandAloneWindowDismissesSheet()
{
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    QGraphicsView *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    EventSpy sheetDismissSpy;
    subject->installEventFilter(&sheetDismissSpy);

    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Appeared);

    standAloneWindow->close();

    QApplication::processEvents(); // sheet disappearance comes on next event loop

    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(sheetDismissSpy.typesOfEventsReceived.count(MDismissEvent::eventType()), 1);
}

void Ut_MSheet::testHidingStandAloneWindowMakesSheetDisappear()
{
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    QGraphicsView *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    EventSpy sheetDismissSpy;
    subject->installEventFilter(&sheetDismissSpy);

    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Appeared);

    standAloneWindow->hide();

    QApplication::processEvents(); // sheet disappearance comes on next event loop

    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(sheetDismissSpy.typesOfEventsReceived.count(MDismissEvent::eventType()), 0);
}

void Ut_MSheet::testDismissingSheetClosesStandAloneWindow()
{
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    QGraphicsView *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    QVERIFY(standAloneWindow->isVisible());

    EventSpy windowCloseSpy;
    standAloneWindow->installEventFilter(&windowCloseSpy);

    subject->dismiss();

    QVERIFY(!standAloneWindow->isVisible());
    QCOMPARE(windowCloseSpy.typesOfEventsReceived.count(QEvent::Close), 1);

    QApplication::processEvents(); // sheet disappearance comes on next event loop

    // Disappearance is immediate from scene manager's point of view
    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSheet::testMakingSheetDisappearHidesStandAloneWindow()
{
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    QGraphicsView *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    QVERIFY(standAloneWindow->isVisible());

    EventSpy windowCloseSpy;
    standAloneWindow->installEventFilter(&windowCloseSpy);

    subject->disappear();

    QVERIFY(!standAloneWindow->isVisible());
    QCOMPARE(windowCloseSpy.typesOfEventsReceived.count(QEvent::Close), 0);

    QApplication::processEvents(); // sheet disappearance comes on next event loop

    // Disappearance is immediate from scene manager's point of view
    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Disappeared);
}

/*
  If user wants its centralWidget to be bigger than the area of the
  central slot he has to add a MPannableViewport himself.
  This behavior also unsures that our internal pannable viewport
  doesn't messes up with some other pannable viewport that user might
  add as a child of his central widget
  */
void Ut_MSheet::testCentralWidgetDoesntGrowBeyondSlotLimits()
{
    QGraphicsWidget *bigWidget = new QGraphicsWidget;
    MSceneManager *sceneManager = new MSceneManager;
    MWindow *window = new MWindow(sceneManager);

    // Make bigWidget want to be bigger than the whole screen
    QSizeF bigSize = window->size();
    bigSize *= 2;
    bigWidget->setPreferredSize(bigSize);
    bigWidget->setMaximumSize(bigSize*4);

    subject->setCentralWidget(bigWidget);

    sceneManager->appearSceneWindowNow(subject);

    // Make sure all layouts set the geometries of their widgets
    app->processEvents();

    QGraphicsWidget *slot = bigWidget->parentWidget();

    QVERIFY(slot->size().width() <= window->size().width());
    QVERIFY(slot->size().height() <= window->size().height());

    // bigWidget should have exactly the same size of the slot
    // that contains it.
    QVERIFY(bigWidget->size().width() == slot->size().width());
    QVERIFY(bigWidget->size().height() == slot->size().height());
}

QGraphicsView *Ut_MSheet::fetchStandAloneWindowOfSubject()
{
    if (subject->scene()
            && subject->scene()->views().count() == 1) {
        return subject->scene()->views()[0];
    } else {
        return 0;
    }
}

bool EventSpy::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    typesOfEventsReceived << event->type();
    return false;
}

QTEST_APPLESS_MAIN(Ut_MSheet)
