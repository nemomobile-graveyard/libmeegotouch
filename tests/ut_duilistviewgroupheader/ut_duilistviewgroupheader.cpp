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
#include "myindexedmodel.h"
#include "ut_duilistviewgroupheader.h"

using namespace DuiListViewPrivateNamespace;

void Ut_DuiListViewGroupHeader::initTestCase()
{
    makePhoneBook();
    makePhoneBookModel();
}

void Ut_DuiListViewGroupHeader::makePhoneBook()
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

void Ut_DuiListViewGroupHeader::makePhoneBookModel()
{
    phoneBookModel = new MyIndexedModel(phoneBook);
}

void Ut_DuiListViewGroupHeader::cleanupTestCase()
{
    delete phoneBookModel;
}

void Ut_DuiListViewGroupHeader::init()
{
    listViewPrivate = new DuiGroupHeaderListViewPrivate;
    listViewPrivate->model = phoneBookModel;

    DuiWidget * separator = new DuiWidget;
    separator->setGeometry(0, 0, 300, 2);
    separator->setPreferredHeight(2);
    listViewPrivate->setSeparator(separator);

    listViewPrivate->itemHeight = 100;

    listViewPrivate->setHeadersCreator(new DummyHeaderCreator);
    listViewPrivate->hdrHeight = GroupHeaderHeight;

    DuiListModel *model = new DuiListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(true);
    listViewPrivate->controllerModel = model;

    listViewPrivate->updateHeadersRows();
    listViewPrivate->updateHeadersPositions();
}

void Ut_DuiListViewGroupHeader::cleanup()
{
    delete listViewPrivate;
    delete listViewPrivate->controllerModel;
}

void Ut_DuiListViewGroupHeader::testPhoneBook()
{
    // Make sure our phone book model is valid
    QCOMPARE(phoneBook->children().size(), 3);
    QCOMPARE(phoneBook->children().at(0)->children().size(), 3);
    QCOMPARE(phoneBook->children().at(1)->children().size(), 4);
    QCOMPARE(phoneBook->children().at(2)->children().size(), 1);
}

void Ut_DuiListViewGroupHeader::testUpdateHeadersRows()
{
    QCOMPARE(3, listViewPrivate->headersRows.size());
    QCOMPARE(0, listViewPrivate->headersRows[0]);
    QCOMPARE(4, listViewPrivate->headersRows[1]);
    QCOMPARE(9, listViewPrivate->headersRows[2]);
}

void Ut_DuiListViewGroupHeader::testUpdateHeadersPositions()
{
    QCOMPARE(listViewPrivate->headersPositions.size(), 3);
    QCOMPARE(listViewPrivate->headersPositions[0], 0);
    QCOMPARE(listViewPrivate->headersPositions[1], 344);
    QCOMPARE(listViewPrivate->headersPositions[2], 790);
}

void Ut_DuiListViewGroupHeader::testHeaderHeightShouldBeZeroIfNoHeadersCreatorSpecified()
{
    QSKIP("Work in progress...", SkipSingle);
    listViewPrivate->setHeadersCreator(NULL);
    QCOMPARE(0, listViewPrivate->headerHeight());
}

void Ut_DuiListViewGroupHeader::testHeaderHeightShouldBeZeroIfHeadersNotVisible()
{
    QSKIP("Work in progress...", SkipSingle);
    listViewPrivate->controllerModel->setShowGroups(false);
    QCOMPARE(listViewPrivate->headerHeight(), 0);
}

void Ut_DuiListViewGroupHeader::testHeaderHeight()
{
    QCOMPARE(listViewPrivate->headerHeight(), GroupHeaderHeight);
}

void Ut_DuiListViewGroupHeader::testGroupSize()
{
    QCOMPARE(listViewPrivate->groupSize(0), 304);
    QCOMPARE(listViewPrivate->groupSize(1), 406);
    QCOMPARE(listViewPrivate->groupSize(2), 100);
}

