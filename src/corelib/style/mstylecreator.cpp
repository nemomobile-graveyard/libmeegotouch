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

#include "mstylecreator.h"
#include "mclassfactory.h"

class MStyleCreatorBasePrivate
{
public:
    QString assemblyName;
    M::AssemblyType assemblyType;
};

MStyleCreatorBase::MStyleCreatorBase(const char *styleClassName, const char *styleAssemblyName, M::AssemblyType styleAssemblyType) :
    d_ptr(new MStyleCreatorBasePrivate)
{
    d_ptr->assemblyName = styleAssemblyName;
    d_ptr->assemblyType = styleAssemblyType;
    MClassFactory::instance()->registerStyleCreator(this, styleClassName);
}

MStyleCreatorBase::~MStyleCreatorBase()
{
    MClassFactory::instance()->unregisterStyleCreator(this);
    delete d_ptr;
}

QString MStyleCreatorBase::assemblyName() const
{
    return d_ptr->assemblyName;
}

M::AssemblyType MStyleCreatorBase::assemblyType() const
{
    return d_ptr->assemblyType;
}
