/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include <DuiWidget>
#include "../../src/widgets/views/duilistview_p.h"
#include "duilistmodel.h"
#include "duiseparator.h"
#include "myindexedmodel.h"
#include "ut_duilistviewseparators.h"

void Ut_DuiListViewSeparators::initTestCase()
{
    makePhoneBook();
    makePhoneBookModel();
}

void Ut_DuiListViewSeparators::makePhoneBook()
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
}

void Ut_DuiListViewSeparators::makePhoneBookModel()
{
    phoneBookModel = new MyIndexedModel(phoneBook);
}

void Ut_DuiListViewSeparators::cleanupTestCase()
{
    delete phoneBookModel;
}

DuiWidget *Ut_DuiListViewSeparators::newHorizontalSeparator()
{
    DuiWidget *sep = new DuiWidget;
    sep->setGeometry(0, 0, 300, 2);
    sep->setPreferredHeight(17);
    return sep;
}

DuiWidget *Ut_DuiListViewSeparators::newVerticalSeparator()
{
    DuiWidget *sep = new DuiWidget;
    sep->setGeometry(0, 0, 2, 300);
    sep->setPreferredWidth(17);
    return sep;
}

void Ut_DuiListViewSeparators::init()
{
    listViewPrivate = new DuiPlainListViewPrivate;
    listViewPrivate->model = phoneBookModel;
    listViewPrivate->itemHeight = 100;
    listViewPrivate->viewWidth = 300;

    plainMultiColumnListViewPrivate = new DuiPlainMultiColumnListViewPrivate;
    plainMultiColumnListViewPrivate->model = phoneBookModel;
    plainMultiColumnListViewPrivate->itemHeight = 100;
    plainMultiColumnListViewPrivate->viewWidth = 300;

    groupedListViewPrivate = new DuiGroupHeaderListViewPrivate;
    groupedListViewPrivate->model = phoneBookModel;
    groupedListViewPrivate->itemHeight = 100;
    groupedListViewPrivate->viewWidth = 300;

    multiColumnListViewPrivate = new DuiMultiColumnListViewPrivate;
    multiColumnListViewPrivate->model = phoneBookModel;
    multiColumnListViewPrivate->itemHeight = 100;
    multiColumnListViewPrivate->viewWidth = 300;


    DuiListModel *model = new DuiListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(false);
    listViewPrivate->controllerModel = model;

    model = new DuiListModel;
    model->setColumns(2);
    model->setItemModel(phoneBookModel);
    model->setShowGroups(false);
    plainMultiColumnListViewPrivate->controllerModel = model;

    model = new DuiListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(true);
    groupedListViewPrivate->controllerModel = model;

    model = new DuiListModel;
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

void Ut_DuiListViewSeparators::testPlainSetNullHorizontalSeparator()
{
    listViewPrivate->setSeparator(NULL);
    QCOMPARE(listViewPrivate->hseparatorHeight, 0);
}

void Ut_DuiListViewSeparators::testPlainMulticolumnSetNullHorizontalSeparator()
{
    plainMultiColumnListViewPrivate->setSeparator(NULL);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparatorHeight, 0);
}

void Ut_DuiListViewSeparators::testGroupedSetNullHorizontalSeparator()
{
    groupedListViewPrivate->setSeparator(NULL);
    QCOMPARE(groupedListViewPrivate->hseparatorHeight, 0);
}

void Ut_DuiListViewSeparators::testGroupedMulticolumnSetNullHorizontalSeparator()
{
    multiColumnListViewPrivate->setSeparator(NULL);;
    QCOMPARE(multiColumnListViewPrivate->hseparatorHeight, 0);
}

void Ut_DuiListViewSeparators::testPlainMulticolumnSetNullVerticalSeparator()
{
    plainMultiColumnListViewPrivate->setVerticalSeparator(NULL);;
    QCOMPARE(plainMultiColumnListViewPrivate->vseparatorWidth, 0);
}

void Ut_DuiListViewSeparators::testGroupedMulticolumnSetNullVerticalSeparator()
{
    multiColumnListViewPrivate->setVerticalSeparator(NULL);;
    QCOMPARE(multiColumnListViewPrivate->vseparatorWidth, 0);
}

void Ut_DuiListViewSeparators::testPlainMulticolumnHorizontalSeparatorWidthWith2Columns()
{
    plainMultiColumnListViewPrivate->controllerModel->setColumns(2);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_DuiListViewSeparators::testPlainMulticolumnHorizontalSeparatorWidthWith4Columns()
{
    plainMultiColumnListViewPrivate->controllerModel->setColumns(4);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_DuiListViewSeparators::testPlainMulticolumnHorizontalSeparatorWidthWith8Columns()
{
    plainMultiColumnListViewPrivate->controllerModel->setColumns(8);
    QCOMPARE(plainMultiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_DuiListViewSeparators::testGroupedMulticolumnHorizontalSeparatorWidthWith2Columns()
{
    multiColumnListViewPrivate->controllerModel->setColumns(2);
    QCOMPARE(multiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_DuiListViewSeparators::testGroupedMulticolumnHorizontalSeparatorWidthWith4Columns()
{
    multiColumnListViewPrivate->controllerModel->setColumns(4);
    QCOMPARE(multiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_DuiListViewSeparators::testGroupedMulticolumnHorizontalSeparatorWidthWith8Columns()
{
    multiColumnListViewPrivate->controllerModel->setColumns(8);
    QCOMPARE(multiColumnListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_DuiListViewSeparators::testPlainHorizontalSeparatorWidth()
{
    QCOMPARE(listViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_DuiListViewSeparators::testGroupedHorizontalSeparatorWidth()
{
    QCOMPARE(groupedListViewPrivate->hseparator->size().width(), 300.0);
}

void Ut_DuiListViewSeparators::testPlainMulticolumnVerticalSeparatorWidth()
{
    QCOMPARE(plainMultiColumnListViewPrivate->vseparatorWidth, 17);
}

void Ut_DuiListViewSeparators::testGroupedMulticolumnVerticalSeparatorWidth()
{
    QCOMPARE(multiColumnListViewPrivate->vseparatorWidth, 17);
}

void Ut_DuiListViewSeparators::testPlainMulticolumnVerticalSeparatorHeight()
{
    QCOMPARE(plainMultiColumnListViewPrivate->vseparator->size().height(), 100.0);
}

void Ut_DuiListViewSeparators::testGroupedMulticolumnVerticalSeparatorHeight()
{
    QCOMPARE(multiColumnListViewPrivate->vseparator->size().height(), 100.0);
}


void Ut_DuiListViewSeparators::cleanup()
{
    delete listViewPrivate;
    delete listViewPrivate->controllerModel;

    delete plainMultiColumnListViewPrivate;
    delete plainMultiColumnListViewPrivate->controllerModel;

    delete groupedListViewPrivate;
    delete groupedListViewPrivate->controllerModel;

    delete multiColumnListViewPrivate;
    delete multiColumnListViewPrivate->controllerModel;
}

QTEST_APPLESS_MAIN(Ut_DuiListViewSeparators);
