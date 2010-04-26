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
#include "myindexedmodel.h"
#include "ut_mlistviewmulticolumn.h"

using namespace MListViewPrivateNamespace;

void Ut_MListViewMultiColumn::initTestCase()
{
    makePhoneBook();
    makePhoneBookModel();
}

void Ut_MListViewMultiColumn::makePhoneBook()
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

void Ut_MListViewMultiColumn::makePhoneBookModel()
{
    phoneBookModel = new MyIndexedModel(phoneBook);
}

void Ut_MListViewMultiColumn::cleanupTestCase()
{
    delete phoneBookModel;
}

void Ut_MListViewMultiColumn::init()
{
    listViewPrivate = new MMultiColumnListViewPrivate;
    listViewPrivate->model = phoneBookModel;

    MWidget * separator = new MWidget;
    separator->setGeometry(0, 0, 300, 2);
    separator->setPreferredHeight(2);
    listViewPrivate->setSeparator(separator);

    listViewPrivate->hseparator = new MWidget;
    listViewPrivate->hseparator->setGeometry(0, 0, 2, 100);
    listViewPrivate->itemHeight = 100;
    listViewPrivate->viewWidth = 300;

    listViewPrivate->setHeadersCreator(new DummyHeaderCreator);
    listViewPrivate->hdrHeight = GroupHeaderHeight;

    MListModel *model = new MListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(true);
    model->setColumns(2);
    listViewPrivate->controllerModel = model;

    listViewPrivate->layoutChanged();
}

void Ut_MListViewMultiColumn::cleanup()
{
    delete listViewPrivate->controllerModel;
    listViewPrivate->controllerModel = NULL;
    delete listViewPrivate;
}

void Ut_MListViewMultiColumn::testPhoneBook()
{
    // Make sure our phone book model is valid
    QCOMPARE(phoneBook->children().size(), 3);
    QCOMPARE(phoneBook->children().at(0)->children().size(), 3);
    QCOMPARE(phoneBook->children().at(1)->children().size(), 4);
    QCOMPARE(phoneBook->children().at(2)->children().size(), 1);
}

void Ut_MListViewMultiColumn::testUpdateHeadersRows()
{
    QCOMPARE(listViewPrivate->headersRows.size(), 3);
    QCOMPARE(listViewPrivate->headersRows[0], 0);
    QCOMPARE(listViewPrivate->headersRows[1], 4);
    QCOMPARE(listViewPrivate->headersRows[2], 9);
}

void Ut_MListViewMultiColumn::testUpdateHeadersPositions()
{
    QCOMPARE(listViewPrivate->headersPositions.size(), 3);
    QCOMPARE(listViewPrivate->headersPositions[0], 0);
    QCOMPARE(listViewPrivate->headersPositions[1], 242);
    QCOMPARE(listViewPrivate->headersPositions[2], 484);
}

void Ut_MListViewMultiColumn::testHeaderHeightShouldBeZeroIfNoHeadersCreatorSpecified()
{
    QSKIP("Failing already in group header tests...", SkipSingle);
    listViewPrivate->setHeadersCreator(NULL);
    QCOMPARE(0, listViewPrivate->headerHeight());
}

void Ut_MListViewMultiColumn::testHeaderHeightShouldBeZeroIfHeadersNotVisible()
{
    QSKIP("Failing already in group header tests...", SkipSingle);
    listViewPrivate->controllerModel->setShowGroups(false);
    QCOMPARE(listViewPrivate->headerHeight(), 0);
}

void Ut_MListViewMultiColumn::testHeaderHeight()
{
    QCOMPARE(listViewPrivate->headerHeight(), GroupHeaderHeight);
}

void Ut_MListViewMultiColumn::testGroupSize()
{
    QCOMPARE(listViewPrivate->groupSize(0), 202);
    QCOMPARE(listViewPrivate->groupSize(1), 202);
    QCOMPARE(listViewPrivate->groupSize(2), 100);
}

