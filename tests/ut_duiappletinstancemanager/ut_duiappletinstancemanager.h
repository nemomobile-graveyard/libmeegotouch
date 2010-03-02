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

#ifndef UT_DUIAPPLETINSTANCEMANAGER_H
#define UT_DUIAPPLETINSTANCEMANAGER_H

#include <QObject>
#include <DuiWidget>
#include "duiappletinstancemanager.h"

class DuiApplication;
class DuiWindow;
class MockDataStore;
class DuiDataStore;
class QDBusPendingCallWatcher;

class DuiTestAppletInstanceManager : public DuiAppletInstanceManager
{
public:
    DuiTestAppletInstanceManager(const QString &identifier, DuiDataStore *dataStore = NULL);
};

// AppletInstanceCollection is used to store all instantiated applets into.
// The collection can be used to verify that correct applets were instantiated during the test run.
class AppletInstanceCollection : public QObject
{
    Q_OBJECT

public:
    virtual ~AppletInstanceCollection();
    int count() const;
    DuiWidget *at(int index) const;

public slots:
    void addInstance(DuiWidget *widget, DuiDataStore &store);
    void removeInstance(DuiWidget *widget);

private:
    QList<DuiWidget *> instances;
};

class Ut_DuiAppletInstanceManager : public QObject
{
    Q_OBJECT

public:
    // For storing information about the D-Bus stub calls made
    static bool captureCalls;
    static QString callServiceName;
    static QString callObjectPath;
    static QString callInterface;
    static QList<QString> callMethods;
    static QList< QList<QVariant> > callArguments;
    static bool callError;
    static QString replyDataString;

private:
    DuiApplication *app;
    MockDataStore *defaultDataStore;
    DuiTestAppletInstanceManager *manager;
    QString dataPath;

signals:
    void appletUninstalled(const QString &desktopFile);
    void operationComplete(const QString &operation, const QString &pkg, const QString &error);
    void receiveOperation(QDBusPendingCallWatcher *watcher);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testInProcessAppletRestoration();
    void testOutOfProcessAppletRestoration();
    void testAppletDataStoring();
    void testAppletInstantiation();
    void testAppletInstantiationWithMissingDesktopFiles();
    void testAppletInstanceRemoval();
    void testAppletUninstallation();
    void testAppletInstanceDataStoreIsNonReadWrite();
    void testOOPAppletStartsToBrokenState();
    void testPreconfiguredAppletInstances();
    void testInstallingAppletFromPackage();
    void testInstallationOperationComplete();
    void testInstallationOperationCompleteForUnknownPackage();
    void testUnknownOperationComplete();
    void testRestoringAppletsBeingInstalled();
    void testReceiveOperationFailed();
    void testReceiveUnknownOperation();
    void testReceiveInstallOperation();
};

#endif // UT_DUIAPPLETINSTANCEMANAGER_H
