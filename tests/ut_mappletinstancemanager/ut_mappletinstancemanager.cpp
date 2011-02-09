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
#include <QDataStream>
#include <QLocalSocket>
#include "mdbusinterface.h"
#include <QDBusPendingCall>
#include <QDBusPendingReply>

#include "mapplication.h"
#include "mwindow.h"
#include "mappletpackagemetadata.h"
#include "mappletinstancemanager.h"
#include "mappletinstancedata.h"
#include "mappletmessage.h"
#include "mapplethandle_stub.h"
#include "mextensionhandle_stub.h"
#include "mextensionwatcher_stub.h"
#include "mbutton.h"
#include "mappletmetadata.h"
#include "ut_mappletinstancemanager.h"
#include "mfiledatastore.h"
#include "mscenemanager.h"
#include "mappletinstancemanagerdbusadaptor.h"
#include "mappletloader_stub.h"
#include "mappletid.h"
#include "mappletsettings_stub.h"
#include "morientationtracker_stub.h"
#include "mapplication.h"
#include "mockdatastore.h"

bool outOfProcess = false;
int appletValueIteration = 0;
QStringList removedFiles;
QString pluginLoaderFileName;

// QDBusInterface stubs (used by MRemoteAction)
MDBusInterface::MDBusInterface(const QString &service, const QString &path, const char *interface, const QDBusConnection &connection, QObject *parent) :
        QDBusAbstractInterface("service", "path", "interface", connection, parent)
{
    Ut_MAppletInstanceManager::callServiceName = service;
    Ut_MAppletInstanceManager::callObjectPath = path;
    Ut_MAppletInstanceManager::callInterface = interface;
}

MDBusInterface::~MDBusInterface()
{
}

// QDBusPendingCall stubs (used by MAppletInstanceManager)
QDBusPendingCall QDBusAbstractInterface::asyncCall(const QString &method,
        const QVariant &arg1,
        const QVariant &arg2,
        const QVariant &arg3,
        const QVariant &arg4,
        const QVariant &arg5,
        const QVariant &arg6,
        const QVariant &arg7,
        const QVariant &arg8)

