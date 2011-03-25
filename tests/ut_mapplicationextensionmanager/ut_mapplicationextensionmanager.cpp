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

#include "ut_mapplicationextensionmanager.h"
#include "mapplicationextensionmanager.h"

#include <mextensionhandle_stub.h>
#include <mapplicationextensionmetadata_stub.h>
#include <mwidget_stub.h>
#include <mwidgetcontroller_stub.h>
#include <mtheme_stub.h>
#include "mfiledatastore_stub.h"
#include "mdataaccess_stub.h"

#include <QtTest/QtTest>

const MTheme::ViewType MWidgetController::defaultType = "default";

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

QLibrary::LoadHints gPluginLoaderHints;
void QPluginLoader::setLoadHints(QLibrary::LoadHints loadHints)
{
    gPluginLoaderHints = loadHints;
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

const QString interfaceName("com.meego.core.MApplicationExtensionInterface/1.0");

void SignalListener::extensionInstantiated(MApplicationExtensionInterface *extension)
{
    instantiatedExtensions.append(extension);
}

void SignalListener::extensionRemoved(MApplicationExtensionInterface *extension)
{
    QString extensionName;
    GoodExtension* goodExtension = dynamic_cast<GoodExtension*>(extension);
    if (goodExtension != NULL) {
        extensionName = goodExtension->name;
    }
    removedExtensions.append(qMakePair(extension, extensionName));
}

void SignalListener::widgetCreated(QGraphicsWidget* widget, MDataStore&)
{
    createdWidgets.append(widget);
}

void SignalListener::widgetRemoved(QGraphicsWidget *widget)
{
    removedWidgets.append(widget);
}

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

// The test class
void Ut_MApplicationExtensionManager::init()
{
    gDesktopEntryList.clear();

    setupTestSubject();
    signalListener.instantiatedExtensions.clear();
    signalListener.removedExtensions.clear();
    signalListener.createdWidgets.clear();
    signalListener.removedWidgets.clear();

    extensions.clear();
    goodExtensionCount = 0;
    initializedInterface.clear();
}

void Ut_MApplicationExtensionManager::cleanup()
{
    delete manager;
    manager = NULL;
    fileExists = false;

    qDeleteAll(gQPluginLoaderInstances);
    gQPluginLoaderInstances.clear();
}

void Ut_MApplicationExtensionManager::initTestCase()
{
    manager = NULL;

    gDefaultMApplicationExtensionMetaDataStub.stubReset();
    gDefaultMDesktopEntryStub.stubSetReturnValue("type", QString("X-MeeGoApplicationExtension"));

    gPluginLoaderHints = 0;
}

void Ut_MApplicationExtensionManager::cleanupTestCase()
{
}

void Ut_MApplicationExtensionManager::setupTestSubject(const QString &inProcessFilter, const QString &outOfProcessFilter,
                                                       const QStringList &order)
{
    delete manager;
    manager = new MApplicationExtensionManager(interfaceName);
    if (!inProcessFilter.isEmpty()) {
        manager->setInProcessFilter(QRegExp(inProcessFilter));
    }
    if (!outOfProcessFilter.isEmpty()) {
        manager->setOutOfProcessFilter(QRegExp(outOfProcessFilter));
    }
    if (!order.isEmpty()) {
        manager->setOrder(order);
    }
    manager->init();
    connect(this, SIGNAL(directoryChanged(QString)), manager, SLOT(updateAvailableExtensions(QString)));

    connect(manager, SIGNAL(extensionInstantiated(MApplicationExtensionInterface *)), &signalListener, SLOT(extensionInstantiated(MApplicationExtensionInterface *)));
    connect(manager, SIGNAL(extensionRemoved(MApplicationExtensionInterface *)), &signalListener, SLOT(extensionRemoved(MApplicationExtensionInterface *)));
    connect(manager, SIGNAL(widgetCreated(QGraphicsWidget*, MDataStore&)), &signalListener, SLOT(widgetCreated(QGraphicsWidget*, MDataStore&)));
    connect(manager, SIGNAL(widgetRemoved(QGraphicsWidget*)), &signalListener, SLOT(widgetRemoved(QGraphicsWidget*)));
    connect(this,
            SIGNAL(extensionChanged(
                       const MDesktopEntry &)),
            manager,
            SLOT(updateExtension(
                     const MDesktopEntry &)));
}

void Ut_MApplicationExtensionManager::setupGoodExtension(bool success, QGraphicsWidget* widget, const QString &name)
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

void Ut_MApplicationExtensionManager::testMonitorAddExtension()
{
    gDesktopEntryList.clear();
    gDesktopEntryList << "test1.desktop";

    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("isValid", true);
    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("interface", interfaceName);
    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("extensionBinary", QString("test1"));
    setupGoodExtension(true, NULL, "1st extension");

    // Notify about a file system change
    emit directoryChanged(APPLICATION_EXTENSION_DATA_DIR);

    // Observe that the correct extension was added
    QCOMPARE(signalListener.instantiatedExtensions.count(), 1);
    QCOMPARE(signalListener.instantiatedExtensions.at(0), extensions.at(0));
}

void Ut_MApplicationExtensionManager::testMonitorRemoveExtension()
{
    gDesktopEntryList.clear();

    // Fire up a couple of extensions
    gDesktopEntryList << "test1.desktop" << "test2.desktop";

    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("isValid", true);
    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("interface", interfaceName);
    gDefaultMApplicationExtensionMetaDataStub.stubSetExtensionBinaryMultiple("test1");
    gDefaultMApplicationExtensionMetaDataStub.stubSetExtensionBinaryMultiple("test2");
    setupGoodExtension(true, NULL, "1st extension");
    setupGoodExtension(true, NULL, "2nd extension");

    setupTestSubject();

    // Remove one extension from the file system
    gDesktopEntryList.clear();
    gDesktopEntryList << "test1.desktop";

    // The test subject asks the library name from the removed meta data
    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("extensionBinary", QString("test2"));

    // Notify about a file system change
    emit directoryChanged(APPLICATION_EXTENSION_DATA_DIR);

    // Observe that the correct extension was removed
    QCOMPARE(signalListener.removedExtensions.count(), 1);
    QCOMPARE(signalListener.removedExtensions.at(0).second, QString("2nd extension"));
}

void Ut_MApplicationExtensionManager::testMonitorRemoveExtensionWithTwoExtensionInstancesFromTheSameExtension()
{
    gDesktopEntryList.clear();

    // Fire up a couple of extensions
    gDesktopEntryList << "test1.desktop" << "test2.desktop";

    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("isValid", true);
    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("interface", interfaceName);
    gMApplicationExtensionMetaDataStub->stubSetReturnValue("extensionBinary", QString("test"));
    setupGoodExtension(true, NULL, "extension");
    // Put the same extension a second time to the list
    gQPluginLoaderInstances.append(gQPluginLoaderInstances.first());

    setupTestSubject();

    // Remove one extension from the file system
    gDesktopEntryList.clear();
    gDesktopEntryList << "test1.desktop";

    // Notify about a file system change
    emit directoryChanged(APPLICATION_EXTENSION_DATA_DIR);

    // Observe that the correct extension was removed
    QCOMPARE(signalListener.removedExtensions.count(), 1);
    QCOMPARE(signalListener.removedExtensions.at(0).second, QString("extension"));
}

void Ut_MApplicationExtensionManager::testInstantiateInProcessExtensionWhichDoesNotExist()
{
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData), false);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
}

