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

#ifndef MSERVICEINVOKER_STUB
#define MSERVICEINVOKER_STUB

#include "mserviceinvoker.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MServiceInvokerStub : public StubBase
{
public:
    virtual void MServiceInvokerConstructor();
    virtual void invoke();
};

// 2. IMPLEMENT STUB
void MServiceInvokerStub::MServiceInvokerConstructor()
{

}

void MServiceInvokerStub::invoke()
{
    stubMethodEntered("invoke");
}



// 3. CREATE A STUB INSTANCE
MServiceInvokerStub gDefaultMServiceInvokerStub;
MServiceInvokerStub *gMServiceInvokerStub = &gDefaultMServiceInvokerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MServiceInvoker::MServiceInvoker()
{
    gMServiceInvokerStub->MServiceInvokerConstructor();
}

MServiceInvoker *MServiceInvoker::instance()
{
    return new MServiceInvoker;
}

void MServiceInvoker::invoke()
{
    gMServiceInvokerStub->invoke();
}


#endif
