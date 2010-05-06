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

#ifndef MVIEWFACTORY_STUB
#define MVIEWFACTORY_STUB

#include "mviewfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
class MViewFactoryStub : public StubBase
{
public:
    virtual void MViewFactoryConstructor();
    virtual void MViewFactoryDestructor();
    virtual bool registerViewClass(const QString &viewClass, ViewCreatorFunction function);
    virtual bool unregisterViewClass(const QString &viewClass);
    virtual MWidgetView *createView(const QString &viewClass, const MWidgetController *controller);
};

// 2. IMPLEMENT STUB
void MViewFactoryStub::MViewFactoryConstructor()
{

}
void MViewFactoryStub::MViewFactoryDestructor()
{

}
bool MViewFactoryStub::registerViewClass(const QString &viewClass, ViewCreatorFunction function)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(viewClass));
    params.append(new Parameter<ViewCreatorFunction >(function));
    stubMethodEntered("registerViewClass", params);
    return stubReturnValue<bool>("registerViewClass");
}

bool MViewFactoryStub::unregisterViewClass(const QString &viewClass)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(viewClass));
    stubMethodEntered("unregisterViewClass", params);
    return stubReturnValue<bool>("unregisterViewClass");
}

MWidgetView *MViewFactoryStub::createView(const QString &viewClass, const MWidgetController *controller)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(viewClass));
    params.append(new Parameter<const MWidgetController * >(controller));
    stubMethodEntered("createView", params);
    return stubReturnValue<MWidgetView *>("createView");
}



// 3. CREATE A STUB INSTANCE
MViewFactoryStub gDefaultMViewFactoryStub;
MViewFactoryStub *gMViewFactoryStub = &gDefaultMViewFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MViewFactory::MViewFactory()
{
    gMViewFactoryStub->MViewFactoryConstructor();
}

MViewFactory::~MViewFactory()
{
    gMViewFactoryStub->MViewFactoryDestructor();
}

bool MViewFactory::registerViewClass(const QString &viewClass, ViewCreatorFunction function)
{
    return gMViewFactoryStub->registerViewClass(viewClass, function);
}

bool MViewFactory::unregisterViewClass(const QString &viewClass)
{
    return gMViewFactoryStub->unregisterViewClass(viewClass);
}

MWidgetView *MViewFactory::createView(const QString &viewClass, const MWidgetController *controller)
{
    return gMViewFactoryStub->createView(viewClass, controller);
}


#endif
