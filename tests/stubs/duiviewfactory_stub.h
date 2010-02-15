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

#ifndef DUIVIEWFACTORY_STUB
#define DUIVIEWFACTORY_STUB

#include "duiviewfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
class DuiViewFactoryStub : public StubBase
{
public:
    virtual void DuiViewFactoryConstructor();
    virtual void DuiViewFactoryDestructor();
    virtual bool registerViewClass(const QString &viewClass, ViewCreatorFunction function);
    virtual bool unregisterViewClass(const QString &viewClass);
    virtual DuiWidgetView *createView(const QString &viewClass, const DuiWidgetController *controller);
};

// 2. IMPLEMENT STUB
void DuiViewFactoryStub::DuiViewFactoryConstructor()
{

}
void DuiViewFactoryStub::DuiViewFactoryDestructor()
{

}
bool DuiViewFactoryStub::registerViewClass(const QString &viewClass, ViewCreatorFunction function)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(viewClass));
    params.append(new Parameter<ViewCreatorFunction >(function));
    stubMethodEntered("registerViewClass", params);
    return stubReturnValue<bool>("registerViewClass");
}

bool DuiViewFactoryStub::unregisterViewClass(const QString &viewClass)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(viewClass));
    stubMethodEntered("unregisterViewClass", params);
    return stubReturnValue<bool>("unregisterViewClass");
}

DuiWidgetView *DuiViewFactoryStub::createView(const QString &viewClass, const DuiWidgetController *controller)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(viewClass));
    params.append(new Parameter<const DuiWidgetController * >(controller));
    stubMethodEntered("createView", params);
    return stubReturnValue<DuiWidgetView *>("createView");
}



// 3. CREATE A STUB INSTANCE
DuiViewFactoryStub gDefaultDuiViewFactoryStub;
DuiViewFactoryStub *gDuiViewFactoryStub = &gDefaultDuiViewFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiViewFactory::DuiViewFactory()
{
    gDuiViewFactoryStub->DuiViewFactoryConstructor();
}

DuiViewFactory::~DuiViewFactory()
{
    gDuiViewFactoryStub->DuiViewFactoryDestructor();
}

bool DuiViewFactory::registerViewClass(const QString &viewClass, ViewCreatorFunction function)
{
    return gDuiViewFactoryStub->registerViewClass(viewClass, function);
}

bool DuiViewFactory::unregisterViewClass(const QString &viewClass)
{
    return gDuiViewFactoryStub->unregisterViewClass(viewClass);
}

DuiWidgetView *DuiViewFactory::createView(const QString &viewClass, const DuiWidgetController *controller)
{
    return gDuiViewFactoryStub->createView(viewClass, controller);
}


#endif
