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

#ifndef DUISTYLEDESCRIPTION_STUB
#define DUISTYLEDESCRIPTION_STUB

#include "duistyledescription.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiStyleDescriptionStub : public StubBase
{
public:
    virtual void DuiStyleDescriptionConstructor();
    virtual void DuiStyleDescriptionConstructor(const DuiStyleDescription &other);
    virtual void DuiStyleDescriptionDestructor();
    virtual const DuiStyleDescription &operator=(const DuiStyleDescription &other);
    virtual bool operator==(const DuiStyleDescription &other) const;
    virtual void addAttribute(int index, const QString &name);
    virtual QString attributeAtIndex(int index) const;
    virtual int numberOfAttributes() const;
    DuiStyleDescriptionPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiStyleDescriptionStub::DuiStyleDescriptionConstructor()
{

}
void DuiStyleDescriptionStub::DuiStyleDescriptionConstructor(const DuiStyleDescription &other)
{
    Q_UNUSED(other);

}
void DuiStyleDescriptionStub::DuiStyleDescriptionDestructor()
{

}
const DuiStyleDescription &DuiStyleDescriptionStub::operator=(const DuiStyleDescription &other)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiStyleDescription & >(other));
    stubMethodEntered("operator=", params);
    return stubReturnValue<const DuiStyleDescription>("operator=");
}

bool DuiStyleDescriptionStub::operator==(const DuiStyleDescription &other) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiStyleDescription & >(other));
    stubMethodEntered("operator==", params);
    return stubReturnValue<bool>("operator==");
}

void DuiStyleDescriptionStub::addAttribute(int index, const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("addAttribute", params);
}

QString DuiStyleDescriptionStub::attributeAtIndex(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("attributeAtIndex", params);
    return stubReturnValue<QString>("attributeAtIndex");
}

int DuiStyleDescriptionStub::numberOfAttributes() const
{
    stubMethodEntered("numberOfAttributes");
    return stubReturnValue<int>("numberOfAttributes");
}



// 3. CREATE A STUB INSTANCE
// while executing, static member initization is done first.
static DuiStyleDescriptionStub gDefaultDuiStyleDescriptionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiStyleDescription::DuiStyleDescription() : d_ptr(0)
{
    gDefaultDuiStyleDescriptionStub.DuiStyleDescriptionConstructor();
}

DuiStyleDescription::DuiStyleDescription(const DuiStyleDescription &other) : d_ptr(0)
{
    gDefaultDuiStyleDescriptionStub.DuiStyleDescriptionConstructor(other);
}

DuiStyleDescription::~DuiStyleDescription()
{
    gDefaultDuiStyleDescriptionStub.DuiStyleDescriptionDestructor();
}

const DuiStyleDescription &DuiStyleDescription::operator=(const DuiStyleDescription &other)
{
    return gDefaultDuiStyleDescriptionStub.operator = (other);
}

bool DuiStyleDescription::operator==(const DuiStyleDescription &other) const
{
    return gDefaultDuiStyleDescriptionStub.operator == (other);
}

void DuiStyleDescription::addAttribute(int index, const QString &name)
{
    gDefaultDuiStyleDescriptionStub.addAttribute(index, name);
}

QString DuiStyleDescription::attributeAtIndex(int index) const
{
    return gDefaultDuiStyleDescriptionStub.attributeAtIndex(index);
}

int DuiStyleDescription::numberOfAttributes() const
{
    return gDefaultDuiStyleDescriptionStub.numberOfAttributes();
}


#endif
