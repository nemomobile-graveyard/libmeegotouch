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

#include "ut_mgridlayoutpolicy.h"

#include <mapplication.h>
#include <mlayout.h>
#include <mgridlayoutpolicy.h>

#include <QtGui/QGraphicsGridLayout>
#include <QtGui/QGraphicsWidget>

/**
    MockLayoutItem objects are laid out with the layout policy to test.
    MockLayoutItems are then investigated for the correct geometry values
    assigned by the test layout.
*/
class MockLayoutItem : public QGraphicsLayoutItem
{
protected:
    QSizeF sizeHint(Qt::SizeHint , const QSizeF &) const {
        return QSize(-1, -1);
    }
};


Ut_MGridLayoutPolicy::Ut_MGridLayoutPolicy()
    : m_mockLayout(0)
    , m_policy(0)
    , m_mockItem100(0)
    , m_mockItem200(0)
    , m_mockItem300(0)
    , m_mockItem400(0)
    , m_form(0)
{ }

Ut_MGridLayoutPolicy::~Ut_MGridLayoutPolicy()
{ }

MApplication *app;
void Ut_MGridLayoutPolicy::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mgridlayoutpolicy" };
    app = new MApplication(argc, argv);
    app->setLayoutDirection(Qt::LeftToRight);
}

void Ut_MGridLayoutPolicy::cleanupTestCase()
{
    delete app;
}

void Ut_MGridLayoutPolicy::init()
{
    Q_ASSERT(0 == m_mockLayout);
    m_mockLayout = new MLayout();
    m_form = new QGraphicsWidget;
    m_form->setLayout(m_mockLayout);
    Q_ASSERT(0 == m_policy);
    m_policy = new MGridLayoutPolicy(m_mockLayout);
    m_policy->setSpacing(0);
    m_mockLayout->setContentsMargins(0, 0, 0, 0);

    // set up some mock items
    Q_ASSERT(0 == m_mockItem100);
    m_mockItem100 = new MockLayoutItem;
    m_mockItem100->setMinimumSize(100.0, 100.0);
    m_mockItem100->setPreferredSize(100.0, 100.0);
    m_mockItem100->setMaximumSize(100.0, 100.0);

    Q_ASSERT(0 == m_mockItem200);
    m_mockItem200 = new MockLayoutItem;
    m_mockItem200->setMinimumSize(200.0, 200.0);
    m_mockItem200->setPreferredSize(200.0, 200.0);
    m_mockItem200->setMaximumSize(200.0, 200.0);

    Q_ASSERT(0 == m_mockItem300);
    m_mockItem300 = new MockLayoutItem;
    m_mockItem300->setMinimumSize(300.0, 300.0);
    m_mockItem300->setPreferredSize(300.0, 300.0);
    m_mockItem300->setMaximumSize(300.0, 300.0);

    Q_ASSERT(0 == m_mockItem400);
    m_mockItem400 = new MockLayoutItem;
    m_mockItem400->setMinimumSize(400.0, 400.0);
    m_mockItem400->setPreferredSize(400.0, 400.0);
    m_mockItem400->setMaximumSize(400.0, 400.0);

    m_mockLayout->setPolicy(m_policy);
    m_mockLayout->setAnimation(0); // turn off animation
}

void Ut_MGridLayoutPolicy::cleanup()
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

    Q_ASSERT(0 != m_mockItem400);
    m_mockLayout->removeItem(m_mockItem400);
    m_mockItem400 = 0;

    Q_ASSERT(0 != m_policy);
    delete m_policy;
    m_policy = 0;

    Q_ASSERT(0 != m_mockLayout);
    delete m_form; //should delete layout too
    m_mockLayout = 0;
    m_form = 0;
}

void Ut_MGridLayoutPolicy::testDeletingLayout()
{
    MLayout *layout = new MLayout;
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);
    policy->addItem(m_mockItem100, 0, 0);
    delete policy;
    delete layout;
    qApp->processEvents();

}

/** Simplest test with one item, all the layout's sizes are determined by the
    sizes of the item. */
void Ut_MGridLayoutPolicy::testOneItemLayout()
{
    m_policy->addItem(m_mockItem100, 0, 0);
    qApp->processEvents();

    // item moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0),
             m_mockItem100->preferredSize()));

    // check layout size hints
    QCOMPARE(m_mockLayout->minimumSize(), QSizeF(100.0, 100.0));
    QCOMPARE(m_mockLayout->preferredSize(), QSizeF(100.0, 100.0));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(100.0, 100.0));
}