void Ut_MApplicationExtensionManager::testInstantiateInProcessExtensionWhichDoesNotImplementGivenInterface()
{
    gQPluginLoaderInstances.append(new QObject);
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData), false);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
}

void Ut_MApplicationExtensionManager::testInstantiateInProcessExtensionWhichDoesNotDeriveFromBaseInterface()
{
    gQPluginLoaderInstances.append(new BadExtension);
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData), false);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
}

void Ut_MApplicationExtensionManager::testInstantiateInProcessExtensionWhichFails()
{
    setupGoodExtension(false);
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData), false);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
}

void Ut_MApplicationExtensionManager::testInstantiateInProcessExtensionWhichSucceedsWithoutWidget()
{
    setupGoodExtension();
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData), true);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 1);
    QCOMPARE(signalListener.instantiatedExtensions.at(0), extensions.at(0));
    QCOMPARE(signalListener.instantiatedExtensions.at(0)->widget(), (QGraphicsWidget *)NULL);
    QCOMPARE(signalListener.createdWidgets.count(), 0);
    QCOMPARE(initializedInterface, interfaceName);
}

void Ut_MApplicationExtensionManager::testInstantiateInProcessExtensionWhichSucceedsWithWidget()
{
    QGraphicsWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData), true);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 1);
    QCOMPARE(signalListener.instantiatedExtensions.at(0), extensions.at(0));
    QCOMPARE(signalListener.instantiatedExtensions.at(0)->widget(), &extensionWidget);
    QCOMPARE(signalListener.createdWidgets.count(), 1);
    QCOMPARE(signalListener.createdWidgets.at(0), &extensionWidget);
}

void Ut_MApplicationExtensionManager::testInstantiateOutOfProcessExtension()
{
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    manager->instantiateOutOfProcessExtension(metaData);
    QCOMPARE(signalListener.createdWidgets.count(), 1);
}

