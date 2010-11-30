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
#include "mfiledatastore.h"
#include "mfiledatastore_p.h"
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
    QFileInfo fileInfo(filePath);
    QString directory;
    bool fileExists = fileInfo.exists();
    if (fileExists) {
        // If the file exists, we can take the canonical path directly
        directory = fileInfo.canonicalPath();
    } else {
        // If the file doesn't exist, canonicalPath would return an empty string. That's why
        // we need to get the parent directory first.
        QFileInfo parentPath(fileInfo.absolutePath());
        if (parentPath.exists()) {
            directory = parentPath.canonicalFilePath();
        }
    }

    if (!directory.isEmpty()) {
        // Watch the directory if it's not being watched yet
        if (!watcher->directories().contains(directory)) {
            watcher->addPath(directory);
        }
    }

    // Watch the file itself if it's not being watched yet
    if (fileExists && !watcher->files().contains(filePath)) {
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

MFileDataStorePrivate::MFileDataStorePrivate(const QString &filePath) :
    settings(filePath, QSettings::IniFormat),
    watcher(new QFileSystemWatcher())
{
    settings.sync();
}

MFileDataStore::MFileDataStore(const QString &filePath) :
    d_ptr(new MFileDataStorePrivate(filePath))
{
    Q_D(MFileDataStore);
    takeSnapshot();
    addPathsToWatcher(filePath, d->watcher);
    connect(d->watcher.data(), SIGNAL(fileChanged(QString)),
            this, SLOT(fileChanged(QString)));
    connect(d->watcher.data(), SIGNAL(directoryChanged(QString)),
            this, SLOT(directoryChanged(QString)));
}

MFileDataStore::~MFileDataStore()
{
    delete d_ptr;
}

bool MFileDataStore::createValue(const QString &key, const QVariant &value)
{
    Q_D(MFileDataStore);
    bool returnValue = false;
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable()) {
        bool originalValueSet = d->settings.contains(key);
        QVariant originalValue = d->settings.value(key);
        d->settings.setValue(key, value);
        bool syncOk = doSync(d->settings, d->watcher);
        if (syncOk) {
            returnValue = true;
            // Emit valueChanged signal when value is changed or a new key is added
            if ((originalValueSet && originalValue != value)
                    || !originalValueSet) {
                d->settingsSnapshot[key] = value;
                emit valueChanged(key, value);
            }
        } else if (originalValueSet) {
            // if sync fails, make sure the value in memory is the original
            d->settings.setValue(key, originalValue);
        } else {
            d->settings.remove(key);
        }

    }
    return returnValue;
}

bool MFileDataStore::setValue(const QString &key, const QVariant &value)
{
    Q_D(MFileDataStore);
    bool returnValue = false;
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable() && d->settings.contains(key)) {
        QVariant originalValue = d->settings.value(key);
        d->settings.setValue(key, value);
        bool syncOk = doSync(d->settings, d->watcher);
        if (syncOk) {
            returnValue = true;
            // Emit valueChanged signal when value is changed
            if (originalValue != value) {
                d->settingsSnapshot[key] = value;
                emit valueChanged(key, value);
            }
        } else {
            // if sync fails, make sure the value in memory is the original
            d->settings.setValue(key, originalValue);
        }
    }
    return returnValue;
}

QVariant MFileDataStore::value(const QString &key) const
{
    Q_D(const MFileDataStore);
    return d->settings.value(key);
}

QStringList MFileDataStore::allKeys() const
{
    Q_D(const MFileDataStore);
    return d->settings.allKeys();
}

void MFileDataStore::remove(const QString &key)
{
    Q_D(MFileDataStore);
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable()) {
        bool originalValueSet = d->settings.contains(key);
        if (!originalValueSet) {
            return;
        }
        QVariant originalValue = d->settings.value(key);
        d->settings.remove(key);
        bool syncOk = doSync(d->settings, d->watcher);
        if (!syncOk) {
            if (originalValueSet) {
                // if sync fails, make sure the value in memory is the original
                d->settings.setValue(key, originalValue);
            }
        } else {
            d->settingsSnapshot.remove(key);
            emit valueChanged(key, QVariant());
        }
    }
}

void MFileDataStore::clear()
{
    Q_D(MFileDataStore);
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable()) {
        d->settings.clear();
        d->settings.sync();
        takeSnapshot();
    }
}

bool MFileDataStore::contains(const QString &key) const
{
    Q_D(const MFileDataStore);
    return d->settings.contains(key);
}

bool MFileDataStore::isReadable() const
{
    Q_D(const MFileDataStore);
    return d->settings.status() == QSettings::NoError;
}

bool MFileDataStore::isWritable() const
{
    Q_D(const MFileDataStore);
    return d->settings.isWritable() && d->settings.status() == QSettings::NoError;
}

void MFileDataStore::takeSnapshot()
{
    Q_D(MFileDataStore);
    d->settingsSnapshot.clear();
    foreach(const QString & key, d->settings.allKeys()) {
        d->settingsSnapshot.insert(key, d->settings.value(key));
    }
}

void MFileDataStore::fileChanged(const QString &fileName)
{
    Q_D(MFileDataStore);
    // sync the settings and add the path, for observing
    // the file even if it was deleted
    d->settings.sync();
    addPathsToWatcher(d->settings.fileName(), d->watcher);
    if (d->settings.fileName() == fileName && isWritable()) {
        // Check whether the values for existing keys have changed or
        // if keys have been deleted
        foreach(const QString & key, d->settingsSnapshot.keys()) {
            if ((d->settings.contains(key)
                    && d->settings.value(key) != d->settingsSnapshot.value(key))
                    || (!d->settings.contains(key))) {
                emit valueChanged(key, d->settings.value(key));
            }
        }
        // Check whether new keys have been added
        foreach(const QString & key, d->settings.allKeys()) {
            if (!d->settingsSnapshot.contains(key)) {
                emit valueChanged(key, d->settings.value(key));
            }
        }
        takeSnapshot();
    }
}

void MFileDataStore::directoryChanged(const QString &fileName)
{
    Q_D(MFileDataStore);
    if (fileName == QFileInfo(d->settings.fileName()).canonicalPath()) {
        // we can't know which file changed, so we'll sync at this
        // point. This is not very optimal, but it at least works.
        fileChanged(d->settings.fileName());
    }
}

