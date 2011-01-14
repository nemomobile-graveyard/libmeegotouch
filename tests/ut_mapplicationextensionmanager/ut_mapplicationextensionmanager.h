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

#ifndef UT_MAPPLICATIONEXTENSIONMANAGER
#define UT_MAPPLICATIONEXTENSIONMANAGER

#include <QObject>
#include <QPair>
#include "mapplicationextensioninterface.h"
#include "mwidget.h"

class MApplicationExtensionManager;
class MDataStore;
class MApplicationExtensionMetaData;
class MDesktopEntry;
/*!
 * Class for listening to MApplicationExtensionManager signals
 */
class SignalListener : public QObject
{
    Q_OBJECT

public slots:
    void extensionInstantiated(MApplicationExtensionInterface *extension);

    void extensionRemoved(MApplicationExtensionInterface *extension);

    void widgetCreated(QGraphicsWidget*, MDataStore&);

    void widgetRemoved(QGraphicsWidget* widget);

public:
    QList<MApplicationExtensionInterface *> instantiatedExtensions;
    QList<QPair<MApplicationExtensionInterface *, QString> > removedExtensions;
    QList<QGraphicsWidget*> createdWidgets;
    QList<QGraphicsWidget*> removedWidgets;
};

/*!
 * A bad application extension interface that does not derive from the base class
 */
class BadApplicationExtensionInterface
{
};

Q_DECLARE_INTERFACE(BadApplicationExtensionInterface, "com.nokia.m.core.BadApplicationExtensionInterface/1.0")

/*!
 * A bad application extension that does not derive from the correct interface
 */
class BadExtension : public QObject, public BadApplicationExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(BadApplicationExtensionInterface)
};

/*!
 * A good application extension that does derive from the correct interface
 */
class GoodExtension : public QObject, public MApplicationExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(MApplicationExtensionInterface)

public:
    virtual bool initialize(const QString &interface);
    virtual QGraphicsWidget *widget();

    bool success;
    QGraphicsWidget *widget_;
    QString name;
};

class Ut_MApplicationExtensionManager : public QObject
{
    Q_OBJECT

private:
    MApplicationExtensionManager *manager;
    SignalListener signalListener;

    /*!
     * Sets up the test subject.
     */
    void setupTestSubject(const QString &inProcessFilter = QString(),
                          const QString &outOfProcessFilter = QString(),
                          const QStringList &order = QStringList());

    int goodExtensionCount;

    /*!
     * Sets up a good extension to the extension variable. The extension is set to
     * return the widget parameter when the widget() method of the extension is called.
     * \param success determines whether the extension initializes succesfully.
     * \param widget the widget that the extension returns.
     * \param name name for the extension. If left empty, a name is generated automatically.
     */
    void setupGoodExtension(bool success = true, QGraphicsWidget* widget = NULL, const QString &name = QString());

    QList<MApplicationExtensionInterface*> extensions;

    void verifyOrderCreatedInDataStore(int order);

signals:
    void directoryChanged(QString path);
    void extensionChanged(const MDesktopEntry &extensionData) const;

private slots:
    // Executed once before every test case
    void init();

    // Executed once after every test case
    void cleanup();

    // Executed once before first test case
    void initTestCase();

    // Executed once after last test case
    void cleanupTestCase();

    // Test that adding a new extension during runtime will be noticed by the manager
    void testMonitorAddExtension();

    // Test that removing an extension during runtime will be noticed by the manager
    void testMonitorRemoveExtension();
    void testMonitorRemoveExtensionWithTwoExtensionInstancesFromTheSameExtension();

    // Test that instantating an inexisting extension fails
    void testInstantiateInProcessExtensionWhichDoesNotExist();

    // Test that instantating an extension that does not implement the given API fails
    void testInstantiateInProcessExtensionWhichDoesNotImplementGivenInterface();

    // Test that instantating an extension that does not derive from the base interface fails
    void testInstantiateInProcessExtensionWhichDoesNotDeriveFromBaseInterface();

    // Test that instantating an extension that implements proper interfaces fails based on whether the extension says so
    void testInstantiateInProcessExtensionWhichFails();

    // Test that instantating an extension that implements proper interfaces succeeds if the extension says so
    void testInstantiateInProcessExtensionWhichSucceedsWithoutWidget();

    // Test that instantating an extension that implements proper interfaces gets the desired widget
    void testInstantiateInProcessExtensionWhichSucceedsWithWidget();

    // Test instantiating out of process extension
    void testInstantiateOutOfProcessExtension();

    void testRemoveInProcessExtension();
    void testRemoveNonExistentInProcessExtension();
    // Test filtering of in and out of process extensions
    void testInProcessExtensionFiltering();
    void testOutOfProcessExtensionFiltering();
    void testUpdateInProcessExtension();

    // Test ordering of the extensions
    void testOrdering();

    // Test that manager returns list of instantiated in process extensions when requested.
    void testRequestForAllInProcessExtensionsReturnsAListOfExtensions();

    // Test that addWidget singal is not sent when extension is added without widget
    void testAddWidgetInProcessExtensionWithoutWidget();
    // Test that addWidget singal is not sent when extension is added with widget
    void testAddWidgetInProcessExtensionWithWidget();
    // Test removing in process extension widget
    void testRemoveWidgetInProcessExtension();
    // Test removing out of porcess extension
    void testRemoveOutOfProcessExtension();

    void testInstantiatingTwoInProcessExtensionsFromTheSameSource();

    void testInstantiatingInProcessExtensionUsesCorrectLoaderHints();
};

#endif // UT_MAPPLICATIONEXTENSIONMANAGER
