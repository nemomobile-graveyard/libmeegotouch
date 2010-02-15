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

#ifndef DUIAPPLETSHAREDMUTEXSTUB_H
#define DUIAPPLETSHAREDMUTEXSTUB_H

#include <stubbase.h>
#include <duiappletsharedmutex.h>

class DuiAppletSharedMutexStub : public StubBase
{
public:
    virtual void duiAppletSharedMutexStubConstructor();
    virtual void duiAppletSharedMutexStubDestructor();
    virtual bool init(const QString &key);
    virtual bool lock();
    virtual bool unlock();
    virtual bool tryLock();
};

void DuiAppletSharedMutexStub::duiAppletSharedMutexStubConstructor()
{
    stubMethodEntered("duiAppletSharedMutexStubConstructor");
}

void DuiAppletSharedMutexStub::duiAppletSharedMutexStubDestructor()
{
    stubMethodEntered("duiAppletSharedMutexStubDestructor");
}

bool
DuiAppletSharedMutexStub::init(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    stubMethodEntered("init", params);
    return stubReturnValue<bool>("init");
}


bool
DuiAppletSharedMutexStub::lock()
{
    stubMethodEntered("lock");
    return stubReturnValue<bool>("lock");
}


bool
DuiAppletSharedMutexStub::unlock()
{
    stubMethodEntered("unlock");
    return stubReturnValue<bool>("unlock");
}


bool
DuiAppletSharedMutexStub::tryLock()
{
    stubMethodEntered("tryLock");
    return stubReturnValue<bool>("tryLock");
}

DuiAppletSharedMutexStub gDefaultDuiAppletSharedMutexStub;
DuiAppletSharedMutexStub *gDuiAppletSharedMutexStub = &gDefaultDuiAppletSharedMutexStub;

DuiAppletSharedMutex::DuiAppletSharedMutex()
{
    gDuiAppletSharedMutexStub->duiAppletSharedMutexStubConstructor();
}

DuiAppletSharedMutex::~DuiAppletSharedMutex()
{
    gDuiAppletSharedMutexStub->duiAppletSharedMutexStubDestructor();
}


bool
DuiAppletSharedMutex::init(const QString &key)
{
    return gDuiAppletSharedMutexStub->init(key);
}


bool
DuiAppletSharedMutex::lock()
{
    return gDuiAppletSharedMutexStub->lock();
}


bool
DuiAppletSharedMutex::unlock()
{
    return gDuiAppletSharedMutexStub->unlock();
}


bool
DuiAppletSharedMutex::tryLock()
{
    return gDuiAppletSharedMutexStub->tryLock();
}

#endif