void Ut_MApplicationExtensionManager::testRemoveInProcessExtension()
{
    QGraphicsWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    gMApplicationExtensionMetaDataStub->stubSetReturnValue("extensionBinary", QString("test"));
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    manager->instantiateInProcessExtension(metaData);
    manager->removeInProcessExtension(*metaData);
    QCOMPARE(signalListener.removedExtensions.count(), 1);
    QCOMPARE(signalListener.removedExtensions.at(0).first, extensions.at(0));
}

void Ut_MApplicationExtensionManager::testRemoveNonExistentInProcessExtension()
{
    QGraphicsWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    const QString TEST_METADATA("test");
    const QString NONEXISTENT_METADATA("non-existent");
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    gMApplicationExtensionMetaDataStub->stubSetReturnValue(
        "extensionBinary", QString(TEST_METADATA));
    manager->instantiateInProcessExtension(metaData);
    const MApplicationExtensionMetaData
        nonexistentMetaData(NONEXISTENT_METADATA);
    gMApplicationExtensionMetaDataStub->stubSetReturnValue(
        "extensionBinary", QString(NONEXISTENT_METADATA));
    manager->removeInProcessExtension(nonexistentMetaData);
    QCOMPARE(signalListener.removedExtensions.count(), 0);
}

void Ut_MApplicationExtensionManager::testUpdateInProcessExtension()
{
    QGraphicsWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    manager->instantiateInProcessExtension(metaData);
    int createdWidgetCount = 1;
    int createdExtensionCount = 1;
    QCOMPARE(signalListener.createdWidgets.count(), createdWidgetCount);
    QCOMPARE(signalListener.instantiatedExtensions.count(),
             createdExtensionCount);
    setupGoodExtension(true, &extensionWidget);
    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("runnerBinary", QString(""));
    emit extensionChanged(*metaData);
    QCOMPARE(signalListener.removedExtensions.count(), 1);
    QCOMPARE(signalListener.removedWidgets.count(), 1);
    QCOMPARE(signalListener.createdWidgets.count(), createdWidgetCount + 1);
    QCOMPARE(signalListener.instantiatedExtensions.count(),
             createdExtensionCount + 1);
}

void Ut_MApplicationExtensionManager::testInProcessExtensionFiltering()
{
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test.desktop"));
    QGraphicsWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    // Test that not allowing test.desktop in-process but allowing it out-of-process does nothing
    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("runnerBinary", QString(""));
    setupTestSubject("$^", "^test.desktop$");
    manager->instantiateExtension(metaData);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 0);
    // Test that allowing test.desktop in-process but not allowing it out-of-process instantiates the extension
    setupTestSubject("^test.desktop$", "$^");
    manager->instantiateExtension(metaData);
    QCOMPARE(signalListener.instantiatedExtensions.count(), 1);
}

void Ut_MApplicationExtensionManager::testOutOfProcessExtensionFiltering()
{
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test.desktop"));

    // Test that not allowing test.desktop out-of-process but allowing it in-process does nothing
    gDefaultMApplicationExtensionMetaDataStub.stubSetReturnValue("runnerBinary", QString("test"));
    setupTestSubject("^test.desktop$", "$^");
    manager->instantiateExtension(metaData);
    QCOMPARE(signalListener.createdWidgets.count(), 0);

    // Test that allowing test.desktop out-of-process but not allowing it in-process instantiates the extension
    setupTestSubject("$^", "^test.desktop$");
    manager->instantiateExtension(metaData);
    QCOMPARE(signalListener.createdWidgets.count(), 1);
}

void Ut_MApplicationExtensionManager::verifyOrderCreatedInDataStore(int order)
{
    QVERIFY(gMFileDataStoreStub->stubCallCount("createValue") > 0);
    MethodCall *call = gMFileDataStoreStub->stubCallsTo("createValue").takeLast();
    QVERIFY(call->parameter<const QString>(0).endsWith("order"));
    QVERIFY(call->parameter<const QVariant>(1).canConvert(QVariant::Int));
    QCOMPARE(call->parameter<const QVariant>(1).toInt(), order);
}

void Ut_MApplicationExtensionManager::testOrdering()
{
    MWidget extensionWidget1;
    setupGoodExtension(true, &extensionWidget1);
    gMDesktopEntryStub->stubSetReturnValue("fileName", QString(APPLICATION_EXTENSION_DATA_DIR) + "/test");
    QSharedPointer<const MApplicationExtensionMetaData> metaData(new MApplicationExtensionMetaData(""));

    // order with one entry
    QStringList order;
    order << "test";
    setupTestSubject(QString(), QString(), order);
    setupGoodExtension(true, &extensionWidget1);
    QCOMPARE(manager->instantiateInProcessExtension(metaData), true);
    verifyOrderCreatedInDataStore(0);

    // order with three entries
    order.insert(0, "test2");
    order.insert(1, "");
    setupTestSubject(QString(), QString(), order);
    setupGoodExtension(true, &extensionWidget1);
    QCOMPARE(manager->instantiateInProcessExtension(metaData), true);
    verifyOrderCreatedInDataStore(2);
    // check that unordered entry goes to correct place
    gMDesktopEntryStub->stubSetReturnValue("fileName", QString(APPLICATION_EXTENSION_DATA_DIR) + "/unordered");
    setupGoodExtension(true, &extensionWidget1);
    QCOMPARE(manager->instantiateInProcessExtension(metaData), true);
    verifyOrderCreatedInDataStore(1);
    // check the first entry
    gMDesktopEntryStub->stubSetReturnValue("fileName", QString(APPLICATION_EXTENSION_DATA_DIR) + "/test2");
    setupGoodExtension(true, &extensionWidget1);
    QCOMPARE(manager->instantiateInProcessExtension(metaData), true);
    verifyOrderCreatedInDataStore(0);
}

