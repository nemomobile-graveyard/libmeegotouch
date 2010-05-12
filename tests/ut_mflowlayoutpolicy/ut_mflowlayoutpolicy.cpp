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

#include "ut_mflowlayoutpolicy.h"

#include <mapplication.h>
#include <mlayout.h>
#include <mflowlayoutpolicy.h>
#include <mbasiclayoutanimation.h>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QSizePolicy>
#include <MApplicationWindow>
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


Ut_MFlowLayoutPolicy::Ut_MFlowLayoutPolicy()
    : m_mockLayout(0)
    , m_policy(0)
    , m_mockItem100(0)
    , m_mockItem200(0)
    , m_mockItem300(0)
    , m_form(0)
{ }

Ut_MFlowLayoutPolicy::~Ut_MFlowLayoutPolicy()
{ }

MApplication *app;
MApplicationWindow *appWin;
void Ut_MFlowLayoutPolicy::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mflowlayoutpolicy" };
    app = new MApplication(argc, argv);
    app->setLayoutDirection(Qt::LeftToRight);
    appWin = new MApplicationWindow;
}

void Ut_MFlowLayoutPolicy::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_MFlowLayoutPolicy::init()
{
    Q_ASSERT(0 == m_mockLayout);
    m_mockLayout = new MLayout();
    m_mockLayout->setContentsMargins(0, 0, 0, 0);
    m_form = new QGraphicsWidget;
    m_form->setLayout(m_mockLayout);
    Q_ASSERT(0 == m_policy);
    m_policy = new MFlowLayoutPolicy(m_mockLayout);
    m_policy->setSpacing(0);

    // set up some mock items
    Q_ASSERT(0 == m_mockItem100);
    m_mockItem100 = new MockLayoutItem;
    m_mockItem100->setPreferredSize(100, 100);

    Q_ASSERT(0 == m_mockItem200);
    m_mockItem200 = new MockLayoutItem;
    m_mockItem200->setPreferredSize(200, 100);

    Q_ASSERT(0 == m_mockItem300);
    m_mockItem300 = new MockLayoutItem;
    m_mockItem300->setPreferredSize(300, 100);

    m_mockLayout->setPolicy(m_policy);
    m_mockLayout->setAnimation(0); // turn off animation
}

void Ut_MFlowLayoutPolicy::cleanup()
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

/** Simplest test with one item, layout's preferred size is determined by the
    preferred size of the item. */
void Ut_MFlowLayoutPolicy::testOneItemLayout()
{
    m_policy->addItem(m_mockItem100);
    qApp->processEvents(); //for spacing change to trigger relayout

    // item moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0, 0),
             m_mockItem100->preferredSize()));

    // check layout size hints
    //QCOMPARE(m_mockLayout->minimumSize(), m_mockItem100->minimumSize());
    QCOMPARE(m_mockLayout->minimumSize(), m_mockItem100->preferredSize());
    QCOMPARE(m_mockLayout->preferredSize(), m_mockItem100->preferredSize());
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

    QCOMPARE(m_policy->rowCount(), 1);
}

/** Test with two items, layout is wide enough to keep both items in a single
    row. */
void Ut_MFlowLayoutPolicy::testTwoItemsHorizontally()
{
    QCOMPARE(m_mockLayout->sizePolicy().horizontalPolicy(), QSizePolicy::Expanding);
    QCOMPARE(m_mockLayout->sizePolicy().verticalPolicy(), QSizePolicy::Expanding);
    QCOMPARE(m_mockLayout->sizePolicy().hasHeightForWidth(), true);
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);

    m_form->setGeometry(QRectF(0, 0, m_mockItem100->preferredWidth() +
                               m_mockItem200->preferredWidth(), 500));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, m_mockItem100->preferredWidth() +
             m_mockItem200->preferredWidth(), 500));

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0, 0),
             m_mockItem100->preferredSize()));
    QCOMPARE(m_mockItem200->geometry(), QRectF(
                 QPointF(m_mockItem100->preferredWidth(), 0),
                 m_mockItem200->preferredSize()));

    // check layout size hints
    /*    QCOMPARE(m_mockLayout->minimumSize(),
                 QSizeF(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth(),
                        qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight())));*/
    QCOMPARE(m_mockLayout->minimumSize(),
             QSizeF(qMax(m_mockItem100->preferredWidth(), m_mockItem200->preferredWidth()),
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight())));

    QCOMPARE(m_mockLayout->preferredSize(),
             QSizeF(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth(),
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight())));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    QCOMPARE(m_policy->rowCount(), 1);
}

