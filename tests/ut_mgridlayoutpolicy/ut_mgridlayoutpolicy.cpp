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
#include <mlinearlayoutpolicy.h>

#include <QtGui/QGraphicsGridLayout>
#include <QtGui/QGraphicsLinearLayout>
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

    QCOMPARE(subLayout100->geometry(), QRectF(QPointF(200.0, 0.0), QSize(100.0, 100.0)));
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

void Ut_MGridLayoutPolicy::testRtl()
{
    m_form->setLayoutDirection(Qt::LeftToRight);
    m_policy->setSpacing(0);
    m_policy->addItem(m_mockItem100,0,0);
    m_policy->addItem(m_mockItem200,0,1);
    m_mockLayout->activate();
    qApp->processEvents(); //let relayout happen
    // now m_mockItem100 should be on the left side and
    // m_mockItem200 on the right side:
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(100.0, 0.0), QSizeF(200.0, 200.0)));

    m_form->setLayoutDirection(Qt::RightToLeft);
    qApp->processEvents(); //let relayout happen
    // now the two items should have reversed their order,
    // i.e. now m_mockItem100 should be on the right side and
    // m_mockItem200 on the left side:
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(200.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(0.0, 0.0), QSizeF(200.0, 200.0)));
}

void Ut_MGridLayoutPolicy::testLayoutInLayoutRefresh()
{
    m_policy->setSpacing(0);
    m_mockLayout->activate();

    QGraphicsWidget *widget = new QGraphicsWidget;
    MLayout *layout = new MLayout(widget);
    layout->setContentsMargins(0,0,0,0);
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);
    m_policy->addItem(widget,0,0);

    QGraphicsWidget *leftSpacerWithWidget = new QGraphicsWidget;
    policy->addItem(leftSpacerWithWidget, 0, 0);
    policy->addItem(m_mockItem100, 0, 1);
    policy->addItem(new QGraphicsWidget, 0, 2);
    policy->addItem(m_mockItem200, 0, 3);

    m_form->resize(400,200);

    qApp->processEvents();
    qApp->processEvents();

    QCOMPARE(m_mockItem100->geometry(), QRectF(50,0,100,100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(200,0,200,200));

    leftSpacerWithWidget->setMaximumWidth(0);

    qApp->processEvents();

    QCOMPARE(m_mockItem100->geometry(), QRectF(0,0,100,100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(200,0,200,200));
}

QGraphicsWidget *createSpacer()
{
    QGraphicsWidget *spacer = new QGraphicsWidget();

    spacer->hide();
    spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    spacer->setMinimumSize(0, 0);
    spacer->setPreferredSize(0, 0);

    return spacer;
}

//A widget that likes to have a square shape
class SquareWidget : public QGraphicsWidget
{
    public:
        SquareWidget() {
            QSizePolicy p = sizePolicy();
            p.setHeightForWidth(true);
            setSizePolicy(p);
        }
    protected:
        virtual QSizeF sizeHint ( Qt::SizeHint which, const QSizeF & constraint = QSizeF() ) const
        {
            if (which != Qt::PreferredSize)
                return QGraphicsWidget::sizeHint(which, constraint);
            if (constraint.width() >= 0)
                return QSizeF(constraint.width(), constraint.width());
            else
                return QSizeF(500,500);
        }
};

void Ut_MGridLayoutPolicy::testHeightForWidthInSubLayout_data()
{
    QTest::addColumn<bool>("useMLayout"); //Whether to MLayout or QGraphicsGridLayout
    QTest::addColumn<bool>("useInnerMLayout"); //Whether to MLayout or QGraphicsGridLayout
    QTest::addColumn<bool>("putInnerWidgetInWidget"); //Whether to put the square widget inside of another widget

    QTest::newRow("Use MLayouts only") << true << true << false;
    QTest::newRow("Use MLayouts with inner widget") << true << true << true;
    QTest::newRow("Use QGraphicsLayouts only") << false << false << false;
    QTest::newRow("Use QGraphicsLayouts with inner widget") << false << false << true;
    QTest::newRow("Use outer MLayouts with inner QGraphicsLayout") << true << false << false;
    QTest::newRow("Use outer MLayouts with inner QGraphicsLayout and inner widget") << true << false << true;
    QTest::newRow("Use outer QGraphicsLayout with inner MLayout") << false << true << false;
    QTest::newRow("Use outer QGraphicsLayout with inner MLayout and inner widget") << false << true << true;

}
void Ut_MGridLayoutPolicy::testHeightForWidthInSubLayout()
{
    QFETCH(bool, useMLayout);
    QFETCH(bool, useInnerMLayout);
    QFETCH(bool, putInnerWidgetInWidget);

    QGraphicsWidget *form = new QGraphicsWidget;


    MGridLayoutPolicy *mpolicy = NULL;
    QGraphicsGridLayout *qlayout = NULL;

    if (useMLayout) {
        MLayout *mlayout = new MLayout(form);
        mlayout->setContentsMargins(0, 0, 0, 0);
        mpolicy = new MGridLayoutPolicy(mlayout);
        mpolicy->setSpacing(0);
    } else {
        qlayout = new QGraphicsGridLayout(form);
        qlayout->setContentsMargins(0, 0, 0, 0);
        qlayout->setSpacing(0);
    }

    QGraphicsWidget *topSpacer = createSpacer();
    QGraphicsWidget *leftSpacer = createSpacer();
    QGraphicsWidget *rightSpacer = createSpacer();
    leftSpacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    rightSpacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    QGraphicsWidget *square = new SquareWidget;

    QGraphicsLayout *innerLayout = NULL;
    if (useInnerMLayout) {
        innerLayout = new MLayout();
        MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(static_cast<MLayout *>(innerLayout), Qt::Horizontal);
        policy->addItem(square);
    } else {
        innerLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        static_cast<QGraphicsLinearLayout *>(innerLayout)->addItem(square);
    }
    innerLayout->setContentsMargins(0,0,0,0);

    QGraphicsLayoutItem *innerItem;
    if (putInnerWidgetInWidget) {
        QGraphicsWidget *innerWidget = new QGraphicsWidget;
        innerWidget->setLayout(innerLayout);
        innerItem = innerWidget;
    } else {
        innerItem = innerLayout;
    }

    if (useMLayout) {
        mpolicy->addItem(topSpacer, 0, 1);
        mpolicy->addItem(leftSpacer, 1, 0);
        mpolicy->addItem(rightSpacer, 1, 2);
        mpolicy->addItem(innerItem, 1, 1);
    } else {
        qlayout->addItem(topSpacer, 0, 1);
        qlayout->addItem(leftSpacer, 1, 0);
        qlayout->addItem(rightSpacer, 1, 2);
        qlayout->addItem(innerItem, 1, 1);
    }


    QCOMPARE(form->preferredSize(), QSizeF(500,500));
    QCOMPARE(form->effectiveSizeHint(Qt::PreferredSize, QSizeF(100,-1)), QSizeF(100,100));
}

void Ut_MGridLayoutPolicy::testDeleteItem_data()
{
    QTest::addColumn<bool>("addToVisiblePolicy");
    QTest::addColumn<bool>("addToInvisiblePolicy");

    QTest::newRow("Add item to visible policy only") << true << false;
    QTest::newRow("Add item to invisible policy only") << false << true;
    QTest::newRow("Add item to both policies") << true << true;
}
void Ut_MGridLayoutPolicy::testDeleteItem()
{
    QFETCH(bool, addToVisiblePolicy);
    QFETCH(bool, addToInvisiblePolicy);

    QGraphicsWidget *w1 = new QGraphicsWidget;
    QGraphicsWidget *w2 = new QGraphicsWidget;
    QGraphicsWidget *w3 = new QGraphicsWidget;

    MGridLayoutPolicy *policy2 = new MGridLayoutPolicy(m_mockLayout);

    if (addToVisiblePolicy) {
        m_policy->addItem(w1, 0,0);
        m_policy->addItem(w2, 1,0);
        m_policy->addItem(w3, 0,1);
        QCOMPARE( m_policy->count(), 3);
    }
    if (addToInvisiblePolicy) {
        policy2->addItem(w1, 0,0);
        policy2->addItem(w2, 1,0);
        policy2->addItem(w3, 0,1);
        QCOMPARE( policy2->count(), 3);
    }

    delete w1;

    QCOMPARE( m_policy->count(), addToVisiblePolicy?2:0);
    QCOMPARE( policy2->count(), addToInvisiblePolicy?2:0);
    QCOMPARE( m_mockLayout->count(), 2);

    m_mockLayout->activate();

    delete w2;

    QCOMPARE( m_policy->count(), addToVisiblePolicy?1:0);
    QCOMPARE( policy2->count(), addToInvisiblePolicy?1:0);
    QCOMPARE( m_mockLayout->count(), 1);

    qApp->processEvents(); //let relayout happen

    delete w3;

    QCOMPARE( m_policy->count(), 0);
    QCOMPARE( policy2->count(), 0);
    QCOMPARE( m_mockLayout->count(), 0);

    //Just make sure there are no more events that could cause us to crash
    qApp->processEvents();
    qApp->processEvents();
    qApp->processEvents();
}


QTEST_APPLESS_MAIN(Ut_MGridLayoutPolicy)
