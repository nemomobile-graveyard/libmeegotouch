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

#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QString>
#include <QStringList>

#include "mservicemapper_p.h"

QStringList MServiceMapperPrivate::fillServiceFileList() const
{
    QDir dir = QDir(serviceFileDir);
    dir.setNameFilters(QStringList("*.service"));
    return dir.entryList(QDir::Files | QDir::NoSymLinks, QDir::Name);
}

bool MServiceMapperPrivate::fileExistsAndReadable(const QString &fileName) const
{
    bool fileExists = QFile::exists(fileName);
    bool fileIsReadable = (QFile::permissions(fileName) & QFile::ReadUser);

    return fileExists && fileIsReadable;
}

QIODevice *MServiceMapperPrivate::accessFile(const QString &fileName) const
{
    return new QFile(fileName);
}
