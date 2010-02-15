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

#include "duiappletsettings.h"
#include "duisettingslanguageparser.h"
#include "duiaggregatedataaccess.h"
#include <duigconfdatastore.h>
#include <duiappletid.h>

#include <QFileInfo>
#include <QDir>
#include <DuiLocale>
#include <DuiAppletMetaData>


/*
Implementation note:

This class's implementation uses lazy evaluation. The returned
values/objects from the query methods of the class are only constructed when they
are needed for the first time. That's why most of the private member variables of
this class are marked as mutable - they are changed inside const methods. There are
flags (...Uptodate) that track the validity of the objects. Those flags should be
used to mark when changes to some objects are needed.
*/

DuiAppletSettings::DuiAppletSettings(const QString &metaDataFileName, const DuiAppletId &appletId) :
    instanceSettingsBinaryObject(NULL),
    instanceSettingsBinaryUptodate(false),
    globalSettingsBinaryObject(NULL),
    globalSettingsBinaryUptodate(false),
    settingsAggregate(NULL),
    instanceGConfDataStore(NULL),
    globalGConfDataStore(NULL)
{
    init(metaDataFileName, appletId.toString());
}

DuiAppletSettings::DuiAppletSettings(const QString &metaDataFileName, const QString &appletId) :
    instanceSettingsBinaryObject(NULL),
    instanceSettingsBinaryUptodate(false),
    globalSettingsBinaryObject(NULL),
    globalSettingsBinaryUptodate(false),
    settingsAggregate(NULL),
    instanceGConfDataStore(NULL),
    globalGConfDataStore(NULL)
{
    init(metaDataFileName, appletId);
}

void DuiAppletSettings::init(const QString &metaDataFileName, const QString &appletId)
{
    QString appletBaseName = QFileInfo(metaDataFileName).baseName();
    QFileInfo fileInfo(QDir(QString(APPLET_SETTINGS_DIR)), appletBaseName);
    QString settingsFileStub = fileInfo.absoluteFilePath();

    instanceSettingsFileName = settingsFileStub + "-instance.xml";
    globalSettingsFileName = settingsFileStub + "-global.xml";

    instanceGConfPrefix = QString("/apps/") + appletId;
    globalGConfPrefix = QString("/apps/") + appletBaseName;

    // Load the applet's translation catalog for translating settings titles
    DuiAppletMetaData metaData(metaDataFileName);
    DuiLocale locale;
    locale.installTrCatalog(metaData.resourceIdentifier());
    DuiLocale::setDefault(locale);
}

DuiAppletSettings::~DuiAppletSettings()
{
    delete globalSettingsBinaryObject;
    delete instanceSettingsBinaryObject;
    deleteDataStores();
}

const DuiSettingsLanguageBinary *DuiAppletSettings::instanceSettingsBinary() const
{
    ensureSettingsBinaryObjectUptodate(instanceSettingsBinaryUptodate, &instanceSettingsBinaryObject, instanceSettingsFileName);
    return instanceSettingsBinaryObject;
}

const DuiSettingsLanguageBinary *DuiAppletSettings::globalSettingsBinary() const
{
    ensureSettingsBinaryObjectUptodate(globalSettingsBinaryUptodate, &globalSettingsBinaryObject, globalSettingsFileName);
    return globalSettingsBinaryObject;
}

bool DuiAppletSettings::hasSettings() const
{
    return instanceSettingsBinary() != NULL || globalSettingsBinary() != NULL;
}

void DuiAppletSettings::deleteDataStores() const
{
    delete settingsAggregate;
    delete instanceGConfDataStore;
    delete globalGConfDataStore;
    settingsAggregate = NULL;
    instanceGConfDataStore = NULL;
    globalGConfDataStore = NULL;
}

void DuiAppletSettings::createDataStoresIfNeeded() const
{
    if (instanceGConfDataStore == NULL || globalGConfDataStore == NULL || settingsAggregate == NULL) {
        // Delete any previous data store
        deleteDataStores();

        instanceGConfDataStore = new DuiGConfDataStore(instanceGConfPrefix);
        globalGConfDataStore   = new DuiGConfDataStore(globalGConfPrefix);
        settingsAggregate = new DuiAggregateDataAccess(*instanceGConfDataStore, *globalGConfDataStore);
    }
}

DuiDataStore *DuiAppletSettings::instanceDataStore() const
{
    createDataStoresIfNeeded();

    return instanceGConfDataStore;
}

DuiDataStore *DuiAppletSettings::globalDataStore() const
{
    createDataStoresIfNeeded();

    return globalGConfDataStore;
}

DuiDataAccess *DuiAppletSettings::dataAccess() const
{
    createDataStoresIfNeeded();

    return settingsAggregate;
}

void DuiAppletSettings::removeInstanceSettingValues() const
{
    createDataStoresIfNeeded();

    if (instanceGConfDataStore) {
        instanceGConfDataStore->clear();
    }
}

void DuiAppletSettings::ensureSettingsBinaryObjectUptodate(bool &settingsBinaryUptodate,
        DuiSettingsLanguageBinary **settingsBinaryObjectPointer,
        const QString &settingsFileName)
{
    if (!settingsBinaryUptodate) {
        // Delete any previous object
        delete *settingsBinaryObjectPointer;
        *settingsBinaryObjectPointer = NULL;

        // Convert the settings XML to a settings binary
        QFile xmlFile(settingsFileName);
        DuiSettingsLanguageParser parser;
        if (parser.readFrom(xmlFile)) {
            *settingsBinaryObjectPointer = parser.createSettingsBinary();
        }
        settingsBinaryUptodate = true;
    }
}
