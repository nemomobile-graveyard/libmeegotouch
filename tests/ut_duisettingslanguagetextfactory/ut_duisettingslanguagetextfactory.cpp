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

#include <QGraphicsLinearLayout>
#include <QVariant>
#include <duiapplication.h>
#include <duiapplicationwindow.h>
#include <duisettingslanguagetextfactory.h>
#include <duisettingslanguagebinary_stub.h>
#include <duisettingslanguagewidget_stub.h>
#include <duisettingslanguagesettings_stub.h>
#include <duisettingslanguagetext_stub.h>
#include <duisettingslanguagetextcontroller_stub.h>
#include <duilabel.h>
#include <duitextedit.h>
#include <duidatastore.h>
#include "ut_duisettingslanguagetextfactory.h"
#include "mockdatastore.h"

void Ut_DuiSettingsLanguageTextFactory::initTestCase()
{
    // DuiApplication and DuiApplicationWindow must exist
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_duisettingslanguagetextfactory" };
    app = new DuiApplication(argc, app_name);
    appWin = new DuiApplicationWindow;
}

void Ut_DuiSettingsLanguageTextFactory::cleanupTestCase()
{
    // Destroy the DuiApplicationWindow
    delete appWin;
    // Destroy the DuiApplication
    delete app;
}

void Ut_DuiSettingsLanguageTextFactory::init()
{
}

void Ut_DuiSettingsLanguageTextFactory::cleanup()
{
}

void Ut_DuiSettingsLanguageTextFactory::testCreateWidget()
{
    // Create a settings text
    DuiSettingsLanguageText settingsText("TestKey", "Title");
    DuiSettingsLanguageWidget dds;
    MockDataStore dataStore;
    DuiWidget *widget = DuiSettingsLanguageTextFactory::createWidget(settingsText, dds, &dataStore);
    QVERIFY(widget != NULL);

    // Expecting the widget to have a QGraphicsLinearLayout
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(widget->layout());
    QVERIFY(layout != NULL);

    // Expecting the layout to contain a DuiLabel and a DuiTextEdit
    QCOMPARE(layout->count(), 2);

    // The label's title should be the SettingsText's title
    DuiLabel *label = dynamic_cast<DuiLabel *>(layout->itemAt(0));
    QVERIFY(label != NULL);
    QCOMPARE(label->text(), settingsText.title());

    // The label's title should be the specified key's value
    DuiTextEdit *textEdit = dynamic_cast<DuiTextEdit *>(layout->itemAt(1));
    QVERIFY(textEdit != NULL);
    QCOMPARE(textEdit->text(), dataStore.value("TestKey").toString());
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageTextFactory)