/** Test with two items, layout is so narrow that each item should end up in
    its own row. */
void Ut_MFlowLayoutPolicy::testTwoItemsVertically()
{
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);
    m_form->setGeometry(QRectF(0, 0, m_mockItem100->preferredWidth() +
                               m_mockItem200->preferredWidth() - 1, 500));

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0, 0),
             m_mockItem100->preferredSize()));
    QCOMPARE(m_mockItem200->geometry(), QRectF(
                 QPointF(0, m_mockItem100->preferredHeight()),
                 m_mockItem200->preferredSize()));

    // check layout size hints.  Setting geometry shouldn't affect size hint, so the items should still layout out on the same row
    //QCOMPARE(m_mockLayout->minimumSize(), QSizeF(2,1));
    QCOMPARE(m_mockLayout->minimumSize(),
             QSizeF(qMax(m_mockItem100->preferredWidth(), m_mockItem200->preferredWidth()),
                    m_mockItem100->preferredHeight() + m_mockItem200->preferredHeight()));

    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

    // Now set the maximum width. This time the size hint should respect the maximum width
    m_mockLayout->setMaximumWidth(m_mockItem100->preferredWidth() +
                                  m_mockItem200->preferredWidth() - 100);
    m_mockLayout->setPreferredWidth(m_mockItem100->preferredWidth() +
                                    m_mockItem200->preferredWidth() - 100);
    //QCOMPARE(m_mockLayout->minimumSize(), QSizeF(2,1));
    QCOMPARE(m_mockLayout->minimumSize(),
             QSizeF(qMax(m_mockItem100->preferredWidth(), m_mockItem200->preferredWidth()),
                    m_mockItem100->preferredHeight() + m_mockItem200->preferredHeight()));

    QCOMPARE(m_mockLayout->preferredSize(),
             QSizeF(qMax(m_mockItem100->preferredWidth(), m_mockItem200->preferredWidth()),
                    m_mockItem100->preferredHeight() + m_mockItem200->preferredHeight()));
    QCOMPARE(m_mockLayout->maximumSize(),
             QSizeF(m_mockItem100->preferredWidth() +
                    m_mockItem200->preferredWidth() - 100,
                    QWIDGETSIZE_MAX));

    QCOMPARE(m_policy->rowCount(), 2);
}

/** Test with three items, layout is so narrow that the items should end up in
    two rows. */
