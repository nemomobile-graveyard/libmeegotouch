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

#include "ut_mfiledatastore.h"
#include "mfiledatastore.h"

// Map for storing the original settings values
QMap<QString, QVariant> * originalSettingsMap;
// Map for storing the changed settings values
QMap<QString, QVariant> * changedSettingsMap;
// Map to use for the current settings instance values
QMap<QString, QVariant> * settingsMapForNextQSettingsInstance;

// Indicator whether QSettings object has been synchronized.
bool gIsSynchronized;
// File in which the data will be stored.
QList<QString> gStoreFile;
// Status of the QSettings
QSettings::Status gQSettingsStatus;
// Whether QSettings is writable or not
bool gQSettingsIsWritable;
// Whether QSettings should synchronize itself when sync()ed
bool gSynchronize;
// Whether a sync fails
bool gQSettingsSyncFails;
//QSettings original file index
static const int ORIGINAL_SETTINGS_FILE_INDEX = 0;

// QMap for storing the settings object and the map for using settings values
QMap<const QSettings *, QMap<QString, QVariant> *> mapForQSettingsInstance;

//File path to data file
static const QString DATA_FILE_PATH("/tmp/store.data");

// Stubs of QSettings methods
QSettings::QSettings(const QString &fileName, Format format, QObject *parent)
{
    Q_UNUSED(format);
    Q_UNUSED(parent);
    gStoreFile.append(fileName);
    mapForQSettingsInstance.insert(this, settingsMapForNextQSettingsInstance);
}
QSettings::~QSettings()
{
}
QStringList QSettings::allKeys() const
{
    return mapForQSettingsInstance.value(this)->keys();
}
void QSettings::setValue(const QString &key, const QVariant &value)
{
    mapForQSettingsInstance.value(this)->insert(key, value);
}
QVariant QSettings::value(const QString &key, const QVariant &defaultValue) const
{
    if (mapForQSettingsInstance.value(this)->contains(key)) {
        return mapForQSettingsInstance.value(this)->value(key);
    } else {
        return defaultValue;
    }
}

void QSettings::sync()
{
    if (gQSettingsSyncFails) {
        return;
    }
    gIsSynchronized = true;
    if (gSynchronize) {
        mapForQSettingsInstance.value(this)->clear();
        foreach(const QString & key, settingsMapForNextQSettingsInstance->keys()) {
            mapForQSettingsInstance.value(this)->insert(key, settingsMapForNextQSettingsInstance->value(key));
        }
    }
}

void QSettings::remove(const QString &key)
{
    mapForQSettingsInstance.value(this)->remove(key);
}
void QSettings::clear()
{
    if (gQSettingsSyncFails) {
        return;
    }
    mapForQSettingsInstance.value(this)->clear();
}
bool QSettings::contains(const QString &key) const
{
    return mapForQSettingsInstance.value(this)->contains(key);
}
QSettings::Status QSettings::status() const
{
    if (gQSettingsSyncFails) {
        return QSettings::AccessError;
    }
    return gQSettingsStatus;
}
bool QSettings::isWritable() const
{
    return gQSettingsIsWritable;
}
QString QSettings::fileName() const
{
    return gStoreFile.at(ORIGINAL_SETTINGS_FILE_INDEX);
}

static bool fileExists = true;
bool QFileInfo::exists() const
{
    return fileExists;
}

//! QFileSystemWatcher stubs
static QList<QString> watcherPaths;
void QFileSystemWatcher::addPath(const QString & path)
{
    watcherPaths.append(path);
}

//! Signal Receptor class
void SignalReceptor::valueChanged(const QString &key, QVariant value)
{
    keys.append(key);
    values.append(value);
}

void Ut_MFileDataStore::init()
{
    fileExists = true;
    watcherPaths.clear();
    gQSettingsSyncFails = false;
    gIsSynchronized = false;
    gSynchronize = false;
    gStoreFile.clear();
    gQSettingsStatus = QSettings::NoError;
    gQSettingsIsWritable = true;

    originalSettingsMap = new QMap<QString, QVariant>;
    settingsMapForNextQSettingsInstance = originalSettingsMap;
    m_subject = new MFileDataStore(DATA_FILE_PATH);
}

void Ut_MFileDataStore::cleanup()
{
    delete m_subject;
    m_subject = NULL;

    delete originalSettingsMap;
    originalSettingsMap = NULL;
}

void Ut_MFileDataStore::testDataFileDoesNotExists()
{
    fileExists = false;
    delete m_subject;
    watcherPaths.clear();
    m_subject = new MFileDataStore(DATA_FILE_PATH);
    QCOMPARE((bool)watcherPaths.contains(DATA_FILE_PATH), false);
}

void Ut_MFileDataStore::testDataFileExists()
{
    QCOMPARE((bool)watcherPaths.contains(DATA_FILE_PATH), true);
}

