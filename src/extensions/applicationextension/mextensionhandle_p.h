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

#ifndef MEXTENSIONHANDLE_P_H
#define MEXTENSIONHANDLE_P_H

#include "mextensionhandle.h"
#include "private/mwidgetcontroller_p.h"
#include "mappletserver.h"

#include <QElapsedTimer>
#include <QTimer>
#include <QProcess>

class MAction;

#ifdef HAVE_CONTEXTSUBSCRIBER
class ContextProperty;
#endif

/*!
 * A private data class for MExtensionHandle.
 */
class MExtensionHandlePrivate : public QObject
{
    Q_DECLARE_PUBLIC(MExtensionHandle)
    Q_OBJECT

public:
    /*!
     * Constructor.
     */
    MExtensionHandlePrivate();

    /*!
     * Destructor.
     */
    virtual ~MExtensionHandlePrivate();

    //! The name for the communication server
    QString serverName;

    //! The arguments that were given to init()
    QStringList arguments;

    //! The name of the runner binary
    QString runnerBinary;

    //! The runner process
    QProcess process;

    //! The number of times this process has been restarted due to malfunction
    int restartCount;

    //! Time since the process was started, for checking how long the remote process could run before breaking
    QTime processStartTime;

    //! A server for IPC communication between the processes
    MAppletServer communicator;

    //! Timer to send alive message requests to runner at constant intervals
    QTimer aliveTimer;

    //! Timer to verify that communication between the processes is working
    QTimer communicationTimer;

    //! Timeout for remote process to respond to an alive request.
    uint aliveResponseTimeout;

    //! Timeout for the runner process to connect to the host process
    uint runnerConnectionTimeout;

    //! Stored geometry
    QRectF oldGeometry;

    //! Application level visibility
    bool applicationVisible;

    //! A list of remote actions
    QList<MAction *> remoteActions;

#ifdef HAVE_CONTEXTSUBSCRIBER
    //! Context property for getting the screen blanking status
    ContextProperty *screenBlankProperty;
#endif

    //! D-Bus service of the Package Manager
    static const QString PACKAGE_MANAGER_DBUS_SERVICE;

    //! D-Bus path of the Package Manager
    static const QString PACKAGE_MANAGER_DBUS_PATH;

    //! D-Bus interface of the Package Manager
    static const QString PACKAGE_MANAGER_DBUS_INTERFACE;

    /*!
     * Sends the necessary visibility change message when the application or
     * widget level visibility has changed
     */
    void visibilityChanged();

public slots:
    /*!
     * Slot for listening to operation complete signals from the Package Manager
     *
     * \param operation Name of the operation. Possible names are: Install, Uninstall and Upgrade
     * \param pkg Name of the package that operation is performed on
     * \param error Empty if operation was successful, DBus error name otherwise
     */
    void operationComplete(const QString &operation, const QString &pkg, const QString &error);

    /*!
      * Receives information about the progress of a Package Manager operation.
      *
      * \param operation Name of the operation. Possible names are: Install, Uninstall and Upgrade
      * \param pkg Name of the package that operation is performed on.
      * \param percentage Current progress percentage
      */
    void operationProgress(const QString &operation, const QString &pkg, int percentage);

protected:
    MExtensionHandle *q_ptr;

};

#endif
