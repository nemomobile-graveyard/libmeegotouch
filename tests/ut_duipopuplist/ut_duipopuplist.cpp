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

#include <QTest>
#include <QSignalSpy>
#include <QList>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QStringListModel>
#include <QModelIndex>
#include <DuiApplication>
#include <QStandardItemModel>

#include "duipopuplistview.h"
#include "duipopuplistview_p.h"
#include "duipopuplistview.cpp"
#include "duigriditem.h"

#include "ut_duipopuplist.h"

DuiApplication *app;

void Ut_DuiPopupList::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_duipopuplist" };
    app = new DuiApplication(argc, argv);
    m_popuplist = new DuiPopupList();
}


void Ut_DuiPopupList::cleanupTestCase()
{
    delete m_popuplist;
    m_popuplist = NULL;
    delete app;
    app = NULL;
}

void Ut_DuiPopupList::setContent()
{
    QStringList buffer;
    buffer << "Just a test1";

    QStringListModel model;
    model.setStringList(buffer);

    qRegisterMetaType<QAbstractItemModel *>("QAbstractItemModel*");
    QSignalSpy modelChangedSpy(m_popuplist, SIGNAL(itemModelChanged(QAbstractItemModel *)));
    // Check the signal does actually exist still
    QVERIFY(modelChangedSpy.isValid());

    model.setStringList(m_stringList);
    m_popuplist->setItemModel(&model);
    // Check that setModel worked for an empty model
    QCOMPARE(&model, m_popuplist->itemModel());
    // Check a modelChanged signal was sent
    QCOMPARE(modelChangedSpy.count(), 1);

    m_stringList << "one" << "two";
    m_model.setStringList(m_stringList);
    m_popuplist->setItemModel(&m_model);

    // Check that changing the model works and doesn't delete our old model
    QCOMPARE(&m_model, m_popuplist->itemModel());
    // Check another modelChanged signal was sent
    QCOMPARE(modelChangedSpy.count(), 2);

    // Check selectionModel
    qRegisterMetaType<QItemSelectionModel *>("QItemSelectionModel*");
    QSignalSpy selectionChangedSpy(m_popuplist, SIGNAL(selectionModelChanged(QItemSelectionModel *)));

    QItemSelectionModel *smodel = new QItemSelectionModel(&m_model, m_popuplist);
    m_popuplist->setSelectionModel(smodel);
    QCOMPARE(smodel, m_popuplist->selectionModel());
    QCOMPARE(selectionChangedSpy.count(), 1);
}

void Ut_DuiPopupList::testCurrentIndex()
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

void Ut_DuiPopupList::testSetItemIconID()
{
    DuiPopupListViewPrivate *view = new DuiPopupListViewPrivate;
    QStandardItemModel *itemModel = new QStandardItemModel;
    view->itemModel = itemModel;
    view->selectionModel = new QItemSelectionModel(itemModel);

    DuiContentItem *item;
    DuiWidgetRecycler recycler;

    // First add item with text to model and build it
    itemModel->appendRow(new QStandardItem("Item"));
    item = (DuiContentItem *)(view->createCell(itemModel->index(0, 0),recycler));
    // no icon was set to model, so it should be invisible
    QVERIFY(item->pixmap().isNull());

    // Add icon to previously set item
    itemModel->setData(itemModel->index(0, 0), "Icon-music", Qt::DecorationRole);
    view->updateCell(itemModel->index(0,0), item);
    // now, icon should be visible
    QVERIFY(!item->pixmap().isNull());

    delete itemModel;
    delete view;
}

QTEST_APPLESS_MAIN(Ut_DuiPopupList)
