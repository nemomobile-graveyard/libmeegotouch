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

#include <QTest>
#include <QSignalSpy>
#include <QList>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QStringListModel>
#include <QModelIndex>
#include <MApplication>
#include <QStandardItemModel>

#include "mpopuplistview.h"
#include "mpopuplistview_p.h"
#include "mpopuplistview.cpp"

#include "ut_mpopuplist.h"


MApplication *app;

void Ut_MPopupList::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mpopuplist" };
    app = new MApplication(argc, argv);
    m_popuplist = new MPopupList();
    qRegisterMetaType<QModelIndex>("QModelIndex");
}


void Ut_MPopupList::cleanupTestCase()
{
    delete m_popuplist;
    m_popuplist = NULL;
    delete app;
    app = NULL;
}

void Ut_MPopupList::setContent()
{
    QStringList buffer;
    buffer << "Just a test1";

    QStringListModel model;
    model.setStringList(buffer);

    qRegisterMetaType<QAbstractItemModel *>("QAbstractItemModel*");

    model.setStringList(m_stringList);
    m_popuplist->setItemModel(&model);
    // Check that setModel worked for an empty model
    QCOMPARE(&model, m_popuplist->itemModel());

    m_stringList << "one" << "two";
    m_model.setStringList(m_stringList);
    m_popuplist->setItemModel(&m_model);

    // Check that changing the model works and doesn't delete our old model
    QCOMPARE(&m_model, m_popuplist->itemModel());

    // Check selectionModel
    qRegisterMetaType<QItemSelectionModel *>("QItemSelectionModel*");

    QItemSelectionModel *smodel = new QItemSelectionModel(&m_model, m_popuplist);
    m_popuplist->setSelectionModel(smodel);
    QCOMPARE(smodel, m_popuplist->selectionModel());
}

void Ut_MPopupList::testCurrentIndex()
{
    QSignalSpy currentIndexChangedSpy(m_popuplist, SIGNAL(currentIndexChanged(QModelIndex)));
    // Check the signals do actually exist still
    QVERIFY(currentIndexChangedSpy.isValid());

    // Check if we have NULL item model,  test current index
    m_popuplist->setItemModel(NULL);
    QCOMPARE(m_popuplist->currentIndex(), QModelIndex());

    m_popuplist->setCurrentIndex(m_model.index(1, 0));
    QCOMPARE(m_popuplist->currentIndex(), QModelIndex());

    m_popuplist->setItemModel(&m_model);

    // Check that we have the right number of items
    QCOMPARE(m_popuplist->itemModel()->rowCount(), 2);

    // We haven't set a current row yet - so should be invalid
    QCOMPARE(m_popuplist->currentIndex(), QModelIndex());

    //Check that changing rows works okay
    m_popuplist->setCurrentIndex(m_model.index(1, 0));
    QCOMPARE(m_popuplist->currentIndex().row(), 1);
    m_popuplist->setCurrentIndex(m_model.index(0, 0));
    QCOMPARE(m_popuplist->currentIndex().row(), 0);
    m_popuplist->setCurrentIndex(QModelIndex());
    QCOMPARE(m_popuplist->currentIndex(), QModelIndex());
    // Check we got three indexChanged signals
    QCOMPARE(currentIndexChangedSpy.count(), 3);

    m_popuplist->setCurrentIndex(m_model.index(0, 0, QModelIndex()));
    QCOMPARE(m_popuplist->currentIndex().row(), 0);
    m_popuplist->setCurrentIndex(m_model.index(1, 0, QModelIndex()));
    QCOMPARE(m_popuplist->currentIndex().row(), 1);
    // Check we got two more indexChanged signals
    QCOMPARE(currentIndexChangedSpy.count(), 5);

    //Change it to what it should already be on
    m_popuplist->setCurrentIndex(m_model.index(1, 0, QModelIndex()));
    QCOMPARE(m_popuplist->currentIndex().row(), 1);
    // Since we didn't change the current index, no new signals should have been sent
    QCOMPARE(currentIndexChangedSpy.count(), 5);
}