void Ut_DuiListViewGroupHeader::testSizeHint()
{
    QCOMPARE(listViewPrivate->totalHeight(), 930);
}

void Ut_DuiListViewGroupHeader::testItemCountIndexedModel()
{
    QCOMPARE(listViewPrivate->itemsCount(), 8);
}

void Ut_DuiListViewGroupHeader::testItemCount()
{
    QCOMPARE(listViewPrivate->itemsCount(0), 3);
    QCOMPARE(listViewPrivate->itemsCount(1), 4);
    QCOMPARE(listViewPrivate->itemsCount(2), 1);
}

void Ut_DuiListViewGroupHeader::testSeparatorsCount()
{
    QCOMPARE(listViewPrivate->separatorsCount(), 5);
}

void Ut_DuiListViewGroupHeader::testFirstFlatRowToIndex()
{
    QModelIndex index(listViewPrivate->flatRowToIndex(0));
    QCOMPARE(index.row(), 0);
    QCOMPARE(index.parent(), QModelIndex());
}

void Ut_DuiListViewGroupHeader::testSecondFlatRowToIndex()
{
    QModelIndex index(listViewPrivate->flatRowToIndex(1));
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 0);
}

void Ut_DuiListViewGroupHeader::testLastFlatRowToIndex()
{
    QModelIndex index(listViewPrivate->flatRowToIndex(10));
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 2);
}

void Ut_DuiListViewGroupHeader::testAfterLastFlatRowToIndex()
{
    QModelIndex index(listViewPrivate->flatRowToIndex(11));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 2);
}

void Ut_DuiListViewGroupHeader::testInvalidIndexToFlatRow()
{
    QModelIndex index;
    QCOMPARE(listViewPrivate->indexToFlatRow(index), -1);
}

void Ut_DuiListViewGroupHeader::testFirstIndexWithoutParentToFlatRow()
{
    QModelIndex index = listViewPrivate->model->index(1, 0);
    QCOMPARE(listViewPrivate->indexToFlatRow(index), 4);
}

void Ut_DuiListViewGroupHeader::testIndexWithParentToFlatRow()
{
    QModelIndex index = listViewPrivate->model->index(1, 0, listViewPrivate->model->index(1, 0));
    QCOMPARE(listViewPrivate->indexToFlatRow(index), 6);
}

void Ut_DuiListViewGroupHeader::testLastIndexWithParentToFlatRow()
{
    QModelIndex index = listViewPrivate->model->index(0, 0, listViewPrivate->model->index(2, 0));
    QCOMPARE(listViewPrivate->indexToFlatRow(index), 10);
}

void Ut_DuiListViewGroupHeader::testLocateVisibleIndexAtZero()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(0));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(!index.parent().isValid());
}

void Ut_DuiListViewGroupHeader::testLocateVisibleIndexAt41()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(41));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 0);
}

void Ut_DuiListViewGroupHeader::testLocateVisibleIndexAtMiddleOfHeader()
{
    QModelIndex index(listViewPrivate->locateVisibleIndexAt(345));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 1);
    QVERIFY(!index.parent().isValid());
}

void Ut_DuiListViewGroupHeader::testLocateVisibleRowAt0()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(0), 0);
}

void Ut_DuiListViewGroupHeader::testLocateVisibleRowAt41()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(41), 1);
}

void Ut_DuiListViewGroupHeader::testLocateVisibleRowAt384()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(385), 5);
}

void Ut_DuiListViewGroupHeader::testLocateVisibleRowAt900()
{
    // last item
    QCOMPARE(listViewPrivate->locateVisibleRowAt(900), 11);
}

void Ut_DuiListViewGroupHeader::testLocateVisibleRowAt931()
{
    // last item
    QCOMPARE(listViewPrivate->locateVisibleRowAt(931), 11);
}

void Ut_DuiListViewGroupHeader::testLocatePosOfItemAt0Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(0), 0);
}

void Ut_DuiListViewGroupHeader::testLocatePosOfItemAt1Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(1), 40);
}

