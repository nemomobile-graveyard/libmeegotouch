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

#include "ut_mscrollchain.h"
#include "mabstractscroller.h"
#include "mscrollchain.h"

#include "scroller.h"
#include "scrollablewidget.h"

#include <MDeviceProfile>
#include <QGraphicsWidget>
#include <QGraphicsScene>


Q_DECLARE_METATYPE(QList<int>);

void Ut_MScrollChain::initTestCase()
{
    qRegisterMetaType< QList<int> >("QList<int>");

    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mscrollchain" };
    MApplication::setLoadMInputContext(false);

    app.reset(new MApplication(argc, app_name));

    scene.reset(new QGraphicsScene(QRectF(QPointF(), MDeviceProfile::instance()->resolution())));

    rootElement = new QGraphicsWidget;
    rootElement->setFlag(QGraphicsItem::ItemHasNoContents, true);
    scene->addItem(rootElement);

    MScrollChain::registerScroller<ScrollableWidget>(QSharedPointer<Scroller>(new Scroller));
}

void Ut_MScrollChain::cleanupTestCase()
{
    app->quit();
}

void Ut_MScrollChain::init()
{
    Scroller::maximumOffset = QPoint(10000, 10000);
}

void Ut_MScrollChain::cleanup()
{
    subject.reset(0);
}

void Ut_MScrollChain::testPointScroll_data()
{
    // In this test a point at (0,0) of 200x200 child widget is moved
    // by the amount of 'contentsOffset'. The parent scroller widget
    // keeps this point in its clipping area which is 100x100.

    QTest::addColumn<QPoint>("contentsOffset");
    QTest::addColumn<QPoint>("resultingPosition");
    QTest::newRow("no offset") << QPoint() << QPoint();
    QTest::newRow("positive offset") << QPoint(10, 10) << QPoint(10, 10);
    QTest::newRow("positive offset, restricted") << QPoint(100, 100) << QPoint(99, 99);
    QTest::newRow("negative offset, restricted") << QPoint(-10, -10) << QPoint();
}

void Ut_MScrollChain::testPointScroll()
{
    QFETCH(QPoint, contentsOffset);
    QFETCH(QPoint, resultingPosition);

    ScrollableWidget scrollableWidget;
    scene->addItem(&scrollableWidget);

    QGraphicsWidget *child = new QGraphicsWidget;
    scrollableWidget.setContentItem(child);

    const QSize parentSize(100, 100);
    const QSize childSize(200, 200);
    scrollableWidget.resize(parentSize);
    child->resize(childSize);

    // Create the chain
    subject.reset(new MScrollChain(child));

    QVERIFY(qFuzzyCompare(child->pos().x(), static_cast<qreal>(0.0)));
    QVERIFY(qFuzzyCompare(child->pos().y(), static_cast<qreal>(0.0)));

    const QRect pointRect(0, 0, 1, 1);
    const QPoint currentPos = subject->root()->mapFromItem(child, 0, 0).toPoint();

    subject->addBottomUpScroll(QRect(currentPos + contentsOffset, pointRect.size()),
                               QPoint());
    subject->applyScrolling();

    qDebug() << "child->pos()" << child->pos() << ", expected " << resultingPosition;
    QVERIFY(qFuzzyCompare(child->pos().x(), resultingPosition.x()));
    QVERIFY(qFuzzyCompare(child->pos().y(), resultingPosition.y()));
}

