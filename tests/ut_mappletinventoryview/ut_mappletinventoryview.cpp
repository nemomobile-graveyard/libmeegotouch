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

#include <QtTest/QtTest>
#include <MApplication>
#include <MApplicationWindow>
#include <MLayout>
#include <MFlowLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MSceneWindow>
#include <MScene>
#include <MSceneManager>
#include <MContainer>
#include <mappletinventory.h>
#include <mappletinventorymodel.h>
#include <mappletbutton_stub.h>
#include <mobjectmenu_stub.h>
#include <mclassfactory.h>
#include "mwidgetcontroller_p.h"
#include "ut_mappletinventoryview.h"
#include "mapplicationextensionarea_stub.h"
#include "mextensionarea_stub.h"

// MClassFactory stubs
void MClassFactory::registerViewCreator(MViewCreatorBase *, const char *)
{
}

void MClassFactory::unregisterViewCreator(MViewCreatorBase *)
{
}

// MAppletInventory stubs
MAppletInventory::MAppletInventory(QGraphicsItem *parent) :
    MWidgetController(new MWidgetControllerPrivate, new MAppletInventoryModel, parent),
    mashupCanvas(NULL),
    watcher(new QFileSystemWatcher()),
    appletPath(APPLET_DATA),
    initialized(false)
{
}

MAppletInventory::~MAppletInventory()
{
}

void MAppletInventory::appletPathChanged(QString const &)
{
}

void MAppletInventory::appletButtonClicked()
{
}

void MAppletInventory::instantiateAppletsInPackage(QString const &)
{
}

void MAppletInventory::setMAppletInventoryInterface(MApplicationExtensionInterface *)
{
}

// MSceneManager stubs
void MSceneManager::appearSceneWindow(MSceneWindow *window, MSceneWindow::DeletionPolicy)
{
    window->show();
    Ut_MAppletInventoryView::appearCalled = true;
}

void MSceneManager::disappearSceneWindow(MSceneWindow *window)
{
    window->hide();
    Ut_MAppletInventoryView::disappearCalled = true;
}

// QGraphicsItem stubs (used by MSceneManager)
void QGraphicsItem::setZValue(qreal z)
{
    Q_UNUSED(z);
}

// MContainer stubs
QList<MContainer *> gMContainers;
QList<QGraphicsWidget *> gMContainerCentralWidgets;
void MContainer::setCentralWidget(QGraphicsWidget *centralWidget, bool /*destroy*/)
{
    gMContainers.append(this);
    gMContainerCentralWidgets.append(centralWidget);
}

// TestAppletInventoryView implementation
TestAppletInventoryView::TestAppletInventoryView(MAppletInventory *controller) : MAppletInventoryView(controller)
{
}

bool Ut_MAppletInventoryView::appearCalled;
bool Ut_MAppletInventoryView::disappearCalled;

void Ut_MAppletInventoryView::initTestCase()
{
    // Create a MAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_mappletinventoryview";
    app = new MApplication(argc, &app_name);
    appWin = new MApplicationWindow;
}

void Ut_MAppletInventoryView::cleanupTestCase()
{
    delete appWin;
    // this is commented out for now, to prevent crash at exit:
    // delete app;
}

void Ut_MAppletInventoryView::init()
{
    gMApplicationExtensionAreaStub->stubReset();

    controller = new MAppletInventory;
    view = new TestAppletInventoryView(controller);
    controller->setView(view);
    appWin->scene()->addItem(controller);

    appearCalled = false;
    disappearCalled = false;
}

void Ut_MAppletInventoryView::cleanup()
{
    static_cast<MAppletInventoryModel*>(controller->model())->setWidgets(WidgetList());
    delete controller;
}

void Ut_MAppletInventoryView::testInitialization()
{
    QCOMPARE(gMApplicationExtensionAreaStub->stubCallCount("init"), 1);
}

void Ut_MAppletInventoryView::testModelModifiedWidgets()
{
    // Set three widgets in the model
    MWidget widget1;
    MWidget widget2;
    MWidget widget3;
    QList<MWidget *> widgets;
    widgets.append(&widget1);
    widgets.append(&widget2);
    widgets.append(&widget3);
    static_cast<MAppletInventoryModel*>(controller->model())->setWidgets(widgets);

    // The widgets should now be in the MFlowLayoutPolicy
    MLayout *layout = dynamic_cast<MLayout *>(controller->layout());
    QVERIFY(layout != NULL);

    MContainer *appletContainer = dynamic_cast<MContainer *>(layout->itemAt(0));
    QVERIFY(appletContainer != NULL);

    layout = dynamic_cast<MLayout *>(appletContainer->centralWidget()->layout());
    QVERIFY(layout != NULL);

    MFlowLayoutPolicy *policy = dynamic_cast<MFlowLayoutPolicy *>(layout->policy());
    QVERIFY(policy != NULL);
    QCOMPARE(policy->count(), 3);
    QCOMPARE(dynamic_cast<MWidget *>(policy->itemAt(0)), &widget1);
    QCOMPARE(dynamic_cast<MWidget *>(policy->itemAt(1)), &widget2);
    QCOMPARE(dynamic_cast<MWidget *>(policy->itemAt(2)), &widget3);

    // Remove two widgets from the list
    widgets.removeAt(0);
    widgets.removeAt(1);
    static_cast<MAppletInventoryModel*>(controller->model())->setWidgets(widgets);

    // Verify that only the relevant one is still there
    QCOMPARE(policy->count(), 1);
    QCOMPARE(dynamic_cast<MWidget *>(policy->itemAt(0)), &widget2);
}

void Ut_MAppletInventoryView::testModelModifiedCloseButtonVisible()
{
    // Set the close button visible
    static_cast<MAppletInventoryModel*>(controller->model())->setCloseButtonVisible(true);
    QVERIFY(!disappearCalled);
    QVERIFY(appearCalled);

    // Set the close button invisible
    appearCalled = false;
    static_cast<MAppletInventoryModel*>(controller->model())->setCloseButtonVisible(false);
    QVERIFY(disappearCalled);
    QVERIFY(!appearCalled);
}

QTEST_APPLESS_MAIN(Ut_MAppletInventoryView)
