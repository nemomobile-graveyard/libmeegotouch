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

#include <MDeviceProfile>
#include <QGraphicsWidget>
#include <QGraphicsScene>

class ScrollableWidget : public QGraphicsWidget
{
public:
    ScrollableWidget(QGraphicsItem *parent = 0)
        : QGraphicsWidget(parent),
          mContentItem(0)
    {
        // Not really needed but illustrates the use of this widget.
        setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    }
    virtual ~ScrollableWidget() {}

    void setContentItem(QGraphicsItem *item)
    {
        delete mContentItem;
        mContentItem = item;
        item->setParentItem(this);
        item->setPos(QPointF());
    }

    const QGraphicsItem *contentItem() const
    {
        return mContentItem;
    }

    void scrollContents(const QPoint &offset)
    {
        if (mContentItem) {
            mContentItem->setPos(mContentItem->pos() + offset);
        }
    }

    void clearScroll()
    {
        if (mContentItem) {
            mContentItem->setPos(QPointF());
        }
    }

private:
    QGraphicsItem *mContentItem;
};



class Scroller : public MAbstractScroller
{
public:
    Scroller() {}
    virtual ~Scroller(){}

    virtual QPoint queryScrollingAmount(const QGraphicsWidget *widget,
                                        const QRect &targetRect,
                                        const QPoint &originPoint,
                                        const QPoint &)
    {
        const QRect boundaries(widget->boundingRect().toRect());

        QRect movedTargetRect(targetRect);
        moveRectInsideArea(boundaries, movedTargetRect);

        const QPoint offset(movedTargetRect.topLeft() - originPoint);
        const QPoint limitedOffset(qBound(-maximumOffset.x(), offset.x(), maximumOffset.x()),
                                   qBound(-maximumOffset.y(), offset.y(), maximumOffset.y()));

        return limitedOffset;
    }

    virtual void applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset)
    {
        ScrollableWidget *scrollableWidget = static_cast<ScrollableWidget *>(widget);
        scrollableWidget->scrollContents(contentsOffset);
    }

    virtual void restoreScrolling(QGraphicsWidget *widget)
    {
        ScrollableWidget *scrollableWidget = static_cast<ScrollableWidget *>(widget);
        scrollableWidget->clearScroll();
    }

    static QPoint maximumOffset; // absolute offset values
};

QPoint Scroller::maximumOffset;

void Ut_MScrollChain::initTestCase()
{
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

void Ut_MScrollChain::addBottomUpScroll(QGraphicsWidget *widget, const QPoint &offset)
{
    const QRect widgetRect = subject->root()->mapRectFromItem(widget, widget->rect()).toRect();
    const QRect targetRect(widgetRect.topLeft() + offset, widgetRect.size());
    subject->addBottomUpScroll(targetRect, QPoint());
}

QTEST_APPLESS_MAIN(Ut_MScrollChain)
