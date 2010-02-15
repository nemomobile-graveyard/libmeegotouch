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

#include "ut_duiappletsettings.h"

#include <duiappletsettings.h>
#include <duisettingslanguageparser.h>
#include <duigconfdatastore.h>
#include <duidataaccess.h>
#include <duiappletid_stub.h>

#include <QtTest/QtTest>

// Some constants for testing purposes
static const QString gAppletBaseName("appletmetadatafile");
static const QString gMetaDataFile(gAppletBaseName + ".desktop");
static const QString gInstanceSettingsFile(QString(APPLET_SETTINGS_DIR "/") + gAppletBaseName + "-instance.xml");
static const QString gGlobalSettingsFile(QString(APPLET_SETTINGS_DIR "/") + gAppletBaseName + "-global.xml");

// Unit test class static variables
const DuiAppletId Ut_DuiAppletSettings::appletId("applicationName", "canvas", 0);
DuiSettingsLanguageBinary *Ut_DuiAppletSettings::instanceSettingsBinary = NULL;
DuiSettingsLanguageBinary *Ut_DuiAppletSettings::globalSettingsBinary = NULL;
bool Ut_DuiAppletSettings::instanceSettingsFileExists = false;
bool Ut_DuiAppletSettings::globalSettingsFileExists = false;
QList<QFileInfo> Ut_DuiAppletSettings::readXmlFiles;
QHash<QString, QString> Ut_DuiAppletSettings::gconfKeys;
QList<QString> Ut_DuiAppletSettings::gUnsetGConfKeys;

// DuiSettingsLanguageParser stubs
DuiSettingsLanguageParser::~DuiSettingsLanguageParser()
{
    // Needs to overwrite the destructor so it doesn't try to delete the document
    // member which in this test is used for another purpose.
}

QDomDocument *parserDocument = NULL;
bool DuiSettingsLanguageParser::readFrom(QIODevice &device)
{
    QFile *file = qobject_cast<QFile *>(&device);
    if (file != NULL) {
        Ut_DuiAppletSettings::readXmlFiles.append(file->fileName());
        if (file->fileName().contains("-instance.xml")) {
            parserDocument = (QDomDocument *)Ut_DuiAppletSettings::instanceSettingsBinary;
            return Ut_DuiAppletSettings::instanceSettingsFileExists;
        } else if (file->fileName().contains("-global.xml")) {
            parserDocument = (QDomDocument *)Ut_DuiAppletSettings::globalSettingsBinary;
            return Ut_DuiAppletSettings::globalSettingsFileExists;
        }
    }

    return false;
}

DuiSettingsLanguageBinary *DuiSettingsLanguageParser::createSettingsBinary()
{
    return (DuiSettingsLanguageBinary *)parserDocument;
}

// DuiGConfDataStore stubs
QStringList DuiGConfDataStore::allKeys() const
{
    return Ut_DuiAppletSettings::gconfKeys.values();
}

DuiGConfDataStore *clearedDataStore;
void DuiGConfDataStore::clear()
{
    clearedDataStore = this;
}

// The test class
void Ut_DuiAppletSettings::initializeInstanceSettings()
{
    instanceSettingsFileExists = true;
    instanceSettingsBinary = new DuiSettingsLanguageBinary();
}

void Ut_DuiAppletSettings::initializeGlobalSettings()
{
    globalSettingsFileExists = true;
    globalSettingsBinary = new DuiSettingsLanguageBinary();
}

QString Ut_DuiAppletSettings::instanceKey(const QString &key)
{
    return QString("/apps/") + appletId.toString() + QDir::separator() + key;
}

QString Ut_DuiAppletSettings::globalKey(const QString &key)
{
    return QString("/apps/") + gAppletBaseName + QDir::separator() + key;
}

void Ut_DuiAppletSettings::initTestCase()
{
}

void Ut_DuiAppletSettings::cleanupTestCase()
{
}

void Ut_DuiAppletSettings::init()
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
    m_subject = new DuiAppletSettings(gMetaDataFile, appletId);
}

void Ut_DuiAppletSettings::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

void Ut_DuiAppletSettings::testGettingEmptyInstanceSettingsBinary()
{
    const DuiSettingsLanguageBinary *sb = m_subject->instanceSettingsBinary();
    QVERIFY(sb == NULL);
}

void Ut_DuiAppletSettings::testGettingEmptyGlobalSettingsBinary()
{
    const DuiSettingsLanguageBinary *sb = m_subject->globalSettingsBinary();
    QVERIFY(sb == NULL);
}

void Ut_DuiAppletSettings::testGettingInstanceSettingsBinary()
{
    initializeInstanceSettings();
    const DuiSettingsLanguageBinary *sb = m_subject->instanceSettingsBinary();
    QCOMPARE(sb, instanceSettingsBinary);
    QCOMPARE(bool(readXmlFiles.contains(gInstanceSettingsFile)), true);
}

void Ut_DuiAppletSettings::testGettingGlobalSettingsBinary()
{
    initializeGlobalSettings();
    const DuiSettingsLanguageBinary *sb = m_subject->globalSettingsBinary();
    QCOMPARE(sb, globalSettingsBinary);
    QCOMPARE(bool(readXmlFiles.contains(gGlobalSettingsFile)), true);
}

void Ut_DuiAppletSettings::testHasNoSettings()
{
    QCOMPARE(m_subject->hasSettings(), false);
}

void Ut_DuiAppletSettings::testHasInstanceSettings()
{
    initializeInstanceSettings();
    QCOMPARE(m_subject->hasSettings(), true);
}

void Ut_DuiAppletSettings::testHasGlobalSettings()
{
    initializeGlobalSettings();
    QCOMPARE(m_subject->hasSettings(), true);
}

void Ut_DuiAppletSettings::testHasBothSettings()
{
    initializeInstanceSettings();
    initializeGlobalSettings();
    QCOMPARE(m_subject->hasSettings(), true);
}

void Ut_DuiAppletSettings::testEmptyDataStore()
{
    QVERIFY(m_subject->instanceDataStore() != NULL);
    QCOMPARE(m_subject->instanceDataStore()->allKeys().count(), 0);
    QVERIFY(m_subject->globalDataStore() != NULL);
    QCOMPARE(m_subject->globalDataStore()->allKeys().count(), 0);
    QVERIFY(m_subject->dataAccess() != NULL);
    QCOMPARE(m_subject->dataAccess()->allKeys().count(), 0);
}

void Ut_DuiAppletSettings::testInstanceSettingsDataStore()
{
    initializeInstanceSettings();
    QVERIFY(m_subject->instanceDataStore() != NULL);
    QVERIFY(m_subject->dataAccess() != NULL);
}

void Ut_DuiAppletSettings::testGlobalSettingsDataStore()
{
    initializeGlobalSettings();
    QVERIFY(m_subject->globalDataStore() != NULL);
    QVERIFY(m_subject->dataAccess() != NULL);
}

void Ut_DuiAppletSettings::testBothSettingsDataStores()
{
    initializeInstanceSettings();
    initializeGlobalSettings();

    QVERIFY(m_subject->instanceDataStore() != NULL);
    QVERIFY(m_subject->globalDataStore() != NULL);
    QVERIFY(m_subject->dataAccess() != NULL);
}

void Ut_DuiAppletSettings::testRemoveInstanceSettingValues()
{
    // Add both instance and global settings
    initializeInstanceSettings();
    initializeGlobalSettings();

    m_subject->removeInstanceSettingValues();
    QCOMPARE(clearedDataStore, m_subject->instanceDataStore());
}


QTEST_APPLESS_MAIN(Ut_DuiAppletSettings)
