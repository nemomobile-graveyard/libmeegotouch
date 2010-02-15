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

#ifndef DUIVIEWFACTORYREGISTER_STUB
#define DUIVIEWFACTORYREGISTER_STUB

#include "duiviewfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiViewFactoryRegisterStub : public StubBase
{
public:
    const char *name ;
    ViewCreatorFunction function ;
    DuiViewFactoryRegister *nextView ;
    DuiViewFactoryRegister *viewsHead ;
    virtual void DuiViewFactoryRegisterConstructor(const char *viewName, ViewCreatorFunction viewCreator);
};

// 2. IMPLEMENT STUB
void DuiViewFactoryRegisterStub::DuiViewFactoryRegisterConstructor(const char *viewName, ViewCreatorFunction viewCreator)
{
    Q_UNUSED(viewName);
    Q_UNUSED(viewCreator);

}


// 3. CREATE A STUB INSTANCE
DuiViewFactoryRegisterStub gDefaultDuiViewFactoryRegisterStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiViewFactoryRegister::DuiViewFactoryRegister(const char *viewName, ViewCreatorFunction viewCreator)
{
    gDefaultDuiViewFactoryRegisterStub.DuiViewFactoryRegisterConstructor(viewName, viewCreator);
}


#endif
