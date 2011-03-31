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

#include <QGraphicsLinearLayout>
#include <QVariant>
#include <mapplication.h>
#include <mapplicationwindow.h>
#include <msettingslanguagetextfactory.h>
#include <msettingslanguagebinary_stub.h>
#include <msettingslanguagewidget_stub.h>
#include <msettingslanguagesettings_stub.h>
#include <msettingslanguagetext_stub.h>
#include <msettingslanguagetextcontroller_stub.h>
#include <mlabel.h>
#include <mtextedit.h>
#include <mdatastore.h>
#include "ut_msettingslanguagetextfactory.h"
#include "mockdatastore.h"

void Ut_MSettingsLanguageTextFactory::initTestCase()
{
    // MApplication and MApplicationWindow must exist
    static int argc = 1;
    static char *app_name[1] = { (char *)"./ut_msettingslanguagetextfactory" };
    app = new MApplication(argc, app_name);
    appWin = new MApplicationWindow;
}

void Ut_MSettingsLanguageTextFactory::cleanupTestCase()
{
    // Destroy the MApplicationWindow
    delete appWin;
    // Destroy the MApplication
    delete app;
}

void Ut_MSettingsLanguageTextFactory::init()
{
}

void Ut_MSettingsLanguageTextFactory::cleanup()
{
}

void Ut_MSettingsLanguageTextFactory::testCreateWidget()
{
    // Create a settings text
    MSettingsLanguageText settingsText("TestKey", "Title");
    MSettingsLanguageWidget dds;
    MockDataStore dataStore;
    MWidget *widget = MSettingsLanguageTextFactory::createWidget(settingsText, dds, &dataStore);
    QVERIFY(widget != NULL);

    // Expecting the widget to have a layout and linear policy
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(widget->layout());
    QVERIFY(layout != NULL);

    // Expecting the layout to contain a MLabel and a MTextEdit
    QCOMPARE(layout->count(), 2);

    // The label's title should be the SettingsText's title
    MLabel *label = dynamic_cast<MLabel *>(layout->itemAt(0));
    QVERIFY(label != NULL);
    QCOMPARE(label->text(), settingsText.title());

    // The label's title should be the specified key's value
    MTextEdit *textEdit = dynamic_cast<MTextEdit *>(layout->itemAt(1));
    QVERIFY(textEdit != NULL);
    QCOMPARE(textEdit->text(), dataStore.value("TestKey").toString());
}

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageTextFactory)
