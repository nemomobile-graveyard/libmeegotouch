/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>

#include <msliderview.h>
#include "views/msliderview_p.h"

#include <mseekbar.h>
#include <msliderstyle.h>
#include "ut_msliderview.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mapplicationpage.h"
#include "mscenemanager.h"
#include <mdebug.h>

MApplication *app;

Ut_MSliderView::Ut_MSliderView() :
        m_seekbar(0),
        m_subject(0)
{
}

void Ut_MSliderView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_msliderview" };
    app = new MApplication(argc, app_name);
}

void Ut_MSliderView::cleanupTestCase()
{
    delete app;
}

void Ut_MSliderView::init()
{
    app->setLayoutDirection(Qt::LeftToRight);

    m_seekbar = new MSeekBar();
    m_subject = new MSliderView(m_seekbar);
    m_seekbar->setView(m_subject);
    m_seekbar->setRange(0, 100);
}

void Ut_MSliderView::cleanup()
{
    delete m_seekbar;
    m_seekbar = 0;
}

void Ut_MSliderView::sliderResize()
{
    m_seekbar->setOrientation(Qt::Horizontal);
    //just give some size for slider groove
    QRectF rect = QRectF(0, 0, 400, 100);
    m_seekbar->setGeometry(rect);
    QRectF seekbarGeometry = m_seekbar->geometry();

    QCOMPARE(rect.width(), seekbarGeometry.width());

    //change size of slider groove
    rect = QRectF(0, 0, 600, 100);
    m_seekbar->setGeometry(rect);
    seekbarGeometry = m_seekbar->geometry();

    QCOMPARE(rect.width(), seekbarGeometry.width());

    m_seekbar->setOrientation(Qt::Vertical);
    //resize slider groove accordingly to vertical orientation
    rect = QRectF(0, 0, 100, 400);
    m_seekbar->setGeometry(rect);
    seekbarGeometry = m_seekbar->geometry();

    QCOMPARE(rect.height(), seekbarGeometry.height());

    //change size of slider groove
    rect = QRectF(0, 0, 100, 600);
    m_seekbar->setGeometry(rect);
    seekbarGeometry = m_seekbar->geometry();

    QCOMPARE(rect.height(), seekbarGeometry.height());
}

void Ut_MSliderView::sliderGrooveMargin()
{
    QRectF rect = QRectF(0, 0, 400, 100);
    m_seekbar->setGeometry(rect);

    MSliderGroove *groove = m_subject->d_func()->sliderGroove;
    qreal w = groove->sliderHandle->rect().width();

    // initial dimensions
    m_seekbar->setOrientation(Qt::Horizontal);
    groove->setIndicatorMargin(0);

    for (int i = 0; i < 4; i++)
    {
        // set groove-margin
        groove->setGrooveMargin(i * 10);
        groove->updateGeometry();

        qreal coord(0.0);

        m_seekbar->setValue(0);
        groove->updateHandlePos();
        coord = groove->sliderHandle->pos().x();
        // the left edge of the handle matches the left edge of the slider
        QCOMPARE(coord, 0.0);

        // the centre of the handle matches the centre of the slider
        m_seekbar->setValue(50);
        groove->updateHandlePos();
        coord = groove->sliderHandle->pos().x();
        QCOMPARE(coord + w/2.0, groove->geometry().width() / 2.0);

        // the right edge of the handle matches the right edge of the slider
        m_seekbar->setValue(100);
        groove->updateHandlePos();
        coord = groove->sliderHandle->pos().x();
        QCOMPARE(coord + w, groove->geometry().width());
    }
}

void Ut_MSliderView::sliderPositionByValue()
{
    MSliderGroove *groove = m_subject->d_func()->sliderGroove;
    MSliderHandle *handle = m_subject->d_func()->sliderGroove->sliderHandle;

    m_seekbar->setOrientation(Qt::Horizontal);
    app->processEvents();
    qreal handleY = handle->geometry().y();

    for (int i = 0; i <= 10; i++)
    {
        m_seekbar->setValue(i * 10);

        // check that the handle is positioned correctly from the left edge
        QCOMPARE(handle->geometry().x(),
                 groove->geometry().x() + ((float)m_seekbar->value()/100) * (groove->geometry().width()-handle->geometry().width()));
        QCOMPARE(handle->geometry().y(), handleY);
    }

    m_seekbar->setOrientation(Qt::Vertical);
    app->processEvents();
    qreal handleX = handle->geometry().x();

    for (int i = 0; i <= 10; i++)
    {
        m_seekbar->setValue(i * 10);

        // check that the handle is positioned correctly from the bottom edge
        QCOMPARE(handle->geometry().bottom(),
                 groove->geometry().height() - ((float)m_seekbar->value()/100) * (groove->geometry().height()-handle->geometry().height()));
        QCOMPARE(handle->geometry().x(), handleX);
    }
}