void Ut_DuiListViewGroupHeader::testLocatePosOfItemAt3Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(3), 244);
}

void Ut_DuiListViewGroupHeader::testLocatePosOfItemAt0Index()
{
    QModelIndex index(phoneBookModel->index(0, 0));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 0);
}

void Ut_DuiListViewGroupHeader::testLocatePosOfItemAt3Index()
{
    QModelIndex index(phoneBookModel->index(2, 0, phoneBookModel->index(0, 0)));
    QCOMPARE(listViewPrivate->locatePosOfItem(index), 244);
}

void Ut_DuiListViewGroupHeader::testLocatePosOfItemAt10Row()
{
    QCOMPARE(listViewPrivate->locatePosOfItem(10), 830);
}

void Ut_DuiListViewGroupHeader::testFindLowerIndexInEmptyArray()
{
    QVector<int> vec;
    QCOMPARE(dFindLowerIndex(vec, 0), 0);
    QCOMPARE(dFindLowerIndex(vec, 1), 0);
}

void Ut_DuiListViewGroupHeader::testFindLowerIndexIn2ItemsArray()
{
    QVector<int> vec;
    vec << 10 << 15;
    QCOMPARE(dFindLowerIndex(vec, 4), 0);
    QCOMPARE(dFindLowerIndex(vec, 10), 0);
    QCOMPARE(dFindLowerIndex(vec, 11), 0);
    QCOMPARE(dFindLowerIndex(vec, 15), 1);
    QCOMPARE(dFindLowerIndex(vec, 100), 1);
}

void Ut_DuiListViewGroupHeader::testFindLowerIndexIn3ItemsArray()
{
    QVector<int> vec;
    vec << 0 << 5 << 9;
    QCOMPARE(dFindLowerIndex(vec, 2), 0);
    QCOMPARE(dFindLowerIndex(vec, 5), 1);
    QCOMPARE(dFindLowerIndex(vec, 6), 1);
    QCOMPARE(dFindLowerIndex(vec, 9), 2);
}

void Ut_DuiListViewGroupHeader::testPerformance()
{
    QSKIP("currently doesn't work", SkipSingle);
    listViewPrivate->viewportTopLeft = QPoint(0, 0);
    listViewPrivate->viewportVisibleHeight = 800;
    for (int i = 0; i < 10; i++) {
        if (i % 1 == 0)
            listViewPrivate->viewportTopLeft = QPoint(0, 0);
        else
            listViewPrivate->viewportTopLeft = QPoint(0, 800);

        QModelIndex firstVisibleRow = listViewPrivate->locateVisibleIndexAt(listViewPrivate->viewportTopLeft.y());
        listViewPrivate->updateFirstVisibleRow(firstVisibleRow);
        QModelIndex lastVisibleRow = listViewPrivate->locateVisibleIndexAt(listViewPrivate->viewportTopLeft.y() + listViewPrivate->viewportVisibleHeight);
        listViewPrivate->updateLastVisibleRow(lastVisibleRow);

        QPoint firstVisibleItemPos(0, listViewPrivate->locatePosOfItem(firstVisibleRow));
        QPoint lastVisibleItemPos(0, listViewPrivate->locatePosOfItem(lastVisibleRow));
        listViewPrivate->removeInvisibleItems(firstVisibleItemPos, lastVisibleItemPos);

        if (listViewPrivate->model->rowCount() > 0)
            listViewPrivate->createVisibleItems(firstVisibleRow, lastVisibleRow);
    }
}

void Ut_DuiListViewGroupHeader2::makePhoneBook()
{
    phoneBook = new QObject;

    QObject *itemA = new QObject(phoneBook);
    itemA->setObjectName("A");

    QObject *itemB = new QObject(phoneBook);
    itemB->setObjectName("B");
}

void Ut_DuiListViewGroupHeader2::makePhoneBookModel()
{
    phoneBookModel = new MyIndexedModel(phoneBook);
}

void Ut_DuiListViewGroupHeader2::initTestCase()
{
    makePhoneBook();
    makePhoneBookModel();
}