void Ut_MFlowLayoutPolicy::testHorizontallyAndVertically()
{
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);
    m_policy->addItem(m_mockItem300);

    m_form->setGeometry(QRectF(0, 0, m_mockItem100->preferredWidth() +
                               m_mockItem200->preferredWidth(), 500));

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0, 0),
             m_mockItem100->preferredSize()));
    QCOMPARE(m_mockItem200->geometry(), QRectF(
                 QPointF(m_mockItem100->preferredWidth(), 0),
                 m_mockItem200->preferredSize()));
    QCOMPARE(m_mockItem300->geometry(), QRectF(
                 QPointF(0, qMax(m_mockItem100->preferredHeight(),
                                 m_mockItem200->preferredHeight())),
                 m_mockItem300->preferredSize()));

    // check layout size hints. These should just ignore the current geometry
    //QCOMPARE(m_mockLayout->minimumSize(), QSizeF(3, 1));
    QCOMPARE(m_mockLayout->minimumSize(),
             QSizeF(qMax(qMax(m_mockItem100->preferredWidth(), m_mockItem200->preferredWidth()), m_mockItem300->preferredWidth()),
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight()) + m_mockItem300->preferredHeight()));

    QCOMPARE(m_mockLayout->preferredSize(),
             QSizeF(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth(), //current geometry
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight()) +  m_mockItem300->preferredHeight()));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

    m_mockLayout->setMaximumWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth());
    m_mockLayout->setPreferredWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth());
    //Now lets set the geometry to something with a large width and small height then let it resize with setMaximumWidth
    m_form->setGeometry(QRectF(0, 0, 10000, 0));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth(), qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight()) + m_mockItem300->preferredHeight()));
    QCOMPARE(m_policy->rowCount(), 2);
}
void Ut_MFlowLayoutPolicy::testReorderingFlow()
{
    //This specifically tests bug report 110604
    m_mockLayout->setContentsMargins(1, 1, 1, 1);

    QList<QGraphicsLayoutItem *> items;
    for (int i = 0; i < 3; i++) {
        QGraphicsWidget *item = new QGraphicsWidget;
        item->setMinimumWidth(250);
        item->setPreferredWidth(250);
        item->setMaximumWidth(250);
        item->setMinimumHeight(10);
        item->setPreferredHeight(10);
        m_policy->addItem(item);
        items << item;
    }
    // Make the parent widget big enough to fit all 3 widgets
    m_form->setMinimumWidth(864);
    m_form->setMaximumWidth(864);
    qApp->processEvents(); //for spacing change to trigger relayout
    //All three children should be on one row
    QCOMPARE(items[0]->geometry(), QRectF(1, 1, 250, 10));
    QCOMPARE(items[1]->geometry(), QRectF(251, 1, 250, 10));
    QCOMPARE(items[2]->geometry(), QRectF(501, 1, 250, 10));
    QCOMPARE(m_policy->rowCount(), 1);

    // Make the parent widget big enough to fit only 1 widget per row
    m_form->setMinimumWidth(267);
    m_form->setMaximumWidth(267);
    qApp->processEvents();
    QCOMPARE(items[0]->geometry(), QRectF(1, 1, 250, 10));
    QCOMPARE(items[1]->geometry(), QRectF(1, 11, 250, 10));
    QCOMPARE(items[2]->geometry(), QRectF(1, 21, 250, 10));

    QCOMPARE(m_form->minimumHeight(), 32.0);
    QCOMPARE(m_policy->rowCount(), 3);
}

/** Test with margins and three items, layout is so narrow that the items should end up in
    two rows. */
void Ut_MFlowLayoutPolicy::testMargins()
{
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);
    m_policy->addItem(m_mockItem300);

    m_mockLayout->setContentsMargins(1, 1, 1, 1);
    m_form->setGeometry(QRectF(0, 0, m_mockItem100->preferredWidth() +
                               m_mockItem200->preferredWidth() + 2, 500));

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(1, 1),
             m_mockItem100->preferredSize()));
    QCOMPARE(m_mockItem200->geometry(), QRectF(
                 QPointF(m_mockItem100->preferredWidth() + 1, 1),
                 m_mockItem200->preferredSize()));
    QCOMPARE(m_mockItem300->geometry(), QRectF(
                 QPointF(1, 1 + qMax(m_mockItem100->preferredHeight(),
                                     m_mockItem200->preferredHeight())),
                 m_mockItem300->preferredSize()));

    // check layout size hints. These should just ignore the current geometry
    //QCOMPARE(m_mockLayout->minimumSize(), QSizeF(5, 3));
    QCOMPARE(m_mockLayout->minimumSize(),
             QSizeF(qMax(qMax(m_mockItem100->preferredWidth(), m_mockItem200->preferredWidth()), m_mockItem300->preferredWidth()) + 2,
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight()) + m_mockItem300->preferredHeight() + 2));

    QCOMPARE(m_mockLayout->preferredSize(),
             QSizeF(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 2, //current geometry
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight()) + m_mockItem300->preferredHeight() + 2));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    QCOMPARE(m_policy->rowCount(), 2);

    m_mockLayout->setMaximumWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 2);
    m_mockLayout->setPreferredWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 2);
    //Now lets set the geometry to something with a large width and small height then let it resize with setMaximumWidth
    m_form->setGeometry(QRectF(0, 0, 10000, 0));
    QCOMPARE(m_policy->rowCount(), 2);
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 2, 2 + qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight()) + m_mockItem300->preferredHeight()));
}