{
    if (Ut_MAppletInstanceManager::captureCalls) {
        QList<QVariant> args;
        if (arg1.isValid()) {
            args.append(arg1);
            if (arg2.isValid()) {
                args.append(arg2);
                if (arg3.isValid()) {
                    args.append(arg3);
                    if (arg4.isValid()) {
                        args.append(arg4);
                        if (arg5.isValid()) {
                            args.append(arg5);
                            if (arg6.isValid()) {
                                args.append(arg6);
                                if (arg7.isValid()) {
                                    args.append(arg7);
                                    if (arg8.isValid()) {
                                        args.append(arg8);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Ut_MAppletInstanceManager::callMethods.append(method);
        Ut_MAppletInstanceManager::callArguments.append(args);
    }

    return QDBusPendingCall::fromCompletedCall(QDBusMessage());
}

bool QDBusPendingCall::isError() const
{
    return Ut_MAppletInstanceManager::callError;
}

// QDBusPendingReply stubs (used by MAppletInstanceManager)
void QDBusPendingReplyData::setMetaTypes(int, int const *)
{
}

QVariant QDBusPendingReplyData::argumentAt(int) const
{
    return Ut_MAppletInstanceManager::replyDataString;
}

// MAppletInstanceManagerDBusAdpator stubs
MAppletInstanceManagerDBusAdaptor::MAppletInstanceManagerDBusAdaptor(MAppletInstanceManager *parent) : QDBusAbstractAdaptor(parent)
{
}

MAppletInstanceManagerDBusAdaptor::~MAppletInstanceManagerDBusAdaptor()
{
}

void MAppletInstanceManagerDBusAdaptor::instantiateAppletFromPackage(const QString &, const QMap<QString, QVariant> &)
{
}

bool Ut_MAppletInstanceManager::captureCalls = false;
QString Ut_MAppletInstanceManager::callServiceName;
QString Ut_MAppletInstanceManager::callObjectPath;
QString Ut_MAppletInstanceManager::callInterface;
QList<QString> Ut_MAppletInstanceManager::callMethods;
QList< QList<QVariant> > Ut_MAppletInstanceManager::callArguments;
bool Ut_MAppletInstanceManager::callError = false;
QString Ut_MAppletInstanceManager::replyDataString;

// AppletInstanceCollection (connected to MAppletInstanceManager)
AppletInstanceCollection::~AppletInstanceCollection()
{
}

int AppletInstanceCollection::count() const
{
    return instances.count();
}

QGraphicsWidget *AppletInstanceCollection::at(int index) const
{
    return instances.at(index);
}

void AppletInstanceCollection::addInstance(QGraphicsWidget *widget, MDataStore &store)
{
    Q_UNUSED(store);
    instances.append(widget);
}

void AppletInstanceCollection::removeInstance(QGraphicsWidget *widget)
{
    int index = instances.indexOf(widget);
    if (index != -1) {
        instances.removeAt(index);
    }
}

// MAppletMetaData stubs (used by MAppletInstanceManager)
QString MAppletMetaData::appletBinary() const
{
    if (fileName() == "/tmp/testapplet1.desktop") {
        return APPLET_LIBS "/testapplet1.so";
    } else if (fileName() == "/tmp/testapplet2.desktop") {
        return APPLET_LIBS "/testapplet2.so";
    } else if (fileName() == "/tmp/testapplet3.desktop") {
        return APPLET_LIBS "/testapplet3.so";
    } else if (fileName() == "/tmp/testapplet4.desktop") {
        return APPLET_LIBS "/testapplet4.so";
    }

    return QString();
}


// QProcess stubs (used by MAppletHandle)
void QProcess::start(const QString &, const QStringList &, OpenMode)
{
}

// QLocalServer stubs (used by AppletCommunicator)
bool QLocalServer::waitForNewConnection(int, bool *)
{
    return true;
}

QString MDesktopEntry::value(const QString &key) const
{
    // These are used by MDesktopEntry and MAppletMetaData
    if (key == "Desktop Entry/Type")
        return QString("MApplet");
    else if (key == "Desktop Entry/Name")
        return QString("Calendar");
    else if (key == "Desktop Entry/Icon")
        return QString("W-Icon-calendar");
    else if (key == "Desktop Entry/Exec")
        return QString(outOfProcess ? "mappletrunner" : "");
    else if (key == "M/X-MApplet-Applet") {
        // Give a different name for the two applet binaries
        appletValueIteration++;
        if (appletValueIteration == 1)
            return QString("testapplet1.so");
        else
            return QString("testapplet4.so");
    } else
        return QString();
}

bool MDesktopEntry::contains(const QString &key) const
{
    // These are used by MDesktopEntry and MAppletMetaData
    return (key == "Desktop Entry/Type" ||
            key == "Desktop Entry/Name" ||
            key == "Desktop Entry/Icon" ||
            key == "Desktop Entry/Exec" ||
            key == "M/X-MApplet-Applet");
}

bool MAppletMetaData::isValid() const
{
    return true;
}

// MFileDataStore stubs
bool MFileDataStore_isReadable = true;
bool MFileDataStore::isReadable() const
{
    return MFileDataStore_isReadable;
}
bool MFileDataStore_isWritable = true;
bool MFileDataStore::isWritable() const
{
    return MFileDataStore_isWritable;
}

QMap<QString, QVariant> instantiateAppletPlaceHolderMetaData;

// QFile stubs (used by MAppletInstanceManager)
bool QFile::remove(const QString &fileName)
{
    removedFiles.append(fileName);
    return true;
}

QPair<QString, QString> fileCopy;
bool QFile::copy(const QString &fileName, const QString &newName)
{
    fileCopy.first  = fileName;
    fileCopy.second = newName;

    return true;
}

QStringList existingFiles;
bool QFile::exists(const QString &fileName)
{
    if (existingFiles.contains(fileName))
        return true;

    // Do what the real QFile::exists(const QString&) does
    return QFileInfo(fileName).exists();
}

bool QFile::exists() const
{
    return QFile::exists(fileName());
}

// QDir stubs (used by MAppletInstanceManager)
bool dataDirectoryExists = false;
bool QDir::exists(const QString &fileName) const
{
    QString dataDirectoryName = QDir::homePath() + "/.config/ut_mappletinstancemanager/";

    if (fileName == dataDirectoryName) {
        return dataDirectoryExists;
    }

    return true;
}

QString dirMkpath;
bool QDir::mkpath(const QString &dirPath) const
{
    dirMkpath = dirPath;
    return true;
}

MTestAppletInstanceManager::MTestAppletInstanceManager(const QString &identifier, MDataStore *dataStore) :
    MAppletInstanceManager(identifier, dataStore)
{
}

// The test class
void Ut_MAppletInstanceManager::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mappletinstancemanager" };
    app = new MApplication(argc, app_name);
}

void Ut_MAppletInstanceManager::cleanupTestCase()
{
    delete app;
}

void Ut_MAppletInstanceManager::init()
{
    // Initialize default data store with data
    defaultDataStore = new MockDataStore();
    defaultDataStore->createValue(QString("1/desktopFile"), QVariant("/tmp/testapplet1.desktop"));
    defaultDataStore->createValue(QString("1/title"), QVariant("Title 1"));
    defaultDataStore->createValue(QString("1/sizeLandscape"), QVariant("101 201"));
    defaultDataStore->createValue(QString("1/sizePortrait"), QVariant("101 201"));
    defaultDataStore->createValue(QString("3/foo"), QVariant("bar"));
    defaultDataStore->createValue(QString("4/desktopFile"), QVariant("/tmp/testapplet4.desktop"));
    defaultDataStore->createValue(QString("4/title"), QVariant("Title 4"));
    defaultDataStore->createValue(QString("4/sizeLandscape"), QVariant("104 204"));
    defaultDataStore->createValue(QString("4/sizePortrait"), QVariant("104 204"));
    defaultDataStore->createValue(QString("5/desktopFile"), QVariant("/tmp/testapplet5.desktop"));
    defaultDataStore->createValue(QString("5/packageName"), QVariant("/tmp/testapplet5.deb"));
    defaultDataStore->createValue(QString("5/title"), QVariant("Title 5"));
    defaultDataStore->createValue(QString("5/sizeLandscape"), QVariant("105 205"));
    defaultDataStore->createValue(QString("5/sizePortrait"), QVariant("105 205"));

    existingFiles.clear();
    existingFiles.append("/tmp/testapplet1.desktop");
    existingFiles.append("/tmp/testapplet4.desktop");

    // Create applet instance manager under test
    manager = new MTestAppletInstanceManager("testmanager", defaultDataStore);

    removedFiles.clear();

    appletValueIteration = 0;
    outOfProcess = false;

    MFileDataStore_isReadable = true;
    MFileDataStore_isWritable = true;

    gMAppletSettingsStub->stubReset();
    gMAppletHandleStub->stubReset();
    gMExtensionHandleStub->stubReset();
    gMExtensionHandleStub->stubSetReturnValue("state", MExtensionHandleModel::RUNNING);

    dataDirectoryExists = false;
    fileCopy.first = QString();
    fileCopy.second = QString();
    dirMkpath = QString();
    instantiateAppletPlaceHolderMetaData.clear();

    captureCalls = false;
    callServiceName = QString();
    callObjectPath = QString();
    callInterface = QString();
    callMethods.clear();
    callArguments.clear();
    callError = false;
    replyDataString.clear();
    connect(this, SIGNAL(operationComplete(QString, QString, QString)), manager, SLOT(operationComplete(QString, QString, QString)));

    gMAppletHandleStub->stubReset();
}

void Ut_MAppletInstanceManager::cleanup()
{
    // Destroy the manager
    delete manager;
    manager = 0;

    // Destroy default data store
    delete defaultDataStore;
    defaultDataStore = 0;
}

void Ut_MAppletInstanceManager::testOutOfProcessAppletUpdate()
{
    outOfProcess = true;

    // Create AppletInstanceCollection to verify created applet instances
    AppletInstanceCollection collection;
    QObject::connect(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)), &collection, SLOT(addInstance(QGraphicsWidget *, MDataStore &)));

    // Verify that the restoration process does not return an error
    QVERIFY(manager->restoreApplets());

    // Make sure the two applet instances were created
    QCOMPARE(collection.count(), 2);

    connect(this, SIGNAL(extensionChanged(const MDesktopEntry &)), manager, SLOT(updateApplet(const MDesktopEntry &)));

    MAppletInstanceData testApplet;
    QString testFile = "/tmp/testapplet1.desktop";
    testApplet.desktopFile = testFile;
    MAppletMetaData testMetaData(testFile);
    emit extensionChanged(testMetaData);

    // The manager asks the extension to do a reinit == kill+init
    QList<MethodCall *> reinitCalls = gMExtensionHandleStub->stubCallsTo("reinit");
    QCOMPARE(reinitCalls.length(), 1);
}


void Ut_MAppletInstanceManager::testInProcessAppletRestoration()
{
    QSignalSpy spy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));

    // Verify that the restoration process does not return an error
    QVERIFY(manager->restoreApplets());

    // Make sure the appletInstantiated signal was emitted exactly twice
    QCOMPARE(spy.count(), 2);

    // Applets 1 and 4 should be restored, 2 and 3 not
    QCOMPARE(gMAppletLoaderStub->stubCallCount("loadApplet"), 2);
    QCOMPARE(gMAppletLoaderStub->stubCallsTo("loadApplet").at(0)->parameter<QString>(0), QString("/tmp/testapplet1.desktop"));
    QCOMPARE(gMAppletLoaderStub->stubCallsTo("loadApplet").at(1)->parameter<QString>(0), QString("/tmp/testapplet4.desktop"));
}

void Ut_MAppletInstanceManager::testOutOfProcessAppletRestoration()
{
    outOfProcess = true;

    // Create AppletInstanceCollection to verify created applet instances
    AppletInstanceCollection collection;
    QObject::connect(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)), &collection, SLOT(addInstance(QGraphicsWidget *, MDataStore &)));

    // Verify that the restoration process does not return an error
    QVERIFY(manager->restoreApplets());

    // Make sure the two applet instances were created
    QCOMPARE(collection.count(), 2);

    // Make sure correct applets were instantiated as out of process applets
    MAppletHandle *instance1 = dynamic_cast<MAppletHandle *>(collection.at(0));
    MAppletHandle *instance2 = dynamic_cast<MAppletHandle *>(collection.at(1));
    QVERIFY(instance1 != NULL);
    QVERIFY(instance2 != NULL);
    QList<MethodCall *> initCalls = gMAppletHandleStub->stubCallsTo("init");
    QCOMPARE(initCalls.length(), 2);
    QCOMPARE(initCalls[0]->parameter<MAppletId>(3).toString(), QString("ut_mappletinstancemanager/testmanager/1"));
    QCOMPARE(initCalls[1]->parameter<MAppletId>(3).toString(), QString("ut_mappletinstancemanager/testmanager/4"));

    // Make sure the titles are set
    QList<MethodCall *> setAppletTitleCalls = gMAppletHandleStub->stubCallsTo("setAppletTitle");
    QCOMPARE(setAppletTitleCalls.length(), 2);
    QCOMPARE(setAppletTitleCalls[0]->parameter<QString>(0), QString("Title 1"));
    QCOMPARE(setAppletTitleCalls[1]->parameter<QString>(0), QString("Title 4"));

    // Make sure size hints are set
    QList<MethodCall *> setSizeHintsCalls = gMExtensionHandleStub->stubCallsTo("setSizeHints");
    QCOMPARE(setSizeHintsCalls.length(), 2);
    QCOMPARE(setSizeHintsCalls[0]->parameter<QVector<QSizeF> >(0).at(0), QSizeF(101, 201));
    QCOMPARE(setSizeHintsCalls[0]->parameter<QVector<QSizeF> >(0).at(1), QSizeF(101, 201));
    QCOMPARE(setSizeHintsCalls[0]->parameter<QVector<QSizeF> >(0).at(2), QSizeF(101, 201));
    QCOMPARE(setSizeHintsCalls[0]->parameter<QVector<QSizeF> >(0).at(3), QSizeF(101, 201));
    QCOMPARE(setSizeHintsCalls[1]->parameter<QVector<QSizeF> >(0).at(0), QSizeF(104, 204));
    QCOMPARE(setSizeHintsCalls[1]->parameter<QVector<QSizeF> >(0).at(1), QSizeF(104, 204));
    QCOMPARE(setSizeHintsCalls[1]->parameter<QVector<QSizeF> >(0).at(2), QSizeF(104, 204));
    QCOMPARE(setSizeHintsCalls[1]->parameter<QVector<QSizeF> >(0).at(3), QSizeF(104, 204));
}

void Ut_MAppletInstanceManager::testAppletDataStoring()
{
    // Create MAppletInstanceManager with a mock datastore.
    MockDataStore mockDataStore;
    delete manager;
    manager = new MTestAppletInstanceManager("testmanager", &mockDataStore);

    // Instantiate two applets
    QVERIFY(manager->instantiateApplet("/tmp/testapplet1.desktop"));
    QCOMPARE(mockDataStore.value("1/desktopFile"), QVariant("/tmp/testapplet1.desktop"));
    QCOMPARE(mockDataStore.value("1/packageName"), QVariant(""));
    QCOMPARE(mockDataStore.value("1/title"), QVariant(""));
}

void Ut_MAppletInstanceManager::testAppletInstantiation()
{
    // Create MAppletInstanceManager with a mock datastore.
    MockDataStore mockDataStore;
    delete manager;
    manager = new MTestAppletInstanceManager("testmanager", &mockDataStore);

    // Create a signal spy to investigate if appletInstantiated signal is emitted for all applets.
    QSignalSpy spy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));

    // Verify that the instantiation process does not return an error
    QVERIFY(manager->instantiateApplet("/tmp/testapplet1.desktop"));
    QVERIFY(manager->instantiateApplet("/tmp/testapplet4.desktop"));

    // Make sure the appletInstantiated signal was emitted exactly two times
    QCOMPARE(spy.count(), 2);
}


