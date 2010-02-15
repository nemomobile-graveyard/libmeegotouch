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

#include "ut_duisettingslanguagewidget.h"
#include "duisettingslanguagewidget.h"

#include <QtTest/QtTest>
#include <DuiApplication>


void Ut_DuiSettingsLanguageWidget::initTestCase()
{
    // Create a DuiApplication
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_duisettingslanguagewidget" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiSettingsLanguageWidget::cleanupTestCase()
{
    // Destroy DuiApplication
    delete app;
}

void Ut_DuiSettingsLanguageWidget::init()
{
    m_subject = new DuiSettingsLanguageWidget;
    QCoreApplication::processEvents();
}

void Ut_DuiSettingsLanguageWidget::cleanup()
{
    delete m_subject;
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageWidget)
