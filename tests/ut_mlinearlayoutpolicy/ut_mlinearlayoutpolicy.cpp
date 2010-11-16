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

#include "ut_mlinearlayoutpolicy.h"

#include <mwidgetcontroller.h>
#include <mapplication.h>
#include <mlayout.h>
#include <mlinearlayoutpolicy.h>

#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QGraphicsWidget>

/**
    MockLayoutItem objects are layed out with the layout policy to test.
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


Ut_MLinearLayoutPolicy::Ut_MLinearLayoutPolicy()
    : m_mockLayout(0)
    , m_policy(0)
    , m_mockItem100(0)
    , m_mockItem200(0)
    , m_mockItem300(0)
    , m_mockItem400(0)
    , m_form(0)
{ }

Ut_MLinearLayoutPolicy::~Ut_MLinearLayoutPolicy()
{ }

MApplication *app;
void Ut_MLinearLayoutPolicy::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mlinearlayoutpolicy" };
    app = new MApplication(argc, argv);
    app->setLayoutDirection(Qt::LeftToRight);
}

void Ut_MLinearLayoutPolicy::cleanupTestCase()
{
    delete app;
}

void Ut_MLinearLayoutPolicy::init()
{
    Q_ASSERT(0 == m_mockLayout);
    Q_ASSERT(0 == m_policy);

    m_form = new QGraphicsWidget;
    m_mockLayout = new MLayout(m_form);
    m_policy = new MLinearLayoutPolicy(m_mockLayout, Qt::Horizontal);
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

    m_mockLayout->setAnimation(0); // turn off animation
}
void Ut_MLinearLayoutPolicy::cleanup()
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

void Ut_MLinearLayoutPolicy::testSpacing()
{
    // general spacing
    qreal general_spacing = 12.0;
    m_policy->setSpacing(general_spacing);
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);
    m_policy->addItem(m_mockItem300);
    m_policy->addItem(m_mockItem400);
    m_policy->setOrientation(Qt::Horizontal);
    m_mockLayout->activate();
    qApp->processEvents(); //let relayout happen

    QCOMPARE(m_form->geometry(), QRectF(0, 0, 100 + 200 + 300 + 400 + 3 * general_spacing, 400));
    QCOMPARE(m_mockLayout->geometry(), QRectF(0, 0, 100 + 200 + 300 + 400 + 3 * general_spacing, 400));

    // items moved to the correct place?
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(100 + general_spacing, 0),
             QSizeF(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(100 + 200 + 2 * general_spacing, 0),
             QSizeF(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(100 + 200 + 300 + 3 * general_spacing, 0),
             QSizeF(400.0, 400.0)));

    //Since we are horizontal, vertical spacing should do nothing
    m_policy->setVerticalSpacing(17); //trigger a relayout
    qApp->processEvents(); //let relayout happen

    //we should have no change
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(100 + general_spacing, 0),
             QSizeF(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(100 + 200 + 2 * general_spacing, 0),
             QSizeF(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(100 + 200 + 300 + 3 * general_spacing, 0),
             QSizeF(400.0, 400.0)));

    general_spacing = 0;
    m_policy->setHorizontalSpacing(general_spacing); //trigger a relayout
    qApp->processEvents(); //let relayout happen
    //set horizontal spacing to 0 - this should change the spacing
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(100 + general_spacing, 0),
             QSizeF(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(100 + 200 + 2 * general_spacing, 0),
             QSizeF(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(100 + 200 + 300 + 3 * general_spacing, 0),
             QSizeF(400.0, 400.0)));

    //now switch to vertical orientation.  It should now follow the vertical spacing of 17
    general_spacing = 17;
    m_policy->setOrientation(Qt::Vertical);
    qApp->processEvents(); //let relayout happen
    QCOMPARE(m_mockItem100->geometry(), QRectF(QPointF(0.0, 0.0), QSize(100.0, 100.0)));
    QCOMPARE(m_mockItem200->geometry(), QRectF(QPointF(0, 100 + general_spacing),
             QSizeF(200.0, 200.0)));
    QCOMPARE(m_mockItem300->geometry(), QRectF(QPointF(0, 100 + 200 + 2 * general_spacing),
             QSizeF(300.0, 300.0)));
    QCOMPARE(m_mockItem400->geometry(), QRectF(QPointF(0, 100 + 200 + 300 + 3 * general_spacing),
             QSizeF(400.0, 400.0)));
}

void Ut_MLinearLayoutPolicy::testStretch_data()
{
    //Run the test one with QGraphicsLinearLayout to check that the test itself is
    //correct, then again with libmeegotouch to check MLinearLayoutPolicy is correct
    QTest::addColumn<bool>("useQt");
    QTest::newRow("QGraphicsLinearLayout") << true;
    QTest::newRow("MLinearLayoutPolicy") << false;
}
void Ut_MLinearLayoutPolicy::testStretch()
{
    QFETCH(bool, useQt);
    QGraphicsLinearLayout *qt_layout(NULL);
    QGraphicsWidget *form(NULL);
    if (useQt) {
        form = new QGraphicsWidget;
        form->setMinimumSize(600, 600);
        form->setMaximumSize(600, 600);
        qt_layout = new QGraphicsLinearLayout(Qt::Horizontal, form);
        qt_layout->setSpacing(0);
        qt_layout->setContentsMargins(0, 0, 0, 0);
    } else {
        m_form->setMinimumSize(600, 600);
        m_form->setMaximumSize(600, 600);
        m_form->setContentsMargins(0, 0, 0, 0);
        m_mockLayout->setContentsMargins(0, 0, 0, 0);
        m_policy->setSpacing(0);
    }

    if (useQt) {
        qt_layout->addItem(m_mockItem100);
        qt_layout->addStretch(2);
        qt_layout->addItem(m_mockItem200);
        QCOMPARE(qt_layout->count(), 2);
        QVERIFY(qt_layout->itemAt(0) == m_mockItem100);
        QVERIFY(qt_layout->itemAt(1) == m_mockItem200);
    } else {
        m_policy->addItem(m_mockItem100);
        m_policy->addStretch(2);
        m_policy->addItem(m_mockItem200);
        QCOMPARE(m_policy->count(), 2);
        QVERIFY(m_policy->itemAt(0) == m_mockItem100);
        QVERIFY(m_policy->itemAt(1) == m_mockItem200);
    }
    if (useQt)
        qt_layout->activate();
    else
        m_mockLayout->activate();

    QCOMPARE(m_mockItem100->geometry(), QRectF(0, 0, 100, 100));
    QCOMPARE(m_mockItem200->geometry(), QRectF(400, 0, 200, 200));

    //Add a third item and check that it does go at the end
    if (useQt) {
        qt_layout->insertItem(2, m_mockItem300);
        QVERIFY(qt_layout->itemAt(0) == m_mockItem100);
        QVERIFY(qt_layout->itemAt(1) == m_mockItem200);
        QVERIFY(qt_layout->itemAt(2) == m_mockItem300);
    } else {
        m_policy->insertItem(2, m_mockItem300);
        QVERIFY(m_policy->itemAt(0) == m_mockItem100);
        QVERIFY(m_policy->itemAt(1) == m_mockItem200);
        QVERIFY(m_policy->itemAt(2) == m_mockItem300);
    }

    //Add a bunch of stretches
    if (useQt) {
        qt_layout->insertStretch(2);
        qt_layout->insertStretch(2);
        qt_layout->insertStretch(2);
    } else {
        m_policy->insertStretch(2);
        m_policy->insertStretch(2);
        m_policy->insertStretch(2);
    }

    //Even though we have a bunch of stretches, adding an item to position 3 should place
    //the new item after the mock item at 2
    if (useQt) {
        qt_layout->insertItem(3, m_mockItem400);
        QVERIFY(qt_layout->itemAt(0) == m_mockItem100);
        QVERIFY(qt_layout->itemAt(1) == m_mockItem200);
        QVERIFY(qt_layout->itemAt(2) == m_mockItem300);
        QVERIFY(qt_layout->itemAt(3) == m_mockItem400);
    } else {
        m_policy->insertItem(3, m_mockItem400);
        QVERIFY(m_policy->itemAt(0) == m_mockItem100);
        QVERIFY(m_policy->itemAt(1) == m_mockItem200);
        QVERIFY(m_policy->itemAt(2) == m_mockItem300);
        QVERIFY(m_policy->itemAt(3) == m_mockItem400);
    }

    if (useQt) {
        qt_layout->removeItem(m_mockItem200);
        qt_layout->insertItem(-1, m_mockItem200);
    } else {
        m_policy->removeItem(m_mockItem200);
        m_policy->insertItem(-1, m_mockItem200);
    }
}

void Ut_MLinearLayoutPolicy::testInsertingItems()
{
    m_form->setGeometry(0, 0, 700, 100); //4 items of size 100, plus 3 spacers that should become size 100
    QList<QGraphicsLayoutItem *> widgets;
    for (int i = 0; i < 4; i++) {
        QGraphicsLayoutItem *widget = new MockLayoutItem;
        widgets << widget;
        m_policy->addItem(widget);
    }
    QList<QGraphicsLayoutItem *> spacers;
    //Now add some spacer widgets
    for (int i = 0; i < 3; i++) {
        QGraphicsLayoutItem *spacer = new MockLayoutItem;
        spacers << spacer;

        int index = (i << 1) + 1;
        QCOMPARE(m_policy->itemAt(index - 1), widgets.at(i));
        QCOMPARE(m_policy->itemAt(index), widgets.at(i + 1));
        m_policy->insertItem(index, spacer);
        QCOMPARE(m_policy->count(), 5 + i);
        QCOMPARE(m_policy->itemAt(index - 1), widgets.at(i));
        QCOMPARE(m_policy->itemAt(index), spacer);
        QCOMPARE(m_policy->itemAt(index + 1), widgets.at(i + 1));
    }

    QCOMPARE(m_policy->itemAt(0), widgets.at(0));
    QCOMPARE(m_policy->itemAt(1), spacers.at(0));
    QCOMPARE(m_policy->itemAt(2), widgets.at(1));
    QCOMPARE(m_policy->itemAt(3), spacers.at(1));
    QCOMPARE(m_policy->itemAt(4), widgets.at(2));
    QCOMPARE(m_policy->itemAt(5), spacers.at(2));
    QCOMPARE(m_policy->itemAt(6), widgets.at(3));

}

void Ut_MLinearLayoutPolicy::testLayoutPositioning_data()
{
    QTest::addColumn<bool>("isActivePolicy"); //Whether to make m_policy the active policy
    QTest::addColumn<bool>("withStretchAtStart");
    QTest::addColumn<bool>("withStretchInMiddle");
    QTest::addColumn<bool>("withStretchAtEnd");

    for(int stretchAtStart = 0; stretchAtStart < 2; stretchAtStart++)
        for(int stretchInMiddle = 0; stretchInMiddle < 2; stretchInMiddle++)
            for(int stretchAtEnd = 0; stretchAtEnd < 2; stretchAtEnd++)
                for(int active = 0; active < 2; active++) {
                    QString description = (active?"active":"not active");
                    if (stretchAtStart || stretchInMiddle || stretchAtEnd)
                        description += QString(", with stretch at") + (stretchAtStart?" start":"") + (stretchInMiddle?" middle":"") + (stretchAtEnd?" end":"");
                    QTest::newRow(description.toLatin1()) << (bool)active << (bool)stretchAtStart << (bool)stretchInMiddle << (bool)stretchAtEnd;
                }
}
void Ut_MLinearLayoutPolicy::testLayoutPositioning()
{
    QFETCH(bool, isActivePolicy);
    QFETCH(bool, withStretchAtStart);
    QFETCH(bool, withStretchInMiddle);
    QFETCH(bool, withStretchAtEnd);
    MLinearLayoutPolicy *otherPolicy = new MLinearLayoutPolicy(m_mockLayout, Qt::Vertical);
    if(!isActivePolicy)
        otherPolicy->activate();

    m_policy->setOrientation(Qt::Horizontal);
    m_policy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    
    MWidgetController* c0 = new MWidgetController();
    MWidgetController* c1 = new MWidgetController();
    MWidgetController* c2 = new MWidgetController();
    MWidgetController* c3 = new MWidgetController();
    MWidgetController* c4 = new MWidgetController();
    MWidgetController* c5 = new MWidgetController();

    if(withStretchAtStart) {
        m_policy->addStretch();
        otherPolicy->addStretch();
    }
    m_policy->addItem(c0);
    otherPolicy->addItem(c0);
    QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
   
    if(withStretchInMiddle) {
        m_policy->addStretch();
        otherPolicy->addStretch();
    }

    
    m_policy->addItem(c1);
    otherPolicy->addItem(c1);
    if(isActivePolicy) {
        QCOMPARE((int)c0->layoutPosition(), (int)M::HorizontalLeftPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::HorizontalRightPosition);
    } else {
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
    }

    m_policy->addItem(c2);
    otherPolicy->addItem(c2);
    if(isActivePolicy) {
        QCOMPARE((int)c0->layoutPosition(), (int)M::HorizontalLeftPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::HorizontalRightPosition);
    } else {
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
    }

    if(withStretchInMiddle) {
        m_policy->addStretch();
        otherPolicy->addStretch();
    }

    m_policy->addItem(c3);
    otherPolicy->addItem(c3);
    if(isActivePolicy) {
        QCOMPARE((int)c0->layoutPosition(), (int)M::HorizontalLeftPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::HorizontalRightPosition);
    } else {
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
    }
    if(withStretchAtEnd) {
        m_policy->addStretch();
        otherPolicy->addStretch();
        if(isActivePolicy) {
            QCOMPARE((int)c0->layoutPosition(), (int)M::HorizontalLeftPosition);
            QCOMPARE((int)c1->layoutPosition(), (int)M::HorizontalCenterPosition);
            QCOMPARE((int)c2->layoutPosition(), (int)M::HorizontalCenterPosition);
            QCOMPARE((int)c3->layoutPosition(), (int)M::HorizontalRightPosition);
        } else {
            QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
            QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
            QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
            QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
        }
    }

    m_policy->insertItem(0, c4);
    otherPolicy->insertItem(0, c4);
    if(isActivePolicy) {
        QCOMPARE((int)c4->layoutPosition(), (int)M::HorizontalLeftPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::HorizontalRightPosition);
    } else {
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
    }

    m_policy->insertItem(3, c5);
    otherPolicy->insertItem(3, c5);
    if(isActivePolicy) {
        QCOMPARE((int)c4->layoutPosition(), (int)M::HorizontalLeftPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::HorizontalRightPosition);
    } else {
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
    }

    m_policy->setNotifyWidgetsOfLayoutPositionEnabled(false);
    QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
    QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
    QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
    QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
    QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
    QCOMPARE((int)c5->layoutPosition(), (int)M::DefaultPosition);


    m_policy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    if(isActivePolicy) {
        QCOMPARE((int)c4->layoutPosition(), (int)M::HorizontalLeftPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::HorizontalRightPosition);
    } else {
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
    }

    //Temporarily switch between policies
    if(isActivePolicy) {
        otherPolicy->activate();
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
        m_policy->activate();
        QCOMPARE((int)c4->layoutPosition(), (int)M::HorizontalLeftPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::HorizontalRightPosition);
    } else {
        m_policy->activate();
        QCOMPARE((int)c4->layoutPosition(), (int)M::HorizontalLeftPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::HorizontalCenterPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::HorizontalRightPosition);
        otherPolicy->activate();
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
    }

    m_policy->setOrientation(Qt::Vertical);
    if(isActivePolicy) {
        QCOMPARE((int)c4->layoutPosition(), (int)M::VerticalTopPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::VerticalBottomPosition);
    } else {
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c5->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
    }

    m_policy->removeAt(3);
    if(isActivePolicy) {
        QCOMPARE((int)c5->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c4->layoutPosition(), (int)M::VerticalTopPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::VerticalBottomPosition);
    } else {
        QCOMPARE((int)c5->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
    }

    m_policy->removeAt(4);
    if(isActivePolicy) {
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c4->layoutPosition(), (int)M::VerticalTopPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::VerticalBottomPosition);
    } else {
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
    }

    m_policy->removeAt(0);
    if(isActivePolicy) {
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::VerticalTopPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::VerticalCenterPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::VerticalBottomPosition);
    } else {
        QCOMPARE((int)c3->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c4->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
    }

    m_policy->removeAt(0);
    if(isActivePolicy) {
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::VerticalTopPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::VerticalBottomPosition);
    } else {
        QCOMPARE((int)c0->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
        QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);
    }

    m_policy->removeAt(0);
    QCOMPARE((int)c1->layoutPosition(), (int)M::DefaultPosition);
    QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);

    m_policy->removeAt(0);
    QCOMPARE((int)c2->layoutPosition(), (int)M::DefaultPosition);

    delete c0;
    delete c1;
    delete c2;
    delete c3;
    delete c4;
    delete c5;
}

void Ut_MLinearLayoutPolicy::testForLayoutCrashWithStretch()
{
    m_policy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    m_policy->addStretch();

    for(int i = 0; i < 4; i++)
    {
        MWidgetController* w = new MWidgetController();
        m_policy->addItem(w);
        m_policy->addStretch();
    }
}

void Ut_MLinearLayoutPolicy::testRtl()
{
    m_form->setLayoutDirection(Qt::LeftToRight);
    m_policy->setSpacing(0);
    m_policy->addItem(m_mockItem100);
    m_policy->addItem(m_mockItem200);
    m_policy->setOrientation(Qt::Horizontal);
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

void Ut_MLinearLayoutPolicy::testLayoutInLayoutRefresh()
{
    m_policy->setSpacing(0);
    m_policy->setOrientation(Qt::Vertical);
    m_mockLayout->activate();

    MWidget *widget = new MWidget;
    MLayout *layout = new MLayout(widget);
    layout->setContentsMargins(0,0,0,0);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    m_policy->addItem(widget);

    QGraphicsWidget *leftSpacerWithWidget = new QGraphicsWidget;
    policy->addItem(leftSpacerWithWidget);
    policy->addItem(m_mockItem100);
    policy->addItem(new QGraphicsWidget);
    policy->addItem(m_mockItem200);

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
QTEST_APPLESS_MAIN(Ut_MLinearLayoutPolicy)
