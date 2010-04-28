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

#include "ut_mmashupcanvasview.h"

#include "mmashupcanvas.h"
#include "mappletinventory.h"
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

QStringList QDir::entryList(const QStringList &, Filters, SortFlags) const
{
    return QStringList();
}

bool QDBusConnection::registerObject(const QString &, QObject *, RegisterOptions)
{
    return true;
}

void Ut_MMashupCanvasView::initTestCase()
{
    // MApplications must be created manually due to theme system changes
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_mmashupcanvasview" };
    app = new MApplication(argc, app_name);
    appWindow = new MApplicationWindow;
}

void Ut_MMashupCanvasView::cleanupTestCase()
{
    delete appWindow;
    delete app;
}

void Ut_MMashupCanvasView::init()
{
    mashupCanvas = new MMashupCanvas("test");
    m_subject = new TestMMashupCanvasView(mashupCanvas);
    mashupCanvas->setView(m_subject);
}

void Ut_MMashupCanvasView::cleanup()
{
    delete m_subject;

    // Destroy any created widgets
    qDeleteAll(createdWidgets);
    createdWidgets.clear();
}

void Ut_MMashupCanvasView::addWidgetToMashupCanvas(QGraphicsWidget *widget, MDataStore *dataStore)
{
    QMap<QGraphicsWidget *, MDataStore *> *dataStores = const_cast<QMap<QGraphicsWidget *, MDataStore *> *>(mashupCanvas->model()->dataStores());
    (*dataStores)[widget] = dataStore;
    mashupCanvas->model()->dataStoresModified();
}

void Ut_MMashupCanvasView::removeWidgetFromMashupCanvas(MWidget *widget)
{
    QMap<QGraphicsWidget *, MDataStore *> *dataStores = const_cast<QMap<QGraphicsWidget *, MDataStore *> *>(mashupCanvas->model()->dataStores());
    dataStores->remove(widget);
    mashupCanvas->model()->dataStoresModified();
}

MContainer *Ut_MMashupCanvasView::container(QGraphicsWidget *widget)
{
    MContainer *container = NULL;

    if (widget) {
        QGraphicsWidget *parentWidget = widget->parentWidget();
        if (parentWidget != NULL) {
            container = dynamic_cast<MContainer *>(parentWidget);
        }
    }

    return container;
}

bool Ut_MMashupCanvasView::isWidgetInContainer(QGraphicsWidget *widget)
{
    return (container(widget) != NULL);
}

void Ut_MMashupCanvasView::verifyWidgetContainerVisibility(QList<MWidget *> *widgetList, bool visible)
{
    QListIterator<MWidget *> iterator(*widgetList);
    while (iterator.hasNext()) {
        MWidget *widget = iterator.next();
        MContainer *theContainer = container(widget);
        QVERIFY(isWidgetInContainer(widget));
        if (visible) {
            QCOMPARE(theContainer->objectName(), QString(""));
            QVERIFY(theContainer->headerVisible());
        } else {
            QCOMPARE(theContainer->objectName(), QString("MExtensionAreaInvisibleContainer"));
            QVERIFY(!theContainer->headerVisible());
        }
    }
    QGraphicsLayout *appletLayout = dynamic_cast<QGraphicsLayout *>(mashupCanvas->layout()->itemAt(0));
    QVERIFY(appletLayout != NULL);
    QCOMPARE(appletLayout->count(), widgetList->size());
}

void Ut_MMashupCanvasView::createWidgets(int numberOfWidgets, bool containerMode)
{
    m_subject->modifiableStyle()->setContainerMode(containerMode);
    m_subject->applyStyle();
    for (int i = 0; i < numberOfWidgets; ++i) {
        MWidget *widget = new MWidget;
        MockDataStore *store = new MockDataStore;
        addWidgetToMashupCanvas(widget, store);
        createdWidgets.append(widget);
    }
}

void Ut_MMashupCanvasView::testLayoutPolicy()
{
    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(mashupCanvas->layout());
    QVERIFY(mainLayout != NULL);
    MLayout *layout = dynamic_cast<MLayout *>(mainLayout->itemAt(0));
    QVERIFY(layout != NULL);
    MAbstractLayoutPolicy *policy = layout->policy();
    QVERIFY(policy != NULL);
}

