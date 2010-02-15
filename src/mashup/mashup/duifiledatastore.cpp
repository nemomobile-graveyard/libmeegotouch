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

#include "duifiledatastore.h"
#include <QTemporaryFile>
#include <QFileInfo>

/*!
 * Creates a temporary file in the directory of an original file.
 * \param originalPath Absolute path that is used as a base for generating
 * the temporary file.
 * \return Path of the created file, or an empty string if creating the
 * file fails. The returned value is a copy, so for uses where it's assigned
 * to a variable, it need not be const. For cases where it's passed as a
 * parameter, it need not be const because references to non-const will
 * not bind to function return values, because they are rvalues. When
 * C++0x brings rvalue references, the value should not be const in order
 * to allow rvalue references to bind to it and thus enable moving from it.
 */
static QString createTempFile(const QString &originalPath)
{
    QString returnValue;
    QTemporaryFile tempFile(originalPath);
    if (tempFile.open()) {
        tempFile.setAutoRemove(false);
        returnValue = tempFile.fileName();
    }
    return returnValue;
}

/*!
 * Copies settings from a QSettings object to another QSettings object.
 * \param originalSettings Settings to copy.
 * \param newSettings Target of the copy.
 * \return true if copying succeeds, false if it fails.
 */
static bool copySettings(const QSettings &originalSettings,
                         QSettings &newSettings)
{
    QStringList keys = originalSettings.allKeys();
    foreach(const QString & key, originalSettings.allKeys()) {
        newSettings.setValue(key, originalSettings.value(key));
        if (newSettings.status() != QSettings::NoError) {
            return false;
        }
    }
    return true;
}

/*!
 * Renames a file. Ensures that a file with the new name
 * doesn't exist.
 * \param oldname Name of the file to rename.
 * \param newName New name.
 */
static void renameSettingFile(const QString &oldName,
                              const QString &newName)
{
    QFile::remove(newName);
    QFile::rename(oldName, newName);
}

/*!
 * Adds the necessary paths to the file system watcher
 * \param filePath Path (including name) of the file to watch.
 * \param watcher The file system watcher.
 */
static void addPathsToWatcher(const QString &filePath,
                              QScopedPointer<QFileSystemWatcher>& watcher)
{
    // Watch the directory if it's not being watched yet
    QString canonicalPath(QFileInfo(filePath).canonicalPath());
    if (!watcher->directories().contains(canonicalPath)) {
        watcher->addPath(canonicalPath);
    }

    // Watch the file itself if it's not being watched yet
    if (!watcher->files().contains(filePath)) {
        watcher->addPath(filePath);
    }
}

/*!
 * Saves the settings to a file. The settings are first
 * saved to a temporary file, and then that file is copied
 * over the original settings. This avoids clearing settings
 * when there's no disk space.
 * \param originalSettings Settings to save.
 */
static bool doSync(QSettings &originalSettings, QScopedPointer<QFileSystemWatcher>& watcher)
{
    bool returnValue = false;
    QString tempFileName = createTempFile(originalSettings.fileName());
    if (!tempFileName.isEmpty()) {
        QSettings copiedSettings(tempFileName, QSettings::IniFormat);
        if (copySettings(originalSettings, copiedSettings)) {
            copiedSettings.sync();
            if (copiedSettings.status() == QSettings::NoError) {
                renameSettingFile(tempFileName, originalSettings.fileName());
                returnValue = true;
            }
        }
    }
    addPathsToWatcher(originalSettings.fileName(), watcher);
    return returnValue;
}

DuiFileDataStore::DuiFileDataStore(const QString &filePath) :
    settings(QString(filePath), QSettings::IniFormat),
    watcher(new QFileSystemWatcher())
{
    settings.sync();
    takeSnapshot();
    addPathsToWatcher(filePath, watcher);
    connect(watcher.data(), SIGNAL(fileChanged(QString)),
            this, SLOT(fileChanged(QString)));
    connect(watcher.data(), SIGNAL(directoryChanged(QString)),
            this, SLOT(directoryChanged(QString)));
}