void Ut_MAppletInstanceManager::testAppletInstantiationWithMissingDesktopFiles()
{
    // Create MAppletInstanceManager with a mock datastore.
    existingFiles.clear();
    MockDataStore mockDataStore;
    delete manager;
    manager = new MTestAppletInstanceManager("testmanager", &mockDataStore);

    // Create a signal spy to investigate if appletInstantiated signal is emitted for all applets.
    QSignalSpy spy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));

    // Verify that the instantiation process does return an error
    QVERIFY(!manager->instantiateApplet("/tmp/testapplet1.desktop"));
    QVERIFY(!manager->instantiateApplet("/tmp/testapplet4.desktop"));

    // Make sure the appletInstantiated signal was emitted exactly zero times
    QCOMPARE(spy.count(), 0);
}

void Ut_MAppletInstanceManager::testAppletInstanceRemoval()
{
    QSignalSpy instantiateSpy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));
    QSignalSpy removeSpy(manager, SIGNAL(appletRemoved(QGraphicsWidget *)));

    // Verify that the instantiation process does not return an error
    QVERIFY(manager->restoreApplets());

    // Remove one of the instantiated applets
    MAppletId appletId("ut_mappletinstancemanager", "testmanager", 4);
    QVERIFY(manager->removeApplet(appletId));

    // Make sure the appletRemoved signal was emitted exactly once
    QCOMPARE(removeSpy.count(), 1);

    // Verify that applet instance data file was removed
    QCOMPARE(removedFiles.count(), 1);
    QString dataFileName = QDir::homePath() + "/.config/ut_mappletinstancemanager/testmanager/4.data";
    QCOMPARE(removedFiles.at(0), dataFileName);

    // Verify that the applet instance settings were removed
    QCOMPARE(gMAppletSettingsStub->stubCallCount("removeInstanceSettingValues"), 1);
    QCOMPARE(gMAppletSettingsStub->stubLastCallTo("mAppletSettingsConstructor").parameter<QString>(0), QString("/tmp/testapplet4.desktop"));
    QCOMPARE(gMAppletSettingsStub->stubLastCallTo("mAppletSettingsConstructor").parameter<MAppletId>(1).toString(), appletId.toString());

    // verify that applet metadata is removed from the extension watcher
    // when the last instance is removed, but not before that
    int removeFromWatcherCallCount = gMExtensionWatcherStub->stubCallCount("removeExtension");
    MAppletId appletId2("ut_mappletinstancemanager", "testmanager", 2);
    MAppletId appletId3("ut_mappletinstancemanager", "testmanager", 3);
    QVERIFY(manager->instantiateApplet("/tmp/testapplet4.desktop"));
    QVERIFY(manager->instantiateApplet("/tmp/testapplet4.desktop"));
    QVERIFY(manager->removeApplet(appletId2));
    QCOMPARE(gMExtensionWatcherStub->stubCallCount("removeExtension"),
             removeFromWatcherCallCount);
    QVERIFY(manager->removeApplet(appletId3));
    QCOMPARE(gMExtensionWatcherStub->stubCallCount("removeExtension"),
             removeFromWatcherCallCount + 1);
}

