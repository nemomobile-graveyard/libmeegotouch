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

#include "ut_msettingslanguagebooleanfactory.h"

#include "msettingslanguagebooleanfactory.h"
#include "msettingslanguagewidget_stub.h"
#include "msettingslanguageboolean_stub.h"
#include "msettingslanguagebooleancontroller_stub.h"
#include "mockdatastore.h"
#include <MDataStore>
#include <MApplication>
#include <QGraphicsLinearLayout>
#include <MButton>
#include <QtTest/QtTest>
#include <QLayoutItem>
#include <QDebug>

// Tests
void Ut_MSettingsLanguageBooleanFactory::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_msettingslanguagebooleanfactory" };
    app = new MApplication(argc, app_name);
}

void Ut_MSettingsLanguageBooleanFactory::cleanupTestCase()
{
    delete app;
}

void Ut_MSettingsLanguageBooleanFactory::init()
{
}

void Ut_MSettingsLanguageBooleanFactory::cleanup()
{
}

void Ut_MSettingsLanguageBooleanFactory::testBuildWidget()
{
    // widget build as button checked
    testBuildWidgetButtonCheck(true);

    // widget build as button unchecked
    testBuildWidgetButtonCheck(false);
}

void Ut_MSettingsLanguageBooleanFactory::testBuildWidgetButtonCheck(bool aIsChecked)
{
    // Create a settings boolean
    MSettingsLanguageBoolean se("testKey", "ToggleButton1");
    gMSettingsLanguageBooleanStub->stubSetReturnValue("key", QString("testKey"));
    gMSettingsLanguageBooleanStub->stubSetReturnValue("title", QString("ToggleButton1"));
    MSettingsLanguageWidget dds;
    MockDataStore ddatas;
    ddatas.createValue(se.key(), QVariant(aIsChecked));

    MWidgetController *widget = MSettingsLanguageBooleanFactory::createWidget(se, dds, &ddatas);
    QVERIFY(widget != NULL);

    // Just find the toggle button
    MButton *button = NULL;
    foreach(QGraphicsItem * child, widget->childItems()) {
        button = static_cast<MButton *>(child);
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

    MDataStore *dataStore = static_cast<MDataStore *>(button->property("dataStore").value<void *>());
    QCOMPARE(dataStore->value("testKey").toBool(), aIsChecked);

    delete widget;
}

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageBooleanFactory)
