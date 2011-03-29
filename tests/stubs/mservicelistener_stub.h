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

#ifndef MSERVICELISTENER_STUB
#define MSERVICELISTENER_STUB

#include "mservicelistener.h"
#include <stubmap.h>
#include <QDebug>

// == Global & default stub map instance ================================================

StubMap<QString, MServiceListener> gDefaultMServiceListenerStubMap;
StubMap<QString, MServiceListener> *gMServiceListenerStubMap = &gDefaultMServiceListenerStubMap;

// == MServiceListener ==================================================================

MServiceListener::MServiceListener(QDBusConnection::BusType, const QString &busName, QObject *parent)
    : QDBusServiceWatcher(parent)
{
    gMServiceListenerStubMap->createStub(busName, this);
}

MServiceListener::MServiceListener(const QDBusConnection, const QString &busName, QObject *)
{
    gMServiceListenerStubMap->createStub(busName, this);
}

MServiceListener::~MServiceListener()
{
    gMServiceListenerStubMap->deleteStub(this);
}

void MServiceListener::startListening(bool)
{
    gMServiceListenerStubMap->findStub(this)->stubMethodEntered("startListening");
}

MServiceListener::ServicePresence MServiceListener::isServicePresent() const
{
    return gMServiceListenerStubMap->findStub(this)->stubReturnValue<MServiceListener::ServicePresence>("isServicePresent");
}

void MServiceListener::onNameHasOwnerFinished(QDBusPendingCallWatcher *)
{
    gMServiceListenerStubMap->findStub(this)->stubMethodEntered("onNameHasOwnerFinished");
}

void MServiceListener::setServiceGone()
{
    gMServiceListenerStubMap->findStub(this)->stubMethodEntered("setServiceGone");
}

void MServiceListener::setServicePresent()
{
    gMServiceListenerStubMap->findStub(this)->stubMethodEntered("setServicePresent");
}

Q_DECLARE_METATYPE(MServiceListener::ServicePresence);

#endif
