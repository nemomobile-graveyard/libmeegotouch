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
#include "ut_duifastlistview.h"

void Ut_DuiListNewView::initTestCase()
{
    makePhoneBook();
    makePhoneBookModel();
}

void Ut_DuiListNewView::makePhoneBook()
{
}

void Ut_DuiListNewView::makePhoneBookModel()
{
}

void Ut_DuiListNewView::cleanupTestCase()
{
    delete phoneBookModel;
}

void Ut_DuiListNewView::init()
{
    fastListViewPrivate = new DuiFastGroupHeaderListViewPrivate;
    fastListViewPrivate->model = phoneBookModel;
    fastListViewPrivate->separator = new DuiWidget;
    fastListViewPrivate->separator->setGeometry(0, 0, 300, 2);
    fastListViewPrivate->itemHeight = 100;

    fastListViewPrivate->setHeadersCreator(new DummyHeaderCreator);
    fastListViewPrivate->hdrHeight = GroupHeaderHeight;

    DuiListModel *model = new DuiListModel;
    model->setItemModel(phoneBookModel);
    model->setShowGroups(true);
    fastListViewPrivate->controllerModel = model;

    fastListViewPrivate->updateHeadersRows();
    fastListViewPrivate->updateHeadersPositions();
}

void Ut_DuiListNewView::cleanup()
{
    delete fastListViewPrivate;
    delete fastListViewPrivate->controllerModel;
}

void Ut_DuiListNewView::testPhoneBook()
{
    // Make sure our phone book model is valid
    QCOMPARE(phoneBook->children().size(), 3);
    QCOMPARE(phoneBook->children().at(0)->children().size(), 3);
    QCOMPARE(phoneBook->children().at(1)->children().size(), 4);
    QCOMPARE(phoneBook->children().at(2)->children().size(), 1);
}

void Ut_DuiListNewView::testUpdateHeadersRows()
{
    QCOMPARE(3, fastListViewPrivate->headersRows.size());
    QCOMPARE(0, fastListViewPrivate->headersRows[0]);
    QCOMPARE(4, fastListViewPrivate->headersRows[1]);
    QCOMPARE(9, fastListViewPrivate->headersRows[2]);
}

void Ut_DuiListNewView::testUpdateHeadersPositions()
{
    QCOMPARE(fastListViewPrivate->headersPositions.size(), 3);
    QCOMPARE(fastListViewPrivate->headersPositions[0], 0);
    QCOMPARE(fastListViewPrivate->headersPositions[1], 344);
    QCOMPARE(fastListViewPrivate->headersPositions[2], 790);
}

void Ut_DuiListNewView::testHeaderHeightShouldBeZeroIfNoHeadersCreatorSpecified()
{
    fastListViewPrivate->setHeadersCreator(NULL);
    QCOMPARE(0, fastListViewPrivate->headerHeight());
}

void Ut_DuiListNewView::testHeaderHeightShouldBeZeroIfHeadersNotVisible()
{
    fastListViewPrivate->controllerModel->setShowGroups(false);
    QCOMPARE(fastListViewPrivate->headerHeight(), 0);
}

void Ut_DuiListNewView::testHeaderHeight()
{
    QCOMPARE(fastListViewPrivate->headerHeight(), GroupHeaderHeight);
}

void Ut_DuiListNewView::testGroupSize()
{
    QCOMPARE(fastListViewPrivate->groupSize(0), 304);
    QCOMPARE(fastListViewPrivate->groupSize(1), 406);
    QCOMPARE(fastListViewPrivate->groupSize(2), 100);
}

void Ut_DuiListNewView::testSizeHint()
{
    QCOMPARE(fastListViewPrivate->totalHeight(), 930);
}

void Ut_DuiListNewView::testItemCountIndexedModel()
{
    QCOMPARE(fastListViewPrivate->itemsCount(), 8);
}

void Ut_DuiListNewView::testItemCount()
{
    QCOMPARE(fastListViewPrivate->itemsCount(0), 3);
    QCOMPARE(fastListViewPrivate->itemsCount(1), 4);
    QCOMPARE(fastListViewPrivate->itemsCount(2), 1);
}

void Ut_DuiListNewView::testSeparatorsCount()
{
    QCOMPARE(fastListViewPrivate->separatorsCount(), 5);
}

void Ut_DuiListNewView::testFirstFlatRowToIndex()
{
    QModelIndex index(fastListViewPrivate->flatRowToIndex(0));
    QCOMPARE(index.row(), 0);
    QCOMPARE(index.parent(), QModelIndex());
}

void Ut_DuiListNewView::testSecondFlatRowToIndex()
{
    QModelIndex index(fastListViewPrivate->flatRowToIndex(1));
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 0);
}

void Ut_DuiListNewView::testLastFlatRowToIndex()
{
    QModelIndex index(fastListViewPrivate->flatRowToIndex(10));
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 2);
}

void Ut_DuiListNewView::testAfterLastFlatRowToIndex()
{
    QModelIndex index(fastListViewPrivate->flatRowToIndex(11));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 2);
}

void Ut_DuiListNewView::testInvalidIndexToFlatRow()
{
    QModelIndex index;
    QCOMPARE(fastListViewPrivate->indexToFlatRow(index), -1);
}

void Ut_DuiListNewView::testFirstIndexWithoutParentToFlatRow()
{
    QModelIndex index = fastListViewPrivate->model->index(1, 0);
    QCOMPARE(fastListViewPrivate->indexToFlatRow(index), 4);
}

