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
#include "duiappletsettings_p.h"
#include "duisettingslanguageparser.h"
#include "duiaggregatedataaccess.h"
#include <duigconfdatastore.h>
#include <duiappletid.h>

#include <QFileInfo>
#include <QDir>
#include <DuiLocale>
#include <DuiAppletMetaData>

static void ensureSettingsBinaryObjectUptodate(bool &settingsBinaryUptodate,
                                               DuiSettingsLanguageBinary **settingsBinaryObjectPointer,
                                               const QString &settingsFileName);

DuiAppletSettingsPrivate::DuiAppletSettingsPrivate() :
    instanceSettingsBinaryObject(NULL),
    instanceSettingsBinaryUptodate(false),
    globalSettingsBinaryObject(NULL),
    globalSettingsBinaryUptodate(false),
    settingsAggregate(NULL),
    instanceGConfDataStore(NULL),
    globalGConfDataStore(NULL)
{
}

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
    d_ptr(new DuiAppletSettingsPrivate())
{
    init(metaDataFileName, appletId.toString());
}

DuiAppletSettings::DuiAppletSettings(const QString &metaDataFileName, const QString &appletId) :
    d_ptr(new DuiAppletSettingsPrivate())
{
    init(metaDataFileName, appletId);
}

void DuiAppletSettings::init(const QString &metaDataFileName, const QString &appletId)
{
    Q_D(DuiAppletSettings);
    QString appletBaseName = QFileInfo(metaDataFileName).baseName();
    QFileInfo fileInfo(QDir(QString(APPLET_SETTINGS_DIR)), appletBaseName);
    QString settingsFileStub = fileInfo.absoluteFilePath();

    d->instanceSettingsFileName = settingsFileStub + "-instance.xml";
    d->globalSettingsFileName = settingsFileStub + "-global.xml";

    d->instanceGConfPrefix = QString("/apps/") + appletId;
    d->globalGConfPrefix = QString("/apps/") + appletBaseName;

    // Load the applet's translation catalog for translating settings titles
    DuiAppletMetaData metaData(metaDataFileName);
    DuiLocale locale;
    locale.installTrCatalog(metaData.resourceIdentifier());
    DuiLocale::setDefault(locale);
}

DuiAppletSettings::~DuiAppletSettings()
{
    Q_D(DuiAppletSettings);
    delete d->globalSettingsBinaryObject;
    delete d->instanceSettingsBinaryObject;
    deleteDataStores();
    delete d_ptr;
}

const DuiSettingsLanguageBinary *DuiAppletSettings::instanceSettingsBinary() const
{
    Q_D(const DuiAppletSettings);
    ensureSettingsBinaryObjectUptodate(d->instanceSettingsBinaryUptodate, &(d->instanceSettingsBinaryObject), d->instanceSettingsFileName);
    return d->instanceSettingsBinaryObject;
}

const DuiSettingsLanguageBinary *DuiAppletSettings::globalSettingsBinary() const
{
    Q_D(const DuiAppletSettings);
    ensureSettingsBinaryObjectUptodate(d->globalSettingsBinaryUptodate, &(d->globalSettingsBinaryObject), d->globalSettingsFileName);
    return d->globalSettingsBinaryObject;
}

bool DuiAppletSettings::hasSettings() const
{
    return instanceSettingsBinary() != NULL || globalSettingsBinary() != NULL;
}

void DuiAppletSettings::deleteDataStores() const
{
    Q_D(const DuiAppletSettings);
    delete d->settingsAggregate;
    delete d->instanceGConfDataStore;
    delete d->globalGConfDataStore;
    d->settingsAggregate = NULL;
    d->instanceGConfDataStore = NULL;
    d->globalGConfDataStore = NULL;
}

void DuiAppletSettings::createDataStoresIfNeeded() const
{
    Q_D(const DuiAppletSettings);
    if (d->instanceGConfDataStore == NULL || d->globalGConfDataStore == NULL || d->settingsAggregate == NULL) {
        // Delete any previous data store
        deleteDataStores();

        d->instanceGConfDataStore = new DuiGConfDataStore(d->instanceGConfPrefix);
        d->globalGConfDataStore   = new DuiGConfDataStore(d->globalGConfPrefix);
        d->settingsAggregate = new DuiAggregateDataAccess(*d->instanceGConfDataStore, *d->globalGConfDataStore);
    }
}

DuiDataStore *DuiAppletSettings::instanceDataStore() const
{
    Q_D(const DuiAppletSettings);
    createDataStoresIfNeeded();

    return d->instanceGConfDataStore;
}

DuiDataStore *DuiAppletSettings::globalDataStore() const
{
    Q_D(const DuiAppletSettings);
    createDataStoresIfNeeded();

    return d->globalGConfDataStore;
}

DuiDataAccess *DuiAppletSettings::dataAccess() const
{
    Q_D(const DuiAppletSettings);
    createDataStoresIfNeeded();

    return d->settingsAggregate;
}

void DuiAppletSettings::removeInstanceSettingValues() const
{
    Q_D(const DuiAppletSettings);
    createDataStoresIfNeeded();

    if (d->instanceGConfDataStore) {
        d->instanceGConfDataStore->clear();
    }
}

/*!
 * A helper method to ensure that a settings binary object is uptodate.
 * The "uptodateness" is determined by the \a settingsBinaryUptodate parameter.
 * If the binary needs updating, it is updated by reading the settings file determined
 * by the \a settingsFileName parameter.
 * \param settingsBinaryUptodate determines if the settings binary is uptodate or not.
 *        This parameter will be \c true after this method returns.
 * \param settingsBinaryObjectPointer a pointer to the pointer variable of the binary to be manipulated.
 * \param settingsFileName the file name of the settings file.
 */
static void ensureSettingsBinaryObjectUptodate(bool &settingsBinaryUptodate,
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