/** Test with two items in one row. */
void Ut_MGridLayoutPolicy::testHorizontally()
{
    m_policy->setSpacing(0.0);
    m_policy->addItem(m_mockItem100, 0, 0);
    m_policy->addItem(m_mockItem200, 0, 1);
    qApp->processEvents(); //let relayout happen

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(100.0, 0.0), QSize(200.0, 200.0)));

    // check layout size hints
    QCOMPARE(m_mockLayout->minimumSize(), QSizeF(300.0, 200.0));
    QCOMPARE(m_mockLayout->preferredSize(), QSizeF(300.0, 200.0));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(300.0, 200.0));
}

/** Test with two items in one column. */
void Ut_MGridLayoutPolicy::testVertically()
{
    m_policy->setSpacing(0.0);
    m_policy->addItem(m_mockItem100, 0, 0);
    m_policy->addItem(m_mockItem200, 1, 0);
    qApp->processEvents(); //let relayout happen

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(0.0, 100.0), QSize(200.0, 200.0)));

    // check layout size hints
    QCOMPARE(m_mockLayout->minimumSize(), QSizeF(200.0, 300.0));
    QCOMPARE(m_mockLayout->preferredSize(), QSizeF(200.0, 300.0));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(200.0, 300.0));
}

/** Test with three items in two columns and rows. */
void Ut_MGridLayoutPolicy::testHorizontallyAndVertically()
{
    m_policy->setSpacing(0.0);
    m_policy->addItem(m_mockItem100, 0, 0);
    m_policy->addItem(m_mockItem200, 0, 1);
    m_policy->addItem(m_mockItem300, 1, 1);
    qApp->processEvents(); //let relayout happen

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(100.0, 0.0), QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(100.0, 200.0), QSize(300.0, 300.0)));

    // check layout size hints
    QCOMPARE(m_mockLayout->minimumSize(), QSizeF(400.0, 500.0));
    QCOMPARE(m_mockLayout->preferredSize(), QSizeF(400.0, 500.0));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(400.0, 500.0));
}


/** Test with four items in two columns and rows and varying spacing values. */
void Ut_MGridLayoutPolicy::testSpacing()
{
    // general spacing
    qreal general_spacing = 12.0;
    m_policy->setSpacing(general_spacing);
    m_policy->addItem(m_mockItem100, 0, 0);
    m_policy->addItem(m_mockItem200, 0, 1);
    m_policy->addItem(m_mockItem300, 1, 0);
    m_policy->addItem(m_mockItem400, 1, 1);
    qApp->processEvents(); //let relayout happen

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(300.0 + general_spacing, 0.0),
             QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 200.0 + general_spacing),
             QSize(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(300.0 + general_spacing,
             200.0 + general_spacing),
             QSize(400.0, 400.0)));

    // row spacing
    qreal row_spacing = 17.0;
    m_policy->setVerticalSpacing(row_spacing); //trigger a relayout
    qApp->processEvents(); //let relayout happen

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(300.0 + general_spacing, 0.0),
             QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 200.0 + row_spacing),
             QSize(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(300.0 + general_spacing,
             200.0 + row_spacing),
             QSize(400.0, 400.0)));

    // column spacing
    qreal col_spacing = 7.0;
    m_policy->setHorizontalSpacing(col_spacing);
    qApp->processEvents();

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(300.0 + col_spacing, 0.0),
             QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 200.0 + row_spacing),
             QSize(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(300.0 + col_spacing,
             200.0 + row_spacing),
             QSize(400.0, 400.0)));

    // explicit spacing of single row
    qreal single_row_spacing = 33.0;
    m_policy->setRowSpacing(0.0, single_row_spacing);
    qApp->processEvents();

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(300.0 + col_spacing, 0.0),
             QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 200.0 + single_row_spacing),
             QSize(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(300.0 + col_spacing,
             200.0 + single_row_spacing),
             QSize(400.0, 400.0)));

    // explicit spacing of single column
    qreal single_col_spacing = 14.0;
    m_policy->setColumnSpacing(0.0, single_col_spacing);
    qApp->processEvents();

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(300.0 + single_col_spacing, 0.0),
             QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 200.0 + single_row_spacing),
             QSize(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(300.0 + single_col_spacing,
             200.0 + single_row_spacing),
             QSize(400.0, 400.0)));
}


