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

#include "duistylecreator.h"
#include "duiclassfactory.h"

class DuiStyleCreatorBasePrivate
{
public:
    QString assemblyName;
    Dui::AssemblyType assemblyType;
};

DuiStyleCreatorBase::DuiStyleCreatorBase(const char *styleClassName, const char *styleAssemblyName, Dui::AssemblyType styleAssemblyType) :
    d_ptr(new DuiStyleCreatorBasePrivate)
{
    d_ptr->assemblyName = styleAssemblyName;
    d_ptr->assemblyType = styleAssemblyType;
    DuiClassFactory::instance()->registerStyleCreator(this, styleClassName);
}

DuiStyleCreatorBase::~DuiStyleCreatorBase()
{
    DuiClassFactory::instance()->unregisterStyleCreator(this);
    delete d_ptr;
}

QString DuiStyleCreatorBase::assemblyName() const
{
    return d_ptr->assemblyName;
}

Dui::AssemblyType DuiStyleCreatorBase::assemblyType() const
{
    return d_ptr->assemblyType;
}
