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

#ifndef MSTYLEDESCRIPTION_STUB
#define MSTYLEDESCRIPTION_STUB

#include "mstyledescription.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MStyleDescriptionStub : public StubBase
{
public:
    virtual void MStyleDescriptionConstructor();
    virtual void MStyleDescriptionConstructor(const MStyleDescription &other);
    virtual void MStyleDescriptionDestructor();
    virtual const MStyleDescription &operator=(const MStyleDescription &other);
    virtual bool operator==(const MStyleDescription &other) const;
    virtual void addAttribute(int index, const QString &name);
    virtual QString attributeAtIndex(int index) const;
    virtual int numberOfAttributes() const;
    MStyleDescriptionPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MStyleDescriptionStub::MStyleDescriptionConstructor()
{

}
void MStyleDescriptionStub::MStyleDescriptionConstructor(const MStyleDescription &other)
{
    Q_UNUSED(other);

}
void MStyleDescriptionStub::MStyleDescriptionDestructor()
{

}
const MStyleDescription &MStyleDescriptionStub::operator=(const MStyleDescription &other)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MStyleDescription & >(other));
    stubMethodEntered("operator=", params);
    return stubReturnValue<const MStyleDescription>("operator=");
}

bool MStyleDescriptionStub::operator==(const MStyleDescription &other) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MStyleDescription & >(other));
    stubMethodEntered("operator==", params);
    return stubReturnValue<bool>("operator==");
}

void MStyleDescriptionStub::addAttribute(int index, const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("addAttribute", params);
}

QString MStyleDescriptionStub::attributeAtIndex(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("attributeAtIndex", params);
    return stubReturnValue<QString>("attributeAtIndex");
}

int MStyleDescriptionStub::numberOfAttributes() const
{
    stubMethodEntered("numberOfAttributes");
    return stubReturnValue<int>("numberOfAttributes");
}



// 3. CREATE A STUB INSTANCE
// while executing, static member initization is done first.
static MStyleDescriptionStub gDefaultMStyleDescriptionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MStyleDescription::MStyleDescription() : d_ptr(0)
{
    gDefaultMStyleDescriptionStub.MStyleDescriptionConstructor();
}

MStyleDescription::MStyleDescription(const MStyleDescription &other) : d_ptr(0)
{
    gDefaultMStyleDescriptionStub.MStyleDescriptionConstructor(other);
}

MStyleDescription::~MStyleDescription()
{
    gDefaultMStyleDescriptionStub.MStyleDescriptionDestructor();
}

const MStyleDescription &MStyleDescription::operator=(const MStyleDescription &other)
{
    return gDefaultMStyleDescriptionStub.operator = (other);
}

bool MStyleDescription::operator==(const MStyleDescription &other) const
{
    return gDefaultMStyleDescriptionStub.operator == (other);
}

void MStyleDescription::addAttribute(int index, const QString &name)
{
    gDefaultMStyleDescriptionStub.addAttribute(index, name);
}

QString MStyleDescription::attributeAtIndex(int index) const
{
    return gDefaultMStyleDescriptionStub.attributeAtIndex(index);
}

int MStyleDescription::numberOfAttributes() const
{
    return gDefaultMStyleDescriptionStub.numberOfAttributes();
}


#endif
