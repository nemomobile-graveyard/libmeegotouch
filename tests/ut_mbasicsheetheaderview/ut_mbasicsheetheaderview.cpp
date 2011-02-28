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

#include <QObject>
#include <MApplication>
#include <MButton>
#include <QAction>
#include <QSignalSpy>

#include <mbasicsheetheader.h>
#include <mbasicsheetheaderview.h>
#include <mbasicsheetheaderview_p.h>
#include "ut_mbasicsheetheaderview.h"


Ut_MBasicSheetHeaderView::Ut_MBasicSheetHeaderView():
        subject(0),
        controller(0),
        app(0)
{
}

void Ut_MBasicSheetHeaderView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbasicsheetheaderview" };
    app = new MApplication(argc, app_name);
}

void Ut_MBasicSheetHeaderView::cleanupTestCase()
{
    delete app;
}

void Ut_MBasicSheetHeaderView::init()
{
    controller = new MBasicSheetHeader();
    subject = new MBasicSheetHeaderView(controller);
    controller->setView(subject);
}

void Ut_MBasicSheetHeaderView::cleanup()
{
    delete controller;
    controller = 0;
}

void Ut_MBasicSheetHeaderView::testClickingPositiveButtonTriggersPositiveAction()
{
    QAction *action = new QAction(controller);
    controller->setPositiveAction(action);
    testClickingButtonTriggersAction(subject->d_func()->positiveButton, action);
}

void Ut_MBasicSheetHeaderView::testClickingNegativeButtonTriggersNegativeAction()
{
    QAction *action = new QAction(controller);
    controller->setNegativeAction(action);
    testClickingButtonTriggersAction(subject->d_func()->negativeButton, action);
}

void Ut_MBasicSheetHeaderView::testDisablingPositiveActionDisablesPositiveButton()
{
    QAction *action = new QAction(controller);
    controller->setPositiveAction(action);
    testDisablingActionDisablesButton(action, subject->d_func()->positiveButton);
}

void Ut_MBasicSheetHeaderView::testDisablingNegativeActionDisablesNegativeButton()
{
    QAction *action = new QAction(controller);
    controller->setNegativeAction(action);
    testDisablingActionDisablesButton(action, subject->d_func()->negativeButton);
}

void Ut_MBasicSheetHeaderView::testPositiveButtonTextUsesActionText()
{
    QAction *action = new QAction("foo", controller);
    controller->setPositiveAction(action);
    QCOMPARE(subject->d_func()->positiveButton->text(), action->text());
}

void Ut_MBasicSheetHeaderView::testNegativeButtonTextUsesActionText()
{
    QAction *action = new QAction("foo", controller);
    controller->setNegativeAction(action);
    QCOMPARE(subject->d_func()->negativeButton->text(), action->text());
}

/////
// Helpers

void Ut_MBasicSheetHeaderView::testClickingButtonTriggersAction(
        MButton *button, QAction *action)
{
    QSignalSpy spyActionTriggered(action, SIGNAL(triggered(bool)));

    QVERIFY(button != 0);

    QCOMPARE(spyActionTriggered.count(), 0);
    button->click();
    QCOMPARE(spyActionTriggered.count(), 1);
}

void Ut_MBasicSheetHeaderView::testDisablingActionDisablesButton(
        QAction *action, MButton *button)
{
    QVERIFY(button != 0);

    QCOMPARE(button->isEnabled(), action->isEnabled());
    action->setEnabled(false);
    QCOMPARE(button->isEnabled(), action->isEnabled());
}

QTEST_APPLESS_MAIN(Ut_MBasicSheetHeaderView)