void Ut_MAppletInstanceManager::testAppletUninstallation()
{
    QSignalSpy removeSpy(manager, SIGNAL(appletRemoved(QGraphicsWidget *)));

    // Verify that the instantiation process does not return an error
    QVERIFY(manager->restoreApplets());

    // Emit an applet uninstalled signal
    connect(this, SIGNAL(appletUninstalled(QString)), manager, SLOT(appletUninstalled(QString)));
    emit appletUninstalled("/tmp/testapplet1.desktop");

    // Make sure the appletRemoved signal was emitted exactly once
    QCOMPARE(removeSpy.count(), 1);

    // Verify that applet instance data file was removed
    QCOMPARE(removedFiles.count(), 1);
    QString dataFileName = QDir::homePath() + "/.config/ut_mappletinstancemanager/testmanager/1.data";
    QCOMPARE(removedFiles.at(0), dataFileName);
}

void Ut_MAppletInstanceManager::testAppletInstanceDataStoreIsNonReadWrite()
{
    // Create MAppletInstanceManager with a mock datastore.
    MockDataStore mockDataStore;
    delete manager;
    manager = new MTestAppletInstanceManager("testmanager", &mockDataStore);

    // Create a signal spy to investigate if appletInstantiated signal is emitted
    QSignalSpy spy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));

    MFileDataStore_isReadable = false;
    QVERIFY(!manager->instantiateApplet("/tmp/testapplet1.desktop"));
    QVERIFY(!mockDataStore.contains("1/desktopFile"));
    QCOMPARE(spy.count(), 0);

    MFileDataStore_isWritable = false;
    QVERIFY(!manager->instantiateApplet("/tmp/testapplet1.desktop"));
    QVERIFY(!mockDataStore.contains("1/desktopFile"));
    QCOMPARE(spy.count(), 0);

    MFileDataStore_isReadable = true;
    QVERIFY(!manager->instantiateApplet("/tmp/testapplet1.desktop"));
    QVERIFY(!mockDataStore.contains("1/desktopFile"));
    QCOMPARE(spy.count(), 0);
}

