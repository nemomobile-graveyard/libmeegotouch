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

#include "ut_duiapplicationextensionmanager.h"
#include "duiapplicationextensionmanager.h"

#include <duiextensionhandle_stub.h>
#include <duiapplicationextensionmetadata_stub.h>
#include <duiwidget_stub.h>
#include <duiwidgetcontroller_stub.h>
#include <duitheme_stub.h>
#include "duifiledatastore_stub.h"
#include "duidataaccess_stub.h"

#include <QtTest/QtTest>

DuiWidgetModel::~DuiWidgetModel()
{
}

DuiExtensionHandleModel::~DuiExtensionHandleModel()
{
}

bool fileExists = false;
bool QFile::exists(const QString &fileName)
{
    Q_UNUSED(fileName);
    return fileExists;
}

// QPluginLoader stubs
QList<QObject*> gQPluginLoaderInstances;
QObject *QPluginLoader::instance()
{
    if (gQPluginLoaderInstances.count() > 0) {
        return gQPluginLoaderInstances.takeFirst();
    } else {
        return NULL;
    }
}

// QDir stubs
QStringList gDesktopEntryList;
QStringList QDir::entryList(const QStringList &nameFilters, Filters filters, SortFlags sort) const
{
    Q_UNUSED(nameFilters);
    Q_UNUSED(filters);
    Q_UNUSED(sort);

    return gDesktopEntryList;
}

bool QDir::exists() const
{
    return true;
}

const QString interfaceName("com.nokia.dui.core.ApplicationExtensionInterface/1.0");

void SignalListener::extensionInstantiated(DuiApplicationExtensionInterface *extension)
{
    instantiatedExtensions.append(extension);
}

void SignalListener::extensionRemoved(DuiApplicationExtensionInterface *extension)
{
    QString extensionName;
    GoodExtension* goodExtension = dynamic_cast<GoodExtension*>(extension);
    if (goodExtension != NULL) {
        extensionName = goodExtension->name;
    }
    removedExtensions.append(qMakePair(extension, extensionName));
}

void SignalListener::widgetCreated(DuiWidget* widget, DuiDataStore&)
{
    createdWidgets.append(widget);
}

void SignalListener::widgetRemoved(DuiWidget *widget)
{
    removedWidgets.append(widget);
}

bool GoodExtension::initialize()
{
    return success;
}

DuiWidget *GoodExtension::widget()
{
    return widget_;
}

// The test class
void Ut_DuiApplicationExtensionManager::init()
{
    gDesktopEntryList.clear();

    setupTestSubject();
    signalListener.instantiatedExtensions.clear();
    signalListener.removedExtensions.clear();
    signalListener.createdWidgets.clear();
    signalListener.removedWidgets.clear();

    extensions.clear();
    goodExtensionCount = 0;
}

void Ut_DuiApplicationExtensionManager::cleanup()
{
    delete manager;
    manager = NULL;
    fileExists = false;

    qDeleteAll(gQPluginLoaderInstances);
    gQPluginLoaderInstances.clear();
}

void Ut_DuiApplicationExtensionManager::initTestCase()
{
    manager = NULL;

    gDefaultDuiApplicationExtensionMetaDataStub.stubReset();
}

void Ut_DuiApplicationExtensionManager::cleanupTestCase()
{
}

void Ut_DuiApplicationExtensionManager::setupTestSubject()
{
    delete manager;
    manager = NULL;

    manager = new DuiApplicationExtensionManager(interfaceName);
    manager->init();
    connect(this, SIGNAL(directoryChanged(QString)), manager, SLOT(updateAvailableExtensions(QString)));

    connect(manager, SIGNAL(extensionInstantiated(DuiApplicationExtensionInterface *)), &signalListener, SLOT(extensionInstantiated(DuiApplicationExtensionInterface *)));
    connect(manager, SIGNAL(extensionRemoved(DuiApplicationExtensionInterface *)), &signalListener, SLOT(extensionRemoved(DuiApplicationExtensionInterface *)));
    connect(manager, SIGNAL(widgetCreated(DuiWidget*, DuiDataStore&)), &signalListener, SLOT(widgetCreated(DuiWidget*, DuiDataStore&)));
    connect(manager, SIGNAL(widgetRemoved(DuiWidget*)), &signalListener, SLOT(widgetRemoved(DuiWidget*)));
}

