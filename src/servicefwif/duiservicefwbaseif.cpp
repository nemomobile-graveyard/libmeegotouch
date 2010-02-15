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

#include "duiservicefwbaseif.h"

#include <QDBusAbstractInterface>
#include <QDBusConnection>
#include <DuiDebug>

DuiServiceFwBaseIf::DuiServiceFwBaseIf(const QString &iface, QObject *parent) :
    QObject(parent),
    interfaceProxy(0),
    serviceFwProxyPtr(new DuiServiceFwProxy(
                          "com.nokia.DuiServiceFw",
                          "/",
                          QDBusConnection::sessionBus()
                      )
                     ),
    service(),
    interface(iface)
{
    connect(serviceFwProxyPtr, SIGNAL(serviceAvailable(QString, QString)),
            this, SLOT(handleServiceAvailable(QString, QString)));

    connect(serviceFwProxyPtr, SIGNAL(serviceUnavailable(QString)),
            this, SLOT(handleServiceUnavailable(QString)));
}

DuiServiceFwBaseIf::~DuiServiceFwBaseIf()
{
    if (interfaceProxy) {
        delete interfaceProxy;
        interfaceProxy = 0;
    }

    if (serviceFwProxyPtr) {
        delete serviceFwProxyPtr;
        serviceFwProxyPtr = 0;
    }
}

bool DuiServiceFwBaseIf::isValid() const
{
    return (interfaceProxy != 0);
}

QStringList DuiServiceFwBaseIf::serviceNames(const QString &interface) const
{
    return serviceFwProxyPtr->serviceNames(interface);
}

QString DuiServiceFwBaseIf::serviceName() const
{
    return service;
}

DuiServiceFwProxy *DuiServiceFwBaseIf::serviceFwProxy()
{
    return serviceFwProxyPtr;
}

void DuiServiceFwBaseIf::handleServiceUnavailable(const QString &service)
{
    bool noCurrentService      = this->service.isEmpty();
    bool currentServiceHasGone = (service == this->service);

    if (noCurrentService || currentServiceHasGone) {
        // get new service for this interface
        this->service = serviceFwProxyPtr->serviceName(this->interface);

        bool noMoreServicesForThisInterface = this->service.isEmpty();
        if (noMoreServicesForThisInterface) {
            if (!noCurrentService)
                emit serviceUnavailable(service);
        } else {
            setService(this->service);
            emit serviceChanged(this->service);
        }
    }
}

void DuiServiceFwBaseIf::handleServiceAvailable(const QString &service, const QString &interface)
{
    bool newServiceIsForThisInterface = (interface == this->interface);
    if (newServiceIsForThisInterface) {
        QString previousService = this->service;

        // let service mappper choose which service to use
        this->service = serviceFwProxyPtr->serviceName(this->interface);

        bool interfaceWasDead = previousService.isEmpty();
        if (interfaceWasDead) {
            emit serviceAvailable(service);
        }

        bool serviceHasChanged = (previousService != this->service);
        if (serviceHasChanged) {
            setService(this->service);
            emit serviceChanged(this->service);
        }
    }
}

QString DuiServiceFwBaseIf::resolveServiceName(const QString &ifName, const QString &preferredService)
{
    duiDebug("DuiServiceFwBaseIf") << "DuiServiceFwBaseIf::resolveServiceName( ifName=" << ifName << ", preferredService=" << preferredService << " )";
    bool noPreferredSpecified = preferredService.isEmpty();
    if (noPreferredSpecified) {
        // ask the service name from service mapper
        if (serviceFwProxyPtr->connection().isConnected()) {
            duiDebug("DuiServiceFwBaseIf") << "no preferred service and am connected to dbus so asking servicemapper";
            service = serviceFwProxyPtr->serviceName(ifName);
        } else {
            duiDebug("DuiServiceFwBaseIf") << "no preferred service and am not connected to dbus so making I/F invalid";
            service.clear();
        }
    } else {
        duiDebug("DuiServiceFwBaseIf") << "preferredService specified, so returning it";
        service = preferredService;
    }

    return service;
}
