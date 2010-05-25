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

#ifndef _UT_MAPPLETINVENTORY_
#define _UT_MAPPLETINVENTORY_

#include <QtTest/QtTest>
#include <QObject>
#include <mappletinstallationsourceinterface.h>

class MAppletInventory;
class MApplication;
class MApplicationWindow;
class QGraphicsWidget;
class MMashupCanvas;
class MDesktopEntry;

class TestAppletSource : public QObject, public MAppletInstallationSourceInterface
{
    Q_OBJECT
    Q_INTERFACES(MAppletInstallationSourceInterface)

public:
    TestAppletSource();
    virtual ~TestAppletSource();
    // methods derived from MAppletInstallationSourceInterface
    virtual bool initialize(const QString &);
    virtual QGraphicsWidget *widget();
    virtual void setMAppletInventoryInterface(MAppletInventoryInterface &installationSource);
    //! For accessing applet inventory interface
    MAppletInventoryInterface *appletInventoryInterface() const;
    void instantiateAppletsInPackage(const QString &packageName);

private:
    MAppletInventoryInterface *appletInventory;
    QGraphicsWidget *sourceWidget;
};

class Ut_MAppletInventory : public QObject
{
    Q_OBJECT

public:
    static QStringList watchedDirectories;
    static QStringList appletList;
    static QList<MAppletInventoryInterface *> appletInstallationSource;
    static bool addPathCalled;
    static bool directoriesCalled;
    static bool pluginPathExists;
    static bool pluginPathIsReadable;
    static bool tooManyMonitoredPaths;

private:
    // MAppletInventory instance under testing
    MAppletInventory *inventory;

    // MApplication
    MApplication *app;

    // MApplication
    MApplicationWindow *appWin;

    // Mashup canvas for the inventory
    MMashupCanvas *mashupCanvas;

signals:
    void appletPathChanged(const QString path);
    void instantiateAppletsFromPackage(const QString &packageName);

private slots:

    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testSuccessfulInitialization();
    void testInitializationWithInvalidPluginPath();
    void testStartWithExistingApplets();
    void testStartWithInvalidApplets();
    void testAppletAdd();
    void testAppletRemove();
    void testCategories_data();
    void testCategories();
    void testEnabled();
    void testOnlyDotDesktopFilesAreParsed();
    void testInstantiateAppletsInPackage();
};

#endif // _UT_MAPPLETINVENTORY_
