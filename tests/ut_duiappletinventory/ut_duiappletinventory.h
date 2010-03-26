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

#ifndef _UT_DUIAPPLETINVENTORY_
#define _UT_DUIAPPLETINVENTORY_

#include <QtTest/QtTest>
#include <QObject>
#include <duiappletinstallationsourceinterface.h>

class DuiAppletInventory;
class DuiApplication;
class DuiApplicationWindow;
class DuiWidget;
class DuiMashupCanvas;
class DuiDesktopEntry;

class TestAppletSource : public QObject, public DuiAppletInstallationSourceInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiAppletInstallationSourceInterface)

public:
    // methods derived from DuiAppletInstallationSourceInterface
    virtual bool initialize(const QString &);
    virtual DuiWidget *widget();
};

class Ut_DuiAppletInventory : public QObject
{
    Q_OBJECT

public:
    static QStringList watchedDirectories;
    static QStringList appletList;
    static bool addPathCalled;
    static bool directoriesCalled;
    static bool pluginPathExists;
    static bool pluginPathIsReadable;
    static bool tooManyMonitoredPaths;

private:
    // DuiAppletInventory instance under testing
    DuiAppletInventory *inventory;

    // DuiApplication
    DuiApplication *app;

    // DuiApplication
    DuiApplicationWindow *appWin;

    // Mashup canvas for the inventory
    DuiMashupCanvas *mashupCanvas;

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
};

#endif // _UT_DUIAPPLETINVENTORY_
