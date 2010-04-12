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

#include "mservicefwbaseif.h"
#include "mservicefwbaseif_p.h"

#include <QDBusAbstractInterface>
#include <QDBusConnection>
#include <MDebug>

MServiceFwBaseIfPrivate::MServiceFwBaseIfPrivate(const QString &iface) :
    interfaceProxy(0),
    serviceFwProxyPtr(new MServiceFwProxy(
                          "com.nokia.MServiceFw",
                          "/",
                          QDBusConnection::sessionBus()
                      )
                     ),
    interface(iface)
{
}

MServiceFwBaseIfPrivate::~MServiceFwBaseIfPrivate()
{
    if (interfaceProxy)
        delete interfaceProxy;

    if (serviceFwProxyPtr)
        delete serviceFwProxyPtr;
}


MServiceFwBaseIf::MServiceFwBaseIf(const QString &iface, QObject *parent) :
    QObject(parent),
    d_ptr(new MServiceFwBaseIfPrivate(iface))
{
    Q_D(const MServiceFwBaseIf);
    connect(d->serviceFwProxyPtr, SIGNAL(serviceAvailable(QString, QString)),
            this, SLOT(handleServiceAvailable(QString, QString)));

    connect(d->serviceFwProxyPtr, SIGNAL(serviceUnavailable(QString)),
            this, SLOT(handleServiceUnavailable(QString)));
}

MServiceFwBaseIf::~MServiceFwBaseIf()
{
    delete d_ptr;
}

bool MServiceFwBaseIf::isValid() const
{
    Q_D(const MServiceFwBaseIf);
    return (d->interfaceProxy != 0);
}

QDBusAbstractInterface *MServiceFwBaseIf::interfaceProxy() const
{
    Q_D(const MServiceFwBaseIf);
    return d->interfaceProxy;
}

void MServiceFwBaseIf::setInterfaceProxy(QDBusAbstractInterface *newInterfaceProxy)
{
    Q_D(MServiceFwBaseIf);
    if (d->interfaceProxy) {
        delete d->interfaceProxy;
    }
    d->interfaceProxy=newInterfaceProxy;
}

QStringList MServiceFwBaseIf::serviceNames(const QString &interface) const
{
    Q_D(const MServiceFwBaseIf);
    return d->serviceFwProxyPtr->serviceNames(interface);
}

QString MServiceFwBaseIf::serviceName() const
{
    Q_D(const MServiceFwBaseIf);
    return d->service;
}

void MServiceFwBaseIf::setServiceName(const QString &service)
{
    Q_D(MServiceFwBaseIf);
    d->service = service;
}

QString MServiceFwBaseIf::interfaceName() const
{
    Q_D(const MServiceFwBaseIf);
    return d->interface;
}

void MServiceFwBaseIf::setInterfaceName(const QString &ifName)
{
    Q_D(MServiceFwBaseIf);
    d->interface = ifName;
}

MServiceFwProxy *MServiceFwBaseIf::serviceFwProxy()
{
    Q_D(const MServiceFwBaseIf);
    return d->serviceFwProxyPtr;
}

void MServiceFwBaseIf::setServiceFwProxy(MServiceFwProxy *newServiceFwProxy)
{
    Q_D(MServiceFwBaseIf);
    if (d->serviceFwProxyPtr)
        delete d->serviceFwProxyPtr;
    d->serviceFwProxyPtr=newServiceFwProxy;
}

void MServiceFwBaseIf::handleServiceUnavailable(const QString &service)
{
    Q_D(MServiceFwBaseIf);
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

void MServiceFwBaseIf::handleServiceAvailable(const QString &service, const QString &interface)
{
    Q_D(MServiceFwBaseIf);
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

QString MServiceFwBaseIf::resolveServiceName(const QString &ifName, const QString &preferredService)
{
    Q_D(MServiceFwBaseIf);
    mDebug("MServiceFwBaseIf") << "MServiceFwBaseIf::resolveServiceName( ifName=" << ifName << ", preferredService=" << preferredService << " )";
    bool noPreferredSpecified = preferredService.isEmpty();
    if (noPreferredSpecified) {
        // ask the service name from service mapper
        if (d->serviceFwProxyPtr->connection().isConnected()) {
            mDebug("MServiceFwBaseIf") << "no preferred service and am connected to dbus so asking servicemapper";
            d->service = d->serviceFwProxyPtr->serviceName(ifName);
        } else {
            mDebug("MServiceFwBaseIf") << "no preferred service and am not connected to dbus so making I/F invalid";
            d->service.clear();
        }
    } else {
        mDebug("MServiceFwBaseIf") << "preferredService specified, so returning it";
        d->service = preferredService;
    }

    return d->service;
}
