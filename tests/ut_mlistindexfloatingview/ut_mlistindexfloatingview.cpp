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
#include <MApplicationWindow>
#include <QStandardItemModel>
#include <QStandardItem>

#include "mlistindexfloatingview_p.h"
#include "mlistindex.h"
#include "mlistindexmodel.h"
#include "mlistindextooltip.h"
#include "mlistindextooltip_p.h"
#include <MList>
#include <MLabel>

#include "ut_mlistindexfloatingview.h"
#include "ut_mlistindexfloatingview.h"

MApplication *app;

void Ut_MListIndexFloatingView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MListIndexFloatingView" };
    app = new MApplication(argc, app_name);

    //new css will allow a maximum of 10 floating indexes
    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_mlistindexfloatingview.css");
}

void Ut_MListIndexFloatingView::cleanupTestCase()
{
    delete app;
}

void Ut_MListIndexFloatingView::init()
{
    m_widget = new MListIndex(0);
    m_subject = new MListIndexFloatingView(m_widget);
}

void Ut_MListIndexFloatingView::cleanup()
{
    delete m_widget;
    m_widget = 0;
}

void Ut_MListIndexFloatingView::testSetView()
{
    QVERIFY(m_widget != 0);
    QVERIFY(m_subject != 0);

    m_widget->setView(m_subject);
    QVERIFY(m_widget->view() == m_subject);
}

void Ut_MListIndexFloatingView::testHandleList()
{
    QVERIFY(m_widget != 0);
    QVERIFY(m_subject != 0);
    m_widget->setView(m_subject);

    // 10 items, 5 indexes
    QStandardItemModel model(10, 1);
    QMap<QModelIndex, QString> shortcuts;
    for (int i = 0; i < 10; i++) {
        model.appendRow(new QStandardItem(QString("item %1").arg(i)));

        if (i % 2 == 0) {
            shortcuts[model.index(i, 0)] = QString("s%1").arg(i / 2);
        }
    }

    MList list;
    list.setItemModel(&model);

    m_widget->setList(&list);
    m_widget->setShortcuts(shortcuts);

    // the list index is using the proper list
    QVERIFY(m_subject->d_func()->list == &list);

    // force the creation of the indexes data in the tooltip and check them
    m_subject->d_func()->updateTooltipData();
    QVERIFY(m_subject->d_func()->tooltip()->d_func()->indexLabels.size() == 5);
    QCOMPARE(m_subject->d_func()->tooltip()->d_func()->indexLabels.at(0)->text(), QString("s0"));
    QCOMPARE(m_subject->d_func()->tooltip()->d_func()->indexLabels.at(1)->text(), QString("s1"));
    QCOMPARE(m_subject->d_func()->tooltip()->d_func()->indexLabels.at(2)->text(), QString("s2"));
    QCOMPARE(m_subject->d_func()->tooltip()->d_func()->indexLabels.at(3)->text(), QString("s3"));
    QCOMPARE(m_subject->d_func()->tooltip()->d_func()->indexLabels.at(4)->text(), QString("s4"));
}


QTEST_APPLESS_MAIN(Ut_MListIndexFloatingView)
