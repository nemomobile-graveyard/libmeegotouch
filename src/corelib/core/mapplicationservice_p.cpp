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

#include <QProcess>
#include <QStringList>
#include <QDBusConnection>
#include <MDebug>

#include "mapplication.h"
#include "mapplication_p.h"
#include "mapplicationservice_p.h"

MApplicationServicePrivate::MApplicationServicePrivate(const QString &newServiceName):
    serviceName(newServiceName),
    registered(false),
    instanceCounter(0),
    q_ptr(NULL),
    dBusConnection(QDBusConnection::sessionBus()),
    mApp(MApplication::instance())
{
}

MApplicationServicePrivate::~MApplicationServicePrivate()
{
}

void MApplicationServicePrivate::launchNewProcess(const QString &binaryName, const QStringList &arguments)
{
    QProcess *myProcess = new QProcess();

    mDebug("MApplicationService") << "launch() starting" << binaryName;
    bool success = myProcess->startDetached(binaryName, arguments);
    if (!success) {
        mDebug("MApplicationService") << "launch(): failed to start" << binaryName;
    }
}

bool MApplicationServicePrivate::registerService(const QString &serviceName)
{
    return dBusConnection.registerService(serviceName);
}

void MApplicationServicePrivate::registerObject(const QString &path, QObject *object)
{
    dBusConnection.registerObject(path, object);
}

void MApplicationServicePrivate::unregisterObject(const QString &path)
{
    dBusConnection.unregisterObject(path);
}

QString MApplicationServicePrivate::appName()
{
    QString appName = MApplication::instance()->appName();
    return appName;
}

void MApplicationServicePrivate::stdExit(int exitValue)
{
    MApplicationPrivate::stdExit(exitValue);
}

bool MApplicationServicePrivate::isPrestarted()
{
    return mApp->isPrestarted();
}

bool MApplicationServicePrivate::activeWindowSet()
{
    return (mApp->activeWindow() != 0);
}

void MApplicationServicePrivate::activateActiveWindow()
{
    MWindow *win = MApplication::instance()->activeWindow();
    if ( win ) {
        win->activateWindow();
    }
}

void MApplicationServicePrivate::raiseActiveWindow()
{
    MWindow *win = MApplication::instance()->activeWindow();
    if ( win ) {
        win->show();
    }
}

bool MApplicationServicePrivate::thisAppRunWithDBus()
{
    bool thisAppRunWithDBus = !QProcess::systemEnvironment().filter("DBUS_STARTER_BUS_TYPE=").empty();
    return thisAppRunWithDBus;
}

QString MApplicationServicePrivate::binaryName()
{
    QString binaryName = MApplication::instance()->binaryName();
    return binaryName;
}

QStringList MApplicationServicePrivate::arguments()
{
    QStringList arguments = QCoreApplication::arguments();
    return arguments;
}

void MApplicationServicePrivate::closeAllWindows()
{
    mApp->closeAllWindows();
}

bool MApplicationServicePrivate::prestartModeIsLazyShutdown()
{
    return MApplication::prestartMode() == M::LazyShutdown ||
            MApplication::prestartMode() == M::LazyShutdownMultiWindow;
}

void MApplicationServicePrivate::releasePrestart()
{
    MApplication::setPrestarted(false);
}

void MApplicationServicePrivate::restorePrestart()
{
    MApplication::setPrestarted(true);
}
