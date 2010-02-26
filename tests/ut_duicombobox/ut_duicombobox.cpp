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
#include <QModelIndex>
#include <DuiApplication>
#include <QStandardItemModel>
#include <QVariant>
#include <QGraphicsLinearLayout>
#include <DuiImageWidget>
#include <DuiLabel>
#include <DuiWindow>
#include <DuiScene>
#include <DuiSceneManager>
#include <DuiPopupList>
#include <DuiContentItem>

#include "ut_duicombobox.h"
#include "duicancelevent.h"
#include "duicomboboxview.h"
#include "duicomboboxview_p.h"

DuiApplication *app;
DuiWindow *win;

void Ut_DuiComboBox::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_duicombobox" };
    app = new DuiApplication(argc, argv);

    win = new DuiWindow();
    win->setSceneManager(new DuiSceneManager());
}

void Ut_DuiComboBox::cleanupTestCase()
{
    delete win;
    win = NULL;
    delete app;
    app = NULL;
}

void Ut_DuiComboBox::init()
{
    m_combobox = new DuiComboBox();
}

void Ut_DuiComboBox::cleanup()
{
    if (m_combobox)
        delete m_combobox;
    m_combobox = NULL;
}

void Ut_DuiComboBox::testItemModel()
{
    QStringList buffer;
    int total = 10;
    for (int i = 0; i < total; i++)
        buffer << "Item" + QString::number(i);

    // test StandardItemModel
    m_combobox->addItems(buffer);

    QAbstractItemModel *itemModel = m_combobox->itemModel();
    QStandardItemModel *smodel = qobject_cast<QStandardItemModel *>(itemModel);
    QVERIFY(smodel != NULL);

    // test use model from outside
    QStringListModel model;
    model.setStringList(buffer);

    m_combobox->setItemModel(&model);
    QCOMPARE(&model, m_combobox->itemModel());
    QCOMPARE(m_combobox->count(), total);
}

void Ut_DuiComboBox::testActions()
{
    QStringList buffer;
    int total = 10;
    for (int i = 0; i < total; i++)
        buffer << "Item" + QString::number(i);

    // test add items
    m_combobox->addItems(buffer);
    QCOMPARE(m_combobox->count(), 10);

    QAbstractItemModel *itemModel = m_combobox->itemModel();
    QCOMPARE(itemModel->rowCount(), total);

    m_combobox->clear();
    QCOMPARE(m_combobox->count(), 0);

    m_combobox->addItem("Just a test");
    QCOMPARE(m_combobox->count(), 1);

    m_combobox->addItem("Icon", "Just a test");
    QCOMPARE(m_combobox->count(), 2);

    // test remove item
    m_combobox->removeItem(0);
    QCOMPARE(m_combobox->count(), 1);

    m_combobox->removeItem(0);
    QCOMPARE(m_combobox->count(), 0);

    // test insert item
    m_combobox->insertItem(0, "Just a test");
    QCOMPARE(m_combobox->count(), 1);

    m_combobox->insertItem(0, "Icon", "Just a test");
    QCOMPARE(m_combobox->count(), 2);

    m_combobox->clear();
    m_combobox->insertItems(0, buffer);
    QCOMPARE(m_combobox->count(), total);
}

void Ut_DuiComboBox::testCurrentIndex()
{
    QStringList buffer;
    int total = 10;
    for (int i = 0; i < total; i++)
        buffer << "Item" + QString::number(i);

    m_combobox->addItems(buffer);

    QSignalSpy spy1(m_combobox, SIGNAL(currentIndexChanged(int)));
    QSignalSpy spy2(m_combobox, SIGNAL(currentIndexChanged(QString)));

    m_combobox->setCurrentIndex(0);
    QCOMPARE(m_combobox->currentIndex(), 0);
    QCOMPARE(m_combobox->currentText(), QString("Item0"));

    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 1);

    QList<QVariant> arguments1 = spy1.takeFirst();
    QVERIFY(arguments1.at(0).type() == QVariant::Int);
    QVERIFY(arguments1.at(0).toInt() == 0);

    QList<QVariant> arguments2 = spy2.takeFirst();
    QVERIFY(arguments2.at(0).type() == QVariant::String);
    QCOMPARE(arguments2.at(0).toString(), QString("Item0"));
}

