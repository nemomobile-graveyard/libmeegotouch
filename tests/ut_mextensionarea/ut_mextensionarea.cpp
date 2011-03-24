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

#include "ut_mextensionarea.h"
#include "mextensionarea.h"
#include "mextensionareaview.h"
#include "mextensionarea_p.h"
#include "mappletid_stub.h"
#include "mapplication.h"
#include "mappletinstancemanager_stub.h"
#include "mockdatastore.h"
#include "mobjectmenu_stub.h"

#include <QtTest/QtTest>

#include <mwidgetcreator.h>
M_REGISTER_WIDGET(TestExtensionArea);

// TestExtensionArea
class TestExtensionArea : public MExtensionArea
{
public:
    TestExtensionArea() : MExtensionArea() { }
    void addWidget(MWidget *widget, MDataStore &store) {
        MExtensionArea::addWidget(widget, store);
    }
    void removeWidget(MWidget *widget) {
        MExtensionArea::removeWidget(widget);
    }
};

// The test class
void Ut_MExtensionArea::init()
{
    area = new TestExtensionArea();
    gMAppletInstanceManagerStub->stubReset();
}

void Ut_MExtensionArea::cleanup()
{
    // Destroy the extension area.
    delete area;
}

void Ut_MExtensionArea::initTestCase()
{
    // MApplications must be created manually due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_mextensionarea";
    app = new MApplication(argc, &app_name);
}

void Ut_MExtensionArea::cleanupTestCase()
{
     delete app;
}

void Ut_MExtensionArea::testAddition()
{
    QSignalSpy spy(area->model(), SIGNAL(modified(QList<const char *>)));

    MWidget *widget1 = new MWidget;
    MockDataStore store1;
    MWidget *widget2 = new MWidget;
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

void Ut_MExtensionArea::testRemoval()
{
    QSignalSpy spy(area->model(), SIGNAL(modified(QList<const char *>)));

    MWidget *widget1 = new MWidget;
    MockDataStore store1;
    MWidget *widget2 = new MWidget;
    MockDataStore store2;
    MWidget *widget3 = new MWidget;
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

QTEST_APPLESS_MAIN(Ut_MExtensionArea)
