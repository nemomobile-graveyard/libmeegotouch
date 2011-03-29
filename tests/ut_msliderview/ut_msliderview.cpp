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

    m_seekbar = new MSeekBar();
    m_subject = new MSliderView(m_seekbar);
    m_seekbar->setView(m_subject);
    m_seekbar->setRange(0, 100);
}

void Ut_MSliderView::cleanupTestCase()
{
    delete m_seekbar;
    m_seekbar = 0;

    delete app;
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

    for (int i = 0; i <= 10; i++)
    {
        m_seekbar->setValue(i * 10);

        // check that the handle is positioned correctly from the left edge
        QCOMPARE(handle->geometry().x(),
                 groove->geometry().x() + ((float)m_seekbar->value()/100) * (groove->geometry().width()-handle->geometry().width()));
    }
}

QTEST_APPLESS_MAIN(Ut_MSliderView)
