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

#include "ut_duiappletinventory.h"

#include <duiappletinventory.h>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiScene>
#include "ut_duiappletinventory.h"
#include "duiappletinstantiator.h"
#include "duimashupcanvas.h"
#include <duiappletbutton_stub.h>
#include <duidesktopentry_stub.h>
#include <duimashupcanvas_stub.h>
#include <duiappletinstancemanager_stub.h>
#include <duiappletid_stub.h>
#include <duiobjectmenu_stub.h>
#include "duiapplicationextensionarea_stub.h"

QStringList Ut_DuiAppletInventory::watchedDirectories;
QStringList Ut_DuiAppletInventory::appletList;
bool Ut_DuiAppletInventory::addPathCalled = false;
bool Ut_DuiAppletInventory::directoriesCalled = false;
bool Ut_DuiAppletInventory::pluginPathExists = true;
bool Ut_DuiAppletInventory::pluginPathIsReadable = true;
bool Ut_DuiAppletInventory::tooManyMonitoredPaths = false;

// Test applet source
QList<DuiWidget *> gTestAppletSourceCreatedWidgets;
bool gTestAppletSourceCreateWidget = true;
DuiWidget *TestAppletSource::widget()
{
    if (gTestAppletSourceCreateWidget) {
        DuiWidget *sourceWidget = new DuiWidget;
        gTestAppletSourceCreatedWidgets.append(sourceWidget);
        return sourceWidget;
    } else {
        return NULL;
    }
}

bool TestAppletSource::initialize(const QString &)
{
    return gTestAppletSourceCreateWidget;
}

// QPluginLoader stubs
QStringList gQPluginLoaderFileNames;
QPluginLoader::QPluginLoader(const QString &fileName, QObject *parent)
{
    Q_UNUSED(parent);
    // The plugin will load testability.so if libdui is compiled with flag TESTABILITY=on
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
    if (!Ut_DuiAppletInventory::tooManyMonitoredPaths)
        Ut_DuiAppletInventory::watchedDirectories.push_back(path);

    Ut_DuiAppletInventory::addPathCalled = true;
}

QStringList QFileSystemWatcher::directories() const
{
    Ut_DuiAppletInventory::directoriesCalled = true;

    return Ut_DuiAppletInventory::watchedDirectories;
}

bool QDir::exists() const
{
    return Ut_DuiAppletInventory::pluginPathExists;
}

bool QDir::isReadable() const
{
    return Ut_DuiAppletInventory::pluginPathIsReadable;
}

QStringList QDir::entryList(const QStringList &nameFilters, Filters /*filters*/, SortFlags /*sort*/) const
{
    QStringList filteredAppletList;
    foreach(QString filter, nameFilters) {
        if (filter == "*.desktop") {
            foreach(QString appletPath, Ut_DuiAppletInventory::appletList) {
                if (appletPath.right(8) == ".desktop") {
                    filteredAppletList.push_back(appletPath);
                }
            }
        }
    }
    return filteredAppletList;
}

// Unit test cases
void Ut_DuiAppletInventory::init()
{
    // Create DesktopPluginInventory instance
    inventory = new DuiAppletInventory();
    inventory->setMashupCanvas(*mashupCanvas);
    connect(this, SIGNAL(appletPathChanged(QString)), inventory, SLOT(appletPathChanged(QString)));
    connect(this, SIGNAL(instantiateAppletsFromPackage(QString)), inventory, SLOT(instantiateAppletsFromPackage(QString)));

    watchedDirectories.clear();
    appletList.clear();
    addPathCalled = false;
    directoriesCalled = false;
    pluginPathExists = true;
    pluginPathIsReadable = true;
    tooManyMonitoredPaths = false;
    gDuiAppletButtonStub->stubSetReturnValue("initialize", true);
    gQFileInfoExists = false;
    gQFileInfoIsFile = false;
    appWin->scene()->addItem(inventory);
}

void Ut_DuiAppletInventory::cleanup()
{
    // Destroy inventory (which will destroy the applet buttons created by it)
    delete inventory;
}

