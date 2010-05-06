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

#include "ut_mmashupcanvas.h"
#include "mmashupcanvas.h"
#include "mapplication.h"
#include "mcomponentdata.h"
#include "mappletinstancemanager_stub.h"
#include "mappletid_stub.h"
#include "mobjectmenu_stub.h"
#include "mockdatastore.h"

#include <QtTest/QtTest>

#include <mwidgetcreator.h>
M_REGISTER_WIDGET(TestMashupCanvas);

// TestMashupCanvas
class TestMashupCanvas : public MMashupCanvas
{
public:
    TestMashupCanvas() : MMashupCanvas("testcanvas") {}
    void addWidget(MWidget *widget, MDataStore &store) {
        MMashupCanvas::addWidget(widget, store);
    }
    void removeWidget(MWidget *widget) {
        MMashupCanvas::removeWidget(widget);
    }
};

// MComponentData stubs
QString gMComponentDataServiceName = "com.nokia.testapp";
QString MComponentData::serviceName()
{
    return gMComponentDataServiceName;
}

// The test class
void Ut_MMashupCanvas::init()
{
    canvas = new TestMashupCanvas();
    view = new TestMashupCanvasView(canvas);
    canvas->setView(view);
    gMAppletInstanceManagerStub->stubReset();
}

void Ut_MMashupCanvas::cleanup()
{
    // Destroy the mashup canvas.
    delete canvas;
}

void Ut_MMashupCanvas::initTestCase()
{
    // MApplications must be created manually due to theme system changes
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_mmashupcanvas" };
    app = new MApplication(argc, app_name);
}

void Ut_MMashupCanvas::cleanupTestCase()
{
    // this is commented out for now, to prevent crash at exit:
    // delete app;
}

void Ut_MMashupCanvas::testAddition()
{
    QSignalSpy spy(canvas->model(), SIGNAL(modified(QList<const char *>)));

    MWidget *widget1 = new MWidget;
    MockDataStore store1;
    MWidget *widget2 = new MWidget;
    MockDataStore store2;

    // Add one widget and ensure that the model was modified.
    canvas->addWidget(widget1, store1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(canvas->model()->dataStores()->keys().count(), 1);
    QVERIFY(canvas->model()->dataStores()->contains(widget1));
    spy.clear();

    // Add another widget. Ensure that both widgets are in the model.
    canvas->addWidget(widget2, store2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(canvas->model()->dataStores()->keys().count(), 2);
    QVERIFY(canvas->model()->dataStores()->contains(widget1));
    QVERIFY(canvas->model()->dataStores()->contains(widget2));
    spy.clear();

    // Add the first widget again. The model should not be modified.
    canvas->addWidget(widget1, store1);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(canvas->model()->dataStores()->keys().count(), 2);
}

void Ut_MMashupCanvas::testRemoval()
{
    QSignalSpy spy(canvas->model(), SIGNAL(modified(QList<const char *>)));

    MWidget *widget1 = new MWidget;
    MockDataStore store1;
    MWidget *widget2 = new MWidget;
    MockDataStore store2;
    MWidget *widget3 = new MWidget;
    MockDataStore store3;

    // Add three widgets and ensure that they're in the model.
    canvas->addWidget(widget1, store1);
    canvas->addWidget(widget2, store2);
    canvas->addWidget(widget3, store3);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(canvas->model()->dataStores()->keys().count(), 3);
    QVERIFY(canvas->model()->dataStores()->contains(widget1));
    QVERIFY(canvas->model()->dataStores()->contains(widget2));
    QVERIFY(canvas->model()->dataStores()->contains(widget3));
    spy.clear();

    // Remove widget2 and verify that the rest are in the model.
    canvas->removeWidget(widget2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(canvas->model()->dataStores()->keys().count(), 2);
    QVERIFY(canvas->model()->dataStores()->contains(widget1));
    QVERIFY(!canvas->model()->dataStores()->contains(widget2));
    QVERIFY(canvas->model()->dataStores()->contains(widget3));
    spy.clear();

    // Remove widget2 again and verify that the model was not changed.
    canvas->removeWidget(widget2);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(canvas->model()->dataStores()->keys().count(), 2);
    QVERIFY(canvas->model()->dataStores()->contains(widget1));
    QVERIFY(!canvas->model()->dataStores()->contains(widget2));
    QVERIFY(canvas->model()->dataStores()->contains(widget3));
    spy.clear();
}

void Ut_MMashupCanvas::testCategories()
{
    QCOMPARE(canvas->categories(), QStringList());

    QStringList categories;
    categories.append("Category1");
    categories.append("Category2");
    canvas->setCategories(categories);

    QCOMPARE(canvas->categories(), categories);
}

void Ut_MMashupCanvas::testUniqueCanvasIdentifiers()
{
    // Create two canvases with the same name
    MMashupCanvas c1("foo");
    MMashupCanvas c2("foo");

    // Ensure that the canvas names are unique
    QStringList identifiers;
    foreach(MethodCall * call, gMAppletInstanceManagerStub->stubCallHistory()) {
        if (call->name() == "constructor") {
            identifiers.append(call->parameter<QString>(0));
        }
    }
    QCOMPARE(identifiers.count(), 2);
    QVERIFY(identifiers.at(0) != identifiers.at(1));
}

void Ut_MMashupCanvas::testEmptyCanvasIdentifier()
{
    MMashupCanvas emptyCanvas("");
    QStringList identifiers;
    foreach(MethodCall * call, gMAppletInstanceManagerStub->stubCallHistory()) {
        if (call->name() == "constructor") {
            identifiers.append(call->parameter<QString>(0));
        }
    }
    QCOMPARE(identifiers.count(), 1);
    QVERIFY(identifiers.at(0) != "");
}

void Ut_MMashupCanvas::testServiceAddress()
{
    MMashupCanvas canvas("meh");

    QCOMPARE(canvas.serviceAddress(), gMComponentDataServiceName + "/MAppletInstanceManager/meh");
}


QTEST_APPLESS_MAIN(Ut_MMashupCanvas)
