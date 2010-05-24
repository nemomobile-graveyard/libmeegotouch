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

#include "ut_mappletrunner.h"

#include <mextensionrunner_stub.h>
#include <mappletrunner.h>
#include <mappletmetadata_stub.h>
#include <mappletloader_stub.h>
#include <mappletid_stub.h>
#include <mappletsettings_stub.h>
#include <MWindow>
#include <MApplication>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QtTest/QtTest>

// MWindow stubs (to prevent crashing)
MWindow::MWindow(QWidget *parent)
    : QGraphicsView(parent),
      d_ptr(0)
{
}
MWindow::~MWindow()
{
}

// Unit test cases
void Ut_MAppletRunner::init()
{
    m_instance = new MAppletRunner();
    m_instance->parentWidget = new QGraphicsWidget;
    m_instance->parentWidget->setLayout(new QGraphicsLinearLayout(Qt::Vertical));
    m_instance->view = new MWindow;
    gMExtensionRunnerStub->stubSetReturnValue("init", true);

    meta = new MAppletMetaData("ut_mextensionrunner/ut_mextensionrunner.desktop");
    gMAppletMetaDataStub->stubSetReturnValue("isValid", true);

    widget = new QGraphicsWidget;
    gMAppletLoaderStub->stubSetReturnValue("loadApplet", widget);
}

void Ut_MAppletRunner::cleanup()
{
    delete m_instance;
    m_instance = NULL;

    delete meta;
    meta = NULL;

    delete widget;
    widget = NULL;
}

void Ut_MAppletRunner::initTestCase()
{
}

void Ut_MAppletRunner::cleanupTestCase()
{
}

void Ut_MAppletRunner::testInitializationSuccess()
{
    QCOMPARE(m_instance->init("servername", *meta, "appletId", "/tmp/applet.data"), true);
    QCOMPARE(gMAppletSettingsStub->stubLastCallTo("mAppletSettingsConstructor").parameter<QString>(0), QString("ut_mextensionrunner/ut_mextensionrunner.desktop"));
    QCOMPARE(gMAppletSettingsStub->stubLastCallTo("mAppletSettingsConstructor").parameter<QString>(1), QString("appletId"));
}

void Ut_MAppletRunner::testInitializationFailInvalidAppletId()
{
    QCOMPARE(m_instance->init("servername", *meta, "", "/tmp/applet.data"), false);
}

void Ut_MAppletRunner::testInitializationFailInvalidInstanceDataLocation()
{
    QCOMPARE(m_instance->init("servername", *meta, "appletId", ""), false);
}

void Ut_MAppletRunner::testInitializationFailInvalidMetadata()
{
    gMAppletMetaDataStub->stubSetReturnValue("isValid", false);
    QCOMPARE(m_instance->init("servername", *meta, "appletId", "/tmp/applet.data"), false);
}

void Ut_MAppletRunner::testInitializationFailConstructWidgetFails()
{
    gMAppletLoaderStub->stubSetReturnValue("loadApplet", (QGraphicsWidget *)NULL);
    QCOMPARE(m_instance->init("servername", *meta, "appletId", "/tmp/applet.data"), false);
}

QTEST_MAIN(Ut_MAppletRunner)
