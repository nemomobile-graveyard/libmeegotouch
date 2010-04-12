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

#include "ut_msettingslanguagebooleancontroller.h"
#include "msettingslanguagebooleancontroller.h"
#include "msettingslanguagebooleanfactory.h"
#include "msettingslanguageboolean.h"
#include "msettingslanguagewidget.h"

#include <QtTest/QtTest>
#include <MApplication>
#include <QApplication>
#include <MButton>
#include <MDataStore>
#include "../stubs/mockdatastore.h"

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

void Ut_MSettingsLanguageBooleanController::init()
{
}

void Ut_MSettingsLanguageBooleanController::cleanup()
{
}

void Ut_MSettingsLanguageBooleanController::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_msettingslanguagebooleancontroller" };
    app = new MApplication(argc, app_name);
}

void Ut_MSettingsLanguageBooleanController::cleanupTestCase()
{
    delete app;
}

void Ut_MSettingsLanguageBooleanController::testToggleButton()
{
    // Create a settings boolean
    MSettingsLanguageBoolean se("testKey", "Toggle Button1");
    MSettingsLanguageWidget dds;
    MockDataStore ddatas;
    ddatas.createValue(se.key(), QVariant(false));

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

    // Wait for button model events.. shouldn't be needed, but known problem.
    // TODO: can be removed after fixed to m.
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    MDataStore *dataStore = static_cast<MDataStore *>(button->property("dataStore").value<void *>());
    QVERIFY(!button->isChecked());

    button->toggle();

    QVERIFY(button->isChecked());
    QCOMPARE(dataStore->value("testKey").toBool(), true);
}

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageBooleanController)