void Ut_MAppletInstanceManager::testPreconfiguredAppletInstances()
{
    existingFiles.append(M_XDG_DIR "/" "ut_mappletinstancemanager/testmanager.data");

    delete manager;
    manager = new MTestAppletInstanceManager("testmanager");
    QCOMPARE(dirMkpath, QDir::homePath() + "/.config/ut_mappletinstancemanager/");
    QCOMPARE(fileCopy.first,  QString(M_XDG_DIR "/" "ut_mappletinstancemanager/testmanager.data"));
    QCOMPARE(fileCopy.second, QDir::homePath() + "/.config/ut_mappletinstancemanager/testmanager.data");

    // The data directory should not be created if it already exists
    fileCopy.first = QString();
    fileCopy.second = QString();
    dirMkpath = QString();
    dataDirectoryExists = true;
    delete manager;
    manager = new MTestAppletInstanceManager("testmanager");
    QCOMPARE(dirMkpath, QString());
    QCOMPARE(fileCopy.first,  QString(M_XDG_DIR "/" "ut_mappletinstancemanager/testmanager.data"));
    QCOMPARE(fileCopy.second, QDir::homePath() + "/.config/ut_mappletinstancemanager/testmanager.data");

    // The data file should not be copied if it already exists and the dir should not be created
    fileCopy.first = QString();
    fileCopy.second = QString();
    dirMkpath = QString();
    dataDirectoryExists = false;
    existingFiles.append(QDir::homePath() + "/.config/ut_mappletinstancemanager/testmanager.data");
    delete manager;
    manager = new MTestAppletInstanceManager("testmanager");
    QCOMPARE(dirMkpath, QString());
    QCOMPARE(fileCopy.first,  QString());
    QCOMPARE(fileCopy.second, QString());
}

