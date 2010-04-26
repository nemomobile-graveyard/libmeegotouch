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

class MMultiColumnListViewPrivate;
class MyIndexedModel;

class Ut_MListViewMultiColumn : public QObject
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
    void testRowsInGroup();
    void testSizeHint();
    void testItemCountIndexedModel();
    void testItemCount();
    void testHorizontalSeparatorsCount();

    void testLocateVisibleIndexAt0();
    void testLocateVisibleIndexAt40();
    void testLocateVisibleIndexAt142();
    void testLocateVisibleIndexAt242();
    void testLocateVisibleIndexAt282();
    void testLocateVisibleIndexAt384();
    void testLocateVisibleIndexAt484();
    void testLocateVisibleIndexAt524();

    void testLocateVisibleRowAt0();
    void testLocateVisibleRowAt40();
    void testLocateVisibleRowAt40x151();
    void testLocateVisibleRowAt99();
    void testLocateVisibleRowAt142();
    void testLocateVisibleRowAt199();
    void testLocateVisibleRowAt242();
    void testLocateVisibleRowAt282();
    void testLocateVisibleRowAt384();
    void testLocateVisibleRowAt484();
    void testLocateVisibleRowAt524();

    void testLocatePosOfItemAt0Row();
    void testLocatePosOfItemAt1Row();
    void testLocatePosOfItemAt3Row();
    void testLocatePosOfItemAt4Row();
    void testLocatePosOfItemAt5Row();
    void testLocatePosOfItemAt7Row();
    void testLocatePosOfItemAt9Row();
    void testLocatePosOfItemAt10Row();

    void testLocatePosOfItemAt0Index();
    void testLocatePosOfItemAt1Index();
    void testLocatePosOfItemAt3Index();

    void testLocatePosOfItemAt4Index();
    void testLocatePosOfItemAt5Index();
    void testLocatePosOfItemAt7Index();
    void testLocatePosOfItemAt9Index();
    void testLocatePosOfItemAt10Index();

private:
    QObject *phoneBook;
    MyIndexedModel *phoneBookModel;
    MMultiColumnListViewPrivate *listViewPrivate;
};

#endif
