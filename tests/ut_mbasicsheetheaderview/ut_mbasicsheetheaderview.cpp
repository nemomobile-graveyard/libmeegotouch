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

void Ut_MBasicSheetHeaderView::testSettingMultipleActionOnPositiveButton()
{
    QString actionString1("Action 1");
    QString actionString2("Action 2");
    QAction *dummyAction1 = new QAction(actionString1, NULL);
    QAction *dummyAction2 = new QAction(actionString2, NULL);

    controller->setPositiveAction(dummyAction1);
    QVERIFY(subject->d_func()->positiveButton != 0);
    QCOMPARE(subject->d_func()->positiveButton->text(), actionString1);

    controller->setPositiveAction(dummyAction2);
    QVERIFY(subject->d_func()->positiveButton != 0);
    QCOMPARE(subject->d_func()->positiveButton->text(), actionString2);

    QSignalSpy spy1(dummyAction1, SIGNAL(triggered()));
    QSignalSpy spy2(dummyAction2, SIGNAL(triggered()));

    subject->d_func()->positiveButton->click();
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 1);

    delete dummyAction1;
    delete dummyAction2;
}

void Ut_MBasicSheetHeaderView::testSettingMultipleActionOnNegativeButton()
{
    QString actionString1("Action 1");
    QString actionString2("Action 2");
    QAction *dummyAction1 = new QAction(actionString1, NULL);
    QAction *dummyAction2 = new QAction(actionString2, NULL);

    controller->setNegativeAction(dummyAction1);
    QVERIFY(subject->d_func()->negativeButton != 0);
    QCOMPARE(subject->d_func()->negativeButton->text(), actionString1);

    controller->setNegativeAction(dummyAction2);
    QVERIFY(subject->d_func()->negativeButton != 0);
    QCOMPARE(subject->d_func()->negativeButton->text(), actionString2);

    QSignalSpy spy1(dummyAction1, SIGNAL(triggered()));
    QSignalSpy spy2(dummyAction2, SIGNAL(triggered()));

    subject->d_func()->negativeButton->click();
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 1);

    delete dummyAction1;
    delete dummyAction2;
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