/** Test with margins and two items, layout is so narrow that the items should end up in
    two rows. */
void Ut_MFlowLayoutPolicy::testMarginsAndTwoItems()
{
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);

    m_mockLayout->setContentsMargins(1, 1, 1, 1);
    m_form->setGeometry(QRectF(0, 0, m_mockItem100->preferredWidth() +
                               m_mockItem200->preferredWidth() + 1, 500)); //exactly 1 pixel too small
    QCOMPARE(m_policy->rowCount(), 2);

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(1, 1),
             m_mockItem100->preferredSize()));
    QCOMPARE(m_mockItem200->geometry(), QRectF(
                 QPointF(1, 1 + m_mockItem100->preferredHeight()),
                 m_mockItem200->preferredSize()));
}

/** Test with three items with spacing.  The first and second items should be on one row with a space between them.  The third item on the second row */
void Ut_MFlowLayoutPolicy::testSpacing()
{
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);
    m_policy->addItem(m_mockItem300);
    qApp->processEvents(); //for spacing change to trigger relayout

    m_form->setGeometry(QRectF(0, 0, m_mockItem100->preferredWidth() +
                               m_mockItem200->preferredWidth() + 15, 500));
    m_policy->setSpacing(15);
    QCOMPARE(m_policy->rowCount(), 2);
    qApp->processEvents(); //for spacing change to trigger relayout

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0, 0),
             m_mockItem100->preferredSize()));
    QCOMPARE(m_mockItem200->geometry(), QRectF(
                 QPointF(m_mockItem100->preferredWidth() + 15, 0),
                 m_mockItem200->preferredSize()));
    QCOMPARE(m_mockItem300->geometry(), QRectF(
                 QPointF(0, qMax(m_mockItem100->preferredHeight(),
                                 m_mockItem200->preferredHeight()) + 15),
                 m_mockItem300->preferredSize()));

    // check layout size hints.
    //QCOMPARE(m_mockLayout->minimumSize(), QSizeF(3, 1));
    QCOMPARE(m_mockLayout->minimumSize(),
             QSizeF(qMax(qMax(m_mockItem100->preferredWidth(), m_mockItem200->preferredWidth()), m_mockItem300->preferredWidth()),
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight() + 15 + m_mockItem300->preferredHeight())));

    QCOMPARE(m_mockLayout->preferredSize(),
             QSizeF(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15, //current width
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight()) + 15 + m_mockItem300->preferredHeight()));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    QCOMPARE(m_policy->rowCount(), 2);

    m_form->setMinimumWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15);
    m_form->setMaximumWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15);
    m_form->setPreferredWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15);
    //Now lets set the geometry to something with a large width and small height then let it resize with setMaximumWidth
    m_form->setGeometry(QRectF(0, 0, 10000, 0));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15, m_mockItem200->preferredHeight() + m_mockItem300->preferredHeight() + 15));
    QCOMPARE(m_policy->rowCount(), 2);
}

