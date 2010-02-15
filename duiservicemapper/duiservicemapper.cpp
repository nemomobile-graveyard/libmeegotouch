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

#include <QDir>
#include <QSettings>
#include <QDebug>

#include "duiservicemapper.h"
#include "duiservicemapper_p.h"

namespace
{
    enum { LastList, CurrList };
};

DuiServiceMapper::DuiServiceMapper(const QString &serviceFileDir) :
    d_ptr(new DuiServiceMapperPrivate())
{
    d_ptr->serviceFileDir = serviceFileDir;
    d_ptr->serviceFileList.append(QStringList());
    d_ptr->serviceFileList.append(QStringList());

    d_ptr->serviceFileList[LastList] = d_ptr->fillServiceFileList();
    fillServiceFileInfo();

    d_ptr->watcher.addPath(d_ptr->serviceFileDir);
    connect(&d_ptr->watcher, SIGNAL(directoryChanged(QString)),
            this, SLOT(handleServiceChanged(QString)));
}

DuiServiceMapper::~DuiServiceMapper()
{
}

QString DuiServiceMapper::serviceName(const QString &interfaceName)
{
    QStringList serviceNameList = serviceNames(interfaceName);

    QString serviceName;
    if (!serviceNameList.isEmpty()) {
        serviceName = preferredService(serviceNameList);
    }

    bool serviceNameInvalid = serviceName.contains(" ");   // "not provided" - when the service wouldn't run
    if (serviceNameInvalid) {
        serviceName.clear();
    }

    return serviceName;
}

QStringList DuiServiceMapper::serviceNames(const QString &interfaceName)
{
    d_ptr->serviceFileList[LastList] = d_ptr->fillServiceFileList();
    int serviceFileNum = d_ptr->serviceFileList[LastList].size();

    QStringList serviceNameList;

    for (int i = 0; i < serviceFileNum; ++i) {
        QMap <QString, QString> thisServiceInterfacePair = serviceInterfacePair(d_ptr->serviceFileList[LastList][i]);

        if (interfaceName.isEmpty() || thisServiceInterfacePair.value("Interface") == interfaceName) {
            DuiServiceMapperPrivate::ServiceInfo serviceInfo;
            serviceInfo.interface = interfaceName;
            serviceInfo.service = thisServiceInterfacePair.value("Name");
            serviceNameList << serviceInfo.service;
            d_ptr->serviceFileInfo.insert(d_ptr->serviceFileList[LastList][i], serviceInfo);
        }
    }

    return serviceNameList;
}

QString DuiServiceMapper::preferredService(const QStringList &serviceList) const
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

void DuiServiceMapper::handleServiceChanged(const QString &path)
{
    Q_UNUSED(path);   // we do not support changing of the service dir
    // on the fly for now.

    d_ptr->serviceFileList[CurrList] = d_ptr->fillServiceFileList();

    bool isServiceAdded;
    int currListSize = d_ptr->serviceFileList[CurrList].size();
    int lastListSize = d_ptr->serviceFileList[LastList].size();

    int list0, list1;
    if (currListSize > lastListSize) {
        list0 = LastList;
        list1 = CurrList;
        isServiceAdded = true;
    } else {
        list0 = CurrList;
        list1 = LastList;
        isServiceAdded = false;
    }

    // difference between the lists
    int i = 0;
    const int size = d_ptr->serviceFileList[list0].size();
    for (; i < size; ++i) {
        if (d_ptr->serviceFileList[list0][i] != d_ptr->serviceFileList[list1][i]) {
            break;
        }
    }

    QMap <QString, QString> thisServiceInterfacePair = serviceInterfacePair(d_ptr->serviceFileList[list1][i]);

    if (!thisServiceInterfacePair.isEmpty()) {
        if (isServiceAdded) {
            emit serviceAvailable(thisServiceInterfacePair.value("Name"), thisServiceInterfacePair.value("Interface"));
        } else {
            emit serviceUnavailable(thisServiceInterfacePair.value("Name"));
        }
    } else {
        // serviceInterfacePair is empty
    }

    d_ptr->serviceFileList[LastList] = d_ptr->serviceFileList[CurrList];
}

QMap <QString, QString> DuiServiceMapper::serviceInterfacePair(const QString &fileName) const
{
    QMap <QString, QString> serviceInterfacePair;
    QString absoluteFileName = QDir(d_ptr->serviceFileDir).absoluteFilePath(fileName);

    if (d_ptr->fileExistsAndReadable(absoluteFileName)) {
        QIODevice *file = d_ptr->accessFile(absoluteFileName);
        if (file->open(QIODevice::ReadOnly)) {
            QTextStream in(file);

            while (!in.atEnd()) {
                QString line = in.readLine();

                QStringList fields = line.split('=');

                if (fields[0] == "Name" ||
                        fields[0] == "Interface") {
                    serviceInterfacePair.insert(fields[0], fields[1]);
                }
            }
        }

        delete file;
    } else {
        // file removed
        if (d_ptr->serviceFileInfo.contains(fileName)) {
            serviceInterfacePair.insert("Name", d_ptr->serviceFileInfo.value(fileName).service);
            serviceInterfacePair.insert("Interface", d_ptr->serviceFileInfo.value(fileName).interface);
        } else {
            // serviceFileInfo doesn't contain fileName
        }
    }

    return serviceInterfacePair;
}

QString DuiServiceMapper::servicePath(const QString &interfaceName) const
{
    Q_UNUSED(interfaceName);
    // Iterate through installed services and return the preferred one
    // for now all services' object paths are "/"
    return QString("/");
}

void DuiServiceMapper::fillServiceFileInfo()
{
    d_ptr->serviceFileList[LastList] = d_ptr->fillServiceFileList();
    int serviceFileNum = d_ptr->serviceFileList[LastList].size();

    for (int i = 0; i < serviceFileNum; ++i) {
        QMap <QString, QString> thisServiceInterfacePair = serviceInterfacePair(d_ptr->serviceFileList[LastList][i]);

        DuiServiceMapperPrivate::ServiceInfo serviceInfo;
        serviceInfo.service =  thisServiceInterfacePair.value("Name");
        serviceInfo.interface =  thisServiceInterfacePair.value("Interface");
        d_ptr->serviceFileInfo.insert(d_ptr->serviceFileList[LastList][i], serviceInfo);
    }
}

QString DuiServiceMapper::interfaceName(const QString &serviceName) const
{
    if (serviceName.isEmpty())
        return QString();

    QString retVal;
    d_ptr->serviceFileList[LastList] = d_ptr->fillServiceFileList();
    int serviceFileNum = d_ptr->serviceFileList[LastList].size();

    QStringList serviceNameList;

    bool serviceFound = false;
    bool atEndOfList = false;
    int i = 0;
    while (!serviceFound && !atEndOfList) {
        QMap <QString, QString> thisServiceInterfacePair = serviceInterfacePair(d_ptr->serviceFileList[LastList][i]);

        if (thisServiceInterfacePair.value("Name") == serviceName) {
            retVal = thisServiceInterfacePair.value("Interface");
            serviceFound = true;
        }

        if (i < serviceFileNum) {
            i++;
        } else {
            atEndOfList = true;
        }
    }

    return retVal;
}