void Ut_MApplicationExtensionManager::testRequestForAllInProcessExtensionsReturnsAListOfExtensions()
{
    QGraphicsWidget extensionWidget1;
    setupGoodExtension(true, &extensionWidget1);
    gDefaultMApplicationExtensionMetaDataStub.stubSetExtensionBinaryMultiple("test");
    gDefaultMApplicationExtensionMetaDataStub.stubSetExtensionBinaryMultiple("testanother");
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData), true);
    QGraphicsWidget extensionWidget2;
    setupGoodExtension(true, &extensionWidget2);
    QSharedPointer<const MApplicationExtensionMetaData> metaData2(
        new MApplicationExtensionMetaData("testanother"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData2), true);

    QList<MApplicationExtensionInterface*> listOfExtensions = manager->extensions();
    QCOMPARE(listOfExtensions.count(), 2);
    QVERIFY(signalListener.instantiatedExtensions.contains(
                listOfExtensions.at(0)));
    QVERIFY(signalListener.instantiatedExtensions.contains(
                listOfExtensions.at(1)));
}

void Ut_MApplicationExtensionManager::testAddWidgetInProcessExtensionWithoutWidget()
{
    setupGoodExtension();
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    manager->instantiateInProcessExtension(metaData);
    QCOMPARE(signalListener.createdWidgets.count(), 0);
}

void Ut_MApplicationExtensionManager::testAddWidgetInProcessExtensionWithWidget()
{
    QGraphicsWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    manager->instantiateInProcessExtension(metaData);
    QCOMPARE(signalListener.createdWidgets.count(), 1);
    QCOMPARE(signalListener.createdWidgets.at(0), &extensionWidget);
}

void Ut_MApplicationExtensionManager::testRemoveWidgetInProcessExtension()
{
    QGraphicsWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    manager->instantiateInProcessExtension(metaData);
    manager->removeInProcessExtension(*metaData);
    QCOMPARE(signalListener.removedWidgets.count(), 1);
    QCOMPARE(signalListener.removedWidgets.at(0), &extensionWidget);
}

void Ut_MApplicationExtensionManager::testRemoveOutOfProcessExtension()
{
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    manager->instantiateOutOfProcessExtension(metaData);
    MExtensionHandle *handle = manager->outOfProcessHandles.value(metaData.data());
    manager->removeOutOfProcessExtension(*metaData);
    QCOMPARE(signalListener.removedWidgets.count(), 1);
    QCOMPARE(signalListener.removedWidgets.at(0), handle);
}

void Ut_MApplicationExtensionManager::testInstantiatingTwoInProcessExtensionsFromTheSameSource()
{
    setupGoodExtension();
    // Put the same extension a second time to the list
    gQPluginLoaderInstances.append(gQPluginLoaderInstances.first());

    QSharedPointer<const MApplicationExtensionMetaData> metaData1(new MApplicationExtensionMetaData("test1"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData1), true);
    QSharedPointer<const MApplicationExtensionMetaData> metaData2(new MApplicationExtensionMetaData("test2"));
    QCOMPARE(manager->instantiateInProcessExtension(metaData2), true);

    QCOMPARE(signalListener.instantiatedExtensions.count(), 2);
    QCOMPARE(signalListener.instantiatedExtensions.at(0), extensions.at(0));
    QCOMPARE(signalListener.instantiatedExtensions.at(1), extensions.at(0));
}

void Ut_MApplicationExtensionManager::testInstantiatingInProcessExtensionUsesCorrectLoaderHints()
{
    QGraphicsWidget extensionWidget;
    setupGoodExtension(true, &extensionWidget);
    QSharedPointer<const MApplicationExtensionMetaData> metaData(
        new MApplicationExtensionMetaData("test"));
    manager->instantiateInProcessExtension(metaData);

    QCOMPARE(gPluginLoaderHints, (QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint));
}

QTEST_MAIN(Ut_MApplicationExtensionManager)
