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

#include <QtTest/QtTest>
#include <DuiWidget>
#include <duiapplicationextensionloader.h>
#include <duiapplicationextensioninterface.h>
#include <duiapplicationextensionmetadata.h>
#include "ut_duiapplicationextensionloader.h"

// QObject
bool SomeQObject_destructor_called = false;
SomeQObject::~SomeQObject()
{
    SomeQObject_destructor_called = true;
}

// Good extension
QString initializedInterface;
bool GoodExtension::initialize(const QString &interface)
{
    initializedInterface = interface;
    return success;
}

DuiWidget *GoodExtension::widget()
{
    return widget_;
}

// DuiApplicationExtensionMetaData stubs
QString DuiApplicationExtensionMetaData::extensionBinary() const
{
    return "TestExtensionBinary";
}

QString DuiApplicationExtensionMetaData::interface() const
{
    return "com.nokia.dui.core.ApplicationExtensionInterface/1.0";
}

// QPluginLoader stubs
QString gQPluginLoaderFileName;
QPluginLoader::QPluginLoader(const QString &fileName, QObject *)
{
    gQPluginLoaderFileName = fileName;
}

QPluginLoader::~QPluginLoader()
{
}

void QPluginLoader::setFileName(const QString &fileName)
{
    gQPluginLoaderFileName = fileName;
}

QObject *QPluginLoader_instance_return = NULL;
QObject *QPluginLoader::instance()
{
    return QPluginLoader_instance_return;
}

void Ut_DuiApplicationExtensionLoader::init()
{
    gQPluginLoaderFileName.clear();
    QPluginLoader_instance_return = NULL;
    SomeQObject_destructor_called = false;
    initializedInterface.clear();

    metadata = new DuiApplicationExtensionMetaData("testmetadata.desktop");
}

void Ut_DuiApplicationExtensionLoader::cleanup()
{
    delete metadata;
    metadata = NULL;
}

void Ut_DuiApplicationExtensionLoader::initTestCase()
{
}

void Ut_DuiApplicationExtensionLoader::cleanupTestCase()
{
}

void Ut_DuiApplicationExtensionLoader::testExtensionLoading()
{
    GoodExtension *goodExtension = new GoodExtension;
    goodExtension->success = true;
    QPluginLoader_instance_return = goodExtension;

    DuiApplicationExtensionInterface *extension = DuiApplicationExtensionLoader::loadExtension(*metadata);

    QVERIFY(extension != NULL);
    QCOMPARE(extension, dynamic_cast<DuiApplicationExtensionInterface *>(QPluginLoader_instance_return));
    QCOMPARE(gQPluginLoaderFileName, QString("TestExtensionBinary"));
    QCOMPARE(initializedInterface, QString("com.nokia.dui.core.ApplicationExtensionInterface/1.0"));
}

void Ut_DuiApplicationExtensionLoader::testExtensionLoadingFailNullExtensionObject()
{
    // Returns NULL from QPluginLoader::instance()
    DuiApplicationExtensionInterface *extension = DuiApplicationExtensionLoader::loadExtension(*metadata);
    QVERIFY(extension == NULL);
}

void Ut_DuiApplicationExtensionLoader::testExtensionLoadingFailWrongTypeExtensionObject()
{
    // Use an object that can't be cast to DuiExtensionInterface
    QPluginLoader_instance_return = new SomeQObject;
    DuiApplicationExtensionInterface *extension = DuiApplicationExtensionLoader::loadExtension(*metadata);
    QVERIFY(extension == NULL);

    // Ensure that the loader still deletes the object even though it was of a wrong type
    QCOMPARE(SomeQObject_destructor_called, true);
}

void Ut_DuiApplicationExtensionLoader::testExtensionLoadingFailWrongInterfaceExtensionObject()
{
    // Use an object that does not implement the expected interface
    QPluginLoader_instance_return = new BadExtension;
    DuiApplicationExtensionInterface *extension = DuiApplicationExtensionLoader::loadExtension(*metadata);
    QVERIFY(extension == NULL);
}

QTEST_MAIN(Ut_DuiApplicationExtensionLoader)
