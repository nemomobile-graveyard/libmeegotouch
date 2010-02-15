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

#include "duiwidgetcreator.h"
#include "duiclassfactory.h"

class DuiWidgetCreatorBasePrivate
{
public:
    QString assemblyName;
    Dui::AssemblyType assemblyType;
};

DuiWidgetCreatorBase::DuiWidgetCreatorBase(const char *widgetClassName, const char *widgetAssemblyName, Dui::AssemblyType widgetAssemblyType) :
    d_ptr(new DuiWidgetCreatorBasePrivate)
{
    d_ptr->assemblyName = widgetAssemblyName;
    d_ptr->assemblyType = widgetAssemblyType;
    DuiClassFactory::instance()->registerWidgetCreator(this, widgetClassName);
}

DuiWidgetCreatorBase::~DuiWidgetCreatorBase()
{
    DuiClassFactory::instance()->unregisterWidgetCreator(this);
    delete d_ptr;
}

QString DuiWidgetCreatorBase::assemblyName() const
{
    return d_ptr->assemblyName;
}

Dui::AssemblyType DuiWidgetCreatorBase::assemblyType() const
{
    return d_ptr->assemblyType;
}
