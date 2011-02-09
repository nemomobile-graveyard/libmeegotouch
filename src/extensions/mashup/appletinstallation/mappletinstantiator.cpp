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

#include <mdbusinterface.h>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>
#include <QFileInfo>
#include <QStringList>
#include "mappletinstantiator.h"
#include "mappletinstantiator_p.h"

const QString MAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_SERVICE = "com.nokia.package_manager";
const QString MAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_PATH = "/com/nokia/package_manager";
const char *  MAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_INTERFACE = "com.nokia.package_manager";
const char *  MAppletInstantiatorPrivate::APPLET_INSTANCE_MANAGER_DBUS_INTERFACE = "com.meego.core.MAppletInstanceManager";

MAppletInstantiatorPrivate::MAppletInstantiatorPrivate()
{
}

MAppletInstantiatorPrivate::~MAppletInstantiatorPrivate()
{
}

MAppletInstantiator::MAppletInstantiator() : d_ptr(new MAppletInstantiatorPrivate)
{
}

MAppletInstantiator::~MAppletInstantiator()
{
    delete d_ptr;
}

MAppletInstantiator *MAppletInstantiator::instance()
{
    static MAppletInstantiator appletInstantiatorInstance;
    return &appletInstantiatorInstance;
}

void MAppletInstantiator::instantiateAppletsInPackage(const QString &packageName, const QString &canvasDBusAddress)
{
    // Make sure the canvas D-Bus address is valid (the format is service/path)
    if (canvasDBusAddress.contains('/')) {
        Q_D(MAppletInstantiator);

        // Query the meta data of the package
        MDBusInterface interface(MAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_SERVICE, MAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_PATH,
                                MAppletInstantiatorPrivate::PACKAGE_MANAGER_DBUS_INTERFACE, QDBusConnection::systemBus());
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

void MAppletInstantiatorPrivate::receivePackageData(QDBusPendingCallWatcher *watcher)
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
            MDBusInterface interface(PACKAGE_MANAGER_DBUS_SERVICE, PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, QDBusConnection::systemBus());
            if (packageName.contains('/')) {
                // Install from a local file
                interface.asyncCall("InstallFile", packageName);
            } else {
                // Install from a remote file
                interface.asyncCall("Install", packageName);
            }
        } else {
            // An error occurred so don't start installation but inform the canvas anyway without metadata in order to display the error
            informCanvasAboutPackageInstallation(packageName, QMap<QString, QVariant>(), canvasDBusAddress);
        }
    }
}

void MAppletInstantiatorPrivate::informCanvasAboutPackageInstallation(const QString &packageName, const QMap<QString, QVariant> &metaData, const QString &canvasDBusAddress)
{
    // Split the D-Bus path of the target canvas to service and path from the first slash; the validity of the address has already been checked
    int pathSeparatorIndex = canvasDBusAddress.indexOf('/');
    QString service = canvasDBusAddress.left(pathSeparatorIndex);
    QString path = canvasDBusAddress.mid(pathSeparatorIndex);

    // Call the instantiateApplet method of the target mashup canvas
    MDBusInterface interface(service, path, APPLET_INSTANCE_MANAGER_DBUS_INTERFACE);
    interface.asyncCall("instantiateAppletFromPackage", packageName, metaData);
}

#include "moc_mappletinstantiator.cpp"