void Ut_DuiApplicationExtensionManager::setupGoodExtension(bool success, DuiWidget* widget, const QString &name)
{
    ++goodExtensionCount;
    GoodExtension *goodExtension = new GoodExtension;
    extensions.append(goodExtension);
    goodExtension->success = success;
    goodExtension->widget_ = widget;
    if (name.isEmpty()) {
        goodExtension->name = "Good extension " + QString::number(goodExtensionCount);
    } else {
        goodExtension->name = name;
    }
    gQPluginLoaderInstances.push_back(goodExtension);
}

void Ut_DuiApplicationExtensionManager::testMonitorAddExtension()
{
    gDesktopEntryList.clear();
    gDesktopEntryList << "test1.desktop";

    gDefaultDuiApplicationExtensionMetaDataStub.stubSetReturnValue("isValid", true);
    gDefaultDuiApplicationExtensionMetaDataStub.stubSetReturnValue("interface", interfaceName);
    gDefaultDuiApplicationExtensionMetaDataStub.stubSetReturnValue("extensionBinary", QString("test1"));
    setupGoodExtension(true, NULL, "1st extension");

    // Notify about a file system change
    emit directoryChanged(APPLICATION_EXTENSION_DATA_DIR);

    // Observe that the correct extension was added
    QCOMPARE(signalListener.instantiatedExtensions.count(), 1);
    QCOMPARE(signalListener.instantiatedExtensions.at(0), extensions.at(0));
}

void Ut_DuiApplicationExtensionManager::testMonitorRemoveExtension()
{
    gDesktopEntryList.clear();

    // Fire up a couple of extensions
    gDesktopEntryList << "test1.desktop" << "test2.desktop";

    gDefaultDuiApplicationExtensionMetaDataStub.stubSetReturnValue("isValid", true);
    gDefaultDuiApplicationExtensionMetaDataStub.stubSetReturnValue("interface", interfaceName);
    gDefaultDuiApplicationExtensionMetaDataStub.stubSetExtensionBinaryMultiple("test1");
    gDefaultDuiApplicationExtensionMetaDataStub.stubSetExtensionBinaryMultiple("test2");
    setupGoodExtension(true, NULL, "1st extension");
    setupGoodExtension(true, NULL, "2nd extension");

    setupTestSubject();

    // Remove one extension from the file system
    gDesktopEntryList.clear();
    gDesktopEntryList << "test1.desktop";

    // The test subject asks the library name from the removed meta data
    gDefaultDuiApplicationExtensionMetaDataStub.stubSetReturnValue("extensionBinary", QString("test2"));

    // Notify about a file system change
    emit directoryChanged(APPLICATION_EXTENSION_DATA_DIR);

    // Observe that the correct extension was removed
    QCOMPARE(signalListener.removedExtensions.count(), 1);
    QCOMPARE(signalListener.removedExtensions.at(0).second, QString("2nd extension"));
}

void Ut_DuiApplicationExtensionManager::testInstantiateInProcessExtensionWhichDoesNotExist()
{
    QCOMPARE(manager->instantiateInProcessExtension("test"), false);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
}

void Ut_DuiApplicationExtensionManager::testInstantiateInProcessExtensionWhichDoesNotImplementGivenInterface()
{
    gQPluginLoaderInstances.append(new QObject);
    QCOMPARE(manager->instantiateInProcessExtension("test"), false);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
}

void Ut_DuiApplicationExtensionManager::testInstantiateInProcessExtensionWhichDoesNotDeriveFromBaseInterface()
{
    gQPluginLoaderInstances.append(new BadExtension);
    QCOMPARE(manager->instantiateInProcessExtension("test"), false);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
}

void Ut_DuiApplicationExtensionManager::testInstantiateInProcessExtensionWhichFails()
{
    setupGoodExtension(false);
    QCOMPARE(manager->instantiateInProcessExtension("test"), false);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
}

void Ut_DuiApplicationExtensionManager::testInstantiateInProcessExtensionWhichSucceedsWithoutWidget()
{
    setupGoodExtension();
    QCOMPARE(manager->instantiateInProcessExtension("test"), true);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 1);
    QCOMPARE(signalListener.instantiatedExtensions.at(0), extensions.at(0));
    QCOMPARE(signalListener.instantiatedExtensions.at(0)->widget(), (DuiWidget *)NULL);
    QCOMPARE(signalListener.createdWidgets.count(), 0);
}