/** Test with three items with different horizontal and vertical spacing.  The first and second items should be on one row with a space between them.  The third item on the second row */
void Ut_MFlowLayoutPolicy::testHorizontalVerticalSpacing()
{
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);
    m_policy->addItem(m_mockItem300);
    qApp->processEvents(); //for spacing change to trigger relayout

    m_form->setGeometry(QRectF(0, 0, m_mockItem100->preferredWidth() +
                               m_mockItem200->preferredWidth() + 15, 500));
    m_policy->setHorizontalSpacing(15);
    m_policy->setVerticalSpacing(10);
    qApp->processEvents(); //for spacing change to trigger relayout

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0, 0),
             m_mockItem100->preferredSize()));
    QCOMPARE(m_mockItem200->geometry(), QRectF(
                 QPointF(m_mockItem100->preferredWidth() + 15, 0),
                 m_mockItem200->preferredSize()));
    QCOMPARE(m_mockItem300->geometry(), QRectF(
                 QPointF(0, qMax(m_mockItem100->preferredHeight(),
                                 m_mockItem200->preferredHeight()) + 10),
                 m_mockItem300->preferredSize()));

    // check layout size hints. These should just ignore the current geometry
    //QCOMPARE(m_mockLayout->minimumSize(), QSizeF(3, 1));
    QCOMPARE(m_mockLayout->minimumSize(),
             QSizeF(qMax(qMax(m_mockItem100->preferredWidth(), m_mockItem200->preferredWidth()), m_mockItem300->preferredWidth()),
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight() + 10 + m_mockItem300->preferredHeight())));

    QCOMPARE(m_mockLayout->preferredSize(),
             QSizeF(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15, //current geometry
                    qMax(m_mockItem100->preferredHeight(), m_mockItem200->preferredHeight()) + 10 + m_mockItem300->preferredHeight()));
    QCOMPARE(m_mockLayout->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

    m_form->setMinimumWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15);
    m_form->setMaximumWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15);
    m_form->setPreferredWidth(m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15);
    //Now lets set the geometry to something with a large width and small height then let it resize with setMaximumWidth
    m_form->setGeometry(QRectF(0, 0, 10000, 0));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, m_mockItem100->preferredWidth() + m_mockItem200->preferredWidth() + 15, m_mockItem200->preferredHeight() + m_mockItem300->preferredHeight() + 10));
    QCOMPARE(m_policy->rowCount(), 2);
}

