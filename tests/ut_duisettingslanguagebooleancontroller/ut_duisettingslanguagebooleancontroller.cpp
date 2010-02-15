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

#include "ut_duisettingslanguagebooleancontroller.h"
#include "duisettingslanguagebooleancontroller.h"
#include "duisettingslanguagebooleanfactory.h"
#include "duisettingslanguageboolean.h"
#include "duisettingslanguagewidget.h"

#include <QtTest/QtTest>
#include <DuiApplication>
#include <QApplication>
#include <DuiButton>
#include <DuiDataStore>
#include "../stubs/mockdatastore.h"

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

void Ut_DuiSettingsLanguageBooleanController::init()
{
}

void Ut_DuiSettingsLanguageBooleanController::cleanup()
{
}

void Ut_DuiSettingsLanguageBooleanController::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_duisettingslanguagebooleancontroller" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiSettingsLanguageBooleanController::cleanupTestCase()
{
    delete app;
}

void Ut_DuiSettingsLanguageBooleanController::testToggleButton()
{
    // Create a settings boolean
    DuiSettingsLanguageBoolean se("testKey", "Toggle Button1");
    DuiSettingsLanguageWidget dds;
    MockDataStore ddatas;
    ddatas.createValue(se.key(), QVariant(false));

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

    // Wait for button model events.. shouldn't be needed, but known problem.
    // TODO: can be removed after fixed to dui.
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    DuiDataStore *dataStore = static_cast<DuiDataStore *>(button->property("dataStore").value<void *>());
    QVERIFY(!button->isChecked());

    button->toggle();

    QVERIFY(button->isChecked());
    QCOMPARE(dataStore->value("testKey").toBool(), true);
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageBooleanController)
