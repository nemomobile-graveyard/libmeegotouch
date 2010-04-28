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

#include <QtTest/QtTest>
#include <MLayout>
#include <MApplication>
#include <MLinearLayoutPolicy>
#include "ut_msettingslanguagewidgetfactory.h"
#include "msettingslanguagebinary_stub.h"
#include "msettingslanguagewidgetfactory.h"
#include "msettingslanguagesettingsfactory_stub.h"
#include "msettingslanguagewidget_stub.h"
#include "msettingslanguagesettings_stub.h"
#include "mockdatastore.h"

// Tests
void Ut_MSettingsLanguageWidgetFactory::initTestCase()
{
    // MApplication must exist
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_msettingslanguageintegerfactory" };
    app = new MApplication(argc, app_name);
}

void Ut_MSettingsLanguageWidgetFactory::cleanupTestCase()
{
    // Destroy the MApplication
    delete app;
}

void Ut_MSettingsLanguageWidgetFactory::init()
{
}

void Ut_MSettingsLanguageWidgetFactory::cleanup()
{
}


void Ut_MSettingsLanguageWidgetFactory::testChildrenCreation()
{
    // Create a root with one item
    MSettingsLanguageSettings *item = new MSettingsLanguageSettings();
    MSettingsLanguageBinary binary;
    binary.addChild(item);

    // Create stub return values
    MWidgetController widgetController;
    QList<MSettingsLanguageNode *> children;
    children.append(item);
    gMSettingsLanguageBinaryStub->stubSetReturnValue("children", children);
    gMSettingsLanguageSettingsFactoryStub->stubSetReturnValue("createWidget", &widgetController);

    MockDataStore dataStore;
    MWidget *widget = MSettingsLanguageWidgetFactory::createWidget(binary, &dataStore);
    QVERIFY(widget != NULL);
    QCOMPARE(gMSettingsLanguageSettingsFactoryStub->stubLastCallTo("createWidget").parameter<const MSettingsLanguageSettings *>(0), item);
    QCOMPARE(gMSettingsLanguageSettingsFactoryStub->stubLastCallTo("createWidget").parameter<MDataStore *>(2), &dataStore);
}

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageWidgetFactory)
