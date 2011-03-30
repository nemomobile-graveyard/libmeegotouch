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

#include <QGraphicsWidget>
#include <MAction>
#include "mpannableviewport.h"
#include "mpannablewidget.h"
#include "mwidgetcontroller.h"
#include "mondisplaychangeevent.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mapplication.h"

/*   ## Removing Stubs ##
#include "mpannableviewport_stub.h"
#include "mpannablewidget_stub.h"
#include "mwidgetcontroller_stub.h"
*/
#include "mwidget.h"
#include "ut_mwidget.h"

void Ut_MWidget::init()
{
    widget = new MWidget;
}

void Ut_MWidget::cleanup()
{
    delete widget;
}

void Ut_MWidget::testShape_data()
{
    QTest::addColumn<QSizeF>("size");
    QTest::newRow("0") << QSizeF(50, 50);
    QTest::newRow("1") << QSizeF(5, 5);
}

void Ut_MWidget::testShape()
{
    QFETCH(QSizeF, size);
    widget->resize(size);
    widget->setWindowFrameMargins(0, 0, 0, 0);
    QCOMPARE(widget->shape().boundingRect(), QRectF(QPointF(), size));
}

void Ut_MWidget::testClearActions()
{
    for (int i = 0; i < 5; ++i)
        widget->addAction(new MAction("maction", widget));

    QCOMPARE(widget->actions().count(), 5);

    // empty action list, check
    widget->clearActions();
    QCOMPARE(widget->actions().count(), 0);
}

void Ut_MWidget::testIsOnDisplay_onView()
{
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    view.resize(500, 500);
    view.show();

    scene.addItem(widget);

    widget->show();
    widget->setGeometry(10, 10, 50, 50);

    QVERIFY(widget->isOnDisplay() == true);
}

void Ut_MWidget::testIsOnDisplay_offView()
{
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    view.resize(500, 500);
    view.show();

    scene.addItem(widget);

    widget->show();
    widget->setGeometry(-500, -500, 50, 50);

    if(widget->geometry() != QRectF(-500,-500,50,50)) {
        //Window managers do not have to obey geometry changes
        return;
    }

    QVERIFY(widget->isOnDisplay() == false);
}

void Ut_MWidget::testIsOnDisplay_widgetHidden()
{
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    view.resize(500, 500);
    view.show();

    scene.addItem(widget);

    widget->hide();
    widget->setGeometry(10, 10, 50, 50);

    QVERIFY(widget->isOnDisplay() == false);
}


void Ut_MWidget::testEnteredDisplay()
{
    m_dummySlotCalled = false;

    QRectF viewRect(0, 0, 864, 480);
    {
        MOnDisplayChangeEvent ev(MOnDisplayChangeEvent::FullyOffDisplay,
                                   viewRect);
        widget->event(&ev);
    }

    connect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));

    QVERIFY(m_dummySlotCalled == false);

    {
        MOnDisplayChangeEvent ev(MOnDisplayChangeEvent::FullyOnDisplay,
                                   viewRect);
        widget->event(&ev);
    }

    QVERIFY(m_dummySlotCalled == true);

    disconnect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));
}

void Ut_MWidget::testExitedDisplay()
{
    m_dummySlotCalled = false;

    QRectF viewRect(0, 0, 864, 480);
    {
        MOnDisplayChangeEvent ev(MOnDisplayChangeEvent::FullyOnDisplay,
                                   viewRect);
        widget->event(&ev);
    }

    QVERIFY(m_dummySlotCalled == false);

    connect(widget, SIGNAL(displayExited()), this, SLOT(dummySlot()));

    {
        MOnDisplayChangeEvent ev(MOnDisplayChangeEvent::FullyOffDisplay,
                                   viewRect);
        widget->event(&ev);
    }

    QVERIFY(m_dummySlotCalled == true);

    disconnect(widget, SIGNAL(displayExited()), this, SLOT(dummySlot()));
}

void Ut_MWidget::testShowHideSimple()
{
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    view.resize(500, 500);
    view.show();

    scene.addItem(widget);

    widget->setGeometry(10, 10, 50, 50);

    widget->hide();
    m_dummySlotCalled = false;
    connect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));
    widget->show();
    QVERIFY(m_dummySlotCalled == true);
    disconnect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));

    m_dummySlotCalled = false;
    connect(widget, SIGNAL(displayExited()), this, SLOT(dummySlot()));
    widget->hide();
    QVERIFY(m_dummySlotCalled == true);
    disconnect(widget, SIGNAL(displayExited()), this, SLOT(dummySlot()));

    /* displayEntered() should not be emitted when show() is called
       for an off-view widget */
    widget->setGeometry(-500, -500, 50, 50);
    m_dummySlotCalled = false;
    connect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));
    widget->show();
    QVERIFY(m_dummySlotCalled == false);
    disconnect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));
}

void Ut_MWidget::testShowHidePropagation()
{
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    view.resize(500, 500);
    view.show();

    MWidget* topLevel = new MWidget;
    MLayout *layout = new MLayout(topLevel);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->addItem(widget);

    scene.addItem(topLevel);
    topLevel->setGeometry(10, 10, 50, 50);

    topLevel->hide();
    m_dummySlotCalled = false;
    connect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));
    topLevel->show();
    QVERIFY(m_dummySlotCalled == true);
    disconnect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));

    m_dummySlotCalled = false;
    connect(widget, SIGNAL(displayExited()), this, SLOT(dummySlot()));
    topLevel->hide();
    QVERIFY(m_dummySlotCalled == true);
    disconnect(widget, SIGNAL(displayExited()), this, SLOT(dummySlot()));
}

void Ut_MWidget::dummySlot()
{
    m_dummySlotCalled = true;
}


int main(int argc, char *argv[])
{
    MApplication app(argc, argv, "ut_mwidget");
    Ut_MWidget tc;
    return QTest::qExec(&tc, argc, argv);
}
