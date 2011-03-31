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

#include "ut_msettingslanguageselectionfactory.h"

#include "msettingslanguagewidget.h"
#include "msettingslanguagebinary.h"
#include "msettingslanguageselectionfactory.h"
#include "msettingslanguageselection_stub.h"
#include "msettingslanguageselectioncontroller_stub.h"
#include "msettingslanguageoption_stub.h"

#include <MApplication>
#include <MButtonGroup>
#include <MButton>
#include <QtTest/QtTest>

QList<QString> Ut_MSettingsLanguageSelectionFactory::buttonGroupButtonTitles;
QList<QString> Ut_MSettingsLanguageSelectionFactory::buttonGroupButtonKeys;
QList<int> Ut_MSettingsLanguageSelectionFactory::buttonGroupButtonValues;

// MButtonGroup stubs
void MButtonGroup::addButton(MButton *button)
{
    Ut_MSettingsLanguageSelectionFactory::buttonGroupButtonTitles.append(button->text());
    Ut_MSettingsLanguageSelectionFactory::buttonGroupButtonKeys.append(button->property("key").toString());
    Ut_MSettingsLanguageSelectionFactory::buttonGroupButtonValues.append(button->property("value").toInt());
}

// Tests
void Ut_MSettingsLanguageSelectionFactory::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_msettingslanguageselectionfactory" };
    app = new MApplication(argc, app_name);
}

void Ut_MSettingsLanguageSelectionFactory::cleanupTestCase()
{
    delete app;
}

void Ut_MSettingsLanguageSelectionFactory::init()
{
    // Empty the button lists
    buttonGroupButtonTitles.clear();
    buttonGroupButtonKeys.clear();
    buttonGroupButtonValues.clear();
    gMSettingsLanguageSelectionStub->stubReset();
    gMSettingsLanguageOptionStub->stubReset();
}

void Ut_MSettingsLanguageSelectionFactory::cleanup()
{
}

void Ut_MSettingsLanguageSelectionFactory::testBuildWidget()
{
    // Create a settings enum of two values
    MSettingsLanguageSelection se("TestKey", "TestTitle");
    se.addOption("TestValue0", 0);
    se.addOption("TestValue1", 1);
    MSettingsLanguageOption *option0 = new MSettingsLanguageOption("StubTitle", 5);
    MSettingsLanguageOption *option1 = new MSettingsLanguageOption("StubTitle", 5);
    QList<const MSettingsLanguageOption *> options;
    options.append(option0);
    options.append(option1);
    gMSettingsLanguageSelectionStub->stubSetReturnValue("key", QString("TestKey"));
    gMSettingsLanguageSelectionStub->stubSetReturnValue("options", options);
    gMSettingsLanguageOptionStub->stubSetReturnValue("title", QString("StubTitle"));
    gMSettingsLanguageOptionStub->stubSetReturnValue("value", 5);
    MSettingsLanguageWidget dds;
    MWidget *widget = MSettingsLanguageSelectionFactory::createWidget(se, dds);
    QVERIFY(widget != NULL);

    // Two buttons should be created
    QCOMPARE(gMSettingsLanguageOptionStub->stubCallCount("title"), 2);
    QCOMPARE(gMSettingsLanguageOptionStub->stubCallCount("value"), 2 * 2);
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

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageSelectionFactory)
