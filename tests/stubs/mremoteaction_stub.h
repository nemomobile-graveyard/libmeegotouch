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

#ifndef MREMOTEACTION_STUB
#define MREMOTEACTION_STUB

#include "mremoteaction.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MRemoteActionStub : public StubBase
{
public:
    virtual void MRemoteActionConstructor(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent);
    virtual void MRemoteActionConstructor(const QString &string, QObject *parent);
    virtual void MRemoteActionConstructor(const MRemoteAction &action);
    virtual void MRemoteActionDestructor();
    virtual QString toString() const;
    virtual void call();
    virtual void fromString(const QString &string);
    virtual void MRemoteActionConstructor(MRemoteActionPrivate &dd, QObject *parent);
};

// 2. IMPLEMENT STUB
void MRemoteActionStub::MRemoteActionConstructor(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent)
{
    Q_UNUSED(serviceName);
    Q_UNUSED(objectPath);
    Q_UNUSED(interface);
    Q_UNUSED(methodName);
    Q_UNUSED(arguments);
    Q_UNUSED(parent);

}
void MRemoteActionStub::MRemoteActionConstructor(const QString &string, QObject *parent)
{
    Q_UNUSED(string);
    Q_UNUSED(parent);

}
void MRemoteActionStub::MRemoteActionConstructor(const MRemoteAction &action)
{
    Q_UNUSED(action);

}
void MRemoteActionStub::MRemoteActionDestructor()
{

}
QString MRemoteActionStub::toString() const
{
    stubMethodEntered("toString");
    return stubReturnValue<QString>("toString");
}

void MRemoteActionStub::call()
{
    stubMethodEntered("call");
}

void MRemoteActionStub::fromString(const QString &string)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(string));
    stubMethodEntered("fromString", params);
}

void MRemoteActionStub::MRemoteActionConstructor(MRemoteActionPrivate &dd, QObject *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(parent);

}


// 3. CREATE A STUB INSTANCE
MRemoteActionStub gDefaultMRemoteActionStub;
MRemoteActionStub *gMRemoteActionStub = &gDefaultMRemoteActionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MRemoteAction::MRemoteAction(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent) : MAction(parent)
{
    gMRemoteActionStub->MRemoteActionConstructor(serviceName, objectPath, interface, methodName, arguments, parent);
}

MRemoteAction::MRemoteAction(const QString &string, QObject *parent) : MAction(parent)
{
    gMRemoteActionStub->MRemoteActionConstructor(string, parent);
}

MRemoteAction::MRemoteAction(const MRemoteAction &action) : MAction(NULL)
{
    gMRemoteActionStub->MRemoteActionConstructor(action);
}

MRemoteAction::~MRemoteAction()
{
    gMRemoteActionStub->MRemoteActionDestructor();
}

QString MRemoteAction::toString() const
{
    return gMRemoteActionStub->toString();
}

void MRemoteAction::call()
{
    gMRemoteActionStub->call();
}

void MRemoteAction::fromString(const QString &string)
{
    gMRemoteActionStub->fromString(string);
}

MRemoteAction::MRemoteAction(MRemoteActionPrivate &dd, QObject *parent) : MAction(NULL)
{
    gMRemoteActionStub->MRemoteActionConstructor(dd, parent);
}


#endif
