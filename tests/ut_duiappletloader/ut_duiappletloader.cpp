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

#include "ut_duiappletloader.h"

#include <duiappletloader.h>
#include <DuiWidget>
#include <duidatastore.h>
#include <duiappletmetadata.h>
#include <duidataaccess.h>
#include "../stubs/mockdatastore.h"

#include <QtTest/QtTest>

class MockAppletSettingsInterface : public DuiDataAccess
{
public:
    virtual QVariant value(const QString &) const {
        return QVariant();
    }

    virtual bool setValue(const QString &, const QVariant &) {
        return true;
    }

    virtual QStringList allKeys() const {
        return QStringList();
    }

    virtual bool contains(const QString &) const {
        return true;
    }
};

Q_EXPORT_PLUGIN2(testapplet, TestApplet)

DuiWidget *gLastConstructedWidget = NULL;

DuiWidget *TestApplet::constructWidget(const DuiAppletMetaData &, DuiDataStore &, DuiDataAccess &)
{
    gLastConstructedWidget = new DuiWidget;
    return gLastConstructedWidget;
}

bool SomeQObject_destructor_called = false;
SomeQObject::~SomeQObject()
{
    SomeQObject_destructor_called = true;
}

// DuiAppletMetaData stubs
QString DuiAppletMetaData::appletBinary() const
{
    return "TestAppletBinary";
}

// QPluginLoader stubs
QString gQPluginLoaderFileName;
QPluginLoader::QPluginLoader(const QString &fileName, QObject *parent)
{
    Q_UNUSED(parent);

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
// NOTE: DuiAppletLoader deletes the returned value from this function so it musn't be deleted anywhere in this test case
QObject *QPluginLoader::instance()
{
    return QPluginLoader_instance_return;
}

void Ut_DuiAppletLoader::init()
{
    gLastConstructedWidget = NULL;
    gQPluginLoaderFileName.clear();
    QPluginLoader_instance_return = NULL;
    SomeQObject_destructor_called = false;

    metadata = new DuiAppletMetaData("testmetadata.desktop");
    dataStore = new MockDataStore;
    appletSettingsInterface = new MockAppletSettingsInterface;
}

void Ut_DuiAppletLoader::cleanup()
{
    QPluginLoader_instance_return = NULL;

    delete appletSettingsInterface;
    appletSettingsInterface = NULL;
    delete dataStore;
    dataStore = NULL;
    delete metadata;
    metadata = NULL;
}

void Ut_DuiAppletLoader::initTestCase()
{
}

void Ut_DuiAppletLoader::cleanupTestCase()
{
}

void Ut_DuiAppletLoader::testAppletLoading()
{
    QPluginLoader_instance_return = new TestApplet;

    DuiWidget *widget = DuiAppletLoader::loadApplet(*metadata, *dataStore, *appletSettingsInterface);

    QVERIFY(widget != NULL);
    QCOMPARE(widget, gLastConstructedWidget);
    QCOMPARE(gQPluginLoaderFileName, QString("TestAppletBinary"));
}

void Ut_DuiAppletLoader::testAppletLoadingFailNullAppletObject()
{
    // Returns NULL from QPluginLoader::instance()
    DuiWidget *widget = DuiAppletLoader::loadApplet(*metadata, *dataStore, *appletSettingsInterface);

    QVERIFY(widget == NULL);
}

void Ut_DuiAppletLoader::testAppletLoadingFailWrongTypeAppletObject()
{
    // Use an object that can't be cast to DuiAppletInterface
    QPluginLoader_instance_return = new SomeQObject;

    DuiWidget *widget = DuiAppletLoader::loadApplet(*metadata, *dataStore, *appletSettingsInterface);

    QVERIFY(widget == NULL);
    // Ensure that the loader still deletes the object even though it was of a wrong type
    QCOMPARE(SomeQObject_destructor_called, true);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletLoader)
