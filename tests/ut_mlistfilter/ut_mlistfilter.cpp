/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mlistfilter_p.h"
#include "mpannableviewport.h"
#include "mtextedit.h"
#include "msortfilterproxymodel.h"
#include "ut_mlistfilter.h"

Q_DECLARE_METATYPE(MTheme::ViewType);

MApplication *app;

void Ut_MListFilter::init()
{
    list = new MList();
}

void Ut_MListFilter::cleanup()
{
    delete list;
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
    QVERIFY(list->filtering());
}

void Ut_MListFilter::testEnabling()
{
    QCOMPARE(list->filtering()->enabled(), false);
    list->filtering()->setEnabled(true);
    QCOMPARE(list->filtering()->enabled(), true);
}

void Ut_MListFilter::testEditor()
{
    QVERIFY(list->filtering()->editor() != NULL);
    QCOMPARE(list->filtering()->editor()->inputMethodAutoCapitalizationEnabled(), false);
    QCOMPARE(list->filtering()->editor()->inputMethodCorrectionEnabled(), false);
    QCOMPARE(list->filtering()->editor()->inputMethodPredictionEnabled(), false);
}

void Ut_MListFilter::testProxy()
{
    QStringListModel *listModel = new QStringListModel();
    list->setItemModel(listModel);

    QVERIFY(list->filtering()->proxy() != NULL);
    QVERIFY(list->filtering()->proxy()->sourceModel() == NULL);
    list->filtering()->setEnabled(true);
    QCOMPARE(list->filtering()->proxy()->sourceModel(), listModel);
}

void Ut_MListFilter::testFilterRole()
{
    QCOMPARE(list->filtering()->filterRole(), (int)Qt::DisplayRole);
    list->filtering()->setFilterRole(Qt::DecorationRole);
    QCOMPARE(list->filtering()->filterRole(), (int)Qt::DecorationRole);
}

void Ut_MListFilter::testKeyEvent()
{
    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::ControlModifier, QString("X"));

    list->filtering()->keyPressEvent(event);
    QCOMPARE(list->filtering()->editor()->text().length(), 0);
    list->filtering()->setEnabled(true);
    list->filtering()->keyPressEvent(event);
    QCOMPARE(list->filtering()->editor()->text().length(), 1);

    delete event;
}

void Ut_MListFilter::testEditorTextChange()
{
    QSignalSpy spy(list->filtering()->editor(), SIGNAL(textChanged()));
    QCOMPARE(spy.count(), 0);

    list->filtering()->editor()->setText("X");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(list->filtering()->proxy()->filterRegExp(), QRegExp(".*X.*", Qt::CaseInsensitive, QRegExp::RegExp));
}

void Ut_MListFilter::testItemModelUpdate()
{
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
}

void Ut_MListFilter::testUpdatePannableViewport()
{
    list->filtering()->d_ptr->updatePannableViewport();
    QVERIFY(list->filtering()->d_ptr->cachedPannableViewport);

    QSignalSpy spy(list->filtering()->d_ptr->cachedPannableViewport->physics(), SIGNAL(pointerPressed()));
    QSignalSpy spy2(list->filtering()->d_ptr->cachedPannableViewport->physics(), SIGNAL(pointerReleased()));
    QSignalSpy spy3(list->filtering()->d_ptr->cachedPannableViewport->physics(), SIGNAL(positionChanged(QPointF&)));

    list->filtering()->d_ptr->cachedPannableViewport->physics()->pointerPress(QPointF(0, 0));

    QCOMPARE(spy.count(), 1);

    int positionChangeCount = 10;
    QPointF position(0, 0);

    for (int i = 0; i < positionChangeCount; i++)
    {
        list->filtering()->d_ptr->cachedPannableViewport->physics()->setPosition(position += QPointF(i, i));
    }

    list->filtering()->d_ptr->cachedPannableViewport->physics()->pointerRelease();
    QCOMPARE(spy2.count(), 1);

    //ensure there is no position update after pointer release
    list->filtering()->d_ptr->cachedPannableViewport->physics()->setPosition(position += QPointF(0, 1));

    QCOMPARE(spy3.count(), 10);
}

void Ut_MListFilter::testStartEditorPullDown()
{
    list->filtering()->d_ptr->updatePannableViewport();
    list->filtering()->d_ptr->startEditorPullDown();
}

void Ut_MListFilter::testCheckEditorPullDistance()
{
    list->filtering()->d_ptr->updatePannableViewport();
    QPointF point(0,-10);
    list->filtering()->d_ptr->checkEditorPullDistance(point);

    QPointF point2(0,100);
    //distance will be negative and pulling will be stopped
    list->filtering()->d_ptr->checkEditorPullDistance(point2);

    QVERIFY(list->filtering()->d_ptr->pullStarted);
}

void Ut_MListFilter::testStopEditorPullDown()
{
    list->filtering()->d_ptr->stopEditorPullDown();
    QCOMPARE(list->filtering()->d_ptr->pullStarted, false);
}

void Ut_MListFilter::testFilterMode()
{
    MListFilter::FilterMode filterMode = MListFilter::FilterAsSubstring;
    list->filtering()->setFilterMode(filterMode);

    QCOMPARE(list->filtering()->filterMode(), filterMode);
}

QTEST_APPLESS_MAIN(Ut_MListFilter)
