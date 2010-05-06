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

#include "ut_mappletsettings.h"

#include <mappletsettings.h>
#include <msettingslanguageparser.h>
#include <mgconfdatastore.h>
#include <mdataaccess.h>
#include <mappletid_stub.h>

#include <QtTest/QtTest>

// Some constants for testing purposes
static const QString gAppletBaseName("appletmetadatafile");
static const QString gMetaDataFile(gAppletBaseName + ".desktop");
static const QString gInstanceSettingsFile(QString(APPLET_SETTINGS_DIR "/") + gAppletBaseName + "-instance.xml");
static const QString gGlobalSettingsFile(QString(APPLET_SETTINGS_DIR "/") + gAppletBaseName + "-global.xml");

// Unit test class static variables
const MAppletId Ut_MAppletSettings::appletId("applicationName", "canvas", 0);
MSettingsLanguageBinary *Ut_MAppletSettings::instanceSettingsBinary = NULL;
MSettingsLanguageBinary *Ut_MAppletSettings::globalSettingsBinary = NULL;
bool Ut_MAppletSettings::instanceSettingsFileExists = false;
bool Ut_MAppletSettings::globalSettingsFileExists = false;
QList<QFileInfo> Ut_MAppletSettings::readXmlFiles;
QHash<QString, QString> Ut_MAppletSettings::gconfKeys;
QList<QString> Ut_MAppletSettings::gUnsetGConfKeys;

// MSettingsLanguageParser stubs
MSettingsLanguageParser::~MSettingsLanguageParser()
{
    // Needs to overwrite the destructor so it doesn't try to delete the document
    // member which in this test is used for another purpose.
}

QDomDocument *parserDocument = NULL;
bool MSettingsLanguageParser::readFrom(QIODevice &device)
{
    QFile *file = qobject_cast<QFile *>(&device);
    if (file != NULL) {
        Ut_MAppletSettings::readXmlFiles.append(file->fileName());
        if (file->fileName().contains("-instance.xml")) {
            parserDocument = (QDomDocument *)Ut_MAppletSettings::instanceSettingsBinary;
            return Ut_MAppletSettings::instanceSettingsFileExists;
        } else if (file->fileName().contains("-global.xml")) {
            parserDocument = (QDomDocument *)Ut_MAppletSettings::globalSettingsBinary;
            return Ut_MAppletSettings::globalSettingsFileExists;
        }
    }

    return false;
}

MSettingsLanguageBinary *MSettingsLanguageParser::createSettingsBinary()
{
    return (MSettingsLanguageBinary *)parserDocument;
}

// MGConfDataStore stubs
QStringList MGConfDataStore::allKeys() const
{
    return Ut_MAppletSettings::gconfKeys.values();
}

MGConfDataStore *clearedDataStore;
void MGConfDataStore::clear()
{
    clearedDataStore = this;
}

// The test class
void Ut_MAppletSettings::initializeInstanceSettings()
{
    instanceSettingsFileExists = true;
    instanceSettingsBinary = new MSettingsLanguageBinary();
}

void Ut_MAppletSettings::initializeGlobalSettings()
{
    globalSettingsFileExists = true;
    globalSettingsBinary = new MSettingsLanguageBinary();
}

QString Ut_MAppletSettings::instanceKey(const QString &key)
{
    return QString("/apps/") + appletId.toString() + QDir::separator() + key;
}

QString Ut_MAppletSettings::globalKey(const QString &key)
{
    return QString("/apps/") + gAppletBaseName + QDir::separator() + key;
}

void Ut_MAppletSettings::initTestCase()
{
}

void Ut_MAppletSettings::cleanupTestCase()
{
}

void Ut_MAppletSettings::init()
{
    parserDocument = NULL;
    instanceSettingsBinary = NULL;
    globalSettingsBinary = NULL;
    instanceSettingsFileExists = false;
    globalSettingsFileExists = false;
    readXmlFiles.clear();
    gconfKeys.clear();
    gUnsetGConfKeys.clear();
    clearedDataStore = NULL;
    m_subject = new MAppletSettings(gMetaDataFile, appletId);
}

void Ut_MAppletSettings::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

void Ut_MAppletSettings::testGettingEmptyInstanceSettingsBinary()
{
    const MSettingsLanguageBinary *sb = m_subject->instanceSettingsBinary();
    QVERIFY(sb == NULL);
}

void Ut_MAppletSettings::testGettingEmptyGlobalSettingsBinary()
{
    const MSettingsLanguageBinary *sb = m_subject->globalSettingsBinary();
    QVERIFY(sb == NULL);
}

void Ut_MAppletSettings::testGettingInstanceSettingsBinary()
{
    initializeInstanceSettings();
    const MSettingsLanguageBinary *sb = m_subject->instanceSettingsBinary();
    QCOMPARE(sb, instanceSettingsBinary);
    QCOMPARE(bool(readXmlFiles.contains(gInstanceSettingsFile)), true);
}

void Ut_MAppletSettings::testGettingGlobalSettingsBinary()
{
    initializeGlobalSettings();
    const MSettingsLanguageBinary *sb = m_subject->globalSettingsBinary();
    QCOMPARE(sb, globalSettingsBinary);
    QCOMPARE(bool(readXmlFiles.contains(gGlobalSettingsFile)), true);
}

void Ut_MAppletSettings::testHasNoSettings()
{
    QCOMPARE(m_subject->hasSettings(), false);
}

void Ut_MAppletSettings::testHasInstanceSettings()
{
    initializeInstanceSettings();
    QCOMPARE(m_subject->hasSettings(), true);
}

void Ut_MAppletSettings::testHasGlobalSettings()
{
    initializeGlobalSettings();
    QCOMPARE(m_subject->hasSettings(), true);
}

void Ut_MAppletSettings::testHasBothSettings()
{
    initializeInstanceSettings();
    initializeGlobalSettings();
    QCOMPARE(m_subject->hasSettings(), true);
}

void Ut_MAppletSettings::testEmptyDataStore()
{
    QVERIFY(m_subject->instanceDataStore() != NULL);
    QCOMPARE(m_subject->instanceDataStore()->allKeys().count(), 0);
    QVERIFY(m_subject->globalDataStore() != NULL);
    QCOMPARE(m_subject->globalDataStore()->allKeys().count(), 0);
    QVERIFY(m_subject->dataAccess() != NULL);
    QCOMPARE(m_subject->dataAccess()->allKeys().count(), 0);
}

void Ut_MAppletSettings::testInstanceSettingsDataStore()
{
    initializeInstanceSettings();
    QVERIFY(m_subject->instanceDataStore() != NULL);
    QVERIFY(m_subject->dataAccess() != NULL);
}

void Ut_MAppletSettings::testGlobalSettingsDataStore()
{
    initializeGlobalSettings();
    QVERIFY(m_subject->globalDataStore() != NULL);
    QVERIFY(m_subject->dataAccess() != NULL);
}

void Ut_MAppletSettings::testBothSettingsDataStores()
{
    initializeInstanceSettings();
    initializeGlobalSettings();

    QVERIFY(m_subject->instanceDataStore() != NULL);
    QVERIFY(m_subject->globalDataStore() != NULL);
    QVERIFY(m_subject->dataAccess() != NULL);
}

void Ut_MAppletSettings::testRemoveInstanceSettingValues()
{
    // Add both instance and global settings
    initializeInstanceSettings();
    initializeGlobalSettings();

    m_subject->removeInstanceSettingValues();
    QCOMPARE(clearedDataStore, m_subject->instanceDataStore());
}


QTEST_APPLESS_MAIN(Ut_MAppletSettings)
