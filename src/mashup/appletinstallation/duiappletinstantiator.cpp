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

#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>
#include <QFileInfo>
#include <QStringList>
#include "duiappletinstantiator.h"
#include "duiappletinstantiator_p.h"

const QString DuiAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_SERVICE = "com.nokia.package_manager";
const QString DuiAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_PATH = "/com/nokia/package_manager";
const QString DuiAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_INTERFACE = "com.nokia.package_manager";
const QString DuiAppletInstantiatorPrivate::APPLET_INSTANCE_MANAGER_DBUS_INTERFACE = "org.maemo.dui.DuiAppletInstanceManager";

DuiAppletInstantiatorPrivate::DuiAppletInstantiatorPrivate()
{
}

DuiAppletInstantiatorPrivate::~DuiAppletInstantiatorPrivate()
{
}

DuiAppletInstantiator::DuiAppletInstantiator() : d_ptr(new DuiAppletInstantiatorPrivate)
{
}

DuiAppletInstantiator::~DuiAppletInstantiator()
{
    delete d_ptr;
}

DuiAppletInstantiator *DuiAppletInstantiator::instance()
{
    static DuiAppletInstantiator appletInstantiatorInstance;
    return &appletInstantiatorInstance;
}

void DuiAppletInstantiator::instantiateAppletsInPackage(const QString &packageName, const QString &canvasDBusAddress)
{
    // Make sure the canvas D-Bus address is valid (the format is service/path)
    if (canvasDBusAddress.contains('/')) {
        Q_D(DuiAppletInstantiator);

        // Query the meta data of the package
        QDBusInterface interface(DuiAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_SERVICE, DuiAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_PATH, DuiAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_INTERFACE, QDBusConnection::systemBus());
        QDBusPendingCallWatcher *watcher;
        if (packageName.contains('/')) {
            // Get metadata for a local file
            QDBusPendingCall call = interface.asyncCall("FetchPackageDataFile", packageName);
            watcher = new QDBusPendingCallWatcher(call, this);
        } else {
            // Get metadata for a remote file
            QString packageBaseName = QFileInfo(packageName).baseName().split('_').at(0);
            QDBusPendingCall call = interface.asyncCall("FetchPackageData", packageBaseName, QString());
            watcher = new QDBusPendingCallWatcher(call, this);
        }
        watcher->setProperty("packageName", packageName);
        watcher->setProperty("canvasDBusAddress", canvasDBusAddress);
        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)), d, SLOT(receivePackageData(QDBusPendingCallWatcher *)));
    }
}

void DuiAppletInstantiatorPrivate::receivePackageData(QDBusPendingCallWatcher *watcher)
{
    if (watcher != NULL) {
        // Get the package name
        QString packageName = watcher->property("packageName").toString();
        QString canvasDBusAddress = watcher->property("canvasDBusAddress").toString();

        QDBusPendingReply<QMap<QString, QVariant> > reply = *watcher;
        if (!reply.isError()) {
            // Get the meta data from the reply and inform the canvas about the installation
            informCanvasAboutPackageInstallation(packageName, reply.argumentAt<0>(), canvasDBusAddress);

            // Start the installation of the package
            QDBusInterface interface(PACKAGE_MANAGER_DBUS_SERVICE, PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, QDBusConnection::systemBus());
            if (packageName.contains('/')) {
                // Install from a local file
                interface.call(QDBus::NoBlock, "InstallFile", packageName);
            } else {
                // Install from a remote file
                interface.call(QDBus::NoBlock, "Install", packageName);
            }
        } else {
            // An error occurred so don't start installation but inform the canvas anyway without metadata in order to display the error
            informCanvasAboutPackageInstallation(packageName, QMap<QString, QVariant>(), canvasDBusAddress);
        }
    }
}

void DuiAppletInstantiatorPrivate::informCanvasAboutPackageInstallation(const QString &packageName, const QMap<QString, QVariant> &metaData, const QString &canvasDBusAddress)
{
    // Split the D-Bus path of the target canvas to service and path from the first slash; the validity of the address has already been checked
    int pathSeparatorIndex = canvasDBusAddress.indexOf('/');
    QString service = canvasDBusAddress.left(pathSeparatorIndex);
    QString path = canvasDBusAddress.mid(pathSeparatorIndex);

    // Call the instantiateApplet method of the target mashup canvas
    QDBusInterface interface(service, path, APPLET_INSTANCE_MANAGER_DBUS_INTERFACE);
    interface.call(QDBus::NoBlock, "instantiateAppletFromPackage", packageName, metaData);
}

#include "moc_duiappletinstantiator.cpp"
