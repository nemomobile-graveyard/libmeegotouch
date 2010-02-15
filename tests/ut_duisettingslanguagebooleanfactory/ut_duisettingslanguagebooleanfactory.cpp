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

#include "ut_duisettingslanguagebooleanfactory.h"

#include "duisettingslanguagebooleanfactory.h"
#include "duisettingslanguagewidget_stub.h"
#include "duisettingslanguageboolean_stub.h"
#include "duisettingslanguagebooleancontroller_stub.h"
#include "mockdatastore.h"
#include <DuiDataStore>
#include <DuiApplication>
#include <QGraphicsLinearLayout>
#include <DuiButton>
#include <QtTest/QtTest>
#include <QLayoutItem>
#include <QDebug>

// Tests
void Ut_DuiSettingsLanguageBooleanFactory::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_duisettingslanguagebooleanfactory" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiSettingsLanguageBooleanFactory::cleanupTestCase()
{
    delete app;
}

void Ut_DuiSettingsLanguageBooleanFactory::init()
{
}

void Ut_DuiSettingsLanguageBooleanFactory::cleanup()
{
}

void Ut_DuiSettingsLanguageBooleanFactory::testBuildWidget()
{
    // widget build as button checked
    testBuildWidgetButtonCheck(true);

    // widget build as button unchecked
    testBuildWidgetButtonCheck(false);
}

void Ut_DuiSettingsLanguageBooleanFactory::testBuildWidgetButtonCheck(bool aIsChecked)
{
    // Create a settings boolean
    DuiSettingsLanguageBoolean se("testKey", "ToggleButton1");
    gDuiSettingsLanguageBooleanStub->stubSetReturnValue("key", QString("testKey"));
    gDuiSettingsLanguageBooleanStub->stubSetReturnValue("title", QString("ToggleButton1"));
    DuiSettingsLanguageWidget dds;
    MockDataStore ddatas;
    ddatas.createValue(se.key(), QVariant(aIsChecked));

    DuiWidgetController *widget = DuiSettingsLanguageBooleanFactory::createWidget(se, dds, &ddatas);
    QVERIFY(widget != NULL);

    // Just find the toggle button
    DuiButton *button = NULL;
    foreach(QGraphicsItem * child, widget->childItems()) {
        button = static_cast<DuiButton *>(child);
        if (button != NULL && button->objectName() == "SettingsLanguageBooleanValueButton") {
            break;
        }
    }
    QVERIFY(button != NULL);

    QVERIFY(button->isCheckable());
    QVERIFY(button->isChecked() == aIsChecked);
    QCOMPARE(button->objectName(), QString("SettingsLanguageBooleanValueButton"));
    QCOMPARE(button->property("key").toString(), QString("testKey"));
    QCOMPARE(button->text(), QString("ToggleButton1"));

    DuiDataStore *dataStore = static_cast<DuiDataStore *>(button->property("dataStore").value<void *>());
    QCOMPARE(dataStore->value("testKey").toBool(), aIsChecked);

    delete widget;
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageBooleanFactory)
