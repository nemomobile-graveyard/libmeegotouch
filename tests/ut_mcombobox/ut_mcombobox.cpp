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
#include <QModelIndex>
#include <MApplication>
#include <QStandardItemModel>
#include <QVariant>
#include <QGraphicsLinearLayout>
#include <MImageWidget>
#include <MLabel>
#include <MWindow>
#include <MScene>
#include <MSceneManager>
#include <MPopupList>
#include <MLocale>

#include "ut_mcombobox.h"
#include "mcancelevent.h"
#include "mcomboboxview.h"
#include "mcomboboxview_p.h"
#include "mcomboboxbutton.h"
#include "mlayout.h"
#include "mgridlayoutpolicy.h"

MApplication *app;
MWindow *win;

void Ut_MComboBox::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mcombobox" };
    app = new MApplication(argc, argv);

    // make sure that we only have engineering English loaded:
    MLocale locale;
    locale.removeTrCatalog("libmeegotouch"); // removes libmeegotouch.qm and libmeegotouch_<localeName>.qm
    locale.removeTrCatalog("common"); // removes common.qm and common_<localeName>.qm
    locale.installTrCatalog("libmeegotouch.qm"); // installs libmeegotouch.qm (engineering English)
    MLocale::setDefault(locale);

    win = new MWindow();
    win->setSceneManager(new MSceneManager());
}

void Ut_MComboBox::cleanupTestCase()
{
    delete win;
    win = NULL;
    delete app;
    app = NULL;
}

void Ut_MComboBox::init()
{
    m_combobox = new MComboBox();
}

void Ut_MComboBox::cleanup()
{
    if (m_combobox)
        delete m_combobox;
    m_combobox = NULL;
}

void Ut_MComboBox::testItemModel()
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

void Ut_MComboBox::testActions()
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

void Ut_MComboBox::testCurrentIndex()
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

void Ut_MComboBox::testFunctions()
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

void Ut_MComboBox::testIconVisibility()
{
    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();
    MLayout* comboboxButtonLayout = static_cast<MLayout*>(viewPrivate->button->layout());

    m_combobox->setTitle("Title");
    view->updateData(QList<const char *>() << MComboBoxModel::Title);
    QCOMPARE(comboboxButtonLayout->policy(), viewPrivate->button->policy[MComboBoxButton::TitleShown]);
    QCOMPARE(viewPrivate->button->titleWidget()->text(), QString("Title"));

    m_combobox->setIconID("icon-l-music");
    view->updateData(QList<const char *>() << MComboBoxModel::IconID);
    QCOMPARE(comboboxButtonLayout->policy(), viewPrivate->button->policy[MComboBoxButton::TitleShown|MComboBoxButton::IconShown]);
    QCOMPARE(viewPrivate->button->iconWidget()->image(), QString("icon-l-music"));

    m_combobox->setIconVisible(false);
    view->updateData(QList<const char *>() << MComboBoxModel::IconVisible);
    QCOMPARE(comboboxButtonLayout->policy(), viewPrivate->button->policy[MComboBoxButton::TitleShown]);
    QCOMPARE(viewPrivate->button->titleWidget()->text(), QString("Title"));
    QCOMPARE(viewPrivate->button->iconWidget()->image(), QString("icon-l-music"));

    m_combobox->setIconVisible(true);
    view->updateData(QList<const char *>() << MComboBoxModel::IconVisible);
    QCOMPARE(comboboxButtonLayout->policy(), viewPrivate->button->policy[MComboBoxButton::TitleShown|MComboBoxButton::IconShown]);
    QCOMPARE(viewPrivate->button->iconWidget()->image(), QString("icon-l-music"));
}


void Ut_MComboBox::testClickSlot()
{
    win->scene()->addItem(m_combobox);

    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();

    m_combobox->addItem("Item1");
    m_combobox->addItem("Item2");

    m_combobox->click();

    QVERIFY(viewPrivate->popuplist != 0);
    QVERIFY(viewPrivate->popuplist->isVisible());

    win->scene()->removeItem(m_combobox);
}

