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

#ifndef DUISTYLABLEBASE_STUB
#define DUISTYLABLEBASE_STUB

#include "duistylablebase.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiStylableBaseStub : public StubBase
{
public:
    virtual void DuiStylableBaseConstructor();
    virtual void DuiStylableBaseDestructor();
    virtual void updateStyle();
//  void styleUpdated ;
    virtual const DuiStyleAttribute &styleAttribute(int index) const;
    virtual bool styleHasAttribute(int index) const;
    virtual void initialize(const DuiStyleDescription &description, const QObject *object, const QString &type, const QString &mode);
    virtual const DuiStyleDescription &completeStyleDescription() const;
    virtual const DuiStyleDescription &styleDescription();
    mutable DuiStyleAttribute *att;
    DuiStylableBasePrivate *d_ptr ;
};

// 2. IMPLEMENT STUB

void DuiStylableBaseStub::DuiStylableBaseConstructor()
{
    QColor col(0, 0, 0);
    att = new DuiStyleAttribute(col);

}
void DuiStylableBaseStub::DuiStylableBaseDestructor()
{
    delete att;
}
void DuiStylableBaseStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

const DuiStyleAttribute &DuiStylableBaseStub::styleAttribute(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("styleAttribute", params);
    return *att;
}

bool DuiStylableBaseStub::styleHasAttribute(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("styleHasAttribute", params);
    return stubReturnValue<bool>("styleHasAttribute");
}

void DuiStylableBaseStub::initialize(const DuiStyleDescription &description, const QObject *object, const QString &type, const QString &mode)
{
    Q_UNUSED(object);

    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiStyleDescription & >(description));
    params.append(new Parameter<const QObject *>(object));
    params.append(new Parameter<const QString &>(type));
    params.append(new Parameter<const QString &>(mode));
    stubMethodEntered("initialize", params);
}

const DuiStyleDescription &DuiStylableBaseStub::completeStyleDescription() const
{
    stubMethodEntered("completeStyleDescription");
    return stubReturnValue<const DuiStyleDescription >("completeStyleDescription");
}

const DuiStyleDescription &DuiStylableBaseStub::styleDescription()
{
    stubMethodEntered("styleDescription");
    return stubReturnValue<const DuiStyleDescription >("styleDescription");
}



// 3. CREATE A STUB INSTANCE
DuiStylableBaseStub gDefaultDuiStylableBaseStub;
DuiStylableBaseStub *gDuiStylableBaseStub = &gDefaultDuiStylableBaseStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiStylableBase::~DuiStylableBase()
{
    gDuiStylableBaseStub->DuiStylableBaseDestructor();
}

void DuiStylableBase::updateStyle()
{
    gDuiStylableBaseStub->updateStyle();
}

const DuiStyleAttribute &DuiStylableBase::styleAttribute(int index) const
{
    return gDuiStylableBaseStub->styleAttribute(index);
}

bool DuiStylableBase::styleHasAttribute(int index) const
{
    return gDuiStylableBaseStub->styleHasAttribute(index);
}

void DuiStylableBase::initialize(const DuiStyleDescription &description, const QObject *object, const QString &type, const QString &mode)
{
    gDuiStylableBaseStub->initialize(description, object, type, mode);
}

const DuiStyleDescription &DuiStylableBase::completeStyleDescription() const
{
    return gDuiStylableBaseStub->completeStyleDescription();
}

const DuiStyleDescription &DuiStylableBase::styleDescription()
{
    return gDuiStylableBaseStub->styleDescription();
}


#endif