bool DuiFileDataStore::createValue(const QString &key, const QVariant &value)
{
    bool returnValue = false;
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable()) {
        bool originalValueSet = settings.contains(key);
        QVariant originalValue = settings.value(key);
        settings.setValue(key, value);
        bool syncOk = doSync(settings, watcher);
        if (syncOk) {
            returnValue = true;
            // Emit valueChanged signal when value is changed or a new key is added
            if ((originalValueSet && originalValue != value)
                    || !originalValueSet) {
                settingsSnapshot[key] = value;
                emit valueChanged(key, value);
            }
        } else if (originalValueSet) {
            // if sync fails, make sure the value in memory is the original
            settings.setValue(key, originalValue);
        } else {
            settings.remove(key);
        }

    }
    return returnValue;
}

bool DuiFileDataStore::setValue(const QString &key, const QVariant &value)
{
    bool returnValue = false;
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable() && settings.contains(key)) {
        bool originalValueSet = settings.contains(key);
        QVariant originalValue = settings.value(key);
        settings.setValue(key, value);
        bool syncOk = doSync(settings, watcher);
        if (syncOk) {
            returnValue = true;
            // Emit valueChanged signal when value is changed
            if (originalValue != value) {
                settingsSnapshot[key] = value;
                emit valueChanged(key, value);
            }
        } else if (originalValueSet) {
            // if sync fails, make sure the value in memory is the original
            settings.setValue(key, originalValue);
        } else {
            settings.remove(key);
        }
    }
    return returnValue;
}

QVariant DuiFileDataStore::value(const QString &key) const
{
    return settings.value(key);
}

QStringList DuiFileDataStore::allKeys() const
{
    return settings.allKeys();
}

void DuiFileDataStore::remove(const QString &key)
{
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable()) {
        bool originalValueSet = settings.contains(key);
        if (!originalValueSet) {
            return;
        }
        QVariant originalValue = settings.value(key);
        settings.remove(key);
        bool syncOk = doSync(settings, watcher);
        if (!syncOk) {
            if (originalValueSet) {
                // if sync fails, make sure the value in memory is the original
                settings.setValue(key, originalValue);
            }
        } else {
            settingsSnapshot.remove(key);
            emit valueChanged(key, QVariant());
        }
    }
}

void DuiFileDataStore::clear()
{
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable()) {
        settings.clear();
        settings.sync();
        takeSnapshot();
    }
}

bool DuiFileDataStore::contains(const QString &key) const
{
    return settings.contains(key);
}

bool DuiFileDataStore::isReadable() const
{
    return settings.status() == QSettings::NoError;
}

bool DuiFileDataStore::isWritable() const
{
    return settings.isWritable() && settings.status() == QSettings::NoError;
}

void DuiFileDataStore::takeSnapshot()
{
    settingsSnapshot.clear();
    foreach(const QString & key, settings.allKeys()) {
        settingsSnapshot.insert(key, settings.value(key));
    }
}

void DuiFileDataStore::fileChanged(const QString &fileName)
{
    // sync the settings and add the path, for observing
    // the file even if it was deleted
    settings.sync();
    addPathsToWatcher(settings.fileName(), watcher);
    if (settings.fileName() == fileName && isWritable()) {
        // Check whether the values for existing keys have changed or
        // if keys have been deleted
        foreach(const QString & key, settingsSnapshot.keys()) {
            if ((settings.contains(key)
                    && settings.value(key) != settingsSnapshot.value(key))
                    || (!settings.contains(key))) {
                emit valueChanged(key, settings.value(key));
            }
        }
        // Check whether new keys have been added
        foreach(const QString & key, settings.allKeys()) {
            if (!settingsSnapshot.contains(key)) {
                emit valueChanged(key, settings.value(key));
            }
        }
        takeSnapshot();
    }
}

void DuiFileDataStore::directoryChanged(const QString &fileName)
{
    if (fileName == QFileInfo(settings.fileName()).canonicalPath()) {
        // we can't know which file changed, so we'll sync at this
        // point. This is not very optimal, but it at least works.
        fileChanged(settings.fileName());
    }
}

