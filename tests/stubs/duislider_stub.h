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

#ifndef DUISLIDER_STUB
#define DUISLIDER_STUB

#include <limits>

#include "duislider.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSliderStub : public StubBase
{
public:
    virtual void DuiSliderConstructor(DuiWidget *parent);
    virtual void DuiSliderDestructor();
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
void DuiSliderStub::DuiSliderConstructor(DuiWidget *parent)
{
    Q_UNUSED(parent);
}

void DuiSliderStub::DuiSliderDestructor()
{
}

int DuiSliderStub::minimum()
{
    stubMethodEntered("minimum");
    return stubReturnValue<int>("minimum");
}

int DuiSliderStub::maximum()
{
    stubMethodEntered("maximum");
    return stubReturnValue<int>("maximum");
}

int DuiSliderStub::value()
{
    stubMethodEntered("value");
    return stubReturnValue<int>("value");
}

void DuiSliderStub::setMinimum(int minimum)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(minimum));
    stubMethodEntered("setMinimum", params);
}

void DuiSliderStub::setMaximum(int maximum)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(maximum));
    stubMethodEntered("setMaximum", params);
}

void DuiSliderStub::setRange(int minimum, int maximum)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(minimum));
    params.append(new Parameter<int >(maximum));
    stubMethodEntered("setRange", params);
}

void DuiSliderStub::setValue(int value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(value));
    stubMethodEntered("setValue", params);
}

void DuiSliderStub::valueChanged(int newValue)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(newValue));
    stubMethodEntered("valueChanged", params);
}

bool DuiSliderStub::limitValue()
{
    stubMethodEntered("limitValue");
    return stubReturnValue<bool>("limitValue");
}

void DuiSliderStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

// 3. CREATE A STUB INSTANCE
DuiSliderStub gDefaultDuiSliderStub;
DuiSliderStub *gDuiSliderStub = &gDefaultDuiSliderStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSlider::DuiSlider(DuiWidget *parent)
{
    gDuiSliderStub->DuiSliderConstructor(parent);
}

DuiSlider::~DuiSlider()
{
    gDuiSliderStub->DuiSliderDestructor();
}

int DuiSlider::minimum()
{
    return gDuiSliderStub->minimum();
}

int DuiSlider::maximum()
{
    return gDuiSliderStub->maximum();
}

int DuiSlider::value()
{
    return gDuiSliderStub->value();
}

void DuiSlider::setMinimum(int minimum)
{
    gDuiSliderStub->setMinimum(minimum);
}

void DuiSlider::setMaximum(int maximum)
{
    gDuiSliderStub->setMaximum(maximum);
}

void DuiSlider::setRange(int minimum, int maximum)
{
    gDuiSliderStub->setRange(minimum, maximum);
}

void DuiSlider::setValue(int value)
{
    gDuiSliderStub->setValue(value);
}

bool DuiSlider::limitValue()
{
    return gDuiSliderStub->limitValue();
}

void DuiSlider::updateStyle()
{
    gDuiSliderStub->updateStyle();
}


#endif