Q_DECLARE_METATYPE(QSizePolicy::Policy);
void Ut_MFlowLayoutPolicy::testHorizontalJustification_data()
{
    qRegisterMetaType<QSizePolicy::Policy>("QSizePolicy::Policy");
    QTest::addColumn<QSizePolicy::Policy>("horizontalPolicy");
    QTest::addColumn<QSizePolicy::Policy>("verticalPolicy");
    QTest::addColumn<int>("numItems");

    //Test with 3x3 items
    QTest::newRow("Horizontal: Fixed, Vertical: Fixed, 9 items") << QSizePolicy::Fixed << QSizePolicy::Fixed << 9;
    QTest::newRow("Horizontal: Maximum, Vertical: Maximum, 9 items") << QSizePolicy::Maximum << QSizePolicy::Maximum << 9;
    QTest::newRow("Horizontal: Expanding, Vertical: Maximum, 9 items") << QSizePolicy::Expanding << QSizePolicy::Maximum << 9;
    QTest::newRow("Horizontal: Expanding, Vertical: Expanding, 9 items") << QSizePolicy::Expanding << QSizePolicy::Expanding << 9;
    QTest::newRow("Horizontal: Fixed, Vertical: Expanding, 9 items") << QSizePolicy::Expanding << QSizePolicy::Expanding << 9;

    //Test with 3x3 items, plus 1 that is stretched across the entire row
    QTest::newRow("Horizontal: Fixed, Vertical: Fixed, 10 items") << QSizePolicy::Fixed << QSizePolicy::Fixed << 10;
    QTest::newRow("Horizontal: Maximum, Vertical: Maximum, 10 items") << QSizePolicy::Maximum << QSizePolicy::Maximum << 10;
    QTest::newRow("Horizontal: Expanding, Vertical: Maximum, 10 items") << QSizePolicy::Expanding << QSizePolicy::Maximum << 10;
    QTest::newRow("Horizontal: Expanding, Vertical: Expanding, 10 items") << QSizePolicy::Expanding << QSizePolicy::Expanding << 10;
    QTest::newRow("Horizontal: Fixed, Vertical: Expanding, 10 items") << QSizePolicy::Expanding << QSizePolicy::Expanding << 10;

    //Test with 3x3 items, plus 2 that are equally stretched across the row
    QTest::newRow("Horizontal: Fixed, Vertical: Fixed, 11 items") << QSizePolicy::Fixed << QSizePolicy::Fixed << 11;
    QTest::newRow("Horizontal: Maximum, Vertical: Maximum, 11 items") << QSizePolicy::Maximum << QSizePolicy::Maximum << 11;
    QTest::newRow("Horizontal: Expanding, Vertical: Maximum, 11 items") << QSizePolicy::Expanding << QSizePolicy::Maximum << 11;
    QTest::newRow("Horizontal: Expanding, Vertical: Expanding, 11 items") << QSizePolicy::Expanding << QSizePolicy::Expanding << 11;
    QTest::newRow("Horizontal: Fixed, Vertical: Expanding, 11 items") << QSizePolicy::Expanding << QSizePolicy::Expanding << 11;

}
void Ut_MFlowLayoutPolicy::testEmptyCount()
{
    QCOMPARE(m_policy->rowCount(), 0);
}
void Ut_MFlowLayoutPolicy::testHorizontalJustification()
{
    QFETCH(QSizePolicy::Policy, horizontalPolicy);
    QFETCH(QSizePolicy::Policy, verticalPolicy);
    QFETCH(int, numItems);
    for (int i = 0; i < numItems; ++i) {
        QGraphicsWidget *widget = new QGraphicsWidget;
        if (i % 2) //make one tall, one short, one tall, one short, etc..
            widget->setPreferredSize(100, 100);
        else
            widget->setPreferredSize(100, 50);

        widget->setSizePolicy(horizontalPolicy, verticalPolicy);
        m_policy->addItem(widget);
        m_policy->setAlignment(widget, Qt::AlignLeft | Qt::AlignTop);
        qApp->processEvents(); //for spacing change to trigger relayout
    }
    //test with all three items set to
    m_form->setGeometry(QRectF(0, 0, 300 /*preferred sizes */ + 2 /*spacing*/ + 6 /*expandable size */, 10000));
    m_policy->setHorizontalSpacing(1);
    m_policy->setVerticalSpacing(0);
    qApp->processEvents(); //for spacing change to trigger relayout

    for (int i = 0; i < numItems; ++i) {
        QPointF position;
        QSizeF size;
        //the items should have expanded and moved horizontally to fill up the extra space
        if (horizontalPolicy & QSizePolicy::GrowFlag) {
            position = QPointF(103 * (i % 3), 100 * (i / 3));
            if (i == 10) {
                position.setX(153.5 + 1);
                size.setWidth(153.5);
            } else if (i == 9 && numItems == 11)
                size.setWidth(153.5);
            else if (i == 9 && numItems == 10)
                size.setWidth(300 + 2 + 6);
            else
                size.setWidth(102);
        } else {
            position = QPointF(101 * (i % 3), 100 * (i / 3));
            size.setWidth(100);
        }

        if (verticalPolicy & QSizePolicy::GrowFlag)
            size.setHeight(100);
        else
            size.setHeight((i % 2) ? 100 : 50);
        QCOMPARE(m_mockLayout->itemAt(i)->geometry(), QRectF(position, size));
    }
    if (numItems == 9)
        QCOMPARE(m_policy->rowCount(), 3);
    else
        QCOMPARE(m_policy->rowCount(), 4);
}

