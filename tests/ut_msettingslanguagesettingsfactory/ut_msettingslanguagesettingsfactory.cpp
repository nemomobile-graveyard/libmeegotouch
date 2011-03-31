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

#include "ut_msettingslanguagesettingsfactory.h"
#include "msettingslanguagesettingsfactory.h"
#include "msettingslanguagesettings_stub.h"
#include "msettingslanguageselection_stub.h"
#include "msettingslanguageselectionfactory_stub.h"
#include "msettingslanguagetextfactory_stub.h"
#include "msettingslanguageboolean_stub.h"
#include "msettingslanguagebooleanfactory_stub.h"
#include "msettingslanguageinteger_stub.h"
#include "msettingslanguageintegerfactory_stub.h"
#include "msettingslanguagegroup_stub.h"
#include "msettingslanguagegroupfactory_stub.h"
#include "msettingslanguagenode.h"
#include "msettingslanguagetext_stub.h"
#include "msettingslanguagewidget_stub.h"

#include <QtTest/QtTest>
#include <MApplication>
#include <MImageWidget>
#include <MButton>
#include <MContainer>
#include <MDataStore>
#include "../stubs/mockdatastore.h"

int Ut_MSettingsLanguageSettingsFactory::indicatorWidgets;
int Ut_MSettingsLanguageSettingsFactory::containerIndicators;
QList<QString> Ut_MSettingsLanguageSettingsFactory::containers;

QList<const MSettingsLanguageSelection *> Ut_MSettingsLanguageSettingsFactory::settingsSelections;
QList<const MSettingsLanguageText *> Ut_MSettingsLanguageSettingsFactory::settingsTexts;
QList<const MSettingsLanguageBoolean *> Ut_MSettingsLanguageSettingsFactory::settingsBooleans;
QList<const MSettingsLanguageInteger *> Ut_MSettingsLanguageSettingsFactory::settingsIntegers;
QList<MDataStore *> Ut_MSettingsLanguageSettingsFactory::settingsSelectionsDataStores;
QList<MDataStore *> Ut_MSettingsLanguageSettingsFactory::settingsTextsDataStores;
QList<MDataStore *> Ut_MSettingsLanguageSettingsFactory::settingsBooleansDataStores;
QList<MDataStore *> Ut_MSettingsLanguageSettingsFactory::settingsIntegersDataStores;
MWidgetController *Ut_MSettingsLanguageSettingsFactory::mSettingsLanguageSelectionFactoryWidget = NULL;

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// MImageWidget stubs
void MImageWidget::setImage(const QString &, const QSize &)
{
    Ut_MSettingsLanguageSettingsFactory::indicatorWidgets++;
}

// MContainer stubs
void MContainer::setIconID(const QString &iconId)
{
    Q_UNUSED(iconId);
    Ut_MSettingsLanguageSettingsFactory::containerIndicators++;
}

void MContainer::setCentralWidget(QGraphicsWidget *centralWidget, bool destroy)
{
    Q_UNUSED(centralWidget);
    Q_UNUSED(destroy);
    Ut_MSettingsLanguageSettingsFactory::containers.append(title());
}

// Tests
void Ut_MSettingsLanguageSettingsFactory::initTestCase()
{
    // Create a MApplication
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_msettingslanguagesettingsfactory" };
    app = new MApplication(argc, app_name);
}

void Ut_MSettingsLanguageSettingsFactory::cleanupTestCase()
{
    // Destroy MApplication
    delete app;
}

void Ut_MSettingsLanguageSettingsFactory::init()
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
    mSettingsLanguageSelectionFactoryWidget = NULL;
    dataStore = new MockDataStore;
    gMSettingsLanguageSettingsStub->stubReset();
}

void Ut_MSettingsLanguageSettingsFactory::cleanup()
{
    delete dataStore;
    dataStore = NULL;
}

void Ut_MSettingsLanguageSettingsFactory::testChildrenCreation()
{
    // Create a settings item with a selection of two values, text and group with boolean and integer
    MSettingsLanguageSelection *aSelection = new MSettingsLanguageSelection("EnumTestKey", "selectionTitle");
    aSelection->addOption("TestValue0", 0);
    aSelection->addOption("TestValue1", 1);
    MSettingsLanguageText *text = new MSettingsLanguageText("TextTestKey", "title");

    // Group that boolean and integer are added. If then there are those items, there must be a group..
    MSettingsLanguageGroup *group = new MSettingsLanguageGroup("groupTitle");
    MSettingsLanguageBoolean *booleanNode = new MSettingsLanguageBoolean("BoolTestKey", "titleBool");
    MSettingsLanguageInteger *intNode = new MSettingsLanguageInteger("IntTestKey", "titleInt");

    MSettingsLanguageSettings item;
    item.addChild(group);
    item.addChild(aSelection);
    item.addChild(text);
    item.addChild(booleanNode);
    item.addChild(intNode);

    // Set up the stub to return the same values
    QList<MSettingsLanguageNode *> children;
    children.append(group);
    children.append(aSelection);
    children.append(text);
    children.append(booleanNode);
    children.append(intNode);
    gMSettingsLanguageSettingsStub->stubSetReturnValue("children", children);

    MSettingsLanguageWidget dds;
    MWidget *widget = MSettingsLanguageSettingsFactory::createWidget(item, dds, dataStore);
    QVERIFY(widget != NULL);
    QCOMPARE(gMSettingsLanguageSelectionFactoryStub->stubCallCount("createWidget"), 1);
    QCOMPARE(gMSettingsLanguageSelectionFactoryStub->stubLastCallTo("createWidget").parameter<const MSettingsLanguageSelection *>(0), aSelection);
    QCOMPARE(gMSettingsLanguageTextFactoryStub->stubCallCount("createWidget"), 1);
    QCOMPARE(gMSettingsLanguageTextFactoryStub->stubLastCallTo("createWidget").parameter<const MSettingsLanguageText *>(0), text);
    QCOMPARE(gMSettingsLanguageBooleanFactoryStub->stubCallCount("createWidget"), 1);
    QCOMPARE(gMSettingsLanguageBooleanFactoryStub->stubLastCallTo("createWidget").parameter<const MSettingsLanguageBoolean *>(0), booleanNode);
    QCOMPARE(gMSettingsLanguageIntegerFactoryStub->stubCallCount("createWidget"), 1);
    QCOMPARE(gMSettingsLanguageIntegerFactoryStub->stubLastCallTo("createWidget").parameter<const MSettingsLanguageInteger *>(0), intNode);
    QCOMPARE(gMSettingsLanguageGroupFactoryStub->stubCallCount("createWidget"), 1);
    QCOMPARE(gMSettingsLanguageGroupFactoryStub->stubLastCallTo("createWidget").parameter<const MSettingsLanguageGroup *>(0), group);
}

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageSettingsFactory)