void Ut_MFileDataStore::testFileOpening()
{
    QCOMPARE(m_subject->isReadable(), true);
    QCOMPARE(m_subject->isWritable(), true);
    QCOMPARE(gStoreFile.at(ORIGINAL_SETTINGS_FILE_INDEX), DATA_FILE_PATH);
}

void Ut_MFileDataStore::testValueSettingDataFileAddedToWatch()
{
    fileExists = false;
    delete m_subject;
    watcherPaths.clear();
    m_subject = new MFileDataStore(DATA_FILE_PATH);
    fileExists = true;
    m_subject->createValue("key", "value");
    QCOMPARE((bool)watcherPaths.contains(DATA_FILE_PATH), true);
}

void Ut_MFileDataStore::testValueSetting()
{
    SignalReceptor *receptor = new SignalReceptor;
    connect(m_subject, SIGNAL(valueChanged(QString, QVariant)), receptor, SLOT(valueChanged(QString, QVariant)));
    m_subject->setValue(QString("key"), QVariant(QString("value")));
    QVERIFY(!settingsMapForNextQSettingsInstance->contains("key"));
    QCOMPARE(receptor->keys.count(), 0);
    QCOMPARE(receptor->values.count(), 0);

    m_subject->createValue("key", "value");
    QCOMPARE(settingsMapForNextQSettingsInstance->value("key").toString(), QString("value"));
    QVERIFY(gIsSynchronized);
    // Verify that only one signal is sent with the given key/value
    QCOMPARE(receptor->keys.count(), 1);
    QCOMPARE(receptor->keys.at(0), QString("key"));
    QCOMPARE(receptor->values.count(), 1);
    QCOMPARE(receptor->values.at(0).toString(), QString("value"));

    m_subject->setValue(QString("key"), QVariant(QString("changed_value")));
    QCOMPARE(settingsMapForNextQSettingsInstance->value("key").toString(), QString("changed_value"));
    QVERIFY(gIsSynchronized);
    // Verify that another signal is sent with the new key/value
    QCOMPARE(receptor->keys.count(), 2);
    QCOMPARE(receptor->keys.at(1), QString("key"));
    QCOMPARE(receptor->values.count(), 2);
    QCOMPARE(receptor->values.at(1).toString(), QString("changed_value"));
    delete receptor;
}

void Ut_MFileDataStore::testValueReading()
{
    m_subject->createValue(QString("key"), QVariant(QString("value")));
    QCOMPARE(m_subject->value(QString("key")).toString(), QString("value"));
}

void Ut_MFileDataStore::testGettingAllKeys()
{
    QCOMPARE(m_subject->allKeys().size(), 0);

    m_subject->createValue(QString("key1"), QVariant(QString("foo")));
    m_subject->createValue(QString("key2"), QVariant(QString("bar")));
    QCOMPARE(m_subject->allKeys().size(), 2);
    QCOMPARE(m_subject->allKeys().at(0), QString("key1"));
    QCOMPARE(m_subject->allKeys().at(1), QString("key2"));
}

void Ut_MFileDataStore::testDataRemoval()
{
    m_subject->createValue(QString("key"), QVariant(QString("value")));

    // Clear synchronized flag to ensure that datastore is synchronized when removing data.
    gIsSynchronized = false;

    m_subject->remove("key");
    QCOMPARE(m_subject->allKeys().size(), 0);
    QVERIFY(gIsSynchronized);
}

void Ut_MFileDataStore::testClear()
{
    m_subject->createValue(QString("key"), QVariant(QString("value")));
    m_subject->createValue(QString("key1"), QVariant(QString("value")));

    // Clear synchronized flag to ensure that datastore is synchronized when removing data.
    gIsSynchronized = false;

    // Clear file data store
    m_subject->clear();

    // Verify that all keys were removed from file data store and that the file backend was synchronized.
    QCOMPARE(m_subject->allKeys().size(), 0);
    QVERIFY(gIsSynchronized);
}

void Ut_MFileDataStore::testContains()
{
    m_subject->createValue(QString("key"), QVariant(QString("value")));
    QCOMPARE(m_subject->contains("key"), true);

    m_subject->setValue(QString("he-man"), QVariant(QString("value")));
    QCOMPARE(m_subject->contains("he-man"), false);
}

void Ut_MFileDataStore::testFileNotReadable()
{
    gQSettingsStatus = QSettings::AccessError;

    QCOMPARE(m_subject->isReadable(), false);
    m_subject->createValue("foo", "bar");
    QCOMPARE(m_subject->value("foo"), QVariant());
    QCOMPARE(m_subject->allKeys(), QStringList());
    QCOMPARE(m_subject->contains("foo"), false);
}

