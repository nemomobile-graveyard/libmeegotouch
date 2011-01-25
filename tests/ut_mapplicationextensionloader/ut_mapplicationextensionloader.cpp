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

#include <QtTest/QtTest>
#include <QGraphicsWidget>
#include <mapplicationextensionloader.h>
#include <mapplicationextensioninterface.h>
#include <mapplicationextensionmetadata.h>
#include "ut_mapplicationextensionloader.h"

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

QGraphicsWidget *GoodExtension::widget()
{
    return widget_;
}

// MApplicationExtensionMetaData stubs
QString MApplicationExtensionMetaData::extensionBinary() const
{
    return "TestExtensionBinary";
}

QString MApplicationExtensionMetaData::interface() const
{
    return "com.meego.core.MApplicationExtensionInterface/1.0";
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

QLibrary::LoadHints gPluginLoaderHints;
void QPluginLoader::setLoadHints(QLibrary::LoadHints loadHints)
{
    gPluginLoaderHints = loadHints;
}

QString QPluginLoader::errorString() const
{
    return QString();
}

void Ut_MApplicationExtensionLoader::init()
{
    gQPluginLoaderFileName.clear();
    QPluginLoader_instance_return = NULL;
    SomeQObject_destructor_called = false;
    initializedInterface.clear();

    metadata = new MApplicationExtensionMetaData("testmetadata.desktop");
}

void Ut_MApplicationExtensionLoader::cleanup()
{
    delete metadata;
    metadata = NULL;
}

void Ut_MApplicationExtensionLoader::initTestCase()
{
    gPluginLoaderHints = 0;
}

void Ut_MApplicationExtensionLoader::cleanupTestCase()
{
}

void Ut_MApplicationExtensionLoader::testExtensionLoading()
{
    GoodExtension *goodExtension = new GoodExtension;
    goodExtension->success = true;
    QPluginLoader_instance_return = goodExtension;

    MApplicationExtensionInterface *extension = MApplicationExtensionLoader::loadExtension(*metadata);

    QVERIFY(extension != NULL);
    QCOMPARE(extension, dynamic_cast<MApplicationExtensionInterface *>(QPluginLoader_instance_return));
    QCOMPARE(gQPluginLoaderFileName, QString("TestExtensionBinary"));
    QCOMPARE(initializedInterface, QString("com.meego.core.MApplicationExtensionInterface/1.0"));
}

void Ut_MApplicationExtensionLoader::testExtensionLoadingFailNullExtensionObject()
{
    // Returns NULL from QPluginLoader::instance()
    MApplicationExtensionInterface *extension = MApplicationExtensionLoader::loadExtension(*metadata);
    QVERIFY(extension == NULL);
}

void Ut_MApplicationExtensionLoader::testExtensionLoadingFailWrongTypeExtensionObject()
{
    // Use an object that can't be cast to MExtensionInterface
    QPluginLoader_instance_return = new SomeQObject;
    MApplicationExtensionInterface *extension = MApplicationExtensionLoader::loadExtension(*metadata);
    QVERIFY(extension == NULL);

    // Ensure that the loader still deletes the object even though it was of a wrong type
    QCOMPARE(SomeQObject_destructor_called, true);
}

void Ut_MApplicationExtensionLoader::testExtensionLoadingFailWrongInterfaceExtensionObject()
{
    // Use an object that does not implement the expected interface
    QPluginLoader_instance_return = new BadExtension;
    MApplicationExtensionInterface *extension = MApplicationExtensionLoader::loadExtension(*metadata);
    QVERIFY(extension == NULL);
}

void Ut_MApplicationExtensionLoader::testExtensionLoadingUsesCorrectLoaderHints()
{
    GoodExtension *goodExtension = new GoodExtension;
    goodExtension->success = true;
    QPluginLoader_instance_return = goodExtension;

    MApplicationExtensionLoader::loadExtension(*metadata);

    QCOMPARE(gPluginLoaderHints, (QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint));
}

QTEST_MAIN(Ut_MApplicationExtensionLoader)