// Tests
void Ut_DuiAppletInventory::initTestCase()
{
    // Create a DuiApplication
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duiappletinventory" };
    app = new DuiApplication(argc, app_name);
    appWin = new DuiApplicationWindow;

    mashupCanvas = new DuiMashupCanvas("testcanvas");
}

void Ut_DuiAppletInventory::cleanupTestCase()
{
    // Destroy DuiApplication and desktop
    delete mashupCanvas;
    delete appWin;
    delete app;
}

void Ut_DuiAppletInventory::testSuccessfulInitialization()
{
    inventory->setEnabled(true);
    QVERIFY(addPathCalled);
    QVERIFY(directoriesCalled);
}

void Ut_DuiAppletInventory::testInitializationWithInvalidPluginPath()
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

void Ut_DuiAppletInventory::testStartWithExistingApplets()
{
    // Initialize path with existing applet metadata files
    appletList.push_back("test0.desktop");
    appletList.push_back("test1.desktop");

    // Make applet initialization succeed
    gDuiAppletButtonStub->stubSetReturnValue("initialize", true);

    // Start inventory
    inventory->setEnabled(true);

    // Make sure that the inventory added widgets for both applets in itself.
    QCOMPARE(inventory->model()->widgets().count(), 2);

    for (int i = 0; i < inventory->model()->widgets().count(); i++) {
        DuiAppletButton *b = dynamic_cast<DuiAppletButton *>(inventory->model()->widgets().at(i));

        // Each widget must be an DuiAppletButton
        QVERIFY(b != NULL);

        // Make sure that the applet was initialized with the correct name
        // QCOMPARE(b->metadataFilename(), QString().sprintf("%stest%d.desktop", APPLET_DATA "/", i));
    }
}

void Ut_DuiAppletInventory::testStartWithInvalidApplets()
{
    // Initialize path with existing applet metadata files
    appletList.push_back("test0.desktop");
    appletList.push_back("test1.desktop");

    // Make applet initialization fail
    gDuiAppletButtonStub->stubSetReturnValue("initialize", false);

    // Start inventory
    inventory->setEnabled(true);

    // Make sure that the inventory did not add invalid widgets.
    QCOMPARE(inventory->model()->widgets().count(), 0);
}

void Ut_DuiAppletInventory::testAppletAdd()
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

void Ut_DuiAppletInventory::testAppletRemove()
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

void Ut_DuiAppletInventory::testCategories_data()
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

void Ut_DuiAppletInventory::testCategories()
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
    gDuiDesktopEntryStub->stubSetReturnValue("categories", appletCategories);

    // Start inventory
    inventory->setEnabled(true);

    // There should be four widgets in the view
    QCOMPARE(inventory->model()->widgets().count(), initialWidgetCount);

    for (int i = 0; i < inventory->model()->widgets().count(); i++) {
        DuiAppletButton *b = dynamic_cast<DuiAppletButton *>(inventory->model()->widgets().at(i));

        // Each widget must be an DuiAppletButton
        QVERIFY(b != NULL);

        // Make sure that the applet was initialized with the correct name
//        QCOMPARE(b->metadataFilename(), QString().sprintf("%stest%d.desktop", APPLET_DATA"/", i));
    }

    // Reset the categories for the applet inventory
    inventory->setCategories(changedAppletInventoryCategories);

    // There should be three widgets in the view
    QCOMPARE(inventory->model()->widgets().count(), changedWidgetCount);

    for (int i = 0; i < inventory->model()->widgets().count(); i++) {
        DuiAppletButton *b = dynamic_cast<DuiAppletButton *>(inventory->model()->widgets().at(i));

        // Each widget must be an DuiAppletButton
        QVERIFY(b != NULL);

        // Make sure that the applet was initialized with the correct name
//        QCOMPARE(b->metadataFilename(), QString().sprintf("%stest%d.desktop", APPLET_DATA"/", i));
    }
}

void Ut_DuiAppletInventory::testEnabled()
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

void Ut_DuiAppletInventory::testOnlyDotDesktopFilesAreParsed()
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

QTEST_APPLESS_MAIN(Ut_DuiAppletInventory)
