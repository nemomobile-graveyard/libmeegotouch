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
#include "duiservicefwbaseif_p.h"

#include <QDBusAbstractInterface>
#include <QDBusConnection>
#include <DuiDebug>

DuiServiceFwBaseIfPrivate::DuiServiceFwBaseIfPrivate(const QString &iface) :
    interfaceProxy(0),
    serviceFwProxyPtr(new DuiServiceFwProxy(
                          "com.nokia.DuiServiceFw",
                          "/",
                          QDBusConnection::sessionBus()
                      )
                     ),
    interface(iface)
{
}

DuiServiceFwBaseIfPrivate::~DuiServiceFwBaseIfPrivate()
{
    if (interfaceProxy)
        delete interfaceProxy;

    if (serviceFwProxyPtr)
        delete serviceFwProxyPtr;
}


DuiServiceFwBaseIf::DuiServiceFwBaseIf(const QString &iface, QObject *parent) :
    QObject(parent),
    d_ptr(new DuiServiceFwBaseIfPrivate(iface))
{
    Q_D(const DuiServiceFwBaseIf);
    connect(d->serviceFwProxyPtr, SIGNAL(serviceAvailable(QString, QString)),
            this, SLOT(handleServiceAvailable(QString, QString)));

    connect(d->serviceFwProxyPtr, SIGNAL(serviceUnavailable(QString)),
            this, SLOT(handleServiceUnavailable(QString)));
}

DuiServiceFwBaseIf::~DuiServiceFwBaseIf()
{
    delete d_ptr;
}

bool DuiServiceFwBaseIf::isValid() const
{
    Q_D(const DuiServiceFwBaseIf);
    return (d->interfaceProxy != 0);
}

QDBusAbstractInterface *DuiServiceFwBaseIf::interfaceProxy() const
{
    Q_D(const DuiServiceFwBaseIf);
    return d->interfaceProxy;
}

void DuiServiceFwBaseIf::setInterfaceProxy(QDBusAbstractInterface *newInterfaceProxy)
{
    Q_D(DuiServiceFwBaseIf);
    if (d->interfaceProxy) {
        delete d->interfaceProxy;
    }
    d->interfaceProxy=newInterfaceProxy;
}

QStringList DuiServiceFwBaseIf::serviceNames(const QString &interface) const
{
    Q_D(const DuiServiceFwBaseIf);
    return d->serviceFwProxyPtr->serviceNames(interface);
}

QString DuiServiceFwBaseIf::serviceName() const
{
    Q_D(const DuiServiceFwBaseIf);
    return d->service;
}

void DuiServiceFwBaseIf::setServiceName(const QString &service)
{
    Q_D(DuiServiceFwBaseIf);
    d->service = service;
}

QString DuiServiceFwBaseIf::interfaceName() const
{
    Q_D(const DuiServiceFwBaseIf);
    return d->interface;
}

void DuiServiceFwBaseIf::setInterfaceName(const QString &ifName)
{
    Q_D(DuiServiceFwBaseIf);
    d->interface = ifName;
}

DuiServiceFwProxy *DuiServiceFwBaseIf::serviceFwProxy()
{
    Q_D(const DuiServiceFwBaseIf);
    return d->serviceFwProxyPtr;
}

void DuiServiceFwBaseIf::setServiceFwProxy(DuiServiceFwProxy *newServiceFwProxy)
{
    Q_D(DuiServiceFwBaseIf);
    if (d->serviceFwProxyPtr)
        delete d->serviceFwProxyPtr;
    d->serviceFwProxyPtr=newServiceFwProxy;
}

void DuiServiceFwBaseIf::handleServiceUnavailable(const QString &service)
{
    Q_D(DuiServiceFwBaseIf);
    bool noCurrentService      = d->service.isEmpty();
    bool currentServiceHasGone = (service == d->service);

    if (noCurrentService || currentServiceHasGone) {
        // get new service for this interface
        d->service = d->serviceFwProxyPtr->serviceName(d->interface);

        bool noMoreServicesForThisInterface = d->service.isEmpty();
        if (noMoreServicesForThisInterface) {
            if (!noCurrentService)
                emit serviceUnavailable(service);
        } else {
            setService(d->service);
            emit serviceChanged(d->service);
        }
    }
}

void DuiServiceFwBaseIf::handleServiceAvailable(const QString &service, const QString &interface)
{
    Q_D(DuiServiceFwBaseIf);
    bool newServiceIsForThisInterface = (interface == d->interface);
    if (newServiceIsForThisInterface) {
        QString previousService = d->service;

        // let service mappper choose which service to use
        d->service = d->serviceFwProxyPtr->serviceName(d->interface);

        bool interfaceWasDead = previousService.isEmpty();
        if (interfaceWasDead) {
            emit serviceAvailable(service);
        }

        bool serviceHasChanged = (previousService != d->service);
        if (serviceHasChanged) {
            setService(d->service);
            emit serviceChanged(d->service);
        }
    }
}

QString DuiServiceFwBaseIf::resolveServiceName(const QString &ifName, const QString &preferredService)
{
    Q_D(DuiServiceFwBaseIf);
    duiDebug("DuiServiceFwBaseIf") << "DuiServiceFwBaseIf::resolveServiceName( ifName=" << ifName << ", preferredService=" << preferredService << " )";
    bool noPreferredSpecified = preferredService.isEmpty();
    if (noPreferredSpecified) {
        // ask the service name from service mapper
        if (d->serviceFwProxyPtr->connection().isConnected()) {
            duiDebug("DuiServiceFwBaseIf") << "no preferred service and am connected to dbus so asking servicemapper";
            d->service = d->serviceFwProxyPtr->serviceName(ifName);
        } else {
            duiDebug("DuiServiceFwBaseIf") << "no preferred service and am not connected to dbus so making I/F invalid";
            d->service.clear();
        }
    } else {
        duiDebug("DuiServiceFwBaseIf") << "preferredService specified, so returning it";
        d->service = preferredService;
    }

    return d->service;
}
