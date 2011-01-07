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

#include "mlogicalvalues.h"
#include "mlogicalvalues_p.h"
#include "mstylesheetattribute.h"
#include "mdebug.h"
#include "msystemdirectories.h"

#include <QDir>
#include <QFile>
#include <QStringList>
#include <QTextCodec>
#include <QDateTime>
#include <QCoreApplication>

namespace {
    const unsigned int CACHE_VERSION = 1;
}

MLogicalValues::MLogicalValues() :
    d_ptr(new MLogicalValuesPrivate)
{
}

MLogicalValues::~MLogicalValues()
{
    delete d_ptr;
}

bool MLogicalValuesPrivate::parse(const QFileInfo &fileInfo, Groups &groups)
{
    QFile file(fileInfo.filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QByteArray group = "General";
    groups.insert("General", Values());

    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        // skip comments
        if (line.startsWith("[")) {
            // parse group header
            int index = line.indexOf("]", 1);
            if (index == -1) {
                mWarning("MLogicalValues") << "Error occurred when parsing .ini file:" << line;
                file.close();
                return false;
            }
            // this will be the currently active group
            group = line.mid(1, index - 1);
        } else {

            // key/value pair
            QByteArray key, value;
            QByteArray *target = &key;

            // stores the last 'good' character
            int truncation = 0;
            // go through whole line
            for (int i = 0; i < line.length(); i++) {
                QChar character = line.at(i);
                if (character == ';') {
                    break;
                } else if (character == '=') {
                    // remove trailing whitespaces
                    target->truncate(truncation);
                    // start to parse value
                    target = &value;
                    truncation = 0;
                } else {
                    if (target->isEmpty() && character.isSpace()) {
                        // do not add whitespaces at the beginning
                    } else {
                        (*target) += character;
                        if (!character.isSpace())
                            truncation = target->length();
                    }
                }
            }
            // remove trailing whitespaces
            target->truncate(truncation);

            // consistency check
            if (!line.startsWith(';') && line.length() > 0) {
                if (key.isEmpty() || value.isEmpty()) {
                    mWarning("MLogicalValues") << "Error occurred when parsing .ini file:" << line;
                    file.close();
                    return false;
                }
                // store
                Values &values = groups[group];

                if (!values.contains(key)) {
                    values.insert(key, value);
                }
            }
        }
    }

    saveToBinaryCache(fileInfo, groups);
    file.close();
    return true;
}

bool MLogicalValuesPrivate::loadFromBinaryCache(const QFileInfo &fileInfo, Groups &groups) {
    const QString cacheFileName = createBinaryFilename(fileInfo);

    if (QFile::exists(cacheFileName)) {
        QFile file(cacheFileName);
        if (file.open(QFile::ReadOnly)) {
            QDataStream stream(&file);
            uint version;
            stream >> version;
            if (version != CACHE_VERSION) {
                // will be replaced with up to date version
                file.close();
                return false;
            }
            uint timestamp;
            stream >> timestamp;
            if (timestamp != fileInfo.lastModified().toTime_t()) {
                // will be replaced with up to date version
                file.close();
                return false;
            }

            stream >> groups;

            file.close();
            return true;
        } else {
            mDebug("MLogicalValuesPrivate") << "Failed to load values from cache" << cacheFileName;
        }
    }

    return false;
}

bool MLogicalValuesPrivate::saveToBinaryCache(const QFileInfo &fileInfo, const Groups &groups) const {
    const QString cacheFileName = createBinaryFilename(fileInfo);

    QFile file(cacheFileName);
    if (!file.open(QFile::WriteOnly)) {
        //Maybe it failed because the directory doesn't exist
        QDir().mkpath(QFileInfo(cacheFileName).absolutePath());
        if (!file.open(QFile::WriteOnly)) {
            mDebug("MLogicalValuesPrivate") << "Failed to save cache file for" << fileInfo.fileName() << "to" << cacheFileName;
            return false;
        }
    }

    QDataStream stream(&file);
    stream << CACHE_VERSION;
    stream << fileInfo.lastModified().toTime_t();
    stream << groups;

    file.close();
    return true;
}