void Ut_DuiApplicationExtensionManager::testInstantiateInProcessExtensionWhichSucceedsWithWidget()
{
    DuiWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    QCOMPARE(manager->instantiateInProcessExtension("test"), true);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 1);
    QCOMPARE(signalListener.instantiatedExtensions.at(0), extensions.at(0));
    QCOMPARE(signalListener.instantiatedExtensions.at(0)->widget(), &extensionWidget);
    QCOMPARE(signalListener.createdWidgets.count(), 1);
    QCOMPARE(signalListener.createdWidgets.at(0), &extensionWidget);
}

void Ut_DuiApplicationExtensionManager::testInstantiateOutOfProcessExtension()
{
    const DuiApplicationExtensionMetaData metaData("test.desktop");
    manager->instantiateOutOfProcessExtension(metaData);
    QCOMPARE(signalListener.createdWidgets.count(), 1);
}

void Ut_DuiApplicationExtensionManager::testRemoveInProcessExtension()
{
    DuiWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    manager->instantiateInProcessExtension("test");

    manager->removeInProcessExtension("test");
    QCOMPARE(signalListener.removedExtensions.count(), 1);
    QCOMPARE(signalListener.removedExtensions.at(0).first, extensions.at(0));
}

void Ut_DuiApplicationExtensionManager::testRemoveNonExistentInProcessExtension()
{
    DuiWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    manager->instantiateInProcessExtension("test");

    manager->removeInProcessExtension("non-existent");
    QCOMPARE(signalListener.removedExtensions.count(), 0);
}

void Ut_DuiApplicationExtensionManager::testDisablingLoadingOfInProcessExtensions()
{
    delete manager;
    manager = new DuiApplicationExtensionManager(interfaceName, false);
    connect(manager, SIGNAL(extensionInstantiated(DuiApplicationExtensionInterface *)), &signalListener, SLOT(extensionInstantiated(DuiApplicationExtensionInterface *)));
    QString desktopFile("test.desktop");
    gDefaultDuiApplicationExtensionMetaDataStub.stubSetReturnValue("runnerBinary", QString(""));

    DuiWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);

    // Instantiating extension would not load any extension, since extension loading of in process is diabled
    manager->instantiateExtension(desktopFile);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
}

void Ut_DuiApplicationExtensionManager::testRequestForAllInProcessExtensionsReturnsAListOfExtensions()
{
    DuiWidget extensionWidget1;
    setupGoodExtension(true, &extensionWidget1);
    QCOMPARE(manager->instantiateInProcessExtension("test"), true);
    DuiWidget extensionWidget2;
    setupGoodExtension(true, &extensionWidget2);
    QCOMPARE(manager->instantiateInProcessExtension("testanother"), true);

    QList<DuiApplicationExtensionInterface*> listOfExtensions = manager->extensions();
    QCOMPARE(listOfExtensions.count(), 2);
    QCOMPARE(signalListener.instantiatedExtensions.at(0), listOfExtensions.at(0));
    QCOMPARE(signalListener.instantiatedExtensions.at(1), listOfExtensions.at(1));
}

void Ut_DuiApplicationExtensionManager::testAddWidgetInProcessExtensionWithoutWidget()
{
    setupGoodExtension();
    manager->instantiateInProcessExtension("test");
    QCOMPARE(signalListener.createdWidgets.count(), 0);
}

void Ut_DuiApplicationExtensionManager::testAddWidgetInProcessExtensionWithWidget()
{
    DuiWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    manager->instantiateInProcessExtension("test");
    QCOMPARE(signalListener.createdWidgets.count(), 1);
    QCOMPARE(signalListener.createdWidgets.at(0), &extensionWidget);
}

void Ut_DuiApplicationExtensionManager::testRemoveWidgetInProcessExtension()
{
    DuiWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    manager->instantiateInProcessExtension("test");
    manager->removeInProcessExtension("test");
    QCOMPARE(signalListener.removedWidgets.count(), 1);
    QCOMPARE(signalListener.removedWidgets.at(0), &extensionWidget);
}

void Ut_DuiApplicationExtensionManager::testRemoveOutOfProcessExtension()
{
    DuiApplicationExtensionMetaData metaData("test.desktop");
    manager->instantiateOutOfProcessExtension(metaData);
    DuiExtensionHandle *handle = manager->outOfProcessHandles.value("test.desktop");
    manager->removeOutOfProcessExtension(metaData.fileName());
    QCOMPARE(signalListener.removedWidgets.count(), 1);
    QCOMPARE(signalListener.removedWidgets.at(0), handle);
}

QTEST_MAIN(Ut_DuiApplicationExtensionManager)
