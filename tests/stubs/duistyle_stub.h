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

#ifndef DUISTYLE_STUB
#define DUISTYLE_STUB

#include <QStack>
#include "duistyle.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiStyleStub : public StubBase
{
public:

    virtual void DuiStyleConstructor(const QObject *object, const DuiStyleData *dataLandscape, const DuiStyleData *dataPortrait, const DuiStyleDescription &desc);
    virtual void DuiStyleDestructor();
    virtual const DuiStyleAttribute &operator[](int index) const;
    virtual const DuiStyleAttribute &attribute(int index) const;
    virtual void setOrientation(const Dui::Orientation &orientation);
    virtual const Dui::Orientation &orientation() const;
    virtual void setMode(const QString &mode);
    virtual const QString &mode() const;
    virtual void setAttribute(int index, DuiStyleAttribute *value);
    DuiStylePrivate *d_ptr ;
    mutable QStack<DuiStyleAttribute *> cleanupStack;
    virtual bool landscapePortraitStylesEqual() const;
    virtual const DuiStyleAttribute &attribute(Dui::Orientation orientation, int index) const;
    virtual bool hasAttribute(int index) const;
};

// 2. IMPLEMENT STUB
void DuiStyleStub::DuiStyleConstructor(const QObject *object, const DuiStyleData *dataLandscape, const DuiStyleData *dataPortrait, const DuiStyleDescription &desc)
{
    Q_UNUSED(object);
    Q_UNUSED(dataLandscape);
    Q_UNUSED(dataPortrait);
    Q_UNUSED(desc);

}
void DuiStyleStub::DuiStyleDestructor()
{
    // delete cleanupStack
    while (!cleanupStack.isEmpty())
        delete cleanupStack.pop();
}
const DuiStyleAttribute &DuiStyleStub::operator[](int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("operator[]", params);
    DuiStyleAttribute *myDSA = new DuiStyleAttribute(0);
    cleanupStack.push(myDSA);
    return *myDSA;
}

const DuiStyleAttribute &DuiStyleStub::attribute(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("attribute", params);
    DuiStyleAttribute *myDSA = new DuiStyleAttribute(0);
    cleanupStack.push(myDSA);

    return *myDSA;

    // Can't do the following because of the reference thing and the
    // fact that DuiStyleAttribute doesn't have a default constructor.
    // instead, we do the above, and if the user of the stub needs
    // special behaviour (almost always, I'd think) they can derive
    // a class from this stub class and override this function,
    // making it do what they want.
    // An alternative is to override the StubBase::stubReturnValue(Qstring
    // methodName) function so that, instead of using T() (the
    // default constructor), it uses a different one.
    // ...but this will do for now
    //
    // return stubReturnValue<const DuiStyleAttribute>(QString("attribute"));
}
const DuiStyleAttribute &DuiStyleStub::attribute(Dui::Orientation orientation, int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Dui::Orientation>(orientation));
    params.append(new Parameter<int >(index));
    stubMethodEntered("attribute", params);
    DuiStyleAttribute *myDSA = new DuiStyleAttribute(0);
    cleanupStack.push(myDSA);

    return *myDSA;

    // Can't do the following because of the reference thing and the
    // fact that DuiStyleAttribute doesn't have a default constructor.
    // instead, we do the above, and if the user of the stub needs
    // special behaviour (almost always, I'd think) they can derive
    // a class from this stub class and override this function,
    // making it do what they want.
    // An alternative is to override the StubBase::stubReturnValue(Qstring
    // methodName) function so that, instead of using T() (the
    // default constructor), it uses a different one.
    // ...but this will do for now
    //
    // return stubReturnValue<const DuiStyleAttribute>(QString("attribute"));
}

void DuiStyleStub::setOrientation(const Dui::Orientation &orientation)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Dui::Orientation & >(orientation));
    stubMethodEntered("setOrientation", params);
}

const Dui::Orientation &DuiStyleStub::orientation() const
{
    stubMethodEntered("orientation");
    return stubReturnValue<Dui::Orientation>("orientation");
}

void DuiStyleStub::setMode(const QString &mode)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(mode));
    stubMethodEntered("setMode", params);
}

const QString &DuiStyleStub::mode() const
{
    stubMethodEntered("mode");
    return stubReturnValue<QString>("mode");
}

void DuiStyleStub::setAttribute(int index, DuiStyleAttribute *value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    params.append(new Parameter<DuiStyleAttribute * >(value));
    stubMethodEntered("setAttribute", params);
}

bool DuiStyleStub::landscapePortraitStylesEqual() const
{
    return stubReturnValue<bool>("landscapePortraitStylesEqual");
}

bool DuiStyleStub::hasAttribute(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("hasAttribute", params);
    return stubReturnValue<bool>("hasAttribute");
}


// 3. CREATE A STUB INSTANCE
DuiStyleStub gDefaultDuiStyleStub;
DuiStyleStub *gDuiStyleStub = &gDefaultDuiStyleStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiStyle::DuiStyle(const QObject *object, const DuiStyleData *dataLandscape, const DuiStyleData *dataPortrait, const DuiStyleDescription &desc) : d_ptr(0)
{
    gDuiStyleStub->DuiStyleConstructor(object, dataLandscape, dataPortrait, desc);
}

DuiStyle::~DuiStyle()
{
    gDuiStyleStub->DuiStyleDestructor();
}

const DuiStyleAttribute &DuiStyle::operator[](int index) const
{
    return gDuiStyleStub->operator[](index);
}

const DuiStyleAttribute &DuiStyle::attribute(int index) const
{
    return gDuiStyleStub->attribute(index);
}

const DuiStyleAttribute &DuiStyle::attribute(Dui::Orientation orientation, int index) const
{
    return gDuiStyleStub->attribute(orientation, index);
}

void DuiStyle::setOrientation(const Dui::Orientation &orientation)
{
    gDuiStyleStub->setOrientation(orientation);
}

const Dui::Orientation &DuiStyle::orientation() const
{
    return gDuiStyleStub->orientation();
}

void DuiStyle::setMode(const QString &mode)
{
    gDuiStyleStub->setMode(mode);
}

const QString &DuiStyle::mode() const
{
    return gDuiStyleStub->mode();
}

void DuiStyle::setAttribute(int index, DuiStyleAttribute *value)
{
    gDuiStyleStub->setAttribute(index, value);
}

bool DuiStyle::landscapePortraitStylesEqual() const
{
    return gDuiStyleStub->landscapePortraitStylesEqual();
}

bool DuiStyle::hasAttribute(int index) const
{
    return gDuiStyleStub->hasAttribute(index);
}


#endif
