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

#include "ut_mappletinventory.h"

#include <mappletinventory.h>
#include <MApplication>
#include <MApplicationWindow>
#include <MScene>
#include "ut_mappletinventory.h"
#include "mappletinstantiator.h"
#include "mmashupcanvas.h"
#include <mappletbutton_stub.h>
#include <mdesktopentry_stub.h>
#include <mmashupcanvas_stub.h>
#include <mappletinstancemanager_stub.h>
#include <mappletid_stub.h>
#include <mobjectmenu_stub.h>
#include "mapplicationextensionarea_stub.h"

QStringList Ut_MAppletInventory::watchedDirectories;
QStringList Ut_MAppletInventory::appletList;
QList<MAppletInventoryInterface *> Ut_MAppletInventory::appletInstallationSource;
bool Ut_MAppletInventory::addPathCalled = false;
bool Ut_MAppletInventory::directoriesCalled = false;
bool Ut_MAppletInventory::pluginPathExists = true;
bool Ut_MAppletInventory::pluginPathIsReadable = true;
bool Ut_MAppletInventory::tooManyMonitoredPaths = false;

// Test applet source
TestAppletSource::TestAppletSource() :
        appletInventory(NULL),
        sourceWidget(NULL)
{
}

TestAppletSource::~TestAppletSource()
{
    delete appletInventory;
    delete sourceWidget;
}

QGraphicsWidget *TestAppletSource::widget()
{
    return sourceWidget = new QGraphicsWidget;
}

bool TestAppletSource::initialize(const QString &)
{
    return true;
}

void TestAppletSource::setMAppletInventoryInterface(MAppletInventoryInterface &installationSource)
{
    appletInventory = &installationSource;
    Ut_MAppletInventory::appletInstallationSource.append(&installationSource);
}

MAppletInventoryInterface *TestAppletSource::appletInventoryInterface() const
{
    return appletInventory;
}

void TestAppletSource::instantiateAppletsInPackage(const QString &packageName)
{
    this->appletInventoryInterface()->instantiateAppletsInPackage(packageName);
}

// QPluginLoader stubs
QStringList gQPluginLoaderFileNames;
QPluginLoader::QPluginLoader(const QString &fileName, QObject *parent)
{
    Q_UNUSED(parent);
    // The plugin will load testability.so if libmeegotouch is compiled with flag TESTABILITY=on
    if (!fileName.contains("testability")) {
        gQPluginLoaderFileNames.append(fileName);
    }
}

QPluginLoader::~QPluginLoader()
{
}

bool gQPluginLoaderInstanceCreate = true;
QObject *QPluginLoader::instance()
{
    return gQPluginLoaderInstanceCreate ? new TestAppletSource : NULL;
}

// QFileInfo stubs
bool gQFileInfoExists;
bool QFileInfo::exists() const
{
    return gQFileInfoExists;
}

bool gQFileInfoIsFile;
bool QFileInfo::isFile() const
{
    return gQFileInfoIsFile;
}

// Method mocks
void QFileSystemWatcher::addPath(const QString &path)
{
    if (!Ut_MAppletInventory::tooManyMonitoredPaths)
        Ut_MAppletInventory::watchedDirectories.push_back(path);

    Ut_MAppletInventory::addPathCalled = true;
}

QStringList QFileSystemWatcher::directories() const
{
    Ut_MAppletInventory::directoriesCalled = true;

    return Ut_MAppletInventory::watchedDirectories;
}

bool QDir::exists() const
{
    return Ut_MAppletInventory::pluginPathExists;
}

bool QDir::isReadable() const
{
    return Ut_MAppletInventory::pluginPathIsReadable;
}

QStringList QDir::entryList(const QStringList &nameFilters, Filters /*filters*/, SortFlags /*sort*/) const
{
    QStringList filteredAppletList;
    foreach(QString filter, nameFilters) {
        if (filter == "*.desktop") {
            foreach(QString appletPath, Ut_MAppletInventory::appletList) {
                if (appletPath.right(8) == ".desktop") {
                    filteredAppletList.push_back(appletPath);
                }
            }
        }
    }
    return filteredAppletList;
}

// Unit test cases
void Ut_MAppletInventory::init()
{
    // Create DesktopPluginInventory instance
    inventory = new MAppletInventory();
    inventory->setMashupCanvas(*mashupCanvas);
    connect(this, SIGNAL(appletPathChanged(QString)), inventory, SLOT(appletPathChanged(QString)));
    connect(this, SIGNAL(instantiateAppletsFromPackage(QString)), inventory, SLOT(instantiateAppletsFromPackage(QString)));

    watchedDirectories.clear();
    appletList.clear();
    appletInstallationSource.clear();
    addPathCalled = false;
    directoriesCalled = false;
    pluginPathExists = true;
    pluginPathIsReadable = true;
    tooManyMonitoredPaths = false;
    gMAppletButtonStub->stubSetReturnValue("initialize", true);
    gQFileInfoExists = false;
    gQFileInfoIsFile = false;
    appWin->scene()->addItem(inventory);
}

