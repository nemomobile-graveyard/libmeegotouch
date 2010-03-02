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

#include <QProcess>
#include <QStringList>
#include <QDBusConnection>
#include <DuiDebug>

#include "duiapplication.h"
#include "duiapplication_p.h"
#include "duiapplicationservice_p.h"

DuiApplicationServicePrivate::DuiApplicationServicePrivate(const QString &newServiceName):
    serviceName(newServiceName),
    registered(false),
    instanceCounter(0),
    q_ptr(NULL),
    dBusConnection(QDBusConnection::sessionBus()),
    duiApp(DuiApplication::instance())
{
}

DuiApplicationServicePrivate::~DuiApplicationServicePrivate()
{
}

void DuiApplicationServicePrivate::launchNewProcess(const QString &binaryName, const QStringList &arguments)
{
    QProcess *myProcess = new QProcess();

    duiDebug("DuiApplicationService") << "launch() starting" << binaryName;
    bool success = myProcess->startDetached(binaryName, arguments);
    if (!success) {
        duiDebug("DuiApplicationService") << "launch(): failed to start" << binaryName;
    }
}

bool DuiApplicationServicePrivate::registerService(const QString &serviceName)
{
    return dBusConnection.registerService(serviceName);
}

void DuiApplicationServicePrivate::registerObject(const QString &path, QObject *object)
{
    dBusConnection.registerObject(path, object);
}

void DuiApplicationServicePrivate::unregisterObject(const QString &path)
{
    dBusConnection.unregisterObject(path);
}

QString DuiApplicationServicePrivate::appName()
{
    QString appName = DuiApplication::instance()->appName();
    return appName;
}

void DuiApplicationServicePrivate::stdExit(int exitValue)
{
    DuiApplicationPrivate::stdExit(exitValue);
}

bool DuiApplicationServicePrivate::isPrestarted()
{
    return duiApp->isPrestarted();
}

bool DuiApplicationServicePrivate::activeWindowSet()
{
    return (duiApp->activeWindow() != 0);
}

void DuiApplicationServicePrivate::activateActiveWindow()
{
    duiApp->activeWindow()->activateWindow();
}

void DuiApplicationServicePrivate::raiseActiveWindow()
{
    duiApp->activeWindow()->raise();
}

bool DuiApplicationServicePrivate::thisAppRunWithDBus()
{
    bool thisAppRunWithDBus = !QProcess::systemEnvironment().filter("DBUS_STARTER_BUS_TYPE=").empty();
    return thisAppRunWithDBus;
}

QString DuiApplicationServicePrivate::binaryName()
{
    QString binaryName = DuiApplication::instance()->binaryName();
    return binaryName;
}

QStringList DuiApplicationServicePrivate::arguments()
{
    QStringList arguments = QCoreApplication::arguments();
    return arguments;
}

void DuiApplicationServicePrivate::closeAllWindows()
{
    duiApp->closeAllWindows();
}

bool DuiApplicationServicePrivate::prestartModeIsLazyShutdown()
{
    bool retVal = (DuiApplication::prestartMode() == Dui::LazyShutdown);

    return retVal;
}

void DuiApplicationServicePrivate::releasePrestart()
{
    DuiApplicationPrivate::releasePrestart();
}

void DuiApplicationServicePrivate::restorePrestart()
{
    DuiApplicationPrivate::restorePrestart();
}