void Ut_MSliderView::testSetValueWhileAnimation()
{
    MSeekBar *seekbar = new MSeekBar();
    MSliderView *subject = new MSliderView(seekbar);
    seekbar->setView(subject);
    seekbar->setRange(0, 100);
    QRectF rect = QRectF(0, 0, 400, 100);
    seekbar->setGeometry(rect);
    seekbar->setValue(0);

    MApplicationWindow win;
    MApplicationPage page;
    page.setCentralWidget(seekbar);
    win.sceneManager()->appearSceneWindowNow(&page);
    win.show();

    // this mouse event should start the animation
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(300,50));
    subject->mousePressEvent(&pressEvent);

    // ensure that the animation is running
    QVERIFY(subject->d_func()->positionAnimation->state() == QAbstractAnimation::Running);

    // set a value while the animation is ongoing
    seekbar->setValue(1);

    // ensure that animation has been stopped and value and position are ok
    QVERIFY(subject->d_func()->positionAnimation->state() == QAbstractAnimation::Stopped);
    QCOMPARE(subject->model()->value(), 1);
    QCOMPARE(subject->d_func()->position, 1);
}

void Ut_MSliderView::testMinMaxIndicatorPosition()
{
    m_seekbar->setMaxLabel("max");
    m_seekbar->setMinLabel("min");
    m_seekbar->setMaxLabelVisible(true);
    m_seekbar->setMinLabelVisible(true);

    m_seekbar->setGeometry(QRectF(0, 0, 400, 100));
    m_seekbar->setOrientation(Qt::Horizontal);
    app->processEvents();

    QPointF minIndicatorCenter = m_subject->d_func()->minIndicator->geometry().center();
    QPointF maxIndicatorCenter = m_subject->d_func()->maxIndicator->geometry().center();
    QPointF grooveCenter = m_subject->d_func()->sliderGroove->geometry().center();

    //Minimum indicator is on the left side, groove is in the middle and maximum indicator is on the right
    QVERIFY(minIndicatorCenter.x() < grooveCenter.x());
    QVERIFY(grooveCenter.x() < maxIndicatorCenter.x());
    QVERIFY(minIndicatorCenter.y() == grooveCenter.y());
    QVERIFY(grooveCenter.y() == maxIndicatorCenter.y());

    m_seekbar->setOrientation(Qt::Vertical);
    app->processEvents();

    minIndicatorCenter = m_subject->d_func()->minIndicator->geometry().center();
    maxIndicatorCenter = m_subject->d_func()->maxIndicator->geometry().center();
    grooveCenter = m_subject->d_func()->sliderGroove->geometry().center();

    //Vertical slider has maximum indicator on the top, groove in the middle and minimum indicator at the bottom
    QVERIFY(minIndicatorCenter.y() > grooveCenter.y());
    QVERIFY(grooveCenter.y() > maxIndicatorCenter.y());
    QVERIFY(minIndicatorCenter.x() == grooveCenter.x());
    QVERIFY(grooveCenter.x() == maxIndicatorCenter.x());

    app->setLayoutDirection(Qt::RightToLeft);
    m_seekbar->setOrientation(Qt::Horizontal);

    MSliderView *subject = new MSliderView(m_seekbar);
    m_seekbar->setView(subject);
    app->processEvents();

    minIndicatorCenter = subject->d_func()->minIndicator->geometry().center();
    maxIndicatorCenter = subject->d_func()->maxIndicator->geometry().center();
    grooveCenter = subject->d_func()->sliderGroove->geometry().center();

    //In the right-to-left layout minimum indicator is on the right side,
    //groove is in the middle and maximum indicator is on the left
    QVERIFY(minIndicatorCenter.x() < grooveCenter.x());
    QVERIFY(grooveCenter.x() < maxIndicatorCenter.x());
    QVERIFY(minIndicatorCenter.y() == grooveCenter.y());
    QVERIFY(grooveCenter.y() == maxIndicatorCenter.y());
}

QTEST_APPLESS_MAIN(Ut_MSliderView)
