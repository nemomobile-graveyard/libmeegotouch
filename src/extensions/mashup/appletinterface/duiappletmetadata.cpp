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

#include "duiappletmetadata.h"
#include <QDir>
#include <DuiDebug>


const QString AppletBinaryKey("DUI/X-DUIApplet-Applet");
const QString IdentifierKey("DUI/X-DUIApplet-Identifier");
const QString IconKey("Desktop Entry/Icon");

// Required desktop entry keys in an applet metadata file
QList<QString> RequiredKeys;

DuiAppletMetaData::DuiAppletMetaData(const QString &fileName) : DuiDesktopEntry(fileName)
{
    if (RequiredKeys.isEmpty()) {
        RequiredKeys << AppletBinaryKey << IconKey;
    }
}

DuiAppletMetaData::~DuiAppletMetaData()
{
}

bool DuiAppletMetaData::isValid() const
{
    // Make sure that applet metadata file is a valid desktop file.
    if (!DuiDesktopEntry::isValid())
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

QString DuiAppletMetaData::runnerBinary() const
{
    // If runner binary is specified, return absolute file path.
    QString s = exec();
    if (s.length() > 0) {
        QFileInfo runner(QString(APPLET_LIBS), s);
        if (runner.exists() && runner.isFile() && runner.isExecutable()) {
            return runner.absoluteFilePath();
        } else {
            duiDebug("DuiAppletMetaData") << "runner" << runner.absoluteFilePath() << "does not exist!";
        }
    }
    return NULL;
}

QString DuiAppletMetaData::appletBinary() const
{
    // Fetch the absolute file path and return it.
    QFileInfo applet(QString(APPLET_LIBS), value(AppletBinaryKey));
    if (applet.exists() && applet.isFile()) {
        return applet.absoluteFilePath();
    } else {
        duiDebug("DuiAppletMetaData") << "applet" << applet.absoluteFilePath() << "does not exist!";
    }
    return NULL;
}

QString DuiAppletMetaData::extractLibraryName(const QString &libFileName)
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

QString DuiAppletMetaData::resourceIdentifier() const
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
