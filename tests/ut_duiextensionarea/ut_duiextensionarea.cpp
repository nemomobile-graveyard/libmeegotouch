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

#include "ut_duiextensionarea.h"
#include "duiextensionarea.h"
#include "duiextensionareaview.h"
#include "duiextensionarea_p.h"
#include "duiappletid_stub.h"
#include "duiapplication.h"
#include "duiappletinstancemanager_stub.h"
#include "mockdatastore.h"
#include "duiobjectmenu_stub.h"

#include <QtTest/QtTest>

#include <duiwidgetcreator.h>
DUI_REGISTER_WIDGET(TestExtensionArea);

// TestExtensionArea
class TestExtensionArea : public DuiExtensionArea
{
public:
    TestExtensionArea() : DuiExtensionArea() { }
    void addWidget(DuiWidget *widget, DuiDataStore &store) {
        DuiExtensionArea::addWidget(widget, store);
    }
    void removeWidget(DuiWidget *widget) {
        DuiExtensionArea::removeWidget(widget);
    }
};

// The test class
void Ut_DuiExtensionArea::init()
{
    area = new TestExtensionArea();
    gDuiAppletInstanceManagerStub->stubReset();
}

void Ut_DuiExtensionArea::cleanup()
{
    // Destroy the extension area.
    delete area;
}

void Ut_DuiExtensionArea::initTestCase()
{
    // DuiApplications must be created manually due to theme system changes
    int argc = 1;
    char *app_name = (char *)"./ut_duiextensionarea";
    app = new DuiApplication(argc, &app_name);
}

void Ut_DuiExtensionArea::cleanupTestCase()
{
    // this is commented out for now, to prevent crash at exit:
    // delete app;
}

void Ut_DuiExtensionArea::testAddition()
{
    QSignalSpy spy(area->model(), SIGNAL(modified(QList<const char *>)));

    DuiWidget *widget1 = new DuiWidget;
    MockDataStore store1;
    DuiWidget *widget2 = new DuiWidget;
    MockDataStore store2;

    // Add one widget and ensure that the model was modified.
    area->addWidget(widget1, store1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(area->model()->dataStores()->keys().count(), 1);
    QVERIFY(area->model()->dataStores()->contains(widget1));
    spy.clear();

    // Add another widget. Ensure that both widgets are in the model.
    area->addWidget(widget2, store2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(area->model()->dataStores()->keys().count(), 2);
    QVERIFY(area->model()->dataStores()->contains(widget1));
    QVERIFY(area->model()->dataStores()->contains(widget2));
    spy.clear();

    // Add the first widget again. The model should not be modified.
    area->addWidget(widget1, store1);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(area->model()->dataStores()->keys().count(), 2);
}

void Ut_DuiExtensionArea::testRemoval()
{
    QSignalSpy spy(area->model(), SIGNAL(modified(QList<const char *>)));

    DuiWidget *widget1 = new DuiWidget;
    MockDataStore store1;
    DuiWidget *widget2 = new DuiWidget;
    MockDataStore store2;
    DuiWidget *widget3 = new DuiWidget;
    MockDataStore store3;

    // Add three widgets and ensure that they're in the model.
    area->addWidget(widget1, store1);
    area->addWidget(widget2, store2);
    area->addWidget(widget3, store3);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(area->model()->dataStores()->keys().count(), 3);
    QVERIFY(area->model()->dataStores()->contains(widget1));
    QVERIFY(area->model()->dataStores()->contains(widget2));
    QVERIFY(area->model()->dataStores()->contains(widget3));
    spy.clear();

    // Remove widget2 and verify that the rest are in the model.
    area->removeWidget(widget2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(area->model()->dataStores()->keys().count(), 2);
    QVERIFY(area->model()->dataStores()->contains(widget1));
    QVERIFY(!area->model()->dataStores()->contains(widget2));
    QVERIFY(area->model()->dataStores()->contains(widget3));
    spy.clear();

    // Remove widget2 again and verify that the model was not changed.
    area->removeWidget(widget2);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(area->model()->dataStores()->keys().count(), 2);
    QVERIFY(area->model()->dataStores()->contains(widget1));
    QVERIFY(!area->model()->dataStores()->contains(widget2));
    QVERIFY(area->model()->dataStores()->contains(widget3));
    spy.clear();
}

QTEST_APPLESS_MAIN(Ut_DuiExtensionArea)