void Ut_MAppletInventory::cleanup()
{
    // Destroy inventory (which will destroy the applet buttons created by it)
    delete inventory;
}

// Tests
void Ut_MAppletInventory::initTestCase()
{
    // Create a MApplication
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mappletinventory" };
    app = new MApplication(argc, app_name);
    appWin = new MApplicationWindow;

    mashupCanvas = new MMashupCanvas("testcanvas");
}

void Ut_MAppletInventory::cleanupTestCase()
{
    // Destroy MApplication and desktop
    delete mashupCanvas;
    delete appWin;
    delete app;
}

void Ut_MAppletInventory::testSuccessfulInitialization()
{
    inventory->setEnabled(true);
    QVERIFY(addPathCalled);
    QVERIFY(directoriesCalled);
}

void Ut_MAppletInventory::testInitializationWithInvalidPluginPath()
{
    // Make the plugin path not exist; it's OK, there just won't be any applets
    pluginPathExists = false;
    inventory->setEnabled(true);
    QVERIFY(!addPathCalled);
    QVERIFY(!directoriesCalled);

    // Make the plugin path exist but not readable; it's OK, there just won't be any applets
    pluginPathExists = true;
    pluginPathIsReadable = false;
    inventory->setEnabled(true);
    QVERIFY(!addPathCalled);
    QVERIFY(!directoriesCalled);
}

void Ut_MAppletInventory::testStartWithExistingApplets()
{
    // Initialize path with existing applet metadata files
    appletList.push_back("test0.desktop");
    appletList.push_back("test1.desktop");

    // Make applet initialization succeed
    gMAppletButtonStub->stubSetReturnValue("initialize", true);

    // Start inventory
    inventory->setEnabled(true);

    // Make sure that the inventory added widgets for both applets in itself.
    QCOMPARE(inventory->model()->widgets().count(), 2);

    for (int i = 0; i < inventory->model()->widgets().count(); i++) {
        MAppletButton *b = dynamic_cast<MAppletButton *>(inventory->model()->widgets().at(i));

        // Each widget must be an MAppletButton
        QVERIFY(b != NULL);

        // Make sure that the applet was initialized with the correct name
        // QCOMPARE(b->metadataFilename(), QString().sprintf("%stest%d.desktop", APPLET_DATA "/", i));
    }
}

void Ut_MAppletInventory::testStartWithInvalidApplets()
{
    // Initialize path with existing applet metadata files
    appletList.push_back("test0.desktop");
    appletList.push_back("test1.desktop");

    // Make applet initialization fail
    gMAppletButtonStub->stubSetReturnValue("initialize", false);

    // Start inventory
    inventory->setEnabled(true);

    // Make sure that the inventory did not add invalid widgets.
    QCOMPARE(inventory->model()->widgets().count(), 0);
}

void Ut_MAppletInventory::testAppletAdd()
{
    // Start inventory
    inventory->setEnabled(true);

    // There should be no widgets in the view
    QCOMPARE(inventory->model()->widgets().count(), 0);

    // Add an applet metadata file
    appletList.push_back("test0.desktop");

    // Fake a directory change notification
    emit appletPathChanged(QDir(APPLET_DATA).absolutePath());

    // There should be one widget in the view
    QCOMPARE(inventory->model()->widgets().count(), 1);
}

void Ut_MAppletInventory::testAppletRemove()
{
    // Add applet metadata files
    appletList.push_back("test0.desktop");
    appletList.push_back("test1.desktop");
    appletList.push_back("test2.desktop");

    // Start inventory
    inventory->setEnabled(true);

    // There should be three widgets in the view
    QCOMPARE(inventory->model()->widgets().count(), 3);

    // "Delete" the second applet metadata file
    appletList.removeAt(1);

    // Spy on applet uninstallation signal
    QSignalSpy uninstallSpy(inventory, SIGNAL(appletUninstalled(QString)));

    // Fake a directory change notification
    emit appletPathChanged(QDir(APPLET_DATA).absolutePath());

    // There should be two widgets in the view
    QCOMPARE(inventory->model()->widgets().count(), 2);

    // The uninstall signal should have been called once...
    QCOMPARE(uninstallSpy.count(), 1);

    // ...with the argument test1.desktop
    QCOMPARE(uninstallSpy.takeFirst().at(0).toString(), QString(QDir(APPLET_DATA).absolutePath()) + "/test1.desktop");
}

