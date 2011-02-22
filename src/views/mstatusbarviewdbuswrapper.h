/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSTATUSBARVIEWDBUSWRAPPER_H
#define MSTATUSBARVIEWDBUSWRAPPER_H

#include <QObject>

#ifdef HAVE_DBUS
#include <mdbusinterface.h>

class QDBusPendingCallWatcher;
class QDBusServiceWatcher;
#endif

//! \internal

/*
 Internal helper class for MStatusBarView. Makes unit testing of
 MStatusBarView logic easier since we are setting apart the DBus code (which
 is not feasible to mock).

*/
class MStatusBarViewDBusWrapper : public QObject
{
    Q_OBJECT
public:
    MStatusBarViewDBusWrapper(QObject *parent = 0);
    virtual ~MStatusBarViewDBusWrapper();

    bool isPixmapProviderServiceRegistered();

    // Will cause sharedPixmapHandleFromProviderReceived() to be
    // eventually emited
    void querySharedPixmapHandleFromProvider();

    bool havePendingSharedPixmapHandleReply();

    void openStatusIndicatorMenu();

Q_SIGNALS:
    void pixmapProviderServiceRegistered();
    void pixmapProviderServiceUnregistered();

    // \param ok true if reply was successful and false otherwise
    void sharedPixmapHandleFromProviderReceived(quint32 handle, bool ok);

#ifdef HAVE_DBUS
private Q_SLOTS:
    void onSharedPixmapHandleReceived(QDBusPendingCallWatcher *call);
    void onServiceUnregistered();
private:
    //! The name of the status indicator menu D-Bus service
    static const QString STATUS_INDICATOR_MENU_DBUS_SERVICE;

    //! The name of the status indicator menu D-Bus path
    static const QString STATUS_INDICATOR_MENU_DBUS_PATH;

    //! The name of the status indicator menu D-Bus interface
    static const char *STATUS_INDICATOR_MENU_DBUS_INTERFACE;

    //! DBus interface for the status indicator menu
    MDBusInterface statusIndicatorMenuInterface;

    QDBusServiceWatcher *dbusWatcher;
    QDBusPendingCallWatcher *pendingSharedPixmapCallWatcher;
#endif
};

//! \internal_end


#endif // MSTATUSBARVIEWDBUSWRAPPER_H
