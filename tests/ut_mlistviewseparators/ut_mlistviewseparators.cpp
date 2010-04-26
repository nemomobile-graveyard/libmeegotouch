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

#include <MWidget>
#include "../../src/views/mlistview_p.h"
#include "mlistmodel.h"
#include "mseparator.h"
#include "myindexedmodel.h"
#include "ut_mlistviewseparators.h"

void Ut_MListViewSeparators::initTestCase()
{
    makePhoneBook();
    makePhoneBookModel();
}

void Ut_MListViewSeparators::makePhoneBook()
{
    phoneBook = new QObject;

    QObject *itemA = new QObject(phoneBook);
    itemA->setObjectName("A");

    QObject *item;
    item = new QObject(itemA);
    item->setObjectName("Adam");

    item = new QObject(itemA);
    item->setObjectName("Abby");

    item = new QObject(itemA);
    item->setObjectName("Ashley");

    QObject *itemB = new QObject(phoneBook);
    itemB->setObjectName("B");

    item = new QObject(itemB);
    item->setObjectName("Baby");

    item = new QObject(itemB);
    item->setObjectName("Bob");

    item = new QObject(itemB);
    item->setObjectName("Boby");

    item = new QObject(itemB);
    item->setObjectName("Britney");

    QObject *itemC = new QObject(phoneBook);
    itemC->setObjectName("C");

    item = new QObject(itemC);
    item->setObjectName("Cindy");

    QObject *itemD = new QObject(phoneBook);
    itemD->setObjectName("D");

    item = new QObject(itemD);
    item->setObjectName("Daby");

    item = new QObject(itemD);
    item->setObjectName("Dob");

    item = new QObject(itemD);
    item->setObjectName("Doby");

    item = new QObject(itemD);
    item->setObjectName("Dritney");
}

void Ut_MListViewSeparators::makePhoneBookModel()
{
    phoneBookModel = new MyIndexedModel(phoneBook);
}

void Ut_MListViewSeparators::cleanupTestCase()
{
    delete phoneBookModel;
}

MWidget *Ut_MListViewSeparators::newHorizontalSeparator()
{
    MWidget *sep = new MWidget;
    sep->setGeometry(0, 0, 300, 2);
    sep->setPreferredHeight(17);
    return sep;
}

MWidget *Ut_MListViewSeparators::newVerticalSeparator()
{
    MWidget *sep = new MWidget;
    sep->setGeometry(0, 0, 2, 300);
    sep->setPreferredWidth(17);
    return sep;
}

void Ut_MListViewSeparators::init()
{
    listViewPrivate = new MPlainListViewPrivate;
    listViewPrivate->model = phoneBookModel;
    listViewPrivate->itemHeight = 100;
    listViewPrivate->viewWidth = 300;
    listViewPrivate->rowCount = phoneBookModel->rowCount();

    plainMultiColumnListViewPrivate = new MPlainMultiColumnListViewPrivate;
    plainMultiColumnListViewPrivate->model = phoneBookModel;
    plainMultiColumnListViewPrivate->itemHeight = 100;
    plainMultiColumnListViewPrivate->viewWidth = 300;
    plainMultiColumnListViewPrivate->rowCount = phoneBookModel->rowCount();

    groupedListViewPrivate = new MGroupHeaderListViewPrivate;
    groupedListViewPrivate->model = phoneBookModel;
    groupedListViewPrivate->itemHeight = 100;
    groupedListViewPrivate->viewWidth = 300;
    groupedListViewPrivate->rowCount = phoneBookModel->rowCount();

    multiColumnListViewPrivate = new MMultiColumnListViewPrivate;
    multiColumnListViewPrivate->model = phoneBookModel;
    multiColumnListViewPrivate->itemHeight = 100;
    multiColumnListViewPrivate->viewWidth = 300;
    multiColumnListViewPrivate->rowCount = phoneBookModel->rowCount();


    MListModel *model = new MListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(false);
    listViewPrivate->controllerModel = model;

    model = new MListModel;
    model->setColumns(2);
    model->setItemModel(phoneBookModel);
    model->setShowGroups(false);
    plainMultiColumnListViewPrivate->controllerModel = model;

    model = new MListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(true);
    groupedListViewPrivate->controllerModel = model;

    model = new MListModel;
    model->setColumns(4);
    model->setItemModel(phoneBookModel);
    model->setShowGroups(true);
    multiColumnListViewPrivate->controllerModel = model;

    listViewPrivate->setSeparator(newHorizontalSeparator());
    plainMultiColumnListViewPrivate->setSeparator(newHorizontalSeparator());
    plainMultiColumnListViewPrivate->setVerticalSeparator(newVerticalSeparator());
    groupedListViewPrivate->setSeparator(newHorizontalSeparator());
    multiColumnListViewPrivate->setSeparator(newHorizontalSeparator());
    multiColumnListViewPrivate->setVerticalSeparator(newVerticalSeparator());

    listViewPrivate->updateSeparatorSize();
    plainMultiColumnListViewPrivate->updateSeparatorSize();
    groupedListViewPrivate->updateSeparatorSize();
    multiColumnListViewPrivate->updateSeparatorSize();
}

void Ut_MListViewSeparators::testPlainSetNullHorizontalSeparator()
{
    listViewPrivate->setSeparator(NULL);
    QCOMPARE(listViewPrivate->hseparatorHeight, 0);
}

void Ut_MListViewSeparators::testPlainMulticolumnSetNullHorizontalSeparator()
{
    plainMultiColumnListViewPrivate->setSeparator(NULL);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparatorHeight, 0);
}

void Ut_MListViewSeparators::testGroupedSetNullHorizontalSeparator()
{
    groupedListViewPrivate->setSeparator(NULL);
    QCOMPARE(groupedListViewPrivate->hseparatorHeight, 0);
}

