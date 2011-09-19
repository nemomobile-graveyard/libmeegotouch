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

#include <mapplication.h>

#include <QEventLoop>
#include <QStandardItemModel>
#include <QGraphicsItem>

#include <mlist.h>
#include <mlistview.h>
#include <mlistview_p.h>
#include <mpannableviewport.h>

#include "ut_mlistview.h"

MApplication *app(NULL);

Ut_MListView::Ut_MListView() :
        m_subject(0)
{
}

void Ut_MListView::initTestCase()
{
    static char *app_name[1] = { (char *) "./ut_mlistview" };
    static int argc = 0;
    app = new MApplication(argc, app_name);

    MPannableViewport *viewport = new MPannableViewport();

    viewport->resize(50, 100);

    MList *controller = new MList(viewport);
    m_subject = new MListView(controller);

    MListModel *model = new MListModel();
    model->setShowGroups(true);
    QStandardItemModel *itemModel = new QStandardItemModel(4, 2);

    for (int row = 0; row < itemModel->rowCount(); ++row) {
        for (int column = 0; column < itemModel->columnCount(); ++column) {
            QStandardItem *item = new QStandardItem(
            		QString("row %0, column %1").arg(row).arg(column));
            itemModel->setItem(row, column, item);
        }
    }

    QItemSelectionModel *selectionModel = new QItemSelectionModel(itemModel);
    controller->setSelectionModel(selectionModel);
    controller->setItemModel(itemModel);
    model->setSelectionModel(selectionModel);

    m_dummyWidget = new MDummyTestWidget();

    connect(m_dummyWidget, SIGNAL(itemClickSignal()),
    		m_subject, SLOT(itemClick()));

    model->setItemModel(itemModel);
    m_subject->setModel(model);
    m_subject->init();
}

void Ut_MListView::cleanupTestCase()
{
    disconnect(m_dummyWidget, SIGNAL(itemClickSignal()),
    		m_subject, SLOT(itemClick()));

    delete m_dummyWidget;
    m_dummyWidget = 0;

    //to prevent crash in MBasicListItemDeletionAnimationPrivate::resetTargetWidgets()
    //called from the inside of m_subject destructor where itemDeletionAnimation
    //destructor is called anyway

    m_subject->d_func()->itemDeletionAnimation;
    m_subject->d_func()->itemDeletionAnimation = 0;

    delete m_subject;
    m_subject = 0;
}

void Ut_MListView::testItemClick()
{
    m_dummyWidget->emitClickItem();
}

void Ut_MListView::testLongTap()
{
    QPointF point(5, 5);
    m_subject->longTap(point);
}

void Ut_MListView::testPrivateMovingDetectionTimerTimeout()
{
    MListViewPrivate *d = m_subject->d_func();

    QEventLoop loop;
    connect( &d->movingDetectorTimer, SIGNAL( timeout() ),
            &loop, SLOT( quit() ) );

	d->viewportRectChanged(QRectF(5, 5, 100, 100));

	loop.exec();

	QVERIFY2(!d->moving, "List view expected to have moving = false");
}

void Ut_MListView::testPrivateCellClicked()
{
	MListViewPrivate *d = m_subject->d_func();

	MWidget *dummyWidget = new MWidget();
	dummyWidget->setPos(0,0);
	d->cellClicked(dummyWidget);

	delete dummyWidget;
}

void Ut_MListView::testPrivateAppendTargetsToDeleteAnimation()
{
    MListViewPrivate *d = m_subject->d_func();

    d->itemHeight = 20;

    d->MListViewPrivate::createVisibleItems();

    QVERIFY2(d->itemDeletionAnimation, "itemDeletionAnimation isn't initialized!");
    d->MListViewPrivate::appendTargetsToDeleteAnimation(1, 2, 0, 3);
}

