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

#include "ut_mappletloader.h"

#include <mappletloader.h>
#include <QGraphicsWidget>
#include <mdatastore.h>
#include <mappletmetadata.h>
#include <mdataaccess.h>
#include "../stubs/mockdatastore.h"

#include <QtTest/QtTest>

class MockAppletSettingsInterface : public MDataAccess
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

QGraphicsWidget *gLastConstructedWidget = NULL;

QGraphicsWidget *TestApplet::constructWidget(const MAppletMetaData &, MDataStore &, MDataAccess &)
{
    gLastConstructedWidget = new QGraphicsWidget;
    return gLastConstructedWidget;
}

bool SomeQObject_destructor_called = false;
SomeQObject::~SomeQObject()
{
    SomeQObject_destructor_called = true;
}

// MAppletMetaData stubs
QString MAppletMetaData::appletBinary() const
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
// NOTE: MAppletLoader deletes the returned value from this function so it musn't be deleted anywhere in this test case
QObject *QPluginLoader::instance()
{
    return QPluginLoader_instance_return;
}

QLibrary::LoadHints gPluginLoaderHints;
void QPluginLoader::setLoadHints(QLibrary::LoadHints loadHints)
{
    gPluginLoaderHints = loadHints;
}

void Ut_MAppletLoader::init()
{
    gLastConstructedWidget = NULL;
    gQPluginLoaderFileName.clear();
    QPluginLoader_instance_return = NULL;
    SomeQObject_destructor_called = false;

    metadata = new MAppletMetaData("testmetadata.desktop");
    dataStore = new MockDataStore;
    appletSettingsInterface = new MockAppletSettingsInterface;
}

void Ut_MAppletLoader::cleanup()
{
    QPluginLoader_instance_return = NULL;

    delete appletSettingsInterface;
    appletSettingsInterface = NULL;
    delete dataStore;
    dataStore = NULL;
    delete metadata;
    metadata = NULL;
}

void Ut_MAppletLoader::initTestCase()
{
    gPluginLoaderHints = 0;
}

void Ut_MAppletLoader::cleanupTestCase()
{
}

void Ut_MAppletLoader::testAppletLoading()
{
    QPluginLoader_instance_return = new TestApplet;

    QGraphicsWidget *widget = MAppletLoader::loadApplet(*metadata, *dataStore, *appletSettingsInterface);

    QVERIFY(widget != NULL);
    QCOMPARE(widget, gLastConstructedWidget);
    QCOMPARE(gQPluginLoaderFileName, QString("TestAppletBinary"));
}

void Ut_MAppletLoader::testAppletLoadingFailNullAppletObject()
{
    // Returns NULL from QPluginLoader::instance()
    QGraphicsWidget *widget = MAppletLoader::loadApplet(*metadata, *dataStore, *appletSettingsInterface);

    QVERIFY(widget == NULL);
}

void Ut_MAppletLoader::testAppletLoadingFailWrongTypeAppletObject()
{
    // Use an object that can't be cast to MAppletInterface
    QPluginLoader_instance_return = new SomeQObject;

    QGraphicsWidget *widget = MAppletLoader::loadApplet(*metadata, *dataStore, *appletSettingsInterface);

    QVERIFY(widget == NULL);
    // Ensure that the loader still deletes the object even though it was of a wrong type
    QCOMPARE(SomeQObject_destructor_called, true);
}

void Ut_MAppletLoader::testAppletLoadingUsesCorrectLoaderHints()
{
    QPluginLoader_instance_return = new TestApplet;
    MAppletLoader::loadApplet(*metadata, *dataStore, *appletSettingsInterface);

    QCOMPARE(gPluginLoaderHints, (QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint));
}

QTEST_APPLESS_MAIN(Ut_MAppletLoader)
