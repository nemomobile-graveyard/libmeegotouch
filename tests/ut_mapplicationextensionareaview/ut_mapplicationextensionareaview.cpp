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

#include "ut_mapplicationextensionareaview.h"

#include "mapplicationextensionarea.h"
#include "../stubs/mockdatastore.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MLayout>
#include <MContainer>
#include <MAbstractLayoutPolicy>
#include <QDBusInterface>

#include <QGraphicsLinearLayout>
#include <QtTest/QtTest>

void QFileSystemWatcher::addPath(const QString &)
{
}

bool QDBusConnection::registerObject(const QString &, QObject *, RegisterOptions)
{
    return true;
}

void Ut_MApplicationExtensionAreaView::initTestCase()
{
    // MApplications must be created manually due to theme system changes
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_mapplicationextensionareaview" };
    app = new MApplication(argc, app_name);
    appWindow = new MApplicationWindow;
}

void Ut_MApplicationExtensionAreaView::cleanupTestCase()
{
    delete appWindow;
    delete app;
}

void Ut_MApplicationExtensionAreaView::init()
{
    extensionArea = new MApplicationExtensionArea("test");
    m_subject = new TestMApplicationExtensionAreaView(extensionArea);
    extensionArea->setView(m_subject);
}

void Ut_MApplicationExtensionAreaView::cleanup()
{
    delete m_subject;
}

void Ut_MApplicationExtensionAreaView::addWidgetToApplicationExtensionArea(QGraphicsWidget *widget, MDataStore *dataStore)
{
    QMap<QGraphicsWidget *, MDataStore *> *dataStores = const_cast<QMap<QGraphicsWidget *, MDataStore *> *>(extensionArea->model()->dataStores());
    (*dataStores)[widget] = dataStore;
    extensionArea->model()->dataStoresModified();
}

void Ut_MApplicationExtensionAreaView::removeWidgetFromApplicationExtensionArea(QGraphicsWidget *widget)
{
    QMap<QGraphicsWidget *, MDataStore *> *dataStores = const_cast<QMap<QGraphicsWidget *, MDataStore *> *>(extensionArea->model()->dataStores());
    dataStores->remove(widget);
    extensionArea->model()->dataStoresModified();
}

QList<MWidget *> *Ut_MApplicationExtensionAreaView::createWidgets(int numberOfWidgets, bool containerMode)
{
    m_subject->modifiableStyle()->setContainerMode(containerMode);
    m_subject->applyStyle();
    QList<MWidget *> *widgetList = new QList<MWidget *>;
    for (int i = 0; i < numberOfWidgets; ++i) {
        MWidget *widget = new MWidget;
        MockDataStore *store = new MockDataStore;
        addWidgetToApplicationExtensionArea(widget, store);
        widgetList->append(widget);
    }
    return widgetList;
}

bool Ut_MApplicationExtensionAreaView::widgetInLayout(MWidget *widget)
{
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(extensionArea->layout());

    bool found = false;
    if (layout) {
        for (int i = 0; i < layout->count(); ++i) {
            QGraphicsWidget *w = NULL;

            MContainer *container = dynamic_cast<MContainer *>(layout->itemAt(i));
            if (container != NULL) {
                // Widget has a container, take the centralWidget data
                w = container->centralWidget();
            } else {
                // No container
                w = dynamic_cast<MWidget *>(layout->itemAt(i));
            }

            found |= w == widget;
        }
    }

    return found;
}

void Ut_MApplicationExtensionAreaView::testLayoutPolicy()
{
    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(extensionArea->layout());
    QVERIFY(mainLayout != NULL);
}

void Ut_MApplicationExtensionAreaView::testAddition()
{
    MWidget *widget1 = new MWidget;
    MockDataStore store1;
    MWidget *widget2 = new MWidget;
    MockDataStore store2;

    // Add one widget.
    addWidgetToApplicationExtensionArea(widget1, &store1);

    // Check that it was added into the layout.
    QVERIFY(widgetInLayout(widget1));

    // Ensure that the layout data is written to data store.
    QVERIFY(store1.contains("layoutIndex"));

    // Add another widget. Addition will change layout data of both widgets.
    addWidgetToApplicationExtensionArea(widget2, &store2);

    // Check that it was added into the layout.
    QVERIFY(widgetInLayout(widget2));

    // Ensure that new layout data is stored into data store.
    QVERIFY(store1.contains("layoutIndex"));
    QVERIFY(store2.contains("layoutIndex"));

    delete widget1;
    delete widget2;
}


void Ut_MApplicationExtensionAreaView::testRemoval()
{
    QList<MWidget *> *widgetList = createWidgets(3);

    // Remove widget2
    removeWidgetFromApplicationExtensionArea(widgetList->at(1));

    // Ensure that widget1 and widget3 are still in the layout but widget2 is not.
    QVERIFY(widgetInLayout(widgetList->at(0)));
    QVERIFY(!widgetInLayout(widgetList->at(1)));
    QVERIFY(widgetInLayout(widgetList->at(2)));

    while (!widgetList->isEmpty())
        delete widgetList->takeFirst();
}

QTEST_APPLESS_MAIN(Ut_MApplicationExtensionAreaView)
