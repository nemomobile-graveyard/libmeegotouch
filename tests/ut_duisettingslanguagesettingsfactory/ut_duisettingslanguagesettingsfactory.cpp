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

#include "ut_duisettingslanguagesettingsfactory.h"
#include "duisettingslanguagesettingsfactory.h"
#include "duisettingslanguagesettings_stub.h"
#include "duisettingslanguageselection_stub.h"
#include "duisettingslanguageselectionfactory_stub.h"
#include "duisettingslanguagetextfactory_stub.h"
#include "duisettingslanguageboolean_stub.h"
#include "duisettingslanguagebooleanfactory_stub.h"
#include "duisettingslanguageinteger_stub.h"
#include "duisettingslanguageintegerfactory_stub.h"
#include "duisettingslanguagenode.h"
#include "duisettingslanguagetext_stub.h"
#include "duisettingslanguagegroup_stub.h"
#include "duisettingslanguagewidget_stub.h"

#include <QtTest/QtTest>
#include <DuiApplication>
#include <DuiImageWidget>
#include <DuiButton>
#include <DuiContainer>
#include <DuiDataStore>
#include "../stubs/mockdatastore.h"

int Ut_DuiSettingsLanguageSettingsFactory::indicatorWidgets;
int Ut_DuiSettingsLanguageSettingsFactory::containerIndicators;
QList<QString> Ut_DuiSettingsLanguageSettingsFactory::containers;

QList<const DuiSettingsLanguageSelection *> Ut_DuiSettingsLanguageSettingsFactory::settingsSelections;
QList<const DuiSettingsLanguageText *> Ut_DuiSettingsLanguageSettingsFactory::settingsTexts;
QList<const DuiSettingsLanguageBoolean *> Ut_DuiSettingsLanguageSettingsFactory::settingsBooleans;
QList<const DuiSettingsLanguageInteger *> Ut_DuiSettingsLanguageSettingsFactory::settingsIntegers;
QList<DuiDataStore *> Ut_DuiSettingsLanguageSettingsFactory::settingsSelectionsDataStores;
QList<DuiDataStore *> Ut_DuiSettingsLanguageSettingsFactory::settingsTextsDataStores;
QList<DuiDataStore *> Ut_DuiSettingsLanguageSettingsFactory::settingsBooleansDataStores;
QList<DuiDataStore *> Ut_DuiSettingsLanguageSettingsFactory::settingsIntegersDataStores;
DuiWidgetController *Ut_DuiSettingsLanguageSettingsFactory::duiSettingsLanguageSelectionFactoryWidget = NULL;

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// DuiImageWidget stubs
void DuiImageWidget::setImage(const QString &, const QSize &)
{
    Ut_DuiSettingsLanguageSettingsFactory::indicatorWidgets++;
}

// DuiContainer stubs
void DuiContainer::setIconID(const QString &iconId)
{
    Q_UNUSED(iconId);
    Ut_DuiSettingsLanguageSettingsFactory::containerIndicators++;
}

void DuiContainer::setCentralWidget(QGraphicsWidget *centralWidget, bool destroy)
{
    Q_UNUSED(centralWidget);
    Q_UNUSED(destroy);
    Ut_DuiSettingsLanguageSettingsFactory::containers.append(title());
}

// Tests
void Ut_DuiSettingsLanguageSettingsFactory::initTestCase()
{
    // Create a DuiApplication
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duisettingslanguagesettingsfactory" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiSettingsLanguageSettingsFactory::cleanupTestCase()
{
    // Destroy DuiApplication
    delete app;
}

void Ut_DuiSettingsLanguageSettingsFactory::init()
{
    indicatorWidgets = 0;
    containerIndicators = 0;
    containers.clear();
    settingsSelections.clear();
    settingsTexts.clear();
    settingsBooleans.clear();
    settingsIntegers.clear();
    settingsSelectionsDataStores.clear();
    settingsTextsDataStores.clear();
    settingsBooleansDataStores.clear();
    settingsIntegersDataStores.clear();
    duiSettingsLanguageSelectionFactoryWidget = NULL;
    dataStore = new MockDataStore;
    gDuiSettingsLanguageSettingsStub->stubReset();
}

void Ut_DuiSettingsLanguageSettingsFactory::cleanup()
{
    delete dataStore;
    dataStore = NULL;
}

void Ut_DuiSettingsLanguageSettingsFactory::testChildrenCreation()
{
    // Create a settings item with a selection of two values, text and group with boolean and integer
    DuiSettingsLanguageSelection *aSelection = new DuiSettingsLanguageSelection("EnumTestKey");
    aSelection->addOption("TestValue0", 0);
    aSelection->addOption("TestValue1", 1);
    DuiSettingsLanguageText *text = new DuiSettingsLanguageText("TextTestKey", "title");

    // Group that boolean and integer are added. If then there are those items, there must be a group..
    DuiSettingsLanguageGroup *group = new DuiSettingsLanguageGroup();
    DuiSettingsLanguageBoolean *booleanNode = new DuiSettingsLanguageBoolean("BoolTestKey", "titleBool");
    DuiSettingsLanguageInteger *intNode = new DuiSettingsLanguageInteger("IntTestKey", "titleInt");

    group->addChild(intNode);
    group->addChild(booleanNode);

    DuiSettingsLanguageSettings item;
    item.addChild(group);
    item.addChild(aSelection);
    item.addChild(text);

    // Set up the stub to return the same values
    QList<DuiSettingsLanguageNode *> children;
    children.append(group);
    children.append(aSelection);
    children.append(text);
    gDuiSettingsLanguageSettingsStub->stubSetReturnValue("children", children);

    DuiSettingsLanguageWidget dds;
    DuiWidget *widget = DuiSettingsLanguageSettingsFactory::createWidget(item, dds, dataStore);
    QVERIFY(widget != NULL);
    QCOMPARE(gDuiSettingsLanguageSelectionFactoryStub->stubCallCount("createWidget"), 1);
    QCOMPARE(gDuiSettingsLanguageSelectionFactoryStub->stubLastCallTo("createWidget").parameter<const DuiSettingsLanguageSelection *>(0), aSelection);
    QCOMPARE(gDuiSettingsLanguageTextFactoryStub->stubCallCount("createWidget"), 1);
    QCOMPARE(gDuiSettingsLanguageTextFactoryStub->stubLastCallTo("createWidget").parameter<const DuiSettingsLanguageText *>(0), text);
    QCOMPARE(gDuiSettingsLanguageBooleanFactoryStub->stubCallCount("createWidget"), 1);
    QCOMPARE(gDuiSettingsLanguageBooleanFactoryStub->stubLastCallTo("createWidget").parameter<const DuiSettingsLanguageBoolean *>(0), booleanNode);
    QCOMPARE(gDuiSettingsLanguageIntegerFactoryStub->stubCallCount("createWidget"), 1);
    QCOMPARE(gDuiSettingsLanguageIntegerFactoryStub->stubLastCallTo("createWidget").parameter<const DuiSettingsLanguageInteger *>(0), intNode);
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageSettingsFactory)
