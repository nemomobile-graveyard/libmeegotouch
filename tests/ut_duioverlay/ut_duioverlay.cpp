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

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <duioverlay.h>
#include "duioverlay_p.h"
#include <duioverlayview.h>
#include "ut_duioverlay.h"
#include "duiapplication.h"
#include "duioverlaystyle.h"

void Ut_DuiOverlay::init()
{
    m_subject = new DuiOverlay();
}

void Ut_DuiOverlay::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

DuiApplication *app;

void Ut_DuiOverlay::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duioverlay" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiOverlay::cleanupTestCase()
{
    delete app;
    app = 0;
}

void Ut_DuiOverlay::constructor()
{
    QVERIFY(m_subject);

    // verify that model has been set
    QVERIFY(m_subject->model());
    // verify that view has been set
    QVERIFY(m_subject->view());

    // verify that the widget was instantiated
    QVERIFY(m_subject->widget());
}

void Ut_DuiOverlay::setWidget()
{
    DuiWidget *widget = new DuiWidget();
    m_subject->setWidget(widget);

    QCOMPARE(m_subject->widget(), widget);
    delete widget;
}

QTEST_APPLESS_MAIN(Ut_DuiOverlay)
