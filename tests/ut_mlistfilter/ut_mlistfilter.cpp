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

#include <Qt>
#include <QTest>
#include <QStringListModel>
#include <QKeyEvent>

#include "mapplication.h"
#include "mlist.h"
#include "mlistfilter.h"
#include "mtextedit.h"
#include "msortfilterproxymodel.h"
#include "ut_mlistfilter.h"

Q_DECLARE_METATYPE(MTheme::ViewType);

MApplication *app;

void Ut_MListFilter::init()
{
}

void Ut_MListFilter::cleanup()
{
}

void Ut_MListFilter::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mlistfilter" };
    app = new MApplication(argc, app_name);
}

void Ut_MListFilter::cleanupTestCase()
{
    delete app;
}

void Ut_MListFilter::testCreation()
{
    MList *list = new MList();

    QVERIFY(list->filtering() != NULL);

    delete list;
}

void Ut_MListFilter::testEnabling()
{
    MList *list = new MList();

    QCOMPARE(list->filtering()->enabled(), false);
    list->filtering()->setEnabled(true);
    QCOMPARE(list->filtering()->enabled(), true);

    delete list;
}

void Ut_MListFilter::testEditor()
{
    MList *list = new MList();

    QVERIFY(list->filtering()->editor() != NULL);
    QCOMPARE(list->filtering()->editor()->inputMethodAutoCapitalizationEnabled(), false);
    QCOMPARE(list->filtering()->editor()->inputMethodCorrectionEnabled(), false);
    QCOMPARE(list->filtering()->editor()->inputMethodPredictionEnabled(), false);

    delete list;
}

void Ut_MListFilter::testProxy()
{
    MList *list = new MList();
    QStringListModel *listModel = new QStringListModel();
    list->setItemModel(listModel);

    QVERIFY(list->filtering()->proxy() != NULL);
    QVERIFY(list->filtering()->proxy()->sourceModel() == NULL);
    list->filtering()->setEnabled(true);
    QCOMPARE(list->filtering()->proxy()->sourceModel(), listModel);

    delete list;
}

void Ut_MListFilter::testFilterRole()
{
    MList *list = new MList();

    QCOMPARE(list->filtering()->filterRole(), (int)Qt::DisplayRole);
    list->filtering()->setFilterRole(Qt::DecorationRole);
    QCOMPARE(list->filtering()->filterRole(), (int)Qt::DecorationRole);

    delete list;
}

void Ut_MListFilter::testKeyEvent()
{
    MList *list = new MList();
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier, QString("X"));

    list->filtering()->keyPressEvent(event);
    QCOMPARE(list->filtering()->editor()->text().length(), 0);
    list->filtering()->setEnabled(true);
    list->filtering()->keyPressEvent(event);
    QCOMPARE(list->filtering()->editor()->text().length(), 1);

    delete event;
    delete list;
}

void Ut_MListFilter::testEditorTextChange()
{
    MList *list = new MList();
    QSignalSpy spy(list->filtering()->editor(), SIGNAL(textChanged()));
    QCOMPARE(spy.count(), 0);

    list->filtering()->editor()->setText("X");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(list->filtering()->proxy()->filterRegExp(), QRegExp(".*X.*", Qt::CaseInsensitive, QRegExp::RegExp));

    delete list;
}

void Ut_MListFilter::testItemModelUpdate()
{
    MList *list = new MList();

    QVERIFY(list->itemModel() == NULL);
    QVERIFY(list->filtering()->proxy() != NULL);
    QVERIFY(list->filtering()->proxy()->sourceModel() == NULL);

    QStringListModel *listModel = new QStringListModel();
    list->setItemModel(listModel);
    QCOMPARE(list->itemModel(), listModel);
    QVERIFY(list->filtering()->proxy() != NULL);
    QVERIFY(list->filtering()->proxy()->sourceModel() == NULL);

    list->filtering()->setEnabled(true);
    QCOMPARE(list->itemModel(), list->filtering()->proxy());
    QCOMPARE(list->filtering()->proxy()->sourceModel(), listModel);

    QStringListModel *updatedListModel = new QStringListModel();
    list->setItemModel(updatedListModel);
    QCOMPARE(list->itemModel(), list->filtering()->proxy());
    QCOMPARE(list->filtering()->proxy()->sourceModel(), updatedListModel);

    list->filtering()->setEnabled(false);
    QCOMPARE(list->itemModel(), updatedListModel);

    delete list;
}

QTEST_APPLESS_MAIN(Ut_MListFilter)