void Ut_DuiListNewView::testIndexWithParentToFlatRow()
{
    QModelIndex index = fastListViewPrivate->model->index(1, 0, fastListViewPrivate->model->index(1, 0));
    QCOMPARE(fastListViewPrivate->indexToFlatRow(index), 6);
}

void Ut_DuiListNewView::testLastIndexWithParentToFlatRow()
{
    QModelIndex index = fastListViewPrivate->model->index(0, 0, fastListViewPrivate->model->index(2, 0));
    QCOMPARE(fastListViewPrivate->indexToFlatRow(index), 10);
}

void Ut_DuiListNewView::testLocateVisibleIndexAtZero()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(0));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(!index.parent().isValid());
}

void Ut_DuiListNewView::testLocateVisibleIndexAt41()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(41));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QVERIFY(index.parent().isValid());
    QCOMPARE(index.parent().row(), 0);
}

void Ut_DuiListNewView::testLocateVisibleIndexAtMiddleOfHeader()
{
    QModelIndex index(fastListViewPrivate->locateVisibleIndexAt(345));
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 1);
    QVERIFY(!index.parent().isValid());
}

void Ut_DuiListNewView::testLocateVisibleRowAt0()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(0), 0);
}

void Ut_DuiListNewView::testLocateVisibleRowAt41()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(41), 1);
}

void Ut_DuiListNewView::testLocateVisibleRowAt384()
{
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(385), 5);
}

void Ut_DuiListNewView::testLocateVisibleRowAt900()
{
    // last item
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(900), 11);
}

void Ut_DuiListNewView::testLocateVisibleRowAt931()
{
    // last item
    QCOMPARE(fastListViewPrivate->locateVisibleRowAt(931), 11);
}

void Ut_DuiListNewView::testLocatePosOfItemAt0Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(0), 0);
}

void Ut_DuiListNewView::testLocatePosOfItemAt1Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(1), 40);
}

void Ut_DuiListNewView::testLocatePosOfItemAt3Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(3), 244);
}

void Ut_DuiListNewView::testLocatePosOfItemAt0Index()
{
    QModelIndex index(phoneBookModel->index(0, 0));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 0);
}

void Ut_DuiListNewView::testLocatePosOfItemAt3Index()
{
    QModelIndex index(phoneBookModel->index(2, 0, phoneBookModel->index(0, 0)));
    QCOMPARE(fastListViewPrivate->locatePosOfItem(index), 244);
}

void Ut_DuiListNewView::testLocatePosOfItemAt10Row()
{
    QCOMPARE(fastListViewPrivate->locatePosOfItem(10), 830);
}

void Ut_DuiListNewView::testFindLowerIndexInEmptyArray()
{
    QVector<int> vec;
    QCOMPARE(dFindLowerIndex(vec, 0), 0);
    QCOMPARE(dFindLowerIndex(vec, 1), 0);
}

void Ut_DuiListNewView::testFindLowerIndexIn2ItemsArray()
{
    QVector<int> vec;
    vec << 10 << 15;
    QCOMPARE(dFindLowerIndex(vec, 4), 0);
    QCOMPARE(dFindLowerIndex(vec, 10), 0);
    QCOMPARE(dFindLowerIndex(vec, 11), 0);
    QCOMPARE(dFindLowerIndex(vec, 15), 1);
    QCOMPARE(dFindLowerIndex(vec, 100), 1);
}

void Ut_DuiListNewView::testFindLowerIndexIn3ItemsArray()
{
    QVector<int> vec;
    vec << 0 << 5 << 9;
    QCOMPARE(dFindLowerIndex(vec, 2), 0);
    QCOMPARE(dFindLowerIndex(vec, 5), 1);
    QCOMPARE(dFindLowerIndex(vec, 6), 1);
    QCOMPARE(dFindLowerIndex(vec, 9), 2);
}

void Ut_DuiListNewView::testPerformance()
{
    QSKIP("currently doesn't work", SkipSingle);
    fastListViewPrivate->viewportTopLeft = QPoint(0, 0);
    fastListViewPrivate->viewportVisibleHeight = 800;
    for (int i = 0; i < 10; i++) {
        if (i % 1 == 0)
            fastListViewPrivate->viewportTopLeft = QPoint(0, 0);
        else
            fastListViewPrivate->viewportTopLeft = QPoint(0, 800);

        QModelIndex firstVisibleRow = fastListViewPrivate->locateVisibleIndexAt(fastListViewPrivate->viewportTopLeft.y());
        fastListViewPrivate->updateFirstVisibleRow(firstVisibleRow);
        QModelIndex lastVisibleRow = fastListViewPrivate->locateVisibleIndexAt(fastListViewPrivate->viewportTopLeft.y() + fastListViewPrivate->viewportVisibleHeight);
        fastListViewPrivate->updateLastVisibleRow(lastVisibleRow);

        QPoint firstVisibleItemPos(0, fastListViewPrivate->locatePosOfItem(firstVisibleRow));
        QPoint lastVisibleItemPos(0, fastListViewPrivate->locatePosOfItem(lastVisibleRow));
        fastListViewPrivate->removeInvisibleItems(firstVisibleItemPos, lastVisibleItemPos);

        if (fastListViewPrivate->model->rowCount() > 0)
            fastListViewPrivate->createVisibleItems(firstVisibleRow, lastVisibleRow);
    }
}

QTEST_APPLESS_MAIN(Ut_DuiListNewView);
