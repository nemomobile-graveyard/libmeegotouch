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

#include "duiapplication.h"
#include "duiapplicationservice.h"
#include "duiapplicationservice_p.h"
#include "duiwindow.h"
#include "duiapplicationifproxy.h"
#include "duiapplicationifadaptor.h"
#include "duiapplication_p.h"
#include <DuiDebug>

DuiApplicationService::DuiApplicationService(const QString &newServiceName, QObject *parent):
    d_ptr(new DuiApplicationServicePrivate(newServiceName))
{
    Q_UNUSED(parent);
}

DuiApplicationService::~DuiApplicationService()
{
    delete d_ptr;
}

void DuiApplicationService::launch()
{
    Q_D(DuiApplicationService);
    duiDebug("DuiApplicationService") << "launch()";

    if (d->isPrestarted()) {
        d->releasePrestart();
    } else {
        if (d->activeWindowSet()) {
            duiDebug("DuiApplicationService") << "launch() raising window";
            d->activateActiveWindow();
            d->raiseActiveWindow();
        } else {
            duiDebug("DuiApplicationService") << "launch() no window to raise";
        }
    }
}

void DuiApplicationService::launchAnotherWithQProcess()
{
    Q_D(DuiApplicationService);

    static bool firstCall = true;
    bool thisAppRunWithDBus = d->thisAppRunWithDBus();

    duiDebug("DuiApplicationService") << "launch()";
    duiDebug("DuiApplicationService") << "thisAppRunWithDBus = " << thisAppRunWithDBus;

    if (firstCall && thisAppRunWithDBus) {
        duiDebug("DuiApplicationService") << "launch() : first dbus call so doing nothing";
    } else {
        QString binaryName = d->binaryName();
        QStringList arguments = d->arguments();

        arguments.removeFirst(); // remove program name

        d->launchNewProcess(binaryName, arguments);
    }

    firstCall = false;
}

void DuiApplicationService::close()
{
    Q_D(DuiApplicationService);

    // Return to prestarted state if in LazyShutdown mode
    if (d->prestartModeIsLazyShutdown()) {
        d->restorePrestart();
    }

    duiDebug("DuiApplicationService") << "close()";
    d->closeAllWindows();
}

void DuiApplicationService::exit()
{
    Q_D(DuiApplicationService);

    // Do not really exit the app if it was prestarted (lazy shutdown)
    if (d->prestartModeIsLazyShutdown()) {
        d->restorePrestart();
    } else {
        d->stdExit(0);
    }

    duiDebug("DuiApplicationService") << "exit()";
}

void DuiApplicationService::handleServiceRegistrationFailure()
{
    Q_D(DuiApplicationService);

    DuiApplication *duiApp = DuiApplication::instance();

    DuiApplicationIfProxy duiApplicationIfProxy(registeredName(), duiApp);

    if (duiApplicationIfProxy.connection().isConnected()) {
        duiDebug("DuiApplicationService") << "Calling launch() in other application with service :" << registeredName();
        duiApplicationIfProxy.launch();
        d->stdExit(0);
    } else {
        duiDebug("DuiApplicationService") << "DBus not connected; not launching";
    }
}

void DuiApplicationService::incrementAndRegister()
{
    Q_D(DuiApplicationService);

    QString appName = d->appName();

    const int MaxNoInstances = 10;

    if (++d->instanceCounter == MaxNoInstances) {
        duiDebug("DuiApplicationService") << "Reached maximum number instances of this application.";
        d->stdExit(0);
    }

    QString baseServiceName = "com.nokia." + appName;
    QString serviceName = baseServiceName + QString::number(d->instanceCounter);

    // register dbus service
    new DuiApplicationIfAdaptor(this);

    bool anotherAppRunning = !d->registerService(serviceName);
    while (anotherAppRunning) {
        duiDebug("DuiApplicationService") << "registering failed";

        if (++d->instanceCounter == MaxNoInstances) {
            duiDebug("DuiApplicationService") << "Reached maximum number instances of this application.";
            d->stdExit(0);
        }

        serviceName = baseServiceName + QString::number(d->instanceCounter);

        duiDebug("DuiApplicationService") << "attempting to register service :" << serviceName;
        anotherAppRunning = !d->registerService(serviceName);
    }

    d->unregisterObject("/org/maemo/dui");
    d->registerObject("/org/maemo/dui", this);

    setServiceName(serviceName);
}

QString DuiApplicationService::registeredName()
{
    Q_D(DuiApplicationService);

    return d->serviceName;
}

bool DuiApplicationService::isRegistered()
{
    Q_D(DuiApplicationService);

    return !d->serviceName.isEmpty();
}

bool DuiApplicationService::registerService()
{
    Q_D(DuiApplicationService);

    bool success = false;

    if (isRegistered()) {
        new DuiApplicationIfAdaptor(this);

        bool anotherAppRunning = !d->registerService(d->serviceName);
        if (anotherAppRunning) {
            duiDebug("DuiApplicationService") << "registerService() registering failed";
            handleServiceRegistrationFailure();
            success = false;
        } else {
            duiDebug("DuiApplicationService") << "registerService() registering successful";
            d->unregisterObject("/org/maemo/dui");
            d->registerObject("/org/maemo/dui", this);
            success = true;
        }
    }

    return success;
}

void DuiApplicationService::setServiceName(const QString &newServiceName)
{
    Q_D(DuiApplicationService);

    d->serviceName = newServiceName;
}
