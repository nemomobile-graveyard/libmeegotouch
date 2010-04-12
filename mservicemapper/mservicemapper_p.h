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

#ifndef MSERVICEMAPPERPRIVATE_H
#define MSERVICEMAPPERPRIVATE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QFileSystemWatcher>

class MServiceMapperPrivate
{
public:
    struct ServiceInfo {
        QString service;
        QString interface;
    };
    QMap<QString, ServiceInfo> serviceFileInfo;
    QList<QStringList>         serviceFileList;
    QFileSystemWatcher         watcher;
    QString                    serviceFileDir;
public:
    QStringList fillServiceFileList() const;
    bool fileExistsAndReadable(const QString &fileName) const;
    QIODevice *accessFile(const QString &fileName) const;
};

#endif
