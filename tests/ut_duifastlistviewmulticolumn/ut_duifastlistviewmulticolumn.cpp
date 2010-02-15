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
#include "../../src/widgets/views/duifastlistview_p.h"
#include "duilistmodel.h"
#include "myindexedmodel.h"
#include "ut_duifastlistviewmulticolumn.h"

using namespace DuiFastListViewPrivateNamespace;

void Ut_DuiFastListViewMultiColumn::initTestCase()
{
    makePhoneBook();
    makePhoneBookModel();
}

void Ut_DuiFastListViewMultiColumn::makePhoneBook()
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

void Ut_DuiFastListViewMultiColumn::makePhoneBookModel()
{
    phoneBookModel = new MyIndexedModel(phoneBook);
}

void Ut_DuiFastListViewMultiColumn::cleanupTestCase()
{
    delete phoneBookModel;
}

void Ut_DuiFastListViewMultiColumn::init()
{
    fastListViewPrivate = new DuiFastMultiColumnListViewPrivate;
    fastListViewPrivate->model = phoneBookModel;
    fastListViewPrivate->separator = new DuiWidget;
    fastListViewPrivate->separator->setGeometry(0, 0, 300, 2);
    fastListViewPrivate->hseparator = new DuiWidget;
    fastListViewPrivate->hseparator->setGeometry(0, 0, 2, 100);
    fastListViewPrivate->itemHeight = 100;
    fastListViewPrivate->viewWidth = 300;

    fastListViewPrivate->setHeadersCreator(new DummyHeaderCreator);
    fastListViewPrivate->hdrHeight = GroupHeaderHeight;

    DuiListModel *model = new DuiListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(true);
    model->setColumns(2);
    fastListViewPrivate->controllerModel = model;

    fastListViewPrivate->updateHeadersRows();
    fastListViewPrivate->updateHeadersPositions();
}

void Ut_DuiFastListViewMultiColumn::cleanup()
{
    delete fastListViewPrivate;
    delete fastListViewPrivate->controllerModel;
}

void Ut_DuiFastListViewMultiColumn::testPhoneBook()
{
    // Make sure our phone book model is valid
    QCOMPARE(phoneBook->children().size(), 3);
    QCOMPARE(phoneBook->children().at(0)->children().size(), 3);
    QCOMPARE(phoneBook->children().at(1)->children().size(), 4);
    QCOMPARE(phoneBook->children().at(2)->children().size(), 1);
}

void Ut_DuiFastListViewMultiColumn::testUpdateHeadersRows()
{
    QCOMPARE(fastListViewPrivate->headersRows.size(), 3);
    QCOMPARE(fastListViewPrivate->headersRows[0], 0);
    QCOMPARE(fastListViewPrivate->headersRows[1], 4);
    QCOMPARE(fastListViewPrivate->headersRows[2], 9);
}

void Ut_DuiFastListViewMultiColumn::testUpdateHeadersPositions()
{
    QCOMPARE(fastListViewPrivate->headersPositions.size(), 3);
    QCOMPARE(fastListViewPrivate->headersPositions[0], 0);
    QCOMPARE(fastListViewPrivate->headersPositions[1], 242);
    QCOMPARE(fastListViewPrivate->headersPositions[2], 484);
}

void Ut_DuiFastListViewMultiColumn::testHeaderHeightShouldBeZeroIfNoHeadersCreatorSpecified()
{
    QSKIP("Failing already in group header tests...", SkipSingle);
    fastListViewPrivate->setHeadersCreator(NULL);
    QCOMPARE(0, fastListViewPrivate->headerHeight());
}

void Ut_DuiFastListViewMultiColumn::testHeaderHeightShouldBeZeroIfHeadersNotVisible()
{
    QSKIP("Failing already in group header tests...", SkipSingle);
    fastListViewPrivate->controllerModel->setShowGroups(false);
    QCOMPARE(fastListViewPrivate->headerHeight(), 0);
}

void Ut_DuiFastListViewMultiColumn::testHeaderHeight()
{
    QCOMPARE(fastListViewPrivate->headerHeight(), GroupHeaderHeight);
}

