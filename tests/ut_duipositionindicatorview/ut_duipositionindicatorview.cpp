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

#include <QTest>
#include <duipositionindicatorview.h>
#include <duipositionindicator.h>
#include <duitheme.h>
#include <duiapplication.h>

#include "ut_duipositionindicatorview.h"

DuiApplication *app;

void Ut_DuiPositionIndicatorView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duipositionindicatorview" };
    app = new DuiApplication(argc, app_name);

    m_controller = new DuiPositionIndicator(0);
    m_subject = new DuiPositionIndicatorView(m_controller);
    m_controller->setView(m_subject);
}


void Ut_DuiPositionIndicatorView::cleanupTestCase()
{
    delete m_controller; // ~duiwidgetcontroller calls duiwidgetview.destroy() which deletes view (ie m_subject)
    m_controller = 0;
    m_subject = 0;
    delete app;
}

void Ut_DuiPositionIndicatorView::init()
{
}

void Ut_DuiPositionIndicatorView::cleanup()
{
}


QTEST_APPLESS_MAIN(Ut_DuiPositionIndicatorView)

