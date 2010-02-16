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

#include "ut_duimashupcanvasview.h"

#include "duimashupcanvas.h"
#include "duiappletinventory.h"
#include "../stubs/mockdatastore.h"

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiLayout>
#include <DuiContainer>
#include <DuiAbstractLayoutPolicy>
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

void Ut_DuiMashupCanvasView::initTestCase()
{
    // DuiApplications must be created manually due to theme system changes
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_duimashupcanvasview" };
    app = new DuiApplication(argc, app_name);
    appWindow = new DuiApplicationWindow;
}

void Ut_DuiMashupCanvasView::cleanupTestCase()
{
    delete appWindow;
    delete app;
}

void Ut_DuiMashupCanvasView::init()
{
    mashupCanvas = new DuiMashupCanvas("test");
    m_subject = new TestDuiMashupCanvasView(mashupCanvas);
    mashupCanvas->setView(m_subject);
}

void Ut_DuiMashupCanvasView::cleanup()
{
    delete m_subject;
}

void Ut_DuiMashupCanvasView::addWidgetToMashupCanvas(DuiWidget *widget, DuiDataStore *dataStore)
{
    QMap<DuiWidget *, DuiDataStore *> *dataStores = const_cast<QMap<DuiWidget *, DuiDataStore *> *>(mashupCanvas->model()->dataStores());
    (*dataStores)[widget] = dataStore;
    mashupCanvas->model()->dataStoresModified();
}

void Ut_DuiMashupCanvasView::removeWidgetFromMashupCanvas(DuiWidget *widget)
{
    QMap<DuiWidget *, DuiDataStore *> *dataStores = const_cast<QMap<DuiWidget *, DuiDataStore *> *>(mashupCanvas->model()->dataStores());
    dataStores->remove(widget);
    mashupCanvas->model()->dataStoresModified();
}

DuiContainer *Ut_DuiMashupCanvasView::container(QGraphicsWidget *widget)
{
    DuiContainer *container = NULL;

    if (widget) {
        QGraphicsWidget *parentWidget = widget->parentWidget();
        if (parentWidget != NULL) {
            container = dynamic_cast<DuiContainer *>(parentWidget);
        }
    }

    return container;
}

bool Ut_DuiMashupCanvasView::isWidgetInContainer(QGraphicsWidget *widget)
{
    return (container(widget) != NULL);
}

void Ut_DuiMashupCanvasView::verifyWidgetContainerVisibility(QList<DuiWidget *> *widgetList, bool visible)
{
  QListIterator<DuiWidget*> iterator(*widgetList);
  while (iterator.hasNext()) {
    DuiWidget* widget = iterator.next();
    DuiContainer* theContainer = container(widget);
    QVERIFY(isWidgetInContainer(widget));
    if (visible) {
      QCOMPARE(theContainer->objectName(), QString(""));
      QVERIFY(theContainer->headerVisible());
    } else {
      QCOMPARE(theContainer->objectName(), QString("DuiMashupCanvasInvisibleContainer"));
      QVERIFY(!theContainer->headerVisible());
    }
    QGraphicsLayout *appletLayout = dynamic_cast<QGraphicsLayout *>(mashupCanvas->layout()->itemAt(0));
    QVERIFY(appletLayout != NULL);
    QCOMPARE(appletLayout->count(), widgetList->size());
  }
}

QList<DuiWidget *> *Ut_DuiMashupCanvasView::createWidgets(int numberOfWidgets, bool containerMode)
{
    m_subject->modifiableStyle()->setContainerMode(containerMode);
    m_subject->applyStyle();
    QList<DuiWidget *> *widgetList = new QList<DuiWidget *>;
    for (int i = 0; i < numberOfWidgets; ++i) {
        DuiWidget *widget = new DuiWidget;
        MockDataStore *store = new MockDataStore;
        addWidgetToMashupCanvas(widget, store);
        widgetList->append(widget);
    }
    return widgetList;
}

void Ut_DuiMashupCanvasView::testLayoutPolicy()
{
    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(mashupCanvas->layout());
    QVERIFY(mainLayout != NULL);
    DuiLayout *layout = dynamic_cast<DuiLayout *>(mainLayout->itemAt(0));
    QVERIFY(layout != NULL);
    DuiAbstractLayoutPolicy *policy = layout->policy();
    QVERIFY(policy != NULL);
}