void Ut_MScrollChain::testRestore()
{
    const int numWidgets = 5;
    const int containerHeight = 100;

    QList<ScrollableWidget *> scrollables;
    for (int i = 0; i < numWidgets; ++i) {
        ScrollableWidget *newScrollable = new ScrollableWidget;
        newScrollable->resize(100, containerHeight);
        if (!scrollables.isEmpty()) {
            scrollables.back()->setContentItem(newScrollable);
        }
        scrollables.append(newScrollable);
    }

    QGraphicsWidget *leaf = new QGraphicsWidget;
    leaf->resize(1,1);
    scrollables.back()->setContentItem(leaf);

    // Add everything to scene
    scene->addItem(scrollables.front());

    // Restrict scrolling step to one so every widget gets a chance to scroll.
    const qreal verticalStepSize = 1.0;
    Scroller::maximumOffset = QPoint(0, verticalStepSize);
    const QPoint scrollOffset(0, containerHeight - 1); // move widget to bottom of container

    subject.reset(new MScrollChain(leaf));

    addBottomUpScroll(leaf, scrollOffset);
    subject->applyScrolling();

    foreach (const ScrollableWidget *scrollable, scrollables) {
        QVERIFY(qFuzzyCompare(scrollable->contentItem()->pos().y(), verticalStepSize));
    }

    qDebug() << "restoring";
    subject->restore();

    foreach (const ScrollableWidget *scrollable, scrollables) {
        QVERIFY(qFuzzyCompare(scrollable->contentItem()->pos().y(), static_cast<qreal>(0.0)));
    }
}

void Ut_MScrollChain::testMultiplePointScrolls_data()
{
    // This test checks that scroll chain gives correct information
    // about the current widget offset in subsequent scroll requests.

    // how much the test widget can scroll to each direction
    QTest::addColumn<int>("maxAbsoluteOffset");

    // list defines increments to target point rectangle sitting at (0, 0)
    QTest::addColumn< QList<int> >("increments");
    QTest::addColumn<int>("actualIncrement");

    QTest::newRow("zero scrolls") << 10 << (QList<int>() << 0 << 0 << 0) << 0;
    QTest::newRow("scrolling 0, 1, 0") << 10 << (QList<int>() << 0 << 1 << 0) << 1;
    QTest::newRow("scrolling 1, 1, 1") << 3 << (QList<int>() << 1 << 1 << 1) << 3;

    // Viewport clipping prevents scrolling in these
    QTest::newRow("viewport limit 1") << 10 << (QList<int>() << -1 << 0 << 0) << 0;
    QTest::newRow("viewport limit 2") << 10 << (QList<int>() << 1 << 1 << -3) << 0;
    QTest::newRow("viewport limit 3") << 10 << (QList<int>() << -10 << 11) << 1;

    // Test maximum value can be honored
    QTest::newRow("max scroll 2") << 10 << (QList<int>() << 10 << 10) << 10;
    QTest::newRow("max scroll 3") << 10 << (QList<int>() << 3 << 3 << 3 << 3) << 10;
}

void Ut_MScrollChain::testMultiplePointScrolls()
{
    QFETCH(int, maxAbsoluteOffset);
    QFETCH(QList<int>, increments);
    QFETCH(int, actualIncrement);

    ScrollableWidget scrollable;
    QGraphicsWidget child;
    scrollable.setContentItem(&child);
    child.resize(1, 1);

    subject.reset(new MScrollChain(&child));

    // big enough so it doesn't get to our way
    scrollable.resize(100, 100);

    Scroller::maximumOffset = QPoint(0, maxAbsoluteOffset);

    QRect target(0, 0, 1, 1);

    foreach (int increment, increments) {
        target.moveTop(target.top() + increment);
        subject->addBottomUpScroll(target, QPoint());
    }
    subject->applyScrolling();

    QCOMPARE(child.pos().toPoint().y(), actualIncrement);
}

void Ut_MScrollChain::addBottomUpScroll(QGraphicsWidget *widget, const QPoint &offset)
{
    const QRect widgetRect = subject->root()->mapRectFromItem(widget, widget->rect()).toRect();
    const QRect targetRect(widgetRect.topLeft() + offset, widgetRect.size());
    subject->addBottomUpScroll(targetRect, QPoint());
}


QTEST_APPLESS_MAIN(Ut_MScrollChain)
