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

#include <QGraphicsLinearLayout>
#include <QVariant>
#include <duiapplication.h>
#include <duisettingslanguageinteger_stub.h>
#include <duisettingslanguageintegerfactory.h>
#include <duisettingslanguageintegercontroller_stub.h>
#include <duisettingslanguagebinary_stub.h>
#include <duisettingslanguagewidget_stub.h>
#include <duisettingslanguagesettings_stub.h>
#include <duilabel.h>
#include <duislider.h>
#include <duidatastore.h>
#include "ut_duisettingslanguageintegerfactory.h"
#include "mockdatastore.h"

class DuiSettingsLanguageInteger;

void Ut_DuiSettingsLanguageIntegerFactory::initTestCase()
{
    // DuiApplication must exist
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_duisettingslanguageintegerfactory" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiSettingsLanguageIntegerFactory::cleanupTestCase()
{
    // Destroy the DuiApplication
    delete app;
}

void Ut_DuiSettingsLanguageIntegerFactory::init()
{
}

void Ut_DuiSettingsLanguageIntegerFactory::cleanup()
{
}

void Ut_DuiSettingsLanguageIntegerFactory::testCreateWidget()
{
    // Create a settings integer
    DuiSettingsLanguageInteger settingsInteger("TestKey", "Title");
    settingsInteger.setMinValue(0);
    settingsInteger.setMaxValue(1000);
    gDuiSettingsLanguageIntegerStub->stubSetReturnValue("minValue", true);
    gDuiSettingsLanguageIntegerStub->stubSetReturnValue("maxValue", true);
    gDuiSettingsLanguageIntegerStub->stubSetReturnValue("minValueValue", 0);
    gDuiSettingsLanguageIntegerStub->stubSetReturnValue("maxValueValue", 1000);
    DuiSettingsLanguageWidget dds;
    MockDataStore dataStore;
    DuiWidget *widget = DuiSettingsLanguageIntegerFactory::createWidget(settingsInteger, dds, &dataStore);
    QVERIFY(widget != NULL);

    // Expecting the widget to have a QGraphicsLinearLayout
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(widget->layout());
    QVERIFY(layout != NULL);

    // Expecting the layout to contain a DuiLabel and a DuiSlider
    QCOMPARE(layout->count(), 2);

    // The label's text should be the SettingsInteger's title
    DuiLabel *label = dynamic_cast<DuiLabel *>(layout->itemAt(0));
    QVERIFY(label != NULL);
    QCOMPARE(label->text(), settingsInteger.title());

    // The slider's value should be the specified key's value
    DuiSlider *slider = dynamic_cast<DuiSlider *>(layout->itemAt(1));
    QVERIFY(slider != NULL);
    QCOMPARE(slider->value(), dataStore.value("TestKey").toInt());
    QCOMPARE(slider->minimum(), 0);
    QCOMPARE(slider->maximum(), 1000);
}

void Ut_DuiSettingsLanguageIntegerFactory::testCreateWidgetWithKeyValueSet()
{
    MockDataStore dataStore;
    // Create the key value before creating the setting and the widget
    dataStore.createValue(QString("AnotherTestKey"), 12);

    // Create a settings integer
    DuiSettingsLanguageInteger settingsInteger("AnotherTestKey", "Title");
    settingsInteger.setMinValue(0);
    settingsInteger.setMaxValue(1000);
    gDuiSettingsLanguageIntegerStub->stubSetReturnValue("key", QString("AnotherTestKey"));
    gDuiSettingsLanguageIntegerStub->stubSetReturnValue("title", QString("Title"));
    DuiSettingsLanguageWidget dds;
    DuiWidget *widget = DuiSettingsLanguageIntegerFactory::createWidget(settingsInteger, dds, &dataStore);
    QVERIFY(widget != NULL);

    // Expecting the widget to have a QGraphicsLinearLayout
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(widget->layout());
    QVERIFY(layout != NULL);

    // Expecting the layout to contain a DuiLabel and a DuiSlider
    QCOMPARE(layout->count(), 2);

    // The label's text should be the SettingsInteger's title
    DuiLabel *label = dynamic_cast<DuiLabel *>(layout->itemAt(0));
    QVERIFY(label != NULL);
    QCOMPARE(label->text(), settingsInteger.title());

    // The slider's value should be the specified key's value
    DuiSlider *slider = dynamic_cast<DuiSlider *>(layout->itemAt(1));
    QVERIFY(slider != NULL);
    QCOMPARE(slider->value(), dataStore.value("AnotherTestKey").toInt());
    int minVal;
    QVERIFY(settingsInteger.minValue(minVal));
    QCOMPARE(slider->minimum(), minVal);
    int maxVal;
    QVERIFY(settingsInteger.maxValue(maxVal));
    QCOMPARE(slider->maximum(), maxVal);
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageIntegerFactory)
