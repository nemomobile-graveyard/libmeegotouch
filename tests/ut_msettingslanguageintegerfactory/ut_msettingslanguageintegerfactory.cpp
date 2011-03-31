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

#include <QGraphicsLinearLayout>
#include <QVariant>
#include <mapplication.h>
#include <msettingslanguageinteger_stub.h>
#include <msettingslanguageintegerfactory.h>
#include <msettingslanguageintegercontroller_stub.h>
#include <msettingslanguagebinary_stub.h>
#include <msettingslanguagewidget_stub.h>
#include <msettingslanguagesettings_stub.h>
#include <mlabel.h>
#include <mslider.h>
#include <mdatastore.h>
#include "ut_msettingslanguageintegerfactory.h"
#include "mockdatastore.h"

class MSettingsLanguageInteger;

void Ut_MSettingsLanguageIntegerFactory::initTestCase()
{
    // MApplication must exist
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_msettingslanguageintegerfactory" };
    app = new MApplication(argc, app_name);
}

void Ut_MSettingsLanguageIntegerFactory::cleanupTestCase()
{
    // Destroy the MApplication
    delete app;
}

void Ut_MSettingsLanguageIntegerFactory::init()
{
}

void Ut_MSettingsLanguageIntegerFactory::cleanup()
{
}

void Ut_MSettingsLanguageIntegerFactory::testCreateWidget()
{
    // Create a settings integer
    MSettingsLanguageInteger settingsInteger("TestKey", "Title");
    settingsInteger.setMinValue(0);
    settingsInteger.setMaxValue(1000);
    gMSettingsLanguageIntegerStub->stubSetReturnValue("minValue", true);
    gMSettingsLanguageIntegerStub->stubSetReturnValue("maxValue", true);
    gMSettingsLanguageIntegerStub->stubSetReturnValue("minValueValue", 0);
    gMSettingsLanguageIntegerStub->stubSetReturnValue("maxValueValue", 1000);
    MSettingsLanguageWidget dds;
    MockDataStore dataStore;
    MWidget *widget = MSettingsLanguageIntegerFactory::createWidget(settingsInteger, dds, &dataStore);
    QVERIFY(widget != NULL);

    // Expecting the widget to have a linear layout
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(widget->layout());
    QVERIFY(layout != NULL);

    // Expecting the layout to contain a MLabel and a MSlider
    QCOMPARE(layout->count(), 2);

    // The label's text should be the SettingsInteger's title
    MLabel *label = dynamic_cast<MLabel *>(layout->itemAt(0));
    QVERIFY(label != NULL);
    QCOMPARE(label->text(), settingsInteger.title());

    // The slider's value should be the specified key's value
    MSlider *slider = dynamic_cast<MSlider *>(layout->itemAt(1));
    QVERIFY(slider != NULL);
    QCOMPARE(slider->value(), dataStore.value("TestKey").toInt());
    QCOMPARE(slider->minimum(), 0);
    QCOMPARE(slider->maximum(), 1000);
}

void Ut_MSettingsLanguageIntegerFactory::testCreateWidgetWithKeyValueSet()
{
    MockDataStore dataStore;
    // Create the key value before creating the setting and the widget
    dataStore.createValue(QString("AnotherTestKey"), 12);

    // Create a settings integer
    MSettingsLanguageInteger settingsInteger("AnotherTestKey", "Title");
    settingsInteger.setMinValue(0);
    settingsInteger.setMaxValue(1000);
    gMSettingsLanguageIntegerStub->stubSetReturnValue("key", QString("AnotherTestKey"));
    gMSettingsLanguageIntegerStub->stubSetReturnValue("title", QString("Title"));
    MSettingsLanguageWidget dds;
    MWidget *widget = MSettingsLanguageIntegerFactory::createWidget(settingsInteger, dds, &dataStore);
    QVERIFY(widget != NULL);

    // Expecting the widget to have a layout and linear policy
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(widget->layout());
    QVERIFY(layout != NULL);

    // Expecting the layout to contain a MLabel and a MSlider
    QCOMPARE(layout->count(), 2);

    // The label's text should be the SettingsInteger's title
    MLabel *label = dynamic_cast<MLabel *>(layout->itemAt(0));
    QVERIFY(label != NULL);
    QCOMPARE(label->text(), settingsInteger.title());

    // The slider's value should be the specified key's value
    MSlider *slider = dynamic_cast<MSlider *>(layout->itemAt(1));
    QVERIFY(slider != NULL);
    QCOMPARE(slider->value(), dataStore.value("AnotherTestKey").toInt());
    int minVal;
    QVERIFY(settingsInteger.minValue(minVal));
    QCOMPARE(slider->minimum(), minVal);
    int maxVal;
    QVERIFY(settingsInteger.maxValue(maxVal));
    QCOMPARE(slider->maximum(), maxVal);
}

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageIntegerFactory)
