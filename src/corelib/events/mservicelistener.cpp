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

#include <QDBusPendingCallWatcher>
#include <QDBusConnectionInterface>
#include <QPair>
#include <QMap>
#include <QString>
#include "mservicelistener.h"

inline void sconnect(const QObject *from, const char* fromSignal,
                     const QObject *to, const char* toSignal,
                     Qt::ConnectionType type = Qt::AutoConnection)
{
    if (!QObject::connect(from, fromSignal, to, toSignal, type))
        qFatal("    *****************\n"
               "Connect returned false, aborting, enable core dumping (ulimit -c unlimited), \n"
               "enable debug (qmake CONFIG+=debug), recompile, rerun and then use the\n"
               "core file with gdb's backtrace to see the location.\n"
               "    *****************\n");
}

MServiceListener::MServiceListener(QDBusConnection::BusType busType,
                                   const QString &busName, QObject *parent) :
    QDBusServiceWatcher(parent),
    servicePresent(Unknown),
    busType(busType),
    busName(busName),
    listeningStarted(false),
    connection(0)
{
    // Don't do anything, until the user initiates the listening by calling
    // startListening.
    init();
}

MServiceListener::MServiceListener(const QDBusConnection bus, const QString &busName, QObject *parent) :
    QDBusServiceWatcher(parent),
    servicePresent(Unknown), busName(busName), listeningStarted(false), connection(0)
{
    // we copy the connection so we can safely delete it in the destructor
    connection = new QDBusConnection(bus);
    init();
}

void MServiceListener::init()
{
    addWatchedService(busName);
    sconnect(this, SIGNAL(serviceRegistered(const QString&)),
             this, SLOT(setServicePresent()));
    sconnect(this, SIGNAL(serviceUnregistered(const QString&)),
             this, SLOT(setServiceGone()));
}

MServiceListener::~MServiceListener()
{
    delete connection;
    connection = 0;
}

void MServiceListener::startListening(bool nameHasOwnerCheck)
{
    if (listeningStarted) return;

    listeningStarted = true;
    // if we were initialized without a connection, we have to create our own
    if (connection == 0) {
        if (busType == QDBusConnection::SystemBus) {
            connection = new QDBusConnection(QDBusConnection::systemBus());
        } else if (busType == QDBusConnection::SessionBus) {
            connection = new QDBusConnection(QDBusConnection::sessionBus());
        } else {
            return;
        }
    }
    // set the connection + watched service name to the QDBusServiceWatcher
    // base object; now it will start to watch
    setConnection(*connection);

    // Check if the service is already there
    if (nameHasOwnerCheck) {
        if (connection->isConnected() == false) {
            setServiceGone();
            return;
        }
        QDBusPendingCall nameHasOwnerCall = connection->interface()->asyncCall("NameHasOwner", busName);

        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(nameHasOwnerCall, this);

        sconnect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)),
                 this, SLOT(onNameHasOwnerFinished(QDBusPendingCallWatcher *)));
    }
}

void MServiceListener::setServicePresent()
{
    if (servicePresent != Present) {
        servicePresent = Present;
        Q_EMIT nameAppeared();
    }
}

void MServiceListener::setServiceGone()
{
    if (servicePresent != NotPresent) {
        servicePresent = NotPresent;
        Q_EMIT nameDisappeared();
    }
}

void MServiceListener::onNameHasOwnerFinished(QDBusPendingCallWatcher* watcher)
{
    QDBusPendingReply<bool> reply = *watcher;

    if (reply.isError() == false && reply.argumentAt<0>() == true) {
        // The name has an owner
        setServicePresent();
    } else {
        // PropertyHandle's are waiting for any signal if they have initiated the
        // "is commander there" check. So notify also the opposite case.
        setServiceGone();
    }
    watcher->deleteLater();
}

MServiceListener::ServicePresence MServiceListener::isServicePresent() const
{
    return servicePresent;
}
