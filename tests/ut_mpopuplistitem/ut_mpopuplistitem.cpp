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

#include "ut_mpopuplistitem.h"

#include "mapplication.h"
#include "mpopuplistview.h"
#include "mpopuplistview_p.h"
#include "mlabel.h"

void Ut_MPopupListItem::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mpopuplistitem" };
    app = new MApplication(argc, argv);
}

void Ut_MPopupListItem::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MPopupListItem::init()
{
    subject = new MPopupListItem(0);
}

void Ut_MPopupListItem::cleanup()
{
    delete subject;
    subject = NULL;
}

void Ut_MPopupListItem::testSetTitle_data()
{
    QTest::addColumn<QString>("title");
    QTest::newRow("empty title") << QString();
    QTest::newRow("non-empty title") << QString("Cosmic title string");
}

void Ut_MPopupListItem::testSetTitle()
{
    QFETCH(QString, title);
    subject->setTitle(title);
    QCOMPARE(subject->title->text(), title);
}

void Ut_MPopupListItem::testSetTitleForeground_data()
{
    QTest::addColumn<QColor>("color");
    QTest::addColumn<QColor>("expectedColor");

    QTest::newRow("no color") << QColor() << QColor();
    QTest::newRow("blue") << QColor(Qt::blue) << QColor(Qt::blue);
    QTest::newRow("red") << QColor(Qt::red) << QColor(Qt::red);
}

void Ut_MPopupListItem::testSetTitleForeground()
{
    QFETCH(QColor, color);
    QFETCH(QColor, expectedColor);

    subject->setTitleForeground(color);
    QCOMPARE(subject->title->color(), expectedColor);
}

QTEST_APPLESS_MAIN(Ut_MPopupListItem)