void Ut_MComboBox::testDismissSlot()
{
    win->scene()->addItem(m_combobox);
    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();
    m_combobox->addItems(QStringList() << "Item1" << "Item2");

    QSignalSpy dismissedSpy(m_combobox, SIGNAL(dismissed()));
    m_combobox->click();
    m_combobox->dismiss();
    win->sceneManager()->fastForwardSceneWindowTransitionAnimation(viewPrivate->popuplist);
    QCOMPARE(dismissedSpy.count(), 1);
    dismissedSpy.clear();

    m_combobox->click();
    QSignalSpy rejectedSpy(viewPrivate->popuplist, SIGNAL(rejected()));
    m_combobox->dismiss();
    QCOMPARE(dismissedSpy.count(), 1);
    QCOMPARE(rejectedSpy.count(), 1);
    win->sceneManager()->fastForwardSceneWindowTransitionAnimation(viewPrivate->popuplist);
    QCOMPARE(viewPrivate->popuplist->sceneWindowState(), MSceneWindow::Disappeared);

    win->scene()->removeItem(m_combobox);
}

void Ut_MComboBox::testBuiltinModel()
{
    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();
    QSignalSpy spy(m_combobox, SIGNAL(currentIndexChanged(int)));

    m_combobox->addItems(QStringList() << "item0" << "item1" << "item2" << "item3");

    // select item2
    m_combobox->setCurrentIndex(2);
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("item2"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeLast()[0], QVariant(2));
    // remove item1, index should change
    m_combobox->removeItem(1);
    QCOMPARE(m_combobox->currentIndex(), 1);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("item2"));
    // remove current item
    m_combobox->removeItem(1);
    QCOMPARE(m_combobox->currentIndex(), -1);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), qtTrId("qtn_comm_popup_default"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeLast()[0], QVariant(-1));
    // check that after removing items everything looks ok
    QCOMPARE(m_combobox->count(), 2);
    m_combobox->setCurrentIndex(1);
    QCOMPARE(m_combobox->currentIndex(), 1);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("item3"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeLast()[0], QVariant(1));
    // add one item
    m_combobox->insertItem(0, QString("itemX"));
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("item3"));
    QCOMPARE(spy.count(), 0);

    // change current item text
    m_combobox->setItemText(2, QString("beef"));
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("beef"));
}

void Ut_MComboBox::testSelectionModel()
{
    m_combobox->addItems(QStringList() << "Item0" << "Item1" << "Item2");
    QItemSelectionModel* oldSelectionModel = m_combobox->selectionModel();

    QStringListModel* otherModel = new QStringListModel(QStringList() << "A" << "B" << "C");
    QItemSelectionModel* badSelectionModel = new QItemSelectionModel(otherModel);
    badSelectionModel->setCurrentIndex(otherModel->index(2,0), QItemSelectionModel::SelectCurrent);
    m_combobox->setSelectionModel(badSelectionModel);
    QCOMPARE(m_combobox->selectionModel(), oldSelectionModel);
    QCOMPARE(m_combobox->currentIndex(), -1);

    QItemSelectionModel* selectionModel = new QItemSelectionModel(m_combobox->itemModel());
    selectionModel->setCurrentIndex(m_combobox->itemModel()->index(1,0), QItemSelectionModel::SelectCurrent);
    m_combobox->setSelectionModel(selectionModel);
    QCOMPARE(m_combobox->selectionModel(), selectionModel);
    QCOMPARE(m_combobox->currentIndex(), 1);
}

void Ut_MComboBox::testModelSwitching()
{
    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();
    QSignalSpy spy(m_combobox, SIGNAL(currentIndexChanged(int)));

    m_combobox->addItems(QStringList() << "item0" << "item1" << "item2" << "item3");

    m_combobox->setCurrentIndex(2);
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("item2"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeLast()[0], QVariant(2));

    QStringListModel *strListModel = new QStringListModel();
    strListModel->setStringList(QStringList() << "foo0" << "foo1" << "foo2" << "foo3");
    m_combobox->setItemModel(strListModel);

    // model changed selection should reset
    QCOMPARE(m_combobox->currentIndex(), -1);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), qtTrId("qtn_comm_popup_default"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeLast()[0], QVariant(-1));

    // currentIndexChanged signals should not be emitted
    m_combobox->insertItems(0, QStringList() << "bar1" << "bar2" << "bar3");
    QCOMPARE(spy.count(), 0);
}

