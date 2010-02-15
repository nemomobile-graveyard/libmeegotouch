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

#include "ut_duisettingslanguageselectionfactory.h"

#include "duisettingslanguagewidget.h"
#include "duisettingslanguagebinary.h"
#include "duisettingslanguageselectionfactory.h"
#include "duisettingslanguageselection_stub.h"
#include "duisettingslanguageselectioncontroller_stub.h"
#include "duisettingslanguageoption_stub.h"

#include <DuiApplication>
#include <DuiButtonGroup>
#include <DuiButton>
#include <QtTest/QtTest>

QList<QString> Ut_DuiSettingsLanguageSelectionFactory::buttonGroupButtonTitles;
QList<QString> Ut_DuiSettingsLanguageSelectionFactory::buttonGroupButtonKeys;
QList<int> Ut_DuiSettingsLanguageSelectionFactory::buttonGroupButtonValues;

// DuiButtonGroup stubs
void DuiButtonGroup::addButton(DuiButton *button)
{
    Ut_DuiSettingsLanguageSelectionFactory::buttonGroupButtonTitles.append(button->text());
    Ut_DuiSettingsLanguageSelectionFactory::buttonGroupButtonKeys.append(button->property("key").toString());
    Ut_DuiSettingsLanguageSelectionFactory::buttonGroupButtonValues.append(button->property("value").toInt());
}

// Tests
void Ut_DuiSettingsLanguageSelectionFactory::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_duisettingslanguageselectionfactory" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiSettingsLanguageSelectionFactory::cleanupTestCase()
{
    delete app;
}

void Ut_DuiSettingsLanguageSelectionFactory::init()
{
    // Empty the button lists
    buttonGroupButtonTitles.clear();
    buttonGroupButtonKeys.clear();
    buttonGroupButtonValues.clear();
    gDuiSettingsLanguageSelectionStub->stubReset();
    gDuiSettingsLanguageOptionStub->stubReset();
}

void Ut_DuiSettingsLanguageSelectionFactory::cleanup()
{
}

void Ut_DuiSettingsLanguageSelectionFactory::testBuildWidget()
{
    // Create a settings enum of two values
    DuiSettingsLanguageSelection se("TestKey");
    se.addOption("TestValue0", 0);
    se.addOption("TestValue1", 1);
    DuiSettingsLanguageOption *option0 = new DuiSettingsLanguageOption("StubTitle", 5);
    DuiSettingsLanguageOption *option1 = new DuiSettingsLanguageOption("StubTitle", 5);
    QList<const DuiSettingsLanguageOption *> options;
    options.append(option0);
    options.append(option1);
    gDuiSettingsLanguageSelectionStub->stubSetReturnValue("key", QString("TestKey"));
    gDuiSettingsLanguageSelectionStub->stubSetReturnValue("options", options);
    gDuiSettingsLanguageOptionStub->stubSetReturnValue("title", QString("StubTitle"));
    gDuiSettingsLanguageOptionStub->stubSetReturnValue("value", 5);
    DuiSettingsLanguageWidget dds;
    DuiWidget *widget = DuiSettingsLanguageSelectionFactory::createWidget(se, dds);
    QVERIFY(widget != NULL);

    // Two buttons should be created
    QCOMPARE(gDuiSettingsLanguageOptionStub->stubCallCount("title"), 2);
    QCOMPARE(gDuiSettingsLanguageOptionStub->stubCallCount("value"), 2 * 2);
    QCOMPARE(buttonGroupButtonTitles.count(), 2);
    QCOMPARE(buttonGroupButtonTitles.at(0), QString("StubTitle"));
    QCOMPARE(buttonGroupButtonTitles.at(1), QString("StubTitle"));
    QCOMPARE(buttonGroupButtonKeys.count(), 2);
    QCOMPARE(buttonGroupButtonKeys.at(0), QString("TestKey"));
    QCOMPARE(buttonGroupButtonKeys.at(1), QString("TestKey"));
    QCOMPARE(buttonGroupButtonValues.count(), 2);
    QCOMPARE(buttonGroupButtonValues.at(0), 5);
    QCOMPARE(buttonGroupButtonValues.at(1), 5);
    delete option0;
    delete option1;
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageSelectionFactory)
