/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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
#include <DuiAction>
#include "duipannableviewport.h"
#include "duipannablewidget.h"
#include "duiwidgetcontroller.h"
#include "duiondisplaychangeevent.h"

/*   ## Removing Stubs ##
#include "duipannableviewport_stub.h"
#include "duipannablewidget_stub.h"
#include "duiwidgetcontroller_stub.h"
*/
#include "duiwidget.h"
#include "ut_duiwidget.h"

void Ut_DuiWidget::init()
{
    widget = new DuiWidget;
}

void Ut_DuiWidget::cleanup()
{
    delete widget;
}

void Ut_DuiWidget::testShape_data()
{
    QTest::addColumn<QSizeF>("size");
    QTest::newRow("0") << QSizeF(50, 50);
    QTest::newRow("1") << QSizeF(5, 5);
}

void Ut_DuiWidget::testShape()
{
    QFETCH(QSizeF, size);
    widget->resize(size);
    widget->setWindowFrameMargins(0, 0, 0, 0);
    QVERIFY(widget->shape().boundingRect() == QRectF(QPointF(), size));
}

void Ut_DuiWidget::testClearActions()
{
    for (int i = 0; i < 5; ++i)
        widget->addAction(new DuiAction("duiaction", widget));

    QVERIFY(widget->actions().count() == 5);

    // empty action list, check
    widget->clearActions();
    QVERIFY(widget->actions().count() == 0);
}

void Ut_DuiWidget::testIsOnDisplay_onView()
{
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    view.resize(500, 500);
    view.show();

    scene.addItem(widget);

    widget->show();
    widget->setGeometry(10, 10, 50, 50);

    QCOMPARE(widget->isOnDisplay(), true);

    scene.removeItem(widget);
}

void Ut_DuiWidget::testIsOnDisplay_offView()
{
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    view.resize(500, 500);
    view.show();

    scene.addItem(widget);

    widget->show();
    widget->setGeometry(-500, -500, 50, 50);

    QCOMPARE(widget->isOnDisplay(), false);

    scene.removeItem(widget);
}

void Ut_DuiWidget::testIsOnDisplay_widgetHidden()
{
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    view.resize(500, 500);
    view.show();

    scene.addItem(widget);

    widget->hide();
    widget->setGeometry(10, 10, 50, 50);

    QCOMPARE(widget->isOnDisplay(), false);

    scene.removeItem(widget);
}


void Ut_DuiWidget::testEnteredDisplay()
{
    m_dummySlotCalled = false;

    QRectF viewRect(0, 0, 864, 480);
    {
        DuiOnDisplayChangeEvent ev(DuiOnDisplayChangeEvent::FullyOffDisplay,
                                   viewRect);
        widget->event(&ev);
    }

    connect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));

    QVERIFY(m_dummySlotCalled == false);

    {
        DuiOnDisplayChangeEvent ev(DuiOnDisplayChangeEvent::FullyOnDisplay,
                                   viewRect);
        widget->event(&ev);
    }

    QVERIFY(m_dummySlotCalled == true);

    disconnect(widget, SIGNAL(displayEntered()), this, SLOT(dummySlot()));
}

void Ut_DuiWidget::testExitedDisplay()
{
    m_dummySlotCalled = false;

    QRectF viewRect(0, 0, 864, 480);
    {
        DuiOnDisplayChangeEvent ev(DuiOnDisplayChangeEvent::FullyOnDisplay,
                                   viewRect);
        widget->event(&ev);
    }

    QVERIFY(m_dummySlotCalled == false);

    connect(widget, SIGNAL(displayExited()), this, SLOT(dummySlot()));

    {
        DuiOnDisplayChangeEvent ev(DuiOnDisplayChangeEvent::FullyOffDisplay,
                                   viewRect);
        widget->event(&ev);
    }

    QVERIFY(m_dummySlotCalled == true);

    disconnect(widget, SIGNAL(displayExited()), this, SLOT(dummySlot()));
}

void Ut_DuiWidget::dummySlot()
{
    m_dummySlotCalled = true;
}

QTEST_MAIN(Ut_DuiWidget);
