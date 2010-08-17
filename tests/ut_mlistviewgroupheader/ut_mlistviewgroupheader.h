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

#ifndef UT_MLABEL_H
#define UT_MLABEL_H

#include <QtTest/QtTest>
#include <QAbstractListModel>
#include <MWidget>
#include "mabstractcellcreator.h"
const int GroupHeaderHeight = 40;

class DummyHeaderWidget : public MWidget
{
public:
    DummyHeaderWidget() {
    }

    virtual QRectF boundingRect() const {
        return QRectF(QPointF(0, 0), QSizeF(800, GroupHeaderHeight));
    }

    void setViewType(const QString &viewType) {
        Q_UNUSED(viewType);
    }
};

class DummyHeaderCreator : public MAbstractCellCreator<DummyHeaderWidget>
{
public:
    virtual ~DummyHeaderCreator() {
    }

    virtual void updateCell(const QModelIndex &index, MWidget *cell) const {
        Q_UNUSED(index);
        Q_UNUSED(cell);
    }
};

class MGroupHeaderListViewPrivate;
class MyIndexedModel;
class MSortFilterProxyModel;

class Ut_MListViewGroupHeader : public QObject
{
    Q_OBJECT

private:
    void makePhoneBook();
    void makePhoneBookModel();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testPhoneBook();

    void testUpdateHeadersRows();
    void testUpdateHeadersPositions();
    void testHeaderHeightShouldBeZeroIfNoHeadersCreatorSpecified();
    void testHeaderHeightShouldBeZeroIfHeadersNotVisible();
    void testHeaderHeight();
    void testGroupSize();
    void testSizeHint();
    void testItemCountIndexedModel();
    void testItemCount();
    void testSeparatorsCount();

    void testFirstFlatRowToIndex();
    void testSecondFlatRowToIndex();
    void testLastFlatRowToIndex();
    void testAfterLastFlatRowToIndex();

    void testInvalidIndexToFlatRow();
    void testFirstIndexWithoutParentToFlatRow();
    void testIndexWithParentToFlatRow();
    void testLastIndexWithParentToFlatRow();

    void testLocateVisibleIndexAtZero();
    void testLocateVisibleIndexAt41();
    void testLocateVisibleIndexAtMiddleOfHeader();
    void testLocateVisibleRowAt0();
    void testLocateVisibleRowAt41();
    void testLocateVisibleRowAt139();
    void testLocateVisibleRowAt141();
    void testLocateVisibleRowAt143();
    void testLocateVisibleRowAt341();
    void testLocateVisibleRowAt345();
    void testLocateVisibleRowAt384();
    void testLocateVisibleRowAt900();
    void testLocateVisibleRowAt931();

    void testLocatePosOfItemAt0Row();
    void testLocatePosOfItemAt1Row();
    void testLocatePosOfItemAt3Row();
    void testLocatePosOfItemAt10Row();

    void testLocatePosOfItemAt0Index();
    void testLocatePosOfItemAt3Index();

    void testFindLowerIndexInEmptyArray();
    void testFindLowerIndexIn2ItemsArray();
    void testFindLowerIndexIn3ItemsArray();

    void testGroupFilteringWildcardC();
    void testGroupFilteringWildcardY();
    void testGroupFilteringWildcardAsh();

    void testPerformance();

private:
    QObject *phoneBook;
    MyIndexedModel *phoneBookModel;
    MSortFilterProxyModel *proxyModel;
    MGroupHeaderListViewPrivate *listViewPrivate;
};

class Ut_MListViewGroupHeader2 : public QObject
{
    Q_OBJECT

private:
    void makePhoneBook();
    void makePhoneBookModel();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testPhoneBook();
    void testLocateVisibleRowAt0();
    void testLocateVisibleRowAt41();
    void testLocateVisibleRowAt100();

    void testOutOfBoundFlatRowToIndex();

private:
    QObject *phoneBook;
    MyIndexedModel *phoneBookModel;
    MGroupHeaderListViewPrivate *listViewPrivate;
};


class Ut_MListViewGroupHeaderEmptyModel : public QObject
{
    Q_OBJECT

private:
    void makePhoneBook();
    void makePhoneBookModel();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testPhoneBook();

    void testLocateVisibleRowAt0();
private:
    QObject *phoneBook;
    MyIndexedModel *phoneBookModel;
    MGroupHeaderListViewPrivate *listViewPrivate;
};

#endif
