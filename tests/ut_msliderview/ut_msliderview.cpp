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

#include <mseekbar.h>
#include <msliderstyle.h>
#include "ut_msliderview.h"
#include "mapplication.h"

MApplication *app;

void Ut_MSliderView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_msliderview" };
    app = new MApplication(argc, app_name);

    m_seekbar = new MSeekBar();
    m_subject = new MSliderView(m_seekbar);
    m_seekbar->setView(m_subject);
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

QTEST_APPLESS_MAIN(Ut_MSliderView)