void Ut_MComboBox::testStringListModel()
{
    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();

    QStringListModel *strListModel = new QStringListModel();
    strListModel->setStringList(QStringList() << "foo0" << "foo1" << "foo2" << "foo3");
    m_combobox->setItemModel(strListModel);

    // select item 2
    m_combobox->setCurrentIndex(2);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("foo2"));
    // remove item1, index should change
    strListModel->removeRow(1);
    QCOMPARE(m_combobox->currentIndex(), 1);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("foo2"));
    // remove current item
    strListModel->removeRow(1);
    QCOMPARE(m_combobox->currentIndex(), -1);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), qtTrId("qtn_comm_popup_default"));
    // check that after removing items everything looks ok
    QCOMPARE(m_combobox->count(), 2);
    m_combobox->setCurrentIndex(1);
    QCOMPARE(m_combobox->currentIndex(), 1);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("foo3"));
    // add one item
    strListModel->insertRow(0);
    strListModel->setData(strListModel->index(0, 0), QString("fooX"), Qt::DisplayRole);
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("foo3"));

    // change current item text
    strListModel->setData(strListModel->index(2, 0), QString("lamb"), Qt::DisplayRole);
    QCOMPARE(m_combobox->currentIndex(), 2);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), QString("lamb"));
}

void Ut_MComboBox::testStringListModelSetStringList()
{
    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();
    QSignalSpy spy(m_combobox, SIGNAL(currentIndexChanged(int)));

    QStringListModel *strListModel = new QStringListModel();
    strListModel->setStringList(QStringList() << "foo0" << "foo1" << "foo2" << "foo3");
    m_combobox->setItemModel(strListModel);

    // select foo2
    m_combobox->setCurrentIndex(2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst()[0], QVariant(2));
    // change string list completely
    strListModel->setStringList(QStringList() << "bar0" << "bar1" << "bar2" << "bar3");
    QCOMPARE(m_combobox->currentIndex(), -1);
    QCOMPARE(viewPrivate->button->subtitleWidget()->text(), qtTrId("qtn_comm_popup_default"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst()[0], QVariant(-1));
}

void Ut_MComboBox::testActivatedSignal()
{
    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();
    m_combobox->addItems(QStringList() << "foo0" << "foo1" << "foo2" << "foo3");
    m_combobox->click();

    QSignalSpy activatedIntSpy(m_combobox, SIGNAL(activated(int)));
    QSignalSpy activatedQStringSpy(m_combobox, SIGNAL(activated(QString)));
    QSignalSpy currentItemChangedIntSpy(m_combobox, SIGNAL(currentIndexChanged(int)));
    QSignalSpy currentItemChangedQStringSpy(m_combobox, SIGNAL(currentIndexChanged(QString)));

    // click first item
    viewPrivate->popuplist->click(m_combobox->itemModel()->index(1, 0));

    QCOMPARE(activatedIntSpy.count(), 1);
    QCOMPARE(activatedIntSpy.last()[0].toInt(), 1);
    QCOMPARE(activatedQStringSpy.count(), 1);
    QCOMPARE(activatedQStringSpy.last()[0].toString(), QString("foo1"));

    QCOMPARE(currentItemChangedIntSpy.count(), 1);
    QCOMPARE(currentItemChangedIntSpy.last()[0].toInt(), 1);
    QCOMPARE(currentItemChangedQStringSpy.count(), 1);
    QCOMPARE(currentItemChangedQStringSpy.last()[0].toString(), QString("foo1"));

    // click first item again, currentItemChanged signal should not be emited
    viewPrivate->popuplist->click(m_combobox->itemModel()->index(1, 0));

    QCOMPARE(activatedIntSpy.count(), 2);
    QCOMPARE(activatedIntSpy.last()[0].toInt(), 1);
    QCOMPARE(activatedQStringSpy.count(), 2);
    QCOMPARE(activatedQStringSpy.last()[0].toString(), QString("foo1"));

    QCOMPARE(currentItemChangedIntSpy.count(), 1);
    QCOMPARE(currentItemChangedQStringSpy.count(), 1);

    // click second item
    viewPrivate->popuplist->click(m_combobox->itemModel()->index(2, 0));

    QCOMPARE(activatedIntSpy.count(), 3);
    QCOMPARE(activatedIntSpy.last()[0].toInt(), 2);
    QCOMPARE(activatedQStringSpy.count(), 3);
    QCOMPARE(activatedQStringSpy.last()[0].toString(), QString("foo2"));

    QCOMPARE(currentItemChangedIntSpy.count(), 2);
    QCOMPARE(currentItemChangedIntSpy.last()[0].toInt(), 2);
    QCOMPARE(currentItemChangedQStringSpy.count(), 2);
    QCOMPARE(currentItemChangedQStringSpy.last()[0].toString(), QString("foo2"));
}

void Ut_MComboBox::testSetCurrentIndex()
{
    win->scene()->addItem(m_combobox);

    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();

    QSignalSpy spy(m_combobox, SIGNAL(currentIndexChanged(int)));

    m_combobox->addItem("text1");
    m_combobox->setCurrentIndex(0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst()[0].toInt(), 0);

    m_combobox->addItem("text2");
    m_combobox->click();
    viewPrivate->popuplist->click(m_combobox->itemModel()->index(1, 0));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst()[0].toInt(), 1);

    win->scene()->removeItem(m_combobox);
}