void Ut_DuiMashupCanvasView::testAdditionWithFlowLayoutPolicy()
{
    DuiWidget *widget1 = new DuiWidget;
    MockDataStore store1;
    DuiWidget *widget2 = new DuiWidget;
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


void Ut_DuiMashupCanvasView::testRemovalWithFlowLayoutPolicy()
{

    QList<DuiWidget *> *widgetList = createWidgets(3);

    // Remove widget2
    removeWidgetFromMashupCanvas(widgetList->at(1));

    // Ensure that widget1 and widget3 are still in the layout but widget2 is not.
    QVERIFY(widgetInLayout(widgetList->at(0)));
    QVERIFY(!widgetInLayout(widgetList->at(1)));
    QVERIFY(widgetInLayout(widgetList->at(2)));

    while (!widgetList->isEmpty())
        delete widgetList->takeFirst();

}


bool Ut_DuiMashupCanvasView::widgetInLayout(DuiWidget *widget)
{
    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(mashupCanvas->layout());
    DuiLayout *layout = dynamic_cast<DuiLayout *>(mainLayout->itemAt(0));
    DuiAbstractLayoutPolicy *policy = layout->policy();

    bool found = false;
    for (int i = 0; i < policy->count(); ++i) {
        DuiWidget *w = NULL;

        DuiContainer *container = dynamic_cast<DuiContainer *>(policy->itemAt(i));
        if (container != NULL) {
            // Widget has a container, take the centralWidget data
            w = container->centralWidget();
        } else {
            // No container
            w = dynamic_cast<DuiWidget *>(policy->itemAt(i));
        }

        found |= w == widget;
    }

    return found;
}

void Ut_DuiMashupCanvasView::testSettingContainerModeOff()
{
    QList<DuiWidget *> *widgetList = createWidgets(2, true);

    // Check that the containers are visible
    verifyWidgetContainerVisibility(widgetList, true);

    m_subject->modifiableStyle()->setContainerMode(false);
    m_subject->applyStyle();

    // Check that the containers are not visible
    verifyWidgetContainerVisibility(widgetList, false);

    while (!widgetList->isEmpty())
        delete widgetList->takeFirst();
}

void Ut_DuiMashupCanvasView::testSettingContainerModeOn()
{
    QList<DuiWidget *> *widgetList = createWidgets(2, false);
    verifyWidgetContainerVisibility(widgetList, false);

    m_subject->modifiableStyle()->setContainerMode(true);
    m_subject->applyStyle();

    verifyWidgetContainerVisibility(widgetList, true);

    while (!widgetList->isEmpty())
        delete widgetList->takeFirst();
}

void Ut_DuiMashupCanvasView::testSettingContainerModeOnWhenContainerModeIsOn()
{
    QList<DuiWidget *> *widgetList = createWidgets(2, true);

    verifyWidgetContainerVisibility(widgetList, true);

    m_subject->modifiableStyle()->setContainerMode(true);
    m_subject->applyStyle();

    verifyWidgetContainerVisibility(widgetList, true);

    while (!widgetList->isEmpty())
        delete widgetList->takeFirst();
}

void Ut_DuiMashupCanvasView::testSettingContainerModeOffWhenContainerModeIsOff()
{
    QList<DuiWidget *> *widgetList = createWidgets(2, false);
    verifyWidgetContainerVisibility(widgetList, false);

    m_subject->modifiableStyle()->setContainerMode(false);
    m_subject->applyStyle();

    verifyWidgetContainerVisibility(widgetList, false);

    while (!widgetList->isEmpty())
        delete widgetList->takeFirst();
}


void Ut_DuiMashupCanvasView::testSettingContainerModeOffWidgetsHaveCorrectLayoutOrder()
{
    QList<DuiWidget *> *widgetList = createWidgets(2, true);

    m_subject->modifiableStyle()->setContainerMode(false);
    m_subject->applyStyle();

    // Get the widgets from the layout of mashup canvas
    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(mashupCanvas->layout());
    DuiLayout *layout = dynamic_cast<DuiLayout *>(mainLayout->itemAt(0));
    DuiAbstractLayoutPolicy *policy = layout->policy();

    DuiContainer *container1 = dynamic_cast<DuiContainer *>(policy->itemAt(0));
    DuiContainer *container2 = dynamic_cast<DuiContainer *>(policy->itemAt(1));

    QCOMPARE(widgetList->count(), 2);
    QCOMPARE(layout->count(), 2);
    QCOMPARE(policy->count(), 2);

    // and check if the order is the same.
    QCOMPARE(widgetList->at(0), container1->centralWidget());
    QCOMPARE(widgetList->at(1), container2->centralWidget());

    while (!widgetList->isEmpty())
        delete widgetList->takeFirst();
}

QTEST_APPLESS_MAIN(Ut_DuiMashupCanvasView)
