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

#ifndef CONTEXTPROPERTY_STUB_H
#define CONTEXTPROPERTY_STUB_H

#include <contextsubscriber/contextproperty.h>
#include <contextsubscriber/contextpropertyinfo.h>
#include <stubmap.h>
#include <QDebug>

// == Global & default stub map instance ================================================

StubMap<QString, ContextProperty> gDefaultContextPropertyStubMap;
StubMap<QString, ContextProperty> *gContextPropertyStubMap = &gDefaultContextPropertyStubMap;

// == ContextPropertyInfo ===============================================================

QString ContextPropertyInfo::providerDBusName() const
{
    return "sessionbus";
}

QDBusConnection::BusType ContextPropertyInfo::providerDBusType() const
{
    return QDBusConnection::SessionBus;
}

// == ContextProperty ===================================================================

ContextProperty::ContextProperty(const QString &key, QObject *parent)
    : QObject(parent)
{
    gContextPropertyStubMap->createStub(key, this)->stubSetReturnValue("mykey", key);
}

ContextProperty::~ContextProperty()
{
    gContextPropertyStubMap->deleteStub(this);
}

QVariant ContextProperty::value() const
{
    return gContextPropertyStubMap->findStub(this)->stubReturnValue<QVariant>("value");
}

void ContextProperty::subscribe() const
{
    gContextPropertyStubMap->findStub(this)->stubMethodEntered("subscribe");
}

void ContextProperty::unsubscribe() const
{
    gContextPropertyStubMap->findStub(this)->stubMethodEntered("unsubscribe");
}

void ContextProperty::waitForSubscription() const
{
    gContextPropertyStubMap->findStub(this)->stubMethodEntered("waitForSubscription");
}

void ContextProperty::waitForSubscription(bool block) const
{
    Q_UNUSED(block);

    gContextPropertyStubMap->findStub(this)->stubMethodEntered("waitForSubscription");
}

const ContextPropertyInfo *ContextProperty::info() const
{
    static ContextPropertyInfo instance("stubbed-context-property");
    return &instance;
}

void ContextProperty::ignoreCommander()
{
}

void ContextProperty::setTypeCheck(bool typeCheck)
{
    Q_UNUSED(typeCheck);
}

void ContextProperty::onValueChanged()
{
    return gContextPropertyStubMap->findStub(this)->stubMethodEntered("onValueChanged");
}

#endif // CONTEXTPROPERTY_STUB_H