void Ut_DuiFastListViewMultiColumn::testGroupSize()
{
    QCOMPARE(fastListViewPrivate->groupSize(0), 202);
    QCOMPARE(fastListViewPrivate->groupSize(1), 202);
    QCOMPARE(fastListViewPrivate->groupSize(2), 100);
}

void Ut_DuiFastListViewMultiColumn::testRowsInGroup()
{
    QCOMPARE(fastListViewPrivate->rowsInGroup(0), 2);
    QCOMPARE(fastListViewPrivate->rowsInGroup(1), 2);
    QCOMPARE(fastListViewPrivate->rowsInGroup(2), 1);
}

void Ut_DuiFastListViewMultiColumn::testSizeHint()
{
    QCOMPARE(fastListViewPrivate->totalHeight(), 624);
}

void Ut_DuiFastListViewMultiColumn::testItemCountIndexedModel()
{
    QCOMPARE(fastListViewPrivate->itemsCount(), 8);
}

void Ut_DuiFastListViewMultiColumn::testItemCount()
{
    QCOMPARE(fastListViewPrivate->itemsCount(0), 3);
    QCOMPARE(fastListViewPrivate->itemsCount(1), 4);
    QCOMPARE(fastListViewPrivate->itemsCount(2), 1);
}

void Ut_DuiFastListViewMultiColumn::testSeparatorsCount()
{
    QCOMPARE(fastListViewPrivate->separatorsCount(), 5);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleIndexAt0()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(0));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(!index.parent().isValid());
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleIndexAt40()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(40));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 0);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleIndexAt142()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(142));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 2);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 0);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleIndexAt242()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(242));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 1);
    QVERIFY(!index.parent().isValid());
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleIndexAt282()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(282));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 1);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleIndexAt384()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(384));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 2);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 1);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleIndexAt484()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(484));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 2);
    QVERIFY(!index.parent().isValid());
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleIndexAt524()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(524));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 2);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleRowAt0()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(0), 0);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleRowAt40()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(40), 1);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleRowAt40x151()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(40, 151), 2);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleRowAt142()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(142), 3);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleRowAt242()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(242), 4);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleRowAt282()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(282), 5);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleRowAt384()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(384), 7);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleRowAt484()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(484), 9);
}

void Ut_DuiFastListViewMultiColumn::testLocateVisibleRowAt524()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(524), 10);
}


void testLocatePosOfItemAt0Row();
void testLocatePosOfItemAt1Row();
void testLocatePosOfItemAt3Row();
void testLocatePosOfItemAt4Row();
void testLocatePosOfItemAt5Row();
void testLocatePosOfItemAt7Row();
void testLocatePosOfItemAt9Row();
void testLocatePosOfItemAt10Row();

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt0Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(0), 0);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt1Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(1), 40);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt3Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(3), 142);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt4Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(4), 242);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt5Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(5), 282);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt7Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(7), 384);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt9Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(9), 484);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt10Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(10), 524);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt0Index()
{
    QModelIndex index(phoneBookModel->index(0, 0));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 0);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt1Index()
{
    QModelIndex index(phoneBookModel->index(0, 0, phoneBookModel->index(0, 0)));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 40);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt3Index()
{
    QModelIndex index(phoneBookModel->index(2, 0, phoneBookModel->index(0, 0)));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 142);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt4Index()
{
    QModelIndex index(phoneBookModel->index(1, 0));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 242);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt5Index()
{
    QModelIndex index(phoneBookModel->index(0, 0, phoneBookModel->index(1, 0)));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 282);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt7Index()
{
    QModelIndex index(phoneBookModel->index(2, 0, phoneBookModel->index(1, 0)));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 384);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt9Index()
{
    QModelIndex index(phoneBookModel->index(2, 0));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 484);
}

void Ut_DuiFastListViewMultiColumn::testLocatePosOfItemAt10Index()
{
    QModelIndex index(phoneBookModel->index(0, 0, phoneBookModel->index(2, 0)));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 524);
}

QTEST_APPLESS_MAIN(Ut_DuiFastListViewMultiColumn);
