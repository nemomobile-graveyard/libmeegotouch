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

#include "mappletmetadata.h"
#include <QDir>
#include <MDebug>

const QString AppletBinaryKey("X-MeeGoApplet/Applet");
const QString IdentifierKey("X-MeeGoApplet/Identifier");
const QString IconKey("Desktop Entry/Icon");

// Required desktop entry keys in an applet metadata file
QList<QString> RequiredKeys;

MAppletMetaData::MAppletMetaData(const QString &fileName) : MDesktopEntry(fileName)
{
    if (RequiredKeys.isEmpty()) {
        RequiredKeys << AppletBinaryKey << IconKey;
    }
}

MAppletMetaData::~MAppletMetaData()
{
}

bool MAppletMetaData::isValid() const
{
    // Make sure that applet metadata file is a valid desktop file.
    if (!MDesktopEntry::isValid())
        return false;

    // Loop through keys and check that all are found
    const int length = RequiredKeys.length();
    for (int i = 0; i < length; ++i) {
        if (!contains(RequiredKeys[i])) {
            return false;
        }
    }

    // If metadata contains runner binary, check that the binary exists.
    if (exec().length() > 0 && runnerBinary() == NULL) {
        return false;
    }

    // Check that applet binary exists in filesystem.
    if (appletBinary() == NULL) {
        return false;
    }

    return true;
}

QString MAppletMetaData::runnerBinary() const
{
    // If runner binary is specified, return absolute file path.
    QString s = exec();
    if (s.length() > 0) {
        QFileInfo runner(QString(APPLET_LIBS), s);
        if (runner.exists() && runner.isFile() && runner.isExecutable()) {
            return runner.absoluteFilePath();
        } else {
            mDebug("MAppletMetaData") << "runner" << runner.absoluteFilePath() << "does not exist!";
        }
    }
    return NULL;
}

QString MAppletMetaData::appletBinary() const
{
    // Fetch the absolute file path and return it.
    QFileInfo applet(QString(APPLET_LIBS), value(AppletBinaryKey));
    if (applet.exists() && applet.isFile()) {
        return applet.absoluteFilePath();
    } else {
        mDebug("MAppletMetaData") << "applet" << applet.absoluteFilePath() << "does not exist!";
    }
    return NULL;
}

QString MAppletMetaData::extractLibraryName(const QString &libFileName)
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

QString MAppletMetaData::resourceIdentifier() const
{
    QString resourceId;
    QString binary = appletBinary();

    if (contains(IdentifierKey)) {
        resourceId = value(IdentifierKey);
    } else if (!binary.isNull()) {
        resourceId = extractLibraryName(binary);
    } else {
        return NULL;
    }

    return resourceId;
}
