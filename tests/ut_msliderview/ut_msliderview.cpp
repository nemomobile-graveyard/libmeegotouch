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
    MSliderHandle *handle = m_subject->d_func()->sliderGroove->sliderHandle;

    // initial dimensions
    m_seekbar->setOrientation(Qt::Horizontal);
    groove->indicatorMargin = 0;

    for (int i = 0; i < 4; i++)
    {
        // set groove-margin
        groove->grooveMargin = i * 10;
        groove->updateGeometry();

        // set inital position
        m_seekbar->setValue(0);
        m_subject->setPosition(0);

        // check that the handle's center is groove->grooveMargin away from the left edge
        QCOMPARE(groove->geometry().x() + groove->grooveMargin,
                 handle->geometry().x() + handle->geometry().width() / 2);

        // center position is always the same, regardless of groove margin
        m_seekbar->setValue(50);
        m_subject->setPosition(50);
        QCOMPARE(groove->geometry().x() + groove->geometry().width() / 2,
                 handle->geometry().x() + handle->geometry().width() / 2);

        // set final position
        m_seekbar->setValue(100);
        m_subject->setPosition(100);

        // check that the handle's center is groove->grooveMargin away from the right edge
        QCOMPARE(groove->geometry().x() + groove->geometry().width() - groove->grooveMargin,
                 handle->geometry().x() + handle->geometry().width() / 2);
    }
}

QTEST_APPLESS_MAIN(Ut_MSliderView)
