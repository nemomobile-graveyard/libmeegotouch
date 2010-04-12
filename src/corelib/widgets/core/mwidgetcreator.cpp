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

#include "mwidgetcreator.h"
#include "mclassfactory.h"

class MWidgetCreatorBasePrivate
{
public:
    QString assemblyName;
    M::AssemblyType assemblyType;
};

MWidgetCreatorBase::MWidgetCreatorBase(const char *widgetClassName, const char *widgetAssemblyName, M::AssemblyType widgetAssemblyType) :
    d_ptr(new MWidgetCreatorBasePrivate)
{
    d_ptr->assemblyName = widgetAssemblyName;
    d_ptr->assemblyType = widgetAssemblyType;
    MClassFactory::instance()->registerWidgetCreator(this, widgetClassName);
}

MWidgetCreatorBase::~MWidgetCreatorBase()
{
    MClassFactory::instance()->unregisterWidgetCreator(this);
    delete d_ptr;
}

QString MWidgetCreatorBase::assemblyName() const
{
    return d_ptr->assemblyName;
}

M::AssemblyType MWidgetCreatorBase::assemblyType() const
{
    return d_ptr->assemblyType;
}
