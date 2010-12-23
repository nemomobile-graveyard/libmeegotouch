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

#ifndef MAPPLETINSTANTIATOR_P_H_
#define MAPPLETINSTANTIATOR_P_H_

#include <QObject>
#include <QHash>

class QDBusPendingCallWatcher;

/*!
 * Private class for MAppletInstantiator.
 */
class MAppletInstantiatorPrivate : public QObject
{
    Q_OBJECT

public:
    /*!
     * Creates a new private class for MAppletInstantiator.
     */
    MAppletInstantiatorPrivate();

    /*!
     * Destroys the MAppletInstantiatorPrivate.
     */
    virtual ~MAppletInstantiatorPrivate();

    //! D-Bus service of the Package Manager
    static const QString PACKAGE_MANAGER_DBUS_SERVICE;

    //! D-Bus path of the Package Manager
    static const QString PACKAGE_MANAGER_DBUS_PATH;

    //! D-Bus interface of the Package Manager
    static const char *PACKAGE_MANAGER_DBUS_INTERFACE;

    //! Name of the MAppletInstanceManager D-Bus Interface
    static const char *APPLET_INSTANCE_MANAGER_DBUS_INTERFACE;

    /*!
     * Informs the canvas about the installation of a package.
     *
     * \param packageName the name of the package to be installed
     * \param metaData relevant meta data for the package
     * \param canvasDBusAddress the D-Bus address of the canvas to instantiate to in service/path format
     */
    void informCanvasAboutPackageInstallation(const QString &packageName, const QMap<QString, QVariant> &metaData, const QString &canvasDBusAddress);

public slots:
    /*!
     * Slot for receiving package data from the Package Manager
     *
     * \param watcher the QDBusPendingCallWatcher for the D-Bus call
     */
    void receivePackageData(QDBusPendingCallWatcher *watcher);
};

#endif /* MAPPLETINSTANTIATOR_P_H_ */
