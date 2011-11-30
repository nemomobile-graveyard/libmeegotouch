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

#include "ut_mapplicationmenubutton.h"

#include "mapplication.h"
#include "mapplicationmenubutton.h"

MApplication *app;

void Ut_MApplicationMenuButton::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplicationmenubutton" };
    app = new MApplication(argc, app_name);
}

void Ut_MApplicationMenuButton::cleanupTestCase()
{
    delete app;
}

void Ut_MApplicationMenuButton::init()
{
    m_subject = new MApplicationMenuButton();
}

void Ut_MApplicationMenuButton::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MApplicationMenuButton::testProgressIndicator()
{
    m_subject->setProgressIndicatorVisible(true);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), true);

    m_subject->setProgressIndicatorVisible(false);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), false);
}

void Ut_MApplicationMenuButton::testArrowIcon()
{
    m_subject->setArrowIconVisible(true);
    QCOMPARE(m_subject->isArrowIconVisible(), true);

    m_subject->setArrowIconVisible(false);
    QCOMPARE(m_subject->isArrowIconVisible(), false);
}

QTEST_APPLESS_MAIN(Ut_MApplicationMenuButton)
