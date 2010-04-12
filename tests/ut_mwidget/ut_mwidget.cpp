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
    QVERIFY(widget->shape().boundingRect() == QRectF(QPointF(), size));
}

void Ut_MWidget::testClearActions()
{
    for (int i = 0; i < 5; ++i)
        widget->addAction(new MAction("maction", widget));

    QVERIFY(widget->actions().count() == 5);

    // empty action list, check
    widget->clearActions();
    QVERIFY(widget->actions().count() == 0);
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

    QCOMPARE(widget->isOnDisplay(), true);

    scene.removeItem(widget);
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

    QCOMPARE(widget->isOnDisplay(), false);

    scene.removeItem(widget);
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

    QCOMPARE(widget->isOnDisplay(), false);

    scene.removeItem(widget);
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

void Ut_MWidget::dummySlot()
{
    m_dummySlotCalled = true;
}

QTEST_MAIN(Ut_MWidget);
