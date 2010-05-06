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

#include "mappletsettings.h"
#include "mappletsettings_p.h"
#include "msettingslanguageparser.h"
#include "maggregatedataaccess.h"
#include <mgconfdatastore.h>
#include <mappletid.h>

#include <QFileInfo>
#include <QDir>
#include <MLocale>
#include <MAppletMetaData>

static void ensureSettingsBinaryObjectUptodate(bool &settingsBinaryUptodate,
                                               MSettingsLanguageBinary **settingsBinaryObjectPointer,
                                               const QString &settingsFileName);

MAppletSettingsPrivate::MAppletSettingsPrivate() :
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

MAppletSettings::MAppletSettings(const QString &metaDataFileName, const MAppletId &appletId) :
    d_ptr(new MAppletSettingsPrivate())
{
    init(metaDataFileName, appletId.toString());
}

MAppletSettings::MAppletSettings(const QString &metaDataFileName, const QString &appletId) :
    d_ptr(new MAppletSettingsPrivate())
{
    init(metaDataFileName, appletId);
}

void MAppletSettings::init(const QString &metaDataFileName, const QString &appletId)
{
    Q_D(MAppletSettings);
    QString appletBaseName = QFileInfo(metaDataFileName).baseName();
    QFileInfo fileInfo(QDir(QString(APPLET_SETTINGS_DIR)), appletBaseName);
    QString settingsFileStub = fileInfo.absoluteFilePath();

    d->instanceSettingsFileName = settingsFileStub + "-instance.xml";
    d->globalSettingsFileName = settingsFileStub + "-global.xml";

    d->instanceGConfPrefix = QString("/apps/") + appletId;
    d->globalGConfPrefix = QString("/apps/") + appletBaseName;

    // Load the applet's translation catalog for translating settings titles
    MAppletMetaData metaData(metaDataFileName);
    MLocale locale;
    locale.installTrCatalog(metaData.resourceIdentifier());
    MLocale::setDefault(locale);
}

MAppletSettings::~MAppletSettings()
{
    Q_D(MAppletSettings);
    delete d->globalSettingsBinaryObject;
    delete d->instanceSettingsBinaryObject;
    deleteDataStores();
    delete d_ptr;
}

const MSettingsLanguageBinary *MAppletSettings::instanceSettingsBinary() const
{
    Q_D(const MAppletSettings);
    ensureSettingsBinaryObjectUptodate(d->instanceSettingsBinaryUptodate, &(d->instanceSettingsBinaryObject), d->instanceSettingsFileName);
    return d->instanceSettingsBinaryObject;
}

const MSettingsLanguageBinary *MAppletSettings::globalSettingsBinary() const
{
    Q_D(const MAppletSettings);
    ensureSettingsBinaryObjectUptodate(d->globalSettingsBinaryUptodate, &(d->globalSettingsBinaryObject), d->globalSettingsFileName);
    return d->globalSettingsBinaryObject;
}

bool MAppletSettings::hasSettings() const
{
    return instanceSettingsBinary() != NULL || globalSettingsBinary() != NULL;
}

void MAppletSettings::deleteDataStores() const
{
    Q_D(const MAppletSettings);
    delete d->settingsAggregate;
    delete d->instanceGConfDataStore;
    delete d->globalGConfDataStore;
    d->settingsAggregate = NULL;
    d->instanceGConfDataStore = NULL;
    d->globalGConfDataStore = NULL;
}

void MAppletSettings::createDataStoresIfNeeded() const
{
    Q_D(const MAppletSettings);
    if (d->instanceGConfDataStore == NULL || d->globalGConfDataStore == NULL || d->settingsAggregate == NULL) {
        // Delete any previous data store
        deleteDataStores();

        d->instanceGConfDataStore = new MGConfDataStore(d->instanceGConfPrefix);
        d->globalGConfDataStore   = new MGConfDataStore(d->globalGConfPrefix);
        d->settingsAggregate = new MAggregateDataAccess(*d->instanceGConfDataStore, *d->globalGConfDataStore);
    }
}

MDataStore *MAppletSettings::instanceDataStore() const
{
    Q_D(const MAppletSettings);
    createDataStoresIfNeeded();

    return d->instanceGConfDataStore;
}

MDataStore *MAppletSettings::globalDataStore() const
{
    Q_D(const MAppletSettings);
    createDataStoresIfNeeded();

    return d->globalGConfDataStore;
}

MDataAccess *MAppletSettings::dataAccess() const
{
    Q_D(const MAppletSettings);
    createDataStoresIfNeeded();

    return d->settingsAggregate;
}

void MAppletSettings::removeInstanceSettingValues() const
{
    Q_D(const MAppletSettings);
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
					       MSettingsLanguageBinary **settingsBinaryObjectPointer,
					       const QString &settingsFileName)
{
    if (!settingsBinaryUptodate) {
        // Delete any previous object
        delete *settingsBinaryObjectPointer;
        *settingsBinaryObjectPointer = NULL;

        // Convert the settings XML to a settings binary
        QFile xmlFile(settingsFileName);
        MSettingsLanguageParser parser;
        if (parser.readFrom(xmlFile)) {
            *settingsBinaryObjectPointer = parser.createSettingsBinary();
        }
        settingsBinaryUptodate = true;
    }
}
