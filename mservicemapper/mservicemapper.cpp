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

#include "mservicemapper.h"

#include <QDir>
#include <QSettings>
#include <QDebug>

#include "mservicemapper_p.h"

#ifdef HAVE_QTM_SERVICEMAPPER
#include <QServiceManager>

QTM_USE_NAMESPACE
#endif

MServiceMapper::MServiceMapper(const QString &serviceFileDir) :
    d_ptr(new MServiceMapperPrivate( serviceFileDir ))
{
    init();
}

void MServiceMapper::init()
{
    d_ptr->init();
    connect(&d_ptr->m_watcher, SIGNAL(directoryChanged(QString)),
            this, SLOT(handleServiceChanged(QString)));

    handleServiceChanged( QString() );
}

MServiceMapper::~MServiceMapper()
{
    delete d_ptr;
}

QString MServiceMapper::serviceName(const QString &interfaceName)
{
    QStringList serviceNameList = serviceNames(interfaceName);

    QString serviceName;
    if (!serviceNameList.isEmpty()) {
        serviceName = d_ptr->preferredService(serviceNameList);
    }

    bool serviceNameInvalid = serviceName.contains(" ");   // "not provided" - when the service wouldn't run
    if (serviceNameInvalid) {
        serviceName.clear();
    }

    return serviceName;
}

QStringList MServiceMapper::serviceNames(const QString &interfaceName)
{
    d_ptr->m_serviceFileList[CurrList] = d_ptr->fillServiceFileList();
    int serviceFileNum = d_ptr->m_serviceFileList[CurrList].size();

    QStringList serviceNameList;

    for (int i = 0; i < serviceFileNum; ++i) {
        MServiceMapperPrivate::ServiceInfo serviceInfo = d_ptr->serviceInterfacePair(d_ptr->m_serviceFileList[CurrList][i]);

        if (interfaceName.isEmpty() || serviceInfo.interface == interfaceName) {
            serviceNameList << serviceInfo.service;
            d_ptr->m_serviceFileInfo.insert(d_ptr->m_serviceFileList[CurrList][i], serviceInfo);
        }
    }

    return serviceNameList;
}

void MServiceMapper::handleServiceChanged(const QString &path)
{
    Q_UNUSED(path);   // we do not support changing of the service dir
    // on the fly for now.

    d_ptr->m_serviceFileList[LastList] = d_ptr->m_serviceFileList[CurrList];

    d_ptr->m_serviceFileList[CurrList] = d_ptr->fillServiceFileList();

    // find removed files - those in LastList that aren't in CurrList
    int lastListSize = d_ptr->m_serviceFileList[LastList].size();
    for ( int index=0; index<lastListSize; ++index ) {
        QString thisFile = d_ptr->m_serviceFileList[LastList][index];
        bool fileRemoved = !d_ptr->m_serviceFileList[CurrList].contains( thisFile );
        if ( fileRemoved ) {
            QString thisServiceName = d_ptr->m_serviceFileInfo.take(thisFile).service;

#ifdef HAVE_QTM_SERVICEMAPPER
            QServiceManager serviceManager;
            serviceManager.removeService( thisServiceName );
#endif
            emit serviceUnavailable( thisServiceName );
        }
    }

    // get info for new services
    d_ptr->fillServiceFileInfo();

    // find added files - those in CurrList that aren't in LastList
    int currListSize = d_ptr->m_serviceFileList[CurrList].size();
    for ( int index=0; index<currListSize; ++index ) {
        QString thisFile = d_ptr->m_serviceFileList[CurrList][index];
        bool fileAdded = !d_ptr->m_serviceFileList[LastList].contains( thisFile );
        if ( fileAdded ) {
            MServiceMapperPrivate::ServiceInfo thisServiceInterfacePair =
                d_ptr->serviceInterfacePair( thisFile );
            QString thisService   = thisServiceInterfacePair.service;
            QString thisInterface = thisServiceInterfacePair.interface;

            QString xmlFileName( "/usr/lib/maemo-meegotouch-services/"+thisService+".xml" );

#ifdef HAVE_QTM_SERVICEMAPPER
            QServiceManager serviceManager;
            serviceManager.addService( xmlFileName );
#endif

            emit serviceAvailable( thisService, thisInterface );
        }
    }

}

QString MServiceMapper::servicePath(const QString &interfaceName) const
{
    Q_UNUSED(interfaceName);
    // Iterate through installed services and return the preferred one
    // for now all services' object paths are "/"
    return QString("/");
}

QString MServiceMapper::interfaceName(const QString &serviceName) const
{
    if (serviceName.isEmpty())
        return QString();

    QString retVal;
    d_ptr->m_serviceFileList[CurrList] = d_ptr->fillServiceFileList();
    int serviceFileNum = d_ptr->m_serviceFileList[CurrList].size();

    QStringList serviceNameList;

    bool serviceFound = false;
    bool atEndOfList = false;
    int i = 0;
    while (!serviceFound && !atEndOfList) {
        MServiceMapperPrivate::ServiceInfo thisServiceInterfacePair = d_ptr->serviceInterfacePair(d_ptr->m_serviceFileList[CurrList][i]);

        if (thisServiceInterfacePair.service == serviceName) {
            retVal = thisServiceInterfacePair.interface;
            serviceFound = true;
        }

        if (i < serviceFileNum-1) {
            i++;
        } else {
            atEndOfList = true;
        }
    }

    return retVal;
}

#ifdef UNIT_TEST
MServiceMapper::MServiceMapper(MServiceMapperPrivate *priv) :
    d_ptr(priv)
{
    init();
}
#endif // UNIT_TEST
