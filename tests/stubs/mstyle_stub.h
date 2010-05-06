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

#ifndef MSTYLE_STUB
#define MSTYLE_STUB

#include <QStack>
#include "mstyle.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MStyleStub : public StubBase
{
public:

    virtual void MStyleConstructor(const QObject *object, const MStyleData *dataLandscape, const MStyleData *dataPortrait, const MStyleDescription &desc);
    virtual void MStyleDestructor();
    virtual const MStyleAttribute &operator[](int index) const;
    virtual const MStyleAttribute &attribute(int index) const;
    virtual void setOrientation(const M::Orientation &orientation);
    virtual const M::Orientation &orientation() const;
    virtual void setMode(const QString &mode);
    virtual const QString &mode() const;
    virtual void setAttribute(int index, MStyleAttribute *value);
    MStylePrivate *d_ptr ;
    mutable QStack<MStyleAttribute *> cleanupStack;
    virtual bool landscapePortraitStylesEqual() const;
    virtual const MStyleAttribute &attribute(M::Orientation orientation, int index) const;
    virtual bool hasAttribute(int index) const;
};

// 2. IMPLEMENT STUB
void MStyleStub::MStyleConstructor(const QObject *object, const MStyleData *dataLandscape, const MStyleData *dataPortrait, const MStyleDescription &desc)
{
    Q_UNUSED(object);
    Q_UNUSED(dataLandscape);
    Q_UNUSED(dataPortrait);
    Q_UNUSED(desc);

}
void MStyleStub::MStyleDestructor()
{
    // delete cleanupStack
    while (!cleanupStack.isEmpty())
        delete cleanupStack.pop();
}
const MStyleAttribute &MStyleStub::operator[](int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("operator[]", params);
    MStyleAttribute *myDSA = new MStyleAttribute(0);
    cleanupStack.push(myDSA);
    return *myDSA;
}

const MStyleAttribute &MStyleStub::attribute(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("attribute", params);
    MStyleAttribute *myDSA = new MStyleAttribute(0);
    cleanupStack.push(myDSA);

    return *myDSA;

    // Can't do the following because of the reference thing and the
    // fact that MStyleAttribute doesn't have a default constructor.
    // instead, we do the above, and if the user of the stub needs
    // special behaviour (almost always, I'd think) they can derive
    // a class from this stub class and override this function,
    // making it do what they want.
    // An alternative is to override the StubBase::stubReturnValue(Qstring
    // methodName) function so that, instead of using T() (the
    // default constructor), it uses a different one.
    // ...but this will do for now
    //
    // return stubReturnValue<const MStyleAttribute>(QString("attribute"));
}
const MStyleAttribute &MStyleStub::attribute(M::Orientation orientation, int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<M::Orientation>(orientation));
    params.append(new Parameter<int >(index));
    stubMethodEntered("attribute", params);
    MStyleAttribute *myDSA = new MStyleAttribute(0);
    cleanupStack.push(myDSA);

    return *myDSA;

    // Can't do the following because of the reference thing and the
    // fact that MStyleAttribute doesn't have a default constructor.
    // instead, we do the above, and if the user of the stub needs
    // special behaviour (almost always, I'd think) they can derive
    // a class from this stub class and override this function,
    // making it do what they want.
    // An alternative is to override the StubBase::stubReturnValue(Qstring
    // methodName) function so that, instead of using T() (the
    // default constructor), it uses a different one.
    // ...but this will do for now
    //
    // return stubReturnValue<const MStyleAttribute>(QString("attribute"));
}

void MStyleStub::setOrientation(const M::Orientation &orientation)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const M::Orientation & >(orientation));
    stubMethodEntered("setOrientation", params);
}

const M::Orientation &MStyleStub::orientation() const
{
    stubMethodEntered("orientation");
    return stubReturnValue<M::Orientation>("orientation");
}

void MStyleStub::setMode(const QString &mode)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(mode));
    stubMethodEntered("setMode", params);
}

const QString &MStyleStub::mode() const
{
    stubMethodEntered("mode");
    return stubReturnValue<QString>("mode");
}

void MStyleStub::setAttribute(int index, MStyleAttribute *value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    params.append(new Parameter<MStyleAttribute * >(value));
    stubMethodEntered("setAttribute", params);
}

bool MStyleStub::landscapePortraitStylesEqual() const
{
    return stubReturnValue<bool>("landscapePortraitStylesEqual");
}

bool MStyleStub::hasAttribute(int index) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("hasAttribute", params);
    return stubReturnValue<bool>("hasAttribute");
}


// 3. CREATE A STUB INSTANCE
MStyleStub gDefaultMStyleStub;
MStyleStub *gMStyleStub = &gDefaultMStyleStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MStyle::MStyle(const QObject *object, const MStyleData *dataLandscape, const MStyleData *dataPortrait, const MStyleDescription &desc) : d_ptr(0)
{
    gMStyleStub->MStyleConstructor(object, dataLandscape, dataPortrait, desc);
}

MStyle::~MStyle()
{
    gMStyleStub->MStyleDestructor();
}

const MStyleAttribute &MStyle::operator[](int index) const
{
    return gMStyleStub->operator[](index);
}

const MStyleAttribute &MStyle::attribute(int index) const
{
    return gMStyleStub->attribute(index);
}

const MStyleAttribute &MStyle::attribute(M::Orientation orientation, int index) const
{
    return gMStyleStub->attribute(orientation, index);
}

void MStyle::setOrientation(const M::Orientation &orientation)
{
    gMStyleStub->setOrientation(orientation);
}

const M::Orientation &MStyle::orientation() const
{
    return gMStyleStub->orientation();
}

void MStyle::setMode(const QString &mode)
{
    gMStyleStub->setMode(mode);
}

const QString &MStyle::mode() const
{
    return gMStyleStub->mode();
}

void MStyle::setAttribute(int index, MStyleAttribute *value)
{
    gMStyleStub->setAttribute(index, value);
}

bool MStyle::landscapePortraitStylesEqual() const
{
    return gMStyleStub->landscapePortraitStylesEqual();
}

bool MStyle::hasAttribute(int index) const
{
    return gMStyleStub->hasAttribute(index);
}


#endif