void Ut_MMashupCanvasView::testAdditionWithFlowLayoutPolicy()
{
    MWidget *widget1 = new MWidget;
    MockDataStore store1;
    MWidget *widget2 = new MWidget;
    MockDataStore store2;

    // Add one widget.
    addWidgetToMashupCanvas(widget1, &store1);

    // Check that it was added into the layout.
    QVERIFY(widgetInLayout(widget1));

    // Ensure that the layout data is written to data store.
    QVERIFY(store1.contains("layoutIndex"));

    // Add another widget. Addition will change layout data of both widgets.
    addWidgetToMashupCanvas(widget2, &store2);

    // Check that it was added into the layout.
    QVERIFY(widgetInLayout(widget2));

    // Ensure that new layout data is stored into data store.
    QVERIFY(store1.contains("layoutIndex"));
    QVERIFY(store2.contains("layoutIndex"));

    delete widget1;
    delete widget2;
}

void Ut_MMashupCanvasView::testRemovalWithFlowLayoutPolicy()
{
    createWidgets(3);

    // Remove widget2
    removeWidgetFromMashupCanvas(createdWidgets.at(1));

    // Ensure that widget1 and widget3 are still in the layout but widget2 is not.
    QVERIFY(widgetInLayout(createdWidgets.at(0)));
    QVERIFY(!widgetInLayout(createdWidgets.at(1)));
    QVERIFY(widgetInLayout(createdWidgets.at(2)));
}

bool Ut_MMashupCanvasView::widgetInLayout(MWidget *widget)
{
    bool found = false;

    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(mashupCanvas->layout());
    if (mainLayout) {
        MLayout *layout = dynamic_cast<MLayout *>(mainLayout->itemAt(0));
        if (layout) {
            MAbstractLayoutPolicy *policy = layout->policy();

            for (int i = 0; i < policy->count(); ++i) {
                QGraphicsWidget *w = NULL;

                MContainer *container = dynamic_cast<MContainer *>(policy->itemAt(i));
                if (container != NULL) {
                    // Widget has a container, take the centralWidget data
                    w = container->centralWidget();
                } else {
                    // No container
                    w = dynamic_cast<MWidget *>(policy->itemAt(i));
                }

                found |= w == widget;
            }
        }
    }

    return found;
}

void Ut_MMashupCanvasView::testSettingContainerModeOff()
{
    createWidgets(2, true);

    // Check that the containers are visible
    verifyWidgetContainerVisibility(&createdWidgets, true);

    m_subject->modifiableStyle()->setContainerMode(false);
    m_subject->applyStyle();

    // Check that the containers are not visible
    verifyWidgetContainerVisibility(&createdWidgets, false);
}

void Ut_MMashupCanvasView::testSettingContainerModeOn()
{
    createWidgets(2, false);
    verifyWidgetContainerVisibility(&createdWidgets, false);

    m_subject->modifiableStyle()->setContainerMode(true);
    m_subject->applyStyle();

    verifyWidgetContainerVisibility(&createdWidgets, true);
}

void Ut_MMashupCanvasView::testSettingContainerModeOnWhenContainerModeIsOn()
{
    createWidgets(2, true);

    verifyWidgetContainerVisibility(&createdWidgets, true);

    m_subject->modifiableStyle()->setContainerMode(true);
    m_subject->applyStyle();

    verifyWidgetContainerVisibility(&createdWidgets, true);
}

void Ut_MMashupCanvasView::testSettingContainerModeOffWhenContainerModeIsOff()
{
    createWidgets(2, false);
    verifyWidgetContainerVisibility(&createdWidgets, false);

    m_subject->modifiableStyle()->setContainerMode(false);
    m_subject->applyStyle();

    verifyWidgetContainerVisibility(&createdWidgets, false);
}


void Ut_MMashupCanvasView::testSettingContainerModeOffWidgetsHaveCorrectLayoutOrder()
{
    createWidgets(2, true);

    m_subject->modifiableStyle()->setContainerMode(false);
    m_subject->applyStyle();

    // Get the widgets from the layout of mashup canvas
    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(mashupCanvas->layout());
    QVERIFY(mainLayout != NULL);
    MLayout *layout = dynamic_cast<MLayout *>(mainLayout->itemAt(0));
    QVERIFY(layout != NULL);
    MAbstractLayoutPolicy *policy = layout->policy();

    QCOMPARE(createdWidgets.count(), 2);
    QCOMPARE(layout->count(), 2);
    QCOMPARE(policy->count(), 2);

    MContainer *container1 = dynamic_cast<MContainer *>(policy->itemAt(0));
    MContainer *container2 = dynamic_cast<MContainer *>(policy->itemAt(1));
    QVERIFY(container1 != NULL);
    QVERIFY(container2 != NULL);
    // and check if the order is the same.
    QCOMPARE(createdWidgets.at(0), container1->centralWidget());
    QCOMPARE(createdWidgets.at(1), container2->centralWidget());
}

QTEST_APPLESS_MAIN(Ut_MMashupCanvasView)
