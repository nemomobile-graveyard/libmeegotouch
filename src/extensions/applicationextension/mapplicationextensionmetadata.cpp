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

#include "mapplicationextensionmetadata.h"
#include <QDir>
#include <MDebug>

const QString EXTENSION_BINARY_KEY("X-MeeGoApplicationExtension/Extension");
const QString IDENTIFIER_KEY("X-MeeGoApplicationExtension/Identifier");
const QString INTERFACE_KEY("X-MeeGoApplicationExtension/Interface");

MApplicationExtensionMetaData::MApplicationExtensionMetaData(const QString &fileName) : MDesktopEntry(fileName)
{
}

MApplicationExtensionMetaData::~MApplicationExtensionMetaData()
{
}

bool MApplicationExtensionMetaData::isValid() const
{
    // Make sure that the metadata file is a valid desktop file.
    if (!MDesktopEntry::isValid()) {
        return false;
    }

    // Loop through keys and check that all are found
    QStringList requiredKeys;
    requiredKeys << EXTENSION_BINARY_KEY << INTERFACE_KEY;
    foreach(const QString &key, requiredKeys) {
        if (!contains(key)) {
            return false;
        }
    }

    // If metadata contains runner binary, check that the binary exists.
    if (exec().length() > 0 && runnerBinary() == NULL) {
        return false;
    }

    // Check that extension binary exists in filesystem.
    if (extensionBinary() == NULL) {
        return false;
    }

    return true;
}

QString MApplicationExtensionMetaData::runnerBinary() const
{
    // If runner binary is specified, return absolute file path.
    QString s = exec();
    if (s.length() > 0) {
        QFileInfo runner(QString(APPLICATION_EXTENSION_LIBS), s);
        if (runner.exists() && runner.isFile() && runner.isExecutable()) {
            return runner.absoluteFilePath();
        } else {
            mDebug("MApplicationExtensionMetaData") << "runner" << runner.absoluteFilePath() << "does not exist!";
        }
    }
    return NULL;
}

QString MApplicationExtensionMetaData::extensionBinary() const
{
    // Fetch the absolute file path and return it.
    QFileInfo extension(QString(APPLICATION_EXTENSION_LIBS), value(EXTENSION_BINARY_KEY));
    if (extension.exists() && extension.isFile()) {
        return extension.absoluteFilePath();
    } else {
        mDebug("MApplicationExtensionMetaData") << "extension" << extension.absoluteFilePath() << "does not exist!";
    }
    return NULL;
}

QString MApplicationExtensionMetaData::interface() const
{
    return value(INTERFACE_KEY);
}

QString MApplicationExtensionMetaData::extractLibraryName(const QString &libFileName)
{
    QRegExp regExp(".*lib([^/]*)[.]so");

    if (regExp.exactMatch(libFileName)) {
        // The given argument can be treated as valid
        QString tmp(libFileName);
        tmp.replace(regExp, "\\1");
        return tmp;
    } else {
        // Invalid input - return empty string
        return QString();
    }
}

QString MApplicationExtensionMetaData::resourceIdentifier() const
{
    QString resourceId;
    QString binary = extensionBinary();

    if (contains(IDENTIFIER_KEY)) {
        resourceId = value(IDENTIFIER_KEY);
    } else if (!binary.isNull()) {
        resourceId = extractLibraryName(binary);
    } else {
        return NULL;
    }

    return resourceId;
}
