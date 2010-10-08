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

    // Destroy any created widgets
    qDeleteAll(createdWidgets);
    createdWidgets.clear();
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

void Ut_MApplicationExtensionAreaView::createWidgets(int numberOfWidgets, bool containerMode)
{
    m_subject->modifiableStyle()->setContainerMode(containerMode);
    m_subject->applyStyle();
    for (int i = 0; i < numberOfWidgets; ++i) {
        MWidget *widget = new MWidget;
        MockDataStore *store = new MockDataStore;
        addWidgetToApplicationExtensionArea(widget, store);
        createdWidgets.append(widget);
    }
}

int Ut_MApplicationExtensionAreaView::widgetPosInLayout(MWidget *widget)
{
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(extensionArea->layout());

    int pos = -1;

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

            if(w == widget) {
                pos = i;
                break;
            }
        }
    }

    return pos;
}

bool Ut_MApplicationExtensionAreaView::widgetInLayout(MWidget *widget)
{
    return widgetPosInLayout(widget) >= 0;
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
    QCOMPARE(widgetInLayout(widget1), true);

    // Ensure that the layout data is written to data store.
    QCOMPARE(store1.contains("layoutIndex"), true);

    // Add another widget. Addition will change layout data of both widgets.
    addWidgetToApplicationExtensionArea(widget2, &store2);

    // Check that it was added into the layout.
    QCOMPARE(widgetInLayout(widget2), true);

    // Ensure that new layout data is stored into data store.
    QCOMPARE(store1.contains("layoutIndex"), true);
    QCOMPARE(store2.contains("layoutIndex"), true);

    delete widget1;
    delete widget2;
}

void Ut_MApplicationExtensionAreaView::testOrdering()
{
    MWidget *widget1 = new MWidget;
    MWidget *widget2 = new MWidget;
    MWidget *widget3 = new MWidget;
    MockDataStore store1;
    MockDataStore store2;
    MockDataStore store3;

    // add one widget with order specified, other without, check ordering
    store1.createValue("order", 0);
    addWidgetToApplicationExtensionArea(widget1, &store1);
    addWidgetToApplicationExtensionArea(widget2, &store2);
    QCOMPARE(widgetPosInLayout(widget1), 0);
    QCOMPARE(widgetPosInLayout(widget2), 1);
    QCOMPARE(extensionArea->layout()->count(), 2);

    // add another widget with order specified, check the ordering
    store3.createValue("order", 2);
    addWidgetToApplicationExtensionArea(widget3, &store3);
    QCOMPARE(widgetPosInLayout(widget1), 0);
    QCOMPARE(widgetPosInLayout(widget3), 1);
    QCOMPARE(widgetPosInLayout(widget2), 2);
    QCOMPARE(extensionArea->layout()->count(), 3);

    // reverse the order of two widgets, check ordering
    store1.createValue("order", 1);
    store2.createValue("order", 0);
    extensionArea->model()->dataStoresModified();
    QCOMPARE(widgetPosInLayout(widget1), 1);
    QCOMPARE(widgetPosInLayout(widget2), 0);
    QCOMPARE(widgetPosInLayout(widget3), 2);
    QCOMPARE(extensionArea->layout()->count(), 3);

    // remove widget from middle, check ordering
    removeWidgetFromApplicationExtensionArea(widget1);
    QCOMPARE(widgetPosInLayout(widget2), 0);
    QCOMPARE(widgetPosInLayout(widget3), 1);
    QCOMPARE(extensionArea->layout()->count(), 2);

    // add a widget with layoutIndex, without order
    store1.createValue("layoutIndex", 0);
    store1.remove("order");
    addWidgetToApplicationExtensionArea(widget1, &store1);
    QCOMPARE(widgetPosInLayout(widget2), 0);
    QCOMPARE(widgetPosInLayout(widget3), 1);
    QCOMPARE(widgetPosInLayout(widget1), 2);
    QCOMPARE(extensionArea->layout()->count(), 3);
}

void Ut_MApplicationExtensionAreaView::testRemoval()
{
    createWidgets(3);

    // Remove widget2
    removeWidgetFromApplicationExtensionArea(createdWidgets.at(1));

    // Ensure that widget1 and widget3 are still in the layout but widget2 is not.
    QCOMPARE(widgetInLayout(createdWidgets.at(0)), true);
    QCOMPARE(widgetInLayout(createdWidgets.at(1)), false);
    QCOMPARE(widgetInLayout(createdWidgets.at(2)), true);
}

void Ut_MApplicationExtensionAreaView::testApplyStyle()
{
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(extensionArea->layout());
    QVERIFY(layout != NULL);

    m_subject->modifiableStyle()->setLayoutOrientation(Qt::Horizontal);
    m_subject->applyStyle();
    QCOMPARE(layout->orientation(), Qt::Horizontal);

    m_subject->modifiableStyle()->setLayoutOrientation(Qt::Vertical);
    m_subject->applyStyle();
    QCOMPARE(layout->orientation(), Qt::Vertical);
}

QTEST_APPLESS_MAIN(Ut_MApplicationExtensionAreaView)
