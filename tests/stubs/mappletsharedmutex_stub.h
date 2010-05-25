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

#ifndef MAPPLETSHAREDMUTEXSTUB_H
#define MAPPLETSHAREDMUTEXSTUB_H

#include <stubbase.h>
#include <mappletsharedmutex.h>

class MAppletSharedMutexStub : public StubBase
{
public:
    virtual void mAppletSharedMutexStubConstructor();
    virtual void mAppletSharedMutexStubDestructor();
    virtual bool init(const QString &key);
    virtual bool lock();
    virtual bool unlock();
    virtual bool tryLock();
};

void MAppletSharedMutexStub::mAppletSharedMutexStubConstructor()
{
    stubMethodEntered("mAppletSharedMutexStubConstructor");
}

void MAppletSharedMutexStub::mAppletSharedMutexStubDestructor()
{
    stubMethodEntered("mAppletSharedMutexStubDestructor");
}

bool
MAppletSharedMutexStub::init(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    stubMethodEntered("init", params);
    return stubReturnValue<bool>("init");
}


bool
MAppletSharedMutexStub::lock()
{
    stubMethodEntered("lock");
    return stubReturnValue<bool>("lock");
}


bool
MAppletSharedMutexStub::unlock()
{
    stubMethodEntered("unlock");
    return stubReturnValue<bool>("unlock");
}


bool
MAppletSharedMutexStub::tryLock()
{
    stubMethodEntered("tryLock");
    return stubReturnValue<bool>("tryLock");
}

MAppletSharedMutexStub gDefaultMAppletSharedMutexStub;
MAppletSharedMutexStub *gMAppletSharedMutexStub = &gDefaultMAppletSharedMutexStub;

MAppletSharedMutex::MAppletSharedMutex()
  : d_ptr( 0 )
{
    gMAppletSharedMutexStub->mAppletSharedMutexStubConstructor();
}

MAppletSharedMutex::~MAppletSharedMutex()
{
    gMAppletSharedMutexStub->mAppletSharedMutexStubDestructor();
}


bool
MAppletSharedMutex::init(const QString &key)
{
    return gMAppletSharedMutexStub->init(key);
}


bool
MAppletSharedMutex::lock()
{
    return gMAppletSharedMutexStub->lock();
}


bool
MAppletSharedMutex::unlock()
{
    return gMAppletSharedMutexStub->unlock();
}


bool
MAppletSharedMutex::tryLock()
{
    return gMAppletSharedMutexStub->tryLock();
}

#endif
