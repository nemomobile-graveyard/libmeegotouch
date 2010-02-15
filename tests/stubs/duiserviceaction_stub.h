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

#ifndef DUISERVICEACTION_STUB
#define DUISERVICEACTION_STUB

#include "duiserviceaction.h"
#include <stubbase.h>
#include <duiaction_stub.h>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiServiceActionStub : public StubBase
{
public:
    virtual void DuiServiceActionConstructor(QObject *parent);
    virtual void DuiServiceActionDestructor();
    virtual void executeService();
    virtual void DuiServiceActionConstructor(DuiServiceActionPrivate &dd, QObject *parent);
};

// 2. IMPLEMENT STUB
void DuiServiceActionStub::DuiServiceActionConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void DuiServiceActionStub::DuiServiceActionDestructor()
{

}
void DuiServiceActionStub::executeService()
{
    stubMethodEntered("executeService");
}

void DuiServiceActionStub::DuiServiceActionConstructor(DuiServiceActionPrivate &dd, QObject *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(parent);

}


// 3. CREATE A STUB INSTANCE
DuiServiceActionStub gDefaultDuiServiceActionStub;
DuiServiceActionStub *gDuiServiceActionStub = &gDefaultDuiServiceActionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiServiceAction::DuiServiceAction(QObject *parent) : DuiAction(parent)
{
    gDuiServiceActionStub->DuiServiceActionConstructor(parent);
}

DuiServiceAction::~DuiServiceAction()
{
    gDuiServiceActionStub->DuiServiceActionDestructor();
}

void DuiServiceAction::executeService()
{
    gDuiServiceActionStub->executeService();
}

DuiServiceAction::DuiServiceAction(DuiServiceActionPrivate &dd, QObject *parent) : DuiAction(parent)
{
    gDuiServiceActionStub->DuiServiceActionConstructor(dd, parent);
}


#endif
