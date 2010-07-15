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

#include <mseparatorview.h>

#include <mseparator.h>
#include <mseparatorstyle.h>
#include "ut_mseparatorview.h"
#include "mapplication.h"

MApplication *app;

Ut_MSeparatorView::Ut_MSeparatorView() :
        m_separator(0),
        m_subject(0)
{
}

void Ut_MSeparatorView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_msliderview" };
    app = new MApplication(argc, app_name);

    m_separator = new MSeparator();
    m_subject = new MSeparatorView(m_separator);
    m_separator->setView(m_subject);
}

void Ut_MSeparatorView::cleanupTestCase()
{
    delete m_separator;
    m_separator = 0;

    delete app;
}

void Ut_MSeparatorView::separatorResize()
{
    m_separator->setOrientation(Qt::Horizontal);
    //just give some size for slider groove
    QRectF rect = QRectF(0, 0, 400, 100);
    m_separator->setGeometry(rect);
    QRectF separatorGeometry = m_separator->geometry();

    QCOMPARE(rect.width(), separatorGeometry.width());

    //change size of slider groove
    rect = QRectF(0, 0, 600, 100);
    m_separator->setGeometry(rect);
    separatorGeometry = m_separator->geometry();

    QCOMPARE(rect.width(), separatorGeometry.width());

    m_separator->setOrientation(Qt::Vertical);
    //resize slider groove accordingly to vertical orientation
    rect = QRectF(0, 0, 100, 400);
    m_separator->setGeometry(rect);
    separatorGeometry = m_separator->geometry();

    QCOMPARE(rect.height(), separatorGeometry.height());

    //change size of slider groove
    rect = QRectF(0, 0, 100, 600);
    m_separator->setGeometry(rect);
    separatorGeometry = m_separator->geometry();

    QCOMPARE(rect.height(), separatorGeometry.height());
}

QTEST_APPLESS_MAIN(Ut_MSeparatorView)