void Ut_MComboBox::testProgressIndicator()
{
    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();
    MLayout* comboboxButtonLayout = static_cast<MLayout*>(viewPrivate->button->layout());

    QCOMPARE(m_combobox->isProgressIndicatorVisible(), false);
    QCOMPARE(comboboxButtonLayout->policy(), viewPrivate->button->layoutPolicy(MComboBoxButton::TitleShown));

    m_combobox->setProgressIndicatorVisible(true);
    QCOMPARE(m_combobox->isProgressIndicatorVisible(), true);
    QCOMPARE(comboboxButtonLayout->policy(), viewPrivate->button->layoutPolicy(MComboBoxButton::TitleShown|MComboBoxButton::ProgressShown));

    m_combobox->setProgressIndicatorVisible(false);
    QCOMPARE(m_combobox->isProgressIndicatorVisible(), false);
    QCOMPARE(comboboxButtonLayout->policy(), viewPrivate->button->layoutPolicy(MComboBoxButton::TitleShown));
}

void Ut_MComboBox::testProgressIndicator2()
{
    MComboBoxView *view = (MComboBoxView *)m_combobox->view();
    MComboBoxViewPrivate *viewPrivate = view->d_func();
    MLayout* comboboxButtonLayout = static_cast<MLayout*>(viewPrivate->button->layout());

    QCOMPARE(m_combobox->isProgressIndicatorVisible(), false);
    QCOMPARE(comboboxButtonLayout->policy(), viewPrivate->button->layoutPolicy(MComboBoxButton::TitleShown));

    m_combobox->showProgressIndicator();
    QCOMPARE(m_combobox->isProgressIndicatorVisible(), true);
    QCOMPARE(comboboxButtonLayout->policy(),
             viewPrivate->button->layoutPolicy(MComboBoxButton::TitleShown|MComboBoxButton::ProgressShown));

    m_combobox->hideProgressIndicator();
    QCOMPARE(m_combobox->isProgressIndicatorVisible(), false);
    QCOMPARE(comboboxButtonLayout->policy(), viewPrivate->button->layoutPolicy(MComboBoxButton::TitleShown));
}

QTEST_APPLESS_MAIN(Ut_MComboBox)

