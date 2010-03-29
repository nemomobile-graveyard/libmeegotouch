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

#include "ut_duiappletsettingsdialog.h"
#include <duiappletsettingsdialog.h>
#include <duiapplication.h>
#include <duiapplicationwindow.h>
#include <duiappletid_stub.h>
#include <duisettingslanguagewidget.h>
#include <duisettingslanguagewidgetfactory.h>
#include <duidialog.h>
#include <duicontainer.h>
#include <duilocale.h>
#include <duiappletsettings_stub.h>

DuiApplication *app;
const DuiSettingsLanguageBinary Ut_DuiAppletSettingsDialog::instanceSettingsBinary;
const DuiSettingsLanguageBinary Ut_DuiAppletSettingsDialog::globalSettingsBinary;
bool Ut_DuiAppletSettingsDialog::dialogCreated = false;
int Ut_DuiAppletSettingsDialog::instanceSettingsWidgetsCount = 0;
int Ut_DuiAppletSettingsDialog::globalSettingsWidgetsCount = 0;
QString Ut_DuiAppletSettingsDialog::containerTitle;

// DuiSettingsLanguageWidgetFactory stubs
DuiSettingsLanguageWidget *DuiSettingsLanguageWidgetFactory::createWidget(const DuiSettingsLanguageBinary &binary, DuiDataStore *)
{
    if (&binary == &Ut_DuiAppletSettingsDialog::instanceSettingsBinary) {
        ++Ut_DuiAppletSettingsDialog::instanceSettingsWidgetsCount;
    } else if (&binary == &Ut_DuiAppletSettingsDialog::globalSettingsBinary) {
        ++Ut_DuiAppletSettingsDialog::globalSettingsWidgetsCount;
    }

    return NULL;
}

// DuiDialog stubs
int DuiDialog::exec(DuiWindow *window)
{
    Q_UNUSED(window);
    Ut_DuiAppletSettingsDialog::dialogCreated = true;
    return 0;
}

void DuiContainer::setTitle(const QString &title)
{
    Ut_DuiAppletSettingsDialog::containerTitle = title;
}

void Ut_DuiAppletSettingsDialog::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duiappletsettingsdialog" };
    app = new DuiApplication(argc, app_name);

    appletSettings = new DuiAppletSettings("metadata.desktop", DuiAppletId());
}

void Ut_DuiAppletSettingsDialog::cleanupTestCase()
{
    delete appletSettings;
    appletSettings = NULL;

    delete app;
    app = NULL;
}

void Ut_DuiAppletSettingsDialog::init()
{
    instanceSettingsWidgetsCount = 0;
    globalSettingsWidgetsCount = 0;
    dialogCreated = false;
    containerTitle.clear();
    gDuiAppletSettingsStub->stubReset();
}

void Ut_DuiAppletSettingsDialog::cleanup()
{
}

void Ut_DuiAppletSettingsDialog::testSettingsDialogCreation()
{
    // Check that dialog is not created when there are no applet settings present
    gDuiAppletSettingsStub->stubSetReturnValue("hasSettings", false);
    DuiAppletSettingsDialog::exec(DuiAppletSettings("foo", "foo"));
    QCOMPARE(dialogCreated, false);

    // Check that dialog is created when there are instance applet settings present
    gDuiAppletSettingsStub->stubSetReturnValue("hasSettings", true);
    DuiAppletSettingsDialog::exec(DuiAppletSettings("foo", "foo"));
    QCOMPARE(dialogCreated, true);
}

void Ut_DuiAppletSettingsDialog::testSettingsContainerCreation()
{
    // Check that when instance binaries are present, widgets are created in a container
    gDuiAppletSettingsStub->stubSetReturnValue("hasSettings", true);
    gDuiAppletSettingsStub->stubSetReturnValue("instanceSettingsBinary", &instanceSettingsBinary);
    DuiAppletSettingsDialog::exec(DuiAppletSettings("foo", "foo"));
    QCOMPARE(instanceSettingsWidgetsCount, 1);
    QCOMPARE(globalSettingsWidgetsCount, 0);
    // the message id used here comes from
    //     src/mashup/mashup/duiappletsettingsdialog.cpp
    // it must be the same as the message id used there.
    //% "Applet Instance Settings"
    QCOMPARE(containerTitle, qtTrId("duiappletsettingsdialog_applet_instance_settings_title"));

    // Check that when global binaries are present, widgets are created in a container
    instanceSettingsWidgetsCount = 0;
    gDuiAppletSettingsStub->stubReset();
    gDuiAppletSettingsStub->stubSetReturnValue("hasSettings", true);
    gDuiAppletSettingsStub->stubSetReturnValue("globalSettingsBinary", &globalSettingsBinary);
    DuiAppletSettingsDialog::exec(DuiAppletSettings("foo", "foo"));
    QCOMPARE(instanceSettingsWidgetsCount, 0);
    QCOMPARE(globalSettingsWidgetsCount, 1);
    // the message id used here comes from
    //     src/mashup/mashup/duiappletsettingsdialog.cpp
    // it must be the same as the message id used there.
    //% "Applet Global Settings"
    QCOMPARE(containerTitle, qtTrId("duiappletsettingsdialog_applet_global_settings_title"));
}

QTEST_APPLESS_MAIN(Ut_DuiAppletSettingsDialog)
