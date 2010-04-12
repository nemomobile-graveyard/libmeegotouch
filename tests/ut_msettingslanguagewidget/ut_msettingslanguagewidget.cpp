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

#include "ut_msettingslanguagewidget.h"
#include "msettingslanguagewidget.h"

#include <QtTest/QtTest>
#include <MApplication>


void Ut_MSettingsLanguageWidget::initTestCase()
{
    // Create a MApplication
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_msettingslanguagewidget" };
    app = new MApplication(argc, app_name);
}

void Ut_MSettingsLanguageWidget::cleanupTestCase()
{
    // Destroy MApplication
    delete app;
}

void Ut_MSettingsLanguageWidget::init()
{
    m_subject = new MSettingsLanguageWidget;
    QCoreApplication::processEvents();
}

void Ut_MSettingsLanguageWidget::cleanup()
{
    delete m_subject;
}

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageWidget)