void Ut_DuiComboBox::testFunctions()
{
    QString value = "test";
    m_combobox->setIconID(value);
    QCOMPARE(m_combobox->iconID(), value);

    m_combobox->setTitle(value);
    QCOMPARE(m_combobox->title(), value);

    m_combobox->setIconVisible(false);
    QCOMPARE(m_combobox->isIconVisible(), false);

    m_combobox->setIconVisible(true);
    QCOMPARE(m_combobox->isIconVisible(), true);

    m_combobox->insertItem(0, "Just a test");
    m_combobox->setItemText(0, value);
    QCOMPARE(m_combobox->itemText(0), value);

    m_combobox->setItemIconID(0, value);
    QCOMPARE(m_combobox->itemIconID(0), value);
}

void Ut_DuiComboBox::testCancelEvent()
{
    QSignalSpy showPopupSpy(m_combobox, SIGNAL(showPopupList()));

    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
    m_combobox->mousePressEvent(&mouseEvent);
    QVERIFY(m_combobox->isDown() == true);

    DuiCancelEvent event;
    m_combobox->cancelEvent(&event);

    QCOMPARE(showPopupSpy.count(), 0);
    QVERIFY(m_combobox->isDown() == false);
}

void Ut_DuiComboBox::testIconVisibility()
{
    DuiComboBoxView *view = (DuiComboBoxView *)m_combobox->view();
    DuiComboBoxViewPrivate *viewPrivate = view->d_func();

    m_combobox->setTitle("Title");
    view->updateData(QList<const char *>() << DuiComboBoxModel::Title);
    QCOMPARE(viewPrivate->contentItem->itemStyle(), DuiContentItem::TwoTextLabels);
    QCOMPARE(viewPrivate->contentItem->title(), QString("Title"));
    QVERIFY(viewPrivate->contentItem->pixmap().isNull());

    m_combobox->setIconID("Icon-music");
    view->updateData(QList<const char *>() << DuiComboBoxModel::IconID);
    QCOMPARE(viewPrivate->contentItem->itemStyle(), DuiContentItem::IconAndTwoTextLabels);
    QVERIFY(!viewPrivate->contentItem->pixmap().isNull());

    m_combobox->setIconVisible(false);
    view->updateData(QList<const char *>() << DuiComboBoxModel::IconVisible);
    QCOMPARE(viewPrivate->contentItem->itemStyle(), DuiContentItem::TwoTextLabels);
    QCOMPARE(viewPrivate->contentItem->title(), QString("Title"));
    QVERIFY(viewPrivate->contentItem->pixmap().isNull());

    m_combobox->setIconVisible(true);
    view->updateData(QList<const char *>() << DuiComboBoxModel::IconVisible);
    QCOMPARE(viewPrivate->contentItem->itemStyle(), DuiContentItem::IconAndTwoTextLabels);
    QVERIFY(!viewPrivate->contentItem->pixmap().isNull());
}


void Ut_DuiComboBox::testClickSlot()
{
    win->scene()->addItem(m_combobox);

    DuiComboBoxView *view = (DuiComboBoxView *)m_combobox->view();
    DuiComboBoxViewPrivate *viewPrivate = view->d_func();

    m_combobox->addItem("Item1");
    m_combobox->addItem("Item2");

    m_combobox->click();

    QVERIFY(viewPrivate->popuplist != 0);
    QVERIFY(viewPrivate->popuplist->isVisible());

    win->scene()->removeItem(m_combobox);
}

