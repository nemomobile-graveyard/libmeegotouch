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

#include "ut_mapplicationextensionrunner.h"

#include <mextensionrunner_stub.h>
#include <mapplicationextensionrunner.h>
#include <mapplicationextensionmetadata_stub.h>
#include <mapplicationextensionloader_stub.h>
#include <mappletid_stub.h>
#include <MWindow>
#include <MApplication>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
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

bool TestExtension::initialize(const QString &interface)
{
    Q_UNUSED(interface);
    return true;
}

QGraphicsWidget *TestExtension::widget()
{
    return widget_;
}

// Unit test cases
void Ut_MApplicationExtensionRunner::init()
{
    m_instance = new MApplicationExtensionRunner();
    m_instance->parentWidget = new QGraphicsWidget;
    m_instance->parentWidget->setLayout(new QGraphicsLinearLayout(Qt::Vertical));
    m_instance->view = new MWindow;
    gMExtensionRunnerStub->stubSetReturnValue("init", true);

    meta = new MApplicationExtensionMetaData("ut_mextensionrunner/ut_mextensionrunner.desktop");
    gMApplicationExtensionMetaDataStub->stubSetReturnValue("isValid", true);

    extension = new TestExtension;
    extension->widget_ = NULL;
    gMApplicationExtensionLoaderStub->stubSetReturnValue("loadExtension", (MApplicationExtensionInterface *)extension);
}

void Ut_MApplicationExtensionRunner::cleanup()
{
    delete m_instance;
    m_instance = NULL;

    delete meta;
    meta = NULL;

    delete extension;
    extension = NULL;
}

void Ut_MApplicationExtensionRunner::initTestCase()
{
}

void Ut_MApplicationExtensionRunner::cleanupTestCase()
{
}

void Ut_MApplicationExtensionRunner::testInitializationSuccess()
{
    extension->widget_ = new QGraphicsWidget;
    QCOMPARE(m_instance->init("servername", *meta), true);
    delete extension->widget_;
}

void Ut_MApplicationExtensionRunner::testInitializationFailInvalidMetadata()
{
    gMApplicationExtensionMetaDataStub->stubSetReturnValue("isValid", false);
    QCOMPARE(m_instance->init("servername", *meta), false);
}

void Ut_MApplicationExtensionRunner::testInitializationFailConstructWidgetFails()
{
    gMApplicationExtensionLoaderStub->stubSetReturnValue("loadExtension", (MApplicationExtensionInterface *)NULL);
    QCOMPARE(m_instance->init("servername", *meta), false);
}

QTEST_MAIN(Ut_MApplicationExtensionRunner)