void Ut_MAppletInventory::testCategories_data()
{
    QTest::addColumn<QStringList>("initialAppletInventoryCategories");
    QTest::addColumn<QStringList>("changedAppletInventoryCategories");
    QTest::addColumn<QStringList>("appletCategories");
    QTest::addColumn<int>("initialWidgetCount");
    QTest::addColumn<int>("changedWidgetCount");

    QStringList initialAppletInventoryCategories;
    initialAppletInventoryCategories.append("Utility");
    initialAppletInventoryCategories.append("Game");
    QStringList changedAppletInventoryCategories;
    changedAppletInventoryCategories.append("Utility");
    QStringList applet0Categories;
    applet0Categories.append("Utility");
    QStringList applet1Categories;
    applet1Categories.append("Utility");
    applet1Categories.append("Game");
    QStringList applet2Categories;
    applet2Categories.append("Utility");
    applet2Categories.append("Office");
    QStringList applet3Categories;
    applet3Categories.append("Game");
    QStringList applet4Categories;
    applet4Categories.append("Office");
    QTest::newRow("Includes one") << initialAppletInventoryCategories << changedAppletInventoryCategories << applet0Categories << 1 << 1;
    QTest::newRow("Includes one") << initialAppletInventoryCategories << changedAppletInventoryCategories << applet1Categories << 1 << 1;
    QTest::newRow("Includes one") << initialAppletInventoryCategories << changedAppletInventoryCategories << applet2Categories << 1 << 1;
    QTest::newRow("Includes one") << initialAppletInventoryCategories << changedAppletInventoryCategories << applet3Categories << 1 << 0;
    QTest::newRow("Includes one") << initialAppletInventoryCategories << changedAppletInventoryCategories << applet4Categories << 0 << 0;
}

void Ut_MAppletInventory::testCategories()
{
    QFETCH(QStringList, initialAppletInventoryCategories);
    QFETCH(QStringList, changedAppletInventoryCategories);
    QFETCH(QStringList, appletCategories);
    QFETCH(int, initialWidgetCount);
    QFETCH(int, changedWidgetCount);

    // Set the categories for the applet inventory
    inventory->setCategories(initialAppletInventoryCategories);

    // Add applet metadata files
    appletList.push_back("test0.desktop");

    // Set the categories of the applets
    gMDesktopEntryStub->stubSetReturnValue("categories", appletCategories);

    // Start inventory
    inventory->setEnabled(true);

    // There should be four widgets in the view
    QCOMPARE(inventory->model()->widgets().count(), initialWidgetCount);

    for (int i = 0; i < inventory->model()->widgets().count(); i++) {
        MAppletButton *b = dynamic_cast<MAppletButton *>(inventory->model()->widgets().at(i));

        // Each widget must be an MAppletButton
        QVERIFY(b != NULL);

        // Make sure that the applet was initialized with the correct name
//        QCOMPARE(b->metadataFilename(), QString().sprintf("%stest%d.desktop", APPLET_DATA"/", i));
    }

    // Reset the categories for the applet inventory
    inventory->setCategories(changedAppletInventoryCategories);

    // There should be three widgets in the view
    QCOMPARE(inventory->model()->widgets().count(), changedWidgetCount);

    for (int i = 0; i < inventory->model()->widgets().count(); i++) {
        MAppletButton *b = dynamic_cast<MAppletButton *>(inventory->model()->widgets().at(i));

        // Each widget must be an MAppletButton
        QVERIFY(b != NULL);

        // Make sure that the applet was initialized with the correct name
//        QCOMPARE(b->metadataFilename(), QString().sprintf("%stest%d.desktop", APPLET_DATA"/", i));
    }
}

void Ut_MAppletInventory::testEnabled()
{
    // The close button should not be visible by default
    QVERIFY(!inventory->model()->closeButtonVisible());

    // Enabling the inventory should show the close button
    inventory->setEnabled(true);
    QVERIFY(inventory->model()->closeButtonVisible());

    // Disabling the inventory should hide the close button
    inventory->setEnabled(false);
    QVERIFY(!inventory->model()->closeButtonVisible());
}

void Ut_MAppletInventory::testOnlyDotDesktopFilesAreParsed()
{
    // Add applet metadata files
    appletList.push_back("test0.desktop");
    appletList.push_back("noextension");
    appletList.push_back("test1.desktop");
    appletList.push_back("test2.junkextension");

    // Start inventory
    inventory->setEnabled(true);

    // There should be two widgets in the view
    QCOMPARE(inventory->model()->widgets().count(), 2);
}

void Ut_MAppletInventory::testInstantiateAppletsInPackage()
{
    // Start inventory with no source interface initially
    inventory->setEnabled(true);
    QVERIFY(appletInstallationSource.isEmpty());

    // Check that intallation source interface is correctly set by inventory
    QSignalSpy spy1(inventory, SIGNAL(hideAppletInventory()));
    TestAppletSource *source = new TestAppletSource;
    inventory->setMAppletInventoryInterface(source);
    QCOMPARE(appletInstallationSource.count(), 1);
    QCOMPARE(appletInstallationSource.at(0), source->appletInventoryInterface());

    //Check that applet inventory is closed when package is selected for installation
    source->instantiateAppletsInPackage("package");
    QCOMPARE(spy1.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_MAppletInventory)