void Ut_DuiComboBox::testBuiltinModel()
{
    DuiComboBoxView *view = (DuiComboBoxView *)m_combobox->view();
    DuiComboBoxViewPrivate *viewPrivate = view->d_func();

    m_combobox->addItems(QStringList() << "item0" << "item1" << "item2" << "item3");

    // select item2
    m_combobox->setCurrentIndex(2);
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("item2"));
    // remove item1, index should change
    m_combobox->removeItem(1);
    QCOMPARE(m_combobox->currentIndex(), 1);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("item2"));
    // remove current item
    m_combobox->removeItem(1);
    QCOMPARE(m_combobox->currentIndex(), -1);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("!! Tap to Select"));
    // check that after removing items everything looks ok
    QCOMPARE(m_combobox->count(), 2);
    m_combobox->setCurrentIndex(1);
    QCOMPARE(m_combobox->currentIndex(), 1);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("item3"));
    // add one item
    m_combobox->insertItem(0, QString("itemX"));
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("item3"));

    // change current item text
    m_combobox->setItemText(2, QString("beef"));
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("beef"));
}

void Ut_DuiComboBox::testModelSwitching()
{
    DuiComboBoxView *view = (DuiComboBoxView *)m_combobox->view();
    DuiComboBoxViewPrivate *viewPrivate = view->d_func();

    m_combobox->addItems(QStringList() << "item0" << "item1" << "item2" << "item3");

    m_combobox->setCurrentIndex(2);
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("item2"));

    QStringListModel *strListModel = new QStringListModel();
    strListModel->setStringList(QStringList() << "foo0" << "foo1" << "foo2" << "foo3");
    m_combobox->setItemModel(strListModel);

    // model changed selection should reset
    QCOMPARE(m_combobox->currentIndex(), -1);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("!! Tap to Select"));
}

void Ut_DuiComboBox::testStringListModel()
{
    DuiComboBoxView *view = (DuiComboBoxView *)m_combobox->view();
    DuiComboBoxViewPrivate *viewPrivate = view->d_func();

    QStringListModel *strListModel = new QStringListModel();
    strListModel->setStringList(QStringList() << "foo0" << "foo1" << "foo2" << "foo3");
    m_combobox->setItemModel(strListModel);

    // select item 2
    m_combobox->setCurrentIndex(2);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("foo2"));
    // remove item1, index should change
    strListModel->removeRow(1);
    QCOMPARE(m_combobox->currentIndex(), 1);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("foo2"));
    // remove current item
    strListModel->removeRow(1);
    QCOMPARE(m_combobox->currentIndex(), -1);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("!! Tap to Select"));
    // check that after removing items everything looks ok
    QCOMPARE(m_combobox->count(), 2);
    m_combobox->setCurrentIndex(1);
    QCOMPARE(m_combobox->currentIndex(), 1);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("foo3"));
    // add one item
    strListModel->insertRow(0);
    strListModel->setData(strListModel->index(0, 0), QString("fooX"), Qt::DisplayRole);
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("foo3"));

    // change current item text
    strListModel->setData(strListModel->index(2, 0), QString("lamb"), Qt::DisplayRole);
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("lamb"));
}

void Ut_DuiComboBox::testStringListModelSetStringList()
{
    DuiComboBoxView *view = (DuiComboBoxView *)m_combobox->view();
    DuiComboBoxViewPrivate *viewPrivate = view->d_func();

    QStringListModel *strListModel = new QStringListModel();
    strListModel->setStringList(QStringList() << "foo0" << "foo1" << "foo2" << "foo3");
    m_combobox->setItemModel(strListModel);

    // select foo2
    m_combobox->setCurrentIndex(2);
    // change string list completely
    strListModel->setStringList(QStringList() << "bar0" << "bar1" << "bar2" << "bar3");
    QCOMPARE(m_combobox->currentIndex(), -1);
    QCOMPARE(viewPrivate->contentItem->subtitle(), QString("!! Tap to Select"));
}

QTEST_APPLESS_MAIN(Ut_DuiComboBox)

