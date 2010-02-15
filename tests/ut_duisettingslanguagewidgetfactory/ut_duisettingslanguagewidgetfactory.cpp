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

#include <QtTest/QtTest>
#include <QGraphicsLinearLayout>
#include "ut_duisettingslanguagewidgetfactory.h"
#include "duisettingslanguagebinary_stub.h"
#include "duisettingslanguagewidgetfactory.h"
#include "duisettingslanguagesettingsfactory_stub.h"
#include "duisettingslanguagewidget_stub.h"
#include "duisettingslanguagesettings_stub.h"
#include "mockdatastore.h"

void QGraphicsItem::setParentItem(QGraphicsItem *)
{
}

void QGraphicsLinearLayout::insertItem(int, QGraphicsLayoutItem *)
{
}

// Tests
void Ut_DuiSettingsLanguageWidgetFactory::initTestCase()
{
}

void Ut_DuiSettingsLanguageWidgetFactory::cleanupTestCase()
{
}

void Ut_DuiSettingsLanguageWidgetFactory::init()
{
}

void Ut_DuiSettingsLanguageWidgetFactory::cleanup()
{
}


void Ut_DuiSettingsLanguageWidgetFactory::testChildrenCreation()
{
    // Create a root with one item
    DuiSettingsLanguageSettings *item = new DuiSettingsLanguageSettings();
    DuiSettingsLanguageBinary binary;
    binary.addChild(item);

    // Create stub return values
    DuiWidgetController widgetController;
    QList<DuiSettingsLanguageNode *> children;
    children.append(item);
    gDuiSettingsLanguageBinaryStub->stubSetReturnValue("children", children);
    gDuiSettingsLanguageSettingsFactoryStub->stubSetReturnValue("createWidget", &widgetController);

    MockDataStore dataStore;
    DuiWidget *widget = DuiSettingsLanguageWidgetFactory::createWidget(binary, &dataStore);
    QVERIFY(widget != NULL);
    QCOMPARE(gDuiSettingsLanguageSettingsFactoryStub->stubLastCallTo("createWidget").parameter<const DuiSettingsLanguageSettings *>(0), item);
    QCOMPARE(gDuiSettingsLanguageSettingsFactoryStub->stubLastCallTo("createWidget").parameter<DuiDataStore *>(2), &dataStore);
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageWidgetFactory)