void Ut_MFlowLayoutPolicy::testTwoByTwoThenRemoveThird_data()
{
    QTest::addColumn<bool>("deleteItem");
    QTest::addColumn<bool>("removeFromLayout");
    QTest::addColumn<bool>("removeFromPolicy");

    //test every combination, except all of them false
    for (int removeFromLayout = 0; removeFromLayout < 2; removeFromLayout++)
        for (int removeFromPolicy = 0; removeFromPolicy < 2; removeFromPolicy++)
            for (int deleteItem = (removeFromLayout || removeFromPolicy) ? 0 : 1; deleteItem < 2; deleteItem++) {
                QString description = QString("") +
                                      (removeFromLayout ? "Remove from layout. " : "") +
                                      (removeFromPolicy ? "Remove from policy. " : "") +
                                      (deleteItem ? "Delete." : "");
                QTest::newRow(description.toLatin1()) << (bool)deleteItem << (bool)removeFromLayout << (bool)removeFromPolicy;
            }
}
void Ut_MFlowLayoutPolicy::testTwoByTwoThenRemoveThird()
{
    //This tests specifically for NB#140106
    //We add four items of size 25x25 in a 2x2 order, then remove the third
    QFETCH(bool, deleteItem);
    QFETCH(bool, removeFromLayout);
    QFETCH(bool, removeFromPolicy);

    m_form->setMaximumWidth(54);
    m_form->setMinimumWidth(54);
    m_mockLayout->setContentsMargins(2, 2, 2, 2);
    for (int i = 0; i < 4; i++) {
        QGraphicsWidget *item = new QGraphicsWidget;
        item->setMinimumSize(25, 25);
        item->setMaximumSize(25, 25);
        m_policy->addItem(item);
    }
    qApp->processEvents();
    QCOMPARE(m_form->geometry(), QRectF(0, 0, 54, 54));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, 54, 54));
    for (int i = 0; i < 4; i++) {
        QCOMPARE(m_policy->itemAt(i)->geometry(), QRectF((i % 2) * 25 + 2, (i / 2) * 25 + 2, 25, 25));
        QVERIFY(m_mockLayout->itemAt(i) == m_policy->itemAt(i));
    }
    QGraphicsLayoutItem *thirdItem = m_mockLayout->itemAt(2);
    if (removeFromPolicy)
        m_policy->removeAt(2);
    if (removeFromLayout)
        m_mockLayout->removeAt(2);
    if (deleteItem)
        delete thirdItem;

    QCOMPARE(m_form->geometry(), QRectF(0, 0, 54, 54));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, 54, 54));

    qApp->processEvents();
    QCOMPARE(m_form->geometry(), QRectF(0, 0, 54, 54));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, 54, 54));
    for (int i = 0; i < 3; i++) {
        QCOMPARE(m_policy->itemAt(i)->geometry(), QRectF((i % 2) * 25 + 2, (i / 2) * 25 + 2, 25, 25));
        if (deleteItem || removeFromLayout || i != 2)
            QVERIFY(m_mockLayout->itemAt(i) == m_policy->itemAt(i));
        else
            QVERIFY(m_mockLayout->itemAt(3) == m_policy->itemAt(2));
    }

}
void Ut_MFlowLayoutPolicy::testBigItemInSmallLayout()
{
    m_form->setMaximumSize(QSizeF(40, 40));
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);
    qApp->processEvents();
    QCOMPARE(m_form->geometry(), QRectF(0, 0, 40, 40));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, 200, 200));
    QCOMPARE(m_mockItem100->geometry(), QRectF(0, 0, 100, 100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(0, 100, 200, 100));

    //Now try setting the layout's maximumSize to be smaller than its maximumSize
    //This causes contradictory behaviour, but it should not crash at least
    m_mockLayout->setMaximumSize(QSizeF(40, 40));
    qApp->processEvents();
    QCOMPARE(m_form->geometry(), QRectF(0, 0, 40, 40));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, 40, 40));
    QCOMPARE(m_mockItem100->geometry(), QRectF(0, 0, 40, 100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(0, 100, 40, 100));
}

void Ut_MFlowLayoutPolicy::testMarginsAndMinimumHeight()
{
    m_form->setMinimumWidth(128);
    m_form->setMaximumWidth(128);
    m_form->setContentsMargins(1, 0, 0, 0);
    // Create a child widget and put the child inside the parent
    for (int j = 0; j < 3; j++) {
        // Create a child widget and put the child inside the parent 1
        MockLayoutItem *childWidget = new MockLayoutItem;
        childWidget->setMinimumSize(64, 64);
        m_policy->addItem(childWidget);
    }
    qApp->processEvents();
    QCOMPARE(m_form->geometry(), QRectF(0, 0, 128, 64 * 3));
    QCOMPARE(m_mockLayout->geometry(), QRectF(1, 0, 127, 64 * 3));
}