void Ut_MAppletInstanceManager::testOOPAppletStartsToBrokenState()
{
    outOfProcess = true;
    gMExtensionHandleStub->stubSetReturnValue("state", MAppletHandleModel::BROKEN);

    // Create a signal spy to investigate if appletInstantiated signal is emitted for all applets.
    QSignalSpy spy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));

    // Verify that the instantiation process doesn't succeed
    QVERIFY(!manager->instantiateApplet("/tmp/testapplet1.desktop"));

    // Make sure the appletInstantiated signal is not emitted
    QCOMPARE(spy.count(), 0);
}

void Ut_MAppletInstanceManager::testInstallingAppletFromPackage()
{
    // Create a signal spy to investigate if appletInstantiated signal is emitted
    QSignalSpy spy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));

    // Instantiate an applet from a package
    QMap<QString, QVariant> metaData;
    metaData.insert("Maemo-Desktop-File", "test.desktop");
    metaData.insert("Maemo-Applet-Title", "Test Title");
    metaData.insert("Maemo-Applet-Size-Landscape", "600 300");
    metaData.insert("Maemo-Applet-Size-Portrait", "600 300");
    manager->instantiateAppletFromPackage("testpackage.deb", metaData);

    // Make sure the applet placeholder is instantiated
    QCOMPARE(gMAppletHandleStub->stubCallCount("initPlaceHolder"), 1);
    QCOMPARE(gMAppletHandleStub->stubLastCallTo("initPlaceHolder").parameter<MAppletId>(0).instanceId(), (uint)2);

    // Make sure the titles are set
    QList<MethodCall *> setAppletTitleCalls = gMAppletHandleStub->stubCallsTo("setAppletTitle");
    QCOMPARE(setAppletTitleCalls.length(), 1);
    QCOMPARE(setAppletTitleCalls[0]->parameter<QString>(0), QString("Test Title"));

    // Make sure size hints are set
    QList<MethodCall *> setSizeHintsCalls = gMExtensionHandleStub->stubCallsTo("setSizeHints");
    QCOMPARE(setSizeHintsCalls.length(), 1);
    QCOMPARE(setSizeHintsCalls[0]->parameter<QVector<QSizeF> >(0).at(0), QSizeF(600, 300));
    QCOMPARE(setSizeHintsCalls[0]->parameter<QVector<QSizeF> >(0).at(1), QSizeF(600, 300));
    QCOMPARE(setSizeHintsCalls[0]->parameter<QVector<QSizeF> >(0).at(2), QSizeF(600, 300));
    QCOMPARE(setSizeHintsCalls[0]->parameter<QVector<QSizeF> >(0).at(3), QSizeF(600, 300));
}

