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

#ifndef MSERVICEACTION_STUB
#define MSERVICEACTION_STUB

#include "mserviceaction.h"
#include <stubbase.h>
#include <maction_stub.h>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MServiceActionStub : public StubBase
{
public:
    virtual void MServiceActionConstructor(QObject *parent);
    virtual void MServiceActionDestructor();
    virtual void executeService();
    virtual void MServiceActionConstructor(MServiceActionPrivate &dd, QObject *parent);
};

// 2. IMPLEMENT STUB
void MServiceActionStub::MServiceActionConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void MServiceActionStub::MServiceActionDestructor()
{

}
void MServiceActionStub::executeService()
{
    stubMethodEntered("executeService");
}

void MServiceActionStub::MServiceActionConstructor(MServiceActionPrivate &dd, QObject *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(parent);

}


// 3. CREATE A STUB INSTANCE
MServiceActionStub gDefaultMServiceActionStub;
MServiceActionStub *gMServiceActionStub = &gDefaultMServiceActionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MServiceAction::MServiceAction(QObject *parent) : MAction(parent)
{
    gMServiceActionStub->MServiceActionConstructor(parent);
}

MServiceAction::~MServiceAction()
{
    gMServiceActionStub->MServiceActionDestructor();
}

void MServiceAction::executeService()
{
    gMServiceActionStub->executeService();
}

MServiceAction::MServiceAction(MServiceActionPrivate &dd, QObject *parent) : MAction(parent)
{
    gMServiceActionStub->MServiceActionConstructor(dd, parent);
}


#endif