void Ut_MFlowLayoutPolicy::testAddingRemovingAdding()
{
    new MBasicLayoutAnimation(m_mockLayout);
    QPointer<QGraphicsWidget> w1 = new QGraphicsWidget;
    QPointer<QGraphicsWidget> w2 = new QGraphicsWidget;
    QPointer<QGraphicsWidget> w3 = new QGraphicsWidget;
    QPointer<QGraphicsWidget> w4 = new QGraphicsWidget;
    w1->setMinimumSize(64, 64);
    w2->setMinimumSize(64, 64);
    w3->setMinimumSize(64, 64);
    w4->setMinimumSize(64, 64);
    m_policy->addItem(w1);
    m_policy->addItem(w2);
    m_policy->addItem(w3);
    qApp->processEvents();

    //Remove them all and readd them
    m_mockLayout->removeAt(0);
    m_mockLayout->removeAt(0);
    m_mockLayout->removeAt(0);
    QCOMPARE(m_mockLayout->count(), 0);
    QCOMPARE(m_policy->count(), 0);
    m_policy->addItem(w1);
    m_policy->addItem(w2);
    m_policy->addItem(w3);
    m_policy->addItem(w4);
    QCOMPARE(m_mockLayout->count(), 4);
    QCOMPARE(m_policy->count(), 4);
    qApp->processEvents();

    QCOMPARE(w1.isNull(), false);
    QCOMPARE(w2.isNull(), false);
    QCOMPARE(w3.isNull(), false);
    QCOMPARE(w4.isNull(), false);

    QCOMPARE(w1->isVisible(), true);
    QCOMPARE(w2->isVisible(), true);
    QCOMPARE(w3->isVisible(), true);
    QCOMPARE(w4->isVisible(), true);
}
void Ut_MFlowLayoutPolicy::testAlignment()
{
    //Add two items
    m_policy->addItem(m_mockItem100);
    m_mockItem200->setPreferredSize(200, 200);
    m_policy->addItem(m_mockItem200);

    m_form->setGeometry(QRectF(0, 0, 500, 500));

    // Alignment is to center by default
    QCOMPARE(m_policy->alignment(m_mockItem100), Qt::AlignCenter);
    QCOMPARE(m_policy->alignment(m_mockItem200), Qt::AlignCenter);
    qApp->processEvents();
    QCOMPARE(m_mockItem100->geometry(), QRectF(0, 50, 100, 100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(100, 0, 200, 200));

    m_policy->setAlignment(m_mockItem200, Qt::AlignTop | Qt::AlignLeft);
    QCOMPARE(m_policy->alignment(m_mockItem100), Qt::AlignCenter);
    QCOMPARE(m_policy->alignment(m_mockItem200), Qt::AlignTop | Qt::AlignLeft);
    qApp->processEvents();
    QCOMPARE(m_mockItem100->geometry(), QRectF(0, 50, 100, 100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(100, 0, 200, 200));

    m_policy->setAlignment(m_mockItem200, Qt::AlignCenter);
    QCOMPARE(m_policy->alignment(m_mockItem100), Qt::AlignCenter);
    QCOMPARE(m_policy->alignment(m_mockItem200), Qt::AlignCenter);
    qApp->processEvents();
    QCOMPARE(m_mockItem100->geometry(), QRectF(0, 50, 100, 100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(100, 0, 200, 200));

    m_policy->setAlignment(m_mockItem100, Qt::AlignTop | Qt::AlignLeft);
    QCOMPARE(m_policy->alignment(m_mockItem100), Qt::AlignTop | Qt::AlignLeft);
    QCOMPARE(m_policy->alignment(m_mockItem200), Qt::AlignCenter);
    qApp->processEvents();
    qDebug() << "alignment" << (m_policy->alignment(m_mockItem100) & Qt::AlignVCenter);
    QCOMPARE(m_mockItem100->geometry(), QRectF(0, 0, 100, 100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(100, 0, 200, 200));

    m_policy->setAlignment(m_mockItem100, Qt::AlignBottom | Qt::AlignLeft);
    QCOMPARE(m_policy->alignment(m_mockItem100), Qt::AlignBottom | Qt::AlignLeft);
    QCOMPARE(m_policy->alignment(m_mockItem200), Qt::AlignCenter);
    qApp->processEvents();
    QCOMPARE(m_mockItem100->geometry(), QRectF(0, 100, 100, 100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(100, 0, 200, 200));

}
QTEST_APPLESS_MAIN(Ut_MFlowLayoutPolicy)
