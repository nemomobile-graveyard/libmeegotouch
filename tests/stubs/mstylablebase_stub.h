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

#ifndef MSTYLABLEBASE_STUB
#define MSTYLABLEBASE_STUB

#include "mstylablebase.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MStylableBaseStub : public StubBase
{
public:
    virtual void MStylableBaseConstructor();
    virtual void MStylableBaseDestructor();
    virtual void updateStyle();
//  void styleUpdated ;
    virtual const MStyleAttribute &styleAttribute(int index) const;
    virtual bool styleHasAttribute(int index) const;
    virtual void initialize(const MStyleDescription &description, const QObject *object, const QString &type, const QString &mode);
    virtual const MStyleDescription &completeStyleDescription() const;
    virtual const MStyleDescription &styleDescription();
    mutable MStyleAttribute *att;
    MStylableBasePrivate *d_ptr ;
};

// 2. IMPLEMENT STUB

void MStylableBaseStub::MStylableBaseConstructor()
{
    QColor col(0, 0, 0);
    att = new MStyleAttribute(col);

}
void MStylableBaseStub::MStylableBaseDestructor()
{
    delete att;
}
void MStylableBaseStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

const MStyleAttribute &MStylableBaseStub::styleAttribute(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("styleAttribute", params);
    return *att;
}

bool MStylableBaseStub::styleHasAttribute(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("styleHasAttribute", params);
    return stubReturnValue<bool>("styleHasAttribute");
}

void MStylableBaseStub::initialize(const MStyleDescription &description, const QObject *object, const QString &type, const QString &mode)
{
    Q_UNUSED(object);

    QList<ParameterBase *> params;
    params.append(new Parameter<const MStyleDescription & >(description));
    params.append(new Parameter<const QObject *>(object));
    params.append(new Parameter<const QString &>(type));
    params.append(new Parameter<const QString &>(mode));
    stubMethodEntered("initialize", params);
}

const MStyleDescription &MStylableBaseStub::completeStyleDescription() const
{
    stubMethodEntered("completeStyleDescription");
    return stubReturnValue<const MStyleDescription >("completeStyleDescription");
}

const MStyleDescription &MStylableBaseStub::styleDescription()
{
    stubMethodEntered("styleDescription");
    return stubReturnValue<const MStyleDescription >("styleDescription");
}



// 3. CREATE A STUB INSTANCE
MStylableBaseStub gDefaultMStylableBaseStub;
MStylableBaseStub *gMStylableBaseStub = &gDefaultMStylableBaseStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MStylableBase::~MStylableBase()
{
    gMStylableBaseStub->MStylableBaseDestructor();
}

void MStylableBase::updateStyle()
{
    gMStylableBaseStub->updateStyle();
}

const MStyleAttribute &MStylableBase::styleAttribute(int index) const
{
    return gMStylableBaseStub->styleAttribute(index);
}

bool MStylableBase::styleHasAttribute(int index) const
{
    return gMStylableBaseStub->styleHasAttribute(index);
}

void MStylableBase::initialize(const MStyleDescription &description, const QObject *object, const QString &type, const QString &mode)
{
    gMStylableBaseStub->initialize(description, object, type, mode);
}

const MStyleDescription &MStylableBase::completeStyleDescription() const
{
    return gMStylableBaseStub->completeStyleDescription();
}

const MStyleDescription &MStylableBase::styleDescription()
{
    return gMStylableBaseStub->styleDescription();
}


#endif
