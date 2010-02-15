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

#include "ut_duimashupcanvas.h"
#include "duimashupcanvas.h"
#include "duiapplication.h"
#include "duicomponentdata.h"
#include "duiappletinstancemanager_stub.h"
#include "duiappletid_stub.h"
#include "duiobjectmenu_stub.h"
#include "mockdatastore.h"

#include <QtTest/QtTest>

#include <duiwidgetcreator.h>
DUI_REGISTER_WIDGET(TestMashupCanvas);

// TestMashupCanvas
class TestMashupCanvas : public DuiMashupCanvas
{
public:
    TestMashupCanvas() : DuiMashupCanvas("testcanvas") {}
    void addWidget(DuiWidget *widget, DuiDataStore &store) {
        DuiMashupCanvas::addWidget(widget, store);
    }
    void removeWidget(DuiWidget *widget) {
        DuiMashupCanvas::removeWidget(widget);
    }
};

// DuiComponentData stubs
QString gDuiComponentDataServiceName = "com.nokia.testapp";
QString DuiComponentData::serviceName()
{
    return gDuiComponentDataServiceName;
}

// The test class
void Ut_DuiMashupCanvas::init()
{
    canvas = new TestMashupCanvas();
    view = new TestMashupCanvasView(canvas);
    canvas->setView(view);
    gDuiAppletInstanceManagerStub->stubReset();
}

void Ut_DuiMashupCanvas::cleanup()
{
    // Destroy the mashup canvas.
    delete canvas;
}

void Ut_DuiMashupCanvas::initTestCase()
{
    // DuiApplications must be created manually due to theme system changes
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_duimashupcanvas" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiMashupCanvas::cleanupTestCase()
{
    // this is commented out for now, to prevent crash at exit:
    // delete app;
}

void Ut_DuiMashupCanvas::testAddition()
{
    QSignalSpy spy(canvas->model(), SIGNAL(modified(QList<const char *>)));

    DuiWidget *widget1 = new DuiWidget;
    MockDataStore store1;
    DuiWidget *widget2 = new DuiWidget;
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

void Ut_DuiMashupCanvas::testRemoval()
{
    QSignalSpy spy(canvas->model(), SIGNAL(modified(QList<const char *>)));

    DuiWidget *widget1 = new DuiWidget;
    MockDataStore store1;
    DuiWidget *widget2 = new DuiWidget;
    MockDataStore store2;
    DuiWidget *widget3 = new DuiWidget;
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

void Ut_DuiMashupCanvas::testCategories()
{
    QCOMPARE(canvas->categories(), QStringList());

    QStringList categories;
    categories.append("Category1");
    categories.append("Category2");
    canvas->setCategories(categories);

    QCOMPARE(canvas->categories(), categories);
}

void Ut_DuiMashupCanvas::testUniqueCanvasIdentifiers()
{
    // Create two canvases with the same name
    DuiMashupCanvas c1("foo");
    DuiMashupCanvas c2("foo");

    // Ensure that the canvas names are unique
    QStringList identifiers;
    foreach(MethodCall * call, gDuiAppletInstanceManagerStub->stubCallHistory()) {
        if (call->name() == "constructor") {
            identifiers.append(call->parameter<QString>(0));
        }
    }
    QCOMPARE(identifiers.count(), 2);
    QVERIFY(identifiers.at(0) != identifiers.at(1));
}

void Ut_DuiMashupCanvas::testEmptyCanvasIdentifier()
{
    DuiMashupCanvas emptyCanvas("");
    QStringList identifiers;
    foreach(MethodCall * call, gDuiAppletInstanceManagerStub->stubCallHistory()) {
        if (call->name() == "constructor") {
            identifiers.append(call->parameter<QString>(0));
        }
    }
    QCOMPARE(identifiers.count(), 1);
    QVERIFY(identifiers.at(0) != "");
}

void Ut_DuiMashupCanvas::testServiceAddress()
{
    DuiMashupCanvas canvas("meh");

    QCOMPARE(canvas.serviceAddress(), gDuiComponentDataServiceName + "/DuiAppletInstanceManager/meh");
}


QTEST_APPLESS_MAIN(Ut_DuiMashupCanvas)
