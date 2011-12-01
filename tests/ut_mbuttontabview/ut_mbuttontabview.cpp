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

#include "ut_mbuttontabview.h"
#include "mbuttontabview.h"
#include "mbuttontabview_p.h"
#include "mbutton.h"
#include "mapplication.h"

#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>

MApplication *app;

void Ut_MButtonTabView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbuttontabview" };
    app = new MApplication(argc, app_name);
}

void Ut_MButtonTabView::cleanupTestCase()
{
    delete app;
}

void Ut_MButtonTabView::init()
{
    m_button = new MButton();
    m_subject = new MButtonTabView(m_button);
    m_button->setView(m_subject);
}

void Ut_MButtonTabView::cleanup()
{
    delete m_button;
}

void Ut_MButtonTabView::testClicked()
{
    // when button is not checkable, pressing does nothing
    QSignalSpy clickedSpy(m_button, SIGNAL(clicked()));
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);

    m_subject->mousePressEvent(&pressEvent);
    QCOMPARE(clickedSpy.count(), 0);

    // when the button is checkable, pressing causes a click
    m_button->setCheckable(true);
    m_subject->mousePressEvent(&pressEvent);
    QCOMPARE(clickedSpy.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_MButtonTabView)
