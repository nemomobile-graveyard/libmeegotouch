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

#include "mapplication.h"
#include "mapplicationservice.h"
#include "mapplicationservice_p.h"
#include "mwindow.h"
#include "mapplicationifproxy.h"
#include "mapplicationifadaptor.h"
#include "mapplication_p.h"
#include <MDebug>

MApplicationService::MApplicationService(const QString &newServiceName, QObject *parent):
    d_ptr(new MApplicationServicePrivate(newServiceName))
{
    Q_UNUSED(parent);
}

MApplicationService::~MApplicationService()
{
    delete d_ptr;
}

void MApplicationService::launch()
{
    Q_D(MApplicationService);
    mDebug("MApplicationService") << "launch()";

    if (d->isPrestarted()) {
        d->releasePrestart();
    } else {
        if (d->activeWindowSet()) {
            mDebug("MApplicationService") << "launch() raising window";
            d->activateActiveWindow();
            d->raiseActiveWindow();
        } else {
            mDebug("MApplicationService") << "launch() no window to raise";
        }
    }
}

void MApplicationService::launch(const QStringList& parameters)
{
    Q_UNUSED(parameters);
    mDebug("MApplicationService") << "launch(QStringList)";

    // Default implementation: ignore the parameters and just launch()
    launch();
}

void MApplicationService::launchAnotherWithQProcess()
{
    Q_D(MApplicationService);

    static bool firstCall = true;
    bool thisAppRunWithDBus = d->thisAppRunWithDBus();

    mDebug("MApplicationService") << "launch()";
    mDebug("MApplicationService") << "thisAppRunWithDBus = " << thisAppRunWithDBus;

    if (firstCall && thisAppRunWithDBus) {
        mDebug("MApplicationService") << "launch() : first dbus call so doing nothing";
    } else {
        QString binaryName = d->binaryName();
        QStringList arguments = d->arguments();

        arguments.removeFirst(); // remove program name

        d->launchNewProcess(binaryName, arguments);
    }

    firstCall = false;
}

void MApplicationService::close()
{
    Q_D(MApplicationService);

    // Return to prestarted state if in LazyShutdown mode
    if (d->prestartModeIsLazyShutdown()) {
        d->restorePrestart();
    }

    mDebug("MApplicationService") << "close()";
    d->closeAllWindows();
}

void MApplicationService::exit()
{
    Q_D(MApplicationService);

    // Do not really exit the app if it was prestarted (lazy shutdown)
    if (d->prestartModeIsLazyShutdown()) {
        d->restorePrestart();
    } else {
        d->stdExit(0);
    }

    mDebug("MApplicationService") << "exit()";
}

void MApplicationService::handleServiceRegistrationFailure()
{
    Q_D(MApplicationService);

    MApplication *mApp = MApplication::instance();

    MApplicationIfProxy mApplicationIfProxy(registeredName(), mApp);

    if (mApplicationIfProxy.connection().isConnected()) {
        mDebug("MApplicationService") << "Calling launch() in other application with service :" << registeredName();

        // when we are given command line arguments, call
        // launch( QStringList& ), otherwise call launch()
        QStringList args = QApplication::arguments();
        if ( args.count() <= 1 ) {
            mApplicationIfProxy.launch();
        } else {
            // remove basename
            args.removeFirst();

            mApplicationIfProxy.launch( args );
        }
        d->stdExit(0);
    } else {
        mDebug("MApplicationService") << "DBus not connected; not launching";
    }
}

void MApplicationService::incrementAndRegister()
{
    Q_D(MApplicationService);

    QString appName = d->appName();

    const int MaxNoInstances = 10;

    if (++d->instanceCounter == MaxNoInstances) {
        mDebug("MApplicationService") << "Reached maximum number instances of this application.";
        d->stdExit(0);
    }

    QString baseServiceName = "com.nokia." + appName;
    QString serviceName = baseServiceName + QString::number(d->instanceCounter);

    // register dbus service
    new MApplicationIfAdaptor(this);

    bool anotherAppRunning = !d->registerService(serviceName);
    while (anotherAppRunning) {
        mDebug("MApplicationService") << "registering failed";

        if (++d->instanceCounter == MaxNoInstances) {
            mDebug("MApplicationService") << "Reached maximum number instances of this application.";
            d->stdExit(0);
        }

        serviceName = baseServiceName + QString::number(d->instanceCounter);

        mDebug("MApplicationService") << "attempting to register service :" << serviceName;
        anotherAppRunning = !d->registerService(serviceName);
    }

    d->unregisterObject("/org/maemo/m");
    d->registerObject("/org/maemo/m", this);

    setServiceName(serviceName);
}

QString MApplicationService::registeredName()
{
    Q_D(MApplicationService);

    return d->serviceName;
}

bool MApplicationService::isRegistered()
{
    Q_D(MApplicationService);

    return !d->serviceName.isEmpty();
}

bool MApplicationService::registerService()
{
    Q_D(MApplicationService);

    bool success = false;

    if (isRegistered()) {
        new MApplicationIfAdaptor(this);

        bool anotherAppRunning = !d->registerService(d->serviceName);
        if (anotherAppRunning) {
            mDebug("MApplicationService") << "registerService() registering failed";
            handleServiceRegistrationFailure();
            success = false;
        } else {
            d->unregisterObject("/org/maemo/m");
            d->registerObject("/org/maemo/m", this);
            success = true;
        }
    }

    return success;
}

void MApplicationService::setServiceName(const QString &newServiceName)
{
    Q_D(MApplicationService);

    d->serviceName = newServiceName;
}