void Ut_MListViewSeparators::testGroupedMulticolumnSetNullHorizontalSeparator()
{
    multiColumnListViewPrivate->setSeparator(NULL);;
    QCOMPARE(multiColumnListViewPrivate->hseparatorHeight, 0);
}

void Ut_MListViewSeparators::testPlainMulticolumnSetNullVerticalSeparator()
{
    plainMultiColumnListViewPrivate->setVerticalSeparator(NULL);;
    QCOMPARE(plainMultiColumnListViewPrivate->vseparatorWidth, 0);
}

void Ut_MListViewSeparators::testGroupedMulticolumnSetNullVerticalSeparator()
{
    multiColumnListViewPrivate->setVerticalSeparator(NULL);;
    QCOMPARE(multiColumnListViewPrivate->vseparatorWidth, 0);
}

void Ut_MListViewSeparators::testPlainMulticolumnHorizontalSeparatorWidthWith2Columns()
{
    plainMultiColumnListViewPrivate->controllerModel->setColumns(2);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_MListViewSeparators::testPlainMulticolumnHorizontalSeparatorWidthWith4Columns()
{
    plainMultiColumnListViewPrivate->controllerModel->setColumns(4);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_MListViewSeparators::testPlainMulticolumnHorizontalSeparatorWidthWith8Columns()
{
    plainMultiColumnListViewPrivate->controllerModel->setColumns(8);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_MListViewSeparators::testGroupedMulticolumnHorizontalSeparatorWidthWith2Columns()
{
    multiColumnListViewPrivate->controllerModel->setColumns(2);
    QCOMPARE(multiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_MListViewSeparators::testGroupedMulticolumnHorizontalSeparatorWidthWith4Columns()
{
    multiColumnListViewPrivate->controllerModel->setColumns(4);
    QCOMPARE(multiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_MListViewSeparators::testGroupedMulticolumnHorizontalSeparatorWidthWith8Columns()
{
    multiColumnListViewPrivate->controllerModel->setColumns(8);
    QCOMPARE(multiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_MListViewSeparators::testPlainHorizontalSeparatorWidth()
{
    QCOMPARE(listViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_MListViewSeparators::testGroupedHorizontalSeparatorWidth()
{
    QCOMPARE(groupedListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_MListViewSeparators::testPlainMulticolumnVerticalSeparatorWidth()
{
    QCOMPARE(plainMultiColumnListViewPrivate->vseparatorWidth, 17);
}

void Ut_MListViewSeparators::testGroupedMulticolumnVerticalSeparatorWidth()
{
    QCOMPARE(multiColumnListViewPrivate->vseparatorWidth, 17);
}

void Ut_MListViewSeparators::testPlainMulticolumnVerticalSeparatorHeight()
{
    QCOMPARE(plainMultiColumnListViewPrivate->vseparator->size().height(), 100.0);
}

void Ut_MListViewSeparators::testGroupedMulticolumnVerticalSeparatorHeight()
{
    QCOMPARE(multiColumnListViewPrivate->vseparator->size().height(), 100.0);
}

void Ut_MListViewSeparators::testPlainHorizontalSeparatorsCount()
{
    QCOMPARE(listViewPrivate->hseparatorsCount(), 3);
}

void Ut_MListViewSeparators::testGroupedHorizontalSeparatorsCount()
{
    QCOMPARE(groupedListViewPrivate->hseparatorsCount(), 8);
}

void Ut_MListViewSeparators::testPlainMulticolumnHorizontalSeparatorsCount2Columns()
{
    plainMultiColumnListViewPrivate->controllerModel->setColumns(2);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparatorsCount(), 1);
}

void Ut_MListViewSeparators::testPlainMulticolumnHorizontalSeparatorsCount3Columns()
{
    plainMultiColumnListViewPrivate->controllerModel->setColumns(3);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparatorsCount(), 1);
}

void Ut_MListViewSeparators::testPlainMulticolumnHorizontalSeparatorsCount4Columns()
{
    plainMultiColumnListViewPrivate->controllerModel->setColumns(4);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparatorsCount(), 0);
}

void Ut_MListViewSeparators::testGroupedMulticolumnHorizontalSeparatorsCount2Columns()
{
    multiColumnListViewPrivate->controllerModel->setColumns(2);
    QCOMPARE(multiColumnListViewPrivate->hseparatorsCount(), 3);
}

void Ut_MListViewSeparators::testGroupedMulticolumnHorizontalSeparatorsCount3Columns()
{
    multiColumnListViewPrivate->controllerModel->setColumns(3);
    QCOMPARE(multiColumnListViewPrivate->hseparatorsCount(), 2);
}

void Ut_MListViewSeparators::testGroupedMulticolumnHorizontalSeparatorsCount4Columns()
{
    multiColumnListViewPrivate->controllerModel->setColumns(4);
    QCOMPARE(multiColumnListViewPrivate->hseparatorsCount(), 0);
}

void Ut_MListViewSeparators::cleanup()
{
    delete listViewPrivate->controllerModel;
    listViewPrivate->controllerModel = NULL;
    delete listViewPrivate;

    delete plainMultiColumnListViewPrivate->controllerModel;
    plainMultiColumnListViewPrivate->controllerModel = NULL;
    delete plainMultiColumnListViewPrivate;

    delete groupedListViewPrivate->controllerModel;
    groupedListViewPrivate->controllerModel = NULL;
    delete groupedListViewPrivate;

    delete multiColumnListViewPrivate->controllerModel;
    multiColumnListViewPrivate->controllerModel = NULL;
    delete multiColumnListViewPrivate;
}

QTEST_APPLESS_MAIN(Ut_MListViewSeparators);
