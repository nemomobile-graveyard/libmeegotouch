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

#ifndef MSLIDER_STUB
#define MSLIDER_STUB

#include <limits>

#include "mslider.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSliderStub : public StubBase
{
public:
    virtual void MSliderConstructor(MWidget *parent);
    virtual void MSliderDestructor();
    virtual int minimum();
    virtual int maximum();
    virtual int value();
    virtual void setMinimum(int minimum);
    virtual void setMaximum(int maximum);
    virtual void setRange(int minimum, int maximum);
    virtual void setValue(int value);
    virtual void valueChanged(int newValue);
    virtual bool limitValue();
    virtual void updateStyle();
};

// 2. IMPLEMENT STUB
void MSliderStub::MSliderConstructor(MWidget *parent)
{
    Q_UNUSED(parent);
}

void MSliderStub::MSliderDestructor()
{
}

int MSliderStub::minimum()
{
    stubMethodEntered("minimum");
    return stubReturnValue<int>("minimum");
}

int MSliderStub::maximum()
{
    stubMethodEntered("maximum");
    return stubReturnValue<int>("maximum");
}

int MSliderStub::value()
{
    stubMethodEntered("value");
    return stubReturnValue<int>("value");
}

void MSliderStub::setMinimum(int minimum)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(minimum));
    stubMethodEntered("setMinimum", params);
}

void MSliderStub::setMaximum(int maximum)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(maximum));
    stubMethodEntered("setMaximum", params);
}

void MSliderStub::setRange(int minimum, int maximum)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(minimum));
    params.append(new Parameter<int >(maximum));
    stubMethodEntered("setRange", params);
}

void MSliderStub::setValue(int value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(value));
    stubMethodEntered("setValue", params);
}

void MSliderStub::valueChanged(int newValue)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(newValue));
    stubMethodEntered("valueChanged", params);
}

bool MSliderStub::limitValue()
{
    stubMethodEntered("limitValue");
    return stubReturnValue<bool>("limitValue");
}

void MSliderStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

// 3. CREATE A STUB INSTANCE
MSliderStub gDefaultMSliderStub;
MSliderStub *gMSliderStub = &gDefaultMSliderStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSlider::MSlider(MWidget *parent)
{
    gMSliderStub->MSliderConstructor(parent);
}

MSlider::~MSlider()
{
    gMSliderStub->MSliderDestructor();
}

int MSlider::minimum()
{
    return gMSliderStub->minimum();
}

int MSlider::maximum()
{
    return gMSliderStub->maximum();
}

int MSlider::value()
{
    return gMSliderStub->value();
}

void MSlider::setMinimum(int minimum)
{
    gMSliderStub->setMinimum(minimum);
}

void MSlider::setMaximum(int maximum)
{
    gMSliderStub->setMaximum(maximum);
}

void MSlider::setRange(int minimum, int maximum)
{
    gMSliderStub->setRange(minimum, maximum);
}

void MSlider::setValue(int value)
{
    gMSliderStub->setValue(value);
}

bool MSlider::limitValue()
{
    return gMSliderStub->limitValue();
}

void MSlider::updateStyle()
{
    gMSliderStub->updateStyle();
}


#endif
