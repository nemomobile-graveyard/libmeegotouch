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

#include "ut_duiappletrunner.h"

#include <duiextensionrunner_stub.h>
#include <duiappletrunner.h>
#include <duiappletmetadata_stub.h>
#include <duiappletloader_stub.h>
#include <duiappletid_stub.h>
#include <duiappletsettings_stub.h>
#include <DuiWidget>
#include <DuiWindow>
#include <DuiApplication>
#include <QGraphicsLinearLayout>
#include <QtTest/QtTest>

// DuiWindow stubs (to prevent crashing)
DuiWindow::DuiWindow(QWidget *parent)
    : QGraphicsView(parent),
      d_ptr(0)
{
}
DuiWindow::~DuiWindow()
{
}

// Unit test cases
void Ut_DuiAppletRunner::init()
{
    m_instance = new DuiAppletRunner();
    m_instance->parentWidget = new QGraphicsWidget;
    m_instance->parentWidget->setLayout(new QGraphicsLinearLayout(Qt::Vertical));
    m_instance->view = new DuiWindow;
    gDuiExtensionRunnerStub->stubSetReturnValue("init", true);

    meta = new DuiAppletMetaData("ut_duiextensionrunner/ut_duiextensionrunner.desktop");
    gDuiAppletMetaDataStub->stubSetReturnValue("isValid", true);

    widget = new DuiWidget;
    gDuiAppletLoaderStub->stubSetReturnValue("loadApplet", widget);
}

void Ut_DuiAppletRunner::cleanup()
{
    delete m_instance;
    m_instance = NULL;

    delete meta;
    meta = NULL;

    delete widget;
    widget = NULL;
}

void Ut_DuiAppletRunner::initTestCase()
{
}

void Ut_DuiAppletRunner::cleanupTestCase()
{
}

void Ut_DuiAppletRunner::testInitializationSuccess()
{
    QCOMPARE(m_instance->init("servername", *meta, "appletId", "/tmp/applet.data"), true);
    QCOMPARE(gDuiAppletSettingsStub->stubLastCallTo("duiAppletSettingsConstructor").parameter<QString>(0), QString("ut_duiextensionrunner/ut_duiextensionrunner.desktop"));
    QCOMPARE(gDuiAppletSettingsStub->stubLastCallTo("duiAppletSettingsConstructor").parameter<QString>(1), QString("appletId"));
}

void Ut_DuiAppletRunner::testInitializationFailInvalidAppletId()
{
    QCOMPARE(m_instance->init("servername", *meta, "", "/tmp/applet.data"), false);
}

void Ut_DuiAppletRunner::testInitializationFailInvalidInstanceDataLocation()
{
    QCOMPARE(m_instance->init("servername", *meta, "appletId", ""), false);
}

void Ut_DuiAppletRunner::testInitializationFailInvalidMetadata()
{
    gDuiAppletMetaDataStub->stubSetReturnValue("isValid", false);
    QCOMPARE(m_instance->init("servername", *meta, "appletId", "/tmp/applet.data"), false);
}

void Ut_DuiAppletRunner::testInitializationFailConstructWidgetFails()
{
    gDuiAppletLoaderStub->stubSetReturnValue("loadApplet", (DuiWidget *)NULL);
    QCOMPARE(m_instance->init("servername", *meta, "appletId", "/tmp/applet.data"), false);
}

QTEST_MAIN(Ut_DuiAppletRunner)