void Ut_MAppletInstanceManager::testInstallationOperationComplete()
{
    // Instantiate an applet from a package
    QMap<QString, QVariant> metaData;
    metaData.insert("Maemo-Desktop-File", "/tmp/test.desktop");
    manager->instantiateAppletFromPackage("testpackage.deb", metaData);

    // When the installation of the package being installed is complete the applet should be instantiated
    outOfProcess = true;
    existingFiles.append("/tmp/test.desktop");
    emit operationComplete("Install", "testpackage.deb", QString());
    QCOMPARE(gMAppletHandleStub->stubCallCount("init"), 1);
    QCOMPARE(gMAppletHandleStub->stubLastCallTo("init").parameter<MAppletId>(3).instanceId(), (uint)2);
}

void Ut_MAppletInstanceManager::testInstallationOperationCompleteForUnknownPackage()
{
    // Instantiate an applet from a package
    QMap<QString, QVariant> metaData;
    metaData.insert("Maemo-Desktop-File", "/tmp/test.desktop");
    manager->instantiateAppletFromPackage("testpackage.deb", metaData);

    // When the installation of the package being installed is complete the applet should be instantiated
    outOfProcess = true;
    existingFiles.append("/tmp/test.desktop");
    emit operationComplete("Install", "otherpackage.deb", QString());
    QCOMPARE(gMAppletHandleStub->stubCallCount("init"), 0);
}

void Ut_MAppletInstanceManager::testUnknownOperationComplete()
{
    // Instantiate an applet from a package
    QMap<QString, QVariant> metaData;
    metaData.insert("Maemo-Desktop-File", "/tmp/test.desktop");
    manager->instantiateAppletFromPackage("testpackage.deb", metaData);

    // When the installation of the package being installed is complete the applet should be instantiated
    outOfProcess = true;
    existingFiles.append("/tmp/test.desktop");
    emit operationComplete("UnknownOperation", "testpackage.deb", QString());
    QCOMPARE(gMAppletHandleStub->stubCallCount("init"), 0);
}