void Ut_DuiListViewGroupHeader2::cleanupTestCase()
{
    delete phoneBookModel;
}

void Ut_DuiListViewGroupHeader2::init()
{
    listViewPrivate = new DuiGroupHeaderListViewPrivate;
    listViewPrivate->model = phoneBookModel;
    listViewPrivate->separator = new DuiWidget;
    listViewPrivate->separator->setGeometry(0, 0, 300, 2);
    listViewPrivate->itemHeight = 100;

    listViewPrivate->setHeadersCreator(new DummyHeaderCreator);
    listViewPrivate->hdrHeight = GroupHeaderHeight;

    DuiListModel *model = new DuiListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(true);
    listViewPrivate->controllerModel = model;

    listViewPrivate->updateHeadersRows();
    listViewPrivate->updateHeadersPositions();
}

void Ut_DuiListViewGroupHeader2::cleanup()
{
    delete listViewPrivate;
    delete listViewPrivate->controllerModel;
}

void Ut_DuiListViewGroupHeader2::testPhoneBook()
{
    QCOMPARE(phoneBook->children().size(), 2);
    QCOMPARE(phoneBook->children().at(0)->children().size(), 0);
    QCOMPARE(phoneBook->children().at(1)->children().size(), 0);
}

void Ut_DuiListViewGroupHeader2::testLocateVisibleRowAt0()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(0), 0);
}

void Ut_DuiListViewGroupHeader2::testLocateVisibleRowAt41()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(41), 1);
}

void Ut_DuiListViewGroupHeader2::testLocateVisibleRowAt100()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(100), 2);
}

void Ut_DuiListViewGroupHeader2::testOutOfBoundFlatRowToIndex()
{
    QCOMPARE(listViewPrivate->flatRowToIndex(2).row(), 1);
}

void Ut_DuiListViewGroupHeaderEmptyModel::makePhoneBook()
{
    phoneBook = new QObject;
}

void Ut_DuiListViewGroupHeaderEmptyModel::makePhoneBookModel()
{
    phoneBookModel = new MyIndexedModel(phoneBook);
}

void Ut_DuiListViewGroupHeaderEmptyModel::initTestCase()
{
    makePhoneBook();
    makePhoneBookModel();
}

void Ut_DuiListViewGroupHeaderEmptyModel::cleanupTestCase()
{
    delete phoneBookModel;
}

void Ut_DuiListViewGroupHeaderEmptyModel::init()
{
    listViewPrivate = new DuiGroupHeaderListViewPrivate;
    listViewPrivate->model = phoneBookModel;
    listViewPrivate->separator = new DuiWidget;
    listViewPrivate->separator->setGeometry(0, 0, 300, 2);
    listViewPrivate->itemHeight = 100;

    listViewPrivate->setHeadersCreator(new DummyHeaderCreator);
    listViewPrivate->hdrHeight = GroupHeaderHeight;

    DuiListModel *model = new DuiListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(true);
    listViewPrivate->controllerModel = model;

    listViewPrivate->updateHeadersRows();
    listViewPrivate->updateHeadersPositions();
}

void Ut_DuiListViewGroupHeaderEmptyModel::cleanup()
{
    delete listViewPrivate;
    delete listViewPrivate->controllerModel;
}

void Ut_DuiListViewGroupHeaderEmptyModel::testPhoneBook()
{
    QCOMPARE(phoneBook->children().size(), 0);
}

void Ut_DuiListViewGroupHeaderEmptyModel::testLocateVisibleRowAt0()
{
    QCOMPARE(listViewPrivate->locateVisibleRowAt(0), 0);
}

int main(int argc, char *argv[])
{
    Ut_DuiListViewGroupHeader tc;
    Ut_DuiListViewGroupHeader2 tc2;
    Ut_DuiListViewGroupHeaderEmptyModel tc3;

    return QTest::qExec(&tc, argc, argv) | QTest::qExec(&tc2, argc, argv) | QTest::qExec(&tc3, argc, argv);
}
