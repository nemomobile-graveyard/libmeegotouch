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

#include <QTimer>
#include <QCoreApplication>

#include <QDBusConnection>
#include <QDBusConnectionInterface>

#include "mdbusservicewaiter_p.h"


MDBusServiceWaiter::MDBusServiceWaiter(const QString &name, QObject *parent) :
    QObject(parent), success(false), finished(false), serviceName(name), m_timeout(-1), isInvalid(false)
{
    QDBusConnectionInterface *iface = QDBusConnection::sessionBus().interface();

    if (!QDBusConnection::sessionBus().isConnected() || !iface->isValid()) {
        isInvalid = true;
    } else {
        QObject::connect(iface, SIGNAL(serviceOwnerChanged(QString, QString, QString)),
                         this, SLOT(serviceOwnerChanged(QString, QString, QString)));
    }
}


bool MDBusServiceWaiter::waitForRegistration()
{
    if (isInvalid)
        return false;

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(serviceName))
        return true;

    if (timeout() > 0)
        QTimer::singleShot(timeout(), this, SLOT(serviceTimedOut()));

    while (!finished)
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);

    return success;
}


int MDBusServiceWaiter::timeout() const
{
    return m_timeout;
}

void MDBusServiceWaiter::setTimeout(int newTimeout)
{
    m_timeout = newTimeout;
}


void MDBusServiceWaiter::serviceTimedOut()
{
    finished = true;
}

void MDBusServiceWaiter::serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner)
{
    Q_UNUSED(oldOwner)

    if (name == serviceName && !newOwner.isEmpty()) {
        finished = true;
        success = true;
    }
}

