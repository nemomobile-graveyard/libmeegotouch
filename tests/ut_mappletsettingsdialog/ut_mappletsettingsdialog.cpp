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

#include "ut_mappletsettingsdialog.h"
#include <mappletsettingsdialog.h>
#include <mapplication.h>
#include <mapplicationwindow.h>
#include <mappletid_stub.h>
#include <msettingslanguagewidget.h>
#include <msettingslanguagewidgetfactory.h>
#include <mdialog.h>
#include <mcontainer.h>
#include <mlocale.h>
#include <mappletsettings_stub.h>

MApplication *app;
const MSettingsLanguageBinary Ut_MAppletSettingsDialog::instanceSettingsBinary;
const MSettingsLanguageBinary Ut_MAppletSettingsDialog::globalSettingsBinary;
bool Ut_MAppletSettingsDialog::dialogCreated = false;
int Ut_MAppletSettingsDialog::instanceSettingsWidgetsCount = 0;
int Ut_MAppletSettingsDialog::globalSettingsWidgetsCount = 0;
QString Ut_MAppletSettingsDialog::containerTitle;

// MSettingsLanguageWidgetFactory stubs
MSettingsLanguageWidget *MSettingsLanguageWidgetFactory::createWidget(const MSettingsLanguageBinary &binary, MDataStore *)
{
    if (&binary == &Ut_MAppletSettingsDialog::instanceSettingsBinary) {
        ++Ut_MAppletSettingsDialog::instanceSettingsWidgetsCount;
    } else if (&binary == &Ut_MAppletSettingsDialog::globalSettingsBinary) {
        ++Ut_MAppletSettingsDialog::globalSettingsWidgetsCount;
    }

    return NULL;
}

// MDialog stubs
int MDialog::exec(MWindow *window)
{
    Q_UNUSED(window);
    Ut_MAppletSettingsDialog::dialogCreated = true;
    return 0;
}

void MContainer::setTitle(const QString &title)
{
    Ut_MAppletSettingsDialog::containerTitle = title;
}

void Ut_MAppletSettingsDialog::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mappletsettingsdialog" };
    app = new MApplication(argc, app_name);

    appletSettings = new MAppletSettings("metadata.desktop", MAppletId());
}

void Ut_MAppletSettingsDialog::cleanupTestCase()
{
    delete appletSettings;
    appletSettings = NULL;

    delete app;
    app = NULL;
}

void Ut_MAppletSettingsDialog::init()
{
    instanceSettingsWidgetsCount = 0;
    globalSettingsWidgetsCount = 0;
    dialogCreated = false;
    containerTitle.clear();
    gMAppletSettingsStub->stubReset();
}

void Ut_MAppletSettingsDialog::cleanup()
{
}

void Ut_MAppletSettingsDialog::testSettingsDialogCreation()
{
    // Check that dialog is not created when there are no applet settings present
    gMAppletSettingsStub->stubSetReturnValue("hasSettings", false);
    MAppletSettingsDialog::exec(MAppletSettings("foo", "foo"));
    QCOMPARE(dialogCreated, false);

    // Check that dialog is created when there are instance applet settings present
    gMAppletSettingsStub->stubSetReturnValue("hasSettings", true);
    MAppletSettingsDialog::exec(MAppletSettings("foo", "foo"));
    QCOMPARE(dialogCreated, true);
}

void Ut_MAppletSettingsDialog::testSettingsContainerCreation()
{
    // Check that when instance binaries are present, widgets are created in a container
    gMAppletSettingsStub->stubSetReturnValue("hasSettings", true);
    gMAppletSettingsStub->stubSetReturnValue("instanceSettingsBinary", &instanceSettingsBinary);
    MAppletSettingsDialog::exec(MAppletSettings("foo", "foo"));
    QCOMPARE(instanceSettingsWidgetsCount, 1);
    QCOMPARE(globalSettingsWidgetsCount, 0);
    // the message id used here comes from
    //     src/mashup/mashup/mappletsettingsdialog.cpp
    // it must be the same as the message id used there.
    //% "Applet Instance Settings"
    QCOMPARE(containerTitle, qtTrId("mappletsettingsdialog_applet_instance_settings_title"));

    // Check that when global binaries are present, widgets are created in a container
    instanceSettingsWidgetsCount = 0;
    gMAppletSettingsStub->stubReset();
    gMAppletSettingsStub->stubSetReturnValue("hasSettings", true);
    gMAppletSettingsStub->stubSetReturnValue("globalSettingsBinary", &globalSettingsBinary);
    MAppletSettingsDialog::exec(MAppletSettings("foo", "foo"));
    QCOMPARE(instanceSettingsWidgetsCount, 0);
    QCOMPARE(globalSettingsWidgetsCount, 1);
    // the message id used here comes from
    //     src/mashup/mashup/mappletsettingsdialog.cpp
    // it must be the same as the message id used there.
    //% "Applet Global Settings"
    QCOMPARE(containerTitle, qtTrId("mappletsettingsdialog_applet_global_settings_title"));
}

QTEST_APPLESS_MAIN(Ut_MAppletSettingsDialog)
