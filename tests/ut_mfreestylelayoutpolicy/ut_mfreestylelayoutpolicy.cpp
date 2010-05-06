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

#include "ut_mfreestylelayoutpolicy.h"

#include <mapplication.h>
#include <mlayout.h>
#include <mfreestylelayoutpolicy.h>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QSizePolicy>
#include <QSizePolicy>
/**
    MockLayoutItem objects are layed out with the layout policy to test.
    MockLayoutItems are then investigated for the correct geometry values
    assigned by the test layout.
*/
class MockLayoutItem : public QGraphicsLayoutItem
{
protected:
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {
        Q_UNUSED(which);
        Q_UNUSED(constraint);
        switch (which) {
        case Qt::MinimumSize:
            return QSizeF(1, 1);
        case Qt::PreferredSize:
            return QSizeF(7, 7);
        case Qt::MaximumSize:
            return QSizeF(19, 19);
        default:
            return QSizeF(11, 11);
        }
    }
};


Ut_MFreestyleLayoutPolicy::Ut_MFreestyleLayoutPolicy()
    : m_mockLayout(0)
    , m_policy(0)
    , m_mockItem100(0)
    , m_mockItem200(0)
    , m_mockItem300(0)
    , m_form(0)
{ }

Ut_MFreestyleLayoutPolicy::~Ut_MFreestyleLayoutPolicy()
{ }

MApplication *app;
void Ut_MFreestyleLayoutPolicy::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mfreestylelayoutpolicy" };
    app = new MApplication(argc, argv);
}

void Ut_MFreestyleLayoutPolicy::cleanupTestCase()
{
    delete app;
}

void Ut_MFreestyleLayoutPolicy::init()
{
    Q_ASSERT(0 == m_mockLayout);
    m_form = new QGraphicsWidget;
    m_mockLayout = new MLayout(m_form);
    m_mockLayout->setContentsMargins(0, 0, 0, 0);
    Q_ASSERT(0 == m_policy);
    m_policy = new MFreestyleLayoutPolicy(m_mockLayout);
    m_policy->setSpacing(0);

    // set up some mock items
    Q_ASSERT(0 == m_mockItem100);
    m_mockItem100 = new MockLayoutItem;
    m_mockItem100->setPreferredSize(100, 100);

    Q_ASSERT(0 == m_mockItem200);
    m_mockItem200 = new MockLayoutItem;
    m_mockItem200->setPreferredSize(200, 200);

    Q_ASSERT(0 == m_mockItem300);
    m_mockItem300 = new MockLayoutItem;
    m_mockItem300->setPreferredSize(300, 300);

    m_mockLayout->setPolicy(m_policy);
    m_mockLayout->setAnimation(0); // turn off animation
}

void Ut_MFreestyleLayoutPolicy::cleanup()
{
    Q_ASSERT(0 != m_mockItem100);
    m_mockLayout->removeItem(m_mockItem100);
    m_mockItem100 = 0;

    Q_ASSERT(0 != m_mockItem200);
    m_mockLayout->removeItem(m_mockItem200);
    m_mockItem200 = 0;

    Q_ASSERT(0 != m_mockItem300);
    m_mockLayout->removeItem(m_mockItem300);
    m_mockItem300 = 0;

    Q_ASSERT(0 != m_policy);
    delete m_policy;
    m_policy = 0;

    //FIXME Without this processEvents, we get a segmentation fault as a deleted object gets
    //an event sent to it.  This needs to be fixed
    qApp->processEvents();
    Q_ASSERT(0 != m_mockLayout);
    delete m_mockLayout;
    m_mockLayout = 0;
}

void Ut_MFreestyleLayoutPolicy::testOneItemLayout()
{
    //Test size of layout with no items
    QCOMPARE(m_mockLayout->minimumSize(), QSizeF(0, 0));
    QCOMPARE(m_mockLayout->preferredSize(), QSizeF(0, 0));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    m_form->setGeometry(QRectF(0, 0, 100, 100));

    // Add an item
    m_policy->addItem(m_mockItem100);
    qApp->processEvents(); //for change to trigger relayout
    // item moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0, 0),
             m_mockItem100->preferredSize()));

    // check layout size hints
    //QCOMPARE(m_mockLayout->minimumSize(), m_mockItem100->minimumSize());
    QCOMPARE(m_mockLayout->minimumSize(), m_mockItem100->preferredSize());
    QCOMPARE(m_mockLayout->preferredSize(), m_mockItem100->preferredSize());
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

    //Now remove item.  Layout sizehints should go back to how they were
    //m_mockLayout->removeItem(m_mockItem100);
    m_policy->removeItem(m_mockItem100);
    QCOMPARE(m_mockLayout->minimumSize(), QSizeF(0, 0));
    QCOMPARE(m_mockLayout->preferredSize(), QSizeF(0, 0));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}
void Ut_MFreestyleLayoutPolicy::testThreeItemLayout()
{
    //Test size of layout with no items
    QCOMPARE(m_mockLayout->minimumSize(), QSizeF(0, 0));
    QCOMPARE(m_mockLayout->preferredSize(), QSizeF(0, 0));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    m_form->setGeometry(QRectF(0, 0, 300, 100));

    // Add an item
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);
    m_policy->addItem(m_mockItem300);
    qApp->processEvents(); //for change to trigger relayout
    // item moved to the correct place?
    /*    QCOMPARE(m_mockItem100->geometry(), QRectF(0,200,100,100));
        QCOMPARE(m_mockItem200->geometry(), QRectF(0,300,200,200));
        QCOMPARE(m_mockItem300->geometry(), QRectF(0,500,300,300));
        QCOMPARE(m_mockLayout->geometry(), QRectF(0,0,300,800));*/
}

void Ut_MFreestyleLayoutPolicy::testThreeItemLayoutWithGivenGeometry()
{
    //Test size of layout with no items
    QCOMPARE(m_mockLayout->minimumSize(), QSizeF(0, 0));
    QCOMPARE(m_mockLayout->preferredSize(), QSizeF(0, 0));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    m_form->setGeometry(QRectF(10, 10, 300, 100));

    // Add an item
    m_policy->addItemAtGeometry(m_mockItem100, QRectF(0, 0, 100, 100));
    m_policy->addItemAtGeometry(m_mockItem200, QRectF(100, 0, 200, 200));
    m_policy->addItemAtGeometry(m_mockItem300, QRectF(0, 200, 300, 300));
    qApp->processEvents(); //for change to trigger relayout
    // item moved to the correct place?
    /*    QCOMPARE(m_mockItem100->geometry(), QRectF(0,0,100,100));
        QCOMPARE(m_mockItem200->geometry(), QRectF(100,0,200,200));
        QCOMPARE(m_mockItem300->geometry(), QRectF(0,200,300,300));
        QCOMPARE(m_mockLayout->geometry(), QRectF(0,0,300,500));*/
}

QTEST_APPLESS_MAIN(Ut_MFreestyleLayoutPolicy)
