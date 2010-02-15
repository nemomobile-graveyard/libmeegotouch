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

#ifndef DUISERVICEINVOKER_STUB
#define DUISERVICEINVOKER_STUB

#include "duiserviceinvoker.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiServiceInvokerStub : public StubBase
{
public:
    virtual void DuiServiceInvokerConstructor();
    virtual void invoke();
};

// 2. IMPLEMENT STUB
void DuiServiceInvokerStub::DuiServiceInvokerConstructor()
{

}

void DuiServiceInvokerStub::invoke()
{
    stubMethodEntered("invoke");
}



// 3. CREATE A STUB INSTANCE
DuiServiceInvokerStub gDefaultDuiServiceInvokerStub;
DuiServiceInvokerStub *gDuiServiceInvokerStub = &gDefaultDuiServiceInvokerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiServiceInvoker::DuiServiceInvoker()
{
    gDuiServiceInvokerStub->DuiServiceInvokerConstructor();
}

DuiServiceInvoker *DuiServiceInvoker::instance()
{
    return new DuiServiceInvoker;
}

void DuiServiceInvoker::invoke()
{
    gDuiServiceInvokerStub->invoke();
}


#endif