void Ut_MListView::testPrivateResetAnimatedWidgets()
{
    MListViewPrivate *d = m_subject->d_func();
    int end = d->model->rowCount();

    d->itemHeight = 20;
    d->createVisibleItems();

    QVERIFY2(d->itemDeletionAnimation, "itemDeletionAnimation isn't initialized!");
    d->appendTargetsToDeleteAnimation(0, end, 1, 3);

    QVERIFY2(!d->animatingItems.isEmpty(), "animatingItems expected to be not empty");

    d->resetAnimatedWidgets();

    QVERIFY2(d->animatingItems.isEmpty(), "animatingItems expected to be empty");
}

void Ut_MListView::testPrivateUpdateItemConnections()
{
	MListViewPrivate *d = m_subject->d_func();
	d->updateItemConnections();
}

void Ut_MListView::testPrivateCreateVisibleItems()
{
	MListViewPrivate *d = m_subject->d_func();
	d->MListViewPrivate::createVisibleItems();

	QVERIFY2(d->visibleItems.count() > 0, "Visible items not created");
}

void Ut_MListView::testPrivateReplaceItem()
{
	MListViewPrivate *d = m_subject->d_func();

	MWidget* oldWidget = d->findCellAtRow(0);

	MWidget* newWidget = new MLabel("new widget");
	d->replaceItem(oldWidget, newWidget);
	QCOMPARE(d->findCellAtRow(0), newWidget);
}

void Ut_MListView::testPrivateLocateStartScrollToPosition()
{
	MListViewPrivate *d = m_subject->d_func();

	QPointF position = d->pannableViewport->position();

	//y greater than pannableViewport position
	d->locateStartScrollToPosition(position + QPointF(0, 1));

	d->locateStartScrollToPosition(position);
}

void Ut_MListView::testPrivateScrollToPos()
{
	MListViewPrivate *d = m_subject->d_func();

	QPointF pos(0, 20);

	QEventLoop loop;
	connect(d->scrollToAnimation, SIGNAL( finished() ), &loop, SLOT( quit() ));
	d->scrollToPos(pos, MList::Animated);

	QCOMPARE(d->scrollToAnimation->state(), QAbstractAnimation::Running);

	loop.exec();

	QCOMPARE(d->scrollToAnimation->state(), QAbstractAnimation::Stopped);

	d->scrollToPos(pos, MList::NonAnimated);
}

void Ut_MListView::testPrivateUpdateRecyclerMaxItemsCount()
{
	MListViewPrivate *d = m_subject->d_func();

	//needed for update list height so condition in updateRecyclerMaxItemsCount() will be true
	d->itemHeight = 20;
	d->updateRecyclerMaxItemsCount();
}

void Ut_MListView::testPrivateLocateLastVisibleIndexInRowAt()
{
	MListViewPrivate *d = m_subject->d_func();

	d->MListViewPrivate::locateLastVisibleIndexInRowAt(0);
}

void Ut_MListView::testPrivateUpdateItemSize()
{
	MListViewPrivate *d = m_subject->d_func();

	d->MListViewPrivate::updateItemSize();
}

void Ut_MListView::testIndexToFlatRowAssert()
{
    QVERIFY(m_subject->model()->showGroups());

    MGroupHeaderListViewPrivate *d = static_cast<MGroupHeaderListViewPrivate*>(m_subject->d_func());

    QVector<int> oldHeaderRows = d->headersRows;
    d->headersRows.clear();
    QVERIFY(d->headersRows.isEmpty());

    int result = d->indexToFlatRow(d->model->index(0, 0));
    QCOMPARE(result, -1);

    d->headersRows.append(0);
    d->headersRows.append(15);

    result = d->indexToFlatRow(d->model->index(2, 0));
    d->headersRows = oldHeaderRows;
    QCOMPARE(result, -1);
}

void MDummyTestWidget::emitClickItem()
{
    emit itemClickSignal();
}

QTEST_APPLESS_MAIN(Ut_MListView)