/** Test with four items in two columns and rows and varying alignment values. */
void Ut_MGridLayoutPolicy::testAlignment()
{
    // general spacing
    m_policy->setSpacing(0.0);
    m_policy->addItem(m_mockItem100, 0, 0);
    m_policy->addItem(m_mockItem200, 0, 1);
    m_policy->addItem(m_mockItem300, 1, 0);
    m_policy->addItem(m_mockItem400, 1, 1);
    qApp->processEvents();

    // default is left/top aligned
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(300.0, 0.0), QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 200.0), QSize(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(300.0, 200.0), QSize(400.0, 400.0)));

    // switch first column to center and second column to right alignment
    // (the comparisons assume, that mock item 100 is smaller than mock item 200
    //  and so on)
    m_policy->setColumnAlignment(0, Qt::AlignCenter);
    m_policy->setColumnAlignment(1, Qt::AlignRight);
    qApp->processEvents();

    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(100.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(500.0, 0.0), QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 200.0), QSize(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(300.0, 200.0), QSize(400.0, 400.0)));

    // switch first row to bottom and second row to center alignment
    // (the comparisons assume, that mock item 100 is smaller than mock item 200
    //  and so on)
    m_policy->setRowAlignment(0, Qt::AlignBottom);
    m_policy->setRowAlignment(1, Qt::AlignVCenter);
    qApp->processEvents();

    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(100.0, 100.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(500.0, 0.0), QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 250.0), QSize(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(300.0, 200.0), QSize(400.0, 400.0)));

    // override alignment for single items
    m_policy->setAlignment(m_mockItem100, Qt::AlignLeft | Qt::AlignTop);
    m_policy->setAlignment(m_mockItem300, Qt::AlignLeft | Qt::AlignTop);
    qApp->processEvents();

    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(500.0, 0.0), QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 200.0), QSize(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(300.0, 200.0), QSize(400.0, 400.0)));
}


/** Test that the alignment works on sub layouts, too. */
void Ut_MGridLayoutPolicy::testAlignmentSubLayouts()
{
    // general spacing
    m_policy->setSpacing(0.0);

    // create two sublayouts
    MLayout *subLayout100 = new MLayout;
    subLayout100->setAnimation(0);
    MGridLayoutPolicy *subPolicy100 = new MGridLayoutPolicy(subLayout100);
    m_policy->addItem(subLayout100, 0, 0);
    subPolicy100->setSpacing(0.0);
    subPolicy100->addItem(m_mockItem100, 0, 0);

    MLayout *subLayout200 = new MLayout;
    subLayout200->setAnimation(0);
    MGridLayoutPolicy *subPolicy200 = new MGridLayoutPolicy(subLayout200);
    m_policy->addItem(subLayout200, 1, 0);
    subPolicy200->setSpacing(0.0);
    subPolicy200->addItem(m_mockItem200, 0, 0);

    m_policy->addItem(m_mockItem300, 2, 0);
    qApp->processEvents();

    // default is left/top aligned
    QCOMPARE(subLayout100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(subLayout200->geometry(), QRectF(QPointF(0.0, 100.0), QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 300.0), QSize(300.0, 300.0)));

    // now change some alignments
    m_policy->setAlignment(subLayout100, Qt::AlignRight);
    m_policy->setAlignment(subLayout200, Qt::AlignCenter);
    qApp->processEvents();

    //These currently (2010-09-08) fail because of https://projects.maemo.org/bugzilla/show_bug.cgi?id=189091
    QEXPECT_FAIL("", "See NB#189091", Continue);
    QCOMPARE(subLayout100->geometry(), QRectF(QPointF(200.0, 0.0), QSize(100.0, 100.0)));
    QEXPECT_FAIL("", "See NB#189091", Continue);
    QCOMPARE(subLayout200->geometry(), QRectF(QPointF(50.0, 100.0), QSize(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0.0, 300.0), QSize(300.0, 300.0)));
}

void Ut_MGridLayoutPolicy::testRowCount()
{
    /*  QGraphicsGridLayout layout;
        layout.setColumnFixedWidth(0, 100);
        layout.setColumnFixedWidth(1, 100);
        layout.setColumnFixedWidth(2, 100);
        layout.setColumnFixedWidth(3, 100);
        QCOMPARE(layout.rowCount(), 0);
        QCOMPARE(layout.columnCount(), 0); */

    m_policy->setColumnFixedWidth(0, 100);
    m_policy->setColumnFixedWidth(1, 100);
    m_policy->setColumnFixedWidth(2, 100);
    m_policy->setColumnFixedWidth(3, 100);
    QCOMPARE(m_policy->rowCount(), 0);
    QCOMPARE(m_policy->columnCount(), 0);
}

QTEST_APPLESS_MAIN(Ut_MGridLayoutPolicy)
