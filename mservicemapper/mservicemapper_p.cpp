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
#include <QSettings>

#include "mservicemapper_p.h"

MServiceMapperPrivate::MServiceMapperPrivate( const QString &serviceFileDir ):
    m_serviceFileDir( serviceFileDir )
{
}

void MServiceMapperPrivate::init()
{
    m_serviceFileList.append(QStringList());
    m_serviceFileList.append(QStringList());

    if ( !m_serviceFileDir.isEmpty() ) {
        m_watcher.addPath(m_serviceFileDir);
    }
}

QStringList MServiceMapperPrivate::fillServiceFileList() const
{
    QDir dir = QDir(m_serviceFileDir);
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

QString MServiceMapperPrivate::preferredService(const QStringList &serviceList) const
{
    // policy needed, e.g. define policy in repository
    QSettings policy("services.policy", QSettings::IniFormat);
    //policy.setValue("provider", "nokia");
    QString provider = policy.value("provider").toString();
    int serviceIndex = 0;

    if (!provider.isEmpty()) {
        //iterate list to find the 1st service of the provider
        const int size = serviceList.size();
        for (int i = 0; i < size; ++i) {
            if (serviceList[i].contains(provider)) {
                serviceIndex = i;
                break;
            }
        }
    }

    return serviceList[serviceIndex];
}

MServiceMapperPrivate::ServiceInfo MServiceMapperPrivate::serviceInterfacePair(const QString &fileName) const
{
    MServiceMapperPrivate::ServiceInfo serviceInterfacePair;

    if ( m_serviceFileInfo.contains( fileName ) ) {
        serviceInterfacePair = m_serviceFileInfo.value( fileName );
    } else {
        QString absoluteFileName = QDir(m_serviceFileDir).absoluteFilePath(fileName);

        if (fileExistsAndReadable(absoluteFileName)) {
            QIODevice *file = accessFile(absoluteFileName);
            if (file->open(QIODevice::ReadOnly)) {
                QTextStream in(file);

                while (!in.atEnd()) {
                    QString line = in.readLine();

                    QStringList fields = line.split('=');

                    if (fields[0] == "Name") {
                        serviceInterfacePair.service=fields[1];
                    } else
                        if (fields[0] == "Interface") {
                            serviceInterfacePair.interface=fields[1];
                        }
                }
            }

            delete file;
        }
    }

    return serviceInterfacePair;
}

void MServiceMapperPrivate::fillServiceFileInfo()
{
    m_serviceFileList[CurrList] = fillServiceFileList();
    int serviceFileNum = m_serviceFileList[CurrList].size();

    for (int i = 0; i < serviceFileNum; ++i) {
        MServiceMapperPrivate::ServiceInfo serviceInfo = serviceInterfacePair(m_serviceFileList[CurrList][i]);

        m_serviceFileInfo.insert(m_serviceFileList[CurrList][i], serviceInfo);
    }
}
