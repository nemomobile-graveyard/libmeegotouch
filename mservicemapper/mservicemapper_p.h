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

class MServiceMapper;

namespace
{
    enum { LastList, CurrList };
};

class MServiceMapperPrivate
{
public:
    MServiceMapperPrivate( const QString &serviceFiledir );

    struct ServiceInfo {
        QString service;
        QString interface;
    };
    QMap<QString, ServiceInfo> m_serviceFileInfo;
    QList<QStringList>         m_serviceFileList;
    QFileSystemWatcher         m_watcher;
    QString                    m_serviceFileDir;

public:
    virtual QStringList fillServiceFileList() const;
    virtual bool fileExistsAndReadable(const QString &fileName) const;
    virtual QIODevice *accessFile(const QString &fileName) const;
    virtual void init();

    QString preferredService(const QStringList &serviceList) const;
    ServiceInfo serviceInterfacePair(const QString &filePath) const;
    void fillServiceFileInfo();
};

#endif
