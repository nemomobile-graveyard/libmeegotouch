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

#include "ut_duiapplicationextensionrunner.h"

#include <duiextensionrunner_stub.h>
#include <duiapplicationextensionrunner.h>
#include <duiapplicationextensionmetadata_stub.h>
#include <duiapplicationextensionloader_stub.h>
#include <duiappletid_stub.h>
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

bool TestExtension::initialize(const QString &interface)
{
    Q_UNUSED(interface);
    return true;
}

DuiWidget *TestExtension::widget()
{
    return widget_;
}

// Unit test cases
void Ut_DuiApplicationExtensionRunner::init()
{
    m_instance = new DuiApplicationExtensionRunner();
    m_instance->parentWidget = new QGraphicsWidget;
    m_instance->parentWidget->setLayout(new QGraphicsLinearLayout(Qt::Vertical));
    m_instance->view = new DuiWindow;
    gDuiExtensionRunnerStub->stubSetReturnValue("init", true);

    meta = new DuiApplicationExtensionMetaData("ut_duiextensionrunner/ut_duiextensionrunner.desktop");
    gDuiApplicationExtensionMetaDataStub->stubSetReturnValue("isValid", true);

    extension = new TestExtension;
    extension->widget_ = NULL;
    gDuiApplicationExtensionLoaderStub->stubSetReturnValue("loadExtension", (DuiApplicationExtensionInterface *)extension);
}

void Ut_DuiApplicationExtensionRunner::cleanup()
{
    delete m_instance;
    m_instance = NULL;

    delete meta;
    meta = NULL;

    delete extension;
    extension = NULL;
}

void Ut_DuiApplicationExtensionRunner::initTestCase()
{
}

void Ut_DuiApplicationExtensionRunner::cleanupTestCase()
{
}

void Ut_DuiApplicationExtensionRunner::testInitializationSuccess()
{
    extension->widget_ = new DuiWidget;
    QCOMPARE(m_instance->init("servername", *meta), true);
    delete extension->widget_;
}

void Ut_DuiApplicationExtensionRunner::testInitializationFailInvalidMetadata()
{
    gDuiApplicationExtensionMetaDataStub->stubSetReturnValue("isValid", false);
    QCOMPARE(m_instance->init("servername", *meta), false);
}

void Ut_DuiApplicationExtensionRunner::testInitializationFailConstructWidgetFails()
{
    gDuiApplicationExtensionLoaderStub->stubSetReturnValue("loadExtension", (DuiApplicationExtensionInterface *)NULL);
    QCOMPARE(m_instance->init("servername", *meta), false);
}

QTEST_MAIN(Ut_DuiApplicationExtensionRunner)
