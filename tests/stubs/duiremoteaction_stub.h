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

#ifndef DUIREMOTEACTION_STUB
#define DUIREMOTEACTION_STUB

#include "duiremoteaction.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiRemoteActionStub : public StubBase
{
public:
    virtual void DuiRemoteActionConstructor(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent);
    virtual void DuiRemoteActionConstructor(const QString &string, QObject *parent);
    virtual void DuiRemoteActionConstructor(const DuiRemoteAction &action);
    virtual void DuiRemoteActionDestructor();
    virtual QString toString() const;
    virtual void call();
    virtual void fromString(const QString &string);
    virtual void DuiRemoteActionConstructor(DuiRemoteActionPrivate &dd, QObject *parent);
};

// 2. IMPLEMENT STUB
void DuiRemoteActionStub::DuiRemoteActionConstructor(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent)
{
    Q_UNUSED(serviceName);
    Q_UNUSED(objectPath);
    Q_UNUSED(interface);
    Q_UNUSED(methodName);
    Q_UNUSED(arguments);
    Q_UNUSED(parent);

}
void DuiRemoteActionStub::DuiRemoteActionConstructor(const QString &string, QObject *parent)
{
    Q_UNUSED(string);
    Q_UNUSED(parent);

}
void DuiRemoteActionStub::DuiRemoteActionConstructor(const DuiRemoteAction &action)
{
    Q_UNUSED(action);

}
void DuiRemoteActionStub::DuiRemoteActionDestructor()
{

}
QString DuiRemoteActionStub::toString() const
{
    stubMethodEntered("toString");
    return stubReturnValue<QString>("toString");
}

void DuiRemoteActionStub::call()
{
    stubMethodEntered("call");
}

void DuiRemoteActionStub::fromString(const QString &string)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(string));
    stubMethodEntered("fromString", params);
}

void DuiRemoteActionStub::DuiRemoteActionConstructor(DuiRemoteActionPrivate &dd, QObject *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(parent);

}


// 3. CREATE A STUB INSTANCE
DuiRemoteActionStub gDefaultDuiRemoteActionStub;
DuiRemoteActionStub *gDuiRemoteActionStub = &gDefaultDuiRemoteActionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiRemoteAction::DuiRemoteAction(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent) : DuiAction(parent)
{
    gDuiRemoteActionStub->DuiRemoteActionConstructor(serviceName, objectPath, interface, methodName, arguments, parent);
}

DuiRemoteAction::DuiRemoteAction(const QString &string, QObject *parent) : DuiAction(parent)
{
    gDuiRemoteActionStub->DuiRemoteActionConstructor(string, parent);
}

DuiRemoteAction::DuiRemoteAction(const DuiRemoteAction &action) : DuiAction(NULL)
{
    gDuiRemoteActionStub->DuiRemoteActionConstructor(action);
}

DuiRemoteAction::~DuiRemoteAction()
{
    gDuiRemoteActionStub->DuiRemoteActionDestructor();
}

QString DuiRemoteAction::toString() const
{
    return gDuiRemoteActionStub->toString();
}

void DuiRemoteAction::call()
{
    gDuiRemoteActionStub->call();
}

void DuiRemoteAction::fromString(const QString &string)
{
    gDuiRemoteActionStub->fromString(string);
}

DuiRemoteAction::DuiRemoteAction(DuiRemoteActionPrivate &dd, QObject *parent) : DuiAction(NULL)
{
    gDuiRemoteActionStub->DuiRemoteActionConstructor(dd, parent);
}


#endif