QString MLogicalValuesPrivate::createBinaryFilename(const QFileInfo &fileInfo) const {
    QString binaryDirectory = MSystemDirectories::cacheDirectory() + QLatin1String("logicalValues") + QDir::separator();
    QString binaryFilename(binaryDirectory);

    QString absoluteFilePathEncoded(fileInfo.absoluteFilePath());
    absoluteFilePathEncoded.replace('_', "__");
    absoluteFilePathEncoded.replace('/', "_.");
    binaryFilename += absoluteFilePathEncoded;
    return binaryFilename;
}

void MLogicalValuesPrivate::mergeGroups(const Groups &groups)
{
    Groups::const_iterator i = groups.constBegin();
    while (i != groups.constEnd()) {
        Values &values = data[i.key()];
        Values::const_iterator j = i.value().constBegin();
        while (j != i.value().constEnd()) {
            if (!values.contains(j.key())) {
                values.insert(j.key(), j.value());
            }
            ++j;
        }
        ++i;
    }
}

bool MLogicalValues::append(const QString &fileName)
{
    Q_D(MLogicalValues);

    // make sure that the file exists
    if (!QFile(fileName).exists())
        return false;

    Groups groups;
    QFileInfo fileInfo(fileName);
    if (!d->loadFromBinaryCache(fileInfo, groups)) {
        if (!d->parse(fileInfo, groups)) {
            return false;
        }
    }

    d->timestamps << fileInfo.lastModified().toTime_t();

    d->mergeGroups(groups);

    return true;
}

void MLogicalValues::load(const QStringList &themeInheritanceChain, const QString &locale)
{
    Q_D(MLogicalValues);

    d->data.clear();
    d->timestamps.clear();

    // load locale-specific constant definitions
    if (!locale.isEmpty()) {
        // go through whole inheritance hierarchy
        foreach(QString path, themeInheritanceChain) {
            append(path + QString("meegotouch") + QDir::separator() + QString("locale") + QDir::separator() + locale + QDir::separator() + QString("constants.ini"));
        }
    }

    // go through whole inheritance hierarchy
    foreach(QString path, themeInheritanceChain) {
        append(path + QString("meegotouch") + QDir::separator() + QString("constants.ini"));
    }
}

bool MLogicalValues::findKey(const QByteArray &key, QByteArray &group, QByteArray &value) const
{
    Q_D(const MLogicalValues);

    // search from every group
    for (Groups::const_iterator iterator = d->data.begin(); iterator != d->data.end(); iterator++) {
        // get values from this group
        const Values &values = iterator.value();

        // check if this group contains the key
        if (values.contains(key)) {
            group = iterator.key();
            value = values.value(key);
            return true;
        }
    }

    return false;
}


bool MLogicalValues::value(const QByteArray &group, const QByteArray &key, QByteArray &value) const
{
    Q_D(const MLogicalValues);
    if (!d->data.contains(group)) {
        mWarning("MLogicalValues") << "No such group:" << group;
        return false;
    }

    const Values &values = d->data[group];

    if (!values.contains(key)) {
        mWarning("MLogicalValues") << "No such key:" << group << '/' << key;
        return false;
    }

    value = values.value(key);
    return true;
}


QColor MLogicalValues::color(const QByteArray &group, const QByteArray &key) const
{
    QByteArray string;
    if (!value(group, key, string)) {
        return QColor();
    }

    bool conversionOk = false;
    QColor color = MStyleSheetAttribute::colorFromString(string, &conversionOk);
    if (!conversionOk) {
        mWarning("MLogicalValues") << "Invalid logical color definition for" << key << ':' << string;
    }
    return color;
}

QFont MLogicalValues::font(const QByteArray &group, const QByteArray &key) const
{
    QByteArray string;
    if (!value(group, key, string)) {
        return QFont();
    }

    bool conversionOk = false;
    QFont font = MStyleSheetAttribute::fontFromString(string, &conversionOk);
    if (!conversionOk) {
        mWarning("MLogicalValues") << "Invalid logical font definition for" << key << ':' << string;
    }
    return font;
}

QList<uint> MLogicalValues::timestamps() const
{
    Q_D(const MLogicalValues);
    return d->timestamps;
}

