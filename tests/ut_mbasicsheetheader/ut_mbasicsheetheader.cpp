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

#include "ut_mbasicsheetheader.h"

#include <QObject>
#include <MApplication>
#include <QAction>

#include <mbasicsheetheader.h>

Ut_MBasicSheetHeader::Ut_MBasicSheetHeader():
        subject(0),
        app(0)
{
}

void Ut_MBasicSheetHeader::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbasicsheetheaderview" };
    app = new MApplication(argc, app_name);
}

void Ut_MBasicSheetHeader::cleanupTestCase()
{
    delete app;
}

void Ut_MBasicSheetHeader::init()
{
    subject = new MBasicSheetHeader;;
}

void Ut_MBasicSheetHeader::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_MBasicSheetHeader::testSettingPositiveAction()
{
    QAction *action = new QAction(subject);
    subject->setPositiveAction(action);
    QCOMPARE(subject->positiveAction(), action);
}

void Ut_MBasicSheetHeader::testSettingNegativeAction()
{
    QAction *action = new QAction(subject);
    subject->setNegativeAction(action);
    QCOMPARE(subject->negativeAction(), action);
}

void Ut_MBasicSheetHeader::testExternalDeletionOfPositiveAction()
{
    QObject actionOwner;
    QAction *action = new QAction(&actionOwner);

    subject->setPositiveAction(action);
    delete action;

    QCOMPARE(subject->positiveAction(), static_cast<QAction*>(0));
}

void Ut_MBasicSheetHeader::testExternalDeletionOfNegativeAction()
{
    QObject actionOwner;
    QAction *action = new QAction(&actionOwner);

    subject->setNegativeAction(action);
    delete action;

    QCOMPARE(subject->negativeAction(), static_cast<QAction*>(0));
}

QTEST_APPLESS_MAIN(Ut_MBasicSheetHeader)
