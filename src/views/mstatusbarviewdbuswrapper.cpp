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

#include "mstatusbarviewdbuswrapper.h"

#ifdef HAVE_DBUS

#include <mdbusinterface.h>
#include <QDBusServiceWatcher>
#include <QDBusConnectionInterface>
#include <MDebug>

namespace {
    const QString PIXMAP_PROVIDER_DBUS_SERVICE = "com.meego.core.MStatusBar";
    const QString PIXMAP_PROVIDER_DBUS_PATH = "/statusbar";
    const char *  PIXMAP_PROVIDER_DBUS_INTERFACE = "com.meego.core.MStatusBar";
    const QString PIXMAP_PROVIDER_DBUS_SHAREDPIXMAP_CALL = "sharedPixmapHandle";
}

const QString MStatusBarViewDBusWrapper::STATUS_INDICATOR_MENU_DBUS_SERVICE = "com.meego.core.MStatusIndicatorMenu";
const QString MStatusBarViewDBusWrapper::STATUS_INDICATOR_MENU_DBUS_PATH = "/statusindicatormenu";
const char *  MStatusBarViewDBusWrapper::STATUS_INDICATOR_MENU_DBUS_INTERFACE = "com.meego.core.MStatusIndicatorMenu";

MStatusBarViewDBusWrapper::MStatusBarViewDBusWrapper(QObject *parent)
    : QObject(parent)
    , statusIndicatorMenuInterface(STATUS_INDICATOR_MENU_DBUS_SERVICE,
                                   STATUS_INDICATOR_MENU_DBUS_PATH,
                                   STATUS_INDICATOR_MENU_DBUS_INTERFACE,
                                   QDBusConnection::sessionBus())
    , dbusWatcher(0)
    , pendingSharedPixmapCallWatcher(0)
{
    dbusWatcher = new QDBusServiceWatcher(
        PIXMAP_PROVIDER_DBUS_SERVICE,
        QDBusConnection::sessionBus(),
        QDBusServiceWatcher::WatchForRegistration|QDBusServiceWatcher::WatchForUnregistration,
        this);

    bool ok = true;

    ok &= connect(dbusWatcher, SIGNAL(serviceRegistered(QString)),
                  this, SIGNAL(pixmapProviderServiceRegistered()));


    ok &= connect(dbusWatcher, SIGNAL(serviceUnregistered(QString)),
                  this, SLOT(onServiceUnregistered()));

    Q_ASSERT(ok);
    if (!ok) {
        mWarning("MStatusBarViewDBusWrapper") << "Signal connection failed!";
    }
}

MStatusBarViewDBusWrapper::~MStatusBarViewDBusWrapper()
{
}

bool MStatusBarViewDBusWrapper::isPixmapProviderServiceRegistered()
{
    return QDBusConnection::sessionBus().interface()->isServiceRegistered(PIXMAP_PROVIDER_DBUS_SERVICE);
}

void MStatusBarViewDBusWrapper::querySharedPixmapHandleFromProvider()
{
    Q_ASSERT(isPixmapProviderServiceRegistered());

    MDBusInterface interface(PIXMAP_PROVIDER_DBUS_SERVICE,
                             PIXMAP_PROVIDER_DBUS_PATH,
                             PIXMAP_PROVIDER_DBUS_INTERFACE,
                             QDBusConnection::sessionBus());

    QDBusPendingCall asyncCall =  interface.asyncCall(PIXMAP_PROVIDER_DBUS_SHAREDPIXMAP_CALL);
    pendingSharedPixmapCallWatcher = new QDBusPendingCallWatcher(asyncCall, this);

    connect(pendingSharedPixmapCallWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(onSharedPixmapHandleReceived(QDBusPendingCallWatcher*)));
}

bool MStatusBarViewDBusWrapper::havePendingSharedPixmapHandleReply()
{
    return pendingSharedPixmapCallWatcher != 0;
}

void MStatusBarViewDBusWrapper::openStatusIndicatorMenu()
{
    statusIndicatorMenuInterface.call(QDBus::NoBlock, "open");
}

void MStatusBarViewDBusWrapper::onSharedPixmapHandleReceived(QDBusPendingCallWatcher *call)
{
    bool ok = true;

    QDBusPendingReply<quint32> reply = *call;
    if (reply.isError()) {
        mWarning("MStatusBarViewDBusWrapper")<< reply.error().message();
        ok = false;
    }
    quint32 sharedPixmapHandle = reply;

    Q_ASSERT(pendingSharedPixmapCallWatcher == call);
    pendingSharedPixmapCallWatcher = 0;
    call->deleteLater();

    emit sharedPixmapHandleFromProviderReceived(sharedPixmapHandle, ok);
}

void MStatusBarViewDBusWrapper::onServiceUnregistered()
{
    if (pendingSharedPixmapCallWatcher) {
        delete pendingSharedPixmapCallWatcher;
        pendingSharedPixmapCallWatcher = 0;
    }

    emit pixmapProviderServiceUnregistered();
}
#else // HAVE_DBUS -  We don't have DBus. Let's just have an empty implementation.

MStatusBarViewDBusWrapper::MStatusBarViewDBusWrapper(QObject *parent)
    : QObject(parent)
{
}

MStatusBarViewDBusWrapper::~MStatusBarViewDBusWrapper()
{
}

bool MStatusBarViewDBusWrapper::isPixmapProviderServiceRegistered()
{
    return false;
}

void MStatusBarViewDBusWrapper::querySharedPixmapHandleFromProvider()
{
}

bool MStatusBarViewDBusWrapper::havePendingSharedPixmapHandleReply()
{
    return false;
}

void MStatusBarViewDBusWrapper::openStatusIndicatorMenu()
{
}

#endif // HAVE_DBUS