void Ut_MFileDataStore::testFileNotWritable()
{
    m_subject->createValue("foo", "bar");

    gQSettingsIsWritable = false;

    // Shouldn't be possible to clear
    QStringList keys1 = m_subject->allKeys();
    m_subject->clear();
    QStringList keys2 = m_subject->allKeys();
    QCOMPARE(keys1, keys2);

    // Shouldn't be possible to create a new value
    m_subject->createValue("bar", "bapapa");
    QCOMPARE(m_subject->contains("bar"), false);

    // Shouldn't be possible to change the value
    m_subject->createValue("foo", "baz");
    QCOMPARE(m_subject->value("foo").toString(), QString("bar"));

    // Shouldn't be possible to remove a value
    m_subject->remove("foo");
    QCOMPARE(m_subject->contains("foo"), true);
}


void Ut_MFileDataStore::testFileContentsInvalid()
{
    gQSettingsStatus = QSettings::FormatError;

    QCOMPARE(m_subject->isReadable(), false);
    QCOMPARE(m_subject->isWritable(), false);
}

void Ut_MFileDataStore::testSettingsFileModifiedExternally()
{

    connect(this, SIGNAL(fileChanged(QString)), m_subject, SLOT(fileChanged(QString)));
    QCOMPARE(m_subject->allKeys().count(), 0);
    m_subject->createValue(QString("key"), QVariant(QString("value")));
    QCOMPARE(settingsMapForNextQSettingsInstance->value("key").toString(), QString("value"));
    changedSettingsMap = new QMap<QString, QVariant>;
    changedSettingsMap->insert("key1", QVariant(QString("value1")));
    changedSettingsMap->insert("key2", QVariant(QString("value2")));

    qRegisterMetaType<QVariant>("QVariant");
    QSignalSpy valueSpy(m_subject, SIGNAL(valueChanged(QString, QVariant)));
    // Check nothing happens when fileChanged signal is emitted with wrong file name
    emit fileChanged(QString("nonExistingFile"));
    QCOMPARE(valueSpy.count(), 0);

    settingsMapForNextQSettingsInstance = changedSettingsMap;
    gSynchronize = true;

    // note: the following tests don't need stubbing for the file operations,
    // because MFileDataStore's file change notification merely compares
    // an internal snaphot to internal QSettings. Thus the following tests
    // actually unit test what happens when a file is modified. The QSettings
    // is automatically updated, so the fileChanged slot merely compares
    // those QSettings to the internal snapshot map.

    // Now signal emitted with correct file name. This will remove "key",
    // and add "key1" and "key2"
    emit fileChanged(gStoreFile.at(ORIGINAL_SETTINGS_FILE_INDEX));
    QCOMPARE(valueSpy.count(), 3);
    QCOMPARE(m_subject->contains("key"), false);
    QCOMPARE(m_subject->contains("key1"), true);
    QCOMPARE(m_subject->contains("key2"), true);

    // removal of a key from the file
    changedSettingsMap->remove("key1");
    emit fileChanged(gStoreFile.at(ORIGINAL_SETTINGS_FILE_INDEX));
    QCOMPARE(valueSpy.count(), 4);
    QCOMPARE(m_subject->contains("key1"), false);

    // change of a value in the file
    (*changedSettingsMap)["key2"] = "newValue";
    emit fileChanged(gStoreFile.at(ORIGINAL_SETTINGS_FILE_INDEX));
    QCOMPARE(valueSpy.count(), 5);
    QCOMPARE(m_subject->contains("key2"), true);
    QCOMPARE(m_subject->value("key2").toString(), QString("newValue"));
}

void Ut_MFileDataStore::testSyncFailure()
{
    m_subject->createValue("foo", "bar");
    gQSettingsSyncFails = true;

    // Shouldn't be possible to clear
    QStringList keys1 = m_subject->allKeys();
    m_subject->clear();
    QStringList keys2 = m_subject->allKeys();
    QCOMPARE(keys1, keys2);

    // Shouldn't be possible to create a new value
    m_subject->createValue("bar", "bapapa");
    QCOMPARE(m_subject->contains("bar"), false);

    // Shouldn't be possible to change the value
    m_subject->setValue("foo", "baz");
    QCOMPARE(m_subject->value("foo").toString(), QString("bar"));

    // Shouldn't be possible to remove a value
    m_subject->remove("foo");
    QCOMPARE(m_subject->contains("foo"), true);

    // Removing nonexistent value should not result in
    // the value being present
    m_subject->remove("nonexistent");
    QCOMPARE(m_subject->contains("nonexistent"), false);

    // Setting nonexistent value should not result in
    // the value being present
    m_subject->setValue("nonexistent", "badvalue");
    QCOMPARE(m_subject->contains("nonexistent"), false);

    gQSettingsSyncFails = false;
}

QTEST_APPLESS_MAIN(Ut_MFileDataStore)