void Ut_MPopupList::testSetItemIconID()
{
    MList* list = new MList();
    QStandardItemModel *itemModel = new QStandardItemModel(list);
    MPopupListCellCreator *cellCreator = new MPopupListCellCreator(list);
    list->setCellCreator(cellCreator);
    list->setItemModel(itemModel);
    list->setSelectionModel(new QItemSelectionModel(itemModel));

    MPopupListItem *item;
    MWidgetRecycler recycler;

    // First add item with text to model and build it
    itemModel->appendRow(new QStandardItem("Item"));
    item = (MPopupListItem *)(cellCreator->createCell(itemModel->index(0, 0),recycler));
    QVERIFY(item->icon == 0);

    // Add icon to previously set item
    itemModel->setData(itemModel->index(0, 0), "icon-l-music", Qt::DecorationRole);
    cellCreator->updateCell(itemModel->index(0,0), item);
    QCOMPARE(item->icon->image(), QString("icon-l-music"));

    delete item;
    delete list;
}

void Ut_MPopupList::testSetItemPixmap()
{
    MList* list = new MList();
    QStandardItemModel *itemModel = new QStandardItemModel(list);
    MPopupListCellCreator *cellCreator = new MPopupListCellCreator(list);
    list->setCellCreator(cellCreator);
    list->setItemModel(itemModel);

    MPopupListItem *item;
    MWidgetRecycler recycler;

    // First add item with text to model and build it
    itemModel->appendRow(new QStandardItem("Item"));
    item = (MPopupListItem *)(cellCreator->createCell(itemModel->index(0, 0), recycler));
    QVERIFY(item->icon == 0);

    const QPixmap* pixmap = MTheme::pixmap("icon-l-music");

    // Add icon to previously set item
    itemModel->setData(itemModel->index(0, 0), *pixmap, Qt::DecorationRole);
    cellCreator->updateCell(itemModel->index(0,0), item);
    QVERIFY(item->icon != 0);
    QCOMPARE(*item->icon->pixmap(), *pixmap);

    delete item;
    delete list;

    MTheme::releasePixmap(pixmap);
}

void Ut_MPopupList::testQStandardItemWithIcon()
{
    MList* list = new MList();
    QStandardItemModel *itemModel = new QStandardItemModel(list);
    MPopupListCellCreator *cellCreator = new MPopupListCellCreator(list);
    list->setCellCreator(cellCreator);
    list->setItemModel(itemModel);

    MPopupListItem *item;
    MWidgetRecycler recycler;

    const QPixmap* pixmap = MTheme::pixmap("icon-l-music");

    // add item with icon and text
    itemModel->appendRow(new QStandardItem(*pixmap, "Item"));
    item = (MPopupListItem *)(cellCreator->createCell(itemModel->index(0, 0), recycler));
    QCOMPARE(item->title->text(), QString("Item"));
    QVERIFY(item->icon != 0);
    QCOMPARE(*item->icon->pixmap(), *pixmap);

    delete item;
    delete list;

    MTheme::releasePixmap(pixmap);
}

void Ut_MPopupList::testScrollTo()
{
    MPopupListViewPrivate *view = new MPopupListViewPrivate;
    QStandardItemModel *itemModel = new QStandardItemModel;

    for (int i=0; i<10; i++) {
        itemModel->appendRow(new QStandardItem(QString("Item" + QString::number(i+1))));
    }

    m_popuplist->setItemModel(itemModel);
    m_popuplist->setSelectionModel(new QItemSelectionModel(itemModel));

    QModelIndex index = itemModel->index(6, 0);
    QSignalSpy spy(m_popuplist, SIGNAL(scrollToIndex(QModelIndex)));

    m_popuplist->scrollTo(index);

    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QModelIndex result = qvariant_cast<QModelIndex>(spy.at(0).at(0));

    QCOMPARE( result, index );

    delete itemModel;
    delete view;
}

void Ut_MPopupList::testModelScrollTo()
{
    MPopupListViewPrivate *view = new MPopupListViewPrivate;
    QStandardItemModel *itemModel = new QStandardItemModel;

    for (int i=0; i<10; i++) {
        itemModel->appendRow(new QStandardItem(QString("Item" + QString::number(i+1))));
    }

    m_popuplist->setItemModel(itemModel);
    m_popuplist->setSelectionModel(new QItemSelectionModel(itemModel));

    QModelIndex index = itemModel->index(6, 0);
    m_popuplist->model()->setScrollToIndex( index );

    QCOMPARE(  m_popuplist->model()->scrollToIndex(), index );

    delete itemModel;
    delete view;
}

QTEST_APPLESS_MAIN(Ut_MPopupList)
