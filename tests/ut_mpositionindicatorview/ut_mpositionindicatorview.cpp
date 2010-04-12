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

#include <QTest>
#include <mpositionindicatorview.h>
#include <mpositionindicator.h>
#include <mtheme.h>
#include <mapplication.h>

#include "ut_mpositionindicatorview.h"

MApplication *app;

void Ut_MPositionIndicatorView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mpositionindicatorview" };
    app = new MApplication(argc, app_name);

    m_controller = new MPositionIndicator(0);
    m_subject = new MPositionIndicatorView(m_controller);
    m_controller->setView(m_subject);
}


void Ut_MPositionIndicatorView::cleanupTestCase()
{
    delete m_controller; // ~mwidgetcontroller calls mwidgetview.destroy() which deletes view (ie m_subject)
    m_controller = 0;
    m_subject = 0;
    delete app;
}

void Ut_MPositionIndicatorView::init()
{
}

void Ut_MPositionIndicatorView::cleanup()
{
}


QTEST_APPLESS_MAIN(Ut_MPositionIndicatorView)