void Ut_MAppletInstanceManager::testRestoringAppletsBeingInstalled()
{
    // Verify that the restoration process does not return an error
    captureCalls = true;
    QVERIFY(manager->restoreApplets());

    // Make sure the package installation status is queried with an Operation D-Bus call
    QCOMPARE(callServiceName, QString("com.nokia.package_manager"));
    QCOMPARE(callObjectPath, QString("/com/nokia/package_manager"));
    QCOMPARE(callInterface, QString("com.nokia.package_manager"));
    QCOMPARE(callMethods.count(), 1);
    QCOMPARE(callMethods.at(0), QString("Operation"));
    QCOMPARE(callArguments.count(), 1);
    QCOMPARE(callArguments.at(0).count(), 1);
    QCOMPARE(callArguments.at(0).at(0), QVariant("/tmp/testapplet5.deb"));
}

void Ut_MAppletInstanceManager::testReceiveOperationFailed()
{
    QSignalSpy spy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));
    connect(this, SIGNAL(receiveOperation(QDBusPendingCallWatcher *)), manager, SLOT(receiveOperation(QDBusPendingCallWatcher *)));

    MAppletId appletId("ut_mappletinstancemanager", "testmanager", 5);
    QDBusMessage msg;
    QDBusPendingCall call = QDBusPendingCall::fromCompletedCall(msg);
    QDBusPendingCallWatcher watcher(call, NULL);
    watcher.setProperty("appletId", appletId.toString());

    // When the D-Bus call fails the desktop file should not be relayed
    callError = true;
    emit receiveOperation(&watcher);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(gMAppletHandleStub->stubCallCount("initPlaceHolder"), 1);
    QCOMPARE(gMAppletHandleStub->stubLastCallTo("initPlaceHolder").parameter<MAppletId>(0).instanceId(), (uint)5);
    QCOMPARE(gMAppletHandleStub->stubLastCallTo("initPlaceHolder").parameter<QString>(2), qtTrId("mappletinstancedata_nopackagemetadata"));
}

void Ut_MAppletInstanceManager::testReceiveUnknownOperation()
{
    QSignalSpy spy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));
    connect(this, SIGNAL(receiveOperation(QDBusPendingCallWatcher *)), manager, SLOT(receiveOperation(QDBusPendingCallWatcher *)));

    MAppletId appletId("ut_mappletinstancemanager", "testmanager", 5);
    QDBusMessage msg;
    QDBusPendingCall call = QDBusPendingCall::fromCompletedCall(msg);
    QDBusPendingCallWatcher watcher(call, NULL);
    watcher.setProperty("appletId", appletId.toString());

    // When the operation is not known the desktop file should not be relayed
    callError = false;
    emit receiveOperation(&watcher);
    QCOMPARE(instantiateAppletPlaceHolderMetaData.count(), 0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(gMAppletHandleStub->stubCallCount("initPlaceHolder"), 1);
    QCOMPARE(gMAppletHandleStub->stubLastCallTo("initPlaceHolder").parameter<MAppletId>(0).instanceId(), (uint)5);
    QCOMPARE(gMAppletHandleStub->stubLastCallTo("initPlaceHolder").parameter<QString>(2), qtTrId("mappletinstancedata_nopackagemetadata"));
}

void Ut_MAppletInstanceManager::testReceiveInstallOperation()
{
    QSignalSpy spy(manager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)));
    connect(this, SIGNAL(receiveOperation(QDBusPendingCallWatcher *)), manager, SLOT(receiveOperation(QDBusPendingCallWatcher *)));

    MAppletId appletId("ut_mappletinstancemanager", "testmanager", 5);
    QDBusMessage msg;
    QDBusPendingCall call = QDBusPendingCall::fromCompletedCall(msg);
    QDBusPendingCallWatcher watcher(call, NULL);
    watcher.setProperty("appletId", appletId.toString());

    // When the operation is Install the desktop file should be relayed
    replyDataString = "Install";
    emit receiveOperation(&watcher);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(gMAppletHandleStub->stubCallCount("initPlaceHolder"), 1);
    QCOMPARE(gMAppletHandleStub->stubLastCallTo("initPlaceHolder").parameter<MAppletId>(0).instanceId(), (uint)5);
    QCOMPARE(gMAppletHandleStub->stubLastCallTo("initPlaceHolder").parameter<QString>(2), QString());
}

QTEST_APPLESS_MAIN(Ut_MAppletInstanceManager)