void Ut_MListViewMultiColumn::testRowsInGroup()
{
    QCOMPARE(listViewPrivate->rowsInGroup(0), 2);
    QCOMPARE(listViewPrivate->rowsInGroup(1), 2);
    QCOMPARE(listViewPrivate->rowsInGroup(2), 1);
}

void Ut_MListViewMultiColumn::testSizeHint()
{
    QCOMPARE(listViewPrivate->totalHeight(), 624);
}

void Ut_MListViewMultiColumn::testItemCountIndexedModel()
{
    QCOMPARE(listViewPrivate->itemsCount(), 8);
}

void Ut_MListViewMultiColumn::testItemCount()
{
    QCOMPARE(listViewPrivate->itemsCount(0), 3);
    QCOMPARE(listViewPrivate->itemsCount(1), 4);
    QCOMPARE(listViewPrivate->itemsCount(2), 1);
}

void Ut_MListViewMultiColumn::testHorizontalSeparatorsCount()
{
    QCOMPARE(listViewPrivate->hseparatorsCount(), 2);
}

void Ut_MListViewMultiColumn::testLocateVisibleIndexAt0()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(0));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(!index.parent().isValid());
}

void Ut_MListViewMultiColumn::testLocateVisibleIndexAt40()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(40));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 0);
}

void Ut_MListViewMultiColumn::testLocateVisibleIndexAt142()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(142));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 2);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 0);
}

void Ut_MListViewMultiColumn::testLocateVisibleIndexAt242()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(242));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 1);
    QVERIFY(!index.parent().isValid());
}

void Ut_MListViewMultiColumn::testLocateVisibleIndexAt282()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(282));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 1);
}

void Ut_MListViewMultiColumn::testLocateVisibleIndexAt384()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(384));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 2);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 1);
}

void Ut_MListViewMultiColumn::testLocateVisibleIndexAt484()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(484));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 2);
    QVERIFY(!index.parent().isValid());
}

void Ut_MListViewMultiColumn::testLocateVisibleIndexAt524()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(524));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 2);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt0()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(0), 0);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt40()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(40), 1);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt40x151()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(40, 151), 2);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt99()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(99), 1);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt142()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(142), 3);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt199()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(199), 3);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt242()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(242), 4);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt282()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(282), 5);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt384()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(384), 7);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt484()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(484), 9);
}

void Ut_MListViewMultiColumn::testLocateVisibleRowAt524()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(524), 10);
}


void testLocatePosOfItemAt0Row();
void testLocatePosOfItemAt1Row();
void testLocatePosOfItemAt3Row();
void testLocatePosOfItemAt4Row();
void testLocatePosOfItemAt5Row();
void testLocatePosOfItemAt7Row();
void testLocatePosOfItemAt9Row();
void testLocatePosOfItemAt10Row();

void Ut_MListViewMultiColumn::testLocatePosOfItemAt0Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(0), 0);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt1Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(1), 40);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt3Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(3), 142);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt4Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(4), 242);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt5Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(5), 282);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt7Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(7), 384);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt9Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(9), 484);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt10Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(10), 524);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt0Index()
{
    QModelIndex index(phoneBookModel->index(0, 0));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 0);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt1Index()
{
    QModelIndex index(phoneBookModel->index(0, 0, phoneBookModel->index(0, 0)));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 40);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt3Index()
{
    QModelIndex index(phoneBookModel->index(2, 0, phoneBookModel->index(0, 0)));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 142);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt4Index()
{
    QModelIndex index(phoneBookModel->index(1, 0));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 242);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt5Index()
{
    QModelIndex index(phoneBookModel->index(0, 0, phoneBookModel->index(1, 0)));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 282);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt7Index()
{
    QModelIndex index(phoneBookModel->index(2, 0, phoneBookModel->index(1, 0)));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 384);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt9Index()
{
    QModelIndex index(phoneBookModel->index(2, 0));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 484);
}

void Ut_MListViewMultiColumn::testLocatePosOfItemAt10Index()
{
    QModelIndex index(phoneBookModel->index(0, 0, phoneBookModel->index(2, 0)));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 524);
}

QTEST_APPLESS_MAIN(Ut_MListViewMultiColumn);
