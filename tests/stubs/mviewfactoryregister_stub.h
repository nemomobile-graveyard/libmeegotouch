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

#ifndef MVIEWFACTORYREGISTER_STUB
#define MVIEWFACTORYREGISTER_STUB

#include "mviewfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MViewFactoryRegisterStub : public StubBase
{
public:
    const char *name ;
    ViewCreatorFunction function ;
    MViewFactoryRegister *nextView ;
    MViewFactoryRegister *viewsHead ;
    virtual void MViewFactoryRegisterConstructor(const char *viewName, ViewCreatorFunction viewCreator);
};

// 2. IMPLEMENT STUB
void MViewFactoryRegisterStub::MViewFactoryRegisterConstructor(const char *viewName, ViewCreatorFunction viewCreator)
{
    Q_UNUSED(viewName);
    Q_UNUSED(viewCreator);

}


// 3. CREATE A STUB INSTANCE
MViewFactoryRegisterStub gDefaultMViewFactoryRegisterStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MViewFactoryRegister::MViewFactoryRegister(const char *viewName, ViewCreatorFunction viewCreator)
{
    gDefaultMViewFactoryRegisterStub.MViewFactoryRegisterConstructor(viewName, viewCreator);
}


#endif
