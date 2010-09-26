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

#include "ut_mextensionareaview.h"

#include "mextensionarea.h"
#include <MApplication>
#include <MApplicationWindow>
#include <QGraphicsLinearLayout>
#include <QtTest/QtTest>

TestMExtensionAreaViewPrivate::TestMExtensionAreaViewPrivate()
{
    layout = new QGraphicsLinearLayout(Qt::Vertical);
}

static bool gPrivateDestructorCalled = false;
TestMExtensionAreaViewPrivate::~TestMExtensionAreaViewPrivate()
{
    gPrivateDestructorCalled = true;
}


void Ut_MExtensionAreaView::initTestCase()
{
    // MApplications must be created manually due to theme system changes
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_mextensionareaview" };
    app = new MApplication(argc, app_name);
    appWindow = new MApplicationWindow;
}

void Ut_MExtensionAreaView::cleanupTestCase()
{
    delete appWindow;
    delete app;
}

void Ut_MExtensionAreaView::init()
{
    gPrivateDestructorCalled = false;
    extensionArea = new MExtensionArea;
    m_subject_private = new TestMExtensionAreaViewPrivate;
    m_subject = new TestMExtensionAreaView(m_subject_private, extensionArea);
    extensionArea->setView(m_subject);
}

void Ut_MExtensionAreaView::cleanup()
{
    delete m_subject;
    
}

void Ut_MExtensionAreaView::testDestruction()
{
    MExtensionArea* area = new MExtensionArea;
    TestMExtensionAreaViewPrivate* view_private = 
        new TestMExtensionAreaViewPrivate;
    TestMExtensionAreaView* view = 
        new TestMExtensionAreaView(view_private, area);
    area->setView(view);
    delete view;
    QVERIFY(gPrivateDestructorCalled);
}

QTEST_APPLESS_MAIN(Ut_MExtensionAreaView)
