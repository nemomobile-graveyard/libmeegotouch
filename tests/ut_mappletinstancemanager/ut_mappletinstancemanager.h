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

#ifndef UT_MAPPLETINSTANCEMANAGER_H
#define UT_MAPPLETINSTANCEMANAGER_H

#include <QObject>
#include "mappletinstancemanager.h"

class MApplication;
class MWindow;
class MockDataStore;
class MDataStore;
class QDBusPendingCallWatcher;

class MTestAppletInstanceManager : public MAppletInstanceManager
{
public:
    MTestAppletInstanceManager(const QString &identifier, MDataStore *dataStore = NULL);
};

// AppletInstanceCollection is used to store all instantiated applets into.
// The collection can be used to verify that correct applets were instantiated during the test run.
class AppletInstanceCollection : public QObject
{
    Q_OBJECT

public:
    virtual ~AppletInstanceCollection();
    int count() const;
    QGraphicsWidget *at(int index) const;

public slots:
    void addInstance(QGraphicsWidget *widget, MDataStore &store);
    void removeInstance(QGraphicsWidget *widget);

private:
    QList<QGraphicsWidget *> instances;
};

class Ut_MAppletInstanceManager : public QObject
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
    MApplication *app;
    MockDataStore *defaultDataStore;
    MTestAppletInstanceManager *manager;
    QString dataPath;

signals:
    void appletUninstalled(const QString &desktopFile);
    void operationComplete(const QString &operation, const QString &pkg, const QString &error);
    void receiveOperation(QDBusPendingCallWatcher *watcher);
    // Used by the extesnion watcher
    void fileChanged(const QString &path);
    void extensionChanged(const MDesktopEntry &appletData) const;
private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testOutOfProcessAppletUpdate();
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

#endif // UT_MAPPLETINSTANCEMANAGER_H
