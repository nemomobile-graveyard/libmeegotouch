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

#include <QtTest/QtTest>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiLayout>
#include <DuiFlowLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiSceneWindow>
#include <DuiScene>
#include <DuiSceneManager>
#include <DuiContainer>
#include <duiappletinventory.h>
#include <duiappletbutton_stub.h>
#include <duiobjectmenu_stub.h>
#include <duiclassfactory.h>
#include "duiwidgetcontroller_p.h"
#include "ut_duiappletinventoryview.h"

// DuiClassFactory stubs
void DuiClassFactory::registerViewCreator(DuiViewCreatorBase *, const char *)
{
}

void DuiClassFactory::unregisterViewCreator(DuiViewCreatorBase *)
{
}

// DuiAppletInventory stubs
DuiAppletInventory::DuiAppletInventory(QGraphicsItem *parent) :
    DuiWidgetController(new DuiWidgetControllerPrivate, new DuiAppletInventoryModel, parent),
    mashupCanvas(NULL),
    watcher(new QFileSystemWatcher()),
    appletPath(APPLET_DATA)
{
}

DuiAppletInventory::~DuiAppletInventory()
{
}

void DuiAppletInventory::appletPathChanged(QString const &)
{
}

void DuiAppletInventory::appletButtonClicked()
{
}

void DuiAppletInventory::instantiateAppletsFromPackage(QString const &)
{
}

// DuiSceneManager stubs
void DuiSceneManager::showWindow(DuiSceneWindow *window, DuiSceneWindow::DeletionPolicy)
{
    window->show();
    Ut_DuiAppletInventoryView::appearCalled = true;
}

void DuiSceneManager::hideWindow(DuiSceneWindow *window)
{
    window->hide();
    Ut_DuiAppletInventoryView::disappearCalled = true;
}

// QGraphicsItem stubs (used by DuiSceneManager)
void QGraphicsItem::setZValue(qreal z)
{
    Q_UNUSED(z);
}

// DuiContainer stubs
QList<DuiContainer *> gDuiContainers;
QList<DuiWidget *> gDuiContainerCentralWidgets;
void DuiContainer::setCentralWidget(DuiWidget *centralWidget, bool /*destroy*/)
{
    gDuiContainers.append(this);
    gDuiContainerCentralWidgets.append(centralWidget);
}

// TestAppletInventoryView implementation
TestAppletInventoryView::TestAppletInventoryView(DuiAppletInventory *controller) : DuiAppletInventoryView(controller)
{
}

bool Ut_DuiAppletInventoryView::appearCalled;
bool Ut_DuiAppletInventoryView::disappearCalled;

void Ut_DuiAppletInventoryView::initTestCase()
{
    // Create a DuiAapplication
    int argc = 1;
    char *app_name = (char *)"./ut_duiappletinventoryview";
    app = new DuiApplication(argc, &app_name);
    appWin = new DuiApplicationWindow;
}

void Ut_DuiAppletInventoryView::cleanupTestCase()
{
    delete appWin;
    // this is commented out for now, to prevent crash at exit:
    // delete app;
}

void Ut_DuiAppletInventoryView::init()
{
    controller = new DuiAppletInventory;
    view = new TestAppletInventoryView(controller);
    controller->setView(view);
    appWin->scene()->addItem(controller);

    appearCalled = false;
    disappearCalled = false;
}

void Ut_DuiAppletInventoryView::cleanup()
{
    controller->model()->setWidgets(WidgetList());
    delete controller;
}

void Ut_DuiAppletInventoryView::testModelModifiedWidgets()
{
    // Set three widgets in the model
    DuiWidget widget1;
    DuiWidget widget2;
    DuiWidget widget3;
    QList<DuiWidget *> widgets;
    widgets.append(&widget1);
    widgets.append(&widget2);
    widgets.append(&widget3);
    controller->model()->setWidgets(widgets);

    // The widgets should now be in the DuiFlowLayoutPolicy
    DuiLayout *layout = dynamic_cast<DuiLayout *>(controller->layout());
    QVERIFY(layout != NULL);

    DuiContainer *appletContainer = dynamic_cast<DuiContainer *>(layout->itemAt(0));
    QVERIFY(appletContainer != NULL);

    layout = dynamic_cast<DuiLayout *>(appletContainer->centralWidget()->layout());
    QVERIFY(layout != NULL);

    DuiFlowLayoutPolicy *policy = dynamic_cast<DuiFlowLayoutPolicy *>(layout->policy());
    QVERIFY(policy != NULL);
    QCOMPARE(policy->count(), 3);
    QCOMPARE(dynamic_cast<DuiWidget *>(policy->itemAt(0)), &widget1);
    QCOMPARE(dynamic_cast<DuiWidget *>(policy->itemAt(1)), &widget2);
    QCOMPARE(dynamic_cast<DuiWidget *>(policy->itemAt(2)), &widget3);

    // Remove two widgets from the list
    widgets.removeAt(0);
    widgets.removeAt(1);
    controller->model()->setWidgets(widgets);

    // Verify that only the relevant one is still there
    QCOMPARE(policy->count(), 1);
    QCOMPARE(dynamic_cast<DuiWidget *>(policy->itemAt(0)), &widget2);
}

void Ut_DuiAppletInventoryView::testModelModifiedCloseButtonVisible()
{
    // Set the close button visible
    controller->model()->setCloseButtonVisible(true);
    QVERIFY(!disappearCalled);
    QVERIFY(appearCalled);

    // Set the close button invisible
    appearCalled = false;
    controller->model()->setCloseButtonVisible(false);
    QVERIFY(disappearCalled);
    QVERIFY(!appearCalled);
}

void Ut_DuiAppletInventoryView::testModelModifiedInstallationSources()
{
    QList<DuiWidget *> sourceWidgets;

    // See that containers are created for all source widgets
    gDuiContainerCentralWidgets.clear();
    sourceWidgets.append(new DuiWidget);
    sourceWidgets.append(new DuiWidget);
    controller->model()->setInstallationSources(sourceWidgets);
    QCOMPARE(gDuiContainerCentralWidgets, sourceWidgets);

    gDuiContainerCentralWidgets.clear();
    gDuiContainers.clear();
    sourceWidgets.clear();
    TestSourceWidget *testSourceWidget = new TestSourceWidget;
    testSourceWidget->installationSourceIcon_  = "fooicon";
    testSourceWidget->installationSourceTitle_ = "footitle";
    testSourceWidget->installationSourceText_  = "footext";

    // Source container's properties should come from the source widget
    sourceWidgets.append(testSourceWidget);
    controller->model()->setInstallationSources(sourceWidgets);
    QCOMPARE(gDuiContainerCentralWidgets, sourceWidgets);
    QCOMPARE(gDuiContainers[0]->iconID(), testSourceWidget->installationSourceIcon_);
    QCOMPARE(gDuiContainers[0]->title(), testSourceWidget->installationSourceTitle_);
    QCOMPARE(gDuiContainers[0]->text(), testSourceWidget->installationSourceText_);

    // Signals from source widget should change the values in the container
    testSourceWidget->setInstallationSourceIcon("baricon");
    testSourceWidget->setInstallationSourceTitle("bartitle");
    testSourceWidget->setInstallationSourceText("bartext");
    QCOMPARE(gDuiContainers[0]->iconID(), testSourceWidget->installationSourceIcon_);
    QCOMPARE(gDuiContainers[0]->title(), testSourceWidget->installationSourceTitle_);
    QCOMPARE(gDuiContainers[0]->text(), testSourceWidget->installationSourceText_);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletInventoryView)
